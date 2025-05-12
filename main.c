#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "generateboard.h"
#include "SA_algorithm.h"

#define MAX_SIZE 16 // Maksymalny rozmiar planszy
//globalne zmienne przechowujace stan gry
int **board; // Aktualna plansza gry
int **solution; //pełne rozwiązanie planszy
int **isFixed; // znacznik czy pole jest stałe
int size; //Rozmiar planszy (4, 9 lub 16)

/**
 * Alokuje pamięć dla dynamicznej tablicy 2D
 * @param size - rozmiar planszy
 * @return wskaźnik do zaalokowanej tablicy
 */
int **allocateBoard(int size) {
    int **grid = malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++)
        grid[i] = calloc(size, sizeof(int));
    return grid;
}

/**
 * Zwalnia pamięć zajmowaną przez tablicę 2D
 * @param grid - tablica do zwolnienia
 * @param size - rozmiar tablicy
 */
void freeBoard(int **grid, int size) {
    for (int i = 0; i < size; i++)
        free(grid[i]);
    free(grid);
}

/**
 * Kopiuje zawartość jednej tablicy do drugiej
 * @param src - tablica źródłowa
 * @param dest - tablica docelowa
 */
void copyBoard(int **src, int **dest) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            dest[i][j] = src[i][j];
}

/**
 * Usuwa określoną liczbę pól z planszy
 * @param num - liczba pól do usunięcia
 */
void removeCells(int num) {
    int removed = 0;
    int attempts = 0;
    int maxAttempts = size * size * 2; // Ograniczenie liczby prób
    // usuwanie w losowych miejscach
    while (removed < num && attempts < maxAttempts) {
        int r = rand() % size;
        int c = rand() % size;
        if (board[r][c] != 0) {
            board[r][c] = 0;
            isFixed[r][c] = 0;
            removed++;
        }
        attempts++;
    }
    
    if (removed < num) {
        // Jeśli nie udało się usunąć wystarczającej liczby komórek,
        // przejdź przez planszę i usuń pozostałe
        for (int i = 0; i < size && removed < num; i++) {
            for (int j = 0; j < size && removed < num; j++) {
                if (board[i][j] != 0) {
                    board[i][j] = 0;
                    isFixed[i][j] = 0;
                    removed++;
                }
            }
        }
    }
}

/**
 * Wyświetla aktualny stan planszy w terminalu
 * z kolorowym oznaczeniem wierszy i kolumn
 */
void printBoard() {
    printf("   ");
    for (int i = 0; i < size; i++) printf("\033[1;32m%2d \033[0m", i + 1);
    printf("\n");

    for (int i = 0; i < size; i++) {
        printf("\033[1;32m%2d \033[0m", i + 1);
        for (int j = 0; j < size; j++) {
            if (board[i][j] == 0)
                printf(" . ");
            else
                printf("%2d ", board[i][j]);
        }
        printf("\n");
    }
}

/**
 * Sprawdza czy wszystkie pola są wypełnione
 * @return 1 jeśli plansza jest pełna, 0 w przeciwnym wypadku
 */
int isFinished() {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (board[i][j] == 0)
                return 0;
    return 1;
}

/**
 * Wyświetla instrukcję gry
 */
void showInstructions() {
    printf("===== Instrukcja gry w Sudoku =====\n");
    printf("1. Celem gry jest wypełnienie planszy liczbami od 1 do wybranego rozmiaru,\n");
    printf("   tak aby każda liczba występowała tylko raz w każdym wierszu, kolumnie i w każdym kwadracie √rozmiarx√rozmiar.\n\n");
    
    printf("2. Gra będzie wyświetlała planszę z brakującymi liczbami, które należy uzupełnić.\n");
    printf("   W zależności od tego ile luk zostało wybranych, tyle pol trzeba bedzie uzupełnić\n\n");

    printf("3. Twoim zadaniem jest wprowadzenie liczby w odpowiednią komórkę.\n");
    printf("   Aby to zrobić, podaj: wiersz, kolumnę i liczbę.\n");
    printf("   Na przykład, aby wpisać 5 do komórki w 2. wierszu i 3. kolumnie, wpisz: 2 3 5.\n\n");

    printf("4. Możesz także usunąć wpis (wprowadź 0 zamiast liczby).\n");
    printf("   Na przykład, aby usunąć liczbę z komórki 2, 3, wpisz: 2 3 0.\n\n");

    printf("5. Jeśli chcesz zakończyć grę, wpisz 0 0 0 i naciśnij Enter.\n\n");

    printf("6. Pamiętaj, że tylko niektóre komórki są stałe (ich wartości są ustalone na początku)\n");
    printf("   i nie możesz ich zmieniać.\n\n");

    printf("Powodzenia!\n");
    printf("=====================================\n\n");
}

/**
 * Główna pętla gry - obsługuje interakcję z użytkownikiem
 */
