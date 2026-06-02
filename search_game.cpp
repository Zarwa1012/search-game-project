#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

//  Constants 
const int ROWS = 15;
const int COLS = 15;
const int WORD_COUNT = 5; 

char grid[ROWS][COLS];
bool found[ROWS][COLS]; 

char wordsToFind[WORD_COUNT][20];
bool wordDiscovered[WORD_COUNT];

int dr[] = { 0,0,1,-1,1,-1,1,-1};
int dc[] = { 1,-1,0,0,1,-1,-1,1};


int getLength(char str[]) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

bool inBounds(int r, int c) {
    return r >= 0 && r < ROWS && c >= 0 && c < COLS;
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

//  Grid Generation  Word Placement 

bool placeWord(char word[]) {
    int wordLen = getLength(word);
    
    for (int attempt = 0; attempt < 100; attempt++) {
        int dir = rand() % 8;
        int r   = rand() % ROWS;
        int c   = rand() % COLS;

        bool fits = true;
        for (int i = 0; i < wordLen; i++) {
            int nr = r + dr[dir] * i;
            int nc = c + dc[dir] * i;
            
            if (!inBounds(nr, nc)) { fits = false; break; }
            if (grid[nr][nc] != '.' && grid[nr][nc] != word[i]) { fits = false; break; }
        }
        
        if (!fits) continue;

        for (int i = 0; i < wordLen; i++) {
            grid[r + dr[dir] * i][c + dc[dir] * i] = word[i];
        }
        return true;
    }
    return false;
}

void buildGrid() {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            grid[r][c] = '.';
            found[r][c] = false;
        }
    }

    for (int i = 0; i < WORD_COUNT; i++) {
        placeWord(wordsToFind[i]);
    }

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (grid[r][c] == '.') {
                grid[r][c] = 'A' + rand() % 26;
            }
        }
    }
}

//   Display game grid 

void printGrid() {
    // Top column numbers alignment
    cout << "     "; 
    for (int c = 0; c < COLS; c++) {
        if (c + 1 < 10) cout << " " << c + 1 << " ";
        else cout << " " << c + 1;
    }
    cout << "\n    +---------------------------------------------\n";

    // Rows alignment loop
    for (int r = 0; r < ROWS; r++) {
        if (r + 1 < 10) cout << " " << r + 1 << "  |";
        else cout << " " << r + 1 << " |";

        for (int c = 0; c < COLS; c++) {
            if (found[r][c]) {
                cout << "*" << grid[r][c] << "*";
            } else {
                cout << " " << grid[r][c] << " ";
            }
        }
        cout << "\n";
    }
    cout << "    +---------------------------------------------\n";
}

void printWordList() {
    cout << "\n--- Words to Find ---\n";
    for (int i = 0; i < WORD_COUNT; i++) {
        if (wordDiscovered[i]) {
            cout << "[FOUND] " << wordsToFind[i] << "\n";
        } else {
            cout << "  " << wordsToFind[i] << "\n";
        }
    }
    cout << "---------------------\n";
}

// Word Checking Logic
bool checkCoordinates(int startR, int startC, int endR, int endC, char word[], bool markFound) {
    int wordLen = getLength(word);
    int diffR = endR - startR;
    int diffC = endC - startC;

    int stepR = (diffR == 0) ? 0 : (diffR > 0 ? 1 : -1);
    int stepC = (diffC == 0) ? 0 : (diffC > 0 ? 1 : -1);

    int absR = diffR < 0 ? -diffR : diffR;
    int absC = diffC < 0 ? -diffC : diffC;

    if (diffR != 0 && absR != wordLen - 1) return false;
    if (diffC != 0 && absC != wordLen - 1) return false;

    for (int i = 0; i < wordLen; i++) {
        int nr = startR + stepR * i;
        int nc = startC + stepC * i;
        
        if (!inBounds(nr, nc) || grid[nr][nc] != word[i]) {
            return false;
        }
    }

    if (markFound) {
        for (int i = 0; i < wordLen; i++) {
            found[startR + stepR * i][startC + stepC * i] = true;
        }
    }
    return true;
}

//Category Selection 

void loadCategory(int choice) {
    char animals[5][10] = {"LION", "TIGER", "BEAR", "WOLF", "GOAT"};
    char countries[5][10] = {"CHINA", "INDIA", "JAPAN", "ITALY", "SPAIN"};
    char fruits[5][10] = {"APPLE", "MANGO", "GRAPE", "PEACH", "LEMON"};
    char tech[5][10] = {"CODE", "DATA", "DISK", "FILE", "BYTE"};

    for (int i = 0; i < 5; i++) {
        wordDiscovered[i] = false;
        for (int j = 0; j < 10; j++) {
            if (choice == 1) wordsToFind[i][j] = animals[i][j];
            else if (choice == 2) wordsToFind[i][j] = countries[i][j];
            else if (choice == 3) wordsToFind[i][j] = fruits[i][j];
            else wordsToFind[i][j] = tech[i][j];
        }
    }
}

//  Main Game Loop 

void playGame(int category) {
    loadCategory(category);
    buildGrid();

    int totalFound = 0;

    while (totalFound < WORD_COUNT) {
        clearScreen();
        cout << "=== WORD SEARCH GAME ===\n\n";
        printGrid();
        printWordList();

        cout << "Input Format: StartRow StartCol EndRow EndCol (e.g., 3 5 3 8)\n";
        cout << "Enter 0 as the first number to quit: ";
        
        int sr, sc, er, ec;
        cin >> sr;
        if (sr == 0) break;
        cin >> sc >> er >> ec;

        sr--; sc--; er--; ec--;

        if (!inBounds(sr, sc) || !inBounds(er, ec)) {
            cout << "\n[!] Coordinates out of range! Press any key to continue...";
            cin.ignore(); cin.get();
            continue;
        }

        bool matched = false;
        for (int i = 0; i < WORD_COUNT; i++) {
            if (wordDiscovered[i]) continue;

            if (checkCoordinates(sr, sc, er, ec, wordsToFind[i], true) || 
                checkCoordinates(er, ec, sr, sc, wordsToFind[i], true)) {
                
                wordDiscovered[i] = true;
                totalFound++;
                cout << "\n[+] Correct! You found \"" << wordsToFind[i] << "\"!\n";
                matched = true;
                cin.ignore(); cin.get();
                break;
            }
        }

        if (!matched) {
            cout << "\n[-] No word found at those coordinates. Try again!";
            cin.ignore(); cin.get();
        }
    }

    if (totalFound == WORD_COUNT) {
        cout << "\n*** CONGRATULATIONS! You found all the words! ***\n";
    } else {
        cout << "\nGame Over! You found " << totalFound << " word(s).\n";
    }
    cin.ignore(); cin.get();
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    while (true) {
        clearScreen();
        cout << "=== MAIN MENU ===\n";
        cout << "1. Animals\n";
        cout << "2. Countries\n";
        cout << "3. Fruits\n";
        cout << "4. Technology\n";
        cout << "5. Quit\n";
        cout << "-----------------\n";
        cout << "Choose a category (1-5): ";

        int choice;
        cin >> choice;

        if (choice == 5) {
            cout << "\nThank you for playing! Goodbye.\n";
            break;
        }
        if (choice >= 1 && choice <= 4) {
            playGame(choice);
        }
    }
    return 0;
}
