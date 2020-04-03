#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define KEYWORD_MAX_LENGTH 128     //设定搜索串的最大长度


typedef struct {
	long long mem;
	long long cmpnum;

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

int bfStr(char *Mstr, char *substr)
{
	int number = 0;
	char *p;
	char *q;
	while (*Mstr != '\0')
	{
		p = Mstr;
		q = substr;
		while ((byte_cmp(*p, *q) == 0) && (*p != '\0') && (*q != '\0'))
		{
			p++;
			q++;
		}
		if (*q == '\0')
		{
			number++;
		}
		Mstr++;
	}
	return number;
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
		wordtimes[low++] ->times= wordtimes[high]->times;
		strcpy(wordtimes[low-1]->word, wordtimes[high]->word);

		while (low < high && wordtimes[low]->times <= part_element) {
			low++;
		}
		if (low >= high) break;
		wordtimes[high--]->times = wordtimes[low]->times;
		strcpy(wordtimes[high+1]->word , wordtimes[low]->word);
	}

	wordtimes[high]->times = part_element;
	strcpy(wordtimes[high]->word, c);
	return high;
}

int cmp(const void *a, const void *b)
{
	return (*(wordtime *)a).times > (*(wordtime *)b).times ? 1 : -1;
}

int main()
{
	wordtime * wordtimes[1500];
	memset(wordtimes, 0, sizeof(wordtime*) *1500);
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
	FILE * fw = fopen("result.txt", "r");
	global_stats.cmpnum = 0.0;
	while (!feof(frp))
	{
		fgets(c, 128, frp);
		c[strlen(c) - 1] = '\0';
		fprintf(fw, "%s %d\n", c, bfStr(word_str, c));
	}
	fprintf(fw, "%lld %lld", global_stats.cmpnum / 1000,global_stats.mem / 1024 );
	printf("%lld %lld", global_stats.cmpnum / 1000,global_stats.mem / 1024 );
	fclose(frp);
	fclose(fw);
	for (int i = 0; i <= 1499; i++)
	{
		wordtimes[i] = (wordtime *)bupt_malloc(sizeof(wordtime));
		wordtimes[i]->word = (char *)bupt_malloc(sizeof(char)*128);
		wordtimes[i]->times = 0;
	}

	
	FILE * fw1 = fopen("result1.txt", "w");

	for(int j=0; j<=1499;j++)
	{
		    char c[128];
			memset(c, 0, sizeof(char)*128 );
		    fscanf(fw, "%s %d\n", c, &wordtimes[j]->times);
			strcpy(wordtimes[j]->word, c);
			

	}

	quicksort(wordtimes, 0, 1499);
	for(int i=1499;i>=0;i--)
	    fprintf(fw1, "%s %d\n", wordtimes[i]->word, wordtimes[i]->times);
	return 0;
}

