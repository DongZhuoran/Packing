/*=============================================================================
#     FileName: Packing-2.cpp
#         Desc: problem of packing
#       Author: DongZhuoran
#        Email: 727533109@qq.com
#     HomePage: https://github.com/DongZhuoran
#      Version: 0.0.1
#   LastChange: 2016-05-22 17:02:23
#      History:
============================================================================*/

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define INF 1e99
#define T 100            //初始温度
#define delta 0.98       //温度下降速度
#define THRESHOLD 1e-8   //停止搜索阈值条件
#define MOVE 1e6         //确定每个圆圆心的比较次数
#define PREOPT 1e-6      //optimize精度
#define alpha 0.2        //学习速率
#define MINEX 1e-1       //有解条件

using namespace std;

int dx[4] = { 0, 0, -1, 1 };
int dy[4] = { 1, -1, 0, 0 };  //上下左右四个方向

struct Circle
{
	double radius;
	double x;
	double y;
};

Circle* initialize(int n)
{
	Circle *cir = (Circle*)malloc(sizeof(Circle) * n);
	for (int i = 0; i < n; i++)
	{
		cir[i].radius = 0;
		cir[i].x = 0;
		cir[i].y = 0;
	}
	return cir;
}

double dist(Circle A, Circle B)
{
	return sqrt(pow((A.x - B.x), 2) + pow((A.y - B.y), 2));
}

double Getsum(Circle cir[], int n, Circle c)
{
	double ans = 0;
	for (int i = 0; i < n; i++)
	{
		ans += pow(cir[i].radius + c.radius - dist(cir[i], c), 2);
	}
	if (c.x + c.radius > 1)
		ans += pow((c.x + c.radius - 1), 2);
	if (c.x - c.radius < -1)
		ans += pow((c.x - c.radius + 1), 2);
	if (c.y + c.radius > 1)
		ans += pow((c.y + c.radius - 1), 2);
	if (c.y - c.radius < -1)
		ans += pow((c.y - c.radius + 1), 2);
	/*printf("Getsum: dx = %f\n", c.x);
	printf("Getsum: dy = %f\n", c.y);
	printf("ans = %f\n", ans);
	getchar();*/
	return ans;
}

Circle GetCenter(Circle cir[], int n)
{
	Circle c;
	int flag = 0;
	c.radius = 0;
	c.x = 2 * (double)rand() / RAND_MAX - 1;
	c.y = 2 * (double)rand() / RAND_MAX - 1;
	/*printf("dx = %f\n", c.x);
	printf("dy = %f\n", c.y);*/
	while (flag < n)
	{
		c.x = 2 * (double)rand() / RAND_MAX - 1;
		c.y = 2 * (double)rand() / RAND_MAX - 1;
		/*printf("dx = %f\n", c.x);
		printf("dy = %f\n", c.y);
		getchar();*/
		flag = 0;
		for (int i = 0; i < n; i++)  //问题： for (int i = 0, flag = 0; i < n; i++){flag++;}最后flag=?
		{
			//printf("The dist between cir[i] to c: %f\n", dist(cir[i], c));
			if (dist(cir[i], c) > cir[i].radius)
				flag++;
			else
				break;
		}
		//printf("flag is : %d\n", flag);
	}
	//printf("find one\n");
	return c;
}

Circle GetCircle(Circle cir[], int n, double curradius)
{
	Circle MinCir;
	double MinEx = INF;
	MinCir.radius = 0;
	MinCir.x = 0;
	MinCir.y = 0;
	//配合梯度下降法使用
	Circle c;
	double Ex = INF;
	for (int k = 0; k < MOVE; k++)
	{
		c = GetCenter(cir, n);
		c.radius = curradius;
		Ex = Getsum(cir, n, c);
		if (Ex < MinEx)
		{
			MinCir = c;
			MinEx = Ex;
		}
	}
	//printf("MinEx = %f\n", MinEx);
	/*Circle c;
	double Ex = INF;
	while (MinEx > limit)
	{
		c = GetCenter(cir, n);
		c.radius = curradius;
		Ex = Getsum(cir, n, c);
		if (Ex < MinEx)
		{
			MinCir = c;
			MinEx = Ex;
		}
	}*/
	//printf("limit = %f\n", limit);
	return MinCir;
}

