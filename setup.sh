#! /bin/bash
sudo apt-get update -y
sudo apt-get install -y libgcrypt-dev
dpkg -L libgcrypt20-dev
# Allow to find where the package is installed
