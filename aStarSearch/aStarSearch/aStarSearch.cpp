// aStarSearch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <math.h>
#define BOARDSIZE 10

#define EMPTY 0
#define MARKED 1
#define NEXT 2
#define START 3
#define END 4
#define WALL -1
#define SUCCESS 10

typedef struct grid{
    float Hval; //
    float Gval;
    int parentX;
    int parentY;
    int NaviState; // 0 empty, 1 marked, 2 next,3 start, 4 end, -1 wall
}grid;

typedef struct XYpos {
    int x;
    int y;
} pos;

int min(int a, int b) {
    return a < b ? a : b;
}

float CalcHval(int startX, int startY, int destX, int destY) {
    int distanceX = abs(destX - startX), distanceY = abs(destY - startY);
    return distanceX + distanceY + (sqrt(2) - (float)2) * min(distanceX, distanceY);
}

int MarkAvail(grid board[BOARDSIZE][BOARDSIZE], int myX, int myY, int destX, int destY) {
    int x = myX-1;
    int y = myY-1;
    int boxX = x + 3;
    int boxY = y + 3;
    for (y = myY - 1 ; y < boxY; y++) {
        for (x = myX-1; x < boxX; x++) {
            if (x >= 0 && y >= 0) {
                if (board[y][x].NaviState == EMPTY || board[y][x].NaviState == END) {
                    board[y][x].NaviState = NEXT;
                    board[y][x].parentX = myX;
                    board[y][x].parentY = myY;
                    board[y][x].Hval = CalcHval(x, y, destX, destY);
                    if (x == myX || y == myY) { //adds on how much it moved from previous point
                        board[y][x].Gval = board[myY][myX].Gval + 1;
                    }
                    else {
                        board[y][x].Gval = board[myY][myX].Gval + sqrt(2);
                    }
                }
            }
        }
    }
    return 0;
}

/*
int GetNextHval(grid board[BOARDSIZE][BOARDSIZE], int myX, int myY, int destX, int destY) {
    myX--; //go to top left of 3x3 grid
    myY--;
    for (myY = 0; myY < 3; myY++) {
        for (myX = 0; myX < 3; myX++) {
            if (myX >= 0 && myY >= 0) {
                if (board[myY][myX].NaviState == NEXT) {
                    board[myY][myX].Hval = CalcHval(myX, myY, destX, destY);
                }
            }
        }
    }
    return 0;
}
*/

int* findFmin(grid board[BOARDSIZE][BOARDSIZE]) {
    float FvalSave = 0;
    char first = true;
    int Psave[2] = { 0,0 };
    for(int y = 0; y < BOARDSIZE; y++){
        for (int x = 0; x < BOARDSIZE; x++) {
            if (board[y][x].NaviState == NEXT) {
                if (first) {
                    Psave[1] = x;
                    Psave[0] = y;
                    FvalSave = board[y][x].Gval + board[y][x].Hval;
                    first = false;
                }else if (round((board[y][x].Gval + board[y][x].Hval)*100)/100 < round(FvalSave*100)/100) {
                    Psave[1] = x;
                    Psave[0] = y; 
                    FvalSave = board[y][x].Gval + board[y][x].Hval;
                }
            }
        }     
    }
    return Psave;
}

void printBoard(grid board[BOARDSIZE][BOARDSIZE]) {
    std::cout << "   ";
    int i = 0;
    for (i = 0; i < BOARDSIZE; i++) {
        std::cout << "|" << i+1;
    }
    std::cout << "\n";
    for (int y = 0; y < BOARDSIZE; y++) {
        std::cout << " " << y+1 << ":";
        for (int x = 0; x < BOARDSIZE; x++) {
            std::cout << " ";
            switch (board[y][x].NaviState) {
                case EMPTY:
                    std::cout << "_";
                    break;
                case NEXT:
                    std::cout << "n";
                    break;
                case START:
                    std::cout << "@";
                    break;
                case END:
                    std::cout << "^";
                    break;
                case MARKED:
                    std::cout << "m";
                    break;
                case WALL:
                    std::cout << "#";
                    break;
                case SUCCESS:
                    std::cout << "O";
                    break;
                default:
                    break;
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

//h = D * (dx + dy) + (D2 - 2 * D) * min(dx, dy)
int main()
{
    grid board[BOARDSIZE][BOARDSIZE];
    memset(board, EMPTY, sizeof(board)); //wet whole board as empty
    int StartingX, StartingY;
    int DestinationX, DestinationY;
    int wallX, wallY;
    std::cout << "Enter you starting point:";
    std::cin >> StartingX >> StartingY;
    std::cout << "Enter you destination";
    std::cin >> DestinationX >> DestinationY;
    StartingX--;
    StartingY--;
    DestinationX--;
    DestinationY--;
    board[StartingY][StartingX].NaviState = START;
    int originX = StartingX, originY = StartingY;
    board[DestinationY][DestinationX].NaviState = END;
    while (true) {
        std::cout << "Place Wall";
        std::cin >> wallX >> wallY;
        wallX--;
        wallY--;
        if (wallX == -1) {
            break;
        }
        else {
            board[wallY][wallX].NaviState = WALL;
        }
        printBoard(board);
    }
    while (StartingX != DestinationX || StartingY != DestinationY) {
        MarkAvail(board, StartingX, StartingY, DestinationX, DestinationY);
        printBoard(board);
        StartingY = *(findFmin(board));
        StartingX = *(findFmin(board) + 1);
        board[StartingY][StartingX].NaviState = MARKED;
    }


    int tpx, tpy;
    while (StartingX != originX || StartingY != originY) {
        board[StartingY][StartingX].NaviState = SUCCESS;
        tpx = StartingX;
        tpy = StartingY;
        StartingX = board[tpy][tpx].parentX;
        StartingY = board[tpy][tpx].parentY;
    }
    printBoard(board);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
