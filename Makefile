CC = gcc
CFLAGS = -Wall -Iinclude

SRC = main.c src/user.c src/system_services.c src/system_calls.c \
      src/kernel.c src/interrupt.c src/cpu.c src/process.c src/thread.c
OBJ_DIR = ./obj
OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC))
BIN_DIR = ./bin
BIN = $(BIN_DIR)/os_simulation

all: $(BIN)

$(BIN): $(OBJ)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(OBJ_DIR)/src
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)