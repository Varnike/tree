#include "tree.h"
#include <stdio.h>

int main()
{
	TREE tree = {};
	TreeCtor(&tree, 10);

	TNODE *root = tree.root;
	
	TreeInsert(root, LEFT, 11);
	TreeInsert(root, RIGHT, 21);
	TreeInsert(root->left, LEFT, 12);
	TreeInsert(root->left, RIGHT, 13);

	TreeDump(tree.root);
	
	TreeDtor(tree.root);
}
