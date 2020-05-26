FROM golang:1.9.2-stretch

WORKDIR /go/src/app

COPY ./src/ /go/src/app

# Better use a localfolders
RUN go get github.com/go-sql-driver/mysql \
    && go get github.com/jmoiron/sqlx \
    && go get github.com/gorilla/mux \
    && echo 1

EXPOSE 8081

CMD exec go run server.go


