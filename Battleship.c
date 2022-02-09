#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define BOARDROWS 9
#define BOARDCOLS 11

bool dumpComputer = false; //causes dump of computer board before game if true (for debugging)

// boards. 0 means empty, non-zero means ship is there, number is size, positive means hit on a ship
int playerBoard[BOARDROWS+1][BOARDCOLS+1];
int computerBoard[BOARDROWS+1][BOARDCOLS+1];
int compShotBoard[BOARDROWS+1][BOARDCOLS+1];

int getRand(int lowval,int highval){
    return (rand()%(highval+1-lowval) + lowval);
}

int getShot(bool fromUser, int board[BOARDROWS+1][BOARDCOLS+1]){
    int row,col;
    
    while(1){
        if(fromUser){
            printf("Give a shot (row, col):");
            scanf("%d%d",&row,&col);
            if(col<1 || col > BOARDCOLS || row < 1 || row > BOARDROWS)
                printf("Invalid input\n");
            else  //valid input
                break;
        }
        else { //computer generated
            row=getRand(1,BOARDROWS);
            col=getRand(1,BOARDCOLS);
            if(compShotBoard[row][col]==0) { //make sure haven't shot here before
                compShotBoard[row][col]=1; //valid shot
                break;
            }
        }
    } //will leave this loop with valid input
    
    if(board[row][col] != 0){
        if(board[row][col]>0)
            board[row][col]= -1*board[row][col]; //make sure is negative
        return -1*board[row][col]; //a hit!
    }
    return 0; //miss
}
            
bool allShipsNotHit(int board[BOARDROWS+1][BOARDCOLS+1]){
    for(int i=1;i<=BOARDROWS;i++){
        for(int j=1;j<=BOARDCOLS;j++){
            if(board[i][j]>0) //ship and not hit
                return(true);
        }
    }
    return(false); // No ships found not all hit
}

bool noneLeft(int valueToFind,int board[BOARDROWS+1][BOARDCOLS+1]){
    for(int i=1;i<=BOARDROWS;i++){
        for(int j=1;j<=BOARDCOLS;j++){
            if(board[i][j]==valueToFind) // Ship and not hit
                return(false);
        }
    }
    return(true); //no ships found, all hit
}

bool validInput(int row, int column, int orientation, int size) { 
    if(row < 1 || row > BOARDROWS || column < 1 || column > BOARDCOLS || orientation < 0 || orientation > 1) {
        return(false); // User input numbers are not valid
    }
    else if(orientation == 0) {
        if(column-1+size > BOARDCOLS) { // Falls off the board
            return(false);
        }
    }
    else if(orientation == 1) {
        if(row-1+size > BOARDROWS) { // Falls off the board
            return(false);
        }
    }
    return(true); // Returns boolean type
}

void getBoardParameters(bool getUserInput, int size, int boardParameters[]) { // Give the size and returns 3 values
    int row; 
    int column;
    int orientation;
    bool valid;
  
    do {
        if(getUserInput == true) { // User will manually place their ships
          printf("Give starting row, starting column and orientation (3 inputs) for ship of size=%d:", size);
          scanf("%d %d %d", &row, &column, &orientation); 
        } 
        else {
          row = getRand(1,9); // Calling the getRand function
          column = getRand(1,11);
          orientation = getRand(0,1);
        }
        valid = validInput(row, column, orientation, size); // Validating user input
        
        if(valid == false && getUserInput == true) { // Only prints when it asks the user to get input
              printf("Invalid Input\n");
        }
    } while(valid == false); // Continues to loop when the input is not valid   

    boardParameters[0] = row; // Assign row to the first boardParameters component 
    boardParameters[1] = column; 
    boardParameters[2] = orientation; 
}

