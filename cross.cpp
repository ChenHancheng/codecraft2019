#include <set>
#include <vector>
#include <utility>
#include <algorithm>

#include "cross.h"
#include "car.h"
#include "road.h"
#include "codecraft_util.h"

using std::set;
using std::vector;
using std::pair;

//use static road state to generate an initial value for optimizer
void Cross::InitialValue(vector<Road>& roads, vector<Car>& cars, const vector<vector<int>> cost_matrix){
  vector<priority_queue<int>> road_queue;
  for(int i = 0; i<valid_roads_num; i++){
    road_queue.push_back(roads[dispatch_seq[i]].lane_wait);
  }

  int i=0;
  vector<pair<int, int>> res;
  for(int i = 0; i<valid_roads_num; i++){
    while(roads[dispatch_seq[i]].lane_wait.empty() == false){
      int car_id = roads[dispatch_seq[i]].lane_wait.top();
      int next_road;
      int min_cost = INF;
      for(int j=0; j<valid_roads_num; j++){
        if(j == i) continue;
        int road_id = dispatch_seq[j];
        int cross_id = roads[road_id].start==id?roads[road_id].end:roads[road_id].start;

        int cost = roads[road_id].NewCarTime(car_id) + cost_matrix[cross_id][cars[car_id].end]/cars[car_id].speed;
        if(cost <min_cost){
          next_road = road_id;
          min_cost = cost;
        }
      }
      roads[next_road].AddCar(cars, car_id, 1);
    }
  }
}

void Cross::CalCost(){

}

Cross::Cross(const CrossData& cross_data){
  int road_id0 = cross_data.road_id0;
  int road_id1 = cross_data.road_id1;
  int road_id2 = cross_data.road_id2;
  int road_id3 = cross_data.road_id3;

  if(road_id0 != -1){
    dispatch_seq.push_back(road_id0);
  }
  if(road_id1 != -1){
    dispatch_seq.push_back(road_id1);
  }
  if(road_id2 != -1){
    dispatch_seq.push_back(road_id2);
  }
  if(road_id3 != -1){
    dispatch_seq.push_back(road_id3);
  }

  sort(dispatch_seq.begin(), dispatch_seq.end());
  valid_roads_num = dispatch_seq.size();

  go_straight.insert({road_id0, road_id2});
  go_straight.insert({road_id2, road_id0});
  go_straight.insert({road_id1, road_id3});
  go_straight.insert({road_id3, road_id1});

  turn_left.insert({road_id0, road_id1});
  turn_left.insert({road_id1, road_id2});
  turn_left.insert({road_id2, road_id3});
  turn_left.insert({road_id3, road_id0});
  
  turn_right.insert({road_id0, road_id3});
  turn_right.insert({road_id3, road_id2});
  turn_right.insert({road_id2, road_id1});
  turn_right.insert({road_id1, road_id0});
}

