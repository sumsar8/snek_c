all:
	g++ -I src/include -L src/lib -o main main.c linkedlist.c -lmingw32 -lSDL2main -lSDL2 