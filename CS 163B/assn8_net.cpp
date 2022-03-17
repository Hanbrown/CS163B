#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <limits>

#include <vector>
#include <map>
#include <cassert>

#include <set>

#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <cmath>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
////////////////                         Graph
////////////////////////////////////////////////////////////////////////////////

template <class T>
struct Edge
{
    T a, b;

    Edge() {}

    Edge(const T& a, const T& b) : a(a), b(b)
    {}

};

template <class T>
bool operator <(const Edge<T>& e1,
    const Edge<T>& e2)
{

    return ((e1.a < e2.a) || ((e1.a == e2.a) && (e1.b < e2.b)));
}

/*
* This graph class represents graphs as an adjacency list.
* Each element is a tuple consisting of a vertex and all its edges
*/
template <class T>
class graph {

public:

    typedef T Vertex; // Vertices
    typedef std::unordered_multiset<T> VertexSet; // Edges unordered_multiset
    typedef std::vector<T> Path; // Path

    graph() {

    }

    /**
    * Return the number of vertices in the graph
    * NOTE: const prevents function from modifying any private vars
    */
    size_t n() const {
        return _t.size();
    }
    size_t vertex_count() const {
        return n();
    }

    /**
    * Return number of edges in the graph
    * NOTE: With maps, each element is a pair (a, b)
    * .first references "a" and .second references "b"
    */
    size_t m() const {
        std::size_t ans(0); // Alternate way to assign a variable ans = 0
        std::size_t self_loops(0), sl_current(0);

        // Sum up the size of all edge lists in _t
        for (auto p : _t) {

            // Check for self-loops
            sl_current = std::count(p.second.begin(), p.second.end(), p.first);
            self_loops += sl_current;

            ans += p.second.size() - sl_current;

        }

        // Each edge is double counted because
        // if there's an edge (a, b), we saw this edge while
        // going through both vertex "a" and vertex "b".
        // So we divide ans by two to get the number of edges.
        return ((ans / 2) + self_loops);
    }
    size_t edge_count() const {
        return m();
    }

    // Return all vertices
    VertexSet V() const {
        VertexSet ans;

        for (auto p : _t)
            ans.insert(p.first);

        return ans;
    }
    VertexSet vertices() const {
        return V();
    }

    // Is vertex v in this graph?
    bool isVertex(const T& v) const {
        return (_t.count(v) != 0);
    }

    /*
    * NOTE: With maps, [] is different from .at()
    * With [], if the element does not exist, it is created
    * With .at(), if the element does not exist, an error is thrown
    */
    bool isEdge(const T& v, const T& w) const {
        return (isVertex(v) && isVertex(w) && _t.at(v).count(w) >= 1); // >= for self-loops
    }

    // Return neighbors of v
    VertexSet Adj(const T& v) const {
        assert(isVertex(v));
        return _t.at(v);
    }

    std::size_t deg(const T& v) const {
        assert(isVertex(v));
        return _t.at(v).size();
    }

    void addVertex(const T& v) {
        if (isVertex(v))
            return;

        _t[v] = VertexSet();
    }

    void addEdge(const T& v, const T& w) {
        assert(isVertex(v) && isVertex(w));

        // Since we now allow self-loops, prevent double counts:
        if (v == w)
            _t[v].insert(w);
        else {
            _t[v].insert(w);
            _t[w].insert(v);
        }
    }

    void removeEdge(const T& v, const T& w) {
        if (!isEdge(v, w))
            return;

        if (v == w) {
            const auto pos = _t[v].find(w);
            _t[v].erase(pos);
        }
        else {
            auto pos = _t[v].find(w);
            _t[v].erase(pos);

            pos = _t[w].find(v);
            _t[w].erase(pos);
        }
    }

    void removeVertex(const T& v) {
        if (!isVertex(v))
            return;

        // Remove linked edges
        for (auto nbr : Adj(v)) {
            removeEdge(v, nbr);
        }

        // Remove vertex
        _t.erase(v);
    }

    // Clear graph of all nodes
    void clear() {
        _t.clear();
    }


protected:

    // T is a vertex, VertexSet is an edge
    std::unordered_map<Vertex, VertexSet> _t;
};

////////////////////////////////////////////////////////////////////////////////
////////////////                         digraph
////////////////////////////////////////////////////////////////////////////////

template <class T>
class digraph : public graph<T>
{

public:
    typedef T Vertex;
    typedef std::vector<T> Path; // Path

    digraph reverse() const
    {
        digraph ans;

        for (auto& v : graph<Vertex>::V())
            ans.addVertex(v);

        for (auto& v : graph<Vertex>::V())
        {
            for (auto& w : graph<Vertex>::Adj(v))
                ans.addEdge(w, v);
        }

        return ans;
    }

    std::size_t m() const
    {
        std::size_t ans(0);

        for (auto& v : graph<T>::V())
            ans += graph<T>::Adj(v).size();
        return ans;
    }

