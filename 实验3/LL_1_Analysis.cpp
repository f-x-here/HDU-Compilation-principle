#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cctype>
#include <set>
#include <algorithm>
#include <iterator>
#include <deque>
#include <regex>
#include <stack>

using namespace std;

map<char, vector<string>> sentences;
map<char, set<char>> firstset;
map<char, set<char>> followset;
set<char> terminal;
set<char> inTerminal;


bool haveIntersaction(const set<char> set1, const set<char> set2)
{
	set<char> intersection;
	set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(intersection, intersection.begin()));
	return !intersection.empty();
}

set<char> Firstsolve(char S)
{
	if (!firstset[S].empty())
	{
		return firstset[S];
	}
	if (islower(S))
	{
		return { S };
	}
	set<char> result;
	int flag = 0;
	for (auto& it : sentences[S])
	{
		for (int i = 0; i < it.length(); i++)
		{
			if (islower(it[i]) || it[i] == '@')
			{
				result.insert(it[i]);
				break;
			}
			else if (isupper(it[i]))
			{
				for (auto& f : Firstsolve(it[i]))
				{
					if (f == '@')
					{
						flag = 1;
						if (i == it.length() - 1)
						{
							result.insert('@');
						}
					}
					else
					{
						result.insert(f);
					}
				}
			}
			if (!flag)
			{
				break;
			}
			else
			{
				flag = 0;
			}
		}
	}
	firstset[S] = result;
	return result;
}

void Follow()
{
	for (auto& it : sentences)
	{
		followset[it.first] = {};
	}
	followset['S'].insert('$');
	bool change = true;
	bool cont = false;
	bool needAdd = false;
	while (change)
	{
		change = false;
		for (auto& it : sentences)
		{
			for (auto& iit : it.second) // A -> BCD it.first : A   it.second : BCD
			{
				for (int i = 0; i < iit.length() - 1; i++)
				{
					if (islower(iit[i]) || iit[i] == '@')
						continue;
					for (int j = i + 1; j < iit.length(); j++)
					{
						cont = false;
						if (isupper(iit[j]))
						{
							needAdd = false;
							for (auto& iiit : Firstsolve(iit[j]))
							{
								if (iiit == '@')
								{
									needAdd = true;
									cont = true;
								}
								else if (followset[iit[i]].insert(iiit).second)
								{
									change = true;
								}
							}
							if (needAdd) // follow(B)+=follow(A)
							{
								for (auto& iiit : followset[it.first])
								{
									if (followset[iit[i]].insert(iiit).second)
									{
										change = true;
									}
								}
							}
						}
						else // 终结符直接加
						{
							if (followset[iit[i]].insert(iit[i + 1]).second)
							{
								change = true;
							}
						}
						if (!cont)
							break; // first(B)无ε，跳出
					}
				}
				if (isupper(iit[iit.length() - 1]))
				{
					for (auto& iiit : followset[it.first])
					{
						if (followset[iit[iit.length() - 1]].insert(iiit).second)
						{
							change = true;
						}
					}
				}
			}
		}
	}
}

void First()
{
	for (auto& it : sentences)
	{
		Firstsolve(it.first);
	}
}

set<char> GetFirstForSentence(string s)
{
	set<char> result;
	bool flag = false;
	for (int i = 0; i < s.length(); i++)
	{
		flag = false;
		if (islower(s[i]))
		{
			result.insert(s[i]);
		}
		else if (isupper(s[i]))
		{
			for (auto& ii : firstset[s[i]])
			{
				if (ii == '@')
				{
					flag = true;
					if (i == s.length() - 1)
					{
						result.insert('@');
					}
				}
				else
				{
					result.insert(ii);
				}
			}
		}
		else if (s[i] == '@')
		{
			flag = true;
			if (i == s.length() - 1)
			{
				result.insert('@');
			}
		}
		if (!flag)
			break;
	}
	return result;
}

bool LL1Analysis()
{
	set<char> s1, s2;
	for (auto& it : sentences)
	{
		for (int i = 0; i < it.second.size(); i++)
		{
			for (int j = i + 1; j < it.second.size(); j++)
			{
				s1 = GetFirstForSentence(it.second[i]);
				s2 = GetFirstForSentence(it.second[j]);
				if (haveIntersaction(s1, s2))
				{
					cout << "ERROR!EXISTS First(α) ∩ First(β) != Φ!";
					return false;
				}
				else if (s1.count('@') && haveIntersaction(s2, followset[it.first]))
				{
					cout << "ERROR!EXISTS Follow(A) ∩ First(β) != Φ While First(α) has 'ε'!";
					return false;
				}
				else if (s2.count('@') && haveIntersaction(s1, followset[it.first]))
				{

					cout << "ERROR!EXISTS Follow(A) ∩ First(α) != Φ While First(β) has 'ε'!";
					return false;
				}
			}
		}
	}
	return true;
}

