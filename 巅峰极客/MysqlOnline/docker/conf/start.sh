#!/bin/sh
a2enmod rewrite
mv /tmp/conf /etc/apache2/apache2.conf
/etc/init.d/apache2 start
rm -f /var/www/html/index.html
find /var/lib/mysql -type f -exec touch {} \; && /etc/init.d/mysql start
chown -R mysql:mysql /var/run/mysqld/
mv /tmp/my.cnf /etc/mysql/my.cnf
/etc/init.d/mysql restart
unzip /var/www/html/www.zip -d /var/www/html
rm -rf /var/www/html/__MACOSX
rm -rf /var/www/html/www.zip
rm -rf /var/www/html/.DS_Store
chmod 755 -R /var/www/html
mysql -e 'insert into mysql.user(host,user,password) values("localhost","sandbox",password("QK5VfPFzGeK34Ncp"));'
mysql -e 'insert into mysql.user(host,user,password) values("localhost","xss_bot",password("bT8gGGe5Fuq5v4rx"));'
mysql -e 'create database sandbox;'
mysql -e 'create database xss_bot;'
mysql -e 'FLUSH PRIVILEGES;'
mysql -e 'GRANT all privileges ON `sandbox`.* TO "sandbox"@"localhost" IDENTIFIED BY "QK5VfPFzGeK34Ncp" WITH GRANT OPTION;'
mysql -e 'GRANT all privileges ON `xss_bot`.* TO "xss_bot"@"localhost" IDENTIFIED BY "bT8gGGe5Fuq5v4rx" WITH GRANT OPTION;'
mysql -e "grant all privileges on *.* to 'root'@'%' identified by '1q2w3e4r5t6y';"
mysql -e "grant all privileges on *.* to 'root'@'localhost' identified by '1q2w3e4r5t6y';"
mysql -uroot -p1q2w3e4r5t6y -e "use mysql;"
mysql -uroot -p1q2w3e4r5t6y -e "update user set host = '%' where user ='root';"
mysql -uroot -p1q2w3e4r5t6y -e "flush privileges;"
mysql -uroot -p1q2w3e4r5t6y < /tmp/sql.sql
rm -f /tmp/sql.sql

# xss bot
unzip /root/chromedriver_linux64.zip -d /root/
rm -f /root/chromedriver_linux64.zip
rm -f /root/google-chrome-stable_current_amd64.deb
rm -f /root/selenium-3.13.0-py2.py3-none-any.whl
python /root/xss_bot.py

/bin/bash