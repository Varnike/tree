#ifndef TREE_H
#define TREE_H

#include "include/error.h"
#include "config.h"

#include <cstring>
#include <stdio.h>
#include <cassert>
#include <cstdlib>
#include <typeinfo>
#include <stdint.h>
#include <inttypes.h>


//TODO 4) html dump
//TODO 5) TREE struct?
//TODO 6) func pointer?
//TODO 7) recursion?
//TODO 8) struct size

//#define NODE_BOTH_CHILD_CHECK
#define VALIDATE_TREE_INTEGRY
//#define TREE_CHECK_DUMP

#define DEBUG 1

#define  $ 			printf("\t\t\t---ON LINE %d IN FUNCTION %s---\n", __LINE__, __func__);

#define SET_ERR(errtype)        return ERRNUM = errtype;

#define CHECK_(what, code)      if (what) SET_ERR (code)

#define ERRNUM_CHECK(ret_val)   {if(ERRNUM) return ret_val;}

#define TREE_CHECK(node, ret_val)						\
	{									\
		if (TreeCheck(node)) {						\
			perror(errmsg(ERRNUM));					\
			EXIT_ERR(ret_val);					\
		}								\
	}			

#if DEBUG == 1                 
#define EXIT_ERR(ret) assert(!"ok");

#else
#define EXIT_ERR(ret) return ret;
#endif

typedef char* tval_t;

struct TNODE {
	tval_t data;
	TNODE *left;
	TNODE *right;
#ifdef VALIDATE_TREE_INTEGRY
	int visit_flag;
#endif
};

struct TREE {
	TNODE *root;
	int mode;
};

enum SUBTREE_SIDE {
	LEFT  = 0,
	RIGHT = 1
};

int TreeInsert(TNODE *parent, int side, tval_t data);

//TODO one func
void TreeTraversePre(TNODE *root, void (*visitor)(TNODE *node));
void TreeTraverseIn(TNODE *root, void (*visitor)(TNODE *node));
void TreeTraversePost(TNODE *root, void (*visitor)(TNODE *node));

int TreeCtor(TREE *tree, tval_t val);
int TreeDtor(TNODE *node);
int TreePrintNode(TNODE *node);

TNODE *TreeFind(TNODE *node);

void TreeDump(TNODE *node);
void TreeDotDump(TNODE *node);
void VisitPrint(TNODE *node);

int TreeCheck(TNODE *node);
#endif // TREE_H
