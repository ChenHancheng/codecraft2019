#include <vector>
#include <algorithm>
#include <queue>
#include <iostream>

#include "road.h"
#include "car.h"
#include "codecraft_util.h"

using std::vector;
using std::priority_queue;
using std::max;
using std::min;

int Road::AddCar(vector<Car>& cars, int car_id, int direction){
  int status;
  if(direction == 0){
    status = AddCarLane(cars, car_id, lane0, lane0_last_car, lane0_last_time);
  }
  else{
    status = AddCarLane(cars, car_id, lane1, lane1_last_car, lane1_last_time);
  }
  UpdateLastCar(cars);
  cars[car_id].direction = direction;
  cars[car_id].path.push_back(id);
  return status;
}

int Road::AddCarLane(vector<Car>& cars, int car_id, vector<vector<int>>& lane, vector<int>& lane_last_car, vector<int>& lane_last_time){
  int channelNo=0;
  while(channelNo<channel && lane[channelNo].back() != -1){
    channelNo++;
  }
  if(channelNo == channel) return NEXT_ROAD_FULL;

  if(lane_last_car[channelNo] != POS_BLANK){
    if(cars[lane_last_car[channelNo]].state == WAIT) return FRONT_CAR_WAIT;
  }
  int pos_tmp = -min(cars[car_id].speed, speed_limit)+cars[car_id].pos;
  if(lane_last_car[channelNo] != POS_BLANK){
    cars[car_id].pos = max(pos_tmp, cars[lane_last_car[channelNo]].pos+1);
  }
  else cars[car_id].pos = pos_tmp;

  cars[car_id].state = STOP;
  cars[car_id].channel = channelNo;
  cars[car_id].current_road_id = id;

  if(cars[car_id].pos >length || cars[car_id].pos<0){
    std::cerr<<"position wrong"<<std::endl;
  }
  lane[channelNo][cars[car_id].pos] = car_id;
  // lane_last_car[channelNo] = cars[car_id].pos;
  // lane_last_time[channelNo] = max(lane_last_time[channelNo], length/cars[car_id].speed);
  return ADD_SUCCESS;
}

//calculate how much time it will cost to reach the next cross if we add car car_id to the road
int Road::QueryRoadState(vector<Car>& cars, int car_id, int direction){
    if(direction == 0) return QueryRoadStateLane(cars, car_id, lane0, lane0_last_car, lane0_last_time);
    else return QueryRoadStateLane(cars, car_id, lane1, lane1_last_car, lane1_last_time);
}

int Road::QueryRoadStateLane(vector<Car>& cars, int car_id, vector<vector<int>>& lane, vector<int>& lane_last_car, vector<int>& lane_last_time){
    int channelNo=0;
    while(channelNo<channel && lane[channelNo].back() == 0){
      channelNo++;
    }
    if(channelNo == channel) return INF;
    else return max(lane_last_time[channelNo], length/cars[car_id].speed);
}



void Road::UpdateCar(vector<Car>& cars, int car_id){
  if(cars[car_id].direction == 0){
    UpdateCarLane(cars, car_id, lane0, lane0_last_car, lane0_last_time);
  }
  else{
    UpdateCarLane(cars, car_id, lane1, lane1_last_car, lane1_last_time);
  }
  UpdateLastCar(cars);
}
void Road::UpdateCarLane(vector<Car>& cars, int car_id, vector<vector<int>>& lane,
                vector<int>& lane_last_car, vector<int>& lane_last_time){
    bool is_last;
    if(cars[car_id].pos == lane_last_car[cars[car_id].channel]){
      is_last = true;
    }
    lane[cars[car_id].channel][cars[car_id].pos] = 0;
    cars[car_id].pos -= min(cars[car_id].speed, speed_limit); //TODO:the car may come with the front car of it;

    lane[cars[car_id].channel][cars[car_id].pos] = car_id;
  //   if(is_last == true){
  //     lane_last_car[cars[car_id].channel] = cars[car_id].pos;
  //     lane_last_time[cars[car_id].channel]--;
  // }
}

void Road::DeleteCar(vector<Car>& cars, int car_id){
  if(cars[car_id].direction == 0){
    DeleteCarLane(cars, car_id, lane0, lane0_last_car, lane0_last_time);
  }
  else{
    DeleteCarLane(cars, car_id, lane1, lane1_last_car, lane1_last_time);
  }
  UpdateLastCar(cars);
}
void Road::DeleteCarLane(vector<Car>& cars, int car_id, vector<vector<int>>& lane,
                vector<int>& lane_last_car, vector<int>& lane_last_time){
    bool is_last;
    if(cars[car_id].pos == lane_last_car[cars[car_id].channel]){
      is_last = true;
    }
    lane[cars[car_id].channel][cars[car_id].pos] = 0;
  //   if(is_last == true){
  //     lane_last_car[cars[car_id].channel] = 0;
  //     lane_last_time[cars[car_id].channel] = 0;
  // }
}

void Road::RoadRun(vector<Car>& cars){
  RoadRunLane(cars, lane0);
  if(bidirectional){
    RoadRunLane(cars, lane1);
  }
  UpdateLastCar(cars);
}

void Road::RoadRunLane(vector<Car>& cars, vector<vector<int>>& lane){
  for(int i=0; i<channel; i++){
    int front_car_id = -1;
    for(int j=0; j<length; j++){
      int car_id = lane[i][j];
      if(car_id != -1){
        int max_speed = min(cars[car_id].speed, speed_limit);
        if(front_car_id == -1){ // if this car is the first car in the lane
            if(j >= max_speed){
              cars[car_id].state = STOP;
              cars[car_id].pos = cars[car_id].pos-max_speed;

              lane[i][j] = POS_BLANK;
              lane[i][cars[car_id].pos] = car_id;
            }
            else{
              cars[car_id].state = WAIT;
            }
        }
        else { //if this car is not the first car in the lane, its state will be influenced by the car in front of it
          if(cars[front_car_id].state == WAIT){
            if(j-max_speed > cars[front_car_id].pos){
              cars[car_id].pos = j-max_speed;
              cars[car_id].state = STOP;

              lane[i][j] = POS_BLANK;
              lane[i][cars[car_id].pos] = car_id;
            }
            else{
              cars[car_id].state = WAIT;
              // lane_wait.push(car_id);
            }
          }

          if(cars[front_car_id].state == STOP){
            cars[car_id].pos = std::max(j-max_speed, cars[front_car_id].pos+1);
            cars[car_id].state = STOP;

            lane[i][j] = POS_BLANK;
            lane[i][cars[car_id].pos] = car_id;
          }
        }
        front_car_id = car_id;
      }
    }
  }
}