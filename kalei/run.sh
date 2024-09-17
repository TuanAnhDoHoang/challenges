#!/bin/sh

docker build . -t kalei
docker run -it -p 1337:1337 kalei 