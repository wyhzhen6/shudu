#include<iostream>
using namespace std;

bool flag = false;



int num[9][9] = {//创建9*9的数独矩阵(注意：int型！！！)
{0, 0, 3, 0, 5, 0, 0, 0, 9},
{ 0, 0, 0, 1, 0, 0, 0, 2, 5 },
{ 0, 8, 0, 0, 3, 7, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 8, 0, 9, 7 },
{ 2, 0, 0, 0, 6, 0, 0, 0, 4 },
{ 9, 4, 0, 0, 0, 0, 8, 0, 1 },
{ 0, 0, 0, 6, 9, 0, 4, 0, 0 },
{ 8, 0, 0, 0, 0, 5, 0, 0, 0 },
{ 6, 0, 0, 0, 1, 0, 9, 0, 0 } };


bool Check(int n, int key) {
	//判断key填入n时是否满足条件，key为1~9的其中一个，n为0~80的其中一个
	int i, j;

	for (j = 0; j < 9; j++) {
		//判断n所在行是否合法
		i = n / 9;
		if (num[i][j] == key) {
			return false;
		}
	}

	for (i = 0; i < 9; i++) {
		//判断n所在列是否合法
		j = n % 9;
		if (num[i][j] == key) {
			return false;
		}
	}

	int x = ((n / 9) / 3)*3;//小九宫格左上角横坐标
	cout << n<<"n"<<x<<"x"<<endl;
	int y = ((n % 9) / 3 )* 3;//小九宫格左上角纵坐标
	cout << n << "n" << y << "y" << endl;
	for (i = x; i < x + 3; i++) {
		for (j = y; j < y + 3; j++) {
			if (num[i][j] == key) {
				return false;
			}
		}
	}

	return true;//全部合法
}

int DFS(int n) {
	//深度搜索构造数独
	if (n > 80) {
		//所有的都符合条件
		flag = true;
		return 0;
	}

	if (num[n / 9][n % 9] != 0) {
		//如果不为0，跳过
		DFS(n + 1);
	}
	else {
		//反之，对该位置进行填数
		for (int i = 1; i <= 9; i++) {
			//填入的数字为1~9的其中一个
			if (Check(n, i) == true) {
				num[n / 9][n % 9] = i;//赋值
				DFS(n + 1);//继续搜索
				if (flag == true)//构造成功退出
					return 0;
				num[n / 9][n % 9] = 0;//构造不成功还原当前位
			}
		}
	}
}

void Output() {
	//输出9*9的数独矩阵
	cout << endl << "完成后的9*9数独：" << endl;
	int i, j;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			cout << num[i][j] << " ";
			if (j % 3 == 2) {
				cout << "  ";
			}
		}
		cout << endl;
		if (i % 3 == 2) {
			cout << endl;
		}
	}
}


int main() {
	DFS(0);//共9*9=81个元素，下标为0~81
	Output();//相当于求解

	system("pause");

}
