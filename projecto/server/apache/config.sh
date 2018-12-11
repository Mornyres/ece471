# todo: maybe make this work for any given apache path (export env. variable? so that other people can run this)

sudo cp global.conf /etc/apache2/conf-enabled/bloop.conf
sudo a2enconf bloop

sudo systemctl stop apache2
sudo systemctl start apache2
sudo systemctl status apache2

sudo service apache2 reload
