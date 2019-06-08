#include "sudoku.h"
#include <stdio.h>
/* This program represents the dancing links matrix consisting of columns and rows starting with a
 * 0 reference. The algorithm implemented here that solves Sudoku represents a Sudoku grid also
 * with a 0 referenced coordinate system, including the actual value placed in the Sudoku grid.
 *
 * When the solution is returned back to the main program though the 0 referenced value that
 * each Sudoku square contains is turned into a regular Sudoku value of 1-9
 */


struct Node  /* Represents column headers and data nodes in Knuth's algorithm */
{
	struct Node *left, *right, *up, *down, *C; /* c points back to the column
	header node for the column that the data node is in */

	int name, size; /* Represents data that only a column header will have. If its the master column header,
	name will be set to -1. Name is a string in Knuth's example but	we are using an int	here to represent the name for each column */
} *soluts[500];  /* soluts represent the variable '0' that Knuth uses to keep a list
of solutions in the algorithms in his paper. 0k in his paper represents the kth element
of the array of solution rows */


void cover( struct Node *col)
{
/* Code that covers a column -- argument for function is the header node for the column that needs to be covered
    This algorithm comes from Knuth, page 6 -- procedure for covering the matrix in the middle of the page. Each line of the procedure
    will document which line of the Knuth instructions it is derived from.
    */

	col->right->left = col->left; /* Knuth line 1 */
	col->left->right = col->right; /* Knuth line 1 */

	struct Node* i;
	for( i = col->down; i != col; i = i->down )  /* Knuth line 2 */
	{
		struct Node* j;
		for( j = i->right; j != i; j = j->right ) /* Knuth line 3 */
		{
			j->down->up = j->up;   /* Knuth line 4 */
			j->up->down = j->down; /* Knuth line 4 */
			j->C->size = j->C->size - 1; /* Knuth line 5 */
		}
	}
}


void uncover( struct Node *c )
{
/* The argument to this function is a link to the column you want to uncover. The code
 * here is taken directly from Knuth page 6, on the bottom of the page.
 */

	struct Node *i, *j;

	for( i = c->up; i != c; i = i->up) /* Knuth line 1 */
	{
		for( j = i->left; i != j; j = j->left) /* Knuth line 2 */
		{
			j->C->size = j->C->size + 1; /* Knuth line 3 */
			j->down->up = j;  /* Knuth line 4 */
			j->up->down = j;  /* Knuth line 4 */
		}
	}
	c->right->left = c;  /* Knuth line 5 */
	c->left->right = c; /* Knuth line 5 */

}


struct Node* findBest(struct Node* head)
{ /* Code that finds the next column that should be covered. Algorithm taken
directly from Knuth page 6, top of page (2 line algorithm).
*/
	struct Node *j, *c;
	c = 0;
	int s = 500000; /* represents 'infinity' that Knuth uses in his algorithm */

	for( j = head->right; j != head; j = j->right) /* Knuth line 1 */
	{
		if( j->size < s) /* All of the next lines are Knuth line 2 */
		{
			s = j->size;
			c = j;
		}
	}

	return c;
}



void printSolution(int k)
{
    int i;
    struct Node* n;

    /*DEBUG CODE*/
    int cols[81] = {0};



    for( i = 0; i < k; i++)
    {
        printf(" %i ", soluts[i]->C->name );
        for( n = soluts[i]->right; n != soluts[i]; n = n->right)
            printf(" %i ", n->C->name );

        printf("\n");
    }


    /* DEBUG CODE*/
    for( i = 0; i < 81; i++)
    {
    	n = soluts[i];
    	while( n->C->name > 80)
    		n = n->right;
    	cols[ n->C->name ]++;

    }
    for( i = 0; i < 81; i++)
    	printf("col %d is %d \n", i, cols[i]);

}


void search( int k, struct Node* head)
{
	/* The algorithm for this function is taken directly from Knuth page 5. Each appropriate line
	 * will have a comment indicating what line of Knuth's algorithm it applies to.
	 *
	 * This is the main recursive algorithm that runs the dancing links system.
	 *
	 * It should be known that the variable 'r' in the algorithm is the row that you are
	 * selecting to be part of your solution. It is the row that is selected and that
	 * other columns in the matrix are covered off of.
	 */

	struct Node *r, *j;

	if( head->right == head)  /* Knuth line 1 */
	{	   
		return;   /* we have found the solution! */
	}

	struct Node* c = findBest(head);  /* Knuth line 2 */

	cover(c);   /* Knuth line 3 */

	for(r = c->down; r != c; r = r->down)  /* Knuth line 4 */
	{
		soluts[k] = r;   /* Knuth line 5 */

		for(j = r->right; j != r; j = j->right )  /* Knuth line 6 */
			cover( j->C);   /* Knuth line 7 */

		search(k + 1, head);   /* Knuth line 8 */

		r = soluts[k]; /* Knuth line 9 */
		c = r->C;  /* Knuth line 9 */

		for(j = r->left; j != r; j = j->left)  /* Knuth line 10 */
			uncover( j->C );   /* Knuth line 11 */
	}

	uncover( c ); /* Knuth line 12 */
}



