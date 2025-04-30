## Sudoku Generator & Solver

Program generuje plansze Sudoku w trzech rozmiarach: 4x4, 9x9, 16x16
Po wygenerowaniu pełnego rozwiązania, program usuwa część pól zgodnie z wybranym poziomem trudności, tworząc grę do rozwiązania.

## Kompilacja i uruchomienie

gcc main.c generateboard.c -o sudoku
./sudoku

## Znany problem
W przypadku planszy 4x4 czasami liczba usuniętych pól może być większa niż wybrana liczba.
Obecnie próbuje znaleźć przyczynę i naprawić błąd.