CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -O2
LDFLAGS = -shared
LIBS = -lm

PACKAGE = ffi
VERSION = scm-1

SRC_DIR = src
BUILD_DIR = build

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

.PHONY: all clean

all: $(PACKAGE).so

$(PACKAGE).so: $(OBJ_FILES)
	$(CC) $(LDFLAGS) $(LIBS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR) $(PACKAGE).so