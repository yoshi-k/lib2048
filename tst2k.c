#include "2048.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

game* rndState(int size, unsigned int seed, float occupation){
  // Returns a game, with occupation fraction of size^2 filled.

  game* state= newGame(size, seed);
  unsigned int rseed=rand_r(&seed);
  
  for(int x=0; x<state->size; x++){
    for(int y=0; y<state->size; y++){
      if(rand_r(&rseed)< RAND_MAX * occupation) state->field[pos(x, y, state->size)]=stoneValue(state);
    }
  }

    int high=0;
    int clearf=0;
    for(int x=0; x<state->size; x++){
      for(int y=0; y<state->size; y++){
	if(state->field[pos(x, y, state->size)]==-1) clearf++;
	if(state->field[pos(x, y, state->size)]>high) high=state->field[pos(x, y, state->size)];
      }
  }
    state->highestStone=high;
    state->openFields=clearf;

    return state;
}


unsigned int fuzzMove(unsigned int rseed){
  for(int counter=0; counter < 1e4; counter++){


    game* state=rndState(floor(36.*rand_r(&rseed)/(float)RAND_MAX), rand_r(&rseed), rand_r(&rseed)/(float)RAND_MAX);
    game* backUp=cpyGame(state);
    moveUp(state);
    if(tstBookkeeping(state)!=0) printf("moveUp bookkeeping error %i\n", tstBookkeeping(state));
    if(tstScore(state, backUp)!=0) printf("move Up tstScore error %i\n", tstScore(state, backUp));
    
    delGame(state);
    state=cpyGame(backUp);
    moveDown(state);
    if(tstBookkeeping(state)!=0) printf("moveDown bookkeeping error %i\n", tstBookkeeping(state));
    if(tstScore(state, backUp)!=0) printf("moveDown tstScore error %i\n", tstScore(state, backUp));
    
    delGame(state);
    state=cpyGame(backUp);
    moveLeft(state);
    if(tstBookkeeping(state)!=0) printf("moveLeft bookkeeping error %i\n", tstBookkeeping(state));
    if(tstScore(state, backUp)!=0) printf("moveLeft tstScore error %i\n", tstScore(state, backUp));
    
    delGame(state);
    state=cpyGame(backUp);
    moveRight(state);
    if(tstBookkeeping(state)!=0) printf("moveRight bookkeeping error %i\n", tstBookkeeping(state));
    if(tstScore(state, backUp)!=0) printf("moveRight tstScore error %i\n", tstScore(state, backUp));
    
    if(counter% 1000 == 0)printf("Loop %i passed. \n", counter);

    delGame(state);
    delGame(backUp);
  }

return rseed;
}

int main(int argc, char* argv[]){


  unsigned int rseed=22356;

  rseed=fuzzMove(rseed);

  for(int counter=0; counter < 10; counter++){


  }
  
}
