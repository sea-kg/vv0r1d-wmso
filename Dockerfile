FROM debian:10

RUN apt-get update && apt-get install cmake gcc g++ make \
    pkg-config -y && rm -rf /var/lib/apt/lists/*

COPY . /opt/src

WORKDIR /opt/src

RUN mkdir -p tmp

WORKDIR /opt/src/tmp
RUN cmake ..
RUN make

WORKDIR /opt/src

CMD ["bash", "-c", "./vv-server"]
