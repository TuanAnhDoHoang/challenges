FROM ubuntu:22.04

MAINTAINER anonymous
RUN apt-get update

RUN useradd -m kalei

RUN apt-get update --fix-missing -y

RUN apt-get install -y apt-utils software-properties-common

RUN apt-get install -y build-essential \
        socat \
        netcat 

WORKDIR /home/kalei

ADD ./flag /home/kalei
ADD ./kalei /home/kalei

ENTRYPOINT ["sh", "-c", "exec socat -s TCP-LISTEN:1337,reuseaddr,fork EXEC:/home/kalei/kalei,stderr"]

EXPOSE 1337
