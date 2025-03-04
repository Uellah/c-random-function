# Компилятор
CC = gcc

UNAME_S := $(shell uname -s)

# Флаги компиляции
CFLAGS = -Wall -Wextra -ffast-math -O3 -march=native

# Файлы исходного кода
SRC = src/main.c src/tree.c src/functions.c  src/random_tree.c src/random_func.c  src/solve.c src/fractal_drawer.c

# Директория с заголовочными файлами
INCLUDE = -Iinclude

# Название выходного файла
TARGET = random_tree

LDFLAGS = -lm -lSDL2

ifeq ($(UNAME_S), Darwin)
    CFLAGS += -Xpreprocessor -fopenmp -lomp
    INCLUDE += -I/opt/homebrew/Cellar/sdl2/2.32.2/include -I/opt/homebrew/opt/libomp/include
    LDFLAGS = -lm -L/opt/homebrew/Cellar/sdl2/2.32.2/lib -lSDL2 -L/opt/homebrew/opt/libomp/lib
else
	CFLAGS += -fopenmp
endif

# Правила сборки
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(INCLUDE) $(LDFLAGS)

clean:
	rm -f $(TARGET)

