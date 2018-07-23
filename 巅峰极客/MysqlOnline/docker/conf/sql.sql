use xss_bot;
create table `log`(
     id INT(11) NOT NULL AUTO_INCREMENT,
     content varchar(255),
     time varchar(255),
     ip varchar(255),
     status varchar(255),
     PRIMARY KEY(ID)
);