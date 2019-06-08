# Dancing_Links_Sudoku
Dancing Links Sudoku Solver in C

This program demonstrates the usage of the "Dancing Links" algorithm to solve a Sudoku puzzle.

The Dancing Links algorithm was created by Donald Knuth, and was not originally created to solve Sudoku puzzles. Solving Sudoku is just one usage of it.

The time it takes the dancing links algorithm to work vs a more straight forward brute force approach is compared with this program. The more straight forward approach is to go square by square and iterate through all the numbers. First, try to put a 1 in the square. Then move forward to the next square. If you can't put a 1, try a 2. If you can't put anything in the square, backtrack to the previous square and try the next number in the previous square.

First, you must specify the "givens" of the puzzle in a text file called "givens.txt".

The givens file assumes a 9x9 Sudoku puzzle with the following format:

`row col number`

The row and column numbers are zero indexed. In other words, to place a given "5" number at the top left corner of the Sudoku board, you would put the following in the givens file:

`0 0 5`

The givens file must be in the same directory as the executable. The code will compile and work correctly given any gcc compiler in Windows with:

`gcc *.c -o sudoku.exe`

Here is a sample output from the program:

```Here is the starting board:
0  0  0  |  0  0  0  |  0  0  0
         |           |
0  0  0  |  0  0  3  |  0  8  5
         |           |
0  0  1  |  0  2  0  |  0  0  0
         |           |
--------------------------------
0  0  0  |  5  0  7  |  0  0  0
         |           |
0  0  4  |  0  0  0  |  1  0  0
         |           |
0  9  0  |  0  0  0  |  0  0  0
         |           |
--------------------------------
5  0  0  |  0  0  0  |  0  7  3
         |           |
0  0  2  |  0  1  0  |  0  0  0
         |           |
0  0  0  |  0  4  0  |  0  0  9
         |           |
First, we will time the brute forth algorithm
We have a solution!
9  8  7  |  6  5  4  |  3  2  1
         |           |
2  4  6  |  1  7  3  |  9  8  5
         |           |
3  5  1  |  9  2  8  |  7  4  6
         |           |
--------------------------------
1  2  8  |  5  3  7  |  6  9  4
         |           |
6  3  4  |  8  9  2  |  1  5  7
         |           |
7  9  5  |  4  6  1  |  8  3  2
         |           |
--------------------------------
5  1  9  |  2  8  6  |  4  7  3
         |           |
4  7  2  |  3  1  9  |  5  6  8
         |           |
8  6  3  |  7  4  5  |  2  1  9
         |           |
Time taken was 22.492000 seconds
Now, we will try the dancing links algoritm to solve the puzzle
Solution found for dancing links:
9  8  7  |  6  5  4  |  3  2  1
         |           |
2  4  6  |  1  7  3  |  9  8  5
         |           |
3  5  1  |  9  2  8  |  7  4  6
         |           |
--------------------------------
1  2  8  |  5  3  7  |  6  9  4
         |           |
6  3  4  |  8  9  2  |  1  5  7
         |           |
7  9  5  |  4  6  1  |  8  3  2
         |           |
--------------------------------
5  1  9  |  2  8  6  |  4  7  3
         |           |
4  7  2  |  3  1  9  |  5  6  8
         |           |
8  6  3  |  7  4  5  |  2  1  9
         |           |
Time taken was 0.062000 seconds
```

## Other documentation:

The "dancing_links.pdf" file contains some additional documentation that I wrote up when trying to understand the algorithm.

The original dancing links algorithm paper is in the "knuth_dancing_links_paper.pdf" file contained here.




