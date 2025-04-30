#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "generateboard.h"

/**
 * Funkcja sprawdza czy liczba nie występuje w podkwadracie o rozmiarze boxSize
 * @param grid - tablica Sudoku
 * @param boxSize - rozmiar podkwadratu (sqrt(size))
 * @param rowStart - indeks początkowego wiersza podkwadratu
 * @param colStart - indeks początkowego wiersza kolumny
 * @param num - sprawdzana liczba
 * @return 1 jeśli liczba nie występuje, 0 w przeciwnym wypadku
 */
int unUsedInBox(int **grid, int boxSize, int rowStart, int colStart, int num) {
    for (int i = 0; i < boxSize; i++)
        for (int j = 0; j < boxSize; j++)
            if (grid[rowStart + i][colStart + j] == num)
                return 0;
    return 1;
}

/**
 * Funkcja wypełnia podkwadrat boxSize x boxSize unikalnymi liczbami
 * @param grid - tablica Sudoku
 * @param size - rozmiar całej planszy
 * @param boxSize - rozmiar podkwadratu
 * @param row - indeks wiersza początkowego
 * @param col - indeks kolumny początkowej
 */
void fillBox(int **grid, int size, int boxSize, int row, int col) {
    int num;
    for (int i = 0; i < boxSize; i++) {
        for (int j = 0; j < boxSize; j++) {
            do {
                num = rand() % size + 1; // Losowanie liczby z zakresu 1 do size
            } while (!unUsedInBox(grid, boxSize, row, col, num)); //powtarzaj aż znajdziesz unikalną liczbę
            grid[row + i][col + j] = num;
        }
    }
}
/**
 * Funkcja sprawdza czy liczba nie występuje w wierszu
 * @param grid - tablica Sudoku
 * @param size - rozmiar planszy
 * @param i - indeks wiersza
 * @param num - sprawdzana liczba
 */
int unUsedInRow(int **grid, int size, int i, int num) {
    for (int j = 0; j < size; j++)
        if (grid[i][j] == num)
            return 0;
    return 1;
}

/**
 * Funkcja Sprawdza czy liczba nie występuje w kolumnie
 * @param grid - tablica Sudoku
 * @param size - rozmiar planszy
 * @param j - indeks kolumny
 * @param num - sprawdzana liczba
 */
int unUsedInCol(int **grid, int size, int j, int num) {
    for (int i = 0; i < size; i++)
        if (grid[i][j] == num)
            return 0;
    return 1;
}
/**
 * Funkcja sprawdzająca bezpieczne wstawienie liczby
 * @param grid - tablica Sudoku
 * @param size - rozmiar planszy
 * @param boxSize - rozmiar podkwadratu
 * @param i - indeks wiersza
 * @param j - indeks kolumny
 * @param num - sprawdzana liczba
 */
int checkIfSafe(int **grid, int size, int boxSize, int i, int j, int num) {
    return unUsedInRow(grid, size, i, num) && //sprawdzanie wiersza
           unUsedInCol(grid, size, j, num) &&//sprawdzanie kolumny
           unUsedInBox(grid, boxSize, i - i % boxSize, j - j % boxSize, num);//sprawdzanie podkwadratu
}

/**
 * Funkcja wypełnia podkwadraty na przekątnej planszy
 * @param grid - tablica Sudoku
 * @param size - rozmiar planszy
 * @param boxSize - rozmiar podkwadratu
 */
void fillDiagonal(int **grid, int size, int boxSize) {
    for (int i = 0; i < size; i += boxSize)
        fillBox(grid, size, boxSize, i, i);
}


/**
 * Rekurencyjna funkcja wypełniająca resztę planszy
 * @param grid - tablica Sudoku
 * @param size - rozmiar planszy
 * @param boxSize - rozmiar podkwadratu
 * @param i - indeks wiersza
 * @param j - indeks kolumny
 * @return 1 jeśli udało się wypełnić plansze 0 w przeciwnym wypadku
 */
int fillRemaining(int **grid, int size, int boxSize, int i, int j) {
    if (i == size) //koniec planszy
        return 1;
    if (j == size)
        return fillRemaining(grid, size, boxSize, i + 1, 0);
    if (grid[i][j] != 0)
        return fillRemaining(grid, size, boxSize, i, j + 1);
    //próba wstawienia liczby od 1 do size
    for (int num = 1; num <= size; num++) {
        if (checkIfSafe(grid, size, boxSize, i, j, num)) {
            grid[i][j] = num;
            if (fillRemaining(grid, size, boxSize, i, j + 1))//wypełnianie rekurencyjne
                return 1;
            grid[i][j] = 0;// Backtracking - wycofanie nieudanego wstawienia
        }
    }
    return 0;
}


/**
 * Główna funkcja generujaca planszę Sudoku
 * @param board - wskaźnik do tablicy 2D
 * @param size - rozmiar planszy (4, 9, 16)
 */
void generateBoard(int **board, int size) {
    int boxSize = (int)sqrt(size); //Oblicz rozmiar podkwadratu
    srand(time(NULL)); // Inicjalizacja generatora liczb losowych
    // Inicjalizacja planszy zerami
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            board[i][j] = 0;
    fillDiagonal(board, size, boxSize); // Wypełnij podkwadraty na przekątnej
    fillRemaining(board, size, boxSize, 0, 0); //Wypełnij resztę
}

