#include "2048.h"

#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <string.h>

game* init(const int size);/*?< Initializes the program */

void displayField(const game* const state);/*Displays the field. */ 
