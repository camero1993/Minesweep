#include "stdio.h"
#include "stdlib.h"

#include "minesweep.h"
#include "board.h"
#include "cell.h"

/*
  Implementation of minesweep.c by Magnus Graham. This code implements the functions which comprise the inner workings of the game minesweep.
 */

/* play_game
* args: f_name -- unchecked file name that contains board cofiguration
* out: int return game status if needed for the best score keeping
* creates the game instance
*  reads command line for click's x/y and click type (forever unless otherwise specified):
*  Enter row in range 1-board height: enter column range 1-board width click type(0: unvover 1: mine)
*  Example= 1:3 0 -- row=1 col=3 try uncover
*  Example= 2:1 1 -- row=2 col=1 mark as a mine
*  Enter '-1' to check if game is won and quit the game
*
*  call process_click with x/y and a move type
*  check the process_click return for the game result 
*/
int play_game(char* f_name){
    game* board = load_board(f_name);
    if(board == NULL){
        fprintf(stderr, "ERROR: Cound not allocate board or read from file");
        return EXIT_FAILURE;
    }
    int ret, r, c, m = 0;
    while(1){
        print_board(board);
        printf("Enter row(1-%d):column(1-%d) and click type(0: uncover 1: mark a mine)\n", board->row_max, board->col_max);
        printf("Example= 1:3 0 -- row=1 col=3 try uncover\n");
        printf("Example= 2:1 1 -- row=2 col=1 mark as a mine\n");
        printf("Enter '-1' to quit the game \n");

        scanf("%d:%d%d", &r, &c, &m);
        if(r==-1 || c==-1 || m==-1){
            if(check_game(board))
                printf("CONGRATS YOU WON!!!\n");    
            else
                printf("YOU LOST -- GAME OVER -- mines mis-identified\n");    
            break;
        }else if(r<1 || c<1 || r > board->row_max || c > board->col_max || m < 0 || m > 1)
            printf("Illegal move or click type, try again!\n");
        else{
            ret = process_click(board, --r, --c, m);
            if (ret == 0){
                printf("GAME OVER, you stepped on a mine or marked it as a mine\n");
                break;
            }else
                printf("Good one, keep on clicking\n");
        }

    }
    printf("Thank you for playing\n");
    free_board(board);
    return EXIT_SUCCESS;
}

int process_click(game* board, int row, int col, int move){

  if(move == 1){ //player marks mine
    board->cells[row][col].color = black;
    return 1; //click processed
  }
  else if(move == 0){ //player steps on space
    if(board->cells[row][col].mine == yes){ //if mine
      return 0; //game over
    }
    else {
      if(board->cells[row][col].color == gray){ //if gray, color to white
	uncover(board,row,col);
	return 1; //click processed
      }
      else if (board->cells[row][col].color == white){ //if white, do nothing
	return 1; //click processed
      }
    }
  }
}

int check_game(game* board){
  int val = 1;
  for (int i = 0; i<board->row_max; i++){
    for(int j = 0;j<board->col_max;j++){
      if (board->cells[i][j].mine == -1){
	if (board->cells[i][j].color != black){
	val = 0;
	}
      }
      else if (board->cells[i][j].mine > 0){
	if (board->cells[i][j].color == black){
	  val = 0;
	}
      }
    }
  }
  return val;
}

int uncover(game* board, int row, int col){
  printf("Uncover %d%d\n",row,col);
  //base case, out of bounds
   if (row < 0 || col < 0 || row >= board->row_max || col >= board->col_max){
     return 1;
  }

  //base case 2, space is marked as mine
  else if (board->cells[row][col].color == black){
    //if mine, return otherwise
    if (board->cells[row][col].mine < 0){
      printf("Base case 2a %d%d\n",row,col);
      return 1;
    }
    //if not mine, game over
    else {
      //game over
      printf("Base case 2b %d%d\n",row,col);
      return 0;
    }
  }

   //base case 3, if space is already white
  else if (board->cells[row][col].color == white){
    return 1;
    }
   
  //recursive case
  else {
    board->cells[row][col].color = white; //color current cell white
    printf("White: %d%d\n",row,col);
    
    if (board->cells[row][col].mine == 0){
      //only uncover surrounding spaces if mine is 0
    uncover(board,row-1,col-1);
    uncover(board,row-1,col);
    uncover(board,row-1,col+1);
    uncover(board,row,col-1);
    uncover(board,row,col+1);
    uncover(board,row+1,col-1);
    uncover(board,row+1,col);
    uncover(board,row+1,col+1);
    }
  }
}

void free_board(game* board){
  for (int i = 0; i < board->row_max; i++){
    for (int j =0;j < board->col_max;j++){
      free(board->cells[j]);
    }
    free(board);
  }
}
