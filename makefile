todo: main.o archivo.o version.o mod.o utils.o
	g++ -Wall -o main main.o archivo.o version.o mod.o utils.o
main.o: src/main.c
	g++ -Wall -Iinclude -c src/main.c
archivo.o: include/archivo.h src/archivo.c
	g++ -Wall -Iinclude -c src/archivo.c
version.o: include/version.h src/version.c
	g++ -Wall -Iinclude -c src/version.c
mod.o: include/mod.h src/mod.c
	g++ -Wall -Iinclude -c src/mod.c
utils.o: include/utils.h src/utils.c
	g++ -Wall -Iinclude -c src/utils.c
limpiar:
	rm -f main
	rm -f *.o

