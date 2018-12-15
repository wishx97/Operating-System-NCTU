#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUMBER_OF_THREADS 11 /* Hint */
#define PUZZLE_SIZE 9

/* example puzzle */
int puzzle[PUZZLE_SIZE+1][PUZZLE_SIZE+1] = {
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
        {-1,0,0,0,0,0,0,0,0,0},
        {-1,0,0,0,0,0,0,0,0,0},
        {-1,0,0,0,0,0,0,0,0,0},
        {-1,0,0,0,0,0,0,0,0,0},
        {-1,0,0,0,0,0,0,0,0,0},
        {-1,0,0,0,0,0,0,0,0,0},
        {-1,0,0,0,0,0,0,0,0,0},
        {-1,0,0,0,0,0,0,0,0,0},
        {-1,0,0,0,0,0,0,0,0,0}
};

/* data structure for passing data to threads */
typedef struct
{
    int thread_number;
    int x;
    int y;
} parameters;

/* print puzzle */
#define LINE "====================== \n"
#define COLLINE "\n___________________________________ \n"
#define NAME "||  SUM   CHECKER  || \n"
void print_grid(int grid[10][10])
{
    int i,j;
    printf(LINE);
    printf(NAME);
    printf(LINE);

    for (i = 1; i < 10; i++)
    {
        for (j = 1; j < 10; j++)
        {
            printf("|%1d |",grid[i][j]);
        }
        printf(COLLINE);
    }
    printf("\n");
}
// read file to check sudoku
void SetPuzzle(char filename[]){
    FILE *file = fopen(filename,"r");
    int i,j,ch,num;
    for (i=0;i<=9;i++){
        for(j=0;j<=9;j++){
            while(((ch = getc(file)) != EOF)){
                if(ch == EOF) break;
                if(ch == ',') break;
                if(ch == '\n') break;
                ungetc(ch,file);
                fscanf(file,"%d",&num);
                if(num!=-1)
                    puzzle[i][j] = num;
            }
        }
    }
    print_grid(puzzle);
}

// check row and col
int checkRowCol() {
    int i, j;
    int tmpRowSum = 0;
    for (i=1;i<10;i++){
        tmpRowSum = 0;
        int tmpColSum = 0;
        for(j=1;j<10;j++) {
            tmpRowSum += puzzle[i][j];
            tmpColSum += puzzle[j][i];
        }
        if (tmpRowSum != tmpColSum) {
            return 0;
        }
    }
    return tmpRowSum;
}

int checkSubGrid(int inputRow, int inputCol) {
    int i, j;
    int tmpSum = 0;
    for (i = inputRow; i < inputRow + 3; i++) {
        for (j = inputCol; j < inputCol + 3; j++) {
            tmpSum += puzzle[i][j];
        }
    }
    return tmpSum;
}

int main(int argc, char* argv[]) {
    int rv = 1; // flag to check answer
    // input the sudoku file
    SetPuzzle("test.txt");
    // added code
    int rowColSum = checkRowCol();
    if (rowColSum == 0)
        rv = 0;
    else {
        int i, j;
        int subGridResult = rowColSum;
        for (i = 1 ; i <= 7 ; i += 3) {
            for (j = 1 ; j <= 7 ; j += 3) {
                if (subGridResult != checkSubGrid(i, j)) {
                    rv = 0;
                    break;
                }
            }
            if (rv == 0)
                break;
        }
    }
    if (rv == 1)
        printf("Successful :) \n");
    else
        printf("Must check again! :( \n");
    return 0;
}
