FROM ubuntu:22.04

#install build tools
RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

#copy source

COPY . .

#Build 
RUN cmake -B build && cmake --build build

#default command - runs in random walk mode
CMD ["./build/MarketSim"]