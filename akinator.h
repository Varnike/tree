#ifndef AKINATOR_H
#define AKINATOR_H

#include "tree.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/stack.h"

//TODO perror
//#define ERRNUM_CHECK(ret_val) {if(ERRNUM) return ret_val;}
enum ANSWER_STATE {
	POS_ANSW =  1,
	NEG_ANSW =  0,
	INV_ANSW = -1
};
const int INPUT_BUFF_SIZE = 16;

int AkinatorGuess(TNODE *tree);
int AkinatorProcess(TNODE *tree);
int AkinatorInit(TNODE *tree);
int AkinatorSave(TNODE *tree);
int AkinatorFullInfo(TNODE *tree);
int AkinatorCompare(TNODE *tree);

#endif // AKINATOR_H
