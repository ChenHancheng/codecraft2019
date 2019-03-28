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
// #include "gui.h"

using std::priority_queue;
using std::unordered_map;
using std::pair;
using std::vector;
using std::min;

unsigned long int T_count=0;
void Dispatch(vector<Car>& cars, vector<Road>& roads, vector<Cross>& crosses, Graph& graph, unordered_map<int, priority_queue<int, vector<int>, std::greater<int>>> ready_cars){
  int roads_num = roads.size();
  int crosses_num = crosses.size();
  int cars_num = cars.size();

  while(1){//not all car finished
    for(int i=0; i<roads_num; i++){
      roads[i].RoadRun(cars);
    }
    for(int i=0; i<crosses_num; i++){
     crosses[i].InitialValue(roads, cars, graph.cost_matrix);
    }
    //tabu(cars, roads, crossed, solution);
    RecordSolution(cars, roads, crosses);
    LaunchCar(T_count, cars, crosses, ready_cars, roads, graph);
    std::cout<<T_count<<":Total "<< Cross::reached_cars<<"cars reached "<<std::endl;
    if(Cross::reached_cars == cars.size()){
      std::cout<<"all cars reached"<<std::endl;
      std::cout<<"total cost "<<T_count <<" time"<<std::endl;
      break;
    }
    T_count++;
  }
  unsigned long total_time = 0;
  for(int i=0; i<cars_num; i++){
    total_time += cars[i].end_time-cars[i].plan_time;
  }
  std::cout<<"all cars total cost:"<<total_time<<std::endl;
}
int total_launched;
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
    if(update_status == ADD_SUCCESS) {
      max_car_number++;
      total_launched++;
    }
    if(update_status != ADD_SUCCESS){
      ready_car[T_count+1].push(car_id);
    }
    ready_car[T_count].pop();
  }
  ready_car.erase(T_count);
}

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
        std::cout<<"dead lock happeds"<<std::endl;
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
          break;
      }//end of if
      else if(loop_id == roads[crosses[loop_id].dispatch_seq[j]].end && roads[crosses[loop_id].dispatch_seq[j]].bidirectional == true){
        cars[car_id].next_road_id = crosses[loop_id].dispatch_seq[j];
        break;
      }
    }
  }
}
void RecordSolution(vector<Car>& cars, vector<Road>& roads, vector<Cross>& crosses){ //TO_DO:detect dead lock
  vector<int> wait_list(crosses.size(), -1);
  vector<bool> cross_finish_flag(crosses.size(), false);
  bool all_crosses_finished_flag = false;
  while(all_crosses_finished_flag == false){ //if not all crosses are not complete
    bool dead_lock_flag = true;
    bool has_waited_car_flag = false;
    for(int i = 0; i<crosses.size(); i++){ //repeat dispatching each cross
      int cur_car_id, next_road_id, cur_road_id, direction;
      unordered_map<int, bool> road_wait_flag;
      int count = 0;
      for(int j = 0; j<crosses[i].valid_roads_num && cross_finish_flag[i] == false; j++){//%crosses[i].valid_roads_num){ // find the first not empty road
        // if(crosses[i].road_queue[crosses[i].dispatch_seq[j]].empty() == false 
        //     && road_wait_flag[crosses[i].dispatch_seq[j]] == false){
        //   cur_car_id = crosses[i].road_queue[crosses[i].dispatch_seq[j]].front();
          cur_road_id = crosses[i].dispatch_seq[j];
        //   count = 0;
        //   has_waited_car_flag = true;
        // }
        // else{
        //   if(count == crosses[i].valid_roads_num) break;
        //   count++;
        //   continue;
        // }
        
        while(crosses[i].road_queue[cur_road_id].empty() == false){
              // && road_wait_flag[crosses[i].dispatch_seq[j]] == false){ //if current cross is not complete
          cur_car_id = crosses[i].road_queue[cur_road_id].front();
          if(T_count > 400 && (i == 44 || i == 45)){
            // std::cout<<"cross_id"<<i<<", j:"<<j<<std::endl;
            // std::cout<<"road speed:"<<roads[cur_road_id].speed_limit<<std::endl;
            // std::cout<<"cur_car_id:"<<cur_car_id<<std::endl;
            // std::cout<<"cur_car_id channel:"<<cars[cur_car_id].channel<<std::endl;
            // std::cout<<"cur_car_id pos:"<<cars[cur_car_id].pos<<std::endl;
            // std::cout<<"cur_car_id speed:"<<cars[cur_car_id].speed<<std::endl;
            // std::cout<<std::endl;
            // StateShow(roads, crosses, cars);
          }
          next_road_id = cars[cur_car_id].next_road_id;
          direction = crosses[i].turn_direction[{cur_road_id, next_road_id}];

          if(cars[cur_car_id].state == STOP) {
            if(crosses[i].road_queue[cur_road_id].empty()){
                std::cerr<<"it seems some errors occur"<<std::endl;
            }
            crosses[i].road_queue[cur_road_id].pop();
            dead_lock_flag = false;
          }
          else if(direction == GO_STRAIGHT){
              int add_car_state = crosses[i].UpdateCar(roads, cars, cur_car_id);
              if(add_car_state == FRONT_CAR_WAIT){
                wait_list[i] = roads[next_road_id].start == i?roads[next_road_id].end:roads[next_road_id].start;
                // road_wait_flag[cur_road_id] = true;
                break;
              }
              else if((cars[cur_car_id].state == STOP ||cars[cur_car_id].state == REACHED) && !crosses[i].road_queue[cur_road_id].empty()) {
                crosses[i].road_queue[cur_road_id].pop();
                if(cars[cur_car_id].state == REACHED) cars[cur_car_id].end_time = T_count;
                dead_lock_flag = false;
              }
              else std::cerr<<"it seems some errors occur"<<std::endl;
          }
          else if(direction == TURN_LEFT){ // if the car needs to turn left, we need check wether there are other cars need to go straight
            int road_id_tmp=-1, car_id_tmp=-1, direction_tmp = -1;
            road_id_tmp = crosses[i].turn_road[{cars[cur_car_id].current_road_id, TURN_RIGHT}];
            if(crosses[i].road_queue.count(road_id_tmp)>0 && crosses[i].road_queue[road_id_tmp].empty() == false){//&& road_wait_flag[road_id_tmp] == false){
              car_id_tmp = crosses[i].road_queue[road_id_tmp].front();
              if(cars[car_id_tmp].state == STOP) std::cout<<"asdjfsaf";
              direction_tmp = crosses[i].turn_direction[{cars[car_id_tmp].current_road_id, cars[car_id_tmp].next_road_id}];
            }
            if(direction_tmp == GO_STRAIGHT) {
              break; 
            }
            else{
              int add_car_state;
              add_car_state = crosses[i].UpdateCar(roads, cars, cur_car_id);
              if(add_car_state == FRONT_CAR_WAIT){
                wait_list[i] = roads[next_road_id].start == i?roads[next_road_id].end:roads[next_road_id].start;
                // road_wait_flag[cur_road_id] = true;
                break;
              }
              else if(cars[cur_car_id].state == STOP && !crosses[i].road_queue[cur_road_id].empty()){
                crosses[i].road_queue[cur_road_id].pop();
               // if(cars[cur_car_id].state == REACHED) cars[cur_car_id].end_time = T_count;
                dead_lock_flag = false;
              }
              else std::cerr<<"it seems some errors occur"<<std::endl;
            } 
          }
          else{ // if the car needs to turn left, we need check wether there are car need to go straight or left
            int road_id_tmp=-1, car_id_tmp=-1, direction_tmp = -1;
            road_id_tmp = crosses[i].turn_road[{cars[cur_car_id].current_road_id, TURN_LEFT}];
            if(crosses[i].road_queue.count(road_id_tmp)>0 && crosses[i].road_queue[road_id_tmp].empty() == false){ //&& road_wait_flag[road_id_tmp] == false){
              car_id_tmp = crosses[i].road_queue[road_id_tmp].front();
              if(cars[car_id_tmp].state == STOP) std::cout<<"asdjfsaf";
              direction_tmp = crosses[i].turn_direction[{cars[car_id_tmp].current_road_id, cars[car_id_tmp].next_road_id}];
            }
            int road_id_tmp1=-1, car_id_tmp1 =-1, direction_tmp1 = -1;
            road_id_tmp1 = crosses[i].turn_road[{cars[cur_car_id].current_road_id, GO_STRAIGHT}];
            if(crosses[i].road_queue.count(road_id_tmp1)>0 && crosses[i].road_queue[road_id_tmp1].empty() == false){ //&& road_wait_flag[road_id_tmp1] == false){
              car_id_tmp1 = crosses[i].road_queue[road_id_tmp1].front();
              if(cars[car_id_tmp1].state == STOP) std::cout<<"asdjfsaf";
              direction_tmp1 = crosses[i].turn_direction[{cars[car_id_tmp1].current_road_id, cars[car_id_tmp1].next_road_id}];
            }

            if(direction_tmp == GO_STRAIGHT || direction_tmp1 == TURN_LEFT){  //query wether there is car turns left
              break;
            }
            else{
              int add_car_state;
              add_car_state = crosses[i].UpdateCar(roads, cars, cur_car_id);
              if(add_car_state == FRONT_CAR_WAIT){
                wait_list[i] = roads[next_road_id].start == i?roads[next_road_id].end:roads[next_road_id].start;
                // road_wait_flag[cur_road_id] = true;
                break;
              }
              else if((cars[cur_car_id].state == STOP) && !crosses[i].road_queue[cur_road_id].empty()){
                crosses[i].road_queue[cur_road_id].pop();
                //if(cars[cur_car_id].state == REACHED) cars[cur_car_id].end_time = T_count;
                dead_lock_flag = false;
              }
              else std::cerr<<"it seems some errors occur"<<std::endl;
            }
          }
        }//end for while(cross_finish_flag[i] == false)
      }//end for for(int j=0;...)
      cross_finish_flag[i] = true;
      for(int k = 0; k<crosses[i].valid_roads_num; k++){
        if(crosses[i].road_queue[crosses[i].dispatch_seq[k]].empty() == false ){
          cross_finish_flag[i] = false;
          break;
        }
      }
    }//end for for(int i;...)
    if(dead_lock_flag == true && has_waited_car_flag == true)  DealDealLock(wait_list, crosses, cars, roads);

    all_crosses_finished_flag = true;
    for(int m=0; m<crosses.size(); m++){
      if(cross_finish_flag[m] == false){
        all_crosses_finished_flag = false;
        break;
      }
    }
  }//end for while(1);
}
