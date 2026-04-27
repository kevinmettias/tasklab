export PATH := C:/msys64/ucrt64/bin;$(PATH)

CC       = C:/msys64/ucrt64/bin/gcc.exe

CMOCKA_INC = C:/msys64/ucrt64/include
CMOCKA_LIB = C:/msys64/ucrt64/lib

CFLAGS   = -Wall -Wextra -I$(CMOCKA_INC) -Iinclude
DEBUG    = -g -O0 $(CFLAGS)
LDFLAGS  = -L$(CMOCKA_LIB) -lcmocka

SRC          = $(wildcard src/*.c)
TEST_SRC     = $(wildcard tests/*.c)
BENCH_SRC    = $(wildcard bench/*.c)
SRC_NO_MAIN  = $(filter-out src/main.c, $(SRC))

APP_BIN   = build/tasklab.exe
TEST_BIN  = build/test_runner.exe
BENCH_BIN = build/bench_runner.exe

OBJ = $(patsubst src/%.c, build/%.o, $(SRC))

.PHONY: all test bench clean

all: build $(APP_BIN)

test: build $(TEST_BIN)
	$(TEST_BIN)

bench: build $(BENCH_BIN)
	$(BENCH_BIN)

build:
	if not exist build mkdir build

$(APP_BIN): $(OBJ)
	$(CC) $(DEBUG) $^ -o $@

$(TEST_BIN): $(SRC_NO_MAIN) $(TEST_SRC)
	$(CC) $(DEBUG) $^ -o $@ $(LDFLAGS)

$(BENCH_BIN): $(SRC_NO_MAIN) $(BENCH_SRC)
	$(CC) $(DEBUG) $^ -o $@

build/%.o: src/%.c
	$(CC) $(DEBUG) -c $< -o $@

clean:
	if exist build rmdir /S /Q build
