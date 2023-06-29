#include <iostream>
#include <vector>
#include <utility>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <fstream>
#include<cstring>
#include <map>
using namespace std;

//延续之前的思想使用vector嵌套，来创建一个9*9的空二维数组
typedef vector<vector<char> > Board;

//默认数独大小为9
const int shuduScale = 9;

class SudokuMaker
{
private:
    // 使用位运算来表示某个数是否出现过
    int rowUsed[shuduScale];
    int columnUsed[shuduScale];
    int blockUsed[shuduScale];

public:
    vector<Board> result;//result用于存储解决数独问题后得到的结果
    vector<pair<int, int> > spaces;//spaces用于记录数独中空格的位置

public:
    SudokuMaker()
    {
        initState();
    }

    void initState()
    {
        memset(rowUsed, 0, sizeof(rowUsed));
        memset(columnUsed, 0, sizeof(columnUsed));
        memset(blockUsed, 0, sizeof(blockUsed));
        spaces.clear();
        result.clear();
    }

    void addResult(Board& board)
    {
        vector<vector<char> > obj(board);
        result.push_back(obj);
    }

    bool reverseBit(int i, int j, int digit)//使用位运算来表示某个数是否出现过
    {
        int mask = 1 << digit;//首先，使用 1 << digit 创建一个只有第 digit 位为 1 的掩码
        //之后，将这个掩码与 rowUsed[i]、columnUsed[j] 和 blockUsed[(i / 3) * 3 + j / 3] 进行按位异或操作，以将指定位置的位翻转
        rowUsed[i] ^= mask;
        columnUsed[j] ^= mask;
        blockUsed[(i / 3) * 3 + j / 3] ^= mask;
        return true;
    }

    vector<Board> solveSudoku(Board board)
    {
        initState();
        for (int i = 0; i < shuduScale; i++)
        {
            for (int j = 0; j < shuduScale; j++)
            {
                if (board[i][j] == '$')
                {
                    spaces.push_back(pair<int, int>(i, j));
                }
                else
                {
                    int digit = board[i][j] - '1';
                    reverseBit(i, j, digit);
                }
            }
        }
        DFS(board, 0);//利用深度优先算法进行求解
        return result;
    }

    void DFS(Board& board, int pos)
    {
        if (pos == spaces.size())
        {
            addResult(board);
            return;
        }
        int i = spaces[pos].first;
        int j = spaces[pos].second;
        int mask = ~(rowUsed[i] | columnUsed[j] | blockUsed[(i / 3) * 3 + j / 3]) & 0x1ff;
        int digit = 0;
        while (mask)
        {
            if (mask & 1)
            {
                reverseBit(i, j, digit);
                board[i][j] = '1' + digit;
                DFS(board, pos + 1);
                reverseBit(i, j, digit);
            }
            mask = mask >> 1;
            digit++;
        }
    }

    //输出数独棋盘的方法。将棋盘按行打印到控制台上
    bool printBoard(Board& board)
    {
        for (int i = 0; i < board.size(); i++)
        {
            for (int j = 0; j < board[i].size(); j++)
            {
                cout << board[i][j] << " ";
            }
            cout << "\n";
        }
        return true;
    }


    //遍历result向量中的每个解决方案，调用printBoard方法进行输出。
    void getResult()
    {
        for (size_t i = 0; i < result.size(); i++)
        {
            Board board = result[i];
            printBoard(board);
        }
    }

