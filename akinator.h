#ifndef AKINATOR_H
#define AKINATOR_H

#include "tree.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/stack.h"
#include "include/onegin.h"

/*
 * 1)TODO read from file
 * 2)TODO read sentences and clear input buff
 * 3)TODO Akinator shit
 * 4)TODO Fix warnings
 * 5)TODO Defines ?
 */
enum ANSWER_STATE {
	POS_ANSW =  1,
	NEG_ANSW =  0,
	INV_ANSW = -1
};

enum AKINATOR_MODE {
	AKN_EXIT    = 0,
	AKN_GUESS   = 1,
	AKN_OBJ_DEF = 2,
	AKN_CMP     = 3,
	AKN_SAVE    = 4
};

const int INPUT_BUFF_SIZE = 128;

int AkinatorGuess(TNODE *tree);
int AkinatorProcess(TNODE *tree);
int AkinatorInit(TNODE **tree);
int AkinatorSave(TNODE *tree);
//int AkinatorFullInfo(TNODE *tree);
int AkinatorCompare(TNODE *tree);
int AkinatorSave(TNODE *tree);

#endif // AKINATOR_H
