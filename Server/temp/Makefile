CXX = g++
CXXFLAGS = -std=c++17 -Wall -O3

server: server.o
	$(CXX) $(CXXFLAGS) -o server server.o

server.o:

client: client.o
	$(CXX) $(CXXFLAGS) -o client client.o

client.o:

clean:
	rm -f server client *.o