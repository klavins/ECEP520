#
# To use this image to compile C and C++ projects, do something like
# docker build -t cppenv .
# docker run -v $PWD/hw_1:/source -it cppenv bash
#

# Get the GCC preinstalled image from Docker Hub
FROM gcc:4.9

MAINTAINER Eric Klavins "klavins@uw.edu"

# Get cmake for googletest, graphviz and doxygen for documentation
RUN apt-get update && \
    apt-get install -y cmake && \
    apt-get install -y cppcheck && \
    apt-get install -y graphviz && \
    apt-get install -y doxygen && \
    apt-get install -y nano

# Build google test
WORKDIR /usr/src
RUN git clone https://github.com/google/googletest.git
WORKDIR /usr/src/googletest/install
RUN cmake ../ && make && make install

WORKDIR /source

COPY bashrc /root/.bashrc
