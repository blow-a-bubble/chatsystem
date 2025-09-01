all: client server
client: client.cc echo.pb.cc
	g++ -o $@ $^ -lbrpc -lleveldb -lgflags -lssl -lcrypto -lprotobuf -std=c++17
server:server.cc echo.pb.cc
	g++ -o $@ $^ -lbrpc -lleveldb -lgflags -lssl -lcrypto -lprotobuf -std=c++17
.PHONY: clean
clean:
	rm -f client server
