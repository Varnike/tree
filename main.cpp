#include "akinator.h"
#include <stdio.h>


int main()
{
	char *words[] = {"shit system?", "the worst?","windows", "macos", "linux"};
	TREE tree = {};

	TreeCtor(&tree, words[0]);

	TNODE *root = tree.root;

	TreeInsert(root, LEFT, words[1]);
	TreeInsert(root, RIGHT, words[4]);
	TreeInsert(root->left, LEFT, words[2]);
	TreeInsert(root->left, RIGHT, words[3]);

	TreeDump(tree.root);
	
	AkinatorProcess(tree.root);
	TreeDtor(tree.root);
}
