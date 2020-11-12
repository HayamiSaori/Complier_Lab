#include <iostream>
#include <map>
#include <string>
#include <ctype.h>
#include <fstream>
#include <sstream>
#include "code_table.cpp"
using namespace std;
int isBlank(char c)
{
    if(c=='\n' || c==' ' || c=='\t')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int isDigit(char c)
{
    if(!isalpha(c) && isalnum(c))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
map<string,int> TOKEN_LIST;
struct token
{
    int category;
    string value;
};
string SOURCE_CODE;
string getCode(string code_path)
{
    ifstream code_file;
    string buff,codes = "";
    long cur_lenth = 0;
    code_file.open(code_path,ios::in);
    while (getline(code_file,buff))
    {
        if((cur_lenth + buff.size()) > codes.max_size())
        {
            cout << "the code file is too long to get all" << endl;
            break;
        }
        else
        {
            codes += buff;
            cur_lenth += buff.size();
        }
    }
    code_file.close();
    return codes;
}
int InstallID(string s)
{
    int temp;
    if(s=="int")
    {
        return INT;
    }
    else if (s=="double")
    {
        return DOUBLE;
    }
    else if (s=="char")
    {
        return CHAR;
    }
    else if (s=="if")
    {
        return IF;
    }
    else if (s=="else")
    {
        return ELSE;
    }
    else if (s=="while")
    {
        return WHILE;
    }
    else if (s=="BREAK")
    {
        return BREAK;
    }
    else if (s=="return")
    {
        return RETURN;
    }
    else
    {
        map<string,int>::iterator i;
        i = TOKEN_LIST.find(s);
        if(i!=TOKEN_LIST.end())
        {
            return i->second;
        }
        else
        {
            temp = TOKEN_LIST.size() + 1;
            TOKEN_LIST[s] = temp;
            ofstream character_list;
            character_list.open("character_list.txt",ios::app);
            character_list << temp << " " << s << endl;
            character_list.close();
            return temp;
        }
    }
}
token ScanCode(long& begin,long& end)
{
    end = begin;
    token result;
    string t,ID_s;
    int ID;stringstream ss;
    if(begin >= SOURCE_CODE.size())
    {
        result.category = -1;
        result.value = "";
        return result;
    }
    while(isBlank(SOURCE_CODE[begin]))
    {
        begin++;
    }
    end = begin;
    if(isalpha(SOURCE_CODE[begin]))
    {
        end++;
        while (isalnum(SOURCE_CODE[end]))
        {
            end++;
        }
        t = SOURCE_CODE.substr(begin,end-begin);
        end--;
        ID = InstallID(t);
        ss << ID;
        ID_s = ss.str();
        result.value = ID_s;
        result.category = TOKEN;
        // return result;
    }
    else if(isDigit(SOURCE_CODE[begin]))
    {
        end++;
        while (isDigit(SOURCE_CODE[end]))
        {
            end++;
        }
        if(SOURCE_CODE[end] == '.')
        {
            end++;
            while (isDigit(SOURCE_CODE[end]))
            {
                end++;
            }
            t = SOURCE_CODE.substr(begin,end-begin);
            end--;
            result.value = t;
            result.category = SIGNEDFLOAT;
        }
        else
        {
            t = SOURCE_CODE.substr(begin,end-begin);
            end--;
            result.value = t;
            result.category = SIGNEDINT;
        }
    }
    else
    {
        switch (SOURCE_CODE[begin])
        {
        case '+':
            result.category = ADD;
            result.value = "+";
            break;
        case '-':
            result.category = SUB;
            result.value = "-";
            break;    
        case '*':
            result.category = MUL;
            result.value = "*";
            break;
        case '/':
            result.category = DIV;
            result.value = "/";
            break;
        case '!':
            end++;
            if(SOURCE_CODE[end] == '=')
            {
                result.category = NEQ;
                result.value = "!=";
            }
            else
            {
                end--;
                result.category = NOT;
                result.value = "!";
            }
            break;
        case '|':
            result.category = OR;
            result.value = "|";
            break;
        case '&':
            result.category = AND;
            result.value = "&";
            break;
        case '=':
            end++;
            if(SOURCE_CODE[end] == '=')
            {
                result.category = EQU;
                result.value = "==";
            }
            else
            {
                end--;
                result.category = ASSIGN;
                result.value = "=";
            }
            break;
        case '<':
            end++;
            if(SOURCE_CODE[end] == '=')
            {
                result.category = SOE;
                result.value = "<=";
            }
            else
            {
                end--;
                result.category = SML;
                result.value = "<";
            }
            break;
        case '>':
            end++;
            if(SOURCE_CODE[end] == '=')
            {
                result.category = GOE;
                result.value = ">=";
            }
            else
            {
                end--;
                result.category = GRT;
                result.value = ">";
            }
            break;
        case ';':
            result.category = SEMICOLON;
            result.value = ";";
            break;
        default:
            result.category = -1;
            result.value = "";
            break;
        }
    }
    end++;
    begin = end;
    return result;
}

int main(int argc,char * argv[])
{
    string CODE_FILE_PATH = "code.txt";
    string TOKEN_FILE_PATH = "token.txt";
    ofstream token_file;
    token_file.open(TOKEN_FILE_PATH,ios::out);
    string temp_str;
    SOURCE_CODE = getCode(CODE_FILE_PATH);
    // cout<<SOURCE_CODE<<' '<<SOURCE_CODE.size()<<endl;
    long begin=0,end=0;
    token temptoken = ScanCode(begin,end);
    while (temptoken.category!=-1)
    {
        // temp_str = "(" ;+ ss.str() + "," + temptoken.value;
        token_file << "(" << temptoken.category<<"," + temptoken.value + ")" << endl;
        temptoken = ScanCode(begin,end);
    }   
    token_file.close();
}
