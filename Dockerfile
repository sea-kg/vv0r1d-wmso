FROM golang:1.14-stretch

WORKDIR /go/src/app

COPY ./src/ /go/src/app

RUN ./install_requirements.sh

EXPOSE 80

CMD exec go run server.go


