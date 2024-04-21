FROM ghcr.io/bounverif/boostdevel:latest as builder

ARG ZENOH_VERSION=0.10.1-rc
ARG ZENOHC_VERSION=${ZENOH_VERSION}
ARG REELAY_VERSION=main
ARG SIMDJSON_VERSION=3.9.1

ARG RUST_VERSION=1.72.1
ARG RUST_PROFILE=minimal
ARG RUST_TOOLCHAIN=${RUST_VERSION}-x86_64-unknown-linux-gnu
ENV CARGO_HOME="/usr/local/cargo"

ENV RUSTUP_HOME="/usr/local/rustup"
ENV PATH "/usr/local/cargo/bin:/usr/local/rustup:${PATH}"

RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | \
    sh -s -- -y --no-modify-path --profile ${RUST_PROFILE} --default-toolchain ${RUST_VERSION}

RUN git clone --depth 1 --branch ${ZENOHC_VERSION} https://github.com/eclipse-zenoh/zenoh-c.git /tmp/zenoh-c && \
    cmake -S/tmp/zenoh-c -B/tmp/zenoh-c/build \
        -DCMAKE_BUILD_TYPE=Release \
        -DZENOHC_CARGO_CHANNEL=${RUST_TOOLCHAIN} \
        -DZENOHC_INSTALL_STATIC_LIBRARY=TRUE \
        && \
    cmake --build /tmp/zenoh-c/build/ --target install -j$(nproc) && \
    rm -rf /tmp/zenoh-c

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get install -y \
      automake \
      autotools-dev \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*

RUN git clone --depth 1 https://github.com/doganulus/cudd.git /tmp/cudd \
    && cd /tmp/cudd \
    && ./configure && aclocal && autoconf \
    && ./configure --enable-silent-rules --enable-shared --enable-obj \
    && make && make install \
    && cd / && rm -rf /tmp/cudd

RUN git clone --depth 1 --branch ${REELAY_VERSION} https://github.com/doganulus/reelay.git /tmp/reelay \
    && cmake -S/tmp/reelay -B/tmp/reelay/build \
      -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_REELAY_APPS=OFF \
      -DBUILD_TESTS=OFF \
      -DBUILD_PYTHON_BINDINGS=OFF \
    && cmake --build /tmp/reelay/build/ --target install -j$(nproc) \
    && rm -rf /tmp/reelay

RUN git clone --depth 1 --branch v${SIMDJSON_VERSION} https://github.com/lemire/simdjson.git /tmp/simdjson \
    && cmake -S/tmp/simdjson -B/tmp/simdjson/build \
      -DSIMDJSON_BUILD_STATIC_LIB=ON\
    && cmake --build /tmp/simdjson/build/ --target install -j$(nproc) \
    && rm -rf /tmp/simdjson

COPY . /app/
WORKDIR /app/

RUN cmake --preset default && \
    cmake --build  --target install -j$(nproc) --preset default 

FROM debian:bookworm-slim
COPY --from=builder /usr/local/bin/reelayd /usr/local/bin
