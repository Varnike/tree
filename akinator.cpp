#include "akinator.h"

static int guess_object(TNODE *root, Stack *stack);
static int add_new_node(TNODE *node);
static int define_object(TNODE *node, Stack *stack);
static int get_answer(void);
static int print_database(TNODE *node, FILE *fout);
static int read_database(TNODE **node, textBuff *btext, int ip);

int AkinatorGuess(TNODE *root)
{
	if (!root)
		return ERRNUM = TREE_NULL_NODE;

	TNODE *node = root;
	Stack stack = {};
	
	StackCtor(&stack, 8);
	ERRNUM_CHECK(ERRNUM);

	if (guess_object(root, &stack) == 1) {
		printf("EZ!\n");
		VisitPrint((TNODE *)StackPop(&stack));
		//define_object(root, stack);
	} else {
		add_new_node((TNODE *)StackPop(&stack));
	}

	StackDtor(&stack);
	ERRNUM_CHECK(ERRNUM);

	return 0;
}

int AkinatorProcess(TNODE *tree1)
{
	int mode = 0;
	TNODE *tree = 0;
	AkinatorInit(&tree);	
	do {
		printf("\n\n\nEnter Akinator mode: Exit[0], Guess[1], Get full obj info[2], Compare[3], Save Database[4]\n");
		
		scanf("%d", &mode);
	
		switch(mode) {
		case AKN_GUESS:
			AkinatorGuess(tree);
			break;
		case AKN_OBJ_DEF:
			//AkinatorCompare(tree);
			break;
		case AKN_CMP:
			//cmp
			break;
		case AKN_SAVE:
			AkinatorSave(tree);
			break;
		default:
			mode = 0;
			break;
		}
		ERRNUM_CHECK(ERRNUM);

	} while(mode != AKN_EXIT);

	return 0;
}

int AkinatorFullInfo(TNODE *tree)
{
	printf("Enter object name:\n");
	
	char *name = NULL;
	scanf("%ms", name);

}

static int get_answer(void)
{
	char *answ_buff = NULL;
	scanf("%ms", &answ_buff);

	if (answ_buff == NULL) {
		ERRNUM = CALLOC_ERR;
		return -1;
	}	
	
	int answ = 0;

	if (strcmp("y", answ_buff) == 0)
		answ = POS_ANSW;
	else if (strcmp("n", answ_buff) == 0) 
		answ = NEG_ANSW;
	else 
		answ = INV_ANSW;

	free(answ_buff);
	return answ;
}

static int guess_object(TNODE *node, Stack *stack)
{
	while (node->left && node->right) {
		StackPush(stack, node);
		ERRNUM_CHECK(-1);

		printf("%s\n", node->data);

		switch(get_answer()) {
		case POS_ANSW:
			node = node->left;
			break;
		case NEG_ANSW:
			node = node->right;
			break;
		case INV_ANSW:
			printf("Invalid answer, please try again.\n");
			break;
		}
	}
	
	StackPush(stack, node);
	//TODO check data
	printf("Is it %s\n", node->data);	
	int right_guess = get_answer();
	
	return right_guess;
}

static int add_new_node(TNODE *node)
{
	char *name = NULL;
	char *def  = NULL;

	printf("Enter object name:\n");
	scanf("%ms", &name);
	printf("%s differs from %s: %s is ...\n", name, node->data, name);
	scanf("%ms", &def);
	//scanf("%2000s %2000[^n]%c", a, b, c);
	printf("Adding new node: \"%s\" and \"%s\"\n\n", name, def);
	
	char *old_object = node->data;
	//TODO language
	
	ERRNUM = 0;

	TreeInsert(node, RIGHT,  node->data);
	TreeInsert(node, LEFT,   name);
	
	ERRNUM_CHECK(ERRNUM);

	node->data = def;
	//TODO for future	
}

int AkinatorSave(TNODE *tree)
{
	TREE_CHECK(tree, ERRNUM);

	FILE *fout = fopen("akinator_database.txt", "w");
		
	if (!fout)
		return FOPEN_ERR;
	printf("Saving akinator database.\n");
	print_database(tree, fout);
	fclose(fout);

	return 0;
}

int AkinatorInit(TNODE **tree)
{
	textBuff btext = {};
	if (!tree)
		return ERRNUM = TREE_NULL_NODE;
	
	const char *namein = "akinator_database.txt";
	btext.file_in = fopen(namein, "r");
	read_from_file(&btext, namein);

	printf("%s\n", btext.buff);
	
	read_database(tree, &btext, 0);
	ERRNUM_CHECK(ERRNUM);

	TreeDump(*tree);
	//TreeDtor(*tree);
	fclose(btext.file_in);
	//free(btext.buff);
	return 0;
}

static int define_object(TNODE *node, Stack *stack)
{
	//tval_t data = getStackData(stack);
	//int size    = getStackSize(stack);
//
//	ERRNUM_CHECK(ERRNUM);
//
//	for (int )
	return 0;	
}

static int print_database(TNODE *node, FILE *fout)
{
	if (!fout)
		return ERRNUM = FOPEN_ERR;
	
	fprintf(fout, "{ %s ", node->data);

	if (node->left)
		print_database(node->left, fout);

	if (node->right)
		print_database(node->right, fout);
	fprintf(fout, "} ");
}

//fgets for sentence
//free buffer
//TODO size
//TODO cmp func
static int read_database(TNODE **node, textBuff *btext, int ip)
{
	ERRNUM_CHECK(ip);	

	for ( ; ip != btext->buffsize && isTrash(btext->buff[ip]); ip++)
		btext->buff[ip] = '\0';

	TreeCtor(node, btext->buff + ip++);
	ERRNUM_CHECK(ip);

	for ( ; ip < btext->buffsize; ip++) {
		if (btext->buff[ip] == '{') {
			ip = read_database(&((*node)->left),  btext, ip);
			ip = read_database(&((*node)->right), btext, ip);
		}

		if (btext->buff[ip] == '}') {
			if (ip == 0) {
				ERRNUM = AKINATOR_SYNTAX_ERR;
				return ip;
			}

			btext->buff[ip-1] = '\0';

			VisitPrint(*node);
			return (ip + 1);	
		}
	}
	
	return ERRNUM = AKINATOR_SYNTAX_ERR;	
}
