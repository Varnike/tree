#ifndef AKINATOR_H
#define AKINATOR_H

#include "tree.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/stack.h"
#include "include/onegin.h"

/*
 * 3)TODO Akinator shit
 * 4)TODO Fix warnings + log files
 */
enum ANSWER_STATE {
	POS_ANSW =  1,
	NEG_ANSW =  0,
	INV_ANSW = -1
};

enum search_node_flags {
	NODE_NOT_FOUND = 0,
	NODE_FOUND     = 1
};

enum AKINATOR_MODE {
	AKN_EXIT  = 0,
	AKN_GUESS = 1,
	AKN_FIND  = 2,
	AKN_CMP   = 3,
	AKN_SAVE  = 4
};

const int INPUT_BUFF_SIZE = 128;

int AkinatorGuess(TNODE *tree, Stack *call_nodes);
int AkinatorProcess(TNODE *tree);
int AkinatorInit(TNODE **tree, textBuff *btext);
int AkinatorSave(TNODE *tree);
int AkinatorFind(TNODE *tree);
int AkinatorCompare(TNODE *tree);
int AkinatorSave(TNODE *tree);

#endif // AKINATOR_H
