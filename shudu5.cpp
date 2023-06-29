#include"./shudu5.h"

int main(int argc, char* argv[])
{
    srand((unsigned)time(NULL));
    SudokuMaker maker;

    map<char, string> params = parse(argc, argv);
    map<char, string>::iterator current, tmp;

    int opt = 0;

    vector<int> range;
    int gameNumber;
    int gameLevel = 0;
    bool only_solution = false;

    vector<Board> boards;
    ofstream outfile;

    current = params.begin();
    while (current != params.end()) {
        char param = current->first;
        string value = current->second;

        switch(param) {
        case 'c':
            outfile.open("game.txt", ios::out | ios::trunc);
            range.push_back(0);
            generateGame(atoi(value.c_str()), 0, range, outfile, maker, only_solution);
            range.clear();
            break;

        case 's':
            outfile.open("answer.txt", ios::out | ios::trunc);
            boards = readFile(value);
            for (int i = 0; i < boards.size(); i++) {
                vector<Board> result = maker.solveSudoku(boards[i]);
                writeFile(result, outfile,i);
            }
            outfile.close();
            break;

        case 'n':
        case 'm':
        case 'r':
        case 'u': {
            tmp = params.find('n');
            if (tmp == params.end()) {
                printf("缺少参数 n \n");
                exit(0);
            }

            gameNumber = atoi(tmp->second.c_str());

            tmp = params.find('u');
            if (tmp != params.end()) {
                only_solution = true;
            }
            tmp = params.find('m');
            if (tmp != params.end()) {
                gameLevel = atoi(tmp->second.c_str());
            }

            tmp = params.find('r');
            if (tmp != params.end()) {
                char* p;
                char* pc = new char[100];
                strcpy(pc, tmp->second.c_str());
                p = strtok(pc, "~");
                while (p) {
                    range.push_back(atoi(p));
                    p = strtok(NULL, "~");
                }
            }
            else {
                // 根据不同级别采取挖空数量不同
                if (gameLevel == 1) {
                    range.push_back(20);
                    range.push_back(30);
                }
                else if (gameLevel == 2) {
                    range.push_back(30);
                    range.push_back(40);
                }
                else if (gameLevel == 3) {
                    range.push_back(40);
                    range.push_back(55);
                }
                else {
                    range.push_back(20);
                    range.push_back(55);
                }
            }

            outfile.open("game.txt", ios::out | ios::trunc);
            generateGame(gameNumber, gameLevel, range, outfile, maker,only_solution);
            range.clear();
            break;
        }
        default:
            printf("未知参数: %c\n", param);
            break;
        }

        ++current;
    }

    return 0;
}