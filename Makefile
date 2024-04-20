BIN := snapgram_serial snapgram_thread snapgram_block

CFLAGS := -Wall -Wextra -Werror -O2

all: $(BIN)

# Programs
snapgram_%: snapgram.c sg_%.c
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

# Cleaning
clean:
	-rm -f $(BIN)
