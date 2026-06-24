BIN_DIR=../bin
BUILD_DIR=../build
NAME = $(BIN_DIR)/micro_cc
CC=clang
SAN=-fsanitize=address,undefined
CFLAGS=-g -Wall -Wextra -Werror $(SAN)
SOURCES=$(wildcard */*.c) $(wildcard *.c)
OBJECTS=$(patsubst %.c,$(BUILD_DIR)/%.o,$(SOURCES))

all: $(NAME)
$(NAME):$(OBJECTS)
	mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) $^ -o $@
$(BUILD_DIR)/%.o:%.c
	mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) -c $^ -o $@
clean:
	rm -rf $(OBJECTS)
fclean: clean
	rm -rf $(NAME)
re: fclean all
show:
	echo SOURCES=$(SOURCES)
	echo OBJECTS=$(OBJECTS)
.PHONY: all clean fclean re