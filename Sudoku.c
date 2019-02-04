#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <pthread.h>
//This is the number i had defined which is 27 and i'm using 27 threads to check the suduko

#define num_threads 27


//This is the validit array that I'll use to check the sudoko
int validit[num_threads] = {0};

// This is the struct which I'm using to store the data like row and column to pass

typedef struct {

  int row;

  int column;

} parameters;

int sudoku[9][9];//This is the array being used to store elements on every position of suduko
// This method is to check whether row are unique or not
void *RowChecker(void* param) {
  // This is just for validation
  parameters *params = (parameters*) param;
  int row = params->row;
  if (row > 8) {
    fprintf(stderr, "Row is Invalid in term of size :(");
    pthread_exit(NULL);
  }
  // Program after this will check if number 1-9 appear once in a row or not and it will print the row if they don't
  int validity[9] = {0};
  int j;
  for (j = 0; j < 9; j++) {
    // If the corresponding index for the number is set to 1 in validity array than thois array will not be updated and it will be sent back with a message
    int nbr = sudoku[row][j];
    if (nbr < 1 || nbr > 9 || validity[nbr - 1] == 1) {
      fprintf(stderr, "The word is repeated in row=%d therefore, it's not a right SUDOKU :( \n", (row+1));
      pthread_exit(NULL);
    }
    else {
      validity[nbr - 1] = 1;
	}
  }
  // If the program reach here than it is successfull
  //I'm using all the index after 18 for row in validit array to put equal to 1
  validit[18 + row] = 1;
  pthread_exit(NULL);
}
// ColumnChecker is designed to check if numbers 1 to 9 appear once in the column or not
void *ColumnChecker(void* param) {
  // This is just to confirm
  parameters *params = (parameters*) param;
  int column = params->column;
  //This if statement is to check if rows and columns are of valid size mean they are of 9 numbers each
  if (column > 8) {
    fprintf(stderr, "Column are not of valid size");
    pthread_exit(NULL);
  }
  // This part of the code will check whether number 1-9 occur once in the column or not
  int validity[9] = {0};
  int j;
  for (j = 0; j < 9; j++) {
    int nbr = sudoku[j][column];
    if (nbr < 1 || nbr > 9 || validity[nbr - 1] == 1) {
      fprintf(stderr, "The word in row=%d and col=%d is repeated in the column therefore, it's not a right SUDOKU :( \n", (j+1), (column+1));
      pthread_exit(NULL);
    }
    else {
      validity[nbr - 1] = 1;
    }
  }
  // If the program passes through the for loop than it means that column are okay
  //I'm using indexes from 9 to 18 for column to qual to 1 as a checker
  validit[9 + column] = 1;
  pthread_exit(NULL);
}
// This method will check if all the numbers 1-9 are unique in 3x3 matrix or not
void *B3x3BlockChecker(void* param) {
  // That's for validation
  parameters *params = (parameters*) param;
  int row = params->row;
  int column = params->column;
  //If statement to check that row and columns are of legal size
  if (row > 6 || row % 3 != 0 || column > 6 || column % 3 != 0) {
    fprintf(stderr, "Ether row or column are invalid size:(");
    pthread_exit(NULL);
  }
  int validity[9] = {0};
  int i,j;
  //This loop will check whether the 3x3 block have unique number or not
  for (i = row; i < row + 3; i++) {
    for (j = column; j < column + 3; j++) {
      int nbr = sudoku[i][j];
      if (nbr < 1 || nbr > 9 || validity[nbr - 1] == 1) {
        fprintf(stderr, "The word on row=%d and col=%d is repeated in 3x3 block therefore, it's not a right SUDOKU :( \n", (i+1), (j+1));
        pthread_exit(NULL);
      }
      else {
        validity[nbr - 1] = 1;
      }
    }
  }
  // If the program compile here successfully than this 3x3 is valid
  validit[row + column/3] = 1; // I have left first 9 indexes for the 3x3 blocks
  pthread_exit(NULL);
}

int main() {
  FILE *fle;
  char c[100];
  if((fle = fopen("Sud_Sol.txt", "r")) == NULL){
    printf("ERROR IN OPENING");
  }
  int i,j;
  for(i=0;i<9;i++){
    for(j=0;j<9;j++){
      fscanf(fle, "%s", c);
      int abd=atoi(c);
      sudoku[i][j]=abd;
    }
  }
  fclose(fle);

  pthread_t threads[num_threads];
  int thrdIndex = 0;
  // This program had creted 27 threads in total
  // 9 for rows
  // 9 for columns
  // 9 for 3x3 blocks
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      //This program will create 3x3 check through threads it will create 9 threads to check it
      if (i%3 == 0 && j%3 == 0) {
        parameters *Data = (parameters *) malloc(sizeof(parameters));
        Data->row = i;
        Data->column = j;
        int hg=pthread_create(&threads[thrdIndex++], NULL, B3x3BlockChecker, Data);
      }
      //This will create threads to check rows
      if (j == 0) {
        parameters *rowdata = (parameters *) malloc(sizeof(parameters));
        rowdata->row = i;
        pthread_create(&threads[thrdIndex++], NULL, RowChecker, rowdata);
      }
      //This will create threads to check column
      if (i == 0) {
        parameters *columndata = (parameters *) malloc(sizeof(parameters));
        columndata->column = j;
        pthread_create(&threads[thrdIndex++], NULL, ColumnChecker, columndata);
      }
    }
  }
  //This will join all the threads
  int k;
  for (k = 0; k < num_threads; k++) {
    pthread_join(threads[k], NULL);
  }
  // This will check the array if any value is 0 than its invalid
  int h;
  for (h = 0; h < num_threads; h++) {
    if (validit[h] == 0) {
      printf("Sorry Sudoku is invalid :( \n");
      return EXIT_SUCCESS;
    }
  }
  printf("Congrats!!!! Sudoku is valid :) \n");
  return EXIT_SUCCESS;
}

