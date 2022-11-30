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

struct myCmp
{
    const bool operator()(const pair<int, int> &a, const pair<int, int> &b) const
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

void kCoreDecomp(vector<vector<int>> &hyperEdge, unordered_map<int, vector<int>> &hyperNode, unordered_map<int, int> &core)
{
    core.clear();
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
        core[p.first] = K;
        visitNode[p.first] = true;
        for (auto &edge : hyperNode[p.first])
        {
            if (visitEdge[edge])
                continue;
            visitEdge[edge] = true;
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

void solve(const string &str)
{
    cout<<str<<" starts!"<<endl;
    string filepath = "";
    string filename = filepath + str;
    vector<vector<int>> hyperEdge;
    unordered_map<int, vector<int>> hyperNode;
    unordered_map<int, int> core;
    getGraph(filename, hyperEdge, hyperNode);
    auto t1 = std::chrono::steady_clock::now();
    kCoreDecomp(hyperEdge, hyperNode, core);
    auto t2 = std::chrono::steady_clock::now();
    double dr_ns2 = std::chrono::duration<double, std::nano>(t2 - t1).count();
    vector<int> coreE(int(hyperEdge.size()), 0);
    for (int i = 0; i < int(hyperEdge.size()); i++)
    {
        int mink = INT_MAX;
        for (auto node : hyperEdge[i])
            mink = min(mink, core[node]);
        coreE[i] = mink;
    }

    ofstream fout("./result/" + str + "-kcoreEdgeValue");
    for (auto &it : coreE)
    {
        fout << it << endl;
    }
    fout.close();

    fout.open("./result/" + str + "-kcoreVertexValue");
    for (auto &info : core)
    {
        fout << info.first << ' ' << info.second << endl;
    }
    cout << str << endl;
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
    cout << "all finish!" << endl;
}
// -pthread -std=c++11
