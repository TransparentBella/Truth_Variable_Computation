#include <vector>
#include <stack>
#include <utility>
#include <iostream>
#include <math.h>
#include <algorithm>
int cmp(const void *pa, const void *pb);
int getOpPrecedence(char a);
bool getImplicationEvaluation(bool p, bool q);
int getCntVariable(std::string str);
int getTotalLines(std::string str);
std::vector<int> getOrderBasedONAscIICode(std::string str);
std::string getpostfix(std::string str);
std::pair<std::vector<bool>, int> calculateTruthAssignmentOfLineT(std::string str, int t);
std::vector<bool> calculateResults(std::string str, int cntVariable);
void putTruthtable(std::string);
std::vector<int> getPdnf(std::string str);
std::vector<int> getPcnf(std::string str);
void putPdnf(std::string str);
void putPdnf(std::string str);
int cmp(const void *pa, const void *pb)
{
    char *pca = (char *)pa, *pcb = (char *)pb;
    if ((*pca > *pcb || *pca == '\0') && *pcb != '\0')
        return 1;
    else
        return -1;
}
int getOpPrecedence(char a)
{
    switch (a)
    {
    /* case '(':
    return 0;//不出栈 */
    case '!':
        return 5;
    case '&':
        return 4;
    case '|':
        return 3;
    case '>':
        return 2;
    case '-':
        return 1;
    }
    return -1;
}
bool getImplicationEvaluation(bool p, bool q)
{
    if (p == true && q == false)
        return false;
    else
        return true;
}
int getCntVariable(std::string str)
{
    int cnt = 0, i = 0;
    char p[26] = {0};
    while (i < int(str.length()))
    {
        if (str[i] >= 'a' && str[i] <= 'z' && p[str[i] - 'a'] == '\0')
        {
            p[str[i] - 'a'] = str[i];
            cnt++;
        }
        i++;
    }
    return cnt;
}
int getTotalLines(std::string str)
{
    int cnt = getCntVariable(str);
    return pow(2, cnt);
}
std::vector<int> getOrderBasedONAscIICode(std::string str)
{
    int cnt = getCntVariable(str), i = 0;
    char p[26] = {0}; ///////////////////////////
    std::vector<int> asciiorder;
    ;
    while (i < int(str.length()))
    {
        if (str[i] >= 'a' && str[i] <= 'z' /* && p[str[i] - 'a'] == '\0' */)
        {
            p[str[i] - 'a'] = str[i];
            cnt++;
        }
        i++;
    }
    qsort(p, 26, sizeof(char), cmp);
    for (int j = 0; j < 26; j++)
        asciiorder.push_back(-1);
    for (int s = 0; s < cnt; s++)
    {
        /*         std::cout << p[s] << ' '; */
        asciiorder[p[s] - 'a'] = s;
    }
    return asciiorder;
}
std::string getpostfix(std::string str)
{
    std::string postfix;
    std::stack<char> operationStack;
    int i = 0;
    while (i < int(str.length()))
    {
        if (str[i] >= 'a' && str[i] <= 'z')
        {
            postfix.push_back(str[i]);
        }
        else if (str[i] == '!' || str[i] == '&' || str[i] == '|' || str[i] == '>' || str[i] == '-')
        {
            if (operationStack.empty())
            {
                operationStack.push(str[i]);
            }
            else if (operationStack.top() == '(')
            {
                operationStack.push(str[i]);
            }
            else if (getOpPrecedence(operationStack.top()) > getOpPrecedence(str[i]) && getOpPrecedence(operationStack.top()) > 0 && getOpPrecedence(str[i]) > 0)
            {
                do
                {
                    postfix.push_back(operationStack.top());
                    operationStack.pop();
                } while (!operationStack.empty() && getOpPrecedence(operationStack.top()) > getOpPrecedence(str[i]) && getOpPrecedence(operationStack.top()) > 0 && getOpPrecedence(str[i]) > 0);
                operationStack.push(str[i]);
            }
            else /* if(getOpPrecedence(operationStack.top()) <= getOpPrecedence(str[i])) */
            {
                operationStack.push(str[i]);
            }
        }
        else if (str[i] == '(')
        {
            operationStack.push('(');
        }
        else if (str[i] == ')')
        {
            while (operationStack.top() != '(' && !operationStack.empty())
            {
                postfix.push_back(operationStack.top());
                operationStack.pop();
            }
            operationStack.pop(); // 括号内所有符号按优先级全部弹出
        }
        i++;
    }
    while (!operationStack.empty())
    {
        postfix.push_back(operationStack.top());
        operationStack.pop();
    }
    return postfix;
}
std::pair<std::vector<bool>, int> calculateTruthAssignmentOfLineT(std::string str, int t)
{
    int cntVariable = getCntVariable(str);
    std::string postfix = getpostfix(str);
    std::vector<bool> tempResult;
    std::vector<bool> assignment;
    std::vector<int> asciiorder = getOrderBasedONAscIICode(str);
    bool value[26] = {0};
    for (int l = cntVariable - 1; l >= 0; l--)
    {
        value[l] = (t >> cntVariable - l - 1) % 2;
        assignment.push_back(value[l]);
        /* std::cout << value[l] << ' '; */
    } // 按二进制递增赋真值给以字典序排列的小写字母变量
    // 依次存入EachTruthAssignment数组，注意弹出时先弹出字典序小的变量的取值
    for (auto iter = postfix.begin(); iter != postfix.end(); iter++)
    {
        if (*iter >= 'a' && *iter <= 'z')
        {
            tempResult.push_back(value[asciiorder[*iter - 'a']]);
        }
        else if (*iter == '!' && !tempResult.empty())
        {
            bool temp = !tempResult.back();
            tempResult.pop_back();
            tempResult.push_back(temp);
        }
        else if ((*iter == '&' || *iter == '|') && !tempResult.empty())
        {
            bool temp1 = tempResult.back(), temp2;
            tempResult.pop_back();
            temp2 = tempResult.back();
            tempResult.pop_back();
            *iter == '&' ? tempResult.push_back(temp1 && temp2) : tempResult.push_back(temp1 || temp2);
        }
        else if (*iter == '>' || *iter == '-')
        {
            bool tempbehind = tempResult.back(), tempahead;
            tempResult.pop_back();
            tempahead = tempResult.back();
            tempResult.pop_back();
            *iter == '>' ? tempResult.push_back(getImplicationEvaluation(tempahead, tempbehind)) : tempResult.push_back(getImplicationEvaluation(tempahead, tempbehind) && getImplicationEvaluation(tempbehind, tempahead));
        }
    }
    std::pair<std::vector<bool>, int> p;
    p.first = assignment;
    p.second = tempResult.back();
    tempResult.pop_back();
    if (!tempResult.empty())
        std::cout << "error";
    return p;
}
std::vector<bool> calculateResults(std::string str, int cntVariable)
{
    std::vector<bool> results;
    int times = pow(2, cntVariable);
    std::string postf = getpostfix(str);
    for (int k = 0; k < times; k++)
    {
        results.push_back(calculateTruthAssignmentOfLineT(str, k).second);
    }
    return results;
}
void putTruthtable(std::string str)
{
    int l = getTotalLines(str);
    for (int i = 0; i < l; i++)
    {
        /* std::cout << "i = "<< i << " "; */
        std::pair<std::vector<bool>, int> temppair = calculateTruthAssignmentOfLineT(str, i);
        std::for_each(temppair.first.rbegin(), temppair.first.rend(), [] (bool value)
        {
            std::cout << value << ' ';      
        });
        std::cout << temppair.second << std::endl;
    }
}
std::vector<int> getPdnf(std::string str)
{
    std::vector<bool> results = calculateResults(str, getCntVariable(str));
    std::vector<int> pdnf;
    for(int i = 0; i < results.size(); i++)
    {
        if(results.at(i))
        pdnf.push_back(i);
    }
    return pdnf;
}
std::vector<int> getPcnf(std::string str)
{
    std::vector<bool> results = calculateResults(str, getCntVariable(str));
    std::vector<int> pcnf;
    for(int i = 0; i < results.size(); i++)
    {
        if(!results.at(i))
        pcnf.push_back(i);
    }
    return pcnf;
}
void putPdnf(std::string str)
{
    std::vector<int> pdnf = getPdnf(str);
    for(auto iter = pdnf.begin(); iter != pdnf.end(); iter++)
    {
        std::cout << 'm' << *iter << ' ';
    }
    std::cout << std::endl;
}
void putPcnf(std::string str)
{
    std::vector<int> pcnf = getPcnf(str);
    for(auto iter = pcnf.begin(); iter != pcnf.end(); iter++)
    {
        std::cout << 'M' << *iter << ' ';
    }
    std::cout << std::endl;
}