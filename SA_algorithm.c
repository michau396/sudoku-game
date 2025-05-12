#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "SA_algorithm.h"

typedef struct {
    int **grid; // aktualna plansza Sudoku
    int energy; // energia - ilosc konfliktów
    int size; // rozmiar planszy
    int box_size; // rozmiar pojedynczego podkwadratu
} SudokuState;

/**
 * Funkcja oblicza energię planszy Sudoku.
 * Energia to liczba powtórzeń cyfr w wierszach, kolumnach i blokach.
 * @param grid - tablica Sudoku
 * @param size - rozmiar planszy
 * @return wartość energii 
 */
int calculate_energy(int **grid, int size) {
    int energy = 0;
    int box_size = (int)sqrt(size);

    // Sprawdzenie wierszy i kolumn
    for (int i = 0; i < size; i++) {
        int *row_count = calloc(size + 1, sizeof(int));
        int *col_count = calloc(size + 1, sizeof(int));
        
        for (int j = 0; j < size; j++) {
            if (grid[i][j] != 0) row_count[grid[i][j]]++;
            if (grid[j][i] != 0) col_count[grid[j][i]]++;
        }
        // Zliczanie powtórzeń
        for (int k = 1; k <= size; k++) {
            if (row_count[k] > 1) energy += row_count[k] - 1;
            if (col_count[k] > 1) energy += col_count[k] - 1;
        }

        free(row_count);
        free(col_count);
    }

    // Sprawdzenie bloków
    for (int block = 0; block < size; block++) {
        int *block_count = calloc(size + 1, sizeof(int));
        int row_start = (block / box_size) * box_size;
        int col_start = (block % box_size) * box_size;

        for (int i = 0; i < box_size; i++) {
            for (int j = 0; j < box_size; j++) {
                int val = grid[row_start + i][col_start + j];
                if (val != 0) block_count[val]++;
            }
        }

        for (int k = 1; k <= size; k++) {
            if (block_count[k] > 1) energy += block_count[k] - 1;
        }

        free(block_count);
    }

    return energy;
}

/**
 * Funkcja inicjalizuje stan Sudoku.
 * Wypełnia każdą wolną komórkę w bloku losową wartością z dozwolonych.
 * @param state - struktura stanu Sudoku
 * @param initial - początkowa plansza
 * @param fixed - tablica oznaczająca stałe liczby
 * @param size - rozmiar planszy
 */
void initialize_state(SudokuState *state, int **initial, int **fixed, int size) {
    state->size = size;
    state->box_size = (int)sqrt(size);
    state->grid = malloc(size * sizeof(int *));
    
    for (int i = 0; i < size; i++) {
        state->grid[i] = malloc(size * sizeof(int));
        memcpy(state->grid[i], initial[i], size * sizeof(int));
    }
    // Dla każdego bloku wypełnij brakujące liczby
    for (int block = 0; block < size; block++) {
        int *used = calloc(size + 1, sizeof(int));
        int cells[size][2];
        int count = 0;

        int row_start = (block / state->box_size) * state->box_size;
        int col_start = (block % state->box_size) * state->box_size;
        // Zaznacz już użyte liczby
        for (int i = 0; i < state->box_size; i++) {
            for (int j = 0; j < state->box_size; j++) {
                int r = row_start + i, c = col_start + j;
                if (fixed[r][c]) {
                    used[initial[r][c]] = 1;
                }
            }
        }
        // Losowo umieść brakujące liczby w pustych komórkach
        for (int num = 1; num <= size; num++) {
            if (!used[num]) {
                while (1) {
                    int r = row_start + rand() % state->box_size;
                    int c = col_start + rand() % state->box_size;
                    if (!fixed[r][c] && state->grid[r][c] == 0) {
                        state->grid[r][c] = num;
                        break;
                    }
                }
            }
        }
        free(used);
    }

    state->energy = calculate_energy(state->grid, size);
}

/**
 * Funkcja kopiuje stan Sudoku.
 * @param dest - miejsce docelowe
 * @param src - źródło
 */
void copy_state(SudokuState *dest, const SudokuState *src) {
    dest->size = src->size;
    dest->box_size = src->box_size;
    dest->energy = src->energy;
    
    for (int i = 0; i < src->size; i++) {
        memcpy(dest->grid[i], src->grid[i], src->size * sizeof(int));
    }
}

