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
#include "dispatch.h"
//  #include "gui.h"

using std::priority_queue;
using std::unordered_map;
using std::pair;
using std::vector;
using std::min;

void Dispatch(vector<Car>& cars, vector<Road>& roads, vector<Cross>& crosses, Graph& graph, unordered_map<int, priority_queue<int, vector<int>, std::greater<int>>> ready_cars){
  int roads_num = roads.size();
  int crosses_num = crosses.size();
  int cars_num = cars.size();
  unsigned long int T_count=0;

  while(1){//not all car finished
    for(int i=0; i<roads_num; i++){
      roads[i].RoadRun(cars);
    }
    for(int i=0; i<crosses_num; i++){
     crosses[i].InitialValue(roads, cars, graph.cost_matrix);
    }
    // StateShow(roads, crosses, cars);
    //tabu(cars, roads, crossed, solution);
    RecordSolution(cars, roads, crosses);
    // StateShow(roads, crosses, cars);

    LaunchCar(T_count, cars, crosses, ready_cars, roads, graph);
    //StateShow(roads, crosses, cars);
    if(Cross::reached_cars == cars.size()){
      std::cout<<"all cars reached"<<std::endl;
      std::cout<<"total cost "<<T_count <<" time"<<std::endl;
      break;
    }
// #ifdef DEBUG
    std::cout<<T_count<<":Total "<< Cross::reached_cars<<"cars reached "<<std::endl;
// #endif
    T_count++;
  }
}

// add those cars which should launch at time T_count
void LaunchCar(int T_count, vector<Car>& cars, vector<Cross>& crosses, unordered_map<int, priority_queue<int, vector<int>, std::greater<int>>>& ready_car, vector<Road>& roads, Graph& graph){
  int ready_car_num = 0;
  int max_car_number = 0;
  if(ready_car.count(T_count) > 0) ready_car_num = ready_car[T_count].size();
  for(int i = 0; i<ready_car_num; i++){
    int car_id = ready_car[T_count].top();
    int cross_id = graph.parent_matrix[cars[car_id].start][cars[car_id].end];
    int road_id = graph.adj_matrix[cars[car_id].start][cross_id];
    cars[car_id].direction = (cross_id == roads[road_id].start);
    cars[car_id].next_road_id = road_id;
    cars[car_id].true_start_time = T_count;
    cars[car_id].pos = 0;
    int update_status = -1;
    if(max_car_number<15){
      update_status = crosses[cross_id].UpdateCar(roads, cars, car_id); 
    }
    if(update_status == ADD_SUCCESS) max_car_number++;
    if(update_status != ADD_SUCCESS){
      ready_car[T_count+1].push(car_id);
    }
    ready_car[T_count].pop();
  }
  ready_car.erase(T_count);
}

