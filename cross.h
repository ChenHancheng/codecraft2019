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
  static int reached_cars;
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
  // int CheckTrueWait(vector<Road>& roads, vector<Car>& cars, int car_id){
  
  // }
  int UpdateCar(vector<Road>& roads, vector<Car>& cars, int car_id){
        bool launch_flag = false;
    int update_res;
    if(cars[car_id].state == IN_GARAGE){
      int next_road_id = cars[car_id].next_road_id;
      update_res = roads[next_road_id].AddCar(cars, car_id, cars[car_id].direction);
      launch_flag = true;
    }
    else if(cars[car_id].state == WAIT){
        int s1 = cars[car_id].pos;
        int v2 = min(roads[cars[car_id].next_road_id].speed_limit, cars[car_id].speed);
        int cur_road_id = cars[car_id].current_road_id;
        int next_road_id = cars[car_id].next_road_id;
        if(s1<min(cars[car_id].speed, roads[cur_road_id].speed_limit) && cars[car_id].end == id){ // the car reach goal
          cars[car_id].state = REACHED;
          reached_cars++;
          roads[cur_road_id].DeleteCar(cars, car_id);
          return ADD_SUCCESS; //TO_DO:change ADD_SUCCESS to UPDATE_SUCCESS seems more reasonable
        }
        if(s1>=v2 || (cars[car_id].pos != 0 && cars[car_id].direction == 0 && roads[cur_road_id].lane0[cars[car_id].channel][0]!=POS_BLANK) 
            || (cars[car_id].pos != 0 && cars[car_id].direction == 1 && roads[cur_road_id].lane1[cars[car_id].channel][0]!=POS_BLANK)){
          roads[cur_road_id].UpdateCar(cars, car_id);
        }
        else{
          int direction;
          if(id == roads[next_road_id].start) direction = 0;
          else direction = 1;
          update_res = roads[next_road_id].AddCar(cars, car_id, direction);
          if(update_res == ADD_SUCCESS) roads[cur_road_id].DeleteCar(cars, car_id);
        }
      
    }
    if(update_res != FRONT_CAR_WAIT) cars[car_id].state = STOP;
    if(update_res != ADD_SUCCESS && launch_flag == true) cars[car_id].state = IN_GARAGE;
    return update_res;
  }
  
};

#endif //CROSS_H_