    //检查给定的数独棋盘board是否符合数独规则。对于每个已填数字，检查行、列和3x3块中是否已经出现相同的数字。如果存在重复数字，则返回false，否则返回true
    bool checkBoard(const Board& board)
    {
        initState();
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                if (board[i][j] != '$')
                {
                    int digit = board[i][j] - '1';
                    if ((rowUsed[i] | columnUsed[j] | blockUsed[(i / 3) * 3 + j / 3]) & (1 << digit))
                    {
                        return false;
                    }
                    reverseBit(i, j, digit);
                }
            }
        }
        return true;
    }


    //通过随机填充数独棋盘并使用solveSudoku方法进行求解，直到达到指定的空格数量为止。
    Board generateBoard(int digCount,bool isonly)
    {
        vector<vector<char> > board(shuduScale, vector<char>(shuduScale, '$'));
        vector<int> row = generateRandomSequence();
        for (int i = 0; i < 3; i++)
        {
            board[3][i + 3] = row[i] + '1';
            board[4][i + 3] = row[i + 3] + '1';
            board[5][i + 3] = row[i + 6] + '1';
        }
        copySquare(board, 3, 3, true);
        copySquare(board, 3, 3, false);
        copySquare(board, 3, 0, false);
        copySquare(board, 3, 6, false);

        while (digCount)
        {
            int x = rand() % 9;
            int y = rand() % 9;
            if (board[x][y] == '$')
                continue;
            char tmp = board[x][y];
            board[x][y] = '$';

            solveSudoku(board);
            if (isonly) {
                if (result.size() == 1)
                {
                    digCount--;
                }
                else
                {
                    board[x][y] = tmp;
                }
            }
            else {
                if (result.size() != 0)
                {
                    digCount--;
                }
                else
                {
                    board[x][y] = tmp;
                }
            }
        }
        if (!checkBoard(board))
        {
            cout << "wrong board" << endl;
        }

        return board;
    }

    //生成一个随机的包含1到9的乱序数组。
    //它使用一个变量digit来记录已经出现的数字，通过位运算来判断某个数字是否已经存在。循环进行随机数生成，直到结果数组result的大小达到9时停止，并且确保生成的数字不重复。
    vector<int> generateRandomSequence()
    {
        vector<int> resultSequence;
        int digit = 0;
        while (resultSequence.size() != 9)
        {
            int num = rand() % 9;
            if ((1 << num) & digit)
            {
                continue;
            }
            else
            {
                resultSequence.push_back(num);
                digit ^= (1 << num);
            }
        }
        return resultSequence;
    }

    //copySquare()函数用于复制一个3x3方格到另一个位置，可以选择按行复制或按列复制。
    //它根据一个随机数生成器的结果来确定复制的顺序。
    //根据生成的随机数rand_tmp，设置两个不同的顺序数组order_first和order_second。然后，使用循环将相应位置的数字复制到目标位置
    void copySquare(Board& board, int src_x, int src_y, bool isRow)
    {
        int rand_tmp = rand() % 2 + 1;
        int order_first[3] = { 1, 2, 0 };
        int order_second[3] = { 2, 0, 1 };
        if (rand_tmp == 2)
        {
            order_first[0] = 2;
            order_first[1] = 0;
            order_first[2] = 1;
            order_second[0] = 1;
            order_second[1] = 2;
            order_second[2] = 0;
        }
        for (int i = 0; i < 3; i++)
        {
            if (isRow)
            {
                board[src_x][i] = board[src_x + order_first[0]][src_y + i];
                board[src_x + 1][i] = board[src_x + order_first[1]][src_y + i];
                board[src_x + 2][i] = board[src_x + order_first[2]][src_y + i];
                board[src_x][i + 6] = board[src_x + order_second[0]][src_y + i];
                board[src_x + 1][i + 6] = board[src_x + order_second[1]][src_y + i];
                board[src_x + 2][i + 6] = board[src_x + order_second[2]][src_y + i];
            }
            else
            {
                board[i][src_y] = board[src_x + i][src_y + order_first[0]];
                board[i][src_y + 1] = board[src_x + i][src_y + order_first[1]];
                board[i][src_y + 2] = board[src_x + i][src_y + order_first[2]];
                board[i + 6][src_y] = board[src_x + i][src_y + order_second[0]];
                board[i + 6][src_y + 1] = board[src_x + i][src_y + order_second[1]];
                board[i + 6][src_y + 2] = board[src_x + i][src_y + order_second[2]];
            }
        }
    }
};



