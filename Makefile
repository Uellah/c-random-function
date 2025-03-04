# Переменные
CC = gcc                # Компилятор

CFLAGS = -Wall -Wextra -Xpreprocessor -fopenmp -lomp -ffast-math -O3 -march=native # Флаги компиляции
SRC = src/main.c src/tree.c src/functions.c  src/random_tree.c src/random_func.c  src/solve.c src/fractal_drawer.c# Файлы исходного кода
INCLUDE = -Iinclude -I/opt/homebrew/Cellar/sdl2/2.32.2/include -I/opt/homebrew/opt/libomp/include     # Директория с заголовочными файлами
TARGET = random_tree    # Название выходного файла
LDFLAGS = -lm -L/opt/homebrew/Cellar/sdl2/2.32.2/lib -lSDL2 -L/opt/homebrew/opt/libomp/lib

# Правила сборки
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(INCLUDE) $(LDFLAGS)

clean:
	rm -f $(TARGET)