    std::size_t indeg(const Vertex& v) const
    {
        std::size_t ans(0);

        for (auto& w : graph<T>::V())
        {
            ans += graph<T>::Adj(w).count(v);
        }
        return ans;
    }

    std::size_t outdeg(const Vertex& v) const
    {
        return graph<T>::Adj(v).size();
    }

    void addEdge(const Vertex& v, const Vertex& w)
    {
        assert(graph<T>::isVertex(v) && graph<T>::isVertex(w));
        graph<T>::_t[v].insert(w);
    }

    void removeEdge(const Vertex& v, const Vertex& w)
    {
        if (!graph<T>::isEdge(v, w))
            return;

        graph<T>::_t[v].erase(w);
    }

    // Rudimentary "return source vertices" function. Doesn't work with self-loops and duplicate edges
    Path sources() {
        Path srcs;

        for (auto& v : graph<T>::V()) {
            if (indeg(v) == 0)
                srcs.push_back(v);
        }

        return srcs;
    }

    Path sinks() {
        Path srcs;

        for (auto& v : graph<T>::V()) {
            if (outdeg(v) == 0)
                srcs.push_back(v);
        }

        return srcs;
    }



private:

};

////////////////////////////////////////////////////////////////////////////////
////////////////                         wgraph
////////////////////////////////////////////////////////////////////////////////

template <class T>
struct WEdge : public Edge<T>
{
    double w;

    WEdge();

    WEdge(const T& a, const T& b, double w) : Edge<T>(a, b), w(w)
    {}
};

template <class T>
bool operator < (const WEdge<T>& e1, const WEdge<T>& e2)
{
    return ((e1.w < e2.w) ||
        ((e1.w == e2.w) && (Edge<T>(e1) < Edge<T>(e2))));
}

template <class T>
class wgraph : public graph<T>
{
public:

    typedef T Vertex;

    wgraph()
    {

    }

    void addEdge(const T& a, const T& b, double w)
    {
        graph<T>::addEdge(a, b);
        _w[Edge<T>(a, b)] = _w[Edge<T>(b, a)] = w;
    }

    void addEdge(const Edge<T>& e, double w)
    {
        addEdge(e.a, e.b, w);
    }

    void removeEdge(const T& a, const T& b)
    {
        graph<T>::removeEdge(a, b);
        _w.erase(Edge<T>(a, b));
        _w.erase(Edge<T>(b, a));
    }

    double cost(const T& a, const T& b) const
    {
        return _w.at(Edge<T>(a, b));

    }

    double cost(const Edge<T>& e) const
    {
        return cost(e.a, e.b);
    }

    std::set<WEdge<T>> E() const
    {
        std::set<WEdge<T>> ans;

        for (auto& v : graph<T>::V())
            for (auto& w : graph<T>::Adj(v))
                ans.insert(WEdge<T>(v, w, cost(v, w)));

        return ans;

    }

private:

    std::map<Edge<T>, double> _w;   // maps an edge to its weight



};

////////////////////////////////////////////////////////////////////////////////
////////////////                         Network
////////////////////////////////////////////////////////////////////////////////

template <class T>
class network : public digraph<T>
{

public:

    typedef T Vertex;
    network()
    {

    }

    void addEdge(const T& a, const T& b, double w)
    {
        digraph<T>::addEdge(a, b);
        _w[{a, b}] = w;
    }

    void addEdge(const WEdge<T>& e)
    {
        digraph<T>::addEdge(e.a, e.b);
        _w[{e.a, e.b}] = e.w;
    }

    double cost(const T& a, const T& b) const
    {
        assert(digraph<T>::isEdge(a, b));
        return _w.at({ a, b });
    }

    void setCost(const T& a, const T& b, double w)
    {
        assert(digraph<T>::isEdge(a, b));
        _w[{a, b}] = w;

    }

    std::set<WEdge<T>> E() const
    {
        std::set<WEdge<T>> ans;

        for (auto& a : digraph<T>::V())
            for (auto& b : digraph<T>::Adj(a))
                ans.insert({ a, b, cost(a, b) });
    }


    network<Vertex> Bellman_Ford(const Vertex& s) const
    {

        network<Vertex> ans;

        std::unordered_map<Vertex, double> d, D;  // old and new distances from s to the other vertices

        std::unordered_map<Vertex, Vertex> parent;

        bool nwc(false);   // negative-weight cycle

        for (auto& v : digraph<Vertex>::V())
        {
            d[v] = std::numeric_limits<double>::infinity();
        }
        d[s] = 0.0;

        for (std::size_t i = 1; i <= digraph<Vertex>::n(); ++i)
        {

            D = d;
            for (auto& e : E())
            {
                double temp = d[e.a] + e.w;

                if (temp < D[e.b])
                {
                    D[e.b] = temp;
                    parent[e.b] = e.a;

                    if (i == digraph<Vertex>::n())
                        nwc = true;
                }
            }
            d = D;
        }

        for (auto& v : digraph<Vertex>::V())
            ans.digraph<Vertex>::addVertex(v);

        if (!nwc)
            for (auto& p : parent)
                ans.addEdge(p.second, p.first, cost(p.second, p.first));

        return ans;


    }

private:
    std::map<Edge<T>, double> _w;

};

