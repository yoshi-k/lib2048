#include "nc2048.h"


game* init(const int size){
  /*Initializes all of the variables. */
  game* retGame=newGame(size, 666);

  /*Ncurses init*/
  initscr(); 
  noecho(); //do not print keyboard input
  keypad(stdscr, 1); // read cursor keys
  curs_set(0);

  return retGame;

}



void displayField(const game* const state){
  /*Displays field, uses ncurses. */
  const int offset=4; 
  erase();
  mvprintw(2, 2, "Points %i", state->points);
  mvprintw(2, 20, "2048 hack");
  if(state->message!=NULL) mvprintw(2, 40, "%s", state->message);

  for(int i=0; i < state->size; i++){
    for(int j=0; j< state->size; j++){

#ifndef DEBUG
      if(state->field[pos(i, j, state->size)]<0){
	mvprintw(j*3 + offset, i*5 + offset, "|    |");
	continue;
      }
#endif
      mvprintw(j*3 + offset -1, i*5 + offset, "+----+");
      mvprintw(j*3 + offset, i*5 + offset, "|%4i|", state->field[pos(i, j, state->size)]);
      mvprintw(j*3 + offset +1, i*5 + offset, "+----+");
    }
  }


  mvprintw(state->size * 3 + offset + 5, 2, "state->high %i state->openFields %i tstBookkeeping %i", state->highestStone, state->openFields, tstBookkeeping(state));
  refresh();
}



int main( int argc, char* argv[]){

  game* state=init(3);

#ifdef DEBUG

  int xStones[]={0, 1, 7, 1, 5, 8, 1, 8, 0, 1, -1};
  int yStones[]={5, 5, 5, 6, 6, 6, 7, 3, 4, 4, -1};



  for(int *i=xStones, *j=yStones; *i!=-1; i++, j++){
    state->field[pos(*i, *j, state->size)]=2;
  }

  //  for(int i=0; i<20; i++)cAddStone(state);



  displayField(state);
  getch();
  
  for(int y=0; y<state->size; y++){
    state->points=moveColDown(state, y);

    displayField(state);
    getchar();
  }

#endif  

  //  state=newGame(9);
  cAddStone(state);
  cAddStone(state);



  int command=0;
  int result=0;
  int done=0;

  while(!done){
    displayField(state);
    command=getch();
    
    switch (command){
    case KEY_UP:
      result=moveUp(state);
      break;
    case KEY_DOWN:
      result=moveDown(state);
      break;
    case KEY_LEFT:
      result=moveLeft(state);
      break;
    case KEY_RIGHT:
      result=moveRight(state);
      break;
    case 'q':
      done=1;
      break;
    }

    if(result!=0)cAddStone(state);
    if(victory(state, 2048)){
      //done=1;
      state->message=strdup("Victory!!!");
    } else if(defeat(state)){
      //done=1;
      game* try=cpyGame(state);
      // state->message=strdup("Game Over!");
      if(moveUp(try)==0 && moveDown(try)==0 && moveLeft(try)==0 && moveRight(try)==0) state->message=strdup("Game Over!");
      delGame(try);
    }

      
  }

  delGame(state);
  endwin();
  return EXIT_SUCCESS;

}
