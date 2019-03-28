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

int Cross::reached_cars = 0;
//use static road state to generate an initial value for optimizer
void Cross::InitialValue(vector<Road>& roads, vector<Car>& cars, const vector<vector<int>> cost_matrix){
  //clear the last time vaule;
  road_queue.clear();
  for(int i = 0; i<valid_roads_num; i++){
    for(int j=0; j<roads[dispatch_seq[i]].length; j++){
      for(int k=0; k<roads[dispatch_seq[i]].channel; k++){
        int car_id = -1;
        if(id == roads[dispatch_seq[i]].end){
          car_id = roads[dispatch_seq[i]].lane0[k][j];
        }//end of if
        else if(id == roads[dispatch_seq[i]].start && roads[dispatch_seq[i]].bidirectional == true){
          car_id = roads[dispatch_seq[i]].lane1[k][j];
        }
        if(car_id != -1 && cars[car_id].state==WAIT){
            int min_cost = INF;
            int next_road_id;
            for(int l=0; l<valid_roads_num; l++){
              if(l == i) continue;
              int road_id = dispatch_seq[l];
              int cost;
              int direction_tmp;
              int cross_id = roads[road_id].start==id?roads[road_id].end:roads[road_id].start;
              int ratio;
              if(roads[road_id].start == id){
                direction_tmp = 0;
                ratio = (8*roads[road_id].cars_num_road0/roads[id].max_car_num);
              }
              else{
                direction_tmp = 1;
                ratio = (8*roads[road_id].cars_num_road1/roads[id].max_car_num);
                if(roads[road_id].bidirectional == false) continue;
              }
              cost = roads[road_id].QueryRoadState(cars, car_id, direction_tmp) + cost_matrix[cross_id][cars[car_id].end]/cars[car_id].speed+ratio;
              if(cost <min_cost){
                next_road_id = road_id;
                min_cost = cost;
              }
            }
            cars[car_id].next_road_id = next_road_id;
            if(id == cars[car_id].end) 
            {
              cars[car_id].next_road_id = cars[car_id].current_road_id;
            }
            if(cars[car_id].current_road_id ==  cars[car_id].next_road_id && cars[car_id].end != id){
              std::cerr<<"the initial value is wrong"<<std::endl;
            }
            road_queue[dispatch_seq[i]].push(car_id);
          }
      }//end of for(0->channel)
    }
  }
}

int Cross::UpdateCar(vector<Road>& roads, vector<Car>& cars, int car_id){
  bool launch_flag = false;
  int update_res = ADD_SUCCESS;
  if(cars[car_id].state == IN_GARAGE){
    int next_road_id = cars[car_id].next_road_id;
    update_res = roads[next_road_id].AddCar(cars, car_id, cars[car_id].direction);
    if(update_res == NEXT_ROAD_FULL && launch_flag == true){
      cars[car_id].state = IN_GARAGE;
    }
    return update_res;
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
      // if(s1>=v2 || (cars[car_id].pos != 0 && cars[car_id].direction == 0 && roads[cur_road_id].lane0[cars[car_id].channel][0]!=POS_BLANK) 
      //     || (cars[car_id].pos != 0 && cars[car_id].direction == 1 && roads[cur_road_id].lane1[cars[car_id].channel][0]!=POS_BLANK)){
      if(s1>=v2){
        roads[cur_road_id].UpdateCar(cars, car_id);
        return ADD_SUCCESS;
      }
      else{
        int direction;
        if(id == roads[next_road_id].start) direction = 0;
        else direction = 1;
        update_res = roads[next_road_id].AddCar(cars, car_id, direction);
        if(update_res == NEXT_ROAD_FULL){
          roads[cur_road_id].UpdateCar(cars, car_id);
          return ADD_SUCCESS;
          // return FRONT_CAR_WAIT;
        }
        else if(update_res == ADD_SUCCESS){
          roads[cur_road_id].DeleteCar(cars, car_id);
          cars[car_id].state = STOP;
          return ADD_SUCCESS;
        }
        else
        {
          return FRONT_CAR_WAIT;
        }
        
      }
    
  }
}

void Cross::CalCost(){

}

Cross::Cross(const CrossData& cross_data):id(cross_data.id){
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
  turn_direction.insert({{road_id0, road_id0}, GO_STRAIGHT});
  turn_direction.insert({{road_id1, road_id1}, GO_STRAIGHT});
  turn_direction.insert({{road_id2, road_id2}, GO_STRAIGHT});
  turn_direction.insert({{road_id3, road_id3}, GO_STRAIGHT});

  turn_direction.insert({{road_id0, road_id2}, GO_STRAIGHT});
  turn_direction.insert({{road_id2, road_id0}, GO_STRAIGHT});
  turn_direction.insert({{road_id1, road_id3}, GO_STRAIGHT});
  turn_direction.insert({{road_id3, road_id1}, GO_STRAIGHT});
  turn_direction.insert({{road_id0, road_id1}, TURN_LEFT});
  turn_direction.insert({{road_id1, road_id2}, TURN_LEFT});
  turn_direction.insert({{road_id2, road_id3}, TURN_LEFT});
  turn_direction.insert({{road_id3, road_id0}, TURN_LEFT});
  turn_direction.insert({{road_id0, road_id3}, TURN_RIGHT});
  turn_direction.insert({{road_id3, road_id2}, TURN_RIGHT});
  turn_direction.insert({{road_id2, road_id1}, TURN_RIGHT});
  turn_direction.insert({{road_id1, road_id0}, TURN_RIGHT});

  turn_road.insert({{road_id0, GO_STRAIGHT}, road_id2});
  turn_road.insert({{road_id2, GO_STRAIGHT}, road_id0});
  turn_road.insert({{road_id1, GO_STRAIGHT}, road_id3});
  turn_road.insert({{road_id3, GO_STRAIGHT}, road_id1});
  turn_road.insert({{road_id0, TURN_LEFT}, road_id1});
  turn_road.insert({{road_id1, TURN_LEFT}, road_id2});
  turn_road.insert({{road_id2, TURN_LEFT}, road_id3});
  turn_road.insert({{road_id3, TURN_LEFT}, road_id0});
  turn_road.insert({{road_id0, TURN_RIGHT}, road_id3});
  turn_road.insert({{road_id3, TURN_RIGHT}, road_id2});
  turn_road.insert({{road_id2, TURN_RIGHT}, road_id1});
  turn_road.insert({{road_id1, TURN_RIGHT}, road_id0});
}

