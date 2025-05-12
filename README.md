## Sudoku Generator & Solver

Program generuje plansze Sudoku w trzech rozmiarach: 4x4, 9x9, 16x16
Po wygenerowaniu pełnego rozwiązania, program usuwa część pól zgodnie z wybranym poziomem trudności, tworząc grę do rozwiązania. W przypadku planszy 9x9 mozliwe jest rozwiązanie za pomocą algorytmu wyżarzania.

## Kompilacja i uruchomienie

clang main.c generateboard.c SA_algorithm.c -o main
./main

