/*
    This file was adapted from template files provided by David Wood.
    This was sourced from Homework 3 of his CS 758 at UW - Madison.
    http://pages.cs.wisc.edu/~david/courses/cs758/Fall2016/wiki/index.php?n=Main.Homework3

    That, in turn, was based off an assignment by John Mellor-Crummey at Rice University.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hwtimer.h"
#include "reversi.h"

Move offsets[] =
{
    { 0,  1} /* right */,      { 0, -1} /* left */,
    {-1,  0} /* up */,         { 1,  0} /* down */,
    {-1, -1} /* up-left */,    {-1,  1} /* up-right */,
    { 1,  1} /* down-right */, { 1, -1} /* down-left */
};

int noffsets = sizeof(offsets) / sizeof(Move);

char diskcolor[] =
{
    '.', 'X', 'O', 'I'
};

void PrintDisk(int x_black, int o_white)
{
    printf(" %c", diskcolor[x_black + (o_white << 1)]);
}

void PrintBoardRow(int x_black, int o_white, int disks)
{
    if (disks > 1)
    {
        PrintBoardRow(x_black >> 1, o_white >> 1, disks - 1);
    }

    PrintDisk(x_black & BIT, o_white & BIT);
}

void PrintBoardRows(ull x_black, ull o_white, int rowsleft)
{
    if (rowsleft > 1)
    {
        PrintBoardRows(x_black >> 8, o_white >> 8, rowsleft - 1);
    }

    printf("%d", rowsleft);
    PrintBoardRow((int) (x_black & ROW8), (int) (o_white & ROW8), 8);
    printf("\n");
}

void PrintBoard(Board b)
{
    printf("  1 2 3 4 5 6 7 8\n");
    PrintBoardRows(b.disks[X_BLACK], b.disks[O_WHITE], 8);
    printf("\n");
}

/*
    Place a disk of color at the position specified by m.row and m,col,
    flipping the opponents disk there, if any.
*/
void PlaceOrFlip(Move m, Board *b, int color)
{
    ull bit = MOVE_TO_BOARD_BIT(m);
    b->disks[color] |= bit;
    b->disks[OTHERCOLOR(color)] &= ~bit;
}

/*
    Try to flip disks along a direction specified by a move offset.
    The return value is 0, if no flips were done.
    The return value is 1 + the number of flips, otherwise.
*/
int TryFlips(Move m, Move offset, Board *b, int color, int verbose, int domove)
{
    Move next;
    next.row = m.row + offset.row;
    next.col = m.col + offset.col;

    if (!IS_MOVE_OFF_BOARD(next))
    {
        ull nextbit = MOVE_TO_BOARD_BIT(next);

        if (nextbit & b->disks[OTHERCOLOR(color)])
        {
            int nflips = TryFlips(next, offset, b, color, verbose, domove);

            if (nflips)
            {
                if (verbose)
                {
                    printf("Flipping disk at %d,%d.\n", next.row, next.col);
                }

                if (domove)
                {
                    PlaceOrFlip(next, b, color);
                }

                return nflips + 1;
            }
        }
        else
        {
            if (nextbit & b->disks[color])
            {
                return 1;
            }
        }
    }

    return 0;
}

int FlipDisks(Move m, Board *b, int color, int verbose, int domove)
{
    int i;
    int nflips = 0;

    // Try flipping disks along each of the 8 directions.
    for(i = 0; i < noffsets; i++)
    {
        int flipresult = TryFlips(m, offsets[i], b, color, verbose, domove);
        nflips += (flipresult > 0) ? flipresult - 1 : 0;
    }

    return nflips;
}

