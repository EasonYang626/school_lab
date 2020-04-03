#include<stdio.h>
#include<string.h>
#define MAXNUM 1272000

typedef struct {
	long long mem;
	long long cmpnum;
}global;
global global_stats;

const char * pattern_file = "patterns-127w.txt";
const char * word_file = "words-98w.txt";


//内存分配函数
void* bupt_malloc(size_t size) {
	if (size <= 0) {
		return NULL;
	}
	global_stats.mem += size;
	return malloc(size);
}
//字符比较统计函数 
int byte_cmp(char a, char b) {
	global_stats.cmpnum++;
	return a - b;
}

//字符串比较函数
int str_cmp(const char * str1, const char * str2)
{
	char * a = str1;
	char * b = str2;
	while (*a != '\0' && *b != '\0')
	{
		if (!byte_cmp(*a, *b))
		{
			a++;
			b++;
		}
		else {
			return 0;
		}
	}
	if (*a == '\0' && *b == '\0')
	{
		return 1;
	}
	return 0;
}

//哈希函数
unsigned int RSHash(const char* str, unsigned int length)
{
	unsigned int b = 378551;
	unsigned int a = 63689;
	unsigned int hash = 0;
	unsigned int i = 0;

	for (i = 0; i < length; ++str, ++i)
	{
		hash = hash * a + (*str);
		a = a * b;
	}

	return hash;
}

typedef struct Node {
	char* key;
	struct Node* next;
}Node;
Node * HashTable[MAXNUM];

void hashTableInit()
{
	memset(HashTable, 0, sizeof(Node*) * MAXNUM);
}

void hash_insert(char* key)
{
	int index = RSHash(key, strlen(key)) % MAXNUM;
	Node * head = HashTable[index];
	while (head)
	{
		if (strcmp(head->key, key) == 0)
		{
			
			return;//key相同的重复元素不再存入哈希表中
		}
		head = head->next;
	}
	Node* pNewNode = (Node*)bupt_malloc(sizeof(Node));
	memset(pNewNode, 0, sizeof(Node));
	pNewNode->key = (char*)malloc(sizeof(char) * (strlen(key) + 1));
	strcpy(pNewNode->key, key);
	pNewNode->next = HashTable[index];
	HashTable[index] = pNewNode;
}

int search(char* key)
{
	int index = RSHash(key, strlen(key)) % MAXNUM;
	if (HashTable[index])
	{
		Node* head = HashTable[index];
		while (head)
		{
			if (str_cmp(key, head->key))
				return 1;
			head = head->next;
		}
	}
	return 0;
}

void hashTableRelease()
{
	int i;
	for (i = 0; i < MAXNUM; ++i)
	{
		if (HashTable[i])
		{
			Node* head = HashTable[i];
			while (head)
			{
				Node* temp = head;
				head = head->next;
				if (temp)
				{
					free(temp->key);
					free(temp);
				}
			}
		}
	}
}
int main()
{
	int searchCount = 0;
	int hitCount = 0;
	hashTableInit();
	char* c = (char *)malloc(128 * sizeof(char));
	FILE * p_file = fopen(pattern_file, "r");
	FILE * w_file = fopen(word_file, "r");
	FILE * r_file = fopen("result.txt", "w");
	int n = 0;
	while (fscanf(p_file, "%[^\n]", c) != EOF)
	{
		fgetc(p_file);
		hash_insert(c);

	}
	fclose(p_file);
	while (fscanf(w_file, "%[^\n]", c) != EOF)
	{
		fgetc(w_file);
		searchCount++;
		if (search(c))
		{
			hitCount++;
			fprintf(r_file, "%s yes\n", c);

		}
		else
		{
			fprintf(r_file, "%s no\n", c);

		}
	}
	fprintf(r_file, "%lld %lld %d %d", global_stats.mem / 1024, global_stats.cmpnum, searchCount, hitCount);

	hashTableRelease();
	fclose(w_file);
	fclose(r_file);
	return 0;
}
