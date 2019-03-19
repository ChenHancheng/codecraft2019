#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include <queue>
#include <utility>
#include <vector>

#include "codecraft_util.h"
#include "road.h"

using std::vector;
using std::pair;
using std::priority_queue;

class Graph{
 public:
  Graph(const vector<Road>& roads, int vertex_num){
    vertex_num_ = vertex_num;
    adj_matrix = vector<vector<int>>(vertex_num, vector<int>(vertex_num, INF));
    adj_matrix_reverse = vector<vector<int>>(vertex_num, vector<int>(vertex_num, INF));
    cost_matrix = vector<vector<int>>(vertex_num, vector<int>(vertex_num, INF));
    parent_matrix = vector<vector<int>>(vertex_num, vector<int>(vertex_num, INF));

    for(int i=0; i<roads.size(); i++){
      adj_matrix_reverse[roads[i].end][roads[i].start] = roads[i].id;
      adj_matrix[roads[i].start][roads[i].end] = roads[i].id;
      if(roads[i].bidirectional == true){
        adj_matrix_reverse[roads[i].start][roads[i].end] = roads[i].id;
        adj_matrix[roads[i].end][roads[i].start] = roads[i].id;
      }
    }
  }
  int vertex_num()const { return vertex_num_; }

  vector<vector<int>> adj_matrix;
  vector<vector<int>> adj_matrix_reverse;
  vector<vector<int>> cost_matrix;
  vector<vector<int>> parent_matrix;
  int vertex_num_;

  void Dijkstra(const vector<Road>& roads, int start);
};
#endif //DIJKSTRA_H_
 