#pragma once
#include "pch.h"
#include <iostream>
#include <math.h>
#include "aStarSearch.h"

int Min(int a, int b) {
    return a < b ? a : b;
}

float CalcHval(int startX, int startY, int destX, int destY) {
    int distanceX = abs(destX - startX), distanceY = abs(destY - startY);
    return distanceX + distanceY + (sqrt(2) - (float)2) * Min(distanceX, distanceY);
}

int MarkAvail(grid board[BOARDSIZE][BOARDSIZE], pos mypos, pos destpos, pos PrevPathPos) {
    int x = mypos.x - 1;
    int y = mypos.y - 1;
    int boxX = x + 3;
    int boxY = y + 3;
    for (y = mypos.y - 1; y < boxY; y++) {
        for (x = mypos.x - 1; x < boxX; x++) {
            if (x >= 0 && y >= 0 && x<BOARDSIZE && y<BOARDSIZE) {
                if (board[y][x].NaviState == EMPTY || board[y][x].NaviState == END) {
                    board[y][x].NaviState = NEXT;
                    board[y][x].parentX = mypos.x;
                    board[y][x].parentY = mypos.y;
                    board[y][x].Hval = CalcHval(x, y, destpos.x, destpos.y);
                    if (x == mypos.x || y == mypos.y) { //adds on how much it moved from previous point
                        board[y][x].Gval = board[mypos.y][mypos.x].Gval + 1;
                    }
                    else {
                        board[y][x].Gval = board[mypos.y][mypos.x].Gval + sqrt(2);
                    }
                }
                if (board[y][x].NaviState == NEXT) {
                    float distDiff = 0;
                    if (board[y][x].parentX != board[mypos.y][mypos.x].parentX || board[y][x].parentY != board[mypos.y][mypos.x].parentY) {
                        if (x == mypos.x || y == mypos.y) {
                            distDiff = 1;
                        }
                        else {
                            distDiff = sqrt(2);
                        }
                        //if the current Gval of a already generated -
                        //next sqaure would be higher compared to if it was branched from my square
                        if (board[y][x].Gval > (board[mypos.y][mypos.x].Gval + distDiff)) {
                            board[y][x].parentX = mypos.x;
                            board[y][x].parentY = mypos.y;
                            board[y][x].Gval = board[mypos.y][mypos.x].Gval + distDiff;
                        }
                    }
                }
            }
        }
    }
    return 0;
}


int* findFmin(grid board[BOARDSIZE][BOARDSIZE]) {
    float FvalSave = 0;
    char first = true;
    int Psave[2] = { 0,0 };
    for (int y = 0; y < BOARDSIZE; y++) {
        for (int x = 0; x < BOARDSIZE; x++) {
            if (board[y][x].NaviState == NEXT) {
                if (first) {
                    Psave[1] = x;
                    Psave[0] = y;
                    FvalSave = board[y][x].Gval + board[y][x].Hval;
                    first = false;
                }
                else if (round((board[y][x].Gval + board[y][x].Hval) * 100) / 100 < round(FvalSave * 100) / 100) {
                    Psave[1] = x;
                    Psave[0] = y;
                    FvalSave = board[y][x].Gval + board[y][x].Hval;
                }
            }
        }
    }
    return Psave;
}

COLORREF BoardColor(grid block) {
    COLORREF color;
    switch (block.NaviState) {
    case EMPTY:
        color = 0xFFFFFF;
        break;
    case NEXT:
        color = 0x4980EE;
        break;
    case START:
        color = 0xFFF561;
        break;
    case END:
        color = 0x35FF5D;
        break;
    case MARKED:
        color = 0xFF35A8;
        break;
    case WALL:
        color = 0x0;
        break;
    case SUCCESS:
        color = 0x35FF5D;
        break;
    case CURSEL:
        color = 0xDDFF00;
        break;
    default:
        break;
    }
    return color;
}

pos CoordBlock(POINT Coord, RECT ClientRect,pos BoardSize) {
    pos blockplace;
    pos BlockSize;
    BlockSize.x = ClientRect.right / BoardSize.x;
    BlockSize.y = ClientRect.bottom / BoardSize.y;
    blockplace.x = Coord.x / BlockSize.x;
    blockplace.y = Coord.y / BlockSize.y;
    return blockplace;
}


int BlockCoord(RECT ClientRect, int Hblock, int Vblock, POINT BlockPlace, RECT* RectCoord) {
    RectCoord->left = ClientRect.right / Hblock * BlockPlace.x;
    RectCoord->top = ClientRect.bottom / Vblock * BlockPlace.y;
    RectCoord->right = ClientRect.right / Hblock * (BlockPlace.x+1);
    RectCoord->bottom = ClientRect.bottom / Vblock * (BlockPlace.y+1);
    return 0;
}
