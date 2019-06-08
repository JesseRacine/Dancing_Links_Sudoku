#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dance.h"
#include "sudoku.h"
#include "brute.h"
#include <windows.h>
#include <time.h>

/* This code uses one block of malloc memory for all of the columns, pointed at by head. For the row data
 * nodes it uses one giant struct array since the number of row elements is known at runtime. This allows for easier memory management, since the struct array is an automatic variable and is managed
 * then without the use of malloc/free. The array of columns is also easy to delete by deleting the data
 * at the head.
 */


void getDir(char* dir)
{
    /* This code finds the directory that this program is located in, which turns out to be annoying in C.
    All it does is look at argv[0] and strips out the filename from it, leaving the directory path only
    */
	
    char* c;

	GetModuleFileName(NULL,dir,255);
    c = strrchr(dir, '\\');   

    /* if the program is in directory /program/, char* c now points to the 'm' in program, so we must
    move it one character forward in the string and then end the string
    */
    *(c+1) = 0;
}

int getGivens(char* fname, struct Givens givens[])
{
    /* This function loads an array of Givens structs with the values of the givens in the text file
    givens.txt that is in the same directory as the program. It returns how many givens were loaded.
    */
	FILE* g = fopen(fname, "r");

    int i = 0;
    while( fscanf(g, "%d %d %d", &givens[i].row, &givens[i].col, &givens[i].value) == 3 )
        i++;

    return i;
}

void showGivens(struct Givens givens[], int i)
{
    int j;
    printf("Givens are:\n");
    for( j = 0; j < i; j++)
        printf("Row %d, Column %d contains a %d\n", givens[j].row, givens[j].col, givens[j].value);
}

void printBoard(int board[9][9])
{ /* Prints a 9x9 Sudoku board represented by a 9x9 array */

    int i, j;

    for( i = 0; i < 9; i++)
    {
         if( i != 0 && i % 3 == 0)
         {
             for( j = 0; j < 32; j++)
             {
                 printf("-");
             }
             printf("\n");
         }

        for( j = 0; j < 9; j++)
        {
            if( j % 3 == 0 && j != 0)
            printf("|  ");

            printf("%d  ", board[i][j]);

        }
        printf("\n");
        for(j = 0; j < 9; j++)
        {
            if( j % 3 == 0 && j != 0)
                printf("|  ");
            printf("   ");
        }
        printf("\n");
    }



}

void placeGivens( struct Givens givens[], int size, int board[9][9])
{
	int i;
	for( i = 0; i < size; i++)
		board[givens[i].row][givens[i].col] = givens[i].value;


}

clock_t start_timing()
{
	return clock();	
}

double stop_timing(clock_t start)
{
	double cpu_time_used;
	clock_t end = clock();
	return ((double) (end - start)) / CLOCKS_PER_SEC;
}

void reset_board(int board[9][9])
{
	for( int i = 0; i < 9; i++)
		for( int j = 0; j < 9; j++)
            board[i][j] = 0;
}

int main(int argc, char *argv[])
{
    char dir[255];
    struct Givens givens[81];   
	clock_t start;
	double seconds_taken;
    int amount;
    int board[9][9];  /* Represents a board in row - column format */
    reset_board(board);
    getDir(dir);
    strcat(dir, "givens.txt");
    amount = getGivens(dir, givens);
    placeGivens(givens, amount, board);
    printf("Here is the starting board:\n");
    printBoard(board);   
    printf("First, we will time the brute forth algorithm\n");

 /*
   constructMatrix(board);
   printBoard(board);
*/
	start = start_timing();
	findSolution(board,0,0);
	seconds_taken = stop_timing(start);	
	printf("Time taken was %f seconds\n", seconds_taken);
    
    printf("Now, we will try the dancing links algoritm to solve the puzzle\n");
    reset_board(board);
    placeGivens(givens,amount, board);
    start = start_timing();
    constructMatrix(board);
    printf("Solution found for dancing links:\n");
    printBoard(board);
    seconds_taken = stop_timing(start);
    printf("Time taken was %f seconds\n", seconds_taken);
    

	return 1;
}
