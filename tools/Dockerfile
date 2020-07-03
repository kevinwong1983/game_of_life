FROM ubuntu:18.04

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get -y update && \
    apt-get -y upgrade && \
    apt-get -y dist-upgrade && \
    echo "update done"

RUN apt-get install -y software-properties-common && \
    add-apt-repository ppa:mhier/libboost-latest && \
    add-apt-repository ppa:mosquitto-dev/mosquitto-ppa && \
    apt-get -y update && \
    echo "adding apt repository done"

RUN apt install -y --no-install-recommends apt-utils && \
    apt-get install -y \
    clang \
    cmake \
    clang-tidy \
    g++ \
    gcc \
    git \
    make \
    libboost1.73-dev \
    valgrind \
    mosquitto \
    mosquitto-clients \
    libmosquitto-dev \
    build-essential \
    qt5-default && \
    echo "apt-get install done"

RUN apt-get install -y google-mock && \
    cd /usr/src/googletest/googlemock && \
    cmake CMakeLists.txt && \
    make install -j8 && \
    cd /usr/src/googletest/googletest && \
    cmake CMakeLists.txt && \
    make install -j8 && \
    echo "install gtest and gmock done"

RUN git clone https://github.com/taocpp/json.git && \
    cd json && \
    git checkout tags/1.0.0-beta.11 -b 1_0_0_beta_11 && \
    cmake CMakeLists.txt && \
    make install -j8 && \
    echo "install tao-json done"
