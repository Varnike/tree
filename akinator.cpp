#include "akinator.h"

static int guess_object(TNODE *root, Stack *stack);
static int add_new_node(TNODE *node, Stack *call_nodes);
static int define_object(Stack *stack, int start = 0);
static int get_answer(void);
static int print_database(TNODE *node, FILE *fout);
static int read_database(TNODE **node, textBuff *btext, int ip);
static int read_stdin_sentence(char *buff, int buffsize);
static int free_alloc_nodes(Stack *alloc_nodes);
static int node_search(TNODE *tree, Stack *stack, const char *key);
static int compare_objects(Stack *stack1, Stack *stack2);
static void clearStdin();

int AkinatorGuess(TNODE *root, Stack *alloc_nodes)
{
	if (!root)
		return ERRNUM = TREE_NULL_NODE;

	TNODE *node = root;
	Stack stack = {};
	
	StackCtor(&stack, 8);
	ERRNUM_CHECK(ERRNUM);

	if (guess_object(root, &stack) == 1) {
		printf("EZ!\n");
		define_object(&stack);
	} else {
		add_new_node((TNODE *)StackPop(&stack), alloc_nodes);
	}

	StackDtor(&stack);
	ERRNUM_CHECK(ERRNUM);

	return 0;
}

int AkinatorProcess(TNODE *tree1)
{
	int mode = 0;
	TNODE *tree = 0;
	textBuff btext = {};

	AkinatorInit(&tree, &btext);	
	ERRNUM_CHECK(ERRNUM);

	Stack alloc_nodes = {};
	StackCtor(&alloc_nodes, 8);
	
	do {
		printf("\n\n\nEnter Akinator mode: Exit[0], Guess[1], Find object[2], Compare[3], Save Database[4]\n");
		
		scanf("%d", &mode);
	
		switch(mode) {
		case AKN_GUESS:
			AkinatorGuess(tree, &alloc_nodes);
			break;
		case AKN_FIND:
			AkinatorFind(tree);
			break;
		case AKN_CMP:
			AkinatorCompare(tree);
			break;
		case AKN_SAVE:
			AkinatorSave(tree);
			break;
		default:
			mode = 0;
			break;
		}

		if (ERRNUM)
			break;

	} while(mode != AKN_EXIT);
	
	free_alloc_nodes(&alloc_nodes);	
	free(btext.buff);
	TreeDtor(tree);
	StackDtor(&alloc_nodes);

	return ERRNUM;
}

int AkinatorFind(TNODE *tree)
{
	CHECK_(tree == NULL, TREE_NULL_NODE);
	

	char *target_name = (char *)calloc(INPUT_BUFF_SIZE, sizeof(char));
	CHECK_(!target_name, CALLOC_ERR);	
	Stack node_stack = {};

	printf("Enter object name you want to find: ");
	clearStdin();
	read_stdin_sentence(target_name, INPUT_BUFF_SIZE);
	if (ERRNUM)
		goto err_free_buff;
	
	
	StackCtor(&node_stack, 8);
	if (ERRNUM)
		goto err_free_buff;

	
	if (node_search(tree, &node_stack, target_name) == NODE_FOUND) {
		printf("\nObject [%s] found. It has the following properties:\n", target_name);
		define_object(&node_stack);
	} else {
		printf("\nObject [%s]not found.\n", target_name);
	}

err_free_buff:
	free(target_name);
	return ERRNUM;
}

