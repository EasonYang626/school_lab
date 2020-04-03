#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct {
	long long mem;
	long long cmpnum;
	long long nodenum;
}global;
global global_stats;

const char * pattern_file = "../../patterns-127w.txt";
const char * word_file = "../../words-98w.txt";

//内存分配函数
void* bupt_malloc(size_t size) {
	if (size <= 0) {
		return NULL;
	}
	global_stats.mem += size;
	return malloc(size);
}

//建树
typedef struct Node
{
	int end;
	struct Node * child[2];//子树指针
}RTNode, *RTree;

//创建一个新的节点
RTree NewNode()
{
	RTree newNode;
	newNode = (RTree)bupt_malloc(sizeof(RTNode));
	global_stats.nodenum++;
	newNode->end = 0;
	for (int i = 0; i< 2; i++) {
		newNode->child[i] = NULL;
	}
	return newNode;
}

//插入字符串
RTree insert(RTree T, char *e)
{
	//printf("str: %s\n", e);
	RTree p = T;
	while (*e != '\0')
	{
		for (int i = 7; i >= 0; i--)
		{
			int x1 = ((*e & 1 << i) != 0);//按位与运算的优先级低于移位运算


			if (p->child[x1] != NULL)
			{
				p = p->child[x1];
			}
			else
			{
				//printf("aaa");
				RTree tmp = NewNode();
				p->child[x1] = tmp;
				p = tmp;
			}
			//printf("node num:%d\n", global_stats.nodenum);
		}

		e++;
	}
	p->end = 1;
	return T;
}

//查找
int search(RTree T, char * e)
{
	RTree p = T;
	while (*e != '\0')
	{

		for (int i = 7; i >= 0; i--)
		{
			int x1 = ((*e & 1 << i) != 0);
			global_stats.cmpnum++;
			if (p->child[x1] != NULL)
			{
				p = p->child[x1];
			}
			else
			{
				return 0;
			}
		}
		//global_stats.cmpnum++;
		e++;
	}
	global_stats.cmpnum++;
	if (p->end == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


int main()
{
	int searchCount = 0;
	int hitCount = 0;
	RTree T = NewNode();
	char* c = (char *)malloc(128 * sizeof(char));
	FILE * frp = fopen(pattern_file, "r");
	FILE * frw = fopen(word_file, "r");
	FILE * fw = fopen("result.txt", "w");
	while (fscanf(frp, "%[^\n]", c) != EOF)
	{
		fgetc(frp);
		T = insert(T, c);
		//printf("node count: %d\n", global_stats.nodenum);
	}
	fclose(frp);
	global_stats.cmpnum = 0.0;
	while (fscanf(frw, "%[^\n]", c) != EOF)
	{
		fgetc(frw);
		searchCount++;
		if (search(T, c))
		{
			hitCount++;
			fprintf(fw, "%s yes\n", c);
		}
		else
		{
			fprintf(fw, "%s no\n", c);
			//printf("%s no\n", c);
		}
	}
	fprintf(fw, "%lld %lld %lld %d %d", global_stats.nodenum, global_stats.mem / 1024, global_stats.cmpnum / 1000, searchCount, hitCount);
	printf("%lld %lld %lld %d %d", global_stats.nodenum, global_stats.mem / 1024, global_stats.cmpnum / 1000, searchCount, hitCount);
	fclose(frw);
	fclose(fw);
	return 0;
}
