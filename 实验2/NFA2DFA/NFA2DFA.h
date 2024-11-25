#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <cstring>
#include <set>
#include <algorithm>

using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::set;
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

#pragma region DFA
set<char>changeChar;
struct Dstate{
    int id;
    set<int>nodes;
    void Create()
    {
        static int _id=1;
        id = _id++;
    }
    bool operator<(const Dstate &d) const
    {
        return nodes < d.nodes;
    }

    bool operator==(const Dstate &d) const
    {
        return nodes == d.nodes;
    }
};

struct DFA
{
    map<pair<Dstate,char>,Dstate>G;
    set<Dstate>endstates;
    Dstate startState;
    set<Dstate>states;
    void showDfa();
};


Dstate e_closure(Dstate  T,char a,NFA nfa)
{
    Dstate newDstate;
    stack<int>state2do;
    for(auto &id:T.nodes)
    {
        state2do.push(id);
    }
    int cur;
    while(!state2do.empty())
    {
        cur = state2do.top();
        state2do.pop();
        for(auto &it:nfa.stateList)
        {
            if(it.first.first==cur&&it.second==a && a!='E')
            {
                newDstate.nodes.insert(it.first.second);
            }
            else if(it.first.first==cur&&it.second=='E')
            {
                newDstate.nodes.insert(it.first.second);
                state2do.push(it.first.second);
            }
        }
    }
    return newDstate;
}

bool cmp(Dstate a,Dstate b)
{
    return a.id<b.id;
}

void DFA::showDfa()
{
    cout<<"changes:"<<endl;
    for(auto &it:G)
    {
        cout<<it.first.first.id<<" -- "<<it.first.second<<" -> "<<it.second.id<<endl;
    }
    cout<<"states:(endstates are marked with *)"<<endl;
    vector<Dstate>states_ (states.begin(), states.end());
    sort(states_.begin(), states_.end(),cmp);
    for(auto &it:states_)
    {
        cout<<it.id<<": ";
        for(auto &iit:it.nodes)
        {
            cout<<iit<<" ";
        }
        if(endstates.count(it))
        {
            cout<<"*";
        }
        cout<<endl;
    }
    
}

#pragma endregion


#pragma region testData
// (a|b)*c
// 6 9
// 0 1 a
// 1 5 E
// 2 3 b
// 3 5 E
// 4 0 E
// 4 2 E
// 5 4 E
// 5 7 E
// 6 4 E
// 6 7 E
// 7 8 E
// 8 9 c
#pragma endregion

