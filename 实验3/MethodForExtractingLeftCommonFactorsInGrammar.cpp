#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
struct piece {
    char data;
    vector<piece*>next;
};
void deleteTree(piece* node) {
    if (node == nullptr) return;
    for (auto& child : node->next) {
        deleteTree(child);
    }
    delete node;
}
void showTree(piece* node, string format)
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


int main()
{
    string expr;
    while (getline(cin, expr))
    {
        piece* head = new piece;
        head->data = expr[0];
        piece* cur = head;
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
                    for (auto& it : cur->next)
                    {
                        if (it != NULL && it->data == expr[i])
                        {
                            cur = it;
                            check = true;
                            break;
                        }
                    }
                }
                if (!check)
                {
                    piece* newPiece = new piece;
                    newPiece->data = expr[i];
                    cur->next.push_back(newPiece);
                    cur = newPiece;
                }
            }
        }
        showTree(head, "");
        deleteTree(head);
    }

}