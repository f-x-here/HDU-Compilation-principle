#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
struct sentence
{
    sentence() {}
    sentence(string input)
    {
        int i = 0;
        string temp_right = "";
        while (input[i] != '-' || input[i + 1] != '>')
        {
            left += input[i];
            i++;
        }
        i += 2;
        for (; i < input.length(); i++)
        {
            if (input[i] == '|')
            {
                right.push_back(temp_right);
                temp_right = "";
                continue;
            }
            temp_right += input[i];
        }
        right.push_back(temp_right);
    }
    string left;
    vector<string> right;
    bool operator<(const sentence &rhs)
    {
        return left < rhs.left;
    }
};

string RemoveSpace(string input)
{
    input.erase(remove(input.begin(), input.end(), ' '), input.end());
    return input;
}

void Eliminate(sentence &result1, sentence &result2, sentence raw)
{
    vector<string> alpha;
    vector<string> beta;
    string newLeft = raw.left + "\'";
    for (auto &sent : raw.right)
    {
        if (sent[0] == raw.left[0])
        {
            reverse(sent.begin(), sent.end());
            sent.pop_back();
            reverse(sent.begin(), sent.end());
            sent += " "+newLeft;
            alpha.push_back(sent);
        }
        else
        {
            sent += " "+newLeft;
            beta.push_back(sent);
        }
    }
    alpha.push_back("ε");
    result1.left = raw.left;
    result1.right = beta;
    result2.left = newLeft;
    result2.right = alpha;
}

int main()
{
    vector<sentence> sentences;
    vector<sentence>answer;
    string line;
    while (getline(cin, line))
    {
        if (cin.eof())
            break;
        line = RemoveSpace(line);
        sentences.push_back(sentence(line));
    }
    bool found;
    for (auto it = sentences.begin(); it != sentences.end();it++)
    {
        found = 0;
        for (auto &r : it->right)
        {
            if (it->left.length() == 1 && it->left[0] == r[0])
            {
                sentence s1, s2;
                Eliminate(s1, s2, *it);
                answer.push_back(s1);
                answer.push_back(s2);
                found = 1;
            }
        }
        if (!found)
        {
            answer.push_back(*it);
        }
    }
    sort(answer.begin(),answer.end());
    for(auto &sent:answer)
    {
        cout<<sent.left<<" -> ";
        for(int i=0;i<sent.right.size();i++)
        {
            cout<<sent.right[i];
            if(i!=sent.right.size()-1)
            {
                cout<<" | ";
            }
        }
        cout<<endl;
    }
}