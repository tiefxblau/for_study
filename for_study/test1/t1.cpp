#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cassert>

using namespace std;

template <class W>
struct Edge
{
	size_t _src;
	size_t _dst;
	W _weight;
	Edge* _next;

	Edge(size_t src, size_t dst, W weight)
		: _src(src)
		, _dst(dst)
		, _weight(weight)
		, _next(nullptr){}
};

template <class V, class W>
class Graph
{
	typedef Edge<W> Edge;
public:
	Graph()
		: _vertexNum(0){}

	void AddVertex(const V v)
	{
		_indexMap.insert(make_pair(v, _vertexNum));

		_vertex.push_back(v);
		++_vertexNum;
		_linkTable.push_back(nullptr);
	}
	void AddEdge(const V vsrc, const V vdst, W weight = W())
	{
		int src = GetVertexIndex(vsrc);
		int dst = GetVertexIndex(vdst);
		assert(src != -1 && dst != -1);

		Edge* newEdge = new Edge(src, dst, weight);
		if (_linkTable[src] == nullptr)
		{
			_linkTable[src] = newEdge;
		}
		else
		{
			newEdge->_next = _linkTable[src];
			_linkTable[src] = newEdge;
		}
	}
	bool ExistPath(const V vsrc, const V vdst) const
	{
		int src = GetVertexIndex(vsrc);
		int dst = GetVertexIndex(vdst);
		if (src == -1 || dst == -1)
			return false;

		return DFS(src, dst);
	}
private:
	bool DFS(const int src, const int dst) const
	{
		if (src == dst)
			return true;

		Edge* cur = _linkTable[src];
		while (cur)
		{
			if (cur->_dst != src)
			{
				if (DFS(cur->_dst, dst))
				{
					return true;
				}
			}
		}

		return false;
	}

	int GetVertexIndex(const V& v) const
	{
		typename unordered_map<V, size_t>::const_iterator it = _indexMap.find(v);
		//auto it = _indexMap.find(v);
		if (it != _indexMap.end())
			return it->second;
		else
			return -1;
	}
private:
	vector<Edge*> _linkTable;
	vector<V> _vertex;
	size_t _vertexNum;

	unordered_map<V, size_t> _indexMap;
};

int main()
{
	int n, m;
	cin >> n >> m;
	Graph<int, int> g;

	while (n--)
	{
		int vertex;
		cin >> vertex;
		g.AddVertex(vertex);
	}
	while (m--)
	{
		int src, dst;
		cin >> src >> dst;
		g.AddEdge(src, dst);
	}

	int start, end;
	cin >> start >> end;

	bool ans = g.ExistPath(start, end);
	if (ans)
		cout << "yes" << endl;
	else
		cout << "no" << endl;

	return 0;
}