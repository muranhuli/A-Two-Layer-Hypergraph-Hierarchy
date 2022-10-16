#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <chrono>
#include <map>
#include <climits>
#include <algorithm>
#include <random>
#include <thread>
using namespace std;

struct kcinfo
{
    int k;
    int c;
};

struct edgeinfo
{
    int k;
    int index;
    edgeinfo(int _k, int _index)
    {
        k = _k;
        index = _index;
    }
};

struct myCmp
{
    bool operator()(const pair<int, int> &a, const pair<int, int> &b)
    {
        if (a.first == b.first)
            return false;
        else
        {
            if (a.second != b.second)
                return a.second < b.second;
            else
                return a.first < b.first;
        }
    }
};

/**
 * @brief Get the Graph object 读取一个超图的超边集和超点集
 *
 * @param filename 读取图的文件
 * @param hyperEdge 超边集，存储每一条边对应的点
 * @param hyperNode 超点集，存储每一个点对应的边
 */
void getGraph(const string &filename, vector<vector<int>> &hyperEdge, unordered_map<int, vector<int>> &hyperNode)
{
    ifstream fin(filename, ios::in);
    int count = -1;
    while (true)
    {
        string str;
        getline(fin, str);
        if (str == "")
            break;
        istringstream ss(str);
        int tmp;
        vector<int> e;
        while (ss >> tmp)
        {
            if (find(e.begin(), e.end(), tmp) == e.end())
                e.push_back(tmp);
        }
        if (e.size() == 1)
            continue;
        count++;
        hyperEdge.push_back(e);
        for (auto &node : e)
            hyperNode[node].push_back(count);
    }
}

void kCoreDecomp(vector<vector<int>> &hyperEdge, unordered_map<int, vector<int>> &hyperNode, unordered_map<int, kcinfo> &core, vector<int> &removeEdgeIndex)
{
    core.clear();
    removeEdgeIndex.clear();
    set<pair<int, int>, myCmp> node_count;
    unordered_map<int, int> deg;
    vector<bool> visitEdge(hyperEdge.size(), false);
    unordered_map<int, bool> visitNode;
    for (auto &it : hyperNode)
    {
        deg[it.first] = it.second.size();
        node_count.insert(make_pair(it.first, deg[it.first]));
        visitNode[it.first] = false;
    }
    int K = 0;
    while (!node_count.empty())
    {
        pair<int, int> p = *node_count.begin();
        node_count.erase(node_count.begin());
        K = max(K, p.second);
        core[p.first].k = K;
        visitNode[p.first] = true;
        for (auto &edge : hyperNode[p.first])
        {
            if (visitEdge[edge])
                continue;
            visitEdge[edge] = true;
            removeEdgeIndex.emplace_back(edge);
            for (auto &node : hyperEdge[edge])
            {
                if (node == p.first)
                    continue;
                node_count.erase(make_pair(node, deg[node]));
                deg[node]--;
                node_count.insert(make_pair(node, deg[node]));
            }
        }
    }
}

void computeMaxK(const unordered_map<int, kcinfo> &core, set<int> &maxK, unordered_map<int, int> &knum)
{
    for (auto it : core)
    {
        knum[it.second.k]++;
    }
    for (auto it : knum)
        maxK.insert(it.first);
}

bool judgeEmpty(const vector<bool> &NotExistEdge)
{
    for (auto i = 0; i < int(NotExistEdge.size()); i++)
    {
        if (!NotExistEdge[i])
            return false;
    }
    return true;
}

int findEdge(const vector<vector<int>> &hyperEdge, const vector<int> &removeEdgeIndex, const vector<bool> &NotExistEdge)
{
    int minC = INT_MAX;
    int index = -1;
    for (int i = int(removeEdgeIndex.size()) - 1; i >= 0; i--)
    {
        int edge = removeEdgeIndex[i];
        if (!NotExistEdge[edge])
        {
            if (minC > int(hyperEdge[edge].size()))
            {
                minC = int(hyperEdge[edge].size());
                index = edge;
            }
        }
    }
    return index;
}

int average(const vector<vector<int>> &hyperEdge, const vector<bool> &NotExistEdge)
{
    int len = 0;
    int count = 0;
    for (int i = 0; i < int(hyperEdge.size()); i++)
    {
        if (!NotExistEdge[i])
        {
            len += int(hyperEdge[i].size());
            count++;
        }
    }
    return len / count;
}

void kcCoreDecomp(const vector<vector<int>> &hyperEdge, const unordered_map<int, vector<int>> &hyperNode, unordered_map<int, kcinfo> &core, unordered_map<int, int> &coree,
                  const vector<int> &removeEdgeIndex, vector<bool> &visit, int k)
{
    int num = 0;
    for (int i = 0; i < int(visit.size()); i++)
        if (!visit[i])
            num++;
    while (num > 0)
    {
        int edge = findEdge(hyperEdge, removeEdgeIndex, visit);
        int minValue = int(hyperEdge[edge].size());
        int aveValue = average(hyperEdge, visit);
        queue<int> q;
        for (auto node : hyperEdge[edge])
        {
            q.emplace(node);
        }
        visit[edge] = true;
        coree[edge] = minValue;
        // coree[edge] = aveValue;
        num--;
        unordered_set<int> tmp;
        while (!q.empty())
        {
            int v = q.front();
            q.pop();
            //判断点是否满足kcore的定义
            int count = 0;
            for (auto edge : hyperNode.at(v))
            {
                if (!visit[edge])
                    count++;
            }
            if (count >= k)
                continue;
            if (tmp.find(v) != tmp.end())
                continue;
            tmp.insert(v);
            for (auto edge : hyperNode.at(v))
            {
                if (visit[edge])
                    continue;
                visit[edge] = true;
                coree[edge] = minValue;
                // coree[edge] = aveValue;
                num--;
                for (auto node : hyperEdge[edge])
                {
                    if (node != v)
                        q.emplace(node);
                }
            }
        }
    }
}

