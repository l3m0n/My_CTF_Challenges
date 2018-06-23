FROM je2ryw/redis-centos67:v3.0.5

#RUN mv /etc/yum.repos.d/CentOS-Base.repo /etc/yum.repos.d/CentOS-Base.repo.backup
#ADD CentOS6-Base-163.repo /etc/yum.repos.d/
#RUN yum clean all
#RUN yum makecache

# Install Crontab Programe
RUN yum -y install crontabs
RUN sed -i 's/required/sufficient/' /etc/pam.d/crond

# Install Redis Service
#RUN yum -y update; yum clean all
#RUN yum -y install epel-release; yum clean all
#RUN yum -y install redis; yum clean all

# Change port
# RUN sed -i 's/6379/56379/' /etc/redis.conf
# RUN sed -i 's/127.0.0.1/0.0.0.0/' /etc/redis.conf

# Add flag
ADD flag_in_here /root/

EXPOSE 6379

COPY docker-entrypoint.sh /
RUN chmod +x /docker-entrypoint.sh
ENTRYPOINT /docker-entrypoint.sh


