// scheduling.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "scheduling.h"
#include "stdlib.h"


#define getstruct(type) (type *) malloc(sizeof(type));
PCB* ready = NULL, * p;


bool sort()  //整理进程优先级，确定进程执行顺序的函数，在这种模式下，优先级的判断依据是程序执行需要的时间，时间越少优先级越高
{
	PCB* first, * second;
	int insert = 0;
	if ((ready == NULL) || (p->ntime < ready->ntime))  //没有处于就绪状态的进程，或者存在比处于就绪状态的进程优先级高的进程，系统将进行替换
	{
		p->link = ready;
		ready = p;
	}
	else
	{
		first = ready;  //优先级较高的进程，会直接设置为就绪状态
		second = first->link;   //比前一进程优先级别更低的进程
		while (second != NULL)  //存在优先级较低的进程，然后根据优先级进行插入
		{
			if (p->ntime < second->ntime)  //插入到优先级低的进程前方
			{
				p->link = second;
				first->link = p;
				second = NULL;
				insert = 1;
			}
			else  //插入到优先级低的进程后方
			{
				first = first->link;
				second = second->link;
			}
		}
		if (insert == 0) first->link = p;
	}
	return(true);
}

bool input()  //用户输入进程主函数
{
	int num;
	printf("\n请输入进程数：");
	scanf_s("%d", &num);
	for (int i = 0; i < num; i++)
	{
		printf("\n进程号:%d", i);
		p = getstruct(PCB);
		printf("\n输入进程名:");
		scanf_s("%s", p->name, 10);
		printf("\n输入进优先数:");
		int temp;
		//scanf_s("%d", &temp, sizeof(int));
		//p->nice = temp;
		scanf_s("%d", &(p->nice), sizeof(int));
		printf("\n输入进程运行时间:");
		scanf_s("%d", &(p->ntime), sizeof(int));
		printf("\n");
		p->rtime = 0;
		p->state = 'W';
		p->link = NULL;
		sort();
	}
	return(true);
}
int space()  //统计进程的数量
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
bool disp(PCB* pr)  //输出进程的信息
{
	printf("\n name \t state \t nice \t needtime \t runtime \n");
	printf(" %s\t", pr->name);
	printf(" %c\t", pr->state);
	printf(" %d\t", pr->nice);
	printf(" %d\t", pr->ntime);
	printf("         %d\t", pr->rtime);
	return(true);
}
bool check()  //检查进程队列是否为空，控制输出进程信息
{
	PCB* pr;
	printf("\n ****当前运行的进程是：%s", p->name);
	disp(p);
	pr = ready;
	if (pr != NULL)
	{
		printf("\n****当前就绪队列状态为：");
	}
	else
	{
		printf("\n****当前就绪队列状态为空\n");
	}
	while (pr != NULL)  //按照进程优先级排序输出进程信息
	{
		disp(pr);
		pr = pr->link;
	}
	return(true);
}
bool destory()  //进程运行完成，进行销毁，释放内存空间给其他进程
{
	printf("\n进程[%s]已经被销毁。\n", p->name);
	free(p);
	return(true);
}
bool running()
{
	while (p->rtime < p->ntime)  p->rtime++;  //当前正在运行的进程运行用时增加
	if (p->rtime >= p->ntime) {  //运行用时到达设定值后进行销毁
		destory();
	}
	return(true);  //由于是非抢占短进程优先模式，各进程顺序不会因为当前进程的销毁而改变，因此无需再次整理进程顺序
}


int _tmain(int argc, _TCHAR* argv[])  //主函数
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
		printf("\n按任意键继续.......");
		ch = getchar();
	}
	printf("\n\n 所有进程已经运行完成！\n");
	ch = getchar();
	return 0;
}
