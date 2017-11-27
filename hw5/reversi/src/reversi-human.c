#include <stdio.h>

#include "reversi.h"

void ReadMove(int color, Board *b)
{
    Move m;
    ull movebit;
    for(;;) {
        printf("Enter %c's move as 'row,col': ", diskcolor[color+1]);
        scanf("%d,%d",&m.row,&m.col);

        /* if move is not on the board, move again */
        if (IS_MOVE_OFF_BOARD(m)) {
            printf("Illegal move: row and column must both be between 1 and 8\n");
            PrintBoard(*b);
            continue;
        }
        movebit = MOVE_TO_BOARD_BIT(m);

        /* if board position occupied, move again */
        if (movebit & (b->disks[X_BLACK] | b->disks[O_WHITE])) {
            printf("Illegal move: board position already occupied.\n");
            PrintBoard(*b);
            continue;
        }

        /* if no disks have been flipped */
        {
            int nflips = FlipDisks(m, b,color, 1, 1);
            if (nflips == 0) {
                printf("Illegal move: no disks flipped\n");
                PrintBoard(*b);
                continue;
            }
            PlaceOrFlip(m, b, color);
            printf("You flipped %d disks\n", nflips);
            PrintBoard(*b);
        }
        break;
    }
}

int HumanTurn(Board *b, int color)
{
    Board legal_moves;
    int num_moves = EnumerateLegalMoves(*b, color, &legal_moves);
    if (num_moves > 0) {
        ReadMove(color, b);
        return 1;
    } else return 0;
}
