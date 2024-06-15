BUILD_NAME=barnes-hut

CC=gcc
OPT=3

PREFIX_SRC= ./src/
PREFIX_OBJ= ./obj/
PREFIX_BIN= ./bin/

OBJ= main.o quad_tree.o

OBJ:= $(addprefix $(PREFIX_OBJ), $(OBJ))

FLUGS+= -lm -ltalloc
CFLUGS+= -Wall -O$(OPT)

$(PREFIX_BIN)$(BUILD_NAME) : $(OBJ)
	mkdir -p $(PREFIX_BIN)
	$(CC) $(OBJ) -o $(PREFIX_BIN)$(BUILD_NAME) $(FLUGS)
	ln -f $(PREFIX_BIN)$(BUILD_NAME) $(BUILD_NAME)

$(PREFIX_OBJ)%.o : $(PREFIX_SRC)%.c
	mkdir -p $(PREFIX_OBJ)
	$(CC) -c $< -o $@ $(CFLUGS)

clean :
	rm -fr $(PREFIX_BIN)* $(PREFIX_OBJ)*.o
	rm $(BUILD_NAME)
