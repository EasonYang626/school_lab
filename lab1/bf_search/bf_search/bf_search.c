#include<stdio.h>
#include<string.h>
#define MAXNUM 18432000

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
signed int JSHash(const char* str, unsigned int length)
{
	unsigned int hash = 1315423911;
	unsigned int i = 0;

	for (i = 0; i < length; ++str, ++i)
	{
		hash ^= ((hash << 5) + (*str) + (hash >> 2));
	}

	return hash;
}
unsigned int PJWHash(const char* str, unsigned int length)
{
	const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
	const unsigned int ThreeQuarters = (unsigned int)((BitsInUnsignedInt * 3) / 4);
	const unsigned int OneEighth = (unsigned int)(BitsInUnsignedInt / 8);
	const unsigned int HighBits =
		(unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
	unsigned int hash = 0;
	unsigned int test = 0;
	unsigned int i = 0;

	for (i = 0; i < length; ++str, ++i)
	{
		hash = (hash << OneEighth) + (*str);

		if ((test = hash & HighBits) != 0)
		{
			hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
		}
	}

	return hash;
}
unsigned int ELFHash(const char* str, unsigned int length)
{
	unsigned int hash = 0;
	unsigned int x = 0;
	unsigned int i = 0;

	for (i = 0; i < length; ++str, ++i)
	{
		hash = (hash << 4) + (*str);

		if ((x = hash & 0xF0000000L) != 0)
		{
			hash ^= (x >> 24);
		}

		hash &= ~x;
	}

	return hash;
}
unsigned int BKDRHash(const char* str, unsigned int length)
{
	unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
	unsigned int hash = 0;
	unsigned int i = 0;

	for (i = 0; i < length; ++str, ++i)
	{
		hash = (hash * seed) + (*str);
	}

	return hash;
}
unsigned int SDBMHash(const char* str, unsigned int length)
{
	unsigned int hash = 0;
	unsigned int i = 0;

	for (i = 0; i < length; ++str, ++i)
	{
		hash = (*str) + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}
unsigned int DJBHash(const char* str, unsigned int length)
{
	unsigned int hash = 5381;
	unsigned int i = 0;

	for (i = 0; i < length; ++str, ++i)
	{
		hash = ((hash << 5) + hash) + (*str);
	}

	return hash;
}
unsigned int DEKHash(const char* str, unsigned int length)
{
	unsigned int hash = 0;
	unsigned int i = 0;

	for (i = 0; i < length; ++str, ++i)
	{
		hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
	}

	return hash;
}
unsigned int APHash(const char* str, unsigned int length)
{
	unsigned int hash = 0xAAAAAAAA;
	unsigned int i = 0;

	for (i = 0; i < length; ++str, ++i)
	{
		hash ^= ((i & 1) == 0) ? ((hash << 7) ^ (*str) * (hash >> 3)) :
			(~((hash << 11) + ((*str) ^ (hash >> 5))));
	}

	return hash;
}

//位图
typedef struct bitset
{
	char * a;
	size_t n;
}BitSet;
//初始化位图
void BitSetInit(BitSet* pbs, size_t n)
{
	pbs->n = n;
	size_t size = n / 8 + 1;
	pbs->a = (char*)bupt_malloc(sizeof(char) * size);
	//printf("%d\n", size);
	memset(pbs->a, 0, size);
}
//销毁位图(释放malloc出来的空间)
void BitSetDestory(BitSet* pbs)
{
	free(pbs->a);
	pbs->a = NULL;
	pbs->n = 0;
}
//设置要存的值位置为1
void BitSetSet(BitSet* pbs, size_t x)
{
	x = x % MAXNUM;
	int index = x / 8;
	int num = x % 8;
	//pbs->a[index] = (char*)bupt_malloc(sizeof(char));
	pbs->a[index] |= (1 << num);

}
//判断所给数字是否存在(存在返回1，不存在返回0)

void test() {
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
		j++;


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
				if (m == 1) {
					while (j==m) {
						j++;
						if (j == m - 1)
						{
							
							fputs(" yes\n", result);
							findnum++;//记录成功检索到的word个数
							find = 1;
							i = pnum;//检索到后跳过后续的比较
							for (n = 0; n < m; n++)
							{
								n = 0;
							}

						}
					}
					j = 0;
				}
				i++;//当前匹配失败检索下一个pattern中的字符串
			}
			if (find != 1) {
			
				fputs(" no\n", result);
				for (n = 0; n < m; n++)
				{
					n = 0;
				}
			}
			i = 0;//整个pattern没有检索到当前字符串，进入下一个字符串检索，pattern字符串定位从头开始
			m = 0;//一个字符串检索完成进入下一个，待匹配字符串字符定位回到0
			wnum++;//记录Word总个数

		}
		find = -1;//下一个字符串开始检索置状态为未找到
		
		m++;//定位待匹配字符串当前字符位置
	}
	//处理最后一个word
	i = 0;
	while (i < pnum)
	{
		if (m == 1) {
			while (n==0) {
				j++;
				if (j == m - 1)
				{
					
					fputs(" yes", result);
					findnum++;//记录成功检索到的word个数
					find = 1;
					for (j = 0; j < m; j++)
					{
						n = 0;
					}

				}
			}
			j = 0;
		}
		i++;//当前匹配失败检索下一个pattern中的字符串
	}
	if (find != 1) {
		
		fputs(" no", result);
		for (j = 0; j < m; j++)
		{
			n = 0;
		}
	}
}


