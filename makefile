# Variables
SRC_DIR = src
INC_DIR = include

# Regla principal (mantengo 'todo' del profesor)
todo: main.o archivo.o version.o mod.o utils.o
	g++ -Wall -Wextra -o main main.o archivo.o version.o mod.o utils.o

# Compilación individual
main.o: $(SRC_DIR)/main.c
	g++ -Wall -Wextra -I$(INC_DIR) -c $(SRC_DIR)/main.c

archivo.o: $(INC_DIR)/archivo.h $(SRC_DIR)/archivo.c
	g++ -Wall -Wextra -I$(INC_DIR) -c $(SRC_DIR)/archivo.c

version.o: $(INC_DIR)/version.h $(SRC_DIR)/version.c
	g++ -Wall -Wextra -I$(INC_DIR) -c $(SRC_DIR)/version.c

mod.o: $(INC_DIR)/mod.h $(SRC_DIR)/mod.c
	g++ -Wall -Wextra -I$(INC_DIR) -c $(SRC_DIR)/mod.c

utils.o: $(INC_DIR)/utils.h $(SRC_DIR)/utils.c
	g++ -Wall -Wextra -I$(INC_DIR) -c $(SRC_DIR)/utils.c

# Limpieza (igual al profesor)
limpiar:
	rm -f main
	rm -f *.o

# Extra útil: valgrind
valgrind: todo
	valgrind --leak-check=full --show-leak-kinds=all ./main

.PHONY: todo limpiar valgrind

