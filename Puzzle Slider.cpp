#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cctype>

using namespace std;
const string HIGH_SCORE_FILE = "highscore.txt";

struct Player {
    string name;
    int moves;
    bool quit;
    int board[4][4];
    char moves_history[1000];
    int history_size;
};
int menuChoice() {
    int ch;
    while (true) {
        cout << "-------PUZZLE SLIDER GAME MENU -----\n";
        cout << "1. Display Rules Only\n";
        cout << "2. Play Game Directly Without Rules \n";
        cout << "3. Display Rules then Play Game\n";
        cout << "-----------------------------------\n";
        cout << "Enter your choice (1-3): ";
        cin >> ch;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! Please enter a number between 1 and 3.\n";
            continue;
        }

        if (ch >= 1 && ch <= 3) {
            cin.ignore();
            return ch;
        } else {
            cout << "Invalid choice! Please enter 1, 2, or 3.\n";
        }
    }
}

void getPlayerNames(Player &p1, Player &p2) {
    cout << "Enter name for Player 1: ";
    getline(cin, p1.name);

    cout << "Enter name for Player 2: ";
    getline(cin, p2.name);

    cout << "\nWelcome " << p1.name << " and " << p2.name << "!\n\n";
}

void initBoard(int b[4][4]) {
    int num = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (num <= 15) {
                b[i][j] = num;
                num++;
            } else {
                b[i][j] = 0;
            }
        }
    }
}

void shuffleBoard(int b[4][4]) {
    int blankX = 3, blankY = 3;

    char moves[] = {'u', 'd', 'l', 'r'};
    int totalMoves = 200;

    srand(time(0));

    for (int i = 0; i < totalMoves; i++) {
        char m = moves[rand() % 4];

        int newX = blankX;
        int newY = blankY;

        if (m == 'u') newX--;
        else if (m == 'd') newX++;
        else if (m == 'l') newY--;
        else if (m == 'r') newY++;

        if (newX >= 0 && newX < 4 && newY >= 0 && newY < 4) {
            swap(b[blankX][blankY], b[newX][newY]);
            blankX = newX;
            blankY = newY;
        }
    }
}


void printBoard(int b[4][4]) {
    cout << "+----+----+----+----+\n";
    for (int i = 0; i < 4; i++) {
        cout << "|";
        for (int j = 0; j < 4; j++) {
            if (b[i][j] == 0) {
                cout << "    |";
            } else {
                if (b[i][j] < 10)
                    cout << " " << b[i][j] << "  |";
                else
                    cout << b[i][j] << "  |";
            }
        }
        cout << "\n+----+----+----+----+\n";
    }
}

void findBlank(int b[4][4], int &x, int &y) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (b[i][j] == 0) {
                x = i;
                y = j;
                return;
            }
        }
    }
}

bool moveBlank(int b[4][4], char dir) {
    int x, y;
    findBlank(b, x, y);

    int newX = x;
    int newY = y;

    if (dir == 'u') newX--; // Move up
    else if (dir == 'd') newX++; // Move down
    else if (dir == 'l') newY--; // Move left
    else if (dir == 'r') newY++; // Move right
    else return false;

    if (newX >= 0 && newX < 4 && newY >= 0 && newY < 4) {
        int temp = b[x][y];
        b[x][y] = b[newX][newY];
        b[newX][newY] = temp;
        return true;
    }

    return false;
}

bool isSolved(int b[4][4]) {
    int n = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == 3 && j == 3) {
                if (b[i][j] != 0) return false;
            } else {
                if (b[i][j] != n) return false;
                n++;
            }
        }
    }
    return true;
}

void showSummary(Player p) {
    cout << "\n--- " << p.name << " SUMMARY ---\n";
    cout << "Moves: " << p.moves << "\n";
    cout << "History: ";
    for (int i = 0; i < p.history_size; i++) {
        cout << p.moves_history[i] << " ";
    }
    cout << "\n------------------------\n";
}
int loadHighScore() {
      fstream file;
    int bestScore = -1;
    ifstream in(HIGH_SCORE_FILE);
     file.open(HIGH_SCORE_FILE, ios::in);

    if (file.is_open()) {
        file >> bestScore;
        file.close();
    }
    return bestScore;
}
void saveNewHighScore(int newBestScore) {
    fstream file;
 file.open(HIGH_SCORE_FILE, ios::out);

    if (file.is_open()) {
        file << newBestScore << endl;
        file.close();
        cout << "\n--- NEW HIGH SCORE SAVED: " << newBestScore << " moves! ---\n";
    } else {
        cout << "\nError: Could not save high score to file.\n";
    }
}


void checkAndSaveHighScore(int finalMoves) {
    int currentHighScore = loadHighScore();
    if (currentHighScore == -1 || finalMoves < currentHighScore) {
        saveNewHighScore(finalMoves);
    } else {
        cout << "\nCurrent high score is: " << currentHighScore << " moves. You did not beat it.\n";
    }
}