int AkinatorCompare(TNODE *tree)
{
	CHECK_(tree == NULL, TREE_NULL_NODE);
	
	Stack stack1 = {};
	Stack stack2 = {};

	char *target1 = (char *)calloc(INPUT_BUFF_SIZE, sizeof(char));
	char *target2 = (char *)calloc(INPUT_BUFF_SIZE, sizeof(char));

	if (!target1 || !target2) {
		ERRNUM = CALLOC_ERR;
		goto err_free_buffers;
	}
	
	printf("Enter name of the first object to compare: ");
	clearStdin();
	read_stdin_sentence(target1, INPUT_BUFF_SIZE);
	
	if (ERRNUM)
		goto err_free_buffers;

	printf("Enter name of the second object to compare: ");
	read_stdin_sentence(target2, INPUT_BUFF_SIZE);
	
	if (ERRNUM)
		goto err_free_buffers;

	StackCtor(&stack1, 8);
	StackCtor(&stack2, 8);
	
	if (ERRNUM)
		goto err_free_buffers;

	if (node_search(tree, &stack1, target1) == NODE_NOT_FOUND || 
			node_search(tree, &stack2, target2) == NODE_NOT_FOUND) {
		printf("\nCant found objects to compare\n");
	} else {
		compare_objects(&stack1, &stack2);
	}

	StackDtor(&stack1);
	StackDtor(&stack2);

err_free_buffers:
	if (target1)
		free(target1);
	if (target2)
		free(target2);
	return ERRNUM;
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

		printf("%s\n", node->data.strptr);

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
	
	printf("Is it %s\n", node->data.strptr);	
	int right_guess = get_answer();
	
	return right_guess;
}

