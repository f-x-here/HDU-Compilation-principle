#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
using namespace std;
struct piece
{
    string data;
    vector<piece *> next;
};
void deleteTree(piece *node)
{
    if (node == nullptr)
        return;
    for (auto &child : node->next)
    {
        deleteTree(child);
    }
    delete node;
}
void showTree(piece *node, string format)
{
    for (int i = 0; i < format.length(); i++)
    {
        if (format[i] == '0')
            cout << "  ";
        else
            cout << "| ";
    }
    if (format != "")
    {
        cout << "|-";
    }
    cout << node->data << endl;
    for (int i = 0; i < node->next.size(); i++)
    {
        if (i == node->next.size() - 1)
        {
            showTree(node->next[i], format + "0");
        }
        else
        {
            showTree(node->next[i], format + "1");
        }
    }
}

void ExtractingLeftCommonFactors(piece *head)
{
    piece *cur, *next;
    cur = head;
    queue<piece *> q;
    q.push(cur);
    int i = 0, k = 0;
    while (!q.empty())
    {
        cur = q.front();
        q.pop();
        cout << "(" <<head->data;
        for (int j = 0; j < i; j++)
        {
            cout << "'";
        }
        cout << ") -> ";
        for (int l = 0 ;l<cur->next.size();l++)
        {
            next = cur->next[l];
            while (next->next.size() == 1)
            {
                cout << next->data;
                next = next->next[0];
            }
            if (next->next.size() > 1)
            {
                cout << next->data;
                q.push(next);
                k++;
                cout << "(" << head->data;
                for (int j = 0; j < k; j++)
                {
                    cout << "'";
                }
                cout<<")";
            }
            else
            {
                cout << next->data;
            }
            if(l!=cur->next.size()-1)
            cout<<" | ";
        }
        cout<<endl;
        i++;
    }
}

int main()
{
    string expr;
    vector<piece *> heads;
    while (getline(cin, expr))
    {
        piece *head = new piece;
        head->data = expr[0];
        piece *cur = head;
        bool check;
        for (int i = 1; i < expr.length(); i++)
        {
            if (expr[i] == '|')
            {
                cur = head;
            }
            else if (isalpha(expr[i]))
            {
                check = false;
                if (!cur->next.empty())
                {
                    for (auto &it : cur->next)
                    {
                        if (it != NULL && it->data[0] == expr[i])
                        {
                            cur = it;
                            check = true;
                            break;
                        }
                    }
                }
                if (!check)
                {
                    piece *newPiece = new piece;
                    newPiece->data = expr[i];
                    cur->next.push_back(newPiece);
                    cur = newPiece;
                }
            }
        }
        heads.push_back(head);
    }
    for(auto &h:heads)
    {
        cout<<endl;
        showTree(h, "");
        ExtractingLeftCommonFactors(h);
        cout<<endl;
    }
}
// S -> apple | apply | application | battle | bat | brother | bX