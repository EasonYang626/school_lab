#include<stdio.h>
#include<string.h>

typedef struct {
	long long mem;
	long long cmpnum;
	long long nodeCount;
}global;
global global_stats;

const char * pattern_file = "../../patterns-127w.txt";
const char * word_file = "../../words-98w.txt";

typedef struct node {
	char * data;
	struct node *left;
	struct node *right;
	int height;
}treenode;

//内存分配函数
void* bupt_malloc(size_t size) {
	if (size <= 0) {
		return NULL;
	}
	global_stats.mem += size;
	return malloc(size);
}


//字符串比较函数
int str_cmp(const char *src, const char *dst)
{
	int ret = 0;
	global_stats.cmpnum++;
	while (!(ret = *(unsigned char *)src - *(unsigned char *)dst) && *dst)
	{
		++src;
		++dst;
		global_stats.cmpnum++;
	}
	if (ret < 0)
		ret = -1;
	else if (ret > 0)
		ret = 1;
	return(ret);
}


int search(char *data, struct node* newnode) {
	if (newnode == NULL) {
		return -1;
	}
	while (newnode != NULL) {
		int temp = str_cmp(data, newnode->data);
		if (temp == 0) {
			return 1;
		}
		if (temp > 0) {
			newnode = newnode->right;
		}
		if (temp < 0) {
			newnode = newnode->left;
		}
		if (newnode == NULL)return -1;
	}
}

struct node *root = NULL;

int max(int a, int b)
{
	return (a>b) ? a : b;
}

int Height(struct node *root)
{
	if (root == NULL)
		return 0;
	return
		root->height;
}

int BalanceFactor(struct node *root)
{
	if (root == NULL)
		return 0;
	else
		return Height(root->left) - Height(root->right);
}

struct node *RightRotate(struct node *root) //LeftLeft
{
	struct node *temp1 = root->left; //New Root
	struct node *temp2 = temp1->right;
	//perform right rotation
	temp1->right = root;
	root->left = temp2;
	//update heights
	root->height = 1 + max(Height(root->left), Height(root->right));
	temp1->height = 1 + max(Height(temp1->left), Height(temp1->right));
	//return New Root
	return temp1;
}

struct node *LeftRotate(struct node *root) //RightRight
{
	struct node *temp1 = root->right; //New Root
	struct node *temp2 = temp1->left;
	//perform left rotation
	temp1->left = root;
	root->right = temp2;
	//update heights
	root->height = 1 + max(Height(root->left), Height(root->right));
	temp1->height = 1 + max(Height(temp1->left), Height(temp1->right));
	//return New Root
	return temp1;
}

struct node *Insert(struct node *root, char* data)
{
	//STEP 1: PERFORM STANDARD BST INSERTION
	if (root == NULL)
	{
		struct node *NewNode = (struct node*)bupt_malloc(sizeof(struct node));
		NewNode->data = (char *)bupt_malloc(128 * sizeof(char));
		strcpy(NewNode->data , data);
		NewNode->left = NULL;
		NewNode->right = NULL;
		global_stats.nodeCount++;
		root = NewNode; //a new new node is created for insertion after first root
	}
	else if (str_cmp(data , root->data)<0)
	{
		root->left = Insert(root->left, data);
	}
	else if (str_cmp(data, root->data) == 0)return root;
	else
	{
		root->right = Insert(root->right, data);
	}

	//STEP 3 & 4: AFTER INSERTION FOLLOWING FUNCTIONS ARE PERFORMED TO CONVERT BST INTO AVL

	root->height = 1 + max(Height(root->left), Height(root->right)); //update the height of this ancestor

	int bf = BalanceFactor(root); //check the balance factor of this ancestor node whether this node became unbalanced or not

								  //If this node becomes unbalanced, then there are 4 cases
	if (bf > 1 && str_cmp(data , root->left->data)<0) //LeftLeft
	{
		return RightRotate(root);
	}

	if (bf < -1 && str_cmp(data , root->right->data)>0) //RightRight
	{
		return LeftRotate(root);
	}

	if (bf > 1 && str_cmp(data , root->left->data)>0) //LeftRight
	{
		root->left = LeftRotate(root->left); //converts into LeftLeft
		return RightRotate(root);
	}

	if (bf < -1 && str_cmp(data , root->right->data)<0) //RightLeft
	{
		root->right = RightRotate(root->right); //converts into RightRight
		return LeftRotate(root);
	}

	return root;
}


int main()
{
	int searchCount = 0;
	int hitCount = 0;
	char* c = (char *)bupt_malloc(128 * sizeof(char));
	FILE * p_file = fopen(pattern_file, "r");
	FILE * w_file = fopen(word_file, "r");
	FILE * r_file = fopen("result.txt", "w");
	int n = 0;
	int i = 0;
	while (fscanf(p_file, "%[^\n]", c) != EOF)
	{
		fgetc(p_file);
		root = Insert(root,c);
		
	}
	global_stats.cmpnum = 0;//将构建树时的比较次数置为0
	fclose(p_file);
	while (fscanf(w_file, "%[^\n]", c) != EOF)
	{
		fgetc(w_file);
		searchCount++;
		if (search(c,root)==1)
		{
			hitCount++;
			fprintf(r_file, "%s yes\n", c);

		}
		else
		{
			fprintf(r_file, "%s no\n", c);

		}
	}
	fprintf(r_file, "%lld %lld %lld %d %d", global_stats.nodeCount,global_stats.mem / 1024, global_stats.cmpnum / 1024, searchCount, hitCount);
	printf("%lld %lld %lld %d %d", global_stats.nodeCount, global_stats.mem / 1024, global_stats.cmpnum / 1024, searchCount, hitCount);
	fclose(w_file);
	fclose(r_file);
	return 0;
}