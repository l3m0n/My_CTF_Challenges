#!/bin/sh
# a2enmod rewrite
mv /tmp/tmp/conf /etc/apache2/apache2.conf
/etc/init.d/apache2 start
rm -f /var/www/html/index.html
find /var/lib/mysql -type f -exec touch {} \; && /etc/init.d/mysql start
chown -R mysql:mysql /var/run/mysqld/
mv /tmp/tmp/my.cnf /etc/mysql/my.cnf
/etc/init.d/mysql restart
unzip /var/www/html/www.zip -d /var/www/html
rm -rf /var/www/html/__MACOSX
rm -rf /var/www/html/www.zip
rm -rf /var/www/html/.DS_Store
chmod 755 -R /var/www/html/
chmod 777 -R /var/www/html/uploads/
chattr -R +a /var/www/html/uploads/
mysql -e 'insert into mysql.user(host,user,password) values("localhost","dedefun",password("N6e4puVVxFP9Qs7z"));'
mysql -e 'create database dedefun;'
mysql -e 'FLUSH PRIVILEGES;'
mysql -e 'GRANT all privileges ON `dedefun`.* TO "dedefun"@"localhost" IDENTIFIED BY "N6e4puVVxFP9Qs7z" WITH GRANT OPTION;'
mysql -e "grant all privileges on *.* to 'root'@'%' identified by 'p848ezY4b7Z39W7R';"
mysql -e "grant all privileges on *.* to 'root'@'localhost' identified by 'p848ezY4b7Z39W7R';"
mysql -e "flush privileges;"
mysql -h localhost -u root -pp848ezY4b7Z39W7R dedefun < /tmp/tmp/dedefun.sql
cp /tmp/tmp/flag_a4ga3QJsZ5DTCw8v /tmp/
chmod 555 /tmp/tmp/flag_a4ga3QJsZ5DTCw8v
rm -rf /tmp/tmp/
/bin/bash