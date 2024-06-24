BUILD_NAME=celestial\ mechanics

CC=gcc
OPT=2

SRC_PATH= ./src/
OBJ_PATH= ./obj/
LIB_PATH= ./lib/
BIN_PATH= ./bin/

CIMGUI_DIR=cimgui/
RAYLIB_DIR=raylib/raylib/src/

OBJ=main.o quad_tree.o particle.o talloc.o rlcimgui.o
LIBRAYLIB=$(LIB_PATH)$(RAYLIB_DIR)libraylib.a
LIBCIMGUI=$(LIB_PATH)$(CIMGUI_DIR)libcimgui.a
LIB=$(LIBRAYLIB) $(LIBCIMGUI)

OBJ:= $(addprefix $(OBJ_PATH), $(OBJ))

CFLAGS+= -lm -lstdc++
CCFLAGS+= -Wall -O$(OPT)

$(BIN_PATH)$(BUILD_NAME) : $(OBJ) $(LIB)
	$(CC) $(OBJ) $(LIB) -o $(BIN_PATH)$(BUILD_NAME) $(CFLAGS)
	ln -f $(BIN_PATH)$(BUILD_NAME) $(BUILD_NAME)

$(OBJ_PATH)%.o : $(SRC_PATH)%.c
	$(CC) -c $< -o $@ $(CCFLAGS)

$(LIBCIMGUI) :
	make -C $(LIB_PATH)$(CIMGUI_DIR) CXXFLAGS="-O1 -std=c++11 -DIMGUI_DISABLE_OBSOLETE_KEYIO=1" static

$(LIBRAYLIB) :
	make -C $(LIB_PATH)$(RAYLIB_DIR)

$(OBJ_PATH)talloc.o : $(LIB_PATH)talloc/talloc.c
	$(CC) -c $< -o $@ $(CCFLAGS)

$(OBJ_PATH)rlcimgui.o : $(LIB_PATH)raylib/rlcimgui.c
	$(CC) -c $< -o $@ $(CCFLAGS)

clean :
	make -C $(LIB_PATH)$(CIMGUI_DIR) clean
	make -C $(LIB_PATH)$(RAYLIB_DIR) clean
	rm -fr $(BIN_PATH)* $(OBJ_PATH)*.o $(LIB)
	rm $(BUILD_NAME)