// void RecordSolution(vector<Car>& cars, vector<Car>& cars_tmp, vector<Road>& roads, vector<Road>& road_tmp){
//   for(int i =0; i<cars.size(); i++){
//     cars[i] = cars_tmp[i];
//   }
//   for(int i=0; i<roads.size(); i++){
//     roads[i] = road_tmp[i];
//   }
// }
void DealDealLock(vector<int>& wait_list, vector<Cross>& crosses, vector<Car>& cars, vector<Road>& roads){
  bool dead_lock_flag = false;
  int loop_id;
  for(int i = 0; i<wait_list.size() && !dead_lock_flag; i++){
    int slow = i, fast = i;
    while(wait_list[fast]!=-1 && wait_list[wait_list[fast]] != -1){
      slow = wait_list[slow];
      fast = wait_list[wait_list[fast]];
      if(slow == fast){
        // WriteMapImg(roads, crosses, cars);
        dead_lock_flag = true;
  // #ifdef DEBUG
        std::cout<<"dead lock happeds"<<std::endl;
  // #endif
        loop_id = slow;
        break;
      }
    }
  }
  if(dead_lock_flag == false) return;
  while(crosses[loop_id].valid_roads_num <= 2){
    loop_id = wait_list[loop_id];
  }
  wait_list[loop_id] = -1;
  int car_id = 0;
  for(int j = 0; j<crosses[loop_id].valid_roads_num; j++){ // find the first not empty road
    if(crosses[loop_id].road_queue[crosses[loop_id].dispatch_seq[j]].empty() == false){
      car_id = crosses[loop_id].road_queue[crosses[loop_id].dispatch_seq[j]].front();
      break;
    }
  }
  
  for(int j= 0; j<crosses[loop_id].valid_roads_num; j++){
    if(crosses[loop_id].dispatch_seq[j] != cars[car_id].next_road_id && crosses[loop_id].dispatch_seq[j] != cars[car_id].current_road_id){
      if(loop_id == roads[crosses[loop_id].dispatch_seq[j]].start){
          cars[car_id].next_road_id = crosses[loop_id].dispatch_seq[j];
          // break;
      }//end of if
      else if(loop_id == roads[crosses[loop_id].dispatch_seq[j]].end && roads[crosses[loop_id].dispatch_seq[j]].bidirectional == true){
        cars[car_id].next_road_id = crosses[loop_id].dispatch_seq[j];
        // break;
      }
    }
  }
}
void RecordSolution(vector<Car>& cars, vector<Road>& roads, vector<Cross>& crosses){ //TO_DO:detect dead lock
  int i=0;
  vector<int> wait_list(crosses.size(), -1);
  vector<bool> cross_finish_flag(crosses.size(), false);
  
  while(1){ //if not all crosses are not complete
    int cur_car_id, next_road_id, cur_road_id, direction;
    int j = 0;
    for(j = 0; j<crosses[i].valid_roads_num; j++){ // find the first not empty road
      if(crosses[i].road_queue[crosses[i].dispatch_seq[j]].empty() == false){
        cur_car_id = crosses[i].road_queue[crosses[i].dispatch_seq[j]].front();
        break;
      }
    }
    if(j == crosses[i].valid_roads_num){
      cross_finish_flag[i] = true;
    }
    unordered_map<int, bool> road_wait_flag;
    while(cross_finish_flag[i] == false){ //if current cross is not complete
      next_road_id = cars[cur_car_id].next_road_id;
      cur_road_id = cars[cur_car_id].current_road_id;
      direction = crosses[i].turn_direction[{cur_road_id, next_road_id}];

      if(direction == GO_STRAIGHT){
          int add_car_state = crosses[i].UpdateCar(roads, cars, cur_car_id);
          if(add_car_state == FRONT_CAR_WAIT){
            wait_list[i] = roads[next_road_id].start == i?roads[next_road_id].end:roads[next_road_id].start;
            road_wait_flag[cur_road_id] = true;
            // continue;
          }
          if((cars[cur_car_id].state == STOP ||cars[cur_car_id].state == REACHED) && !crosses[i].road_queue[cur_road_id].empty()) {
            // wait_list[i] = -1;
            crosses[i].road_queue[cur_road_id].pop();
          }
  #ifdef DEBUG
          else std::cerr<<"it seems some errors occur"<<std::endl;
  #endif
      }
      else if(direction == TURN_LEFT){ // if the car needs to turn left, we need check wether there are other cars need to go straight
        int road_id_tmp=-1, car_id_tmp=-1, direction_tmp = -1;
        road_id_tmp = crosses[i].turn_road[{cars[cur_car_id].current_road_id, TURN_RIGHT}];
        if(crosses[i].road_queue[road_id_tmp].empty() == false&& road_wait_flag[road_id_tmp] == false){
          car_id_tmp = crosses[i].road_queue[road_id_tmp].front();
          direction_tmp = crosses[i].turn_direction[{cars[car_id_tmp].current_road_id, cars[car_id_tmp].next_road_id}];
        }

        if(direction_tmp == GO_STRAIGHT) {
          cur_car_id = car_id_tmp;
          continue; 
        }
        else{
          int add_car_state;
              add_car_state = crosses[i].UpdateCar(roads, cars, cur_car_id);
          if(add_car_state == FRONT_CAR_WAIT){
            wait_list[i] = roads[next_road_id].start == i?roads[next_road_id].end:roads[next_road_id].start;
            road_wait_flag[cur_road_id] = true;
            // continue;
          }
          if((cars[cur_car_id].state == STOP ||cars[cur_car_id].state == REACHED) && !crosses[i].road_queue[cur_road_id].empty()){
            // wait_list[i] = -1;
            crosses[i].road_queue[cur_road_id].pop();
          }
  #ifdef DEBUG
          else std::cerr<<"it seems some errors occur"<<std::endl;
  #endif
        } 
      }
      else{ // if the car needs to turn left, we need check wether there are car need to go straight or left
        int road_id_tmp=-1, car_id_tmp=-1, direction_tmp = -1;
        road_id_tmp = crosses[i].turn_road[{cars[cur_car_id].current_road_id, TURN_LEFT}];
        if(crosses[i].road_queue[road_id_tmp].empty() == false && road_wait_flag[road_id_tmp] == false){
          car_id_tmp = crosses[i].road_queue[road_id_tmp].front();
          direction_tmp = crosses[i].turn_direction[{cars[car_id_tmp].current_road_id, cars[car_id_tmp].next_road_id}];
        }
        int road_id_tmp1=-1, car_id_tmp1 =-1, direction_tmp1 = -1;
        road_id_tmp1 = crosses[i].turn_road[{cars[cur_car_id].current_road_id, GO_STRAIGHT}];
        if(crosses[i].road_queue[road_id_tmp1].empty() == false&& road_wait_flag[road_id_tmp1] == false){
          car_id_tmp1 = crosses[i].road_queue[road_id_tmp1].front();
          direction_tmp1 = crosses[i].turn_direction[{cars[car_id_tmp1].current_road_id, cars[car_id_tmp1].next_road_id}];
        }
            
        if(direction_tmp == GO_STRAIGHT){//query wether there is car goes straight
          cur_car_id = car_id_tmp;
          continue;  
        }
        else if(direction_tmp1 == TURN_LEFT){  //query wether there is car turns left
          cur_car_id = car_id_tmp1;
          continue;
        }
        else{
          int add_car_state;
              add_car_state = crosses[i].UpdateCar(roads, cars, cur_car_id);
            if(add_car_state == FRONT_CAR_WAIT){
              wait_list[i] = roads[next_road_id].start == i?roads[next_road_id].end:roads[next_road_id].start;
              road_wait_flag[cur_road_id] = true;
              // continue;
            }
            if((cars[cur_car_id].state == STOP ||cars[cur_car_id].state == REACHED) && !crosses[i].road_queue[cur_road_id].empty()){
              // wait_list[i] = -1;
              crosses[i].road_queue[cur_road_id].pop();
            }
  #ifdef DEBUG
            else std::cerr<<"it seems some errors occur"<<std::endl;
  #endif
          }
        }
        int test_flag = false;
        //find the next car
            if(crosses[i].road_queue[cur_road_id].empty() == false && road_wait_flag[cur_road_id] == false){
              cur_car_id = crosses[i].road_queue[cur_road_id].front();
            }
            else
            {
              for(int lala = 0;lala<crosses[i].valid_roads_num; lala++){
                if(crosses[i].dispatch_seq[lala] == cur_road_id){
                  int haha = (lala+1)%crosses[i].valid_roads_num;
                  while(haha != lala){
                    if(crosses[i].road_queue[crosses[i].dispatch_seq[haha]].empty() == false && 
                      road_wait_flag[crosses[i].dispatch_seq[haha]] == false){
                      cur_car_id = crosses[i].road_queue[crosses[i].dispatch_seq[haha]].front();
                      break;
                    }
                    haha = (haha+1)%crosses[i].valid_roads_num;
                  }
                  if(haha == lala) test_flag = true;
                  if(haha == lala && wait_list[i] == -1) {
                    cross_finish_flag[i] = true;
                  }
                  else break;
                }
              }
            }
            if(test_flag == true) break;
    }//end for while(cross_finish_flag[i] == false)
    i = (i+1)%crosses.size();
    if(i == crosses.size()-1) {
      DealDealLock(wait_list, crosses, cars, roads);
      wait_list = vector<int>(crosses.size(), -1);
    }
    int m = 0;
    for(m=0; m<crosses.size(); m++){
      if(cross_finish_flag[m] != true){
        break;
      }
    }
    if( m == crosses.size()) break; //all crosses finished at this time
  }//end for while(1);
}
