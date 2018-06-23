FROM php:apache

RUN rm -rf /var/www/html/*
# ADD soure/ /var/www/html/

ADD sources.list /etc/apt/sources.list

RUN apt-get update && apt-get install -y gcc wget mariadb-client

RUN wget http://xmlsoft.org/sources/libxml2-2.9.4.tar.gz -O /home/libxml2-2.9.4.tar.gz

RUN cd /home/ && tar -zxvf libxml2-2.9.4.tar.gz && rm -f /home/libxml2-2.9.4.tar.gz

RUN cd /home/libxml2-2.9.4 && ./configure && make && make install && make clean && cd / && rm -rf /home/libxml2-2.9.4

RUN wget https://launchpadlibrarian.net/36710455/curl_7.19.7.orig.tar.gz -O /home/curl_7.19.7.tar.gz

RUN cd /home/ && tar -zxvf curl_7.19.7.tar.gz && rm -f /home/curl_7.19.7.tar.gz

RUN cd /home/curl-7.19.7 && ./configure && make && make install && make clean && cd / && rm -rf /home/curl-7.19.7

# ReInstall php
RUN set -xe \
	&& buildDeps=" \
		$PHP_EXTRA_BUILD_DEPS \
		libcurl4-openssl-dev \
		libedit-dev \
		libsqlite3-dev \
		libssl-dev \
		libxml2-dev \
	" \
	&& apt-get install -y $buildDeps --no-install-recommends && rm -rf /var/lib/apt/lists/* \
	\
	&& docker-php-source extract \
	&& cd /usr/src/php \
	&& ./configure \
		--with-config-file-path="$PHP_INI_DIR" \
		--with-config-file-scan-dir="$PHP_INI_DIR/conf.d" \
		\
		--disable-cgi \
		\
# --enable-ftp is included here because ftp_ssl_connect() needs ftp to be compiled statically (see https://github.com/docker-library/php/issues/236)
		--enable-ftp \
# --enable-mbstring is included here because otherwise there's no way to get pecl to use it properly (see https://github.com/docker-library/php/issues/195)
		--enable-mbstring \
# --enable-mysqlnd is included here because it's harder to compile after the fact than extensions are (since it's a plugin for several extensions, not an extension in itself)
		--enable-mysqlnd \
		\
		--with-curl \
		--with-libedit \
		--with-openssl \
		--with-zlib \
		\
		$PHP_EXTRA_CONFIGURE_ARGS \
	&& make -j "$(nproc)" \
	&& make install \
	&& { find /usr/local/bin /usr/local/sbin -type f -executable -exec strip --strip-all '{}' + || true; } \
	&& make clean \
	&& docker-php-source delete \
	\
	&& apt-get purge -y --auto-remove -o APT::AutoRemove::RecommendsImportant=false $buildDeps wget

# Install php - redis ext
# http://stackoverflow.com/questions/31369867/how-to-install-php-redis-extension-using-the-official-php-docker-image-approach
RUN mkdir -p /usr/src/php/ext/redis

ADD phpredis-3.1.2/ /usr/src/php/ext/redis

RUN echo 'redis' >> /usr/src/php-available-exts \
    && docker-php-ext-install redis

# Install php - mysqli ext
RUN docker-php-ext-install mysqli
ADD sql.sql /

# Install mysql client
# RUN DEBIAN_FRONTEND=noninteractive apt-get -y install mariadb-client

# Open htaccess
RUN cp /etc/apache2/mods-available/rewrite.load /etc/apache2/mods-enabled/
RUN sed -i 's/AllowOverride None/AllowOverride All/' /etc/apache2/apache2.conf

# Add some php - safe config

# Open port
EXPOSE 80

RUN chown www-data:www-data /var/www/html -R
RUN chmod -R 755 /var/www/html

#COPY docker-entrypoint.sh /
#RUN chmod +x /docker-entrypoint.sh

#ENTRYPOINT /docker-entrypoint.sh
