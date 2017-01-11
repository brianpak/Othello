#include <stdlib.h>
#include <stdio.h>

#include <cilk/cilk.h>

#include "othello.h"

int GoodAITurn(Board *b, int color)
{
	Board legal_moves;
	int num_moves = EnumerateLegalMoves(*b, color, &legal_moves);

	if (num_moves > 0) {

		int depth = 0;
		Tuple t = max(num_moves, *b, legal_moves, color, depth);
		int bestMove = t.bit;

		// printf("bestmove: %d\n", bestMove);

		Move m = BIT_TO_MOVE(bestMove);

		int nflips = FlipDisks(m, b, color, 0, 1);
		if (nflips == 0) {
		    printf("Illegal move: no disks flipped!!\n");
		}
		PlaceOrFlip(m, b, color);
		// PrintBoard(*b);
		return 1;
	}
	else 
		return 0;
}

Tuple max(int num_moves, Board b, Board legal_moves, int color, int depth)
{
	ull moves = legal_moves.disks[color];
	ull moves_arr[num_moves];
	for (int i = 0; i < num_moves; i++) 
	{
		moves_arr[i] = moves;
	}

	Tuple tuple_arr[num_moves];
	int nextdepth = depth + 1;

	// byte();
	// binaryULL(moves);

	cilk_for (int i = 0; i < num_moves; i++) 
	{
		ull moves_i = moves_arr[i];
		Board tempB = b;
		Tuple tempT;

		int highestBit;
		for (int j = 0; j < i + 1; j++)
		{
			highestBit = __builtin_clzll(moves_i);
			moves_i -= ((ull)1) << (63-highestBit);
		}
		Move m = BIT_TO_MOVE(highestBit);

		if (depth < DEPTH)
		{
			tempT.bit = highestBit;
			tempT.flip = FlipDisks(m, &tempB, color, 0, 1);
			PlaceOrFlip(m, &tempB, color);
			tuple_arr[i] = tempT;

			// PrintBoard(tempB);
			// printf("max, movenum: %d movebit: %d \n\n", i, highestBit);

			if (nextdepth < DEPTH)
			{
				Board next_legal_moves;
				int other_color = OTHERCOLOR(color);
				int num_moves = EnumerateLegalMoves(tempB, other_color, &next_legal_moves);
				if (num_moves > 0) 
				{
					Tuple newT = min(num_moves, tempB, next_legal_moves, other_color, nextdepth);
					tuple_arr[i].flip += newT.flip;
				}
			}
		}
	}

	// printf("max\n");
	// printTupleArray(tuple_arr, num_moves);
	// printf("\n");

	return findMaxBit(tuple_arr, num_moves);
}

Tuple min(int num_moves, Board b, Board legal_moves, int color, int depth)
{
	ull moves = legal_moves.disks[color];
	ull moves_arr[num_moves];
	for (int i = 0; i < num_moves; i++) 
	{
		moves_arr[i] = moves;
	}

	Tuple tuple_arr[num_moves];
	int nextdepth = depth + 1;

	// byte();
	// binaryULL(moves);

	cilk_for (int i = 0; i < num_moves; i++) 
	{
		ull moves_i = moves_arr[i];
		Board tempB = b;
		Tuple tempT;

		int highestBit;
		for (int j = 0; j < i + 1; j++)
		{
			highestBit = __builtin_clzll(moves_i);
			moves_i -= ((ull)1) << (63-highestBit);
		}
		Move m = BIT_TO_MOVE(highestBit);

		if (depth < DEPTH)
		{
			tempT.bit = highestBit;
			tempT.flip = -(FlipDisks(m, &tempB, color, 0, 1));
			PlaceOrFlip(m, &tempB, color);
			tuple_arr[i] = tempT;

			// PrintBoard(tempB);
			// printf("min, movenum: %d movebit: %d \n\n", i, highestBit);

			if (nextdepth < DEPTH)
			{
				Board next_legal_moves;
				int other_color = OTHERCOLOR(color);
				int num_moves = EnumerateLegalMoves(tempB, other_color, &next_legal_moves);
				if (num_moves > 0) 
				{
					Tuple newT = max(num_moves, tempB, next_legal_moves, other_color, nextdepth);
					tuple_arr[i].flip += newT.flip;
				}
			}
		}
	}

	// printf("min\n");
	// printTupleArray(tuple_arr, num_moves);
	// printf("\n");

	return findMinBit(tuple_arr, num_moves);
}

Tuple findMaxBit(Tuple arr[], int size) 
{
	int index = -1;
	int temp = -64;

	for (int i = 0; i < size; i++)
    {
    	int flip = arr[i].flip;
        if (flip > temp)
        {
        	temp = flip;
        	index = i;
        }
    }
	return arr[index];
}

Tuple findMinBit(Tuple arr[], int size) 
{
	int index = -1;
	int temp = 64;

	for (int i = 0; i < size; i++)
    {
    	int flip = arr[i].flip;
        if (flip < temp)
        {
        	temp = flip;
        	index = i;
        }
    }
	return arr[index];
}

void printTupleArray (Tuple arr[], int size)
{
	int index;
	printf("scores\n");
	for ( index = 0; index < size; index++)
	{
		printf("%d ", arr[index].flip);
	}
	printf("\n");
	printf("bits\n");
	for ( index = 0; index < size; index++)
	{
		printf("%d ", arr[index].bit);
	}
	printf("\n");
}