#ifndef TREE_H
#define TREE_H

#include "include/error.h"

#include <cstring>
#include <stdio.h>
#include <cassert>
#include <cstdlib>
#include <typeinfo>
#include <stdint.h>
#include <inttypes.h>


//TODO 1) ifdef check
//TODO 2) ERR CHECK
//TODO 3) diff types outpust
//TODO 4) html dump
//TODO 5) TREE struct?
//TODO 6) func pointer?
//TODO 7) recursion?
//TODO 8) struct size
//TODO 9) обертка для dtor
#define $ printf("\t\t\t---ON LINE %d IN FUNCTION %s---\n", __LINE__, __func__);

#define TREE_CHECK(ret_val)					\
	{							\
	}			
typedef char* tval_t;

struct TNODE {
	tval_t data;
	TNODE *left;
	TNODE *right;
};

struct TREE {
	TNODE *root;
	int mode;
};

enum TREE_SIDE {
	LEFT  = 0,
	RIGHT = 1
};

int TreeInsert(TNODE *parent, int side, tval_t data);
void TreeTraversePre(TNODE *root, void (*visitor)(TNODE *node));
void TreeTraverseIn(TNODE *root, void (*visitor)(TNODE *node));
/*void TreeTraversePost(TNODE *root, void (*visitor)(TNODE *node));*/

int TreeCtor(TREE *tree, tval_t val);
void TreeDtor(TNODE *node);
int TreePrintNode(TNODE *node);

TNODE *TreeFind(TNODE *node);

void TreeDump(TNODE *node);
void TreeDotDump(TNODE *node);
void VisitPrint(TNODE *node);
#endif // TREE_H
