/*
    This file was adapted from template files provided by David Wood.
    This was sourced from Homework 3 of his CS 758 at UW - Madison.
    http://pages.cs.wisc.edu/~david/courses/cs758/Fall2016/wiki/index.php?n=Main.Homework3

    That, in turn, was based off an assignment by John Mellor-Crummey at Rice University.
*/

#ifndef REVERSI_H
#define REVERSI_H

#define BIT 0x1

#define X_BLACK 0
#define O_WHITE 1
#define OTHERCOLOR(c) (1 - (c))

/*
    Represent the game board squares as a 64-bit unsigned integer.
    These macros index from a (row, column) position on the board
    to a position and bit in a game board bitvector.
*/
#define BOARD_BIT_INDEX(row, col) ((8 - (row)) * 8 + (8 - (col)))
#define BOARD_BIT(row, col) (0x1LL << BOARD_BIT_INDEX(row, col))
#define MOVE_TO_BOARD_BIT(m) BOARD_BIT(m.row, m.col)
#define BIT_TO_MOVE(bit) {bit / 8 + 1, bit % 8 + 1}

// All of the bits in row 8.
#define ROW8 \
    (BOARD_BIT(8,1) | BOARD_BIT(8,2) | BOARD_BIT(8,3) | BOARD_BIT(8,4) |  \
     BOARD_BIT(8,5) | BOARD_BIT(8,6) | BOARD_BIT(8,7) | BOARD_BIT(8,8))

// All of the bits in column 8.
#define COL8 \
    (BOARD_BIT(1,8) | BOARD_BIT(2,8) | BOARD_BIT(3,8) | BOARD_BIT(4,8) |  \
     BOARD_BIT(5,8) | BOARD_BIT(6,8) | BOARD_BIT(7,8) | BOARD_BIT(8,8))

// All of the bits in column 1.
#define COL1 (COL8 << 7)

#define IS_MOVE_OFF_BOARD(m) (m.row < 1 || m.row > 8 || m.col < 1 || m.col > 8)
#define IS_DIAGONAL_MOVE(m) (m.row != 0 && m.col != 0)
#define MOVE_OFFSET_TO_BIT_OFFSET(m) (m.row * 8 + m.col)

typedef unsigned long long ull;

/*
    The game board is represented as a pair of bit vectors.
    One is for x_black disks on the board.
    One is for o_white disks on the board.
*/
typedef struct
{
    ull disks[2];
} Board;

typedef struct
{
    int row;
    int col;
} Move;

char diskcolor[4];

// Players.
int HumanTurn(Board *b, int color);
int SimpleAITurn(Board *b, int color);
int GoodAITurn(Board *b, int color);

// Auxilary functions exported from reversi.c.
int EnumerateLegalMoves(Board b, int color, Board *legal_moves);
void PrintBoard(Board b);
Board NeighborMoves(Board b, int color);
int CountBitsOnBoard(const Board *b, int color);
int FlipDisks(Move m, Board *b, int color, int verbose, int domove);
void PlaceOrFlip(Move m, Board *b, int color);

#endif
