#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <fstream>
#define MAX 507

using namespace std;

struct Node
{
    int x;
    int type;
    Node() {}
    Node(int a, int b)
        :x(a), type(b) {}
    bool operator < (const Node& a) const
    {
        return type < a.type;
    }
};

int mp[MAX][MAX];
vector<int> _edge[MAX];
vector<int> _val[MAX];
vector<int> edge[MAX];
vector<int> val[MAX];
vector<int> point;
vector<pair<int, int> > nodes;
vector<int> change;
vector<Node> col;
int fa[MAX];
int type[MAX];
int state[MAX];
int used[MAX];
int kinds;
int groups_num;
int n;
int m;

void init();
int _find(int x);
void _union(int x, int y);
void _add(int u, int v, int x);
void add(int u, int v, int x);
void minimize();
void make_csv();

int main()
{
    init();
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d", &type[i]);
    getchar();
    for (int i = 0; i < m; i++)
    {
        int u, v;
        char x;
        scanf("%d -- %c -> %d", &u, &x, &v);
        _add(u, v, int(x - 'a'+1));
    }
    minimize();
    printf("points : \n");
    printf("%d\n", (int)point.size());
    for (int i = 0; i < point.size(); i++)
        printf("%d ", point[i]);
    printf("\n");
    printf("edge: \n");
    m = nodes.size();
    for (int i = 0; i < nodes.size(); i++)
        printf("%d %d %c\n", nodes[i].first, nodes[i].second, change[i]+'a'-1);
    puts("");
}

void init()
{
    for (int i = 0; i < MAX; i++)
    {
        edge[i].clear();
        _edge[i].clear();
    }
    point.clear();
    memset(type, 255, sizeof(type));
    memset(used, 0, sizeof(used));
    kinds = 0;
    groups_num = 2;
    col.clear();
    for (int i = 0; i < MAX; i++)
        fa[i] = i;
}

void _add(int u, int v, int x)
{
    _edge[u].push_back(v);
    _val[u].push_back(x);
    if (used[x]) return;
    used[x] = x;
    kinds++;
}

void add(int u, int v, int x)
{
    edge[u].push_back(v);
    val[u].push_back(x);
}

int _find(int x)
{
    return x == fa[x] ? x : fa[x] = _find(fa[x]);
}

void _union(int x, int y)
{
    x = _find(x);
    y = _find(y);
    fa[x] = y;
}

void minimize()
{
    queue<vector<Node> > q[2];
    col.clear();
    for (int i = 1; i <= n; i++)
    {
        if (type[i] == 0)
            col.push_back(Node(i, 0));
    }
    q[1].push(col);
    col.clear();
    for (int i = 1; i <= n; i++)
    {
        if (type[i] == 1)
            col.push_back(Node(i, 1));
    }
    q[1].push(col);
    col.clear();
    for (int i = 1; i <= kinds; i++)
    {
        int cur = i % 2;
        int next = (i + 1) % 2;
        while (!q[next].empty()) q[next].pop();
        while (!q[cur].empty())
        {
            vector<Node> front = q[cur].front();
            q[cur].pop();
            for (int j = 0; j < front.size(); j++)
            {
                Node& temp = front[j];
                int u = temp.x;
                for (int k = 0; k < _edge[u].size(); k++)
                {
                    int v = _edge[u][k];
                    int x = _val[u][k];
                    if (x != i) continue;
                    temp.type = type[v];
                }
            }
            sort(front.begin(), front.end());
            if (front[0].type == front[front.size() - 1].type)
                q[next].push(front);
            else
            {
                col.clear();
                col.push_back(front[0]);
                for (int j = 1; j < front.size(); j++)
                {
                    if (front[j].type != front[j - 1].type)
                    {
                        q[cur].push(col);
                        col.clear();
                        groups_num++;
                    }
                    type[front[j].x] = groups_num;
                    col.push_back(front[j]);
                }
                q[cur].push(col);
            }
        }
    }
    //#define DEBUG
    int id = (kinds + 1) % 2;
    while (!q[id].empty())
    {
        vector<Node> temp = q[id].front();
        q[id].pop();
        for (int i = 1; i < temp.size(); i++)
            _union(temp[i].x, temp[i - 1].x);
    }
    memset(used, 0, sizeof(used));
    memset(mp, 0, sizeof(mp));
    for (int i = 1; i <= n; i++)
    {
        int x = _find(i);
        if (used[x]) continue;
        used[x] = 1;
        point.push_back(x);
    }
    nodes.clear();
    for (int i = 1; i <= n; i++)
        for (int j = 0; j < _edge[i].size(); j++)
        {
            int v = _edge[i][j];
            int x = _val[i][j];
            int a = _find(i);
            int b = _find(v);
            if (mp[a][b]) continue;
            mp[a][b] = 1;
            add(a, b, x);
            nodes.push_back(make_pair(a, b));
            change.push_back(x);
        }
}

//14 42
//0 0 0 1 0 0 1 0 0 1 0 0 0 1
//11 -- a -> 11
//11 -- b -> 11
//11 -- c -> 11
//12 -- a -> 12
//12 -- b -> 13
//12 -- c -> 14
//2 -- a -> 12
//2 -- b -> 13
//2 -- c -> 14
//5 -- a -> 12
//5 -- b -> 13
//5 -- c -> 14
//13 -- a -> 12
//13 -- b -> 13
//13 -- c -> 14
//3 -- a -> 12
//3 -- b -> 13
//3 -- c -> 14
//6 -- a -> 12
//6 -- b -> 13
//6 -- c -> 14
//14 -- a -> 2
//14 -- b -> 3
//14 -- c -> 4
//1 -- a -> 2
//1 -- b -> 3
//1 -- c -> 4
//4 -- a -> 5
//4 -- b -> 6
//4 -- c -> 7
//7 -- a -> 8
//7 -- b -> 9
//7 -- c -> t