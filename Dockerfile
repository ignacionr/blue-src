FROM ubuntu:25.04

RUN apt-get update && apt-get install -y \
    clang++-18 \
    cmake

RUN mkdir -p /src/build
WORKDIR /src
COPY . .

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
RUN cmake --build build --config Release
