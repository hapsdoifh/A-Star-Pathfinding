

#define BOARDSIZE 8

#define EMPTY 0
#define MARKED 1
#define NEXT 2
#define START 3
#define END 4
#define WALL -1
#define SUCCESS 10

typedef struct __grid {
    float Hval; //
    float Gval;
    int parentX;
    int parentY;
    int NaviState; // 0 empty, 1 marked, 2 next,3 start, 4 end, -1 wall
} grid;

typedef struct XYpos {
    int x;
    int y;
} pos;


int Min(int a, int b);
float CalcHval(int startX, int startY, int destX, int destY);
int MarkAvail(grid board[BOARDSIZE][BOARDSIZE], pos mypos, pos destpos, pos PrevPathPos);
int* findFmin(grid board[BOARDSIZE][BOARDSIZE]);
pos CoordBlock(POINT Coord, RECT ClientRect, pos GridSize);
COLORREF BoardColor(grid block);
int BlockCoord(RECT ClientRect, int Hblock, int Vblock, POINT BlockPlace, POINT* PixelCoord);