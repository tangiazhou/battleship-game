#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define BOARDROWS 9
#define BOARDCOLS 11

bool dumpComputer = false; 

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
            else  
                break;
        }
        else { 
            row=getRand(1,BOARDROWS);
            col=getRand(1,BOARDCOLS);
            if(compShotBoard[row][col]==0) { 
                compShotBoard[row][col]=1; 
                break;
            }
        }
    } 
    
    if(board[row][col] != 0){
        if(board[row][col]>0)
            board[row][col]= -1*board[row][col]; 
        return -1*board[row][col]; 
    }
    return 0; 
}
            
bool allShipsNotHit(int board[BOARDROWS+1][BOARDCOLS+1]){
    for(int i=1;i<=BOARDROWS;i++){
        for(int j=1;j<=BOARDCOLS;j++){
            if(board[i][j]>0) 
                return(true);
        }
    }
    return(false); 
}

bool noneLeft(int valueToFind,int board[BOARDROWS+1][BOARDCOLS+1]){
    for(int i=1;i<=BOARDROWS;i++){
        for(int j=1;j<=BOARDCOLS;j++){
            if(board[i][j]==valueToFind) 
                return(false);
        }
    }
    return(true); 
}

bool validInput(int row, int column, int orientation, int size) { 
    if(row < 1 || row > BOARDROWS || column < 1 || column > BOARDCOLS || orientation < 0 || orientation > 1) {
        return(false); 
    }
    else if(orientation == 0) {
        if(column-1+size > BOARDCOLS) { 
            return(false);
        }
    }
    else if(orientation == 1) {
        if(row-1+size > BOARDROWS) { 
            return(false);
        }
    }
    return(true); 
}

void getBoardParameters(bool getUserInput, int size, int boardParameters[]) {
    int row; 
    int column;
    int orientation;
    bool valid;
  
    do {
        if(getUserInput == true) { 
          printf("Give starting row, starting column and orientation (3 inputs) for ship of size=%d:", size);
          scanf("%d %d %d", &row, &column, &orientation); 
        } 
        else {
          row = getRand(1,9); 
          column = getRand(1,11);
          orientation = getRand(0,1);
        }
        valid = validInput(row, column, orientation, size);
        
        if(valid == false && getUserInput == true) {
              printf("Invalid Input\n");
        }
    } while(valid == false);   

    boardParameters[0] = row;  
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
    int boardTemp[BOARDROWS+1][BOARDCOLS+1]={0}; 
    int overlap;
    
    if(getUserInput == true) { 
        printf("Rows are 1 - 9, Columns are 1 - 11\n");
        printf("Orientation is 0 for across, 1 for down\n");
    }
    for(size=5; size>0; size--) {
        do {
            overlap = 0; 
            
            for(i=1; i<=BOARDROWS; i++) {
                for(j=1; j<=BOARDCOLS; j++) {
                    boardTemp[i][j]=0; 
                }  
            }         
            getBoardParameters(getUserInput,size,boardParameters); 
            row = boardParameters[0];
            column= boardParameters[1];
            orientation= boardParameters[2]; 
            
            if(orientation == 0) { 
                for(i=0; i<size; i++) { 
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
        
        for(i=1; i<=BOARDROWS; i++) { 
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
    
     	srand(time(NULL)); 

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

    playerBoardOK=true;
    computerBoardOK=true;
    while(playerBoardOK && computerBoardOK){
        int hit=getShot(true,computerBoard); 
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
   }
   
   if(playerBoardOK)
       printf("You win!\n");
    else
        printf("You lose.\n");
    showBoard(computerBoard);

	return 0;
}
