run: main.o parser.o scanner.o reader.o charcode.o token.o error.o
	gcc -o run main.o parser.o scanner.o reader.o charcode.o token.o error.o
main.o: reader.h parser.h
	gcc -c main.c
parser.o: reader.h scanner.h parser.h error.h
	gcc -c parser.c
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
