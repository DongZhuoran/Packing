/*=============================================================================
#     FileName: Packing-1.cpp
#         Desc: problem of packing
#       Author: DongZhuoran
#        Email: 727533109@qq.com
#     HomePage: https://github.com/DongZhuoran
#      Version: 2.0.6
#   LastChange: 2016-05-23 19:51
#      History:
============================================================================*/

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define INF 1e99
#define TIMEEXCESS 1e99  //超时条件  Question: how to determine a proper one???
#define T 100            //初始温度
#define delta 0.90       //温度下降速度
#define THRESHOLD 1e-8   //停止搜索阈值条件
#define MOVE 1e3         //确定每个圆圆心的比较次数
#define MINOPT 1e-3      //optimize精度
#define alpha 0.1        //学习速率
#define Beta 0.90        //找下一个圆时半径伸缩因子
#define MINEX 1e-3       //有解条件

using namespace std;

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

double Getsum(Circle cir[], int n)
{
	double ans = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (dist(cir[i], cir[j]) < cir[i].radius + cir[j].radius)
				ans += pow(cir[i].radius + cir[j].radius - dist(cir[i], cir[j]), 2);
		}
		if (cir[i].x + cir[i].radius > 1)
			ans += pow((cir[i].x + cir[i].radius - 1), 2);
		if (cir[i].x - cir[i].radius < -1)
			ans += pow((cir[i].x - cir[i].radius + 1), 2);
		if (cir[i].y + cir[i].radius > 1)
			ans += pow((cir[i].y + cir[i].radius - 1), 2);
		if (cir[i].y - cir[i].radius < -1)
			ans += pow((cir[i].y - cir[i].radius + 1), 2);
	}
	return ans;
}

Circle GetCenter(Circle cir[], int n)
{
	Circle c;
	int flag = 0;
	c.radius = 0;
	c.x = 2 * (double)rand() / RAND_MAX - 1;
	c.y = 2 * (double)rand() / RAND_MAX - 1;
	while (flag < n)
	{
		c.x = 2 * (double)rand() / RAND_MAX - 1;
		c.y = 2 * (double)rand() / RAND_MAX - 1;
		flag = 0;
		for (int i = 0; i < n; i++)  //问题： for (int i = 0, flag = 0; i < n; i++){flag++;}最后flag=?
		{
			if (dist(cir[i], c) > cir[i].radius)
				flag++;
			else
				break;
		}
	}
	return c;
}

Circle GetCircle(Circle cir[], int n, double curradius)
{
	Circle MinCir;
	double MinEx = INF;
	MinCir.radius = 0;
	MinCir.x = 0;
	MinCir.y = 0;
	Circle c;
	double Ex = INF;
	for (int k = 0; k < MOVE; k++)
	{
		c = GetCenter(cir, n);
		c.radius = curradius;
		cir[n] = c;
		Ex = Getsum(cir, n + 1);
		if (Ex < MinEx)
		{
			MinCir = c;
			MinEx = Ex;
		}
	}
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
			if (cir[i].radius + cir[j].radius > dist(cir[i], cir[j]))
			{
				ax -= 2 * (cir[i].radius + cir[j].radius - pow(dist(cir[i], cir[j]), 0.5)) * pow(dist(cir[i], cir[j]), -0.5) * (x1 - cir[j].x);
				ay -= 2 * (cir[i].radius + cir[j].radius - pow(dist(cir[i], cir[j]), 0.5)) * pow(dist(cir[i], cir[j]), -0.5) * (y1 - cir[j].y);
			}
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
	while (fabs(ax) > MINOPT && fabs(ay) > MINOPT)
	{
		x1 = x2;
		y1 = y2;
		for (int i = 0; i < n; i++)
		{
			for (int j = i + 1; j < n; j++)
			{
				if (cir[i].radius + cir[j].radius > dist(cir[i], cir[j]))
				{
					ax -= 2 * (cir[i].radius + cir[j].radius - pow(dist(cir[i], cir[j]), 0.5)) * pow(dist(cir[i], cir[j]), -0.5) * (x1 - cir[j].x);
					ay -= 2 * (cir[i].radius + cir[j].radius - pow(dist(cir[i], cir[j]), 0.5)) * pow(dist(cir[i], cir[j]), -0.5) * (y1 - cir[j].y);
				}
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
	return cir[n - 1];
}

double Search(Circle cir[], int n)
{
	Circle c;
	double t = T;
	double ans = 0;
	double curradius = 1;
	srand((unsigned)time(NULL));
	clock_t start_time, end_time;
	double run_time = 0;
	while (t > THRESHOLD)
	{
		for (int i = 0; i < n; i++)
		{
			curradius = 1;
			c = GetCircle(cir, i, curradius);
			cir[i] = c;
			c = optimize(cir, i + 1);
			cir[i] = c;
			start_time = clock();
			while (Getsum(cir, i + 1) > MINEX / n)
			{
				//printf("curradius = %f\n", curradius);
				for (int k = 0; k < 10 && Getsum(cir, i + 1) > MINEX / n; k++)
				{
					c = GetCircle(cir, i, curradius);
					cir[i] = c;
					c = optimize(cir, i + 1);
					cir[i] = c;
				}
				if (Getsum(cir, i + 1) > MINEX / n)
					curradius *= Beta;
				end_time = clock();
				run_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
				if (run_time > TIMEEXCESS)
				{
					printf("Time Excess in %dth circle!\n", i + 1);
					break;
				}
			}
			cir[i] = c;
			ans += Getsum(cir, i + 1);
		}
		if (ans < MINEX)
			return ans;
		else
			t *= delta;
	}
	return -1;
}


int _tmain(int argc, _TCHAR* argv[])
{
	clock_t start_time, end_time;
	int num;
	double ans;
	Circle *cir = NULL;
	while (1)
	{
		cout << "Please enter the number of circles:" << endl;
		cin >> num;
		start_time = clock();
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
		end_time = clock();
		printf("The running time dealing with %d circle(s) is %f seceonds\n", num, (double)(end_time - start_time) / CLOCKS_PER_SEC);
	}

	return 0;
}

