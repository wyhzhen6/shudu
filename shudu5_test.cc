#include <vector>
#include <iostream>
#include "gtest/gtest.h"
#include"./shudu5.h"

using namespace std;





TEST(solveSudoku,Mytest)
{
    EXPECT_EQ(true,test_solveSudoku());

}

TEST(parse_c,Mytest)
{
    char* b[20]={"shudu.exe","-c","100"};
    map<char, string> params;
    params['c']="100";
    EXPECT_EQ(params,parse(3,b));

}

TEST(parse_n,Mytest)
{
    char* b[20]={"shudu.exe","-n","1000"};
    map<char, string> params;
    params['n']="1000";
    EXPECT_EQ(params,parse(3,b));

}

TEST(parse_s,Mytest)
{
    char* b[20]={"shudu.exe","-s","game.txt"};
    map<char, string> params;
    params['s']="game.txt";
    EXPECT_EQ(params,parse(3,b));

}

TEST(parse_m,Mytest)
{
    char* b[20]={"shudu.exe","-n","10","-m","1"};
    map<char, string> params;
    params['n']="10";
    params['m']="1";
    EXPECT_EQ(params,parse(5,b));

}


TEST(parse_u,Mytest)
{
    char* b[20]={"shudu.exe","-n","10","-u"};
    map<char, string> params;
    params['n']="10";
    params['u']="u";
    EXPECT_EQ(params,parse(4,b));

}

TEST(generateGame_c,Mytest)
{

    char* b[20]={"shudu.exe","-c","100"};
    map<char, string> params=parse(3,b);
    SudokuMaker maker;
    ofstream outfile;
    vector<int> range;
    range.push_back(0);
    bool only_solution = false;
    map<char, string>::iterator current = params.begin();
    string value = current->second;
    EXPECT_EQ(true,generateGame(atoi(value.c_str()), 0, range, outfile, maker, only_solution));
    range.clear();
}

TEST(print_board,Mytest)
{

SudokuMaker maker;
Board board=maker.generateBoard(30,false);
EXPECT_EQ(true,maker.printBoard(board));

}



TEST(write_file,Mytest)
{

SudokuMaker maker;
Board board=maker.generateBoard(30,false);
ofstream outfile;
outfile.open("answer.txt", ios::out | ios::trunc);
vector<Board> result={board};
EXPECT_EQ(true,writeFile(result,outfile,0));

}

TEST(reverse_bit,Mytest)
{

SudokuMaker maker;

EXPECT_EQ(true,maker.reverseBit(1,2,3));

}