Circle optimize(Circle cir[], int n)
{
	double x1 = cir[n - 1].x;
	double y1 = cir[n - 1].y;
	double ax = 0;
	double ay = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			ax -= 2 * (cir[i].radius + cir[j].radius - pow(dist(cir[i], cir[j]), 0.5)) * pow(dist(cir[i], cir[j]), -0.5) * (x1 - cir[j].x);
			ay -= 2 * (cir[i].radius + cir[j].radius - pow(dist(cir[i], cir[j]), 0.5)) * pow(dist(cir[i], cir[j]), -0.5) * (y1 - cir[j].y);
		}
		if (cir[i].x + cir[i].radius > 1)
			ax += 2 * (cir[i].x + cir[i].radius - 1);
		if (cir[i].x - cir[i].radius < -1)
			ax += 2 * (cir[i].x - cir[i].radius + 1);
		if (cir[i].y + cir[i].radius > 1)
			ay += 2 * (cir[i].y + cir[i].radius - 1);
		if (cir[i].y - cir[i].radius < -1)
			ay += 2 * (cir[i].y - cir[i].radius + 1);
	}
	double x2 = x1 - alpha * ax;
	double y2 = y1 - alpha * ay;
	cir[n - 1].x = x2;
	cir[n - 1].y = y2;
	//while ((x2 - x1 > PREOPT) && (y2 - y1) > PREOPT)
	while (ax == 0 && ay == 0)
	{
		x1 = x2;
		y1 = y2;
		for (int i = 0; i < n; i++)
		{
			for (int j = i + 1; j < n; j++)
			{
				ax -= 2 * (cir[i].radius + cir[j].radius - pow(dist(cir[i], cir[j]), 0.5)) * pow(dist(cir[i], cir[j]), -0.5) * (x1 - cir[j].x);
				ay -= 2 * (cir[i].radius + cir[j].radius - pow(dist(cir[i], cir[j]), 0.5)) * pow(dist(cir[i], cir[j]), -0.5) * (y1 - cir[j].y);
			}
			if (cir[i].x + cir[i].radius > 1)
				ax += 2 * (cir[i].x + cir[i].radius - 1);
			if (cir[i].x - cir[i].radius < -1)
				ax += 2 * (cir[i].x - cir[i].radius + 1);
			if (cir[i].y + cir[i].radius > 1)
				ay += 2 * (cir[i].y + cir[i].radius - 1);
			if (cir[i].y - cir[i].radius < -1)
				ay += 2 * (cir[i].y - cir[i].radius + 1);
		}
		x2 = x1 - alpha * ax;
		y2 = y1 - alpha * ay;
		cir[n - 1].x = x2;
		cir[n - 1].y = y2;
	}
	/*printf("delta1 = %f, delta2 = %f\n", x2 - x1, y2 - y1);
	getchar();*/
	return cir[n - 1];
}

double Search(Circle cir[], int n)
{
	Circle c;
	double t = T;
	double ans = 0;
	double curradius;
	int *flag = (int*)malloc(sizeof(int) * n);
	srand((unsigned)time(NULL));
	for (int i = 0; i < n; i++)
		flag[i] = 0;
	while (t > THRESHOLD)
	{
		curradius = PREOPT;
		for (int i = 0; i < n; i++)
		{
			curradius = PREOPT;
			if (flag[i] == 1)
				continue;
			c = GetCircle(cir, i, curradius);
			//梯度下降法优化实现
			cir[i] = c;
			c = optimize(cir, i + 1);
			while (Getsum(cir, i, c) < MINEX / n)
			{
				printf("curradius = %f\n", curradius);
				curradius *= 1.1;
				c = GetCircle(cir, i, curradius);
				cir[i] = c;
				c = optimize(cir, i + 1);				
				if (Getsum(cir, i, c) > MINEX / n)
					for (int k = 0; k < 10 && Getsum(cir, i, c) > MINEX / n; k++)
					{
						c = GetCircle(cir, i, curradius);
						cir[i] = c;
						c = optimize(cir, i + 1);
					}
			}
			c.radius = curradius / 1.1;
			cir[i] = c;
			flag[i] = 1;
			ans += Getsum(cir, i, c);
			/*printf("dx = %f\n", c.x);
			printf("dy = %f\n", c.y);
			printf("ans = %f\n", ans);
			getchar();*/
		}
		if (ans < MINEX)
			return ans;
		else
			t *= delta;
	}
	return ans;
}


int _tmain(int argc, _TCHAR* argv[])
{
	int num;
	double ans;
	Circle *cir = NULL;
	while (1)
	{
		cout << "Please enter the number of circles:" << endl;
		cin >> num;
		if (num > 0)
		{
			cir = initialize(num);
			ans = Search(cir, num);
			if (ans == -1)
				cout << "There is no answer, please try again" << endl;
			else
			{
				for (int i = 0; i < num; i++)
					printf("The radius and coordinate are: %f, (%f, %f)\n", cir[i].radius, cir[i].x, cir[i].y);
				printf("e: %f\n", ans);
			}
		}
		else
			break;
	}

	return 0;
}