/*
    Return the set of board positions adjacent to an opponent's disk that are empty.
    These represent a candidate set of positions for a move by color.
*/
Board NeighborMoves(Board b, int color)
{
    int i;
    Board neighbors = {{0,0}};

    for (i = 0;i < noffsets; i++)
    {
        ull colmask = (offsets[i].col != 0) ? ((offsets[i].col > 0) ? COL1 : COL8) : 0;

        int offset = MOVE_OFFSET_TO_BIT_OFFSET(offsets[i]);

        if (offset > 0)
        {
            neighbors.disks[color] |= (b.disks[OTHERCOLOR(color)] >> offset) & ~colmask;
        }
        else
        {
            neighbors.disks[color] |= (b.disks[OTHERCOLOR(color)] << -offset) & ~colmask;
        }
    }

    neighbors.disks[color] &= ~(b.disks[X_BLACK] | b.disks[O_WHITE]);

    return neighbors;
}

/*
    Return the set of board positions that represent legal moves for color.
    This is the set of empty board positions that are adjacent to an opponent's disk,
    where placing a disk of color will cause one or more of the opponent's disks to be flipped.
*/
int EnumerateLegalMoves(Board b, int color, Board *legal_moves)
{
    static Board no_legal_moves =
    {
        {0, 0}
    };

    Board neighbors = NeighborMoves(b, color);
    ull my_neighbor_moves = neighbors.disks[color];
    int row;
    int col;

    int num_moves = 0;
    *legal_moves = no_legal_moves;

    for(row = 8; row >= 1; row--)
    {
        ull thisrow = my_neighbor_moves & ROW8;

        for(col = 8; thisrow && (col >= 1); col--)
        {
            if (thisrow & COL8)
            {
                Move m =
                {
                    row, col
                };

                if (FlipDisks(m, &b, color, 0, 0) > 0)
                {
                    legal_moves->disks[color] |= BOARD_BIT(row, col);
                    num_moves++;
                }
            }

            thisrow >>= 1;
        }

        my_neighbor_moves >>= 8;
    }

    return num_moves;
}

int CountBitsOnBoard(const Board *b, int color)
{
    ull bits = b->disks[color];
    int ndisks = 0;

    for (; bits; ndisks++)
    {
        // Clear the least significant bit set.
        bits &= bits - 1;
    }

    return ndisks;
}

void EndGame(Board b)
{
    int o_score = CountBitsOnBoard(&b, O_WHITE);
    int x_score = CountBitsOnBoard(&b, X_BLACK);

    printf("Game over.\n");

    if (o_score == x_score)
    {
        printf("Tie game. Each player has %d disks.\n", o_score);
    }
    else
    {
        printf("X has %d disks. O has %d disks. %c wins!\n",
            x_score, o_score, (x_score > o_score ? 'X' : 'O'));
    }
}

int main()
{
    srand(10);

    /*  Uncomment the below line to introduce randomness into the SimpleAI.
        DO NOT UNCOMMENT IT while running experiments. The randomness introduced
        by modifying the SimpleAI is more than the change of parallelism.
    */

    // srand(time(NULL));

    // Starting gameboard.
    Board gameboard =
    {
        {
            BOARD_BIT(4,5) | BOARD_BIT(5,4) /* X_BLACK */,
            BOARD_BIT(4,4) | BOARD_BIT(5,5) /* O_WHITE */
        }
    };

    hwtimer_t timer;
    initTimer(&timer);
    ull totalTicks = 0;
    unsigned int turns = 0;

    int move_possible;
    PrintBoard(gameboard);

    do
    {
        move_possible = 0;
        move_possible |= SimpleAITurn(&gameboard, X_BLACK);

        // Uncomment the following line if you want to see the board after Black makes their move.
        // PrintBoard(gameboard);

        /*
            White is the player we are interested in.
            Set this to GoodAITurn() after you finish implementing it.
        */
        startTimer(&timer);

        move_possible |= SimpleAITurn(&gameboard, O_WHITE);

        stopTimer(&timer);
        totalTicks += getTimerTicks(&timer);

        turns++;

        // Uncomment the following lines if you want to see the board after White makes their move.
        // PrintBoard(gameboard);
    } while(move_possible);

    printf("\n");
    EndGame(gameboard);

    printf("Total time: %.0fns.\n", totalTicks / timer.cpuMHz * 1000);
    printf("Timer per turn: %.0fns.\n", totalTicks / turns/timer.cpuMHz * 1000);

    return 0;
}
