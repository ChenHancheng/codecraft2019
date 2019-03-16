#include <vector>
#include <algorithm>
#include <queue>

#include "road.h"
#include "car.h"
#include "codecraft_util.h"

using std::vector;
using std::priority_queue;

bool Road::AddCar(vector<Car>& cars, int car_id, int direction){
  if(direction == 0){
    int i=0;
    while(i<channel && lane[i].back() == 0){
      i++;
    }
    if(i == channel) return false;
    // if car starts from the garage
    if(cars[car_id].state == IN_GARAGE){
      cars[car_id].pos = std::min(cars[car_id].speed+cars[car_id].pos, cars[lane1_last_pos].pos-1);
      cars[car_id].path.push_back(id);
      cars[car_id].state = STOP;

      // lane[]
    }
    //if car comes from other roads
    else{

    }
  }
}

//calculate how much time it will cost to reach the next cross if we add car:car_id to the road
int Road::NewCarTime(int car_id){

}

void Road::RoadRun(vector<Car>& cars){
  RoadRunLane(cars, lane, lane_wait);
  if(bidirectional){
    RoadRunLane(cars, lane1, lane1_wait);
  }
}

void Road::RoadRunLane(vector<Car>& cars, vector<vector<int>>& lane, priority_queue<int> lane_wait){
  for(int i=0; i<channel; i++){
    for(int j=0; j<length; j++){
      int car_id = lane[i][j];
      int last_car_id = 0;
      if(car_id != 0){
        if(last_car_id == 0){ // if this car is the first car in the lane
            if(cars[car_id].speed+j<=length){
              cars[car_id].state = STOP;
              cars[car_id].pos = cars[car_id].speed+j;

              lane[i][j] = 0;
              lane[i][cars[car_id].pos] = cars[car_id].pos;
            }
            else{
              cars[car_id].state = WAIT;
              lane_wait.push(car_id);
            }
        }
        else { //if this car is not the first car in the lane, its state will be influenced by the car in front of it
          if(cars[last_car_id].state == WAIT){
            if(cars[car_id].speed+j< cars[last_car_id].pos){
              cars[car_id].pos = cars[car_id].speed+j;
              cars[car_id].state = STOP;

              lane[i][j] = 0;
              lane[i][cars[car_id].pos] = cars[car_id].pos;
            }
            else{
              cars[car_id].state = WAIT;
              lane_wait.push(car_id);
            }
          }

          if(cars[last_car_id].state == STOP){
            cars[car_id].pos = std::min(cars[car_id].speed+j, cars[last_car_id].pos-1);
            cars[car_id].state = STOP;
          }
        }
        last_car_id = car_id;
      }
    }
  }
}