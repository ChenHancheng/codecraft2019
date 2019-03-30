#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

#include "car.h"
#include "codecraft_util.h"
#include "road.h"

using std::max;
using std::min;
using std::priority_queue;
using std::vector;

int Road::AddCar(vector<Car>& cars, int car_id, int direction) {
  int status;
  if (direction == 0) {
    status = AddCarLane(cars, car_id, lane0, lane0_last_car);
    if(status == ADD_SUCCESS) cars_num_road0++;
  } else {
    status = AddCarLane(cars, car_id, lane1, lane1_last_car);
    if(status == ADD_SUCCESS) cars_num_road1++;
  }
  if (status == ADD_SUCCESS) cars[car_id].direction = direction;
  UpdateLastCar(cars);
  return status;
}

int Road::AddCarLane(vector<Car>& cars, int car_id, vector<vector<int>>& lane,
                     vector<int>& lane_last_car) {
  int channelNo = 0;
  while (channelNo < channel && lane[channelNo].back() != POS_BLANK && cars[lane[channelNo].back()].state == STOP) {
    if(cars[lane_last_car[channelNo]].state == WAIT) return FRONT_CAR_WAIT;
    channelNo++;
  }
  if (channelNo == channel){
    if(cars[lane[channelNo-1].back()].state != STOP){
      std::cout<<"asdewiorut";
    }
    return NEXT_ROAD_FULL;
  }

if((min(cars[car_id].speed, speed_limit) - cars[car_id].pos)<=0){
  std::cout<<"asdfasfdsdfagfdg"<<std::endl;
}
  int pos_tmp =
      length - (min(cars[car_id].speed, speed_limit) - cars[car_id].pos);
  // if the last car of this road is in WAIT state and the new added car will
  // collide with the car
  if (lane_last_car[channelNo] != POS_BLANK &&
      pos_tmp <= cars[lane_last_car[channelNo]].pos &&
      cars[lane_last_car[channelNo]].state == WAIT) {
    return FRONT_CAR_WAIT;
  }
  if (lane_last_car[channelNo] != POS_BLANK && cars[lane_last_car[channelNo]].state == STOP) {
    if(cars[lane_last_car[channelNo]].state != STOP && pos_tmp <= cars[lane_last_car[channelNo]].pos ) std::cout<<"some thing wrong";
    cars[car_id].pos = max(pos_tmp, cars[lane_last_car[channelNo]].pos + 1);
  } else
    cars[car_id].pos = pos_tmp;
  if(cars[car_id].pos<0 || cars[car_id].pos>=length){
    std::cout<<"some thing wrong happened when add car"<<std::endl;
  }
  // cars[car_id].state = STOP;
  cars[car_id].channel = channelNo;
  cars[car_id].current_road_id = id;
  cars[car_id].next_road_id = -1;
  cars[car_id].path.push_back(id);

  if (cars[car_id].pos > length || cars[car_id].pos < 0) {
    std::cerr << "position wrong" << std::endl;
  }
  lane[channelNo][cars[car_id].pos] = car_id;
  return ADD_SUCCESS;
}

// calculate how much time it will cost to reach the next cross if we add car
// car_id to the road
int Road::QueryRoadState(vector<Car>& cars, int car_id, int direction) {
  if (direction == 0)
    return QueryRoadStateLane(cars, car_id, lane0, lane0_last_car,
                              lane0_last_time);
  else
    return QueryRoadStateLane(cars, car_id, lane1, lane1_last_car,
                              lane1_last_time);
}

int Road::QueryRoadStateLane(const vector<Car>& cars, int car_id,
                             vector<vector<int>>& lane,
                             vector<int>& lane_last_car,
                             vector<int>& lane_last_time) {
  int channelNo = 0;
  while (channelNo < channel && lane[channelNo].back() == 0) {
    channelNo++;
  }
  if (channelNo == channel)
    return INF/2;
  else
    return max(lane_last_time[channelNo], length / cars[car_id].speed);
}

