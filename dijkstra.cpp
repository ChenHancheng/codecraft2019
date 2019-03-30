#include <queue>
#include <utility>
#include <vector>

#include "codecraft_util.h"
#include "road.h"
#include "dijkstra.h"

using std::vector;
using std::pair;
using std::priority_queue;

void Graph::Dijkstra(const vector<Road>& roads, int start){
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
      if(adj_matrix[u][j] == INF) continue;
      int cost_startu = min_dist;
      int cost_uj = (10*roads[adj_matrix[u][j]].length)/roads[adj_matrix[u][j]].speed_limit;
      int cost_startj =cost_matrix[start][j];
      if(cost_startu+cost_uj < cost_startj){
        cost_matrix[start][j] = cost_startu + cost_uj;
        parent_matrix[u][j] = u;
        int v = u, last_v = u;
        while(u!= start){
          int tmp = start;
          while(tmp != parent_matrix[tmp][v]){
            tmp = parent_matrix[tmp][v];
          }
          parent_matrix[tmp][j] = v;
          v = tmp;
          if(tmp == start) break;
        }
      }
    }
  }
}