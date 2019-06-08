#include <stdio.h>
#include <time.h>
#include "brute.h"

char solution = 0;


int checkRow(int board[9][9], int row, int val)
{
	/* Check the sudoku board to see if the number val is in the row */

	int i;

	for( i = 0; i < 9; i++)
	{
		if( board[row][i] == val)
			return 1;  /* Return true */
	}

	return 0;
}

int checkCol(int board[9][9], int col, int val)
{
	/* Check the sudoku board to see if the number val is in the column */

	int i;

	for( i = 0; i < 9; i++)
	{
		if( board[i][col] == val)
			return 1;  /* Return true */
	}

	return 0;
}

int checkBlock(int board[9][9], int row, int col, int val)
{
	/* Return true if the number val is contained in the 3x3 sudoku block that
	 * the square (row,col) is in
	 */

	/* Coords for the top left corner of the 3x3 block */
	int topX, topY;

	int i, j;

	topX = col - col % 3;
	topY = row - row % 3;

	for( i = topX; i < topX+3; i++)
	{
		for( j = topY; j < topY+3; j++)
		{
			if( board[j][i] == val )
				return 1;
		}
	}

	return 0;

}

int isSafe(int board[9][9], int row, int col, int val)
{
	if( checkBlock(board, row, col, val) || checkCol(board,col, val) || checkRow(board, row, val) )
		return 0;

	return 1;
}

void findSolution( int board[9][9], int row, int col)
{
	int c;


	if( row == 9)
	{
		printf("We have a solution!\n");
		printBoard(board);
		solution = 1;
		return;
	}

	if( board[row][col] != 0 ) /* Then a given has already been placed here */
		findSolution(board, row + col/8 , (col + 1) % 9);
	else
	{
		for( c = 1; c < 10; c++)
		{
			if(solution)
				return;
			if( isSafe(board, row, col, c) )
			{
				board[row][col] = c;
				findSolution(board, row + col/8 , (col + 1) % 9);
			}
		}

		/* If the code gets to here, a solution could not be found with the current board square.
			 * We will set the board square back to 0
			 */
		board[row][col] = 0;
	}

}


