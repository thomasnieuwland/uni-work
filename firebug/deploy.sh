#!/bin/bash
# Setup file permissions (only works on meme-machine)
echo "* Setting file permissions"
sudo chgrp -R csci222 .
sudo chmod -R g+w .

# Compile CSS
echo "* Compiling CSS"
sass web/style.scss web/style.css

# Compile CGI (see makefile)
echo "* Compiling CGI scripts"
make clean
make all
