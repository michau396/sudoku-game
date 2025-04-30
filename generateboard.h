/**
 * Deklaracja funkcji generującej poprawną planszę Sudoku 
 * o podanym rozmiarze (4x4, 9x9, 16x16)
 */
#ifndef GENERATEBOARD_H
#define GENERATEBOARD_H

/**
 * Generuje planszę Sudoku wypełnioną zgodnie z zasadami
 * @param board - tablica 2D do wypełnienia (musi być wcześniej zaalokowana)
 * @param size - rozmiar planszy (4, 9 lub 16)
 */
void generateBoard(int **board, int size);

#endif

