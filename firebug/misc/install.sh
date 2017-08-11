#!/bin/bash
sudo -s
echo "** Installing LAMP"
apt-get update
apt install apache2
apt-get install mysql-server -y
mysql_secure_installation
apt-get install php libapache2-mod-php php-mcrypt php-mysql -y
systemctl restart apache2
echo "** Installing CPPCONN"
apt-get install libmysqlcppconn-dev
echo "** Installing CGICC"
wget ftp://ftp.gnu.org/gnu/cgicc/cgicc-3.0.tar.gz
tar xvf cgicc-3.0.tar.gz
mv cgicc-3.0 /usr/include
apt-get install build-essential libcgicc5v5 libcgicc5-dev libcgicc-doc -y
echo "** Installing SASS"
apt-get install ruby-sass
echo "** DONE!"
exit
