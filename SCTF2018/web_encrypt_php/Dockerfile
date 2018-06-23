FROM php:5.6.36-apache

MAINTAINER Virink <virink@outlook.com>
LABEL CHALLENGE="SCTF2018 BabySyc - Simple PHP Web"
# MD5 ("SCTF2018 BabySyc - Simple PHP Web") = a17406a0056c753f920cf539cb79219c

ADD src /tmp/

RUN apt-get update \
    && apt-get install --no-install-recommends -y zlib1g-dev \
    \
    && mv /tmp/flag /flag_a17406a0056c753f920cf539cb79219c \
    && mv /tmp/php.ini /usr/local/etc/php/php.ini \
    && mv /tmp/docker-php-entrypoint /docker-php-entrypoint \
    && chmod +x /docker-php-entrypoint \
    \
    && rm -rf /var/www/html \
    && mv /tmp/www /var/www/html \
    \
    && docker-php-source extract \
    && cd /tmp/encrypt_php \
    && phpize \
    && ./configure --with-php-config=/usr/local/bin/php-config \
    && make \
    && strip ./modules/encrypt_php.so \
    && make install \
    && docker-php-ext-enable encrypt_php \
    && cd tools \
    && make \
    && ./screw /var/www/html \
    && docker-php-source delete \
    && rm -rf /tmp/* \
    && rm -rf /var/lib/apt/lists/*

# CMD ["/docker-php-entrypoint"]