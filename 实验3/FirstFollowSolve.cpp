#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cctype>
#include <set>

using namespace std;

map<char, vector<string>> sentences;
map<char, set<char>> firstSet;
map<char, set<char>> followSet;

set<char> Firstsolve(char S)
{
    if (!firstSet[S].empty())
    {
        return firstSet[S];
    }
    if (islower(S))
    {
        return {S};
    }
    set<char> result;
    int flag = 0;
    for (auto &it : sentences[S])
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
                for (auto &f : Firstsolve(it[i]))
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
    firstSet[S] = result;
    return result;
}

void Follow()
{
    for (auto &it : sentences)
    {
        followSet[it.first] = {};
    }
    followSet['S'].insert('$');
    bool change = true;
    bool cont = false;
    bool needAdd = false;
    while (change)
    {
        change = false;
        for (auto &it : sentences)
        {
            for (auto &iit : it.second) // A -> BCD it.first : A   it.second : BCD
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
                            for (auto &iiit : Firstsolve(iit[j]))
                            {
                                if (iiit == '@')
                                {
                                    needAdd = true;
                                    cont = true;
                                }
                                else if (followSet[iit[i]].insert(iiit).second)
                                {
                                    change = true;
                                }
                            }
                            if (needAdd) // follow(B)+=follow(A)
                            {
                                for (auto &iiit : followSet[it.first])
                                {
                                    if (followSet[iit[i]].insert(iiit).second)
                                    {
                                        change = true;
                                    }
                                }
                            }
                        }
                        else //终结符直接加
                        {
                            if (followSet[iit[i]].insert(iit[i + 1]).second)
                            {
                                change = true;
                            }
                        }
                        if(!cont)break; // first(B)无ε，跳出
                    }
                }
                if (isupper(iit[iit.length() - 1]))
                {
                    for (auto &iiit : followSet[it.first])
                    {
                        if (followSet[iit[iit.length() - 1]].insert(iiit).second)
                        {
                            change = true;
                        }
                    }
                }
            }
        }
    }
}

int main()
{
    char left;
    string right, line;
    while (getline(cin, line))
    {
        left = line[0];
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
            }
        }
        sentences[left].push_back(right);
    }
    for (auto &it : sentences)
    {
        Firstsolve(it.first);
    }
    for (auto &it : firstSet)
    {
        cout << "First(" << it.first << ") = { ";
        for (auto &itt : it.second)
        {
            if (itt == '@')
            {
                cout << "ε ";
                continue;
            }
            cout << itt << " ";
        }
        cout << "}" << endl;
    }
    Follow();
    for (auto &it : followSet)
    {
        cout << "Follow(" << it.first << ") = {";
        for (auto &iit : it.second)
        {
            cout << " " << iit;
        }
        cout << " }" << endl;
    }
}