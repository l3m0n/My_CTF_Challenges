FROM ubuntu:14.04

RUN apt-get update
RUN apt-get install -y apache2
RUN apt-get install -y zip
RUN apt-get install -y php5 libapache2-mod-php5
RUN apt-get install -y php5-mysql php5-curl php5-gd php5-idn php-pear php5-imagick php5-imap php5-mcrypt php5-memcache php5-ming php5-ps php5-pspell php5-recode php5-snmp php5-sqlite php5-tidy php5-xmlrpc php5-xsl
RUN apt-get install -y mysql-server
RUN apt-get install -y mysql-client
RUN apt-get install -y libxss1 libappindicator1 libindicator7

COPY src/www.zip /var/www/html/
COPY conf/ /tmp/
COPY conf/sql.sql /tmp/
COPY google-chrome-stable_current_amd64.deb /root/
COPY chromedriver_linux64.zip /root/
COPY xss_bot.py /root/
COPY conf/start.sh /start.sh
RUN chmod +x /start.sh

WORKDIR /root/
RUN apt-get install -y fonts-liberation libasound2 libnspr4 libnss3 libx11-xcb1 wget xdg-utils
RUN dpkg -i google-chrome-stable_current_amd64.deb
RUN apt-get -y install -f
RUN apt-get install -y python-pip python-dev build-essential 
RUN pip install requests
COPY conf/selenium-3.13.0-py2.py3-none-any.whl /root/
RUN pip install selenium-3.13.0-py2.py3-none-any.whl

EXPOSE 80
CMD ["/start.sh"]