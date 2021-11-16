#include "akinator.h"

static int guess_object(TNODE *root, Stack *stack);
static int add_new_node(TNODE *node);
static int define_object(TNODE *node, Stack *stack);
static int get_answer(void);

int AkinatorGuess(TNODE *root)
{
	//TODO check!!!
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

int AkinatorProcess(TNODE *tree)
{
	int mode = 0;
	
	do {
		printf("Enter Akinator mode: Exit[0], Guess[1], Get full obj info[2], Compare[3]\n");
		
		scanf("%d", &mode);
		switch(mode) {
		case 1:
			AkinatorGuess(tree);
			break;
		case 2:
			//AkinatorFullInfo(tree);
			break;
		case 3:
			//AkinatorCompare(tree);
			break;
		case 4:
			TreeDump(tree);
			break;
		default:
			mode = 0;
			break;
		}
	} while(mode != 0);
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

	printf("Adding new node: %s and %s\n\n", name, def);
	
	char *old_object = node->data;
	//TODO language
	
	ERRNUM = 0;

	TreeInsert(node, RIGHT,  node->data);
	TreeInsert(node, LEFT,   name);
	
	ERRNUM_CHECK(ERRNUM);

	node->data = def;
	//TODO for future	
}

