#include <stdio.h>
#include<stdlib.h>

struct newarrays
{
	int length;
	char word[128];

}newarray1;

struct newarrays * newarray;

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


int main()
{
	global_stats.cmpnum = 0;
	global_stats.mem = 0;
	newarray = (struct newarrays*)bupt_malloc(sizeof(struct newarrays) * 1280000);
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
	int end = 1;//������־��
	FILE *fp = NULL;
	FILE *fp1 = NULL;
	FILE *result = NULL;
	fp = fopen("patterns-127w.txt", "r");
	fp1 = fopen("words-98w.txt", "r");
	result = fopen("result.txt", "w");
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
	pnum = i;//pattern�е����ַ�����
	i = 0;
	j = 0;//������λ������ȵ��ַ�����ǰ�ַ��ж�λ��

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
							fputs(newarray1.word,result);
							fputs(" yes",result);
							findnum++;//��¼�ɹ���������word����
							find = 1;
							i = pnum;//�����������������ıȽ�
							for (n = 0; n < m; n++)
							{
								newarray1.word[n] = '\0';
							}

						}
					}
					j = 0;
				}
				i++;//��ǰƥ��ʧ�ܼ�����һ��pattern�е��ַ���
			}
			if (find != 1) {
				fputs(newarray1.word, result);
				fputs(" no",result);
				for (n = 0; n < m; n++)
				{
					newarray1.word[n] = '\0';
				}
			}
			i = 0;//����patternû�м�������ǰ�ַ�����������һ���ַ���������pattern�ַ�����λ��ͷ��ʼ
			m = 0;//һ���ַ���������ɽ�����һ������ƥ���ַ����ַ���λ�ص�0
			wnum++;//��¼Word�ܸ���

		}
		find = -1;//��һ���ַ�����ʼ������״̬Ϊδ�ҵ�
		newarray1.word[m] = ch1;
		m++;//��λ��ƥ���ַ�����ǰ�ַ�λ��
	}
	//�������һ��word
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
					findnum++;//��¼�ɹ���������word����
					find = 1;
					for (j = 0; j < m; j++)
					{
						newarray1.word[j] = '\0';
					}

				}
			}
			j = 0;
		}
		i++;//��ǰƥ��ʧ�ܼ�����һ��pattern�е��ַ���
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
	fprintf( result, "%lld",global_stats.mem/1024);
	fputs("kb ",result);
	fprintf(result,"%lld",global_stats.cmpnum);
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