bool BasicCheck()
{
	set<char> checkset;
	for (auto& it : sentences)
	{
		checkset = {};
		for (auto& iit : it.second)
		{
			if (it.first == iit[0])
			{
				cout << "ERROR!EXISTS DIRECT LEFT RECURSION!";
				return false;
			}
			if (!checkset.insert(iit[0]).second)
			{
				cout << "ERROR!EXISTS LEFT COMMON FACTORS!";
				return false;
			}
		}
	}
	return true;
}

map<pair<char, char>, string> M;

pair<deque<char>, bool> GetFirstForTableBuild(string s)
{
	set<char> result;
	deque<char> answer;
	bool flag = false;
	for (int i = 0; i < s.length(); i++)
	{
		flag = false;
		if (islower(s[i]))
		{
			if (result.insert(s[i]).second)
			{
				answer.push_back(s[i]);
			}
		}
		else if (isupper(s[i]))
		{
			for (auto& ii : firstset[s[i]])
			{
				if (ii == '@')
				{
					flag = true;
					if (i == s.length() - 1)
					{
						if (result.insert('@').second)
						{
							answer.push_back('@');
						}
					}
				}
				else
				{
					if (result.insert(ii).second)
					{
						answer.push_back(ii);
					}
				}
			}
		}
		else if (s[i] == '@')
		{
			flag = true;
			if (i == s.length() - 1)
			{
				if (result.insert('@').second)
				{
					answer.push_back('@');
				}
			}
		}
		if (!flag)
			break;
	}
	return { answer,flag };
}

void BuildTable()
{
	regex reg("@");
	pair<deque<char>, bool>list;
	for (auto& it : sentences)
	{
		for (auto iit : it.second)
		{
			list = GetFirstForTableBuild(iit);
			while (!list.first.empty())
			{
				M[{it.first, list.first.front()}] = (string(1, it.first) + " -> " + regex_replace(iit, reg, "ε"));
				list.first.pop_front();
			}
			if (list.second)
			{
				for (auto& iiit : followset[it.first])
				{
					M[{it.first, iiit}] = (string(1, it.first) + " -> " + regex_replace(iit, reg, "ε"));
				}
			}
		}
	}
}

void showTable()
{
	cout << "\t|";
	for (auto& it : terminal)
	{
		if (it == '@')
		{
			cout << "ε\t|";
		}
		else
		{
			cout << it << "\t|";
		}
	}
	cout << endl;
	for (auto& it : inTerminal)
	{
		cout << it << "\t|";
		for (auto& iit : terminal)
		{
			if (M.count({ it,iit }) == 0)
			{
				cout << "\t|";
			}
			else
			{
				cout << " " << M[{it, iit}] << " |";
			}
		}
		cout << endl;
	}
}

bool LL1Solve(string expr)
{
	stack<char>cur;
	stack<char>wait;
	wait.push('$');
	for (int i = expr.size() - 1; i >= 0; i--)
	{
		wait.push(expr[i]);
	}
	cur.push('S');
	char curSymbol, top;
	string production;
	regex rex("ε");
	while (!cur.empty())
	{
		top = cur.top();
		cur.pop();
		curSymbol = wait.top();
		if (!isupper(top))
		{
			if (top == curSymbol)
			{
				wait.pop();
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (M.count({ top,curSymbol }) != 0)
			{
				production = M[{top, curSymbol}];
				production = regex_replace(production, rex, "@");
				for (int i = production.length() - 1; i > 4; i--)
				{
					if(production[i]!='@')
					cur.push(production[i]);
				}
			}
			else
			{
				return false;
			}
		}
	}
	if (wait.top() == '$')
	{
		return true;
	}
	else
	{
		return false;
	}
}

int main()
{
	char left;
	string right, line;
	int n;
	cout << "input num of lines"<<endl;
	cin >> n;
	getchar();
	for (int i=0;i<n;i++)
	{
		getline(cin, line);
		left = line[0];
		inTerminal.insert(line[0]);
		right = "";
		for (int i = 1; i < line.length(); i++)
		{
			if (line[i] == '-' || line[i] == '>' || line[i] == ' ')
			{
				continue;
			}
			else if (line[i] == '|')
			{
				sentences[left].push_back(right);
				right = "";
			}
			else
			{
				right += line[i];
				if (!isupper(line[i]) && line[i] != '@')
				{
					terminal.insert(line[i]);
				}
			}
		}
		sentences[left].push_back(right);
	}
	terminal.insert('$');
	if (!BasicCheck())
	{
		return 0;
	}
	First();
	Follow();
	if (!LL1Analysis())
	{
		return 0;
	}
	BuildTable();
	showTable();
	cout << endl << "please input string to parser: "<<endl;
	string expr;
	cin >> expr;
	if (LL1Solve(expr))
	{
		cout << "Parssing Successful!" << endl;
	}
	else
	{
		cout << "Syntax Error!" << endl;
	}
}
