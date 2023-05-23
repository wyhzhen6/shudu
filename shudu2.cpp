#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include<Windows.h>
using namespace std;



class sudoku
{
public:
    bool active = false;
    sudoku(vector<vector<char>>& board)
    {
        // printBoard(board);
        active = dfs(board, 0);
    }


    void printBoard(vector<vector<char>>& board) {
        //输出9*9的数独矩阵
        cout << endl << "完成后的9*9数独：" << endl;
        int i, j;
        for (i = 0; i < board.size(); i++) {
            for (j = 0; j < board.size(); j++) {
                cout << board[i][j] << " ";
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

    bool isValid(int row, int col, int num, vector<vector<char>>& board)
    {
        //检查行有没有重复的，如果有返回faulse
        for (int i = 0; i < board.size(); i++)
        {
            if (num == board[row][i] - '0')
            {
                return false;
            }
        }
        //检查列有没有重复的，如果有返回faulse
        for (int i = 0; i < board.size(); i++)
        {
            if (num == board[i][col] - '0')return false;
        }
        //检查数字所在的方块有没有重复的，如果有返回faulse
        int startrow = row / 3 * 3;
        int startcol = col / 3 * 3;
        int endrow = startrow + 3;
        int endcol = startcol + 3;
        for (int i = startrow; i < endrow; i++)
        {
            for (int j = startcol; j < endcol; j++)
            {
                if (num == board[i][j] - '0')return false;
            }
        }
        return true;
    }

    void swapCol(int m, int n, vector<vector<char>>& board)
    {
        vector<char>temp(board.size(), ' ');
        for (int i = 0; i < board.size(); i++)
        {
            temp[i] = board[i][m];
            board[i][m] = board[i][n];
            board[i][n] = temp[i];
        }
    }


    void selectBlank(int nums, vector<vector<char>>& board)
    {
        srand(time(NULL));
        while (nums)
        {
            int row = rand() % 9;
            int col = rand() % 9;
            if (board[row][col] != '0')
            {
                board[row][col] = '0';
                nums--;
            }
        }
    }

    void create(int blank, vector<vector<char>>& board)
    {
        //小九宫格中的行和列交换，有以下９种交换方式
        int choice[9][2] = { {0,1},{0,2},{1,2},{3,4},{3,5},{4,5},{6,7},{6,8},{7,8} };
        srand(time(NULL));//设置动态种子
        for (int j = 0; j < 3; j++)//Ｊ代表交换次数，也可以不用这个循环，就交换一次
        {
            int i = rand() % 9;//取0－8之间的随机值
            board[choice[i][0]].swap(board[choice[i][1]]);//随机交换两行
            swapCol(choice[i][0], choice[i][1], board);//随机交换两列
        }
        selectBlank(blank, board);//根据难度可以选择不同数量的空格

    }


    bool dfs(vector<vector<char>>& board, int start)//从０开始依次遍历81个格子，计算此数独
    {
        if (start == 81)//start=81，说明已经成功解出数独
        {
            return true;
        }
        else
        {
            bool ok = false;
            int row = start / 9;//根据此时方格的序号，计算出此方格的行和列
            int col = start % 9;
            if (board[row][col] == '0')
            {
                for (int i = 1; i <= 9; i++)
                {
                    if (isValid(row, col, i, board))//从１－９依次放入空格，并判断是否合法
                    {
                        board[row][col] = i + '0';//如果有数字合法，就写入该数字的字符
                        ok = dfs(board, start + 1);//判断此方格的下一个方格是否成功写入
                        if (!ok)//如果它的下一个方格是不合法的，说明它现在填入的数，不是正确的解，需回溯
                        {
                            board[row][col] = '0';//回溯
                        }
                    }
                }
            }
            else
            {
                ok = dfs(board, start + 1);
            }
            return ok;
        }


    }
};



int main()
{
    /*vector<vector<char>>board = { vector<char>{'5','3','0','0','7','0','0','0','0'},
                                  vector<char>{'6','0','0','1','9','5','0','0','0'},
                                  vector<char>{'0','9','8','0','0','0','0','6','0'},
                                  vector<char>{'8','0','0','0','6','0','0','0','3'},
                                  vector<char>{'4','0','0','8','0','3','0','0','1'},
                                  vector<char>{'7','0','0','0','2','0','0','0','6'},
                                  vector<char>{'0','6','0','0','0','0','2','8','0'},
                                  vector<char>{'0','0','0','4','1','9','0','0','5'},
                                  vector<char>{'0','0','0','0','8','0','0','7','9'}, };*/

    vector<vector<char>>blank(9, vector<char>(9, '0'));//创建一个9*9的空二维数组

    sudoku s(blank);
    s.create(30, blank);//创建30个空格的数独
    if (s.active)
    {
        cout << "创建的数独为：" << endl;
        s.printBoard(blank);
    }
    else
        cout << "数独表不合法，没有解！" << endl;
    s.dfs(blank, 0);//解数独函数
    if (s.active)
    {
        cout << "此数独的解是：" << endl;
        s.printBoard(blank);
    }
    else
        cout << "解题失败！" << endl;

    return 0;
}
