#include "fractal_drawer.h"
#include "types.h"
#include "solve.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>

#ifdef _OPENMP
#include <omp.h>
#endif

/* Размеры окна */
#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH 800
#endif
#ifndef WINDOW_HEIGHT
#define WINDOW_HEIGHT 800
#endif

/* Разрешение фрактала (число вычисляемых точек) */
#ifndef FRACTAL_WIDTH
#define FRACTAL_WIDTH 400
#endif
#ifndef FRACTAL_HEIGHT
#define FRACTAL_HEIGHT 400
#endif

/* Глобальные переменные для окна и рендерера */
static SDL_Window* window;
static SDL_Renderer* renderer;

/* Структура параметров фрактала */
typedef struct FracParams {
    double x_center;
    double y_center;
    double range;
} FracParams;

/* Глобальные переменные для управления и синхронизации */
static volatile int running = 1;
static FracParams currentFrac = {0.0, 0.0, 2.0};
static SDL_mutex* fracMutex = NULL;

/* Флаги и координаты мыши для зума по зажатому курсору */
static int leftDown = 0;
static int rightDown = 0;
static int mouseX = WINDOW_WIDTH / 2;
static int mouseY = WINDOW_HEIGHT / 2;

/* Инициализация SDL, создание окна и рендерера */
int init(){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Ошибка инициализации SDL: %s\n", SDL_GetError());
        return 1;
    }
    
    window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Ошибка создания окна: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Ошибка создания рендерера: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    return 0;
}

/* Новая функция для сглаженного отображения цвета. 
   t – нормализованное значение (0...1), где 1 соответствует MAX_ITER */
void get_color_smooth(double t, Uint8* r, Uint8* g, Uint8* b) {
    /* Пример полиномиальной интерполяции для плавного перехода */
    *r = (Uint8)(9*(1-t)*t*t*t*255);
    *g = (Uint8)(15*(1-t)*(1-t)*t*t*255);
    *b = (Uint8)(8.5*(1-t)*(1-t)*(1-t)*t*255);
}

/* Функция рисования фрактала с вычислениями на матрице FRACTAL_WIDTH x FRACTAL_HEIGHT.
   Используется сглаживание цвета.
   Если check_convergence_MSI возвращает целое число, можно применить сглаживание,
   предполагая, что значение iterations находится в диапазоне [0, MAX_ITER]. */
void draw_fractal(NODE* func, const FracParams * frac) {
    Uint32* pixels = malloc(FRACTAL_WIDTH * FRACTAL_HEIGHT * sizeof(Uint32));
    if (!pixels) {
        fprintf(stderr, "Ошибка выделения памяти для пикселей.\n");
        return;
    }
    
    /* Создаем текстуру с разрешением фрактала */
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, FRACTAL_WIDTH, FRACTAL_HEIGHT);
    if (!texture) {
        fprintf(stderr, "Ошибка создания текстуры: %s\n", SDL_GetError());
        free(pixels);
        return;
    }
    
    double step = (frac->range * 2) / FRACTAL_WIDTH;
    
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < FRACTAL_HEIGHT; i++) {
        for (int j = 0; j < FRACTAL_WIDTH; j++) {
            double re = frac->x_center - frac->range + j * step;
            double im = frac->y_center + frac->range - i * step;
            double complex point = re + im * I;
            
            int iterations = check_convergence_MSI(func, point);
            double t = (double)iterations / MAX_ITER;
            Uint8 red, green, blue;
            get_color_smooth(t, &red, &green, &blue);
            
            Uint32 pixel = (255 << 24) | (red << 16) | (green << 8) | blue;
            pixels[i * FRACTAL_WIDTH + j] = pixel;
        }
    }
    
    SDL_UpdateTexture(texture, NULL, pixels, FRACTAL_WIDTH * sizeof(Uint32));
    SDL_RenderClear(renderer);
    
    /* Масштабирование текстуры до размеров окна */
    SDL_Rect destRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
    SDL_RenderPresent(renderer);
    
    free(pixels);
    SDL_DestroyTexture(texture);
}

/* Потоковая функция для периодической перерисовки фрактала */
int drawing_thread(void* data) {
    NODE* func = (NODE*)data;
    while (running) {
        SDL_LockMutex(fracMutex);
        FracParams localFrac = currentFrac;
        SDL_UnlockMutex(fracMutex);
        
        draw_fractal(func, &localFrac);
        SDL_Delay(30);  // уменьшена задержка для более частых обновлений
    }
    return 0;
}

/* Основной цикл обработки событий.
   При зажатии левой кнопки происходит приближение к курсору,
   при зажатии правой – отдаление. Центр фрактала пересчитывается так,
   чтобы точка под курсором оставалась на месте. */
void run_fractal_loop(NODE* func) {
    fracMutex = SDL_CreateMutex();
    running = 1;
    currentFrac.x_center = 0.0;
    currentFrac.y_center = 0.0;
    currentFrac.range = 2.0;
    
    SDL_Thread* thread = SDL_CreateThread(drawing_thread, "DrawingThread", func);
    
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        running = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        leftDown = 1;
                        mouseX = event.button.x;
                        mouseY = event.button.y;
                    } else if (event.button.button == SDL_BUTTON_RIGHT) {
                        rightDown = 1;
                        mouseX = event.button.x;
                        mouseY = event.button.y;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT)
                        leftDown = 0;
                    else if (event.button.button == SDL_BUTTON_RIGHT)
                        rightDown = 0;
                    break;
                case SDL_MOUSEMOTION:
                    if (leftDown || rightDown) {
                        mouseX = event.motion.x;
                        mouseY = event.motion.y;
                    }
                    break;
                default:
                    break;
            }
        }
        
        /* При зажатой кнопке вычисляем новый центр и масштаб */
        if (leftDown || rightDown) {
            double factor = leftDown ? 0.99 : 1.01;
            
            SDL_LockMutex(fracMutex);
            double fx = currentFrac.x_center - currentFrac.range + (mouseX / (double)WINDOW_WIDTH) * (2 * currentFrac.range);
            double fy = currentFrac.y_center + currentFrac.range - (mouseY / (double)WINDOW_HEIGHT) * (2 * currentFrac.range);
            double newRange = currentFrac.range * factor;
            double newCenterX = fx + newRange - (mouseX / (double)WINDOW_WIDTH) * (2 * newRange);
            double newCenterY = fy - newRange + (mouseY / (double)WINDOW_HEIGHT) * (2 * newRange);
            currentFrac.x_center = newCenterX;
            currentFrac.y_center = newCenterY;
            currentFrac.range = newRange;
            SDL_UnlockMutex(fracMutex);
        }
        
        SDL_Delay(10);
    }
    
    SDL_WaitThread(thread, NULL);
    SDL_DestroyMutex(fracMutex);
}

