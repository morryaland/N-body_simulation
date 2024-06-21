BUILD_NAME=celestial\ mechanics

CC=gcc
OPT=2

PREFIX_SRC= ./src/
PREFIX_OBJ= ./obj/
PREFIX_LIB= ./lib/
PREFIX_BIN= ./bin/

CIMGUI_DIR=cimgui/
RAYLIB_DIR=raylib/raylib/src/

OBJ=main.o quad_tree.o talloc.o rlcimgui.o
LIB=$(CIMGUI_DIR)libcimgui.a $(RAYLIB_DIR)libraylib.a

OBJ:= $(addprefix $(PREFIX_OBJ), $(OBJ))
LIB:= $(addprefix $(PREFIX_LIB), $(LIB))

CFLAGS+= -lm -lstdc++
CCFLAGS+= -Wall -O$(OPT)

$(PREFIX_BIN)$(BUILD_NAME) : $(OBJ) $(LIB)
	$(CC) $(OBJ) $(LIB) -o $(PREFIX_BIN)$(BUILD_NAME) $(CFLAGS)
	ln -f $(PREFIX_BIN)$(BUILD_NAME) $(BUILD_NAME)

$(PREFIX_OBJ)%.o : $(PREFIX_SRC)%.c
	$(CC) -c $< -o $@ $(CCFLAGS)

$(PREFIX_LIB)$(CIMGUI_DIR)libcimgui.a :
	make -C $(PREFIX_LIB)$(CIMGUI_DIR) CXXFLAGS="-O1 -std=c++11 -DIMGUI_DISABLE_OBSOLETE_KEYIO=1" static

$(PREFIX_LIB)$(RAYLIB_DIR)libraylib.a :
	make -C $(PREFIX_LIB)$(RAYLIB_DIR)

$(PREFIX_OBJ)talloc.o : $(PREFIX_LIB)talloc/talloc.c
	$(CC) -c $< -o $@ $(CCFLAGS)

$(PREFIX_OBJ)rlcimgui.o : $(PREFIX_LIB)raylib/rlcimgui.c
	$(CC) -c $< -o $@ $(CCFLAGS)

clean :
	make -C $(PREFIX_LIB)$(CIMGUI_DIR) clean
	make -C $(PREFIX_LIB)$(RAYLIB_DIR) clean
	rm -fr $(PREFIX_BIN)* $(PREFIX_OBJ)*.o $(LIB)
	rm $(BUILD_NAME)
