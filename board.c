// #include all necessary .h files here.
#include "board.h"
#include "cell.h"
#include "minesweep.h"

/*
  Board.c implemented by Magnus Graham. This code comprises the functions required to load the minesweep board for playing.
 */


/* print_game 
* args: pointer to the current board struct
* out: void
* prints the game board: 
    if cell color is white and number of mines is >0, print mine count otherwise
    if cell is white, print the mine count 0 otherwise
    if cell color is black 'B', print B otherwise, 
    if cell color is gray 'E'
*/
void print_board(game* board){
    printf("Board colors %d x %d\n", board->row_max, board->col_max);
    for(int i=0; i< board->row_max; i++){
        for(int j=0; j < board->col_max; j++){
            if(board->cells[i][j].color == white && board->cells[i][j].mine >0)
                printf("%d\t", board->cells[i][j].mine);
            else if (board->cells[i][j].color == white)
                printf("0\t");
            else if (board->cells[i][j].color == black)
                printf("B\t");
            else 
                printf("E\t");
        }
        printf("\n");
    }
    /* uncommnet this to print the board again showing the mine locations
     printf("Mines on the Board (-1: mine, 1: otherwise): %d x %d\n", board->row_max, board->col_max);
     for(int i=0; i< board->row_max; i++){
         for(int j=0; j < board->col_max; j++){
             printf("%d\t", board->cells[i][j].mine);
         }
         printf("\n");
	 }*/
}

game* load_board(char* input){

  game* gameboard = (game*)malloc(sizeof(game));// allocate size of gameboard
  
  if (gameboard == NULL) { //check for failure
    printf("Error allocating memory");
    exit(1); //exit program
  }

  FILE *boardinput; //open file
  boardinput =  fopen(input, "r");

  if (boardinput == NULL) { //check for failure
    printf("Unable to open file"); 
    exit(2); //exit program
  }

  int rows,cols;
  fscanf(boardinput,"%d %d", &rows, &cols); //get row and col values

  gameboard->row_max = rows;
  gameboard->col_max = cols;

  gameboard->cells = (cell**)malloc(rows*sizeof(cell*));//malloc a 2d array
  
  if (gameboard->cells == NULL) {
    printf("Not able to allocate cell memory");
    free(gameboard);
    fclose(boardinput);
    return NULL;
  }

  for (int i = 0; i < rows; i++){
    gameboard->cells[i] = (cell*)malloc(cols*sizeof(cell));
  }

  int currSpace, i, j;
  
  for (int i = 0; i < gameboard->row_max; i++){
    for (int j = 0; j < gameboard->col_max; j++){
      gameboard->cells[i][j].color = gray; //initialize grid to gray
      fscanf(boardinput, "%d", &currSpace); //grab current value from file

      if (currSpace == -1){ //analyze currentSpace and assign mine value
	gameboard->cells[i][j].mine = -1;
      }
      else if (currSpace == 0){
	gameboard->cells[i][j].mine = 0;
      }
      else if ((currSpace > 0) && (currSpace <= 8)) {
	gameboard->cells[i][j].mine = currSpace;
      }
      else {
	free_board(gameboard);
	printf("Error: Invalid Input.");
	fclose(boardinput);
	return NULL;
      }
    }
  }

 
  fclose(boardinput);
  return gameboard;
}
