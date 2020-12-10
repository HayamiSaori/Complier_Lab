import random
# 主存标记-1
MEMORY = -1
# 可使用的寄存器编号：8-25
reg_file = [8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25]
reg_available = len(reg_file)
# 变量名集合
tokens = set()
# 变量出现次数字典<name,number>
tokens_num = {}
# 寄存器内容字典<reg,content>
reg_content = {}
for reg in reg_file:
    reg_content[reg] = "empty"
# 变量的存储位置字典
tokens_addr = {}

def getMidCode(midcode_path):
    mid_file = open(midcode_path,'r')
    midcode = []
    lines = mid_file.readlines()
    for line in lines:
        item = line.split()
        midcode.append(item)
        for i in range(1,4):
            if(item[i] == "none"):
                continue
            else:
                if item[i] in tokens:
                    tokens_num[item[i]] = tokens_num[item[i]] + 1
                else:
                    tokens_num[item[i]] = 1
                    tokens.add(item[i])
    # print(midcode)
    mid_file.close()
    return midcode

def WriteToFile(midcode,L1,L2,L3):
    target_file = open("TargetCode.txt","a+")
    op = midcode[0]
    arg1 = midcode[1]
    arg2 = midcode[2]
    result = midcode[3]
    if op == "-":
        op_str = "sub"
    elif op == "+":
        op_str = "add"
    if L1 == MEMORY:
        arg1_addr = str(arg1)
    else:
        arg1_addr = "$"+str(L1)
    if L2 == MEMORY:
        arg2_addr = str(arg2)
    else:
        arg2_addr = "$"+str(L2)
    if L3 == MEMORY:
        result_addr = str(result)
    else:
        result_addr = "$"+str(L3)
    target_file.write(op_str+"\t"+arg1_addr+","+arg2_addr+","+result_addr+"\n")
    target_file.close()

# 获取中间代码对应的地址(arg1,arg2,result) => (L1,L2,L3)
def getReg(midcode):
    arg1 = midcode[1]
    arg2 = midcode[2]
    result = midcode[3]
    L1 = tokens_addr[arg1]
    L2 = tokens_addr[arg2]
    L3 = tokens_addr[result]
    global reg_available
    # 若arg1在寄存器中，且之后不会再次使用
    if(tokens_addr[arg1] in reg_file) and (tokens_num[arg1] <= 1):
        L3 = tokens_addr[arg1]
        L2 = tokens_addr[arg2]
        L1 = tokens_addr[arg1]
    # 若变量arg2在寄存器中，且arg2之后不会再次使用
    if(tokens_addr[arg2] in reg_file) and (tokens_num[arg2] <= 1):
        L3 = tokens_addr[arg2]
        L2 = tokens_addr[arg2]
        L1 = tokens_addr[arg1]
    # 否则，若还有空闲寄存器
    elif reg_available > 0:
        for reg in reg_file:
            if reg_content[reg] == "empty":
                reg_available = reg_available - 1
                reg_content[reg] = result
                tokens_addr[result] = reg
                L3 = reg
                break
    # 否则，若x以后还会使用
    # elif tokens_num[x] > 1:
    #     temp_reg = random.randint(reg_file[0],reg_file[len(reg_file) - 1])
    #     WriteToFile("mov\t$"+str(temp_reg)+","+reg_content[temp_reg])
    #     return temp_reg
    # 否则，返回主存位置
    else:
        L3 = MEMORY
    return (L1,L2,L3)

if __name__ == "__main__":
    midcodes = getMidCode("midcode.txt")
    for token in tokens:
        tokens_addr[token] = MEMORY
    for midcode in midcodes:
        x = midcode[3]
        (L1,L2,L3) = getReg(midcode)
        for i in range(3):
            x = midcode[i + 1]
            tokens_num[x] = tokens_num[x] - 1
        WriteToFile(midcode,L1,L2,L3)
        # print(L3)