static int add_new_node(TNODE *node, Stack *alloc_nodes)
{
	char *defin  = (char *)calloc(INPUT_BUFF_SIZE, sizeof(char));
	char *name   = (char *)calloc(INPUT_BUFF_SIZE, sizeof(char));
	CHECK_(!name || !defin, CALLOC_ERR);
	
	ERRNUM = 0;
	printf("Enter object name:\n");
	clearStdin();
	int name_len  = read_stdin_sentence(name, INPUT_BUFF_SIZE);

	printf("%s differs from %s: %s is ...\n", name, node->data.strptr, name);
	int defin_len = read_stdin_sentence(defin, INPUT_BUFF_SIZE);
	
	if (ERRNUM) {
		free(defin);
		free(name);
		return ERRNUM;
	}

	printf("Adding new node: \"%s\" and \"%s\"\n\n", name, defin);
	
	TreeInsert(node, RIGHT,  set_strsize(node->data.len, node->data.strptr));
	TreeInsert(node, LEFT,   set_strsize(name_len, name));
	ERRNUM_CHECK(ERRNUM);

	StackPush(alloc_nodes, node);
	StackPush(alloc_nodes, node->left);

	node->data.strptr = defin;
	node->data.len    = defin_len;

	return 0;
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

int AkinatorInit(TNODE **tree, textBuff *btext)
{
	CHECK_(!tree,		TREE_NULL_NODE);
	CHECK_(!btext,		AKINATOR_NULL_ARG);
	CHECK_(btext->buff,	AKINATOR_BAD_INIT_STR);

	const char *namein = "akinator_database.txt";
	btext->file_in = fopen(namein, "r");
	read_from_file(btext, namein);

	printf("%s\n", btext->buff);
	
	read_database(tree, btext, 0);
	ERRNUM_CHECK(ERRNUM);

	TreeDump(*tree);
	fclose(btext->file_in);

	return 0;
}

static int define_object(Stack *stack, int start)
{
	int size = getStackSize(stack);
	TNODE **stack_data = (TNODE **)getStackData(stack);
	TNODE *tmp = NULL;

	printf("Object properties:\n");
	for (int it = start; it != size - 1; it++) {
		tmp = stack_data[it];
		
		CHECK_(!tmp || !tmp->data.strptr, TREE_NULL_DATA);

		if (tmp->right == (stack_data[it + 1]))
			printf("NOT ");
		
		printf("%s\n", tmp->data.strptr);
	}
	return 0;	
}

static int read_stdin_sentence(char *buff, int buffsize)
{
	char ch = 0;
	int cnt = 0;
	
	while((ch = getchar()) != EOF && ch != '\n') {
		if (ch >= buffsize) {
			ERRNUM = AKINATOR_BUFF_OVERFLOW;
			return -1;
		}
		printf("Char added : %c\n", ch);
		buff[cnt++] = ch;
	}
	
	return cnt;
}

void clearStdin()
{
	int c = 0;
	while((c = getchar()) != EOF && c != '\n')
		;
}

static int print_database(TNODE *node, FILE *fout)
{
	if (!fout)
		return ERRNUM = FOPEN_ERR;
	
	fprintf(fout, "{ %s ", node->data.strptr);

	if (node->left)
		print_database(node->left, fout);

	if (node->right)
		print_database(node->right, fout);
	fprintf(fout, "} ");
}

static int read_database(TNODE **node, textBuff *btext, int ip)
{
	ERRNUM_CHECK(ip);	

	for ( ; ip != btext->buffsize && isTrash(btext->buff[ip]); ip++)
		btext->buff[ip] = '\0';
		
	TreeCtor(node, set_strsize(0, btext->buff + ip++));

	ERRNUM_CHECK(ip);
	
	int str_len = 0;
	for ( ; ip < btext->buffsize; ip++) {
		if (btext->buff[ip] == '{') {
			if (ip == 0) {
				ERRNUM = AKINATOR_SYNTAX_ERR;
				return ip;
			}

			btext->buff[ip - 1] = '\0';

			(*node)->data.len = str_len;
			
			ip = read_database(&((*node)->left),  btext, ip);
			ip = read_database(&((*node)->right), btext, ip);
		}

		if (btext->buff[ip] == '}') {
			if (ip == 0) {
				ERRNUM = AKINATOR_SYNTAX_ERR;
				return ip;
			}

			btext->buff[ip-1] = '\0';
			(*node)->data.len = str_len; 

			VisitPrint(*node);
			return (ip + 1);	
		}

		str_len++;
	}
	
	return ERRNUM = AKINATOR_SYNTAX_ERR;	
}

int free_alloc_nodes(Stack *alloc_nodes)
{
	int node_cnt = getStackSize(alloc_nodes);

	for ( ; node_cnt != 0; node_cnt--) {
		TNODE *node = (TNODE *)StackPop(alloc_nodes);
		ERRNUM_CHECK(ERRNUM);

		CHECK_(node == NULL,	TREE_NULL_NODE);
		node->data.len = 0;
		free(node->data.strptr);
	}

	return 0;
}

static int node_search(TNODE *node, Stack *stack, const char *key)
{
	if (!node || !stack || !key || !node->data.strptr)
		return NODE_NOT_FOUND;
	
	printf("comparing key \"%s\" and str \"%s\"\n", key, node->data.strptr);
	
	StackPush(stack, node);

	if (strncmp(key, node->data.strptr, node->data.len) == 0)
		return NODE_FOUND;
		
	if (node->left) {
		if (node_search(node->left, stack, key) == NODE_FOUND)
			return NODE_FOUND;
	}

	if (node->right) {
		if (node_search(node->right, stack, key) == NODE_FOUND)
			return NODE_FOUND;

	}

	StackPop(stack);
	return NODE_NOT_FOUND;
				
}

static int compare_objects(Stack *stack1, Stack *stack2)
{
	CHECK_(!stack1 || !stack2, NULLPTR_STACK);
	
	int size1 = getStackSize(stack1);
        int size2 = getStackSize(stack2);

	TNODE **stack_data1 = (TNODE **)getStackData(stack1);
	TNODE **stack_data2 = (TNODE **)getStackData(stack2);
	ERRNUM_CHECK(ERRNUM);

	int it = 0;
	printf("%s and %s have common properties:\n", 
			stack_data1[size1 -1]->data.strptr, stack_data2[size2 - 1]->data.strptr);
	/* common properties */
	for ( ; it != ((size1 < size2) ? size1 : size2) - 2; it++) {
		if (stack_data1[it + 1] != stack_data2[it + 1])
			break;
		printf("\t%s\n", stack_data1[it]->data.strptr);
	}
	if (it == 0)
		printf("\tNONE.\n");
	/* difference */
	printf("%s and %s have some differences:\n", 
			stack_data1[size1 -1]->data.strptr, stack_data2[size2 - 1]->data.strptr);

	printf("%s:\n", stack_data1[size1 - 1]->data.strptr);
	define_object(stack1, it);

	printf("%s:\n", stack_data2[size2 - 1]->data.strptr);
	define_object(stack2, it);
	return 0;

}