void initColumns(int n, struct Node* head )
{
	/* Sets up the left-right links and other data for n column headers, with the first header becoming the
	 *  master column header. The column headers that this function sets up are pointed to by the head
	 *  pointer argument
	 */

	int i;


	for( i = 0; i < n ; i++)
	{
		head[i].name = i - 1; /* start naming from -1 with the master column header */
		head[i].up = &head[i]; /* point up link of column header to itself */
		head[i].down = &head[i]; /* point down link back to itself */
		head[i].size = 0;
		head[i].C = 0; /* We will null out pointer C because this is a column */
		if( i == 0)
			head[i].left = &head[n-1];
		else
			head[i].left = &head[i-1]; /* Point left link to the column header to the left */

		if( i == n-1)
			head[i].right = head;
		else
			head[i].right = &head[i+1]; /* Point right link to the column header to the right */

	}

}

void initRowLinks( struct Node* row, int n)
{
	/* Make a row of n data nodes starting at pointer row. Pointer row should point to an array of preallocated
	 * node structures. This function just fills in the left/right links in for them. */

	int i;
	for( i = 0; i < n; i++ )
	{
		if( i == 0)
			row[i].left = &row[n-1];
		else
			row[i].left = &row[i-1];

		if( i == n - 1)
			row[i].right = &row[0];
		else
			row[i].right = &row[i + 1];
	}
}

void placeRow( struct Node* row, int n, int pos[], struct Node* head)
{
	/* Place row, pointed at by pointer row, consisting of an array of n row Nodes, into the columns
	 * denoted by the array pos (stands for position).  For example, if there are 3 nodes for the row,
	 * n would equal 3 and the pos array might contain [1, 5, 8] for an arbitrary example. This would mean to put
	 * the first element in the row in column 1, the second row element in column 5, and the last element in column
	 * 8.
	 *
	 * Essentially, what this function does is sets up the proper up and down pointers for each row and the columns
	 * that point to them.
	 */

	int i, col;

	for( i = 0; i < n; i++)
	{
		/* We must do four things here:
		 * 1. Set the up down links for the row element
         * 2. Set the column pointer for the row element (the variable "C")
		 * 3. Set the down link on whatever is above the row elment we are adding (either column header or other row element)
		 * 4. Set the up link for the column header and increment the column's size variable
		 */
		col = pos[i] + 1; /* Avoiding linking columns to the master column header with the "+1" */

		row[i].up = head[ col ].up;  /* First we set the proper up down links for the row element we are adding */
		row[i].down = &head[col];

		row[i].C = &head[col];

		row[i].up->down = &row[i]; /* Points whatever is above this row element to the new row element we are adding */

		head[col].up = &row[i];  /* Step 4 */
		head[col].size++;   /* Step 4*/

	}
}


int sudokuMatrixRowColumn(int row, int col)
{
	/* Return the proper column of the dancing links matrix that represents
	 * the "rows columns condition" described in the various sudoku algorithm solving texts
	 */

	return row * 9 + col;
}

int sudokuMatrixRowDigit( int row, int val)
{
	/* return the proper column of the dancing links matrix that represents the
	 * "rows digit" condition of the sudoku algorithm
	 */

	return 80 + row * 9 + val;
}

int sudokuMatrixColDigit(int col, int val)
{
	/* returns the proper column of the dancing links matrix that represents the
	 * "columns digit" condition of the sudoku algorithm
	 */

	return 161 + col * 9 + val;
}

int getBox(int row, int col)
{
	/* return the 9x9 box position on a sudoku grid that square row, col, represents,
	 * using a 0 referenced coordinate
	 */

	return (col / 3) + (row/3) * 3;
}

int sudokuMatrixBoxDigit( int row, int col, int val)
{
	/* returns the proper column of the dancing links matrix that represents the
	 * "box digit" constraint of the sudoku algorithm
	 */

	int box = getBox(row, col);

	return 242 + box * 9 + val;

}

