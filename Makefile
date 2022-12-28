SOURCE_PATH    := src
OBJECT_PATH    := dist
BIN_PATH       := bin
EXECUTABLE     := init
EXEC_SRC_PATH  := src/exec

CC             := gcc
CFLAGS         := -O2 -Wall -Wextra -Iincl
LDFLAGS        := -static

EXEC_SRC_FILES := $(wildcard $(EXEC_SRC_PATH)/*.c)
EXEC_FILES     := $(patsubst $(EXEC_SRC_PATH)/%.c,$(BIN_PATH)/%,$(EXEC_SRC_FILES))

SOURCE_FILES   := $(wildcard $(SOURCE_PATH)/*.c)
OBJECT_FILES   := $(patsubst $(SOURCE_PATH)/%.c,$(OBJECT_PATH)/%.o,$(SOURCE_FILES))

.PRECIOUS: $(OBJECT_PATH)/%.o

all: compile_flags.txt $(EXEC_FILES)

clean:
	rm -rf $(BIN_PATH) $(OBJECT_PATH)

$(BIN_PATH) $(OBJECT_PATH):
	mkdir -p $@

$(OBJECT_PATH)/%.o: $(SOURCE_PATH)/%.c Makefile $(HEADER_FILES) | $(OBJECT_PATH)
	$(CC) -o $@ -c $(CFLAGS) $<

$(BIN_PATH)/%: $(EXEC_SRC_PATH)/%.c $(OBJECT_FILES) $(HEADER_FILES) | $(BIN_PATH)
	$(CC) -o $@ $(CFLAGS) $< $(OBJECT_FILES) $(LDFLAGS) $(LDFLAGS_$(patsubst $(BIN_PATH)/%,%,$@))

compile_flags.txt: Makefile
	echo $(CFLAGS) | tr " " "\n" > compile_flags.txt