void printRules(){
    cout << "---------------- WELCOME TO PUZZLE SLIDER GAME  ----------------\n";
    cout<<  "GAME BY :\n";
    cout<<"HAMNA NOOR \n";
    cout<<"------------------------------------------------------------------\n";
    cout<<"HERE ARE SOME BASIC RULES WHICH HELP YOU TO PLAY THE GAME\n";
    cout<<"------------------------------------------------------------------\n";
    cout<<"1.The player wins when the numbers 1 to 15 are arranged in order from left to right\n";
    cout<<"           top to bottom, and the empty space is at the end.\n";
    cout << "2.u=UP d=DOWN l=LEFT r=RIGHT q=QUIT (to skip turn)\n";
    cout << "3.Each player has their own puzzle.\n";
    cout << "4.Whoever solves first wins, or the one with fewer moves wins if both solve.\n";
    cout << "5.Best score (fewest moves to solve by any player) will be saved \n";
    cout << "-----------------------------------------------------------------------\n";
}

 int main() {
    srand(time(0));

    while (true) {
        int choice = menuChoice();
        if (choice == 1) {
            printRules();
            break;
        }

        Player p1 = {"Player 1", 0, false, {}, {}, 0};
        Player p2 = {"Player 2", 0, false, {}, {}, 0};

        getPlayerNames(p1, p2);
        if (choice == 3) {
            printRules();
        }

        initBoard(p1.board);
        shuffleBoard(p1.board);
        initBoard(p2.board);
        shuffleBoard(p2.board);
        p1.moves = p2.moves = 0;
        p1.history_size = p2.history_size = 0;

        int currentHighScore = loadHighScore();
        if (currentHighScore != -1) {
            cout << "Current Best Score: " << currentHighScore << " moves.\n";
        } else {
            cout << "No High Score recorded yet.\n";
        }
        for (int turn = 1; turn <= 2; turn++) {
            Player& currentPlayer = (turn == 1) ? p1 : p2;
            if (isSolved(currentPlayer.board)) {
                cout << "\n" << currentPlayer.name << " already solved the puzzle in a previous round! Skipping turn.\n";
                continue;
            }

            cout << "\n--- " << currentPlayer.name << " TURN ---\n";

            while (true) {
                cout << "Moves: " << currentPlayer.moves << " History: ";
                for (int i = 0; i < currentPlayer.history_size; i++) {
                    cout << currentPlayer.moves_history[i] << " ";
                }
                cout << "\n";
                printBoard(currentPlayer.board);
                cout << "Move (u/d/l/r/q): ";
                char m;
                cin >> m;
                m = tolower(m);

                if (m == 'q') {
                    currentPlayer.quit = true;
                    cout << currentPlayer.name << " chose to skip the rest of their turn.\n";
                    break;
                }

                if (moveBlank(currentPlayer.board, m)) {
                    currentPlayer.moves++;
                    if (currentPlayer.history_size < 1000) {
                        currentPlayer.moves_history[currentPlayer.history_size++] = m;
                    }

                    if (isSolved(currentPlayer.board)) {
                        cout << "\n*** " << currentPlayer.name << " SOLVED PUZZLE! ***\n";
                        printBoard(currentPlayer.board);
                        break;
                    }
                } else {
                    cout << "Invalid move! Try again.\n";
                }
            }
        }
        cout << "\n--- GAME RESULTS ---\n";
        showSummary(p1);
        showSummary(p2);

        bool p1Solved = isSolved(p1.board);
        bool p2Solved = isSolved(p2.board);
        int winningMoves = -1;

        if (p1Solved && p2Solved) {
            if (p1.moves < p2.moves) {
                cout << "WINNER: " << p1.name << " (fewer moves)\n";
                winningMoves = p1.moves;
            }
            else if (p2.moves < p1.moves) {
                cout << "WINNER: " << p2.name << " (fewer moves)\n";
                winningMoves = p2.moves;
            }
            else {
                cout << "IT'S A TIE! (both solved with " << p1.moves << " moves)\n";
                winningMoves = p1.moves;
            }
        } else if (p1Solved) {
            cout << "WINNER: " << p1.name << " (solved first)\n";
            winningMoves = p1.moves;
        } else if (p2Solved) {
            cout << "WINNER: " << p2.name << " (solved first)\n";
            winningMoves = p2.moves;
        } else if (p1.quit && p2.quit) {
            cout << "Both players skipped/quit. No winner.\n";
        } else {
            cout << "No player solved the puzzle.\n";
        }

        if (winningMoves != -1) {
            checkAndSaveHighScore(winningMoves);
        }

        cout << "\nStart NEW game? (y/n): ";
        char again;
        cin >> again;
        cin.ignore();
        if (again == 'n' || again == 'N') break;
    }

    return 0;
}



