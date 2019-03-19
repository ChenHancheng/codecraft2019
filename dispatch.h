#ifndef DISPATCH_h_
#define DISPATCH_h_

#include <vector>
#include <utility>
#include <unordered_map>
#include <iomanip>
#include <queue>
#include <iostream>

#include "road.h"
#include "cross.h"
#include "car.h"
#include "dijkstra.h"

using std::priority_queue;
using std::unordered_map;
using std::pair;
using std::vector;
using std::min;

// void LaunchCar(int T_count, vector<Car>& cars, unordered_map<int, vector<int>>& ready_car, vector<Road>& roads, Graph& graph);
// void RecordSolution(vector<Car>& cars, vector<Car>& cars_tmp, vector<Road>& roads, vector<Road>& road_tmp);
void RecordSolution(vector<Car>& cars, vector<Road>& roads, vector<Cross>& crosses);
void LaunchCar(int T_count, vector<Car>& cars, vector<Cross>& crosses, unordered_map<int, priority_queue<int, vector<int>, std::greater<int>>>& ready_car, vector<Road>& roads, Graph& graph);

void Dispatch(vector<Car>& cars, vector<Road>& roads, vector<Cross>& crosses, Graph& graph, unordered_map<int, priority_queue<int, vector<int>, std::greater<int>>> ready_cars){
  int roads_num = roads.size();
  int crosses_num = crosses.size();
  int cars_num = cars.size();
  int T_count=1;
  while(1){//not all car finished
    for(int i=0; i<roads_num; i++){
      roads[i].RoadRun(cars);
    }
    for(int i=0; i<roads[0].lane0[0].size(); i++){
      std::cout<<std::setw(5)<<roads[0].lane0[0][i];
    }
    std::cout<<std::endl;
    // vector<Road> roads_tmp(roads);
    // vector<Car> cars_tmp(cars);
    // vector<vector<pair<int, int>>> solution;
    // vector<pair<int, int>> init_solution;

    for(int i=0; i<crosses_num; i++){
     crosses[i].InitialValue(roads, cars, graph.cost_matrix);
    }

    //tabu(cars, roads, crossed, solution);
    // RecordSolution(cars, cars_tmp, roads, roads_tmp);
    RecordSolution(cars, roads, crosses);

    LaunchCar(T_count, cars, crosses, ready_cars, roads, graph);

    T_count++;
  }
  WriteAnswerFile(cars);
}

// add those cars which should launch at time T_count
void LaunchCar(int T_count, vector<Car>& cars, vector<Cross>& crosses, unordered_map<int, priority_queue<int, vector<int>, std::greater<int>>>& ready_car, vector<Road>& roads, Graph& graph){
  int ready_car_num = 0;
  if(ready_car.count(T_count) > 0) ready_car_num = ready_car[T_count].size();
  for(int i = 0; i<ready_car_num; i++){
    int car_id = ready_car[T_count].top();
    int cross_id = graph.parent_matrix[cars[car_id].start][cars[car_id].end];
    int road_id = graph.adj_matrix[cars[car_id].start][cross_id];
    cars[car_id].direction = (cross_id == roads[road_id].start);
    cars[car_id].next_road_id = road_id;
    cars[car_id].true_start_time = T_count;
    cars[car_id].pos = roads[road_id].length;
    if(crosses[cross_id].UpdateCar(roads, cars, car_id) != ADD_SUCCESS){
      ready_car[T_count+1].push(car_id);
    }
    ready_car[T_count].pop();
    
  }
}