int find(int x, const unordered_map<int, int> &fa)
{
    if (fa.at(x) == x)
        return x;
    else
        return find(fa.at(x), fa);
}

void merge(int i, int j, unordered_map<int, int> &fa)
{
    fa[i] = find(j, fa);
}

void findKEdge(vector<vector<int>> &hyperEdge, unordered_map<int, vector<int>> &hyperNode, unordered_map<int, kcinfo> &core, vector<int> &coreE, vector<int> &visitEdge, int k)
{
    unordered_map<int, int> fa;
    for (int i = 0; i < int(coreE.size()); i++)
    {
        if (coreE[i] >= k)
        {
            for (auto node : hyperEdge[i])
                fa[node] = node;
        }
    }

    for (int i = 0; i < int(coreE.size()); i++)
    {
        if (coreE[i] >= k)
        {
            for (int j = 0; j < int(hyperEdge[i].size()) - 1; j++)
            {
                if (fa[hyperEdge[i][j]] >= fa[hyperEdge[i][j + 1]])
                {
                    merge(hyperEdge[i][j], hyperEdge[i][j + 1], fa);
                }
                else
                {
                    merge(hyperEdge[i][j + 1], hyperEdge[i][j], fa);
                }
            }
        }
    }

    for (int i = 0; i < int(coreE.size()); i++)
    {
        if (coreE[i] >= k)
        {
            visitEdge[i] = fa[hyperEdge[i][0]];
        }
    }
}

void solve(const string &str)
{
    string filepath = "";
    string filename = filepath + str;
    vector<vector<int>> hyperEdge;
    unordered_map<int, vector<int>> hyperNode;
    unordered_map<int, kcinfo> core;
    vector<int> removeEdgeIndex;
    getGraph(filename, hyperEdge, hyperNode);
    kCoreDecomp(hyperEdge, hyperNode, core, removeEdgeIndex);
    ostringstream ss;
    ss << str << "  kcore finished!" << endl;
    cout << ss.str();
    //求出边的core值
    vector<int> coreE(int(hyperEdge.size()), 0);
    unordered_set<int> s;
    for (auto i = 0; i < int(hyperEdge.size()); i++)
    {
        int mink = INT_MAX;
        for (auto node : hyperEdge[i])
        {
            s.insert(core[node].k);
            mink = min(mink, core[node].k);
        }
        coreE[i] = mink;
    }
    set<int> t;
    for (auto k : s)
        t.insert(k);
    string filepath1 = "./result/statistic/" + str + "-statistic";
    ofstream fout(filepath1);
    fout << "k值 时间 k-core子图个数 (k,s)种类 边数 点数" << endl;
    for (auto k : t)
    {
        auto t1 = std::chrono::steady_clock::now();
        vector<int> visitEdge(int(hyperEdge.size()), 0);
        findKEdge(hyperEdge, hyperNode, core, coreE, visitEdge, k);
        unordered_set<int> ss;
        for (int i = 0; i < int(visitEdge.size()); i++)
        {
            ss.insert(visitEdge[i]);
        }
        ss.erase(0);
        int index = 0;
        unordered_set<int> s1;
        unordered_set<int> s2;
        unordered_set<int> s3;
        map<int, int> num;
        for (auto it : ss)
        {
            vector<bool> visit(int(hyperEdge.size()), 0);
            for (int i = 0; i < int(visitEdge.size()); i++)
            {
                if (visitEdge[i] != it)
                    visit[i] = true;
            }
            unordered_map<int, int> coree;
            kcCoreDecomp(hyperEdge, hyperNode, core, coree, removeEdgeIndex, visit, k);
            for (auto it : coree)
            {
                s1.insert(it.first);
                for (auto node : hyperEdge[it.first])
                    s2.insert(node);
                s3.insert(it.second);
            }
        }
        auto t2 = std::chrono::steady_clock::now();
        double dr_ns2 = std::chrono::duration<double, std::nano>(t2 - t1).count();
        fout << k << " " << dr_ns2 << ' ' << ss.size() << ' ' << s3.size() << " " << s1.size() << " " << s2.size() << endl;
    }
    ss.str("");
    ss << "pid=" << std::this_thread::get_id() << ' ' << str << " finish!" << endl;
    cout << ss.str();
}
int main()
{
    string file = "CoDB CoGe";
    istringstream ss(file);
    string str;
    std::vector<std::thread> thread_vec;
    while (ss >> str)
    {
        thread_vec.emplace_back(std::thread(solve, str));
    }
    for (auto &it : thread_vec)
    {
        it.join();
    }
    cout << "全部完成" << endl;
    return 0;
}