void playGame() {
    int row, col, val;
    char input[100];
    time_t startTime = time(NULL);
    int moveCount = 0;
    while (1) {
        printBoard();
        if (size == 9) {
            char choice;
            printf("Czy chcesz wykorzystać algorytm SA do rozwiązania Sudoku? (T/N): ");
            scanf(" %c", &choice);  
            while(getchar() != '\n'); // Clear input buffer
        
            if (choice == 'T' || choice == 't') {
                printf("Uruchamianie algorytmu wyżarzania...\n");
                double T_start = 5.0;
                double T_end = 1e-3;
                double alpha = 0.999;
                int max_iterations = 1000000;
                
                // Make a copy of isFixed for SA algorithm
                int **fixed_copy = allocateBoard(size);
                for (int i = 0; i < size; i++) {
                    memcpy(fixed_copy[i], isFixed[i], size * sizeof(int));
                }
                
                solve_sudoku_sa(board, fixed_copy, size, T_start, T_end, alpha, max_iterations);
                
                // Update isFixed based on solution
                for (int i = 0; i < size; i++) {
                    for (int j = 0; j < size; j++) {
                        if (board[i][j] != 0) {
                            isFixed[i][j] = 1;
                        }
                    }
                }
                
                freeBoard(fixed_copy, size);
                printBoard();
                exit(0);
            }
        }
        
        printf("\nPodaj: wiersz kolumna wartość (1-%d)\n", size);
        printf("Użyj wartości 0, aby usunąć wpis. Wpisz 0 0 0, aby zakończyć grę.\n");
        printf("Twój ruch: ");
        
        // Najpierw wczytaj całą linię
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Błąd odczytu. Spróbuj ponownie.\n");
            continue;
        }
        
        // Następnie parsuj dane
        if (sscanf(input, "%d %d %d", &row, &col, &val) != 3) {
            printf("Nieprawidłowy format. Wprowadź 3 liczby oddzielone spacjami.\n\n");
            continue;
        }

        // Reszta kodu pozostaje bez zmian
        if (row == 0 && col == 0 && val == 0) {
            printf("Gra zakończona. Do zobaczenia!\n");
            break;
        }
        // Walidacja zakresów
        if (row < 1 || row > size || col < 1 || col > size || val < 0 || val > size) {
            printf("Nieprawidłowe dane. Spróbuj ponownie.\n\n");
            continue;
        }

        row--; col--; // Konwersja na indeksy od 0

        // Sprawdzenie czy pole jest stałe
        if (isFixed[row][col]) {
            printf("Nie można zmienić tej komórki - to pole jest stałe.\n\n");
            moveCount++;
            continue;
        }

        // Obsługa usuwania wartości
        if (val == 0) {
            if (board[row][col] == 0) {
                printf("To pole jest już puste.\n\n");
                moveCount++;
            } else {
                board[row][col] = 0;
                printf("Ruch został usunięty.\n\n");
            }
            continue;
        }

        // Weryfikacja ruchu
        if (solution[row][col] == val) {
            board[row][col] = val;
            printf("Poprawny ruch!\n\n");
            moveCount++;
        } else {
            printf("Błąd! Ta liczba nie pasuje w tym miejscu.\n\n");
            moveCount++;
        }

        // Sprawdzenie ukończenia gry
        if (isFinished()) {
            time_t endTime = time(NULL);
            int duration = (int)(endTime - startTime);
            int minutes = duration / 60;
            int seconds = duration % 60;
            printf("Gratulacje! Sudoku zostało rozwiązane!\n");
            printf("Czas gry: %d minut i %d sekund.\n", minutes, seconds);
            printf("Ilość wykonanych prob: %d\n", moveCount);
            printBoard();
            break;
        }
    }
}

/**
 * Główna funkcja programu
 */
int main() {
    srand(time(NULL));
    showInstructions();
    // Wybór rozmiaru planszy
    printf("Wybierz rozmiar planszy (4, 9, 16): ");
    scanf("%d", &size);
    if (size != 4 && size != 9 && size != 16) {
        printf("Nieobsługiwany rozmiar. Dostępne: 4, 9, 16\n");
        return 1;
    }
    // Wybór trudności
    int difficulty;
        while (1) {
            printf("Wybierz poziom trudności (liczbę pustych pol od 1 - %d): ", size * size - 1);
    
        if (scanf("%d", &difficulty) != 1) {  // Jeśli scanf nie wczyta liczby
            printf("Nieprawidłowe dane. Wprowadź liczbę.\n");
            while (getchar() != '\n');  // Wyczyść bufer wejścia
            continue;
        }
        else {
            if (difficulty < 1) difficulty = 1;
            if (difficulty > size*size - 1) difficulty = size*size - 1;
            while (getchar() != '\n');
            break;
        }
        }

    // Inicjalizacja plansz
    board = allocateBoard(size);
    solution = allocateBoard(size);
    isFixed = allocateBoard(size);

    // Generuj planszę w pętli aż będzie poprawna
    int valid = 0;
        while (!valid) {
            generateBoard(solution, size);
            valid = 1;
            
            // Dodatkowa weryfikacja dla 4x4
            if (size == 4) {
                for (int i = 0; i < size && valid; i++) {
                    for (int j = 0; j < size && valid; j++) {
                        if (solution[i][j] == 0) {
                            valid = 0;
                        }
                    }
                }
            }
        }

    copyBoard(solution, board);
     // Oznaczenie stałych pól
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            isFixed[i][j] = 1;
    removeCells(difficulty);// Usunięcie części pól

    playGame();// Rozpoczęcie gry
        // Zwolnienie pamięci

    freeBoard(board, size);
    freeBoard(solution, size);
    freeBoard(isFixed, size);
    return 0;
}