void populateBoard(bool getUserInput, int board[BOARDROWS+1][BOARDCOLS+1]) {
    int i; 
    int j; 
    int row;
    int column;
    int orientation; 
    int size;
    int boardParameters[3];
    int boardTemp[BOARDROWS+1][BOARDCOLS+1]={0}; // Used for determining conflict 
    int overlap;
    
    if(getUserInput == true) { // User will manually place their ships
        printf("Rows are 1 - 9, Columns are 1 - 11\n");
        printf("Orientation is 0 for across, 1 for down\n");
    }
    for(size=5; size>0; size--) { // Ship size starts at 5 and ends at 1
        do {
            overlap = 0; 
            
            for(i=1; i<=BOARDROWS; i++) {
                for(j=1; j<=BOARDCOLS; j++) {
                    boardTemp[i][j]=0; // Initialize boardTemp to 0
                }  
            }         
            getBoardParameters(getUserInput,size,boardParameters); // Obtain parameters either from user input or random generation 
            row = boardParameters[0];
            column= boardParameters[1];
            orientation= boardParameters[2]; 
            
            if(orientation == 0) { // Horizontal orientation
                for(i=0; i<size; i++) { // Populate the board and detect if conflicts occur
                    boardTemp[row][column+i]=size; 
                    overlap += board[row][column+i]; 
                }
            } 
            else { // Vertical orientation
                for(i=0; i<size; i++) {
                    boardTemp[row+i][column]=size; 
                    overlap += board[row+i][column];
                }
            }
            if(overlap != 0 && getUserInput == true) {
                printf("Conflicts with ship already placed\n");
            }
            
        } while(overlap != 0);    
        
        for(i=1; i<=BOARDROWS; i++) { // Once it exits the loop, adds components of the board and boardTemp to board
            for(j=1; j<=BOARDCOLS; j++) {
              board[i][j] += boardTemp[i][j];
            }  
        }    
    }
}
    
void showBoard(int board[BOARDROWS+1][BOARDCOLS+1]){
    printf("   Cols\n");
    printf("    1  2  3  4  5  6  7  8  9 10 11\n");
    printf("     ________________________________\n");
    for(int i=1;i<=BOARDROWS;i++){
        printf("R%d|",i);
        for(int j=1;j<=BOARDCOLS;j++){
            if(board[i][j]>=0)
                printf(" ");
            printf("%d ",board[i][j]);
        }
        printf("|\n");
    }
    printf("  __________________________________\n");
}

int main(int argc, char **argv)
{
    bool playerBoardOK,computerBoardOK;
    
    // Initialize the seed randomly. 
     	srand(time(NULL)); 

    // Initialize the boards
    for(int i=0;i<BOARDROWS;i++){
        for(int j=0;j<BOARDCOLS;j++){
            playerBoard[i][j]=0;
            computerBoard[i][j]=0;
            compShotBoard[i][j]=0;
        }
    }

    populateBoard(true,playerBoard);
    populateBoard(false,computerBoard);
    printf("Your board is\n");
    showBoard(playerBoard);
    if(dumpComputer){
        printf("\n\nComputer board is\n");
        showBoard(computerBoard);
    }

    // play starts here
    playerBoardOK=true;
    computerBoardOK=true;
    while(playerBoardOK && computerBoardOK){
        int hit=getShot(true,computerBoard); //shot from user
        if(hit){
            printf("HIT on size %d\n",hit);
            if(noneLeft(hit,computerBoard))
                printf("Sunk computer's %d\n",hit);
        }
        else
            printf("MISS!!\n");
        hit=getShot(false,playerBoard);
        if(hit){
            printf("Computer hits! Size=%d\n", hit);
            if(noneLeft(hit,playerBoard))
                printf("Sunk your %d\n",hit);
        }
        else
            printf("Computer miss!\n");
        playerBoardOK = allShipsNotHit(playerBoard);
        computerBoardOK = allShipsNotHit(computerBoard);
//    showBoard(playerBoard); //debug
//    showBoard(computerBoard); //debug
   }
   
   if(playerBoardOK)
       printf("You win!\n");
    else
        printf("You lose.\n");
    showBoard(computerBoard);

	return 0;
}