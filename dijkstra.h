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
  Graph(const vector<Road>& roads, int vertex_num):vertex_num_(vertex_num){
    adj_matrix = vector<vector<int>>(vertex_num, vector<int>(vertex_num, INF));
    adj_matrix_reverse = vector<vector<int>>(vertex_num, vector<int>(vertex_num, INF));

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
  int vertex_num_;

  void Dijkstra(const vector<Road>& roads, int start, vector<vector<int>>& cost_matrix, vector<vector<int>>& parent_matrix){
    vector<int> flag(vertex_num_+1, 1);
    cost_matrix[start][start] = 0;
    for(int i=0; i<vertex_num_; i++){
      int u;
      int min_dist = INF;
      for(int j = 0; j<vertex_num_; j++){
        if(flag[j] == 1){
          int cost_startj = cost_matrix[start][j];
          if(min_dist > cost_startj){
            u = j;
            min_dist = cost_startj;
          }
        }
      }
      flag[u] = 0;

      for(int j = 0; j<vertex_num_; j++){
        if(adj_matrix_reverse[u][j] == INF) continue;
        int cost_startu = min_dist;
        int cost_uj = roads[adj_matrix_reverse[u][j]].length;
        int cost_startj =cost_matrix[start][j];
        if(cost_startu+cost_uj < cost_startj){
          cost_matrix[start][j] = cost_startu + cost_uj;
          parent_matrix[u][j] = u;
          int v = u, last_v = u;
          while(u != start && last_v!=start){
            parent_matrix[v][j] = last_v;
            last_v = v;
            v = parent_matrix[start][v];
          }
        }
      }
    }
  }
};
#endif //DIJKSTRA_H_
 