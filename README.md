# web-mmo-multiplayer

### Production

```
$ docker-compose uo
```

And look on http://localhost:8081/


### Development

Start containers
```
$ docker-compose -f docker-compose.dev.yml up
```
Attach to golang server

```
$ docker exec -it wwm-server bash
root@426a52214ad7:/go/src/app# ./install_requirements.sh
root@426a52214ad7:/go/src/app# exec go run server.go
```

For remove containers use a command: 
```
$ docker-compose -f docker-compose.dev.yml down
```