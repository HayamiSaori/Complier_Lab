#include <iostream>
#include <map>
#include <stack>
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
    /*if(right >= line.size())
    {
        return result;
    }*/
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
        while(right <= buff.size() && buff.size() > 0)
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
                cout << temp_str << ":next=" << act[i][j].next_state << endl;
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
                cout << act[i][j].fomula << ":beta=" << act[i][j].beta << endl;
            }
            else
            {
                act[i][j].mode = 2;
            }
            j++;
            //cout << temp_str << endl;
        }
        cout<<"line:"<<i<<",col:"<<j<<endl;
        //getline(action_file,buff);cout<<buff<<"||size:"<<buff.size()<<endl;
        i++;j=0;
        if(i==22)break;
    }
    for(i=0;i<ACTION_COL;i++)
    {
        act[22][i].mode=0;
    }
    act[22][2].mode=1;act[22][2].fomula="ITEM -> FACTOR OP_HIGH ITEM";act[22][2].beta=3;act[22][2].A="ITEM";
    act[22][3].mode=1;act[22][3].fomula="ITEM -> FACTOR OP_HIGH ITEM";act[22][3].beta=3;act[22][3].A="ITEM";
    act[22][9].mode=1;act[22][9].fomula="ITEM -> FACTOR OP_HIGH ITEM";act[22][9].beta=3;act[22][9].A="ITEM";
    cout<<"now return"<<endl;
    action_file.close();
    return;
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
void UpdateToken(string x)
{
    ifstream list_old;
    ofstream list_new;
    int i,entry,id;
    string token_name;
    stringstream ss;
    ss.str(x);
    ss >> id;
    list_old.open("character_list.txt",ios::in);
    list_new.open("character_list_new.txt",ios::app);
    for(i=0;i<id;i++)
    {
        list_old >> entry >> token_name;
    }
    list_new << entry << "\t" << token_name << "\t" << "number\t" << "0x" << hex << id*4 << endl;
    list_old.close();
    list_new.close();
}
void SyntaxAnalyze(string reduce_path,vector<struct token> tokens,struct singal_action act[ACTION_ROW][ACTION_COL],int goto_list[GOTO_ROW][GOTO_COL])
{
    int len,i,j,cur_state,next,a;
    int tmp,top,cnt=0;
    int cur_factor=1,cur_item=1,cur_exp=1,old_factor=0,old_item=0;
    string cur_id = "0";
    string left_id = "0";
    string cur_num = "0";
    stack<int> character_stack;
    stack<int> state_stack;
    stack<int> total_stack;
    string temp_fomula,temp_A,cur_op_low,cur_op_high;
    len = tokens.size();
    i = 0;
    total_stack.push(ACTION_DICT[SEMICOLON]);
    total_stack.push(0);
    cur_state = 0;
    a = tokens[0].category;
    ofstream reduce_file;
    ofstream midcode_file;
    reduce_file.open(reduce_path,ios::app);
    midcode_file.open("midcode.txt",ios::app);
    while(true)
    {
        a = tokens[i].category;

        if(a == TOKEN && left_id == "0")left_id=tokens[i].value;// 保存最右变量的入口
        if(a == TOKEN)cur_id=tokens[i].value;// 保存当前变量的入口
        if(a == SIGNEDINT || a == SIGNEDFLOAT)cur_num=tokens[i].value;//保存当前常量
        if(act[cur_state][ACTION_DICT[a]].mode == -1) // shift
        {
            // total_stack.push(ACTION_DICT[a]);
            // cout << "push character:" << ACTION_DICT[a] << " and state:" << act[cur_state][ACTION_DICT[a]].next_state << endl;
            total_stack.push(a);
            total_stack.push(act[cur_state][ACTION_DICT[a]].next_state);
            i++;
            cur_state = act[cur_state][ACTION_DICT[a]].next_state;
            // cout << "now shift to " << cur_state << endl;
        }
        else if (act[cur_state][ACTION_DICT[a]].mode == 1) // reduce
        {
            for(j=0;j<2*act[cur_state][ACTION_DICT[a]].beta;j++)
            {
                total_stack.pop();
            }
            top = total_stack.top();
            tmp = GOTO_DICT[act[cur_state][ACTION_DICT[a]].A];
            total_stack.push(GOTO_DICT[act[cur_state][ACTION_DICT[a]].A]);
            total_stack.push(goto_list[top][tmp]);
            //cout << "now reduce" << endl;
            cout << act[cur_state][ACTION_DICT[a]].fomula << endl;
            reduce_file << act[cur_state][ACTION_DICT[a]].fomula << endl;
            //------中间代码生成------
            temp_fomula = act[cur_state][ACTION_DICT[a]].fomula;
            if(temp_fomula == "DEFINE -> KEYWORD TOKEN")
            {
                midcode_file << "def\t" << "none\t" << "none\t" << cur_id << endl;
                UpdateToken(cur_id);
            }
            if(temp_fomula == "ASSIGN -> TOKEN = EXP")
            {
                midcode_file << "=\t" << "e" << cur_exp << "\t" << "none\t" << left_id << endl;
            }
            if(temp_fomula == "EXP -> ITEM")
            {
                //cur_exp++;
                midcode_file << "=\t" << "i" << cur_item << "\t" << "none\t" << "e" << cur_exp << endl;
            }
            if(temp_fomula == "EXP -> ITEM OP_LOW EXP")
            {
                midcode_file << cur_op_low << "\t" << "i" << cur_item - 1 << "\t" <<"e" << cur_exp << "\t" << "e" << cur_exp + 1 << endl;
                cur_exp++;
            }
            if(temp_fomula == "ITEM -> FACTOR")
            {
                midcode_file << "=\t" << "f" << cur_factor - 1 << "\t" << "none\t" << "i" << cur_item << endl;
                //cur_item++;
            }
            if(temp_fomula == "ITEM -> FACTOR OP_HIGH ITEM")
            {
                midcode_file << cur_op_high << "\t" << "f" << cur_factor - 2 << "\t" << "i" << cur_item << "\t" << "i" << cur_item + 1 << endl;
                cur_item++;
            }
            if(temp_fomula == "OP_LOW -> +")
            {
                cur_op_low = "+";
            }
            if(temp_fomula == "OP_LOW -> -")
            {
                cur_op_low = "-";
            }
            if(temp_fomula == "OP_HIGH -> *")
            {
                cur_op_high = "*";
            }
            if(temp_fomula == "OP_HIGH -> /")
            {
                cur_op_high = "/";
            }
            if(temp_fomula == "FACTOR -> TOKEN")
            {
                midcode_file << "=\t" << cur_id << "\t" << "none\t" << "f" << cur_factor << endl;
                cur_factor++;
            }
            if(temp_fomula == "FACTOR -> NUM")
            {
                midcode_file << "=\t" << cur_num << "\t" << "none\t" << "f" << cur_factor << endl;
                cur_factor++;
            }
            cur_state = total_stack.top();
        }
        else if(act[cur_state][ACTION_DICT[a]].mode == 2) // accept
        {
            cout << "----------finish----------" << endl;
            reduce_file << "FINISH" << endl;
            midcode_file << "FINISH" << endl;
            reduce_file.close();
            midcode_file.close();
            return;
        }
        else
        {
            cout << "Error!" << endl;
            reduce_file << "Error!" << endl;
            midcode_file << "Error!" << endl;
            reduce_file.close();
            midcode_file.close();
            return;
        }
    }
}
int main(void)
{
    string TOKEN_FILE_PATH = "token.txt";
    string FOMULA_FILE_PATH = "fomula.txt";
    string ACTION_FILE_PATH = "ACTION_2.txt";
    string REDUCE_FILE_PATH = "reduce.txt";
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
        if(i==token_list.size() - 1)break;
        while(token_list[i].category != SEMICOLON)
        {
            temp_list.push_back(token_list[i]);
            i++;
        }
        temp_list.push_back(token_list[i]);
        for(j=0;j<temp_list.size();j++)
        {
            cout << temp_list[j].category  << '\t' << temp_list[j].value << endl;
        }
        SyntaxAnalyze(FOMULA_FILE_PATH,temp_list,act,goto_list);
        temp_list.clear();
        if(i!=token_list.size() - 1)i++;
        pos = i;
    }
    return 0;
}