// update the car which is in WAIT status but will not pass the next cross
void Road::UpdateCar(vector<Car>& cars, const int car_id) {
  if(cars[car_id].state != WAIT) std::cout<<"asdfsadkfhasdfh"<<std::endl;
  if (cars[car_id].direction == 0) {
    UpdateCarLane(cars, car_id, lane0);
  } else {
    UpdateCarLane(cars, car_id, lane1);
  }
  UpdateLastCar(cars);
}
void Road::UpdateCarLane(vector<Car>& cars, const int car_id,
                         vector<vector<int>>& lane) {
  int front_car_pos = -1;
  // lane[cars[car_id].channel][cars[car_id].pos] = POS_BLANK;
  // for (int pos_i = cars[car_id].pos - 1; pos_i >= 0; pos_i--) {
  //   if (lane[cars[car_id].channel][pos_i] != POS_BLANK) {
  //     front_car_pos = pos_i;
  //     if(cars[lane[cars[car_id].channel][pos_i]].state != STOP){
  //       std::cout<<"some thing wrong happened when update car"<<std::endl;
  //     }
  //     break;
  //   }
  // }
  // cars[car_id].pos -= min(cars[car_id].speed, speed_limit);
  // cars[car_id].pos = max(cars[car_id].pos, front_car_pos + 1);
  // if(cars[car_id].pos<0 || cars[car_id].pos>=length){
  //   std::cout<<"some thing wrong happened when update car"<<std::endl;
  // }
  // lane[cars[car_id].channel][cars[car_id].pos] = car_id;
  int channelNo = cars[car_id].channel;
  for(int i=0; i<length; i++){
    if (lane[channelNo][i] != POS_BLANK ) {
      int cur_car_id = lane[channelNo][i];
      if(cars[cur_car_id].state == WAIT){
        lane[channelNo][cars[cur_car_id].pos] = POS_BLANK;
        cars[cur_car_id].pos -= min(cars[cur_car_id].speed, speed_limit);
        cars[cur_car_id].pos = max(cars[cur_car_id].pos, front_car_pos + 1);
        if (front_car_pos != -1 &&
            cars[lane[cars[cur_car_id].channel][front_car_pos]].state != STOP) {
          std::cout << "jglksajgo" << std::endl;
        }
        if (cars[cur_car_id].pos < 0 || cars[cur_car_id].pos >= length) {
          std::cout << "some thing wrong happened when update car" << std::endl;
        }
        cars[cur_car_id].state = STOP;
        lane[channelNo][cars[cur_car_id].pos] = cur_car_id;
        front_car_pos = cars[cur_car_id].pos;
      }
    }
  }
}

// void Road::DeleteCar(vector<Car>& cars, int car_id){
//   if(cars[car_id].direction == 0){
//     DeleteCarLane(cars, car_id, lane0);
//   }
//   else{
//     DeleteCarLane(cars, car_id, lane1);
//   }
//   UpdateLastCar(cars);
// }
// void Road::DeleteCarLane(vector<Car>& cars, int car_id, vector<vector<int>>&
// lane){
//     lane[cars[car_id].channel][cars[car_id].pos] = POS_BLANK;
// }
void Road::DeleteCar(vector<Car>& cars, int car_id) {
  for (int i = 0; i < channel; i++) {
    for (int j = 0; j < length; j++) {
      if (lane0[i][j] == car_id) {
        lane0[i][j] = POS_BLANK;
        cars_num_road0--;
        if(cars_num_road0 < 0) std::cout<<"the number of cars on road is wrong"<<std::endl;
        UpdateChannelLane(cars, lane0, i);
        UpdateLastCar(cars);
        return;
      }
      if (bidirectional == true && lane1[i][j] == car_id) {
        lane1[i][j] = POS_BLANK;
        cars_num_road1--;
        if(cars_num_road1 < 0) std::cout<<"the number of cars on road is wrong"<<std::endl;
        UpdateChannelLane(cars, lane1, i);
        UpdateLastCar(cars);
        return;
      }
    }
  }
  std::cout << "not found car:" << car_id << std::endl;
}

