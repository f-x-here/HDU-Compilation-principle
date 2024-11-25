#include "REX2NFA.h"
int main()
{
    string expr, exprPro, sentence;
    string suffix = "";
    cin >> expr;
    exprPro = AddConnectionSymbol(expr);
#ifdef DEBUG
    cout << exprPro << endl;
#endif
    ToSuffix(exprPro, suffix);
#ifdef DEBUG
    cout << suffix << endl;
#endif
    NFA finalNfa = Suffix2Nfa(suffix);
    showNfa(finalNfa);
    cout << endl;
    cout << "请输入待检测句子,Ctrl+z结束输出" << endl;
    while (cin >> sentence)
    {
        if(checkSentence(sentence, finalNfa))
        {
            cout<<"correct!"<<endl;
        }
        else
        {
            cout<<"wrong!"<<endl;
        }
    }
}

string AddConnectionSymbol(const string &expr)
{
    string result = "";
    for (int i = 0; i < expr.length() - 1; i++)
    {
        result.push_back(expr[i]);
        if (isalnum(expr[i]))
        {
            if (isalnum(expr[i + 1]) || (expr[i + 1] == '('))
            {
                result.push_back('^');
                continue;
            }
        }
        else if (expr[i] == ')' || expr[i] == '*')
        {
            if (isalnum(expr[i + 1]))
            {
                result.push_back('^');
                continue;
            }
        }
    }
    result.push_back(expr[expr.length() - 1]);
    return result;
}

void ToSuffix(const string &expr, string &result)
{
    map<char, int> Priority;
    Priority['|'] = 0;
    Priority['^'] = 1;
    Priority['*'] = 2;
    stack<char> op;
    for (auto &c : expr)
    {
        if (IsOperator(c))
        {

            if (op.empty())
            {
                op.push(c);
                continue;
            }
            while (!op.empty() && (Priority[c] <= Priority[op.top()]) && !(op.top() == '('))
            {
                result.push_back(op.top());
                op.pop();
            }
            op.push(c);
        }
        else if (c == '(')
        {
            op.push(c);
        }
        else if (c == ')')
        {
            while (op.top() != '(')
            {
                result.push_back(op.top());
                op.pop();
            }
            op.pop();
        }
        else
        {
            result.push_back(c);
        }
    }
    while (!op.empty())
    {
        result.push_back(op.top());
        op.pop();
    }
}

bool IsOperator(const char &c)
{
    if (c == '|' || c == '*' || c == '^')
        return true;
    return false;
}

NFA Suffix2Nfa(const string &expr)
{
    stack<NFA> nfaStack;
    for (auto &c : expr)
    {
        if (isalnum(c))
        {
            NFA nfa;
            nfa.Create(c);
            nfaStack.push(nfa);
        }
        else
        {
            NFA n1 = nfaStack.top();
            nfaStack.pop();
            switch (c)
            {
            case '^':
            {
                NFA n2 = nfaStack.top();
                nfaStack.pop();
                nfaStack.push(n2.Connect(n1));
                break;
            }
            case '|':
            {
                NFA n2 = nfaStack.top();
                nfaStack.pop();
                nfaStack.push(n2.Or(n1));
                break;
            }
            case '*':
            {
                nfaStack.push(n1.Closure());
                break;
            }
            }
        }
    }
    return nfaStack.top();
}

void showNfa(const NFA &finalNfa)
{
    for (auto &it : finalNfa.stateList)
    {
        cout << it.first.first << "->" << it.first.second << ":" << it.second << endl;
    }
    cout << "startNode: " << finalNfa.startNode << " endNode: " << finalNfa.endNode << endl;
}

bool checkSentence(const string &expr, const NFA finalNfa)
{
    int nowNode = finalNfa.startNode;
    stack<int> Node1;
    stack<int> Node2;
    Node2.push(nowNode);
    for (auto &c : expr)
    {
        if (Node2.empty())
        {
            return false;
        }
        else
        {
            Node1 = Node2;
            while (!Node2.empty())
                Node2.pop();
        }
        while (!Node1.empty())
        {
            nowNode = Node1.top();
            Node1.pop();
            for (auto it : finalNfa.stateList)
            {
                if (it.first.first == nowNode)
                {
                    if (it.second == 'E')
                    {
                        Node1.push(it.first.second);
                    }
                    else if (it.second == c)
                    {
                        Node2.push(it.first.second);
                    }
                }
            }
        }
    }
    if(!Node2.empty())
    {
        while(!Node2.empty())
        {
            nowNode = Node2.top();
            Node2.pop();
            if(nowNode==finalNfa.endNode)return true;
            for (auto it : finalNfa.stateList)
            {
                if (it.first.first == nowNode)
                {
                    if (it.second == 'E')
                    {
                        Node2.push(it.first.second);
                    }
                }
            }
        }
    }
    return false;
}