vector<Board> readFile(string filePath)
{
    ifstream infile;
    vector<Board> boards;
    infile.open(filePath);
    char data[100];
    Board tmp;
    vector<char> row;
    while (!infile.eof())
    {
        infile.getline(data, 100);
        if (data[0] == '-')
        {
            boards.push_back(Board(tmp));
            tmp.clear();
            continue;
        }
        for (int i = 0; i < strlen(data); i++)
        {
            if (('1' <= data[i] && data[i] <= '9') || data[i] == '$')
            {
                row.push_back(data[i]);
            }
        }
        tmp.push_back(vector<char>(row));
        row.clear();
    }
    infile.close();
    return boards;
}

bool writeFile(vector<Board> boards, ofstream& f,int num)
{
    for (int k = 0; k < boards.size(); k++)
    {
        for (int i = 0; i < boards[k].size(); i++)
        {
            for (int j = 0; j < boards[k][i].size(); j++)
            {
                f << boards[k][i][j] << " ";
            }
            f << "\n";
        }
        f << "------- " << num << " -------" << endl;
    }
    return true;
}



// 解析输入参数
map<char, string> parse(int argc, char* argv[])
{
    map<char, string> params;
    int compeleteBoardCount = 0;
    int gameNumber = 0;
    int gameLevel = 0;
    vector<int> range;
    string inputFile;

    for (int i = 1; i < argc; i += 2) {
        bool only_solution = false;
        char opt = argv[i][1];
        string value;
        if(opt != 'u')
        value = argv[i + 1];
        
        switch (opt) {
        case 'c':
            compeleteBoardCount = stoi(value);
            if (compeleteBoardCount < 1 || compeleteBoardCount > 1000000) {
                cout << "生成数独终盘数量范围在1～1000000之间" << endl;
                exit(0);
            }
            params[opt] = value;
            break;
        case 's': {
            inputFile = value;
            ifstream file(inputFile);
            if (!file.good()) {
                cout << "file does not exist" << endl;
                exit(0);
            }
            params[opt] = value;
            break;
        }
        case 'n':
            gameNumber = stoi(value);
            if (gameNumber < 1 || gameNumber > 10000) {
                cout << "生成数独游戏数量范围在1～10000之间" << endl;
                exit(0);
            }
            params[opt] = value;
            break;
        case 'm':
            gameLevel = stoi(value);
            if (gameLevel < 1 || gameLevel > 3) {
                cout << "生成游戏难度的范围在1～3之间" << endl;
                exit(0);
            }
            params[opt] = value;
            break;
        case 'r': {
            size_t pos = 0;
            while ((pos = value.find("~")) != string::npos) {
                string num = value.substr(0, pos);
                range.push_back(stoi(num));
                value.erase(0, pos + 1);
            }
            range.push_back(stoi(value));

            if (range.size() != 2) {
                cout << "请输入一个范围参数" << endl;
                exit(0);
            }
            if ((range[0] >= range[1]) || range[0] < 20 || range[1] > 55) {
                cout << "请输入合法范围20～55" << endl;
                exit(0);
            }
            params[opt] = value;
            break;
        }
        case 'u':
            only_solution = true;
            params[opt] = "u";
            break;
        default:
            cout << "请输入合法参数" << endl;
            exit(0);
        }
    }
    return params;
}



bool generateGame(int gameNumber, int gameLevel, vector<int> digCount, ofstream& outfile, SudokuMaker& maker, bool isonly)
{
    for (int i = 0; i < gameNumber; i++)
    {
        int cnt = 0;
        if (digCount.size() == 1)
        {
            cnt = digCount[0];
        }
        else
        {
            cnt = rand() % (digCount[1] - digCount[0] + 1) + digCount[0];
        }
        Board b = maker.generateBoard(cnt,isonly);
        vector<Board> bs;
        bs.push_back(b);
        writeFile(bs, outfile,i);
    }
    outfile.close();
    return true;
}

bool test_solveSudoku()
{
    readFile("./game.txt");
    SudokuMaker maker;
    vector<Board> boards;
    for (int i = 0; i < boards.size(); i++)
     {
        vector<Board> result = maker.solveSudoku(boards[i]);
        //对于数独终盘进行检查
        if(!maker.checkBoard(result[0]))
            return false;
                
    }
    return true;

}