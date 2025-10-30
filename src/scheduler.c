#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task_scheduler.h"
/* File to persist tasks. Must be writable by Apache user (www-data) inside container. */
#define DATA_FILE "/var/www/html/tasks.db"

/* helper: URL decode */
static void url_decode_inplace(char *s) {
    char *d = s;
    char *p = s;
    while (*p) {
        if (*p == '+') { *d++ = ' '; p++; }
        else if (*p == '%' && isxdigit((unsigned char) p[1]) && isxdigit((unsigned char) p[2])) {
            char hex[3] = { p[1], p[2], 0 };
            *d++ = (char) strtol(hex, NULL, 16);
            p += 3;
        } else { *d++ = *p++; }
    }
    *d = 0;
}

/* helper: extract parameter value by key (from query string) */
static int get_int_param(const char *qs, const char *key, int *out) {
    char search[64];
    snprintf(search, sizeof(search), "%s=", key);
    char *p = strstr(qs, search);
    if (!p) return 0;
    p += strlen(search);
    *out = atoi(p);
    return 1;
}

static int get_str_param(const char *qs, const char *key, char *out, int outlen) {
    char search[64];
    snprintf(search, sizeof(search), "%s=", key);
    char *p = strstr(qs, search);
    if (!p) return 0;
    p += strlen(search);
    /* copy until & or end */
    int i = 0;
    while (*p && *p != '&' && i < outlen - 1) out[i++] = *p++;
    out[i] = '\0';
    url_decode_inplace(out);
    return 1;
}

static void print_json_header() {
    printf("Content-Type: application/json\r\n\r\n");
}

int main(void) {
    print_json_header();

    Scheduler *s = createScheduler(MAX_TASKS);
    if (!s) {
        printf("{\"error\":\"alloc_failed\"}");
        return 0;
    }
    /* load persisted tasks (if any) */
    loadSchedulerFromFile(s, DATA_FILE);

    char *method = getenv("REQUEST_METHOD");
    if (!method) {
        printf("{\"error\":\"no_method\"}");
        freeScheduler(s);
        return 0;
    }

    char *query = NULL;
    char buf[4096];

    if (strcmp(method, "GET") == 0) {
        query = getenv("QUERY_STRING");
        if (query == NULL) query = "";
    } else if (strcmp(method, "POST") == 0) {
        char *len_str = getenv("CONTENT_LENGTH");
        int len = len_str ? atoi(len_str) : 0;
        if (len > 0 && len < (int)sizeof(buf)) {
            if (fread(buf, 1, len, stdin) != (size_t)len) {
                /* partial read - still try */
            }
            buf[len] = '\0';
            query = buf;
        } else {
            query = "";
        }
    } else {
        query = "";
    }

    /* action parsing */
    char action[32] = {0};
    if (!get_str_param(query, "action", action, sizeof(action))) {
        /* maybe 'action=...' at start without & - try direct prefix */
        if (strncmp(query, "action=", 7) == 0) {
            char *p = strchr(query, '=');
            if (p) strncpy(action, p+1, sizeof(action)-1);
            char *amp = strchr(action, '&');
            if (amp) *amp = 0;
        }
    }

    if (strcmp(action, "add") == 0) {
        int id = 0, time = 0;
        char details[DETAILS_SIZE] = {0};
        get_int_param(query, "id", &id);
        get_int_param(query, "time", &time);
        get_str_param(query, "details", details, sizeof(details));
        addTask(s, id, time, details);
        saveSchedulerToFile(s, DATA_FILE);
        printf("{\"status\":\"added\"}");
    }
    else if (strcmp(action, "serve") == 0) {
        int id = 0;
        get_int_param(query, "id", &id);
        serveTask(s, id);
        saveSchedulerToFile(s, DATA_FILE);
        printf("{\"status\":\"served\"}");
    }
    else if (strcmp(action, "cancel") == 0) {
        int id = 0;
        get_int_param(query, "id", &id);
        cancelTask(s, id);
        saveSchedulerToFile(s, DATA_FILE);
        printf("{\"status\":\"cancelled\"}");
    }
    else if (strcmp(action, "update") == 0) {
        int id = 0, newtime = 0;
        get_int_param(query, "id", &id);
        get_int_param(query, "newtime", &newtime);
        updateSubmissionTime(s, id, newtime);
        saveSchedulerToFile(s, DATA_FILE);
        printf("{\"status\":\"updated\"}");
    }
    else if (strcmp(action, "view") == 0) {
        /* output tasks as JSON array - return current heap order (not necessarily sorted by time),
           but since heap root is earliest, clients can rely on times if they want ordering */
        printf("{\"tasks\":[");
        for (int i = 0; i < s->size; ++i) {
            Task *t = s->heap[i];
            /* escape double quotes in details for JSON safety */
            char safe_details[DETAILS_SIZE * 2];
            int di = 0;
            for (int j = 0; t->details[j] && di < (int)sizeof(safe_details)-1; ++j) {
                char c = t->details[j];
                if (c == '"') { safe_details[di++] = '\\'; safe_details[di++] = '"'; }
                else if (c == '\\') { safe_details[di++] = '\\'; safe_details[di++] = '\\'; }
                else safe_details[di++] = c;
            }
            safe_details[di] = '\0';

            printf("{\"id\":%d,\"time\":%d,\"done\":%d,\"details\":\"%s\"}%s",
                   t->taskId, t->submissionTime, t->completed, safe_details,
                   (i == s->size - 1 ? "" : ","));
        }
        printf("]}");
    }
    else {
        printf("{\"error\":\"unknown_action\"}");
    }

    freeScheduler(s);
    return 0;
}
