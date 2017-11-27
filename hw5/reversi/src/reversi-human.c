/*
    This file was adapted from template files provided by David Wood.
    This was sourced from Homework 3 of his CS 758 at UW - Madison.
    http://pages.cs.wisc.edu/~david/courses/cs758/Fall2016/wiki/index.php?n=Main.Homework3

    That, in turn, was based off an assignment by John Mellor-Crummey at Rice University.
*/

#include <stdio.h>

#include "reversi.h"

void ReadMove(int color, Board *b)
{
    Move m;
    ull movebit;

    while (1)
    {
        printf("Enter %c's move as 'row, column': ", diskcolor[color+1]);
        scanf("%d,%d", &m.row, &m.col);

        // If move is not on the board, move again.
        if (IS_MOVE_OFF_BOARD(m))
        {
            printf("Illegal move: row and column must both be between 1 and 8.\n");
            PrintBoard(*b);
            continue;
        }

        movebit = MOVE_TO_BOARD_BIT(m);

        // If board position occupied, move again.
        if (movebit & (b->disks[X_BLACK] | b->disks[O_WHITE]))
        {
            printf("Illegal move: board position already occupied.\n");
            PrintBoard(*b);
            continue;
        }

        // If no disks have been flipped.
        int nflips = FlipDisks(m, b, color, 1, 1);

        if (nflips == 0)
        {
            printf("Illegal move: no disks flipped.\n");
            PrintBoard(*b);
            continue;
        }

        PlaceOrFlip(m, b, color);
        printf("You flipped %d disks.\n", nflips);
        PrintBoard(*b);

        break;
    }
}

int HumanTurn(Board *b, int color)
{
    Board legal_moves;

    int num_moves = EnumerateLegalMoves(*b, color, &legal_moves);

    if (num_moves > 0)
    {
        ReadMove(color, b);
        return 1;
    }
    else
    {
        return 0;
    }
}