// void RecordSolution(vector<Car>& cars, vector<Car>& cars_tmp, vector<Road>& roads, vector<Road>& road_tmp){
//   for(int i =0; i<cars.size(); i++){
//     cars[i] = cars_tmp[i];
//   }
//   for(int i=0; i<roads.size(); i++){
//     roads[i] = road_tmp[i];
//   }
// }
void RecordSolution(vector<Car>& cars, vector<Road>& roads, vector<Cross>& crosses){ //TO_DO:detect dead lock
  int i=0;
  vector<bool> cross_finish_flag(crosses.size(), false);
  while(1){ //if not all crosses are not complete
    vector<bool> road_finish_flag(crosses[i].valid_roads_num, false);
    int cur_car_id, next_road_id, cur_road_id, direction;
    int j = 0;
    for(j = 0; j<crosses[i].valid_roads_num; j++){ // find the first not empty road
      if(crosses[i].road_queue[crosses[i].dispatch_seq[j]].empty() == false){
        cur_car_id = crosses[i].road_queue[crosses[i].dispatch_seq[j]].front();
        break;
      }
      else{
        road_finish_flag[j] = true;
      }
    }
    if(j == crosses[i].valid_roads_num){
      cross_finish_flag[i] = true;
    }
    while(cross_finish_flag[i] == false){ //if current cross is not complete
      next_road_id = cars[cur_car_id].next_road_id;
      cur_road_id = cars[cur_car_id].current_road_id;
      direction = crosses[i].turn_direction[{cur_road_id, next_road_id}];

      if(direction == GO_STRAIGHT){
        // if(min(roads[next_road_id].speed_limit, cars[cur_car_id].speed)>cars[cur_car_id].pos){ //TO_DO: these code are repeated three times, maybe i can write in a funtion
          crosses[i].road_queue[cars[cur_car_id].current_road_id].pop();
          int add_car_state = crosses[i].UpdateCar(roads, cars, cur_car_id);
          if(add_car_state == FRONT_CAR_WAIT) break;
          if(add_car_state == NEXT_ROAD_FULL){
            for(int lala = 0; lala<crosses[i].valid_roads_num; lala++){
              if(crosses[i].dispatch_seq[lala] == cars[cur_car_id].current_road_id){
                road_finish_flag[lala] = true;
              }
            }
          }
        // }
      }
      else if(direction == TURN_LEFT){ // if the car needs to turn left, we need check wether there are other cars need to go straight
        int road_id_tmp=-1, car_id_tmp=-1, direction_tmp = -1;
        road_id_tmp = crosses[i].turn_road[{cars[cur_car_id].current_road_id, TURN_RIGHT}];
        if(crosses[i].road_queue.empty() == false){
          car_id_tmp = crosses[i].road_queue[road_id_tmp].front();
          direction_tmp = crosses[i].turn_direction[{cars[car_id_tmp].current_road_id, cars[car_id_tmp].next_road_id}];
        }

        int true_wait_flag = crosses[i].CheckTrueWait(roads, cars, car_id_tmp); //check if the car is in true WAIT state
        if(true_wait_flag == false){
          crosses[i].UpdateCar(roads, cars, car_id_tmp);
          crosses[i].road_queue[road_id_tmp].pop();
        }
        if(direction_tmp == 0 && true_wait_flag) {
          cur_car_id = car_id_tmp;
          continue; 
        }
        else{
          crosses[i].road_queue[cars[cur_car_id].current_road_id].pop();
          int add_car_state;
          if(min(roads[next_road_id].speed_limit, cars[cur_car_id].speed)>cars[cur_car_id].pos){
            if(roads[cur_road_id].end == crosses[i].id){
              crosses[i].UpdateCar(roads, cars, cur_car_id);
            }
            else{
              crosses[i].UpdateCar(roads, cars, cur_car_id);
            }
          }
          if(add_car_state == FRONT_CAR_WAIT) break;
          if(add_car_state == NEXT_ROAD_FULL){
            for(int lala = 0; lala<crosses[i].valid_roads_num; lala++){
              if(crosses[i].dispatch_seq[lala] == cars[cur_car_id].current_road_id){
                road_finish_flag[lala] = true;
              }
            }
          }
        } 
      }
      else{ // if the car needs to turn left, we need check wether there are car need to go straight or left
        int road_id_tmp=-1, car_id_tmp=-1, direction_tmp = -1;
        road_id_tmp = crosses[i].turn_road[{cars[cur_car_id].current_road_id, TURN_RIGHT}];
        if(crosses[i].road_queue.empty() == false){
          car_id_tmp = crosses[i].road_queue[road_id_tmp].front();
          direction_tmp = crosses[i].turn_direction[{cars[car_id_tmp].current_road_id, cars[car_id_tmp].next_road_id}];
        }
        int road_id_tmp1=-1, car_id_tmp1 =-1, direction_tmp1 = -1;
        road_id_tmp1 = crosses[i].turn_road[{cars[cur_car_id].current_road_id, TURN_RIGHT}];
        if(crosses[i].road_queue.empty() == false){
          car_id_tmp1 = crosses[i].road_queue[road_id_tmp].front();
          direction_tmp1 = crosses[i].turn_direction[{cars[car_id_tmp].current_road_id, cars[car_id_tmp].next_road_id}];
        }
        
        int true_wait_flag = crosses[i].CheckTrueWait(roads, cars, car_id_tmp); //check if the car is in true WAIT state
        if(true_wait_flag == false){
          crosses[i].UpdateCar(roads, cars, car_id_tmp);
          crosses[i].road_queue[road_id_tmp].pop();
        }        
        int true_wait_flag1 = crosses[i].CheckTrueWait(roads, cars, car_id_tmp1); //check if the car is in true WAIT state
        if(true_wait_flag == false){
          crosses[i].UpdateCar(roads, cars, car_id_tmp1);
          crosses[i].road_queue[road_id_tmp].pop();
        }        
        if(direction_tmp == GO_STRAIGHT && true_wait_flag == true){//query wether there is car goes straight
          cur_car_id = car_id_tmp;
          continue;  
        }
        else if(direction_tmp1 == TURN_RIGHT && true_wait_flag1 == true){  //query wether there is car turns left
          cur_car_id = car_id_tmp1;
          continue;
        }
        else{
          crosses[i].road_queue[cars[cur_car_id].current_road_id].pop();
          int add_car_state;
          if(min(roads[next_road_id].speed_limit, cars[cur_car_id].speed)>cars[cur_car_id].pos){
            if(roads[cur_road_id].end == crosses[i].id){
              crosses[i].UpdateCar(roads, cars, cur_car_id);
            }
            else{
              crosses[i].UpdateCar(roads, cars, cur_car_id);
            }
            if(add_car_state == FRONT_CAR_WAIT) break;
            if(add_car_state == NEXT_ROAD_FULL){
              for(int lala = 0; lala<crosses[i].valid_roads_num; lala++){
                if(crosses[i].dispatch_seq[lala] == cars[cur_car_id].current_road_id){
                  road_finish_flag[lala] = true;
                }
              }
            }
            }
        }
      }
      int k=0;
      for(k=0; k<crosses[i].valid_roads_num; k++){
        if(crosses[i].road_queue[crosses[i].dispatch_seq[k]].empty() != true){
          road_finish_flag[k] = true;
          break;
        }
      }
      if(j == crosses[i].valid_roads_num) cross_finish_flag[i] = true;
    }//end for while(cross_finish_flag[i] == false)
    i = (i+1)%crosses.size();
    int m = 0;
    for(m=0; m<crosses.size(); m++){
      if(cross_finish_flag[m] != true){
        break;
      }
    }
    if( m == crosses.size()) break; //all crosses finished at this time
  }//end for while(1);
}


#endif //DISPATCH_h_