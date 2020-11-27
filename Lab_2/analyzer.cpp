#include <iostream>
#include <map>
#include <string>
#include <ctype.h>
#include <fstream>
#include <vector>
#include "code_table.h"
const int ACTION_ROW = 23;
const int ACTION_COL = 10;
using namespace std;
struct token
{
    int category;
    string value;
};
struct singal_action
{
    int mode;// -1为shift,0为空,1为reduce
    int next_state;
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
    string buff,temp_str;
    int left,right,i,j;
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
                cout << "find none" << endl;
                act[i][j].mode = 0;
            }
            else if(temp_str[0] == 's')
            {
                cout << "find shift" << endl;
                act[i][j].mode = -1;
            }
            else
            {cout << "find reduce" << endl;
                act[i][j].mode = 1;
            }
            j++;
            //cout << temp_str << endl;
        }
        //cout<<"line:"<<i<<",col:"<<j<<endl;
        i++;j=0;
    }
    for(i=0;i<ACTION_ROW;i++)
    {
        for(j=0;j<ACTION_COL;j++)
        {
            cout << act[i][j].mode << ',';
        }
        cout << endl;
    }

}
int main()
{

    string TOKEN_FILE_PATH = "token.txt";
    string FOMULA_FILE_PATH = "fomula.txt";
    string ACTION_FILE_PATH = "ACTION_2.txt";
    vector<struct token> token_list;
    string ACTION[ACTION_ROW][ACTION_COL];
    struct singal_action act[ACTION_ROW][ACTION_COL];
    token_list = ParseTokenFile(TOKEN_FILE_PATH);
    int i;
    /*for(i=0;i<token_list.size();i++)
    {
        cout << token_list[i].category << ',' << token_list[i].value << endl;
    }*/
    ParseActionFile(ACTION_FILE_PATH,act);
}
