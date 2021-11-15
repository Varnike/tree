#include "akinator.h"

static int guess_object(TNODE *root, Stack *stack);
static int add_new_node(TNODE *node);
static int define_object(TNODE *node, Stack *stack);

int AkinatorGuess(TNODE *root)
{
	//TODO check!!!
	TNODE *node = root;
	Stack stack = {};

	if (guess_object(root, &stack) == 1) {
		printf("EZ!\n");
		//define_object(root, stack);
		return 0;	
	} else {
		add_new_node(root);	
	}
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

//TODO no external buff
static int get_answer(char *answ_buff)
{
	scanf("%s", answ_buff);	
	
	if (strncmp("y", answ_buff, 2) == 0)
		return POS_ANSW;
	 else if (strncmp("n", answ_buff, 2) == 0) 
		return NEG_ANSW;

	return INV_ANSW;		
}

static int guess_object(TNODE *node, Stack *stack)
{
	char *answ_buff = (char *)calloc(sizeof(char), INPUT_BUFF_SIZE); //TODO check for shit

	while (node->left && node->right) {
		printf("%s\n", node->data);

		switch(get_answer(answ_buff)) {
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

	//TODO check data
	printf("Is it %s\n", node->data);	
	int right_guess = get_answer(answ_buff);

	free(answ_buff);

	return right_guess;
}

static int add_new_node(TNODE *node)
{
	char name = NULL;
	char def  = NULL;
	//TODO for future
	printf("");
}