////////////////////////////////////////////////////////////////////////////////
////////////////                         Dary
////////////////////////////////////////////////////////////////////////////////

template <class T>
class dary_heap
{
public:

    dary_heap(std::size_t d = 2) : _d(d)
    {
        _n = 0;
    }

    std::size_t size() const
    {
        return _n;
    }

    T min() const
    {
        assert(_n != 0);
        return _data[0];
    }

    bool empty() const
    {
        return (_n == 0);
    }

    void push(const T& x)
    {

        assert(_l.count(x) == 0);   //  x is not already in heap
        if (_n == _data.size())     //  vector is full
            _data.push_back(x);
        else
            _data[_n] = x;


        // record where x lives in _data and increment the key count
        _l[x] = _n;
        ++_n;

        std::size_t i = _l.at(x);
        while (i > 0)
        {
            std::size_t parent = (i - 1) / _d;
            if (_data[i].second < _data[parent].second)
            {
                swap(_data[i], _data[parent]);
                _l[_data[i]] = i;
                _l[_data[parent]] = parent;
                i = parent;

            }
            else
                break;
        }

    }

    void decrease_key(const T& x, const T& newx)
    {

        // new key is smaller than old key;  old key is in heap; new key is not yet in heap
        assert(newx.second < x.second);
        assert(_l.count(x) != 0);
        assert(_l.count(newx) == 0);

        std::size_t i = _l.at(x);
        _data[i] = newx;
        _l.erase(x);
        _l[newx] = i;


        while (i > 0 && _data[i].second < _data[(i - 1) / _d].second)
        {
            std::size_t parent = (i - 1) / _d;

            swap(_data[i], _data[parent]);
            _l[_data[parent]] = parent;
            _l[_data[i]] = i;
            i = parent;


        }

    }


    void pop_min()
    {

        assert(!empty());

        _l.erase(_data[0]);
        if (--_n == 0)
            return;

        _data[0] = _data[_n];
        _l[_data[0]] = 0;
        std::size_t i = 0;

        while (i * _d + 1 < _n)      //  there is at least one child
        {
            std::size_t left(i * _d + 1), right((i + 1) * _d), m(left);

            for (std::size_t c = left; c < _n && c < right; ++c)
                if (_data[c] < _data[m])
                    m = c;


            // if smallest child is greater or equal to parent, then heap order has been restored
            if (!(_data[m] < _data[i]))
                break;

            // otherwise swap parent and minimum child
            swap(_data[i], _data[m]);
            _l[_data[i]] = i;
            _l[_data[m]] = m;
            i = m;
        }
    }

private:

    std::size_t _n;                   // number of elements in heap
    std::size_t _d;                   // number of children
    std::vector<T> _data;             // heap elements live here
    std::map<T, std::size_t> _l;      // _data[_l[key]] = key





};

////////////////////////////////////////////////////////////////////////////////
////////////////                         Main
////////////////////////////////////////////////////////////////////////////////

typedef pair<int, double> DVERT;
double dijkstra(network<int> G, int src, int T) {
    // Consider point cloud edge case for d-ary heaps and Dijkstra
    size_t nc = G.m() / G.n();

    dary_heap<DVERT> Q(nc);

    map<int, double> dist;

    dist[src] = 0;

    for (auto& v : G.vertices()) {

        DVERT c(v, 0);

        if (v != src) {
            c.second = numeric_limits<double>::infinity();
            dist[v] = numeric_limits<double>::infinity();
        }

        Q.push(c);
    }

    while (!Q.empty()) {
        DVERT min_vert = Q.min();
        int u = min_vert.first;
        Q.pop_min();

        if (u == T)
            return min_vert.second;

        for (auto& v : G.Adj(u)) {
            double alt = dist.at(u) + G.cost(u, v);

            if (alt < dist.at(v)) {
                double cost = dist.at(v);
                dist.at(v) = alt;
                //prev.at(v) = u;

                Q.decrease_key(DVERT(v, cost), DVERT(v, alt));
            }
        }
    }

    return numeric_limits<double>::infinity();

}

int main()
{

    string output = "";

    int N;
    int n, m, S, T;

    int s, t;
    double w;

    cin >> N;

    for (int i = 0; i < N; i++) {
        output += "Case #" + to_string(i+1) + ": ";

        network<int> G;

        cin >> n;
        cin >> m;
        cin >> S;
        cin >> T;

        if (m == 0) {
            output += "unreachable\n";
            continue;
        }

        for (int j = 0; j < n; j++) {
            G.addVertex(j);
        }
        for (int j = 0; j < m; j++) {
            cin >> s;
            cin >> t;
            cin >> w;

            G.addEdge(s, t, w);
            G.addEdge(t, s, w);
        }

        int cost = (int)dijkstra(G, S, T);
        output += ((cost == numeric_limits<double>::infinity()) ? "unreachable" : to_string(cost)) + "\n";
    }

    cout << output;

}