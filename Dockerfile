FROM ubuntu:20.04

RUN apt-get update && apt-get install -y \
    build-essential cmake libssl-dev

WORKDIR /app

COPY . .

RUN cmake . && make

EXPOSE 80

CMD ["./WebServer"]