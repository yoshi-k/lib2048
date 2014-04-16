#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  int* field;
  int size;
  int points;
  int highestStone;
  int openFields;
  unsigned int seed;
  char* message;
} game;

game* newGame(const int size, const unsigned int seed);/*?< Constructor for game of size^2.*/
void  delGame(game* state);/*?< Destructor for game.*/
game* cpyGame(const game* const state);/*?< Copy constructor. */

int pos(const int x,const int y, const int size);/*?< Returns the position of x,y coords.*/
int stoneValue(game* const state);/*Randomly asign 2 or 4 to a stone.*/
void cAddStone(game* const state);/*Computer adds a stone on a empty field at random. */ 
int moveStone(game* const state, const int x, const int y, const int oldx, const int oldy);/*Moves a stone from oldx,oldy to x,y NOTE that const game* const does only protect state->field and not the contents of field.

Error values:
-1 No stone to move
-2 Destination outside of board
-3 Initial position outside of board
-4 Incompatible stones

A return value of 0 indicates a move to a free field, a positive return value indicates a move to a occupied field.
 */
int moveStoneRight( game* const state, const int x, const int y);/* Moves one stone one field to the right.*/
int moveLnRight( game* const state, const int y);/*Moves a line to the right.*/
int moveRight( game* const state);/*Moves the entire field to the right. */
int moveStoneLeft( game* const state, const int x, const int y);/*Moves one stone one field to the left. */
int moveLnLeft( game* const state, const int y);/* Moves one line to the left.*/
int moveLeft( game* const state);/*Moves the entire field to the left.*/
int moveStoneUp( game* const state, const int x, const int y);/*Move one stone one field up. */
int moveColUp( game* const state, const int x);/*Moves a entire coloumn up.*/
int moveUp( game* const state);/*Moves the entire field up.*/
int moveStoneDown( game* const state, const int x, const int y);/*Moves one stone one field down.*/
int moveColDown( game* const state, const int x);/*Moves one coloumn down. */
int moveDown( game* const state);/*Moves the entire field down.*/ 
int victory(const game* state, int highest);/*Checks victory condition and updates clear fields and highestStone*/
int defeat(const game* state);/*Checks if no movement possible anymore.*/

/*Diagnostic functions.*/
int tstBookkeeping(const game* state);/*tests if highest and clear field are correct.*/
int tstScore(const game* state, const game* backUp);/*After a move the sum of all stones should stay the same, since the only way a stone vanishes is by adding to another stone of the same value. */ 
