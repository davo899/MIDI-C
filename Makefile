CC=gcc
OBJ_DIR=build

SRC = $(wildcard src/main/*.c) $(wildcard src/test/*.c)
DEPS = $(wildcard src/main/*.h) $(wildcard src/test/*.h)
OBJ = $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SRC))

test: $(OBJ)
	$(CC) -o $@ $^

dirs:
	mkdir -p $(OBJ_DIR)/main
	mkdir -p $(OBJ_DIR)/test

$(OBJ_DIR)/%.o: src/%.c $(DEPS) dirs
	$(CC) -c -o $@ $<

lib: $(patsubst src/main/%.c,$(OBJ_DIR)/main/%.o,$(wildcard src/main/*.c))
	rm -f lib_MIDI.a
	ar rcs lib_MIDI.a $(patsubst src/main/%.c,$(OBJ_DIR)/main/%.o,$(wildcard src/main/*.c))

.PHONY: clean

clean:
	rm -f $(OBJ_DIR)/main/*.o
	rm -f $(OBJ_DIR)/test/*.o
	rm -f test
	rm -f test.exe
	rm -f lib_MIDI.a
