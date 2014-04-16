/***********************************************
 * 2048 C library 
 * Author Joerg Kulbartz yoshi@yomols.de
 * 
 ***********************************************/
#include "2048.h"

/*****************************
Ideas & Todo:
1. Real RNG

5. Write unit tests for the library functions.
*****************************/

game* newGame(const int size, const unsigned int seed){

  game* retGame=malloc(sizeof(game));
  retGame->size=size;
  retGame->points=0;
  retGame->field=(int*)malloc(size*size*sizeof(int));
  memset(retGame->field, -1, size*size*sizeof(int));
  retGame->message=NULL;
  retGame->highestStone=0;
  retGame->openFields=size*size;
  retGame->seed=seed;
 

  return retGame;
}

void delGame(game* state){
  /*Game Destructor*/
  free(state->field);
  free(state->message);
  free(state);
}

game* cpyGame(const game* const state){

  game* cpy=newGame(state->size, state->seed);
  memcpy(cpy->field, state->field, state->size* state->size*sizeof(int));

  if(state->message != NULL){
    cpy->message=malloc( (strlen(state->message)+1)* sizeof(char));
    strncpy(cpy->message, state->message, strlen(state->message)+1);
  }

  cpy->points=state->points;
  cpy->highestStone=state->highestStone;
  cpy->openFields=state->openFields;

  return cpy; 
}

extern inline int pos(const int x,const int y, const int size){return x+y*size;}
 
int stoneValue( game* const state){
  /* Added value for stones.*/
  float isFour=( (float)rand_r(&(state->seed)) / (float)RAND_MAX);
  return (isFour - .1)>0?2:4;
}

void cAddStone( game* const state){
  /*Computer move, add a stone at a random empty field. With value given by stoneValue()*/
  int done=0;
  int addVal=stoneValue(state);

  if(state->openFields == 0) return; 

  while(!done){
    int x=rand_r(&(state->seed))*(state->size/(float)RAND_MAX);
    int y=rand_r(&(state->seed))*(state->size/(float)RAND_MAX);

    if(state->field[pos(x, y,state->size)]==-1){
      state->field[pos(x,y,state->size)]=addVal;
      done=1;
    }
  }

  state->openFields--;

  if(state->highestStone < addVal)state->highestStone=addVal;
}


int moveStone(game* const state, const int x, const int y, const int oldx, const int oldy){
  int err=0;
  if(x==oldx && y==oldy)return 0;//Nothing to do

  /*All values inbounds. */
  if(x<0 || x >= state->size)return -2;
  if(y<0 || y >= state->size)return -2;
  if(oldx<0 || oldx >= state->size)return -3;
  if(oldy<0 || oldy >= state->size)return -3;

  if(state->field[pos(oldx, oldy, state->size)]==-1) return -1;//No stone to move
  if(state->field[pos(x, y, state->size)]!=-1){
    err=state->field[pos(x,y, state->size)]-state->field[pos(oldx, oldy, state->size)];
    if(err!=0) return -4;//Incompatible stones
    state->field[pos(x, y, state->size)]+=state->field[pos(oldx, oldy, state->size)];
    state->field[pos(oldx, oldy, state->size)]=-1;
    err=state->field[pos(x, y, state->size)];//Return value equals new stone value if added.
    state->points+=err;
    state->openFields+=1;
    if(err > state->highestStone)state->highestStone=err;
  } else {//Move to empty position.
    state->field[pos(x,y, state->size)]=state->field[pos(oldx, oldy, state->size)];
    state->field[pos(oldx, oldy, state->size)]=-1;
  }


  return err;
}

int moveStoneRight(game* const state, const int x, const int y){

  return moveStone(state, x+1, y, x, y);
}

int moveLnRight(game* const state, const int y){

  int counter=0;

  for(int x=state->size -1; x >=0; x--){
    if(state->field[pos(x, y, state->size)]> 0){
      int runx=x;
      while( moveStoneRight(state, runx, y)==0)runx++;
      if(state->field[pos(x, y, state->size)]==-1) counter++;
    }
  }

  return counter;
}


int moveRight(game* const state){

  int count=0;//Number of blocks moved.
  for(int y=0; y<state->size;y++){//For each line
    count+=moveLnRight(state, y);
  }//For loop over lines
  
  
  return count;
}

int moveStoneLeft(game* const state, const int x, const int y){

  return moveStone(state, x-1, y, x, y);
}

int moveLnLeft(game* const state, const int y){

  int counter=0;

  for(int x=0; x < state->size; x++){
    if(state->field[pos(x, y, state->size)]>0){
      int runx=x;
      while( moveStoneLeft(state, runx, y)==0)runx--;
      if(state->field[pos(x, y, state->size)]==-1) counter++;
    }
  }
  return counter; 
}


int moveLeft(game* const state){

  int count=0;
  for(int y=0; y<state->size;y++){
    count+=moveLnLeft(state, y);
  }

  return count;
}

 

int moveStoneUp(game* const state, const int x, const int y){

  return moveStone(state, x, y-1, x, y);
}

int moveColUp(game* const state, const int x){

  int counter=0;

  for(int y=0; y < state->size; y++){
    if(state->field[pos(x, y, state->size)]>0){
      int runy=y;
      while( moveStoneUp(state, x, runy)==0)runy--;
      if(state->field[pos(x, y, state->size)]==-1) counter++;
    }
  }
  return counter;
}


int moveUp(game* const state){

  int count=0;//Number of blocks moved.
  for(int x=0; x<state->size; x++){
    count+=moveColUp(state, x);
  }

  return count;
}

int moveStoneDown(game* const state, const int x, const int y){

  return moveStone(state, x, y+1, x, y);
}

int moveColDown(game* const state, const int x){

  int counter=0;

  for(int y=state->size -1; y >= 0; y--){
    if(state->field[pos(x, y, state->size)]>0){
      int runy=y;
      while( moveStoneDown( state, x, runy)==0)runy++;
      if(state->field[pos(x, y, state->size)]==-1) counter++;
    }
  }
  return counter;
}

int moveDown(game* const state){

  int count=0;
  for(int x=0; x<state->size;x++){
    count+=moveColDown(state, x);
  }

  return count;
}

int victory(const game* state, int victoryCond){

  if(state->highestStone >= victoryCond){return 1;}
  else {return 0;}

}

int defeat(const game* state){

  if(state->openFields==0){ return 1;}
  else{ return 0;}

}

int tstBookkeeping(const game* state){

  int high=0;
  int clearF=0;
  int err=0;

  for(int x=0; x<state->size; x++){
    for(int y=0; y<state->size; y++){
      if(state->field[pos(x, y, state->size)] > high) high=state->field[pos(x, y, state->size)];
      if(state->field[pos(x, y, state->size)] == -1) clearF++;
    }
  }

  if(state->highestStone != high)err |= 1;
  if(state->openFields != clearF)err |= 2;

  return err;
}


int tstScore(const game* state, const game* backUp){

  if(state->size != backUp->size) return -1;

  int score=0;
  int oldScore=0;

  for(int x=0; x<state->size; x++){
    for(int y=0; y<state->size; y++){
      score+=state->field[pos(x, y, state->size)];
      oldScore+=state->field[pos(x, y, backUp->size)];
    }
  }

  return score-oldScore; //Return 0 on success, for consistency with tstBookkeeping.
}
