# ====== Base image ======
FROM debian:bookworm

# ====== Install packages ======
RUN apt-get update && apt-get install -y \
    gcc \
    make \
    lighttpd \
    libc6-dev \
    && rm -rf /var/lib/apt/lists/*

# ====== Set working directory ======
WORKDIR /app

# ====== Copy project files ======
COPY . .

# ====== Compile backend (CGI) ======
RUN gcc src/task_scheduler.c src/scheduler.c -o /usr/lib/cgi-bin/scheduler.cgi -lm
RUN chmod +x /usr/lib/cgi-bin/scheduler.cgi

# ====== Copy frontend files ======
RUN mkdir -p /var/www/html/web
COPY web /var/www/html/web

# ====== Enable CGI module ======
RUN lighty-enable-mod cgi

# ====== Configure Lighttpd ======
RUN echo 'server.document-root = "/var/www/html/web"\n\
server.modules += ( "mod_cgi" )\n\
cgi.assign = ( ".cgi" => "" )\n\
index-file.names = ( "index.html" )\n\
alias.url = ( "/cgi-bin/" => "/usr/lib/cgi-bin/" )\n\
dir-listing.activate = "enable"\n\
server.port = 80' > /etc/lighttpd/lighttpd.conf

EXPOSE 80

# ====== Start server ======
CMD ["lighttpd", "-D", "-f", "/etc/lighttpd/lighttpd.conf"]
