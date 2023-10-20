// scheduling.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "scheduling.h"
#include "stdlib.h"


#define getstruct(type) (type *) malloc(sizeof(type));
PCB* ready = NULL, * p;


bool sort()  //����������ȼ���ȷ������ִ��˳��ĺ�����������ģʽ�£����ȼ����ж������ǳ���ִ����Ҫ��ʱ�䣬ʱ��Խ�����ȼ�Խ��
{
	PCB* first, * second;
	int insert = 0;
	if ((ready == NULL) || (p->ntime < ready->ntime))  //û�д��ھ���״̬�Ľ��̣����ߴ��ڱȴ��ھ���״̬�Ľ������ȼ��ߵĽ��̣�ϵͳ�������滻
	{
		p->link = ready;
		ready = p;
	}
	else
	{
		first = ready;  //���ȼ��ϸߵĽ��̣���ֱ������Ϊ����״̬
		second = first->link;   //��ǰһ�������ȼ�����͵Ľ���
		while (second != NULL)  //�������ȼ��ϵ͵Ľ��̣�Ȼ��������ȼ����в���
		{
			if (p->ntime < second->ntime)  //���뵽���ȼ��͵Ľ���ǰ��
			{
				p->link = second;
				first->link = p;
				second = NULL;
				insert = 1;
			}
			else  //���뵽���ȼ��͵Ľ��̺�
			{
				first = first->link;
				second = second->link;
			}
		}
		if (insert == 0) first->link = p;
	}
	return(true);
}

bool input()  //�û��������������
{
	int num;
	printf("\n�������������");
	scanf_s("%d", &num);
	for (int i = 0; i < num; i++)
	{
		printf("\n���̺�:%d", i);
		p = getstruct(PCB);
		printf("\n���������:");
		scanf_s("%s", p->name, 10);
		printf("\n�����������:");
		int temp;
		//scanf_s("%d", &temp, sizeof(int));
		//p->nice = temp;
		scanf_s("%d", &(p->nice), sizeof(int));
		printf("\n�����������ʱ��:");
		scanf_s("%d", &(p->ntime), sizeof(int));
		printf("\n");
		p->rtime = 0;
		p->state = 'W';
		p->link = NULL;
		sort();
	}
	return(true);
}
int space()  //ͳ�ƽ��̵�����
{
	int i = 0;
	PCB* pr = ready;
	while (pr != NULL)
	{
		i++;
		pr = pr->link;
	}
	return(i);
}
bool disp(PCB* pr)  //������̵���Ϣ
{
	printf("\n name \t state \t nice \t needtime \t runtime \n");
	printf(" %s\t", pr->name);
	printf(" %c\t", pr->state);
	printf(" %d\t", pr->nice);
	printf(" %d\t", pr->ntime);
	printf("         %d\t", pr->rtime);
	return(true);
}
bool check()  //�����̶����Ƿ�Ϊ�գ��������������Ϣ
{
	PCB* pr;
	printf("\n ****��ǰ���еĽ����ǣ�%s", p->name);
	disp(p);
	pr = ready;
	if (pr != NULL)
	{
		printf("\n****��ǰ��������״̬Ϊ��");
	}
	else
	{
		printf("\n****��ǰ��������״̬Ϊ��\n");
	}
	while (pr != NULL)  //���ս������ȼ��������������Ϣ
	{
		disp(pr);
		pr = pr->link;
	}
	return(true);
}
bool destory()  //����������ɣ��������٣��ͷ��ڴ�ռ����������
{
	printf("\n����[%s]�Ѿ������١�\n", p->name);
	free(p);
	return(true);
}
bool running()
{
	while (p->rtime < p->ntime)  p->rtime++;  //��ǰ�������еĽ���������ʱ����
	if (p->rtime >= p->ntime) {  //������ʱ�����趨ֵ���������
		destory();
	}
	return(true);  //�����Ƿ���ռ�̽�������ģʽ��������˳�򲻻���Ϊ��ǰ���̵����ٶ��ı䣬��������ٴ��������˳��
}


int _tmain(int argc, _TCHAR* argv[])  //������
{
	int len, h = 0;
	char ch;
	input();
	len = space();
	while ((len != 0) && (ready != NULL))
	{
		ch = getchar();
		h++;
		printf("\n The execute number:%d \n", h);
		p = ready;
		ready = p->link;
		p->link = NULL;
		p->state = 'R';
		check();
		running();
		printf("\n�����������.......");
		ch = getchar();
	}
	printf("\n\n ���н����Ѿ�������ɣ�\n");
	ch = getchar();
	return 0;
}
