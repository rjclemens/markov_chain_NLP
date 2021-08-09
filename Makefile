SRCS=$(wildcard *.cpp)
OBJS=out
FLGS=-std=c++17
OPTS=-o2 -g

all: build clean
	
build: compile
	./$(OBJS)

clean:
	rm -f $(OBJS)
	rm -r *.dSYM

compile:
	g++ $(SRCS) $(OPTS) $(FLGS) -o $(OBJS)
