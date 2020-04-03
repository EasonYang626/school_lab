#include <stdio.h>
#include <stdlib.h>

struct Global_stats
{
	unsigned long long cmpnum;
	unsigned long long mem;
} global_stats;

void* bupt_malloc(size_t size)
{
	if (size <= 0)
	{
		return NULL;
	}
	global_stats.mem += size;
	return malloc(size);
}

int byte_cmp(char a, char b)
{
	global_stats.cmpnum++;
	return a - b;
}

struct node {
	int key;//用第一个字的对应的几个char值作key
	char word[128];
	struct node *next;
};

struct table {
	int size;
	struct node *list;
};

struct table *createTable(int size) {
	struct table *t = (struct table*)bupt_malloc(sizeof(struct table));
	t->size = size;
	t->list = (struct node**)bupt_malloc(sizeof(struct node*)*size);
	int i;
	return t;
}

int hashCode(struct table *t, int key) {
	if (key<0)
		return -(key%t->size);
	return key%t->size;
}



int lookup(struct table *t, int key) {
	int pos = hashCode(t, key);
	struct node *list = t->list[pos];
	struct node *temp = list;
	while (temp) {
		if (temp->key == key) {
			return temp->val;
		}
		temp = temp->next;
	}
	return -1;
}


int main()
{
	struct table *t = createTable(1280000);
	global_stats.cmpnum = 0;
	global_stats.mem = 0;
	char ch, ch1;
	int pnum = 0;
	int wnum = 0;
	int findnum = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	int m = 0;
	int n = 0;
	int find = -1;
	int end = 1;//结束标志符
	FILE *fp = NULL;
	FILE *fp1 = NULL;
	FILE *result = NULL;
	fp = fopen("patterns-127w.txt", "r");
	fp1 = fopen("words-98w.txt", "r");
	result = fopen("result1.txt", "w");
	if (NULL == fp)
	{
		printf("failed to open \n");
		return 1;
	}

	while ((ch = fgetc(fp)) != EOF)
	{
		if (ch == '\n')
		{
			j = 0;
			i++;

		}
		newarray[i].word[j] = ch;
		j++;
		newarray[i].length = j;


	}
	pnum = i;//pattern中的总字符串数
	i = 0;
	j = 0;//用来定位长度相等的字符串当前字符判定位置

	while ((ch1 = fgetc(fp1)) != EOF)
	{

		if (ch1 == '\n')
		{
			while (i < pnum)
			{
				if (m == newarray[i].length) {
					while (byte_cmp(newarray[i].word[j], newarray1.word[j]) == 0) {
						j++;
						if (j == m - 1)
						{
							fputs(newarray1.word, result);
							fputs(" yes\n", result);
							findnum++;//记录成功检索到的word个数
							find = 1;
							i = pnum;//检索到后跳过后续的比较
							for (n = 0; n < m; n++)
							{
								newarray1.word[n] = '\0';
							}

						}
					}
					j = 0;
				}
				i++;//当前匹配失败检索下一个pattern中的字符串
			}
			if (find != 1) {
				fputs(newarray1.word, result);
				fputs(" no\n", result);
				for (n = 0; n < m; n++)
				{
					newarray1.word[n] = '\0';
				}
			}
			i = 0;//整个pattern没有检索到当前字符串，进入下一个字符串检索，pattern字符串定位从头开始
			m = 0;//一个字符串检索完成进入下一个，待匹配字符串字符定位回到0
			wnum++;//记录Word总个数

		}
		find = -1;//下一个字符串开始检索置状态为未找到
		newarray1.word[m] = ch1;
		m++;//定位待匹配字符串当前字符位置
	}
	//处理最后一个word
	i = 0;
	while (i < pnum)
	{
		if (m == newarray[i].length) {
			while (byte_cmp(newarray[i].word[j], newarray1.word[j]) == 0) {
				j++;
				if (j == m - 1)
				{
					fputs(newarray1.word, result);
					fputs(" yes\n", result);
					findnum++;//记录成功检索到的word个数
					find = 1;
					for (j = 0; j < m; j++)
					{
						newarray1.word[j] = '\0';
					}

				}
			}
			j = 0;
		}
		i++;//当前匹配失败检索下一个pattern中的字符串
	}
	if (find != 1) {
		fputs(newarray1.word, result);
		fputs(" no\n", result);
		for (j = 0; j < m; j++)
		{
			newarray1.word[j] = '\0';
		}
	}
	wnum++;
	fprintf(result, "%lld", global_stats.mem / 1024);
	fputs("kb ", result);
	fprintf(result, "%lld", global_stats.cmpnum);
	fputs(" ", result);
	fprintf(result, "%d", wnum);
	fputs(" ", result);
	fprintf(result, "%d", findnum);
	fclose(fp);
	fclose(fp1);
	fclose(result);
	printf("finished\n");
	return 0;
}