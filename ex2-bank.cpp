/*********************************银行家算法实验*********************************/
/*注意：本实验仅附带已分配资源是否超过系统可用资源的错误检测，并不附带已分配资源是否超过该进程最大需求资源的检测
 *因此若输入不当，可能会出现需求矩阵是负数的情况，请合理填写（一般也不会有人故意这样子输入吧）*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define N 3
#define OVERFLOW 2
#define ERROR -1
#define max maxnn

struct pcb {  //定义进程结构体
	int no;  //进程序号
	int max[N];  //最大需求
	int allo[N];  //已分配
	int need[N];  //需求
	bool flag;  //用于记录是否已经运行完成的标签
	struct pcb* next;  //下一进程节点
};
typedef struct pcb PCB;

int available[N];  //可用系统资源
int count;  //进程数量
int* seqlist;  //记录进程运行序列
PCB* p;
PCB* events = NULL;

bool sort() {  //将零散的进程整合在一起
	if (events == NULL) {
		//p->next = events;
		events = p;
	}
	else {
		PCB* temp = events;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = p;
	}
	return (true);
}

bool display(PCB* temp) {  //显示详细信息，进程的编号按照进程的添加顺序展示
	printf("%d\t", temp->no);
	for (int i = 0; i < N - 1; i++) {
		printf("%d ", temp->max[i]);
	}
	printf("%d\t\t", temp->max[N - 1]);
	for (int i = 0; i < N - 1; i++) {
		printf("%d ", temp->allo[i]);
	}
	printf("%d\t\t", temp->allo[N - 1]);
	for (int i = 0; i < N - 1; i++) {
		printf("%d ", temp->need[i]);
	}
	printf("%d\n", temp->need[N - 1]);
	return (true);
}

int find() {  //寻找下一个可以运行的进程，如果没有则返回-1
	PCB* temp = events;
	while (temp != NULL) {
		if (temp->flag == true) {  //如果进程已经运行过了，则跳过，寻找下一个进程
			temp = temp->next;
			continue;
		}
		else {
			bool isValid = true;
			for (int i = 0; i < N; i++) {
				if (temp->need[i] > available[i]) {  //系统可用资源不能满足进程运行需要，则无法运行
					isValid = false;
					break;
				}
			}
			if (isValid == true) {  //系统资源满足运行条件，运行进程结束后释放已占用资源，在运行顺序中记录该进程
				temp->flag = true;  //标记进程已运行过，下次再次寻找时就不会再寻找该进程
				for (int i = 0; i < N; i++) {  //释放资源
					available[i] += temp->allo[i];
				}
				return temp->no;
			}
			else {  //不满足运行条件则寻找下一个进程（不代表系统一定不安全）
				temp = temp->next;
				continue;
			}
		}
	}
	return -1;
}

bool isSafe() {
	int n = count;
	int seq[100];  //给出进程运行序列
	int nextEvent = -1;  //代表下一个可运行的进程，如果等于-1则说明没有了
	for (int i = 0; i < n; i++) {
		nextEvent = find();
		if (nextEvent == -1) {  //一旦出现-1就说明系统资源无法满足剩下的所有进程的运行，则系统不安全
			return (false);
		}
		else {  //出现了可以运行的进程，记录之，然后再次寻找下一个可运行的进程
			seq[i] = nextEvent;
		}
	}
	seqlist = seq;  //如果代码运行到这里就说明所有的进程都已经运行完了，说明系统安全，记录运行序列，返回安全状态
	return (true);
}


bool input() {  //控制用户输入进程以及系统资源
	printf("请输入系统可用资源，请输入%d个数字：", N);
	for (int i = 0; i < N; i++) {
		scanf("%d", &available[i]);
	}
	printf("请输入进程数量：");
	scanf("%d", &count);
	for (int i = 0; i < count; i++) {
		p = (PCB*)malloc(sizeof(PCB));
		if (!p) {
			printf("添加进程失败！");
			exit(OVERFLOW);
		}
		printf("****第%d个进程****\n", i + 1);
		printf("请输入最大需求，请输入%d个数字：", N);
		for (int j = 0; j < N; j++) {
			scanf("%d", &(p->max[j]));
		}
		printf("请输入已分配，请输入%d个数字：", N);
		for (int j = 0; j < N; j++) {
			scanf("%d", &(p->allo[j]));
			available[j] -= p->allo[j];
		}
		for (int j = 0; j < N; j++) {  //计算进程的需求资源数量
			p->need[j] = (p->max[j]) - (p->allo[j]);
		}
		p->flag = false;
		p->next = NULL;
		p->no = i;
		sort();
	}
	for (int i = 0; i < N; i++) {  //检测已分配的资源是否已经超过系统可用资源，这种情况下一定不可能成立（造成此类情况可能是由于用户输入错误）
		if (available[i] < 0) {
			printf("所有已分配的资源数量之和超过了系统可用资源！\n"); 
			//printf("系统不安全，可能会出现死锁。");
			exit(ERROR);
		}
	}
	printf("-----------------------------\n%d个进程添加成功！进程信息与系统资源如下：\n", count);
	printf("No.\tMax\t\tAllocation\tNeed\n");
	PCB* temp = events;
	while (temp != NULL) {
		display(temp);
		temp = temp->next;
	}
	printf("系统可用资源：%d %d %d\n", available[0], available[1], available[2]);
	printf("-----------------------------\n");
	return (true);
}

bool extraAllo() {
	int num;
	int change[N];
	printf("请输入需要额外分配资源的进程编号（%d-%d）：", 0, count - 1);
	scanf("%d", &num);
	printf("请输入分配量，请输入%d个数字：", N);
	for (int i = 0; i < N; i++) {
		scanf("%d", &change[i]);
	}
	PCB* temp = events;
	while (temp != NULL && temp->no != num) {  //找到与用户输入进程号相匹配的进程
		temp = temp->next;
	}
	if (temp == NULL) {
		printf("未找到相关进程！分配失败！\n");
	}
	else {
		for (int i = 0; i < N; i++) {  //更改进程数据以及系统资源数据
			temp->allo[i] += change[i];
			temp->need[i] -= change[i];
			available[i] -= change[i];
		}
		for (int i = 0; i < N; i++) {  //检测已分配的资源是否已经超过系统可用资源，这种情况下一定不可能成立（造成此类情况可能是由于用户输入错误）
			if (available[i] < 0) {
				printf("所有已分配的资源数量之和超过了系统可用资源！\n"); 
				//printf("系统不安全，可能会出现死锁。");
				exit(ERROR);
			}
		}
		printf("-----------------------------\n资源分配成功！新的进程信息与系统资源如下：\n");
		printf("No.\tMax\t\tAllocation\tNeed\n");
		temp = events;
		while (temp != NULL) {
			display(temp);
			temp = temp->next;
		}
		printf("系统可用资源：%d %d %d\n", available[0], available[1], available[2]);
	}
	printf("-----------------------------\n是否还需要额外分配资源？是=1，否=其他：");
	int choice = 0;
	scanf("%d", &choice);
	if (choice == 1) {
		extraAllo();
	}
	return (true);
}

int main() {  //程序主函数，用于输出结果
	input();
	int choice;
	printf("是否额外分配进程资源？是=1，否=其他：");
	scanf("%d", &choice);
	if (choice == 1) {
		extraAllo();
	}
	if (isSafe()) {
		printf("系统是安全的。一个安全的进程运行序列：");
		for (int i = 0; i < count; i++) {
			printf("%d ", seqlist[i]);
		}
	}
	else {
		printf("系统不安全，可能会出现死锁。");
	}
	return 0;
}
