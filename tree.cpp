#include "tree.h"

static void set_node(TNODE *node, tval_t val, TNODE *left = NULL, TNODE *right = NULL);
static void visit_check(TNODE *node, int flagval = 0);

FILE *file = NULL;

int TreeCtor(TREE *tree, tval_t val)
{
	CHECK_(tree->root != NULL, TREE_BAD_CTOR_ROOT);

	assert(tree->root == NULL);

	TNODE *root = (TNODE*)calloc(1,sizeof(TNODE));

	if (root == NULL)
		return ERRNUM = CALLOC_ERR;

	
	set_node(root, val);
	
	tree->root = root;

	assert(root);
	return 0;
}

int TreeInsert(TNODE *parent, int side, tval_t data)
{
	//assert(parent);
	TREE_CHECK(parent, ERRNUM);

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
	
	TREE_CHECK(parent, ERRNUM);
	return 0;

err_free_buff:
	free(node);
	return ERRNUM;
}

int TreeDtor(TNODE *node) 
{
	TREE_CHECK(node, ERRNUM);
	
	TreeTraversePost(node, (void(*)(TNODE *))free);		
	
	return 0;
}

void TreeTraversePost(TNODE *node, void (*visitor)(TNODE *node))
{
	if (!node)
		return;
	
	if (node->left)
		TreeTraversePost(node->left, visitor);
	
	if (node->right)
		TreeTraversePost(node->right, visitor);
	
	visitor(node);
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
	TREE_CHECK(root, ERRNUM);

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

int _tree_check(TNODE *node)
{
	CHECK_(node == NULL,  						TREE_NULL_NODE);
	CHECK_(node->data == NULL,  					TREE_NULL_DATA);
	CHECK_((node->left == node->right) && (node->left != NULL),  	TREE_SAME_CHILD);
#ifdef NODE_BOTH_CHILD_CHECK
	CHECK_((node->left == NULL) ^ (node->right == NULL),		TREE_ONE_CHILD);
#endif
	return NO_ERR;
}

int TreeCheck(TNODE *node)
{
#ifdef TREE_CHECK_DUMP
	file = fopen("dump/dump.dot", "w");
	assert(file);
#endif

#ifdef VALIDATE_TREE_INTEGRY
	static int flag_visit = 1;
	visit_check(node, flag_visit);
#else
	visit_check(node);
#endif
	ERRNUM_CHECK(ERRNUM);

#ifdef VALIDATE_TREE_INTEGRY
	flag_visit++;
#endif

#ifdef TREE_CHECK_DUMP
	fclose(file);
#endif
	return 0;
}

static void visit_check(TNODE *node, int flagval)
{
	_tree_check(node);
	ERRNUM_CHECK(;);

#ifdef VALIDATE_TREE_INTEGRY
	
	if (node->visit_flag == flagval)
		ERRNUM = TREE_LOOP_FOUND;
	ERRNUM_CHECK(;);
	
	node->visit_flag = flagval;
#endif

#ifdef TREE_CHECK_DUMP
	TreeDotDump(node);
#endif
	if (node->left)
		visit_check(node->left, flagval);
	
	if (node->right)
		visit_check(node->right, flagval);
}

void VisitPrint(TNODE *node)
{
	printf("node : [  %p  ], data : [  %s  ], left : [  %p  ], right : [  %p  ]\n", 
		node, node->data, node->left, node->right);
}

void TreeDotDump(TNODE *node)
{
	assert(node);
	assert(file);

	fprintf(file, "node%p [shape=plaintext\n\
			\t\tlabel=<<table border='1' cellborder='1'>\n\
			\t\t<tr><td colspan=\"2\" bgcolor=\"lightskyblue\" >%s</td></tr>\n\
			\t\t<tr><td port= \"lchild\">Left</td><td port=\"rchild\">Right</td></tr>\n\
			\t</table>>];\n", node, node->data);
	
	if (node->left)
		fprintf(file, "\tnode%p:lchild -> node%p[style=bold, arrowhead=vee label = \"Yes\"];\n",
			       	node, node->left);

	if (node->right)
		fprintf(file, "\tnode%p:rchild -> node%p[style=bold, arrowhead=vee label = \"No\"];\n", 
				node, node->right);
}
