CC=gcc
OBJ_DIR=build

SRC = $(wildcard src/main/*.c) $(wildcard src/test/*.c)
DEPS = $(wildcard src/main/*.h) $(wildcard src/test/*.h)
OBJ = $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SRC))

dirs:
	mkdir -p $(OBJ_DIR)/main
	mkdir -p $(OBJ_DIR)/test

$(OBJ_DIR)/%.o: src/%.c $(DEPS) dirs
	$(CC) -c -o $@ $<

test: $(OBJ)
	$(CC) -o $@ $^

.PHONY: clean

clean:
	rm -f $(OBJ_DIR)/*.o
	rm test
