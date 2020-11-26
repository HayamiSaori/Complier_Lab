#include <iostream>
#include <map>
#include <string>
#include <ctype.h>
#include <fstream>
#include <vector>
#include "code_table.h"

using namespace std;
struct token
{
    int category;
    string value;
};
vector<struct token> ParseToken(string path)
{
    ifstream token_file;
    struct token temp;
    vector<struct token> token_list;
    token_file.open(path,ios::in);
    while(!token_file.eof())
    {
        token_file >> temp.category >> temp.value;
        cout << temp.category << ',' << temp.value << endl;
        token_list.push_back(temp);
    }
    token_file.close();
    return token_list;
}
int main()
{

    string TOKEN_FILE_PATH = "token.txt";
    string FOMULA_FILE_PATH = "fomula.txt";
    vector<struct token> token_list;
    token_list = ParseToken(TOKEN_FILE_PATH);
    int i;cout << "--------------" << endl;
    for(i=0;i<token_list.size();i++)
    {
        cout << token_list[i].category << ',' << token_list[i].value << endl;
    }
}
