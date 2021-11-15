#include "tree.h"

static void set_node(TNODE *node, tval_t val, TNODE *left = NULL, TNODE *right = NULL);

FILE *file = NULL;

int TreeCtor(TREE *tree, tval_t val)
{
	assert(tree->root == NULL);

	TNODE *root = (TNODE*)calloc(1,sizeof(TNODE));

	if (root == NULL)
		return ERRNUM = CALLOC_ERR;

	
	set_node(root, val);
	
	tree->root = root;

	return 0;
}

int TreeInsert(TNODE *parent, int side, tval_t data)
{
	TNODE *node = (TNODE*)calloc(1,sizeof(TNODE));

	if (node == NULL) {
		ERRNUM = CALLOC_ERR;
		goto err_free_buff;
	}
	
	set_node(node, data);

	if (side == LEFT) {
		if (parent->left != NULL) {
			ERRNUM = TREE_BUSY_PARENT;
			goto err_free_buff;
		}
		parent->left  = node;
	} else {
		if (parent->right != NULL) {
			ERRNUM = TREE_BUSY_PARENT;
			goto err_free_buff;
		}

		parent->right = node;
	}

	return 0;

err_free_buff:
	free(node);
	return ERRNUM;
}

void TreeDtor(TNODE *node) 
{
	//TODO ERR_CHECK
	
	if (!node)
		return;
	if (node->left)
		TreeDtor(node->left);
	
	if (node->right)
		TreeDtor(node->right);

	free(node);
}

void TreeTraverseIn(TNODE *node, void (*visitor)(TNODE *node))
{
	if (!node)
		return;
	
	visitor(node);
	
	if (node->left)
		TreeTraverseIn(node->left, visitor);
	
	if (node->right)
		TreeTraverseIn(node->right, visitor);
}

void TreeTraversePre(TNODE *node, void (*visitor)(TNODE *node))
{
	if (!node)
		return;
	
	if (node->left)
		TreeTraversePre(node->left, visitor);
	
	visitor(node);
		
	if (node->right)
		TreeTraversePre(node->right, visitor);
}

void TreeDump(TNODE *root)
{
	file = fopen("dump/dump.dot", "w");
	assert(file);

	if (file == NULL) {
		ERRNUM = FOPEN_ERR;
		return;
	}

	fprintf(file, "digraph dump_graph {\n\trankdir=TV; \n");

	TreeTraversePre(root, TreeDotDump);
	
	fprintf(file, "}\n");
		
	fclose(file);

	system("dot -Tsvg dump/dump.dot -o dump/dump.svg");
}

static void set_node(TNODE *node, tval_t val, TNODE *left, TNODE *right)
{
	if (node == NULL)
		return;

	node->data = val;
	node->left = left;
	node->right = right;
}

void VisitPrint(TNODE *node)
{
	printf("node : [  %p  ], data : [  %d  ], left : [  %p  ], right : [  %p  ]\n", 
		node, node->data, node->left, node->right);
}

void TreeDotDump(TNODE *node)
{
	assert(node);
	assert(file);

	fprintf(file, "node%p [shape=plaintext\n\
			\t\tlabel=<<table border='1' cellborder='1'>\n\
			\t\t<tr><td colspan=\"2\" bgcolor=\"lightskyblue\" >Data: %d</td></tr>\n\
			\t\t<tr><td port= \"lchild\">Left</td><td port=\"rchild\">Right</td></tr>\n\
			\t</table>>];\n", node, node->data);
	
	if (node->left)
		fprintf(file, "\tnode%p:lchild -> node%p[style=bold, arrowhead=vee];\n", node, node->left);

	if (node->right)
		fprintf(file, "\tnode%p:rchild -> node%p[style=bold, arrowhead=vee];\n", node, node->right);
}
