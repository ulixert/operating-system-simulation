FROM debian:bullseye

RUN apt-get update && apt-get install -y gcc make

WORKDIR /os-simulation
COPY . .

RUN make clean && make

CMD ["./bin/os_simulation"]