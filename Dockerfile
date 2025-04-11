FROM ubuntu:25.04

RUN apt-get update && apt-get install -y \
    clang++-18 \
    cmake \
    libcurl4-openssl-dev \
    git curl zip unzip tar pkg-config

# Clone vcpkg
RUN git clone https://github.com/Microsoft/vcpkg.git /opt/vcpkg

# Build vcpkg
RUN /opt/vcpkg/bootstrap-vcpkg.sh

# Add vcpkg to PATH
ENV PATH="/opt/vcpkg:${PATH}"

RUN mkdir -p /src/build
WORKDIR /src
COPY . .

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
RUN cmake --build build --config Release

ENTRYPOINT [ "/src/build/blue-src" ]
