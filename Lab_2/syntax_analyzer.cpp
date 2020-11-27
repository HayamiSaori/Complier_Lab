#include <iostream>
#include <map>
#include <string>
#include <ctype.h>
#include <fstream>
#include <vector>
#include <sstream>
#include "code_table.h"
const int ACTION_ROW = 23;
const int ACTION_COL = 10;
const int GOTO_ROW = 23;
const int GOTO_COL = 9;
using namespace std;
map<int,int> ACTION_DICT;
map<string,int> GOTO_DICT;
struct token
{
    int category;
    string value;
};
struct singal_action
{
    int mode;// -1为shift,0为空,1为reduce,2为accept
    int next_state;
    int beta;
    string A;
    string fomula;
};
vector<struct token> ParseTokenFile(string path)
{
    ifstream token_file;
    struct token temp;
    vector<struct token> token_list;
    token_file.open(path,ios::in);
    while(!token_file.eof())
    {
        token_file >> temp.category >> temp.value;
        // cout << temp.category << ',' << temp.value << endl;
        token_list.push_back(temp);
    }
    token_file.close();
    return token_list;
}
string CutString(string line,int& left,int& right)
{
    string result="Empty";
    while(line[right]!=',' && line[right]!='\n')
    {
        right++;
    }
    if(left != right)
    {
        result = line.substr(left,right-left);
        if(result == "")result = "Empty";
    }
    right++;
    left = right;
    return result;
}
void ParseActionFile(string path,/*&vector<vector<struct singal_action>> act*/struct singal_action act[ACTION_ROW][ACTION_COL])
{
    ifstream action_file;
    string temp[ACTION_ROW][ACTION_COL];
    string buff,temp_str,num_str;
    int left,right,i,j,k;
    action_file.open(path,ios::in);
    i=0;j=0;
    while(getline(action_file,buff))
    {
        left = 0;right = 0;
        while(right <= buff.size())
        {
            temp_str = CutString(buff,left,right);
            if(temp_str == "Empty" || temp_str == "")
            {
                act[i][j].mode = 0;
            }
            else if(temp_str[0] == 's')// shift
            {
                act[i][j].mode = -1;
                num_str = temp_str.substr(6);
                stringstream ss;
                ss << num_str;
                ss >> act[i][j].next_state;
                //cout << temp_str << ":next=" << act[i][j].next_state << endl;
            }
            else if(temp_str[0] == 'r')// reduce
            {
                k=0;
                act[i][j].mode = 1;
                act[i][j].beta = 0;
                act[i][j].fomula = temp_str.substr(7);
                while(act[i][j].fomula[k] != ' ')
                {
                    k++;
                }
                act[i][j].A = temp_str.substr(7,k);
                while(act[i][j].fomula[k] != '>')
                {
                    k++;
                }
                for(;k<act[i][j].fomula.size();k++)
                {
                    if(act[i][j].fomula[k] == ' ')act[i][j].beta+=1;
                }
                //cout << act[i][j].fomula << ":beta=" << act[i][j].beta << endl;
            }
            else
            {
                act[i][j].mode = 2;
            }
            j++;
            //cout << temp_str << endl;
        }
        //cout<<"line:"<<i<<",col:"<<j<<endl;
        i++;j=0;
    }
}
void InitGotoList(int goto_list[GOTO_ROW][GOTO_COL])
{
    int i,j;
    for(i=0;i<GOTO_ROW;i++)
    {
        for(j=0;j<GOTO_COL;j++)
        {
            goto_list[i][j] = -1;
        }
    }
    goto_list[0][3] = 1;goto_list[0][4] = 2;goto_list[0][5] = 3;goto_list[0][8] = 4;
    goto_list[9][0] = 10;goto_list[9][1] = 11;goto_list[9][2] = 12;
    goto_list[11][6] = 15;
    goto_list[12][7] = 18;
    goto_list[15][0] = 21;goto_list[15][1] = 11;goto_list[15][2] = 12;
    goto_list[18][1] = 22;goto_list[18][2] = 12;
}
void InitDict()
{
    ACTION_DICT[TOKEN] = 0;ACTION_DICT[SIGNEDINT] = 1;ACTION_DICT[SIGNEDFLOAT] = 1;
    ACTION_DICT[ADD] = 2;ACTION_DICT[SUB] = 3;ACTION_DICT[MUL] = 4;
    ACTION_DICT[DIV] = 5;ACTION_DICT[INT] = 6;ACTION_DICT[DOUBLE] = 7;
    ACTION_DICT[ASSIGN] = 8;ACTION_DICT[SEMICOLON] = 9;
    GOTO_DICT["EXP"] = 0;GOTO_DICT["ITEM"] = 1;GOTO_DICT["FACTOR"] = 2;
    GOTO_DICT["DEFINE"] = 3;GOTO_DICT["ASSIGN"] = 4;GOTO_DICT["KEYWORD"] = 5;
    GOTO_DICT["OP_LOW"] = 6;GOTO_DICT["OP_HIGH"] = 7;GOTO_DICT["S"] = 8;
}
int main()
{
    string TOKEN_FILE_PATH = "token.txt";
    string FOMULA_FILE_PATH = "fomula.txt";
    string ACTION_FILE_PATH = "ACTION_2.txt";
    int goto_list[GOTO_ROW][GOTO_COL];
    int i,j,pos;
    vector<struct token> token_list;
    vector<struct token> temp_list;
    string ACTION[ACTION_ROW][ACTION_COL];
    struct singal_action act[ACTION_ROW][ACTION_COL];

    token_list = ParseTokenFile(TOKEN_FILE_PATH);
    ParseActionFile(ACTION_FILE_PATH,act);
    InitGotoList(goto_list);
    InitDict();
    pos = 0;
    while(true)
    {
        i = pos;
        while(token_list[i].category != SEMICOLON)
        {
            temp_list.push_back(token_list[i]);
            i++;
        }
        temp_list.push_back(token_list[i]);
        for(j=0;j<temp_list.size();j++)
        {
            cout << temp_list[j].category  << temp_list[j].value << endl;
        }
        cout << "----------" << endl;
        temp_list.clear();
        if(i==token_list.size() - 1)
        {
            break;
        }
        else
        {
            i++;
        }
        pos = i;
    }
}
