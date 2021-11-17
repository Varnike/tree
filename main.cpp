#include "akinator.h"
#include <stdio.h>


int main()
{
	char *words[] = {"shit system?", "the worst?","windows", "macos", "linux"};
	TREE tree = {};

	TreeCtor(&tree.root, words[0]);
	assert(tree.root);
	TNODE *root = tree.root;
	//assert(root);
	//printf("mode : %d\n", tree.mode);
$	TreeInsert(root, LEFT, words[1]);
$	TreeInsert(root, RIGHT, words[4]);
$	TreeInsert(root->left, LEFT, words[2]);
$	TreeInsert(root->left, RIGHT, words[3]);

	AkinatorProcess(tree.root);
	
	//TreeDump(tree.root);
	
	TreeDtor(tree.root);
}
