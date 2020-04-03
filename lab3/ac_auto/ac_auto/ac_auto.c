#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define subNodeNumber 4
typedef struct {
	long long mem;
	long long cmpnum;
	
}global;
global global_stats;
char * buf[2256695];
int count[2256695];
int head, tail;

const char * pattern_file = "../../pattern.txt";
const char * word_file = "../../string2.txt";

//内存分配函数
void* bupt_malloc(size_t size) {
	if (size <= 0) {
		return NULL;
	}
	global_stats.mem += size;
	return malloc(size);
}
int byte_cmp(char a, char b) {
	global_stats.cmpnum++;
	return a - b;
}

typedef struct node {
	struct node * next[subNodeNumber];
	struct node * fail;
	//int cnt;
	int pos;
}node;
node* q[225669100];

typedef struct {
	char * word;
	int times;
}wordtime;
wordtime * wordtimes[2256689];

node* getNewNode()
{
	node* newNode;
	newNode = (node *)bupt_malloc(sizeof(node));
	for (int i = 0; i < subNodeNumber; i++)
		newNode->next[i] = NULL;
	//newNode->cnt = 0;
	newNode->pos = -1;
	newNode->fail = NULL;
	return newNode;
}


void insert(node*root, char *e, int pos)
{
	node *p = root;
	int j = 0, index;
	while (*e != '\0') {
		for (int i = 7; i >= 0; i--)
		{
			int x1 = ((*e & 1 << i--) != 0);
			int x2 = ((*e & 1 << i) != 0);
			index = x1 * 2 + x2;
			if (p->next[index] == NULL)
				p->next[index] = getNewNode();
			p = p->next[index];
		}
		++e;
	}
	p->pos = pos;
	//++p->cnt;
}

void build(node* root)
{
	root->fail = NULL;
	q[tail++] = root;
	while (head < tail) {
		node* temp = q[head++];
		node* p = NULL;
		for (int i = 0; i < subNodeNumber; ++i) {
			if (temp->next[i] != NULL) {
				if (temp == root)temp->next[i]->fail = root;
				else {
					p = temp->fail;
					while (p != NULL) {
						if (p->next[i] != NULL) {
							temp->next[i]->fail = p->next[i];
							break;
						}
						p = p->fail;
					}
					if (p == NULL)temp->next[i]->fail = root;
				}
				q[tail++] = temp->next[i];
			}
		}
	}
}

void query(node *root, char * e)
{
	int cnt = 0, index;
	node* p = root;
	while (*e != '\0') {
		for (int i = 7; i >= 0; i--)
		{
			int x1 = ((*e & 1 << i--) != 0);
			int x2 = ((*e & 1 << i) != 0);
			index = x1 * 2 + x2;
			while (p->next[index] == NULL && p != root)
			{
				p = p->fail;
				global_stats.cmpnum++;
			}
			p = p->next[index];
			if (p == NULL)p = root;
			node* temp = p;
			while (temp != root && temp->pos != -1) {
				//count[temp->pos] += temp->cnt;
				count[temp->pos] ++;
				global_stats.cmpnum++;
				//temp->cnt = -1;
				temp = temp->fail;
			}
		}
		++e;
	}
}

void quicksort(wordtime * wordtimes[], int low, int high)
{
	int middle;

	if (low >= high) return;
	middle = split(wordtimes, low, high);
	quicksort(wordtimes, low, (middle - 1));
	quicksort(wordtimes, (middle + 1), high);
}

int split(wordtime * wordtimes[], int low, int high)
{
	int part_element = wordtimes[low]->times;
	char* c = (char *)bupt_malloc(128 * sizeof(char));
	strcpy(c, wordtimes[low]->word);
	for (;;) {
		while (low < high && part_element <= wordtimes[high]->times) {
			high--;
		}
		if (low >= high) break;
		wordtimes[low++]->times = wordtimes[high]->times;
		strcpy(wordtimes[low - 1]->word, wordtimes[high]->word);

		while (low < high && wordtimes[low]->times <= part_element) {
			low++;
		}
		if (low >= high) break;
		wordtimes[high--]->times = wordtimes[low]->times;
		strcpy(wordtimes[high + 1]->word, wordtimes[low]->word);
	}

	wordtimes[high]->times = part_element;
	strcpy(wordtimes[high]->word, c);
	return high;
}

int cmp(const void *arg1, const void *arg2)
{
	//return ((wordtime *)a)->times - ((wordtime *)b)->times ;
	int val;
	wordtime **stArg1 = arg1;
	wordtime **stArg2 = arg2;
	//printf("%d\n",(*stArg1)->times );
	if ((*stArg1)->times < (*stArg2)->times)
	{
		val = -1;
	}
	else if ((*stArg1)->times ==(*stArg2)->times)
	{
		val = 0;
	}
	else
	{
		val = 1;
	}
	return  val;
}

int main()
{
	
	memset(wordtimes, 0, sizeof(wordtime*) * 2256689);
	int time = 0;
	node * root = getNewNode();
	long long t;
	FILE *data = fopen(word_file, "rb");
	fseek(data, 0L, 2);
	t = ftell(data);
	rewind(data);
	char* word_str = (char *)bupt_malloc((t + 1) * sizeof(char));
	//fseek(data,0L,SEEK_SET);
	fread(word_str, sizeof(char), t, data);
	fclose(data);

	FILE * frp = fopen(pattern_file, "r");
	FILE * fw = fopen("result.txt", "r");
	global_stats.cmpnum = 0.0;
	int k = 0;
	char * c = (char *)bupt_malloc(sizeof(char) * 128);
	while (!feof(frp))
	{
		fgets(c, 128, frp);
		c[strlen(c) - 1] = '\0';
		buf[k] = (char *)bupt_malloc(sizeof(char) * (strlen(c) + 1));

		//c[strlen(c)-1] = '\0';
		strcpy(buf[k], c);
		//buf[k][strlen(buf[k]) - 1] = '\0';
		insert(root, buf[k], k);
		k++;
	}
	build(root);
	query(root, word_str);
	for (int i = 0; i < k; i++)
	{
		fprintf(fw, "%s %d\n", buf[i], count[i]);

	}
	fprintf(fw, "%lld %lld", global_stats.cmpnum / 1000, global_stats.mem / 1024);
	printf("%lld %lld", global_stats.cmpnum / 1000, global_stats.mem / 1024);
	fclose(frp);
	fclose(fw);
	for (int i = 0; i <= 2256688; i++)
	{
		wordtimes[i] = (wordtime *)bupt_malloc(sizeof(wordtime));
		wordtimes[i]->word = (char *)bupt_malloc(sizeof(char) * 128);
		wordtimes[i]->times = 0;
	}


	FILE * fw1 = fopen("result1.txt", "w");

	for (int j = 0; j <= 2256688; j++)
	{
		char c[128];
		memset(c, 0, sizeof(char) * 128);
		fscanf(fw, "%s %d\n", c, &wordtimes[j]->times);
		strcpy(wordtimes[j]->word, c);

	}
	//quicksort(wordtimes, 0, 2256688);递归太深导致栈溢出
	qsort(wordtimes, 2256689, sizeof(wordtime*), cmp);
	for (int i = 2256688; i >= 0; i--)
		fprintf(fw1, "%s %d\n", wordtimes[i]->word, wordtimes[i]->times);
	return 0;
}
