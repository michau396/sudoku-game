/**
 * Deklaracja funkcji rozwiązującej Sudoku przy użyciu algorytmu wyżarzania
 * Funkcja działa na wcześniej wygenerowanej planszy z lukami.
 */

 #ifndef SA_ALGORITHM_H
 #define SA_ALGORITHM_H
 
 /**
  * Rozwiązuje Sudoku przy użyciu algorytmu wyżarzania
  * 
  * @param initial - wskaźnik na dwuwymiarową tablicę z początkową planszą Sudoku (z lukami)
  * @param fixed - wskaźnik na dwuwymiarową tablicę oznaczającą, które pola są stałe 
  * @param size - rozmiar planszy Sudoku
  * @param T_start - temperatura początkowa
  * @param T_end - temperatura końcowa (końcowy warunek zatrzymania)
  * @param alpha - współczynnik zmniejszania temperatury
  * @param max_iterations - maksymalna liczba iteracji algorytmu
  */
 void solve_sudoku_sa(int **initial, int **fixed, int size, double T_start, double T_end, double alpha, int max_iterations);
 
 #endif
 
