run: scanner.o reader.o charcode.o token.o error.o
	gcc -o run scanner.o reader.o charcode.o token.o error.o
scanner.o: reader.h charcode.h token.h error.h
	gcc -c scanner.c
reader.o: reader.h
	gcc -c reader.c
charcode.o: charcode.h
	gcc -c charcode.c
token.o: token.h
	gcc -c token.c
error.o: error.h
	gcc -c error.c
clean:
	rm -f *.o run
