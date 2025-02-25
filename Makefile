# Переменные
CC = gcc                # Компилятор
CFLAGS = -Wall -Wextra -fopenmp # Флаги компиляции
SRC = src/main.c src/tree.c src/functions.c  src/random_tree.c src/random_func.c  src/solve.c# Файлы исходного кода
INCLUDE = -Iinclude     # Директория с заголовочными файлами
TARGET = random_tree    # Название выходного файла
LDFLAGS = -lm

# Правила сборки
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(INCLUDE) $(LDFLAGS)

clean:
	rm -f $(TARGET)