/**
 * Funkcja generuje sąsiada poprzez zamianę dwóch zmiennych komórek w tym samym bloku.
 * @param neighbor - nowy stan
 * @param current - obecny stan
 * @param fixed - tablica stałych wartości
 */
void generate_neighbor(SudokuState *neighbor, const SudokuState *current, int **fixed) {
    copy_state(neighbor, current);

    int block = rand() % current->size;
    int r0 = (block / current->box_size) * current->box_size;
    int c0 = (block % current->box_size) * current->box_size;

    int candidates[current->size][2];
    int count = 0;
    // Zbierz współrzędne niezamrożonych komórek w bloku
    for (int i = 0; i < current->box_size; i++) {
        for (int j = 0; j < current->box_size; j++) {
            int r = r0 + i, c = c0 + j;
            if (!fixed[r][c]) {
                candidates[count][0] = r;
                candidates[count][1] = c;
                count++;
            }
        }
    }
    // Jeśli są przynajmniej dwie zmienne komórki, zamień je miejscami
    if (count >= 2) {
        int a = rand() % count, b;
        do {
            b = rand() % count;
        } while (a == b);

        int r1 = candidates[a][0], c1 = candidates[a][1];
        int r2 = candidates[b][0], c2 = candidates[b][1];
        // Zamiana dwóch wartości
        int tmp = neighbor->grid[r1][c1];
        neighbor->grid[r1][c1] = neighbor->grid[r2][c2];
        neighbor->grid[r2][c2] = tmp;
        // Oblicz nową energię
        neighbor->energy = calculate_energy(neighbor->grid, neighbor->size);
    }
}

/**
 * Funkcja zwalnia pamięć zaalokowaną dla stanu Sudoku.
 * @param state - struktura do zwolnienia
 */
void free_state(SudokuState *state) {
    for (int i = 0; i < state->size; i++) {
        free(state->grid[i]);
    }
    free(state->grid);
}

/**
 * Główna funkcja wykonująca optymalizację symulowanym wyżarzaniem.
 * Szuka stanu planszy o minimalnej energii.
 * @param initial - początkowa plansza
 * @param fixed - tablica stałych wartości
 * @param size - rozmiar planszy
 * @param T_start - temperatura początkowa
 * @param T_end - temperatura końcowa
 * @param alpha - współczynnik chłodzenia
 * @param max_iterations - maksymalna liczba iteracji
 */
void solve_sudoku_sa(int **initial, int **fixed, int size, double T_start, double T_end, double alpha, int max_iterations) {
    srand(time(NULL));

    SudokuState current, neighbor, best;
    
    // Alokacja pamięci
    current.grid = malloc(size * sizeof(int *));
    neighbor.grid = malloc(size * sizeof(int *));
    best.grid = malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        current.grid[i] = malloc(size * sizeof(int));
        neighbor.grid[i] = malloc(size * sizeof(int));
        best.grid[i] = malloc(size * sizeof(int));
    }
    // Inicjalizacja
    initialize_state(&current, initial, fixed, size);
    copy_state(&best, &current);

    double T = T_start;
    int iteration = 0;
    // Pętla SA
    while (T > T_end && best.energy > 0 && iteration < max_iterations) {
        generate_neighbor(&neighbor, &current, fixed);

        int delta = neighbor.energy - current.energy;
        // Warunek akceptacji (przyjmuje gorsze rozwiązanie z pewnym prawdopodobieństwem)
        if (delta < 0 || (exp(-delta / T) > (double)rand() / RAND_MAX)) {
            copy_state(&current, &neighbor);
            if (current.energy < best.energy) {
                copy_state(&best, &current);
            }
        }

        T *= alpha;
        iteration++;

        if (iteration % 100 == 0) {
            printf("Iteracja: %d, T: %.4f, Energia: %d\n", iteration, T, best.energy);
        }
    }

    // Przepisanie najlepszego rozwiązania
    for (int i = 0; i < size; i++) {
        memcpy(initial[i], best.grid[i], size * sizeof(int));
    }

    printf("\nFinalna energia: %d\n", best.energy);
    // Zwolnienie pamięci
    free_state(&current);
    free_state(&neighbor);
    free_state(&best);
}

