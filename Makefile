
CC=g++
SRC_DIR=src
BIN_DIR=bin
OBJ_DIR=obj
INC_DIR=include
SDL_CONFIG=sdl2-config
SRC=$(wildcard $(SRC_DIR)/*.cpp)
OBJS=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))
OUT=texel
CFLAGS=$(shell $(SDL_CONFIG) --cflags) -std=c++20
INC=-I$(INC_DIR)/
LIBS=$(shell $(SDL_CONFIG) --libs) -lGLEW
EXTRA_ARGS=
ifeq ($(OS), Windows_NT)
	DEBUG=-g
	CC=x86_64-w64-mingw32-gcc 
	SDL_CONFIG=$(shell echo $$WINDOWS_SDL_CONFIG_LOCATION)
	CFLAGS += --std=c99
else
	OS=$(shell uname)
	DEBUG=-ggdb
	INC += -I/usr/local/include
	ifeq ($(OS), Darwin)
		INC += -I/opt/homebrew/include
		EXTRA_ARGS+=-framework OpenGL
	else
		INC += -I/usr/local/include
		LIBS+=-lGL
	endif
endif


debug: CFLAGS += $(DEBUG)
debug: CFLAGS += -DDEBUG_TEST

all: $(OBJS)
	$(CC) $^ $(CFLAGS) $(LIBS) -o $(addprefix $(BIN_DIR)/,$(OUT)) $(EXTRA_ARGS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

debug: $(OBJS)
	$(CC) $^ $(CFLAGS) $(LIBS) -o $(addprefix $(BIN_DIR)/,$(OUT)_debug)

install: 
	cp ./bin/shdr /usr/local/bin

clean:
	rm -rf $(BIN_DIR)/*
	rm -f $(OBJ_DIR)/*.o
