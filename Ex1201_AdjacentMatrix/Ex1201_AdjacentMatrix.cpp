#include <iostream>
#include <cassert>

#include "../shared/Stack.h"
#include "../shared/Queue.h"

using namespace std;

template<typename T>
class AdjMatGraph // Adjacent Matrix Graph
{
public:
	struct Vertex // Node
	{
		T item = T();
	};

	AdjMatGraph(int max_vertices)
	{
		max_vertices_ = max_vertices; // capacity

		matrix_ = new int* [max_vertices_];
		for (int r = 0; r < max_vertices_; r++) {
			matrix_[r] = new int[max_vertices_];
			for (int c = 0; c < max_vertices_; c++)
				matrix_[r][c] = 0;
		}

		vertices_ = new Vertex[max_vertices_];

		n_ = 0;
	}

	~AdjMatGraph()
	{
		delete[] vertices_;

		for (int i = 0; i < max_vertices_; i++)
			delete[] matrix_[i];
		delete[] matrix_;

		if (visited_)
			delete[] visited_;
	}

	void PrintMatrix()
	{
		using namespace std;
		if (n_)
			for (int r = 0; r < n_; r++) {
				for (int c = 0; c < n_; c++)
					cout << matrix_[r][c] << ", ";
				cout << endl;
			}
		else
			cout << "Empty" << endl;

		cout << endl;
	}

	void InsertVertex(T item)
	{
		vertices_[n_].item = item;
		n_++;
	}

	void InsertEdge(int u, int v) // 여기서 u, v는 인덱스
	{
		assert(u < n_ && v < n_);

		matrix_[u][v] = 1;
	}

	int Capacity()
	{
		return max_vertices_;
	}

	void DepthFirstTraversal()
	{
		ResetVisited();

		DepthFirstTraversal(0);
		// IterDFT();

		cout << endl;
	}

	void DepthFirstTraversal(int vertexIdx) // v는 인덱스
	{
		// 방문 처리
		visited_[vertexIdx] = true;
		std::cout << vertices_[vertexIdx].item << " ";

		// 방문한 적 없는 인접한 정점 dfs
		for (int adjacentIdx = 0; adjacentIdx < max_vertices_; ++adjacentIdx)
		{
			if ((matrix_[vertexIdx][adjacentIdx] == 1) && 
				(visited_[adjacentIdx] == false))
			{
				DepthFirstTraversal(adjacentIdx);
			}
		}
	}

	void IterDFT()
	{
		Stack<int> vertexIndices;
		vertexIndices.Push(0);

		// 스택에 정점 인덱스가 남아있으면 반복
		while (vertexIndices.IsEmpty() == false)
		{
			// 스택에서 정점 인덱스 꺼낸다
			const int vertexIdx = vertexIndices.Top();
			vertexIndices.Pop();

			// 방문한 적 없는 정점이면
			if (visited_[vertexIdx] == false)
			{
				// 정점 방문 처리
				visited_[vertexIdx] = true;
				std::cout << vertices_[vertexIdx].item << " ";

				// 인접한 정점의 인덱스를 스택에 넣는다
				for (int adjacentIdx = 0; adjacentIdx < max_vertices_; ++adjacentIdx)
				{
					if (matrix_[vertexIdx][adjacentIdx] == 1)
					{
						vertexIndices.Push(adjacentIdx);
					}
				}
			}
		}
	}

	void BreadthFirstTraversal()
	{
		ResetVisited();

		Queue<int> vertexIndices;
		vertexIndices.Enqueue(0);
		visited_[0] = true;

		// 큐에 정점 인덱스가 남아있으면 반복
		while (vertexIndices.IsEmpty() == false)
		{
			// 큐에서 정점 인덱스를 꺼낸다
			const int visitedIndex = vertexIndices.Front();
			vertexIndices.Dequeue();

			std::cout << vertices_[visitedIndex].item << " ";

			// 방문한 적 없는 정점 인덱스를 큐에 넣는다
			for (int adjacentIdx = 0; adjacentIdx < max_vertices_; ++adjacentIdx)
			{
				if ((matrix_[visitedIndex][adjacentIdx] == 1) &&
					(visited_[adjacentIdx] == false))
				{
					vertexIndices.Enqueue(adjacentIdx);
					visited_[adjacentIdx] = true;
				}
			}
		}
	}

	void ResetVisited()
	{
		if (!visited_)
			visited_ = new bool[max_vertices_];
		for (int i = 0; i < max_vertices_; i++) visited_[i] = false; // 초기화
	}

private:
	int** matrix_ = nullptr;
	Vertex* vertices_ = nullptr;
	int n_ = 0;					// size
	int max_vertices_ = 0;		// capacity

	bool* visited_ = nullptr;
};

int main()
{
	// 정점(vertex), 간선(edge)

	AdjMatGraph<int> g(7);

	for (int i = 0; i < g.Capacity(); i++)
		g.InsertVertex(i);

	
	// 위키피디아 버전
	g.InsertEdge(0, 2);	g.InsertEdge(2, 0);
	g.InsertEdge(0, 1);	g.InsertEdge(1, 0);
	g.InsertEdge(1, 4);	g.InsertEdge(4, 1);
	g.InsertEdge(1, 3);	g.InsertEdge(3, 1);
	g.InsertEdge(2, 4);	g.InsertEdge(4, 2);
	g.InsertEdge(3, 6);	g.InsertEdge(6, 3);
	g.InsertEdge(4, 6);	g.InsertEdge(6, 4);
	g.InsertEdge(5, 6);	g.InsertEdge(6, 5);
	g.InsertEdge(3, 0);	g.InsertEdge(0, 3);
	g.InsertEdge(4, 0);	g.InsertEdge(0, 4);
	

	// 강의 영상 버전
	/*g.InsertEdge(0, 2);	g.InsertEdge(2, 0);
	g.InsertEdge(0, 1);	g.InsertEdge(1, 0);
	g.InsertEdge(1, 4);	g.InsertEdge(4, 1);
	g.InsertEdge(1, 3);	g.InsertEdge(3, 1);
	g.InsertEdge(2, 4);	g.InsertEdge(4, 2);
	g.InsertEdge(3, 6);	g.InsertEdge(6, 3);
	g.InsertEdge(4, 6);	g.InsertEdge(6, 4);
	g.InsertEdge(5, 6);	g.InsertEdge(6, 5);*/

	g.PrintMatrix();

	g.DepthFirstTraversal();

	g.BreadthFirstTraversal();

	return 0;
}