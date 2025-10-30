# ====== Base image ======
FROM debian:bookworm

# ====== Install packages ======
RUN apt-get update && apt-get install -y \
    gcc \
    make \
    lighttpd \
    libc6-dev \
    && rm -rf /var/lib/apt/lists/*

# ====== Copy project files ======
WORKDIR /app
COPY . .

# ====== Compile backend ======
RUN gcc src/task_scheduler.c src/scheduler.c -o /usr/lib/cgi-bin/scheduler.cgi -lm

# ====== Permissions ======
RUN chmod +x /usr/lib/cgi-bin/scheduler.cgi

# ====== Copy web files ======
RUN mkdir -p /var/www/html/web
COPY web /var/www/html/web

# ====== Configure Lighttpd ======
RUN echo 'server.document-root = "/var/www/html/web"\n\
cgi.assign = ( ".cgi" => "" )\n\
server.modules += ( "mod_cgi" )\n\
index-file.names = ( "index.html" )' > /etc/lighttpd/lighttpd.conf

EXPOSE 80

# ====== Start server ======
CMD ["lighttpd", "-D", "-f", "/etc/lighttpd/lighttpd.conf"]
