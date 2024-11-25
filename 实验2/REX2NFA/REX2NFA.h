#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <cstring>

using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::stack;
using std::string;
using std::vector;
#pragma region NFAstruct

struct NFA
{
    NFA Create(const char &msg);
    NFA Closure();
    NFA Connect(NFA &other);
    NFA Or(NFA &other);
    map<pair<int, int>, char> stateList;
    int startNode;
    int endNode;
};

NFA NFA::Create(const char &msg)
{
    static int stateIndex = 0;
    startNode = stateIndex++;
    endNode = stateIndex++;
    stateList[{startNode, endNode}] = msg;
    return *this;
}

NFA NFA::Connect(NFA &other)
{
    stateList[{endNode, other.startNode}] = 'E';
    endNode = other.endNode;
    stateList.insert(other.stateList.begin(), other.stateList.end());
    return *this;
}

NFA NFA::Or(NFA &other)
{
    NFA nfa;
    nfa.Create('E');
    nfa.stateList[{nfa.startNode, startNode}] = 'E';
    nfa.stateList[{endNode, nfa.endNode}] = 'E';
    nfa.stateList[{nfa.startNode, other.startNode}] = 'E';
    nfa.stateList[{other.endNode, nfa.endNode}] = 'E';
    nfa.stateList.insert(stateList.begin(), stateList.end());
    nfa.stateList.insert(other.stateList.begin(), other.stateList.end());
    nfa.stateList.erase({nfa.startNode, nfa.endNode});
    return nfa;
}

NFA NFA::Closure()
{
    NFA nfa;
    nfa.Create('E');
    nfa.stateList[{nfa.startNode, startNode}] = 'E';
    nfa.stateList[{endNode, nfa.endNode}] = 'E';
    nfa.stateList[{endNode, startNode}] = 'E';
    nfa.stateList.insert(stateList.begin(), stateList.end());
    return nfa;
}
#pragma endregion

string AddConnectionSymbol(const string &expr);
void ToSuffix(const string &expr, string &result);
bool IsOperator(const char &c);
NFA Suffix2Nfa(const string &expr);
void showNfa(const NFA &finalNfa);
bool checkSentence(const string &expr , const NFA finalNfa);