void Road::UpdateChannelLane(vector<Car>& cars,vector<vector<int>>& lane, int channelNo){
  int first_car_id = -1;
  for(int i=0; i<length; i++){
    if(lane[channelNo][i] != POS_BLANK){
      first_car_id = lane[channelNo][i];
      break;
    }
  }
  if(first_car_id == -1 || cars[first_car_id].state == STOP){
    return;
  }
  else{
    if(cars[first_car_id].pos<min(speed_limit, cars[first_car_id].speed) ){
      if(cars[first_car_id].state != WAIT) std::cout<<"asdjfkldjsf";
      return;
    }
    else{
      int front_car_pos = -1;
      for(int j = cars[first_car_id].pos; j<length; j++){
        if(lane[channelNo][j] != POS_BLANK){
          int cur_car_id = lane[channelNo][j];
          if(cars[cur_car_id].state == WAIT){
            lane[channelNo][j] = POS_BLANK;
            cars[cur_car_id].pos -= min(cars[cur_car_id].speed, speed_limit);
            cars[cur_car_id].pos = max(cars[cur_car_id].pos, front_car_pos + 1);
            if(front_car_pos != -1 && cars[lane[channelNo][front_car_pos]].state != STOP){
              std::cout<<"jglksajgo"<<std::endl;
            }
            if(cars[cur_car_id].pos<0 || cars[cur_car_id].pos>=length){
              std::cout<<"some thing wrong happened when update car"<<std::endl;
            }
            cars[cur_car_id].state = STOP;
            lane[channelNo][cars[cur_car_id].pos] = cur_car_id;
            front_car_pos = cars[cur_car_id].pos;
          }
        }
      }
    }
  }
}
void Road::RoadRun(vector<Car>& cars) {
  RoadRunLane(cars, lane0);
  if (bidirectional) {
    RoadRunLane(cars, lane1);
  }
  UpdateLastCar(cars);
}
void Road::RoadRunLane(vector<Car>& cars, vector<vector<int>>& lane) {
  for (int i = 0; i < channel; i++) {
    int front_car_id = -1;
    for (int j = 0; j < length; j++) {
      int car_id = lane[i][j];
      if (car_id != POS_BLANK) {
        int max_speed = min(cars[car_id].speed, speed_limit);
        if (front_car_id == -1) {  // if this car is the first car in the lane
          if (j >= max_speed) {
            cars[car_id].pos = j - max_speed;
            cars[car_id].state = STOP;
            // if(cars[car_id].pos<0 || cars[car_id].pos>=length){
            //   std::cout<<"asdjfklsjdaf";
            // }
            lane[i][j] = POS_BLANK;
            lane[i][cars[car_id].pos] = car_id;
          } else {
            cars[car_id].state = WAIT;
          }
        } else {  // if this car is not the first car in the lane, its state
                  // will be influenced by the car in front of it
          if (cars[front_car_id].state == WAIT) {
            if (j - max_speed > cars[front_car_id].pos) {
              cars[car_id].pos = j - max_speed;
              cars[car_id].state = STOP;
            // if(cars[car_id].pos<0 || cars[car_id].pos>=length){
            //   std::cout<<"asdjfklsjdaf";
            // }
              lane[i][j] = POS_BLANK;
              lane[i][cars[car_id].pos] = car_id;
            } else {
              cars[car_id].state = WAIT;
            }
          }
          else if (cars[front_car_id].state == STOP) {
            cars[car_id].pos =
                std::max(j - max_speed, cars[front_car_id].pos + 1);
            // if(cars[car_id].pos<0 || cars[car_id].pos>=length){
            //   std::cout<<"asdjfklsjdaf";
            // }
            cars[car_id].state = STOP;

            lane[i][j] = POS_BLANK;
            lane[i][cars[car_id].pos] = car_id;
          }
          else {
            std::cout<<"asdfasdl";
          }
        }
        front_car_id = car_id;
      }
    }
  }
}

void Road::UpdateLastCar(const vector<Car>& cars) {
  for (int i = 0; i < lane0.size(); i++) {
    int j = 0;
    for (j = 0; j < length; j++) {
      if (lane0[i][length - j - 1] != POS_BLANK) {
        lane0_last_car[i] = lane0[i][length - j - 1];
        int car_id = lane0[i][length - j - 1];
        lane0_last_time[i] = cars[car_id].pos / cars[car_id].speed;
        break;
      }
    }
    if (j == length) {
      lane0_last_car[i] = POS_BLANK;
      lane0_last_time[i] = 0;
    }
  }
  for (int i = 0; i < lane1.size(); i++) {
    int j = 0;
    for (j = 0; j < length; j++) {
      if (lane1[i][length - 1 - j] != POS_BLANK) {
        lane1_last_car[i] = lane1[i][length - j - 1];
        int car_id = lane1[i][length - j - 1];
        lane1_last_time[i] = cars[car_id].pos / cars[car_id].speed;
        break;
      }
    }
    if (j == length) {
      lane1_last_car[i] = POS_BLANK;
      lane1_last_time[i] = 0;
    }
  }
}