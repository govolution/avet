# v0.1
#
# This Dockerfile encapsulates the Metasploit Framework and Avet.
#
# Also included:
# hasherezade/pe_to_shellcode
# Mr-Un1k0d3r/DKMC
# gentilkiwi/mimikatz
#
# To build this run:
# $ sudo docker build -t avet .
#
# To start this:
# $ sudo docker run -it --net=host --env="DISPLAY" --volume="$HOME/.Xauthority:/root/.Xauthority:rw" -v $(pwd):/tools/avet/output avet /bin/bash
# (alias this)
#
# This mounts the Current Working directory as a Docker Volume and attach it to /avet/output.
# To be save, choose an empty directory to prevent overriding files.
#
# Right now, the only option to configure the build scripts inside the Container
# is using avet.py or with vim/nano/gedit in /avet/build
# You can also install other editors if you like
#


FROM debian:buster

# Preparation
ARG DEBIAN_FRONTEND=noninteractive

RUN dpkg --add-architecture i386 && apt-get update && apt-get install -y \
	wget \
	git \
	unzip \
	apt-utils \
	gnupg2 \
	vim \
	nano \
	gedit \
	wine \
	wine32 \
	python \
	python3 \
	mingw-w64 \
	&& rm -rf /var/lib/apt/lists/*


# For metasploit
RUN echo 'deb http://apt.metasploit.com/ buster main' > /etc/apt/sources.list.d/metasploit-framework.list
RUN wget -O - http://apt.metasploit.com/metasploit-framework.gpg.key | apt-key add -

RUN apt-get update && apt-get install -y \
	metasploit-framework \
	&& rm -rf /var/lib/apt/lists/*


# For avet
WORKDIR /tools/avet
COPY . .


# For pe2shc
WORKDIR /tools/pe_to_shellcode
RUN wget https://github.com/hasherezade/pe_to_shellcode/releases/download/v0.7/pe2shc.exe


# For mimikatz
WORKDIR /tools/mimikatz
RUN wget https://github.com/gentilkiwi/mimikatz/releases/download/2.2.0-20200308/mimikatz_trunk.zip \
	&& unzip mimikatz_trunk.zip \
	&& cp x64/mimikatz.exe /tools/avet/input


# For dkmc
WORKDIR /tools
RUN git clone https://github.com/Mr-Un1k0d3r/DKMC.git


# configure wine during build
RUN wine cmd


WORKDIR /tools/avet
