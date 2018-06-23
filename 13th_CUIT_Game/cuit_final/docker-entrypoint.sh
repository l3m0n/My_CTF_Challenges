#!/bin/bash
sleep 20s
echo "nameserver 8.8.8.8" >> /etc/resolv.conf
echo "create database $MYSQL_DB_NAME" | mysql --host mysql --user root -p$MYSQL_ROOT_PASS
mysql --host mysql --user root -p$MYSQL_ROOT_PASS $MYSQL_DB_NAME < /sql.sql
ip=$(awk 'END{print $1}' /etc/hosts)
echo "delete from mysql.user where user='root' and host='$ip';GRANT ALL PRIVILEGES ON *.* TO 'root'@'$ip' IDENTIFIED BY '$MYSQL_ROOT_PASS' WITH GRANT OPTION;    flush privileges;" | mysql --host mysql --user root -p$MYSQL_ROOT_PASS
echo "CREATE USER '$MYSQL_DB_USER'@'$ip' IDENTIFIED BY '$MYSQL_DB_PASS';grant all privileges on $MYSQL_DB_NAME.* to $MYSQL_DB_USER@$ip identified by '$MYSQL_DB_PASS';flush privileges;" | mysql --host mysql --user root -p$MYSQL_ROOT_PASS
apache2-foreground
