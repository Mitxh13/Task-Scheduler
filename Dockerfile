# ====== Base image ======
FROM debian:bookworm

# ====== Install packages ======
RUN apt-get update && \
    apt-get install -y apache2 gcc make libc6-dev && \
    a2enmod cgi && \
    mkdir -p /var/www/html/web /usr/lib/cgi-bin && \
    rm -rf /var/lib/apt/lists/*

# ====== Copy project files ======
WORKDIR /app
COPY src/ src/
COPY main.c .
COPY web/ /var/www/html/web/
COPY README.md .

# ====== Compile CGI backend ======
RUN gcc src/task_scheduler.c src/scheduler.c -o /usr/lib/cgi-bin/scheduler.cgi -lm

# ====== Permissions ======
RUN chmod +x /usr/lib/cgi-bin/scheduler.cgi

# ====== Apache configuration ======
RUN echo 'ScriptAlias /cgi-bin/ /usr/lib/cgi-bin/' >> /etc/apache2/conf-enabled/cgi.conf && \
    echo '<Directory "/usr/lib/cgi-bin">\n\
    AllowOverride None\n\
    Options +ExecCGI -MultiViews +SymLinksIfOwnerMatch\n\
    Require all granted\n\
</Directory>' > /etc/apache2/conf-available/serve-cgi-bin.conf

# ====== Expose port and run Apache ======
EXPOSE 80
CMD ["/usr/sbin/apache2ctl", "-D", "FOREGROUND"]
