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

typedef struct
{
    int sum_value;
} answers;

/* print puzzle */
#define LINE "====================== \n"
#define COLLINE "\n___________________________________ \n"
#define NAME "||  SUM   CHECKER  || \n"
void print_grid(int grid[10][10]) {
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

// check row
void * checkRow(void * param) {
    int i, j;
    int tmpRowSum = 0;
    int rowSum = 0;
    answers * ans = (answers *) malloc(sizeof(answers));
    for (i=1;i<10;i++){
        tmpRowSum = 0;
        for(j=1;j<10;j++) {
            tmpRowSum += puzzle[i][j];
        }
        if (rowSum == 0)
            rowSum = tmpRowSum;
        else if (tmpRowSum != rowSum) {
            ans->sum_value = 0;
            return (void *) ans;
        }
    }
    ans->sum_value = rowSum;
    return (void *) ans;
}

// check col
void * checkCol(void * param) {
    int i, j;
    int tmpColSum = 0;
    int colSum = 0;
    answers * ans = (answers *) malloc(sizeof(answers));
    for (i=1;i<10;i++){
        tmpColSum = 0;
        for(j=1;j<10;j++) {
            tmpColSum += puzzle[j][i];
        }
        if (colSum == 0)
            colSum = tmpColSum;
        else if (tmpColSum != colSum) {
            ans->sum_value = 0;
            return (void *) ans;
        }
    }
    ans->sum_value = colSum;
    return (void *) ans;
}

void * checkSubGrid(void * param) {
    parameters * input_data = (parameters *) param;
    int i, j;
    int tmpSum = 0;
    for (i = input_data->x; i < input_data->x + 3; i++) {
        for (j = input_data->y; j < input_data->y + 3; j++) {
            tmpSum += puzzle[i][j];
        }
    }
    answers * ans = (answers *) malloc(sizeof(answers));
    ans->sum_value = tmpSum;
    return (void *) ans;
}

parameters * createParameter(int x, int y, int thread_number) {
    parameters * param = (parameters *) malloc(sizeof(parameters));
    param->x = x;
    param->y = y;
    param->thread_number = thread_number;
    return param;
}

int main(int argc, char* argv[]) {
    int rv = 1; // flag to check answer
    // input the sudoku file
    SetPuzzle("D:\\ash13\\Documents\\CLionProjects\\untitled\\test2.txt");

    // added code
    int i, j;
    // setup parameter for multithread
    parameters * parameterSet[NUMBER_OF_THREADS];
    parameterSet[0] = createParameter(1, 1, 0);
    parameterSet[1] = createParameter(1, 1, 1);
    int k = 2; // variable for thread count
    for (i = 1 ; i <= 7 ; i += 3) {
        for (j = 1 ; j <= 7 ; j += 3) {
            parameterSet[k] = createParameter(i, j, k);
            k += 1;
        }
    }
    pthread_t thread_list[NUMBER_OF_THREADS]; // array storing created thread
    answers * result[NUMBER_OF_THREADS]; // array storing thread result

    // initialize thread
    pthread_create(&thread_list[0], NULL, checkRow, (void *) parameterSet[0]);
    pthread_create(&thread_list[1], NULL, checkCol, (void *) parameterSet[1]);
    for (i = 2; i < NUMBER_OF_THREADS; i ++) {
        pthread_create(&thread_list[i], NULL, checkSubGrid, (void *) parameterSet[i]);
    }
    // join created thread
    for (i = 0; i < NUMBER_OF_THREADS; i ++) {
        pthread_join(thread_list[i], (void *) &result[i]);
    }

    // check if sum is same
    int correctSum = result[0]->sum_value;
    for (i = 1; i < NUMBER_OF_THREADS; i ++) {
        if (result[i]->sum_value != correctSum) {
            rv = 0;
            break;
        }
    }
    // check rv value
    if (rv == 1)
        printf("Successful :) \n");
    else
        printf("Must check again! :( \n");
    return 0;
}
