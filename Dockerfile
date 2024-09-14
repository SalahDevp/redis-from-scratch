FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y cmake build-essential g++ && \
    apt-get clean

WORKDIR /app

COPY . .

RUN cmake -S . -B build && \
    cmake --build build

CMD ["./build/bin/server"]
