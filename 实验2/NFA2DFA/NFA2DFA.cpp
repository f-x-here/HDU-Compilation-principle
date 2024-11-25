#include "NFA2DFA.h"

int main()
{
	int a, b;
	char c;
	NFA nfa;
	cin >> a;
	nfa.startNode = a;
	cin >> b;
	nfa.endNode = b;
	while (cin >> a >> b >> c)
	{
		nfa.stateList[{a, b}] = c;
		if (c != 'E' && std::count(changeChar.begin(), changeChar.end(), c) == 0)
		{
			changeChar.insert(c);
		}
	}
	DFA dfa;
	Dstate cur, next;
	cur.nodes.insert(nfa.startNode);
	cur = e_closure(cur, 'E', nfa);
	cur.Create();
	cur.nodes.insert(nfa.startNode);
	dfa.startState = cur;
	dfa.states.insert(cur);
	stack<Dstate>state2do;
	state2do.push(cur);
	int flag;
	while (!state2do.empty())
	{
		cur = state2do.top();
		state2do.pop();
		if (cur.nodes.empty())continue;
		for (auto& c : changeChar)
		{
			next = e_closure(cur, c, nfa);
			flag = 0;
			for (auto it : dfa.states)
			{
				if (it.nodes == next.nodes)
				{
					next.id = it.id;
					flag = 1;
					break;
				}
			}
			if (flag ==0)
			{
				next.Create();
				state2do.push(next);
				dfa.states.insert(next);
			}
			dfa.G[{cur, c}] = next;
			if (next.nodes.count(nfa.endNode))
			{
				dfa.endstates.insert(next);
			}
		}
	}
	dfa.showDfa();
}