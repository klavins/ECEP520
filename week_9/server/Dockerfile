#
# To use this image to compile C and C++ projects, do something like
# docker build -t cppenv-http .
# docker run -v $PWD/hw_1:/source -it cppenv-http bash
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

# Install json.h
RUN mkdir /usr/local/include/json
WORKDIR /usr/local/include/json
RUN curl -O -J -L https://github.com/nlohmann/json/releases/download/v3.5.0/json.hpp
RUN mv json.hpp json.h

# Install httplib
WORKDIR /tmp
RUN git clone https://github.com/klavins/cpp-httplib.git
RUN mkdir /usr/local/include/httplib
RUN mv /tmp/cpp-httplib/httplib.h /usr/local/include/httplib
RUN rm -r /tmp/cpp-httplib
EXPOSE 80

# Build server
COPY Makefile /home
COPY server.cc /home
WORKDIR /home
RUN make

# Run server
CMD /home/bin/server
