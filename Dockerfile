FROM scratch

COPY httpd /

EXPOSE 80

CMD ["/httpd", "80"]