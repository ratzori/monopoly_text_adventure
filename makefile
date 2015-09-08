CC=gcc
CFLAGS=-Wall
DEBUG_FLAGS=-g
BIN=monopoly_text_adventure

release:
	$(CC) $(CFLAGS) src/*.c -o $(BIN)
debug:
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) src/*.c -o $(BIN)
clean:
	rm -f $(BIN)

