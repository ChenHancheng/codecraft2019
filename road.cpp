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
    status = AddCarLane(cars, car_id, lane0, lane0_last_car, lane0_last_time);
  } else {
    status = AddCarLane(cars, car_id, lane1, lane1_last_car, lane1_last_time);
  }
  if (status == ADD_SUCCESS) cars[car_id].direction = direction;
  UpdateLastCar(cars);
  return status;
}

int Road::AddCarLane(vector<Car>& cars, int car_id, vector<vector<int>>& lane,
                     vector<int>& lane_last_car, vector<int>& lane_last_time) {
  int channelNo = 0;
  while (channelNo < channel && lane[channelNo].back() != POS_BLANK) {
    channelNo++;
  }
  if (channelNo == channel) return NEXT_ROAD_FULL;

  int pos_tmp =
      length - (min(cars[car_id].speed, speed_limit) - cars[car_id].pos);
  // if the last car of this road is in WAIT state and the new added car will
  // collide with the car
  if (lane_last_car[channelNo] != POS_BLANK &&
      pos_tmp <= cars[lane_last_car[channelNo]].pos &&
      cars[lane_last_car[channelNo]].state == WAIT) {
    return FRONT_CAR_WAIT;
  }
  if (lane_last_car[channelNo] != POS_BLANK) {
    cars[car_id].pos = max(pos_tmp, cars[lane_last_car[channelNo]].pos + 1);
  } else
    cars[car_id].pos = pos_tmp;

  cars[car_id].state = STOP;
  cars[car_id].channel = channelNo;
  cars[car_id].current_road_id = id;
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

int Road::QueryRoadStateLane(vector<Car>& cars, int car_id,
                             vector<vector<int>>& lane,
                             vector<int>& lane_last_car,
                             vector<int>& lane_last_time) {
  int channelNo = 0;
  while (channelNo < channel && lane[channelNo].back() == 0) {
    channelNo++;
  }
  if (channelNo == channel)
    return INF;
  else
    return max(lane_last_time[channelNo], length / cars[car_id].speed);
}

// update the car which is in WAIT status but will not pass the next cross
void Road::UpdateCar(vector<Car>& cars, const int car_id) {
  if (cars[car_id].direction == 0) {
    UpdateCarLane(cars, car_id, lane0, lane0_last_car, lane0_last_time);
  } else {
    UpdateCarLane(cars, car_id, lane1, lane1_last_car, lane1_last_time);
  }
  UpdateLastCar(cars);
}
void Road::UpdateCarLane(vector<Car>& cars, const int car_id,
                         vector<vector<int>>& lane, vector<int>& lane_last_car,
                         vector<int>& lane_last_time) {
  int front_car_pos = -1;
  lane[cars[car_id].channel][cars[car_id].pos] = POS_BLANK;
  for (int pos_i = cars[car_id].pos - 1; pos_i >= 0; pos_i--) {
    if (lane[cars[car_id].channel][pos_i] != POS_BLANK) {
      front_car_pos = cars[lane[cars[car_id].channel][pos_i]].pos;
      break;
    }
  }
  cars[car_id].pos -= min(cars[car_id].speed, speed_limit);
  cars[car_id].pos = max(cars[car_id].pos, front_car_pos + 1);

  lane[cars[car_id].channel][cars[car_id].pos] = car_id;
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
        UpdateLastCar(cars);
        return;
      }
      if (bidirectional == true && lane1[i][j] == car_id) {
        lane1[i][j] = POS_BLANK;
        UpdateLastCar(cars);
        return;
      }
    }
  }
  std::cout << "not found car:" << car_id << std::endl;
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
    int reached_flag = false;
    for (int j = 0; j < length; j++) {
      int car_id = lane[i][j];
      if (car_id != -1) {
        int max_speed = min(cars[car_id].speed, speed_limit);
        if (front_car_id == -1) {  // if this car is the first car in the lane
          if (j >= max_speed) {
            cars[car_id].state = STOP;
            cars[car_id].pos = cars[car_id].pos - max_speed;

            lane[i][j] = POS_BLANK;
            lane[i][cars[car_id].pos] = car_id;
          } else {
            // if the car reach goal, delete the car from the road net
            cars[car_id].state = WAIT;
          }
        } else {  // if this car is not the first car in the lane, its state
                  // will be influenced by the car in front of it
          if (cars[front_car_id].state == WAIT) {
            if (j - max_speed > cars[front_car_id].pos) {
              cars[car_id].pos = j - max_speed;
              cars[car_id].state = STOP;

              lane[i][j] = POS_BLANK;
              lane[i][cars[car_id].pos] = car_id;
            } else {
              cars[car_id].state = WAIT;
              // lane_wait.push(car_id);
            }
          }
          if (cars[front_car_id].state == STOP) {
            cars[car_id].pos =
                std::max(j - max_speed, cars[front_car_id].pos + 1);
            cars[car_id].state = STOP;

            lane[i][j] = POS_BLANK;
            lane[i][cars[car_id].pos] = car_id;
          }
        }
        front_car_id = car_id;
        // if(reached_flag == false) front_car_id = car_id;
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
    int j;
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