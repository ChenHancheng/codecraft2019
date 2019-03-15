#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include <queue>
#include <utility>
#include <vector>

#include "graph.h"

#define INF 0x3f3f3f3f

using std::vector;
using std::pair;
using std::priority_queue;
struct Vertex
{
  int cross_id;
  int cost;
};


class Graph{
 public:
  Graph(const vector<Road>& roads, int vertex_num):vertex_num_(vertex_num){
    adj_list.resize(vertex_num+1);
    Vertex tmp;
    for(int i=0; i<roads.size(); i++){
      tmp.cross_id = roads[i].start;
      tmp.cost = roads[i].length;
      adj_list[roads[i].end].push_back(tmp);
      if(roads[i].bidirectional == true){
        tmp.cross_id = roads[i].end;
        adj_list[roads[i].start].push_back(tmp);
      }
    }
  }
  int vertex_num()const { return vertex_num_; }
  vector<vector<Vertex>> adj_list;
  int vertex_num_;
};

void Dijkstra(const Graph& graph, int start, vector<vector<int>>& cost_matrix, vector<vector<int>>& parent_matrix){
  int vertex_num = graph.vertex_num();


  cost_matrix[start][start] = 0;

  vector<int> flag(vertex_num+1, 1);
  for(int i=1; i<=vertex_num; i++){
    int u;
    int min_dist = INF;
    for(int j = 1; j<=vertex_num; j++){
      if(flag[j] == 1){
        if(min_dist > cost_matrix[start][j]){
          u = j;
          min_dist = cost_matrix[start][j];
          
        }
      }
    }
    flag[u] = 0;

    for(int j = 0; j<graph.adj_list[u].size(); j++){
      int v = graph.adj_list[u][j].cross_id;
      if(graph.adj_list[u][j].cost + cost_matrix[start][u] < cost_matrix[start][v]){
        cost_matrix[start][v] = graph.adj_list[u][j].cost + cost_matrix[start][u];
        parent_matrix[start][v] = u;
      }
    }
  }
}
#endif //DIJKSTRA_H_
