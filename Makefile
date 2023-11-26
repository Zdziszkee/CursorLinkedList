CC = g++
CFLAGS = -Wall -Wextra -O3 -pedantic -std=c++11

all: CursorList.x ArrayList.x

CursorList.x: CursorList.cpp
	$(CC) $(CFLAGS) -o CursorList.x CursorList.cpp

ArrayList.x: ArrayList.cpp
	$(CC) $(CFLAGS) -o ArrayList.x ArrayList.cpp

clean:
	rm -f *.x