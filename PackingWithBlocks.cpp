/*=============================================================================
#     FileName: PackingWithBlocks.cpp
#         Desc: problem of packing
#       Author: DongZhuoran
#        Email: 727533109@qq.com
#     HomePage: https://github.com/DongZhuoran
#      Version: 1.1.1
#   LastChange: 2016-06-05
#      History:
============================================================================*/

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define INF 1e99
#define TIMEEXCESS 1e99  //��ʱ����  Question: how to determine???
#define T 100            //��ʼ�¶�
#define delta 0.90       //�¶��½��ٶ�
#define THRESHOLD 1e-8   //ֹͣ������ֵ����
#define MOVE 1e3         //ȷ��ÿ��ԲԲ�ĵıȽϴ���
#define MINOPT 1e-3      //optimize����
#define alpha 0.1        //ѧϰ����
#define Beta 2           //ÿ��Ѱ����һ��Բʱ�뾶����ϵ��
#define MINEX 1e-3       //�н�����

using namespace std;

int dx[4] = { 0, 0, -1, 1 };
int dy[4] = { 1, -1, 0, 0 };  //���������ĸ�����

struct Circle
{
	double radius;
	double x;
	double y;
};

typedef Circle Block;

Circle* initCircles(int n)
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

Block* initBlocks(int n)
{
	Block tmpBlo;
	Block *blo = (Block*)malloc(sizeof(Block) * n);
	for (int i = 0; i < n; i++)
	{
		tmpBlo.radius = 0;
		cout << "x = ";
		cin >> tmpBlo.x;
		cout << "y = ";
		cin >> tmpBlo.y;
		blo[i] = tmpBlo;
	}
	return blo;
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
		//printf("cir[%d].x = %f\ncir[%d].y = %f\n", i, cir[i].x, i, cir[i].y);
	}
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
	while (flag < n)
	{
		c.x = 2 * (double)rand() / RAND_MAX - 1;
		c.y = 2 * (double)rand() / RAND_MAX - 1;
		flag = 0;
		for (int i = 0; i < n; i++)  //���⣺ for (int i = 0, flag = 0; i < n; i++){flag++;}���flag=?
		{
			if (dist(cir[i], c) > cir[i].radius)
				flag++;
			else
				break;
		}
	}
	return c;
}

Circle GetCircle(Circle cir[], Block blo[], int n, int m, double curradius)
{
	Circle MinCir;
	double MinEx = INF;
	MinCir.radius = 0;
	MinCir.x = 0;
	MinCir.y = 0;
	Circle c;
	double Ex = INF;
	int block_flag = 0;
	for (int k = 0; k < MOVE; k++)
	{
		if (m == 0)
		{
			c = GetCenter(cir, n);
			c.radius = curradius;
		}
		else
		{
			while (block_flag < m)
			{
				for (int k = 0; k < 10 && block_flag < m; k++)
				{
					block_flag = 0;
					c = GetCenter(cir, n);
					c.radius = curradius;
					for (int i = 0; i < m; i++)
					{
						if (dist(blo[i], c) >= c.radius)
							block_flag++;
						else
							break;
					}
				}
				if (block_flag < m)
					curradius *= 0.9;
				/*printf("block_flag = %d\n", block_flag);
				getchar();*/
			}
		}
		cir[n] = c;
		Ex = Getsum(cir, n + 1);
		if (Ex < MinEx)
		{
			MinCir = c;
			MinEx = Ex;
		}
	}
	//printf("find circle\n");
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
	/*printf("delta1 = %f, delta2 = %f\n", x2 - x1, y2 - y1);
	getchar();*/
	return cir[n - 1];
}

//n is the number of circles, m is the number of blocks
double Search(Circle cir[], Block blo[], int n, int m)
{
	Circle c;
	double t = T;
	double ans = 0;
	double curradius = 1;
	int *flag = (int*)malloc(sizeof(int) * n);
	srand((unsigned)time(NULL));
	clock_t start_time, end_time;
	double run_time = 0;
	while (t > THRESHOLD)
	{
		for (int i = 0; i < n; i++)
			flag[i] = 0;
		for (int i = 0; i < n; i++)
		{
			if (flag[i] == 1)
				continue;
			//�ݶ��½����Ż�ʵ��
			curradius = 1;
			c = GetCircle(cir, blo, i, m, curradius);
			//printf("find circle\n");
			cir[i] = c;
			c = optimize(cir, i + 1);
			cir[i] = c;
			start_time = clock();
			while (Getsum(cir, i + 1) > MINEX / n)
			{
				//printf("curradius = %f\n", curradius);
				for (int k = 0; k < 10 && Getsum(cir, i + 1) > MINEX / n; k++)
				{
					c = GetCircle(cir, blo, i, m, curradius);
					cir[i] = c;
					c = optimize(cir, i + 1);
					cir[i] = c;
				}
				if (Getsum(cir, i + 1) > MINEX / n)
					curradius *= 0.90;
				end_time = clock();
				run_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
				if (run_time > TIMEEXCESS)
				{
					printf("Time Excess in %dth circle!\n", i + 1);
					break;
				}
			}
			cir[i] = c;
			flag[i] = 1;
			ans += Getsum(cir, i + 1);
			printf("find one\n");
			/*if (flag[i] == 1 && i > 0)
			curradius *= Beta;*/
		}
		if (ans < MINEX)
			return ans;
		else
		{
			t *= delta;
			curradius = 1;
		}
	}
	return -1;
}


int _tmain(int argc, _TCHAR* argv[])
{
	clock_t start_time, end_time;
	int numCir;
	int numBlo;
	double ans;
	Circle *cir = NULL;
	Block *blo = NULL;
	while (1)
	{
		cout << "Please enter the number of circles:" << endl;
		cin >> numCir;
		cout << "Please enter the number of blocks:" << endl;
		cin >> numBlo;
		if (numCir > 0)
		{
			cir = initCircles(numCir);
			blo = initBlocks(numBlo);
			
			start_time = clock();
			ans = Search(cir, blo, numCir, numBlo);
			end_time = clock();

			if (ans == -1)
				cout << "There is no answer, please try again" << endl;
			else
			{
				for (int i = 0; i < numCir; i++)
					printf("The radius and coordinate are: %f, (%f, %f)\n", cir[i].radius, cir[i].x, cir[i].y);
				printf("e: %f\n", ans);
			}
		}
		else
			break;
		printf("The running time dealing with %d circle(s) is %f seceonds\n", numCir, (double)(end_time - start_time) / CLOCKS_PER_SEC);
	}

	return 0;
}

