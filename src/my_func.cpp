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
struct singal_action
{
    int mode;// -1为shift,0为空,1为reduce,2为accept
    int next_state;
    int beta;
    string A;
    string fomula;
};
void ManualActionList(struct singal_action act[ACTION_ROW][ACTION_COL])
{
    //-----------col 0----------
    act[0][0].mode=-1;act[0][0].next_state=5;
    act[3][0].mode=-1;act[3][0].next_state=8;
    act[6][0].mode=1;act[6][0].A="KEYWORD";act[6][0].beta=1;act[6][0].fomula="KEYWORD -> int";
    
}