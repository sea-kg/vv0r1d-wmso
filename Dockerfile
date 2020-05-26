FROM golang:1.9.2-stretch

WORKDIR /go/src/app

COPY ./src/ /go/src/app

# Better use a localfolders
RUN go get github.com/go-sql-driver/mysql
RUN go get github.com/jmoiron/sqlx
RUN go get github.com/gorilla/mux

EXPOSE 8081

CMD exec go run server.go


