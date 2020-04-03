#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXNUM 1500     //设定搜索串的最大个数


typedef struct {
	long long mem;
	long long cmpnum;
	//long long nodenum;
}global;
global global_stats;

const char * pattern_file = "../../pattern_bf_kmp.txt";
const char * word_file = "../../string2.txt";

typedef struct {
	char * word;
	int times;
}wordtime;


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

int net[MAXNUM];
void getnext(char * b)
{
	net[0] = -1;
	int j = 0, k = -1, len = strlen(b);
	while (j<len)
	{
		if (k == -1 || b[j] == b[k])
			net[++j] = ++k;
		else k = net[k];
	}
}
int kmp(char * a, char * b)
{
	int ans = 0;
	int i = 0, j = 0;
	int la = strlen(a), lb = strlen(b);
	while (i<la&&j <= lb)
	{
		if (j == -1 || a[i] == b[j])
		{
			global_stats.cmpnum++;
			i++;
			j++;
		}
		else j = net[j];
		if (j == lb) ans++;
	}
	return ans;
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


int main()
{
	wordtime * wordtimes[1500];
	memset(wordtimes, 0, sizeof(wordtime*) * 1500);
	int time = 0;
	long long t;
	FILE *data = fopen(word_file, "rb");
	fseek(data, 0L, 2);
	t = ftell(data);
	rewind(data);
	char* word_str = (char *)bupt_malloc((t + 1) * sizeof(char));
	//fseek(data,0L,SEEK_SET);
	fread(word_str, sizeof(char), t, data);
	fclose(data);



	char* c = (char *)bupt_malloc(128 * sizeof(char));
	FILE * frp = fopen(pattern_file, "r");
	FILE * fw = fopen("result.txt", "w");
	global_stats.cmpnum = 0.0;
	while (!feof(frp))
	{
		fgets(c, 128, frp);
		c[strlen(c) - 1] = '\0';
		getnext(c);
		fprintf(fw, "%s %d\n", c, kmp(word_str, c));
		time = time + 1;
		if (time % 100 == 0)printf("%d", time);
	}
	fprintf(fw, "%lld %lld", global_stats.cmpnum / 1000, global_stats.mem / 1024);
	printf("%lld %lld", global_stats.cmpnum / 1000, global_stats.mem / 1024);
	fclose(frp);
	fclose(fw);
	for (int i = 0; i <= 1499; i++)
	{
		wordtimes[i] = (wordtime *)bupt_malloc(sizeof(wordtime));
		wordtimes[i]->word = (char *)bupt_malloc(sizeof(char) * 128);
		wordtimes[i]->times = 0;
	}


	FILE * fw1 = fopen("result1.txt", "w");

	for (int j = 0; j <= 1499; j++)
	{
		char c[128];
		memset(c, 0, sizeof(char) * 128);
		fscanf(fw, "%s %d\n", c, &wordtimes[j]->times);
		strcpy(wordtimes[j]->word, c);


	}
	quicksort(wordtimes, 0, 1499);
	for (int i = 1499; i >= 0; i--)
		fprintf(fw1, "%s %d\n", wordtimes[i]->word, wordtimes[i]->times);
	return 0;
}

