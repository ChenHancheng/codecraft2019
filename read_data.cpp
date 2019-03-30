#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <algorithm>

#include "data_type.h"
#include "read_data.h"
#include "road.h"
#include "car.h"
#include "cross.h"

using std::priority_queue;
using std::string;
using std::ifstream;
using std::vector;
using std::unordered_map;
using std::sort;

unordered_map<int, int> carid_count;
unordered_map<int, int> carcount_id;

unordered_map<int, int> crossid_count;

unordered_map<int, int> roadid_count;
unordered_map<int, int> roadcount_id;

void ReadData(const string& car_path, const string& cross_path, const string& road_path,
  vector<Car>& cars, vector<Cross>& crosses, vector<Road>& roads, unordered_map<int, priority_queue<int, vector<int>, std::greater<int>>>& ready_cars){
  vector<CarData> cars_data;
  vector<CrossData> crosses_data;
  vector<RoadData> roads_data;

  ReadCar(car_path, cars_data);
  ReadCross(cross_path, crosses_data);
  ReadRoad(road_path, roads_data);
  sort(cars_data.begin(), cars_data.end(), [](CarData a, CarData b){
    return a.id<b.id;
  });

  sort(roads_data.begin(), roads_data.end(), [](RoadData a, RoadData b){
    return a.id<b.id;
  });
  sort(crosses_data.begin(), crosses_data.end(), [](CrossData a, CrossData b){
    return a.id<b.id;
  });
  for(int i=0; i<cars_data.size(); i++){
    cars_data[i].start = crossid_count[cars_data[i].start];
    cars_data[i].end = crossid_count[cars_data[i].end];
    ready_cars[cars_data[i].plan_time].push(cars_data[i].id);

    cars.push_back(Car(cars_data[i]));
  }
    
  for(int i=0; i<crosses_data.size(); i++){
    if(crosses_data[i].road_id0 != -1)
      crosses_data[i].road_id0 = roadid_count[crosses_data[i].road_id0]; 
    if(crosses_data[i].road_id1 != -1)
      crosses_data[i].road_id1 = roadid_count[crosses_data[i].road_id1]; 
    if(crosses_data[i].road_id2 != -1)
      crosses_data[i].road_id2 = roadid_count[crosses_data[i].road_id2]; 
    if(crosses_data[i].road_id3 != -1)
      crosses_data[i].road_id3 = roadid_count[crosses_data[i].road_id3]; 
    crosses.push_back(Cross(crosses_data[i]));
  }
  for(int i=0; i<roads_data.size(); i++){
    roads_data[i].end = crossid_count[roads_data[i].end];
    roads_data[i].start = crossid_count[roads_data[i].start];
    roads.push_back(Road(roads_data[i]));
  }
}
bool ReadRoad(const string& road_path, vector<RoadData>& roads_data){
  ifstream ifs(road_path, std::ifstream::in);
  if(ifs.is_open() == false){
    std::cerr<<"Open road file failed!"<<std::endl;
  }

  RoadData tmp;
  char a;
  int count = 0;
  while(ifs.good()){
    ifs>>a;
    if(a == '#') {
      string discard_line;
      getline(ifs, discard_line);
    }
    else{
      if(ifs>>tmp) {
        roadid_count[tmp.id] = count;
        roadcount_id[count] = tmp.id;
        tmp.id = count; //make id starts from 0
        roads_data.push_back(tmp);
        count++;
      }
    }
  }
  ifs.close();
}

bool ReadCross(const string& cross_path, vector<CrossData>& cross_data){
  ifstream ifs(cross_path, std::ifstream::in);
  if(ifs.is_open() == false){
    std::cerr<<"Open cross file failed!"<<std::endl;
  }

  CrossData tmp;
  char a;
  int count = 0;
  while(ifs.good()){
    ifs>>a;
    if(a == '#') {
      string discard_line;
      getline(ifs, discard_line);
    }
    else{
      if(ifs>>tmp){
        crossid_count[tmp.id] = count;
        tmp.id = count;//make id starts from 0
        cross_data.push_back(tmp);
        count++;
      }
    }
  }
  ifs.close();
}

bool ReadCar(const string& car_path, vector<CarData>& cars_data){
  ifstream ifs(car_path, std::ifstream::in);
  if(ifs.is_open() == false){
    std::cerr<<"Open car file failed!"<<std::endl;
  }

  CarData tmp;
  char a;
  int count = 0;
  while(ifs.good()){
    ifs>>a;
    if(a == '#') {
      string discard_line;
      getline(ifs, discard_line);
    }
    else{
      if(ifs>>tmp) {
        carid_count[tmp.id] = count;
        carcount_id[count] = tmp.id;
        tmp.id = count; //make id starts from 0
        cars_data.push_back(tmp);
        count++;
      }
    }
  }
  ifs.close();
}