void placeSolution(int board[9][9])
{
	/* Place the solution found with the dancing links algorithm into the puzzle */

	int i, row, col, val;

	struct Node* n;

	for( i = 0; i < 81; i++)
	{
		n = soluts[i];

		while( n->C->name > 80)
			n = n->right;

		row = n->C->name / 9;
		col = n->C->name % 9;

		n = n->right;

		val = (n->C->name - 81) % 9 + 1;

		board[row][col] = val;

	}

}

void constructMatrix(int board[9][9])
{
	/* This function creates a "dancing links" matrix of the initial sudoku grid. The
	 * initial sudoku grid has each grid position (81 total) filled with all numbers, 1 through
	 * 9. We create a dancing-links matrix based on this by adding 729 rows with 4 nodes each.
	 * We are adding 729 rows because there are 81 sudoku squares total, and each square at the start
	 * contains all 9 numbers, so 81 * 9 = 729.
	 *
	 * However, this is not completely true because for each "Given" that we know about the Sudoku
	 * problem, we do not fill the given squares with the numbers 1 through 9. For each given square,
	 * we only fill the number given by the given. So, if we know that the upper Sudoku square, 0, 0,
	 * contains a 5, then we only have to fill the first square with the number 5. So, in terms of our
	 * dancing links matrix, we will only have to create 729 - 8 = 721 rows, or 80 * 9 + 1 = 721.
	 *
	 *The givens are passed to this function by sending it a 9x9 array that functions as a
	 *sudoku board. The board squares filled with 0s are empty, and any squares with other
	 *sudoku numbers serve as the givens.
	 *
	 *Every row in our matrix will have 4 nodes.
	 */
	/* Below variables used in for loop to iterate through the 81 squares in a sudoku board */
	int row, col;


	/* For our algorithm we are actually hardcoding our columns/rows as arrays. For any dancing links problem you will always
	 * know before hand the number of columns and rows needed, and Sudoku problem is no different. This makes it much faster because
	 * we won't waste time mallocing the individual nodes that we need
	 */
	struct Node matColumns[325]; /* For this algorithm we need 324 columns, + 1 for the master header */
	struct Node matRows[729][4]; /* Worst case we need 2916 nodes. With the givens, we actually need fewer. */
	int matRowCount = 0;
	int placement[4], i;

	initColumns( 325, matColumns);

	/* Now we will create and place our rows. The number of rows we will create is going to be
	 *  (81 - number_of_givens) * 9 + number_of_givens
	 */
	matRowCount = 0;
	for( row = 0; row < 9; row++)
	{
		for( col = 0; col < 9; col++)
		{

			if( board[row][col] != 0) /* We have a given we can use */
			{
				initRowLinks(matRows[matRowCount], 4);
				placement[0] = sudokuMatrixRowColumn(row,col);
				placement[1] = sudokuMatrixRowDigit(row, board[row][col]);
				placement[2] = sudokuMatrixColDigit(col, board[row][col]);
				placement[3] = sudokuMatrixBoxDigit(row, col, board[row][col]);


				placeRow( matRows[matRowCount], 4, placement, matColumns );

				matRowCount++;
			}
			else
				for( i = 1; i < 10; i++)
				{
					initRowLinks(matRows[matRowCount], 4);
					placement[0] = sudokuMatrixRowColumn(row,col);
					placement[1] = sudokuMatrixRowDigit(row, i);
					placement[2] = sudokuMatrixColDigit(col, i);
					placement[3] = sudokuMatrixBoxDigit(row, col, i);


					placeRow( matRows[matRowCount], 4, placement, matColumns );

					matRowCount++;
				}

		}
	}


	search(0, matColumns);

	placeSolution(board);
	/*printSolution(81);*/
}

void test(void)
{
	/* For this test we make 4 column headers and the main header h *
	 * We will create the following dancing links matrix and solve it:
	 * 	0 1 0 0
	 *  1 0 0 0
	 *  1 1 1 0
	 *  0 0 1 1
	 */

	struct Node columns[5];  /* 5 columns total because we need an extra for the column header */

	struct Node* head;

	head = columns;

	initColumns( 5, head );

	int b[3];

	struct Node i[7];




	initRowLinks(i,1);

	initRowLinks(i+1, 1);

	initRowLinks(i+2, 3);

	initRowLinks(i+5, 2);




	b[0] = 1;
	placeRow(&i[0], 1, b, head);


	b[0] = 0;
	placeRow(&i[1], 1, b, head);

	b[0] = 0;
	b[1] = 1;
	b[2] = 2;
	placeRow(&i[2], 3, b, head);

	b[0] = 2;
	b[1] = 3;
	placeRow(&i[5], 2, b, head);




	search(0, head);

}
