FROM ubuntu:20.04

# Set environment variables to non-interactive
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y \
    cmake \
    g++ \
    libasio-dev \
    libssl-dev \
    libsqlite3-dev \
    git \
    wget \
    && apt-get clean

RUN git clone https://github.com/google/googletest.git /usr/src/gtest && \
    cd /usr/src/gtest && \
    cmake . && \
    make && \
    make install

COPY . .

RUN mkdir build && cd build && \
    cmake .. && \
    make

EXPOSE 18080

CMD ["./build/DominoRest"]
