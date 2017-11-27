# This file was adapted from template files provided by David Wood.
# This was sourced from Homework 3 of his CS 758 at UW - Madison.
# http://pages.cs.wisc.edu/~david/courses/cs758/Fall2016/wiki/index.php?n=Main.Homework3

# That, in turn, was based off an assignment by John Mellor-Crummey at Rice University.

CXX = gcc
OUTPUT = reversi

DEBUG = TRUE

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
CFLAGS = -Wall -Wextra $(INCLUDES)

ifdef DEBUG
	CFLAGS += -g
else
	CFLAGS += -O2
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
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/$(OUTPUT)