int BitSetSearch(BitSet* pbs, size_t x)
{
	global_stats.cmpnum += 1;//统计调用hash函数的次数
	x = x % MAXNUM;
	int index = x / 8;
	int num = x % 8;

	int ret = pbs->a[index] & (1 << num);
	//printf("ret:%d\n", ret);
	if (0 == ret)
	{
		return 0;
	}
	else
		return 1;
}

int main()
{
	
	int searchCount = 0;
	int hitCount = 0;
	char* c = (char *)malloc(128 * sizeof(char));
	FILE * frp = fopen(pattern_file, "r");
	FILE * frw = fopen(word_file, "r");
	FILE * fw = fopen("result.txt", "w");
	BitSet bs;
	BitSetInit(&bs, MAXNUM);
	while (fscanf(frp, "%[^\n]", c) != EOF)
	{
		fgetc(frp);
		BitSetSet(&bs, BKDRHash(c, strlen(c)));
		BitSetSet(&bs, APHash(c, strlen(c)));
		BitSetSet(&bs, RSHash(c, strlen(c)));
		BitSetSet(&bs, JSHash(c, strlen(c)));
		BitSetSet(&bs, PJWHash(c, strlen(c)));
		BitSetSet(&bs, ELFHash(c, strlen(c)));
		BitSetSet(&bs, SDBMHash(c, strlen(c)));
		BitSetSet(&bs, DJBHash(c, strlen(c)));
		BitSetSet(&bs, DEKHash(c, strlen(c)));
	}
	fclose(frp);
	while (fscanf(frw, "%[^\n]", c) != EOF)
	{
		fgetc(frw);
		searchCount++;
		if (BitSetSearch(&bs, BKDRHash(c, strlen(c))) && BitSetSearch(&bs, APHash(c, strlen(c))) && BitSetSearch(&bs, RSHash(c, strlen(c))) && BitSetSearch(&bs, JSHash(c, strlen(c))) && BitSetSearch(&bs, PJWHash(c, strlen(c))) && BitSetSearch(&bs, ELFHash(c, strlen(c))) && BitSetSearch(&bs, SDBMHash(c, strlen(c))) && BitSetSearch(&bs, DJBHash(c, strlen(c))) && BitSetSearch(&bs, DEKHash(c, strlen(c))))
		{
			hitCount++;
			fprintf(fw, "%s yes\n", c);
			//printf("%s yes\n", c);
		}
		else
		{
			fprintf(fw, "%s no\n", c);
			//printf("%s no\n", c);
		}
	}

	fprintf(fw,"%lld ", global_stats.mem / 1024);
	fprintf(fw, "%lld ", global_stats.cmpnum);
	fprintf(fw, "%d ", searchCount);
	fprintf(fw, "%d", hitCount);
	fclose(frw);
	fclose(fw);
	BitSetDestory(&bs);
	return 0;

}
