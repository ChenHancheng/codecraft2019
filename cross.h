#ifndef CROSS_H_
#define CROSS_H_

#include <set>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>
#include <queue>

#include "car.h"
#include "road.h"
#include "data_type.h"

#define GO_STRAIGHT 0
#define TURN_LEFT 1
#define TURN_RIGHT 2

using std::map;
using std::set;
using std::vector;
using std::pair;
using std::queue;

class Cross{
 public:
  const int id;
  Cross() = default;
  Cross(const CrossData& cross_data);

  vector<int> dispatch_seq;
  map<pair<int, int>, int> turn_direction;//eg:turn_direction[{road_id0, road_id1}] returns TRUN_LEFT
  map<pair<int, int>, int> turn_road;//eg:turn_road[{road_id0, TURN_LEFT}] returns road_id1
  // map<pair<int, int>> road_map;
  // set<pair<int, int>> turn_left;
  // set<pair<int, int>> turn_right;
  int valid_roads_num;
  map<int, queue<int>> road_queue;

  void CalCost();
  void InitialValue(vector<Road>& roads, vector<Car>& cars, const vector<vector<int>> cost_matrix);
  int CheckTrueWait(vector<Road>& roads, vector<Car>& cars, int car_id){
    int s1 = cars[car_id].pos;
    int v2 = roads[cars[car_id].next_road_id].speed_limit;
    if(s1>=v2){
      return false;
    }
    else{
      return true;
    }
  }
  int UpdateCar(vector<Road>& roads, vector<Car>& cars, int car_id){
    int update_res;
    if(cars[car_id].state == IN_GARAGE){
      int next_road_id = cars[car_id].next_road_id;
      update_res = roads[next_road_id].AddCar(cars, car_id, cars[car_id].direction);
    }
    else if(cars[car_id].state == WAIT){
      int cur_road_id = cars[car_id].current_road_id;
      int next_road_id = cars[car_id].next_road_id;
      update_res = roads[next_road_id].AddCar(cars, car_id, cars[car_id].direction);
      roads[cur_road_id].DeleteCar(cars, car_id);
    }
    cars[car_id].state = STOP;
    return update_res;
  }
  
};

#endif //CROSS_H_
