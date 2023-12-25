#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define N 3

//记录内存中的页面（第一个下标）以及在内存中的存在时间（第二个下标）,页面编号为-1表示为空
int storeIn[N][2];
//统计缺页次数
int count = 0;
int choice;

//数组初始化
void init(void) {
    for (int i = 0; i < N; i++) {
        storeIn[i][0] = -1;
        storeIn[i][1] = 0;
    }
}

//内存中的各个元素添加存在时间
void add(void) {
    for (int i = 0 ; i < N; i++) {
        storeIn[i][1] += 1;
    }
}

//判断storeIn数组是否已经被占满
bool isFull(void) {
    for (int i = 0; i < N; i++) {
        if (storeIn[i][0] == -1) {
            return false;
        }
    }
    return true;
}

//判断页面编号是否已经储存
bool isIn(int number) {
    for (int i = 0; i < N; i++) {
        if (storeIn[i][0] == number) {
            if (choice == 2) {
                storeIn[i][1] = 0; //如果是LRU算法，在命中后还需要额外将其优先级重置
            }
            return true;
        }
    }
    return false;
}

//将页面插入到内存中
void insert(int number) {
    if (isFull()) {
        int level = 0;
        int pos = 0;
        for (int i = 0; i < N; i++) {
            if (storeIn[i][1] > level) {
                level = storeIn[i][1];
                pos = i;
            }
        }
        storeIn[pos][0] = number;
        storeIn[pos][1] = 0;
        count++;
    }
    else {
        for (int i = 0; i < N; i++) {
            if (storeIn[i][0] == -1) {
                storeIn[i][0] = number;
                storeIn[i][1] = 0;
                count++;
                break;
            }
        }
    }
}

//输出内存中储存的页面编号信息
void output(void) {
    printf("目前内存中的页面编号如下：");
    int i = 0;
    while (i < N && storeIn[i][0] != -1) {
        printf("%d\t", storeIn[i][0]);
        i++;
    }
    printf("\n");
}


//计算页面置换算法，统计缺页次数和缺页率
void calc(int* queue, int type) {
    int read;
    int length;
    if (type == 1) {
        length = 20;
    }
    else if (type == 2) {
        length = 1000;
    }
    for (int i = 0; i < length; i++) {
        if (queue[i] == -1) {
            break;
        }
        else {
            read = queue[i];
            if (!isIn(read)) {
                insert(read);
                printf("%d添加到内存页框成功！\n", read);
                add();
                output();
            }
            else {
                printf("%d已经命中！\n", read);
                add();
                output();
            }
        }
    }
}

//用户输入然后进行统计
void input(void) {
    printf("请输入访问页面的序列，最多输入20个数字(输入-1提前结束)：");
    int userIn[20];
    int i = 0;
    while (i < 20) {
        scanf("%d", &userIn[i]);
        i++;
        if (userIn[i - 1] == -1) {
            i--;
            break;
        }
    }
    printf("输入成功！读取页面的序列如下：\n");
    i = 0;
    while (i < 20 && userIn[i] != -1) {
        printf("%d\t", userIn[i]);
        i++;
    }
    printf("\n");
    calc(userIn, 1);
    double rate = (double)count / i;
    printf("缺页次数：%d次，缺页率：%.3lf\n", count, rate);
}

//系统随机生成序列然后进行统计
void gen(void) {
    srand((unsigned int)time);
    int range = rand() % 11 + 5; //系统随机生成一个范围在5-10的整数，表示最大页面编号
    int queue[1000];
    for (int i = 0; i < 1000; i++) {
        queue[i] = rand() % range + 1;
    }
    calc(queue, 2);
    double rate = (double)count / 1000;
    printf("缺页次数：%d次，缺页率：%.3lf\n", count, rate);
}

int main(void) {
    printf("输入1采用FIFO算法，输入2采用LRU算法：");
    scanf("%d", &choice);
    while (choice != 1 && choice != 2) {
        printf("输入有误，请重新输入：");
        scanf("%d", &choice);
    }
    init();
    printf("输入1由系统随机产生一个长度为1000的页面访问序列，输入2由用户手动输入序列：");
    int choice2;
    scanf("%d", &choice2);
    while (choice2 != 1 && choice2 != 2) {
        printf("输入有误，请重新输入：");
        scanf("%d", &choice2);
    }
    if (choice2 == 2) {
        input();
    }
    else if (choice2 == 1) {
        gen();
    }
    return 0;
}
