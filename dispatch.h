#ifndef DISPATCH_h_
#define DISPATCH_h_

#include <vector>
#include <utility>
#include <unordered_map>

#include "road.h"
#include "cross.h"
#include "car.h"

using std::unordered_map;
using std::pair;
using std::vector;

void Dispatch(vector<Car>& cars, vector<Road>& roads, vector<Cross>& crosses){
  int roads_num = roads.size();
  int crosses_num = crosses.size();
  int cars_num = cars.size();
  int T_count=0;
  while(1){//not all car finished
    for(int i=0; i<roads_num; i++){
      roads[i].RoadRun(cars);
    }

    vector<Road> roads_tmp(roads);
    vector<Car> cars_tmp(cars);
    // vector<pair<int, int>> solution;
    // vector<pair<int, int>> init_solution;

    for(int i=0; i<crosses_num; i++){
     crosses[i].InitialValue(roads_tmp, cars_tmp);
    }

    //tabu(cars, roads, crossed, solution);
    RocordSlution(cars, cars_tmp, roads, roads_tmp);
    AddNewCar(T_count, cars, ready_car, roads);

    T_count++;
  }
}
// add those cars which should launch at time T_count
void AddNewCar(int T_count, vector<Car>& cars, unordered_map<int, vector<int>>& ready_car, vector<Road> roads){
  int ready_car_num = ready_car[T_count].size();
  for(int i = 0; i<ready_car_num; i++){
    int car_id = ready_car[T_count][i];
    int cross_id = parent_matrix[cars[car_id].start][cars[car_id].end];
    int road_id = global_map[cars[car_id].start][cross_id];
    if(cross_id == roads[road_id].end){
      if( roads[road_id].AddCar(cars, car_id, 0) == false){ //if the car fail to launch at time T_count, then try to launch it in time T_count+1  
        ready_car[T_count+1].push_back(car_id);
      }
    }
    else{
      if(roads[road_id].AddCar(cars, car_id, 1) == false){
        ready_car[T_count+1].push_back(car_id);
      }
    }
  }
}

void RecordSulution(vector<Car>& cars, vector<Car>& cars_tmp, vector<Road>& roads, vector<Road>& road_tmp){
  for(int i =0; i<cars.size(); i++){
    cars[i] = cars_tmp[i];
  }
  for(int i=0; i<roads.size(); i++){
     roads[i] = road_tmp[i];
  }
}


#endif //DISPATCH_h_