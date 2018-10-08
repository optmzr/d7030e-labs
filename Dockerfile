FROM ubuntu:latest

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update

RUN apt-get install -y \
	build-essential \
	gcc g++ python python-dev \
	mercurial python-setuptools git \
	qt5-default \
	qtbase5-dev \
	gir1.2-goocanvas-2.0 python-gi python-gi-cairo python-pygraphviz \
	python3-gi python3-gi-cairo python3-pygraphviz gir1.2-gtk-3.0 ipython \
	ipython3 \
	openmpi-bin openmpi-common openmpi-doc libopenmpi-dev \
	autoconf cvs bzr unrar \
	gdb valgrind \
	uncrustify \
	doxygen graphviz imagemagick \
	texlive texlive-extra-utils texlive-latex-extra texlive-font-utils \
	texlive-lang-portuguese dvipng \
	python-sphinx dia \
	gsl-bin libgslcblas0 libgsl23 \
	flex bison libfl-dev \
	tcpdump \
	sqlite sqlite3 libsqlite3-dev \
	libxml2 libxml2-dev \
	cmake libc6-dev libc6-dev-i386 libclang-dev llvm-dev automake \
	python-pip \
	vtun \
	lxc \
	g++-multilib \
	wget \
	vim


RUN pip install cxxfilt

WORKDIR /usr

RUN wget http://www.nsnam.org/release/ns-allinone-3.29.tar.bz2
RUN tar -xf ns-allinone-3.29.tar.bz2

RUN cd ns-allinone-3.29 && ./build.py --enable-examples --enable-tests
RUN ln -s /usr/ns-allinone-3.29/ns-3.29 /usr/ns3

RUN apt-get clean && \
    	rm -rf /var/lib/apt && \
    	rm /usr/ns-allinone-3.29.tar.bz2

WORKDIR /usr/ns3

RUN /bin/bash
