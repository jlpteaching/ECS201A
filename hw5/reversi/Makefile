CXX = gcc
OUTPUT = reversi

DEBUG_MODE = TRUE

BIN_DIR = ./bin
INC_DIR = ./include
OBJ_DIR = ./obj
SRC_DIR = ./src

OBJS = $(OBJ_DIR)/hwtimer.o \
	   $(OBJ_DIR)/reversi.o \
	   $(OBJ_DIR)/reversi-human.o \
	   $(OBJ_DIR)/reversi-simple-ai.o \
	   $(OBJ_DIR)/reversi-good-ai.o

INCLUDES += -I$(INC_DIR)
CFLAGS = -Wall $(INCLUDES)

ifdef DEBUG_MODE
	CFLAGS += -g
else
	CFLAGS += -O3
endif

all: directories $(BIN_DIR)/$(OUTPUT)

$(BIN_DIR)/$(OUTPUT): $(OBJS)
	$(CXX) $(OBJS) $(CFLAGS) -o $(BIN_DIR)/$(OUTPUT)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CXX) -c $(CFLAGS) $< -o $@

directories:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

clean:
	-rm $(OBJ_DIR)/*.o $(BIN_DIR)/$(OUTPUT)
