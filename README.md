# Fractal Function Generator & Drawer

Система генерации случайных функций и визуализации фракталов на языке C с использованием SDL2 и OpenMP.  
Проект позволяет создавать случайные деревья функций, оценивать их сходимость и отображать фракталы в интерактивном окне.

## Примеры фракталов

![Пример фрактала 1](https://i.imgur.com/abjYq5k.png)
![Пример фрактала 2](https://i.imgur.com/w5wFvUT.png)
![Пример фрактала 3](https://i.imgur.com/b5ezU4Y.png)
![Пример фрактала 4](https://i.imgur.com/ae0ctJX.png)

## Возможности

- **Генерация случайных деревьев функций**  
  Построение бинарного дерева операций (унарных и бинарных) с настраиваемой глубиной.
- **Динамическая привязка функций**  
  Поддержка функций с одним параметром (√, ln, sin, cos, tan, exp, сопряжение) и с двумя параметрами (+, −, ×, ÷, ^), а также комплексных констант.
- **Анализ сходимости**  
  Два метода проверки сходимости (MSI и BM) с порогами, настраиваемыми через заголовочные файлы.
- **Построение и рендеринг фракталов**  
  Интерактивное окно SDL2 с поддержкой плавного цветового градиента и масштабирования по мыши.
- **Параллельные вычисления**  
  Использование OpenMP для ускорения расчётов точек фрактала и записи результатов в CSV.

## Требования

- Компилятор GCC (или совместимый), поддерживающий C11  
- SDL2 (заголовочные файлы и библиотека)  
- Библиотека math (`-lm`)  
- (Опционально) OpenMP для многопоточности  

## Установка

1. Клонировать репозиторий:
   ```sh
   git clone https://github.com/Uellah/c-random-function
   cd c-random-function
   ```
2. Установить зависимости:
   - На Ubuntu / Debian:
     ```sh
     sudo apt-get update
     sudo apt-get install build-essential libsdl2-dev
     ```
   - На macOS (Homebrew):
     ```sh
     brew install gcc sdl2
     ```

## Сборка

Сборка и линковка производится через Makefile:

```sh
make
```

Для очистки артефактов:
```sh
make clean
```

## Использование

Запуск генерации и отображения фрактала:
```sh
./random_tree
```

- **Левая кнопка мыши**: приближение (zoom-in) к текущей позиции курсора  
- **Правая кнопка мыши**: отдаление (zoom-out)  
- **ESC** или закрытие окна: завершить работу

Результаты расчётов можно сохранить в CSV:
```c
save_points_to_file("output.csv", func, x_center, y_center, range, resolution);
```

## Конфигурация

Настройки размеров окна и разрешения фрактала задаются в `fractal_drawer.h`:

```c
#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   800
#define FRACTAL_WIDTH   800
#define FRACTAL_HEIGHT  800
#define MAX_ITER        1000
#define EPS             1e-14
#define SUCCESS_PROB    0.7    // вероятность ветвления дерева
```

При необходимости можно изменить максимальную глубину случайного дерева, передав аргумент в `get_random_tree(depth)`.


