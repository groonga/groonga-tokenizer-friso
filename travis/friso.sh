#!/bin/bash

set -e

WORK_DIR=$(pwd)

git clone --depth 1 https://github.com/lionsoul2014/friso.git
cd friso
sed -i -e 's,./vendors/dict/UTF-8/,/usr/local/share/friso/dict/UTF-8/,' friso.ini
sudo cp friso.ini /usr/local/etc/
sudo mkdir -p /usr/local/share/friso
sudo cp -r ./vendors/dict /usr/local/share/friso
cd src && make && sudo make install
cd $WORK_DIR
