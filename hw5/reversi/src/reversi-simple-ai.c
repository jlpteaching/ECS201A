#include <stdio.h>
#include <stdlib.h>

#include "reversi.h"

int SimpleAITurn(Board *b, int color)
{
    Board legal_moves;
    int num_moves = EnumerateLegalMoves(*b, color, &legal_moves);
    if (num_moves > 0) {
        // do a random move (rand is seeded in main)
        int moveNum = rand()%num_moves;
        ull moves = legal_moves.disks[color];
        // clz returns the bit number of the first bit set counting from the msb
        int highestBit = __builtin_clzll(moves);
        for (int i=0; i<moveNum; i++) {
            // Remove the highest bit from possible moves
            moves -= ((ull)1) << (63-highestBit);
            // Get the next highest bit
            highestBit = __builtin_clzll(moves);
        }
        Move m = BIT_TO_MOVE(highestBit);
        // Set the third parameter to 1 to echo disks flipped
        int nflips = FlipDisks(m, b,color, 0, 1);
        if (nflips == 0) {
                printf("Illegal move: no disks flipped!!\n");
        }
        PlaceOrFlip(m, b, color);
        //PrintBoard(*b);
        return 1;
    } else return 0;
}
