#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <unordered_map>

#include "data_type.h"
#include "read_data.h"
#include "road.h"
#include "car.h"
#include "cross.h"

using std::string;
using std::ifstream;
using std::vector;
using std::unordered_map;

unordered_map<int, int> carid_count;
unordered_map<int, int> crossid_count;
unordered_map<int, int> roadid_count;

void ReadData(const string& car_path, const string& cross_path, const string& road_path,
  vector<Car>& cars, vector<Cross>& crosses, vector<Road>& roads, unordered_map<int, vector<int>> ready_cars){
  vector<CarData> cars_data;
  vector<CrossData> crosses_data;
  vector<RoadData> roads_data;

  ReadCar(car_path, cars_data);
  ReadCross(cross_path, crosses_data);
  ReadRoad(road_path, roads_data);

  for(int i=0; i<cars_data.size(); i++){
    cars_data[i].start = crossid_count[cars_data[i].start];
    cars_data[i].end = crossid_count[cars_data[i].end];
    ready_cars[cars_data[i].plan_time].push_back(cars_data[i].id);
    cars.push_back(Car(cars_data[i]));
  }
    
  for(int i=0; i<crosses_data.size(); i++){
    crosses_data[i].road_id0 = roads_data[crosses_data[i].road_id0].id; 
    crosses_data[i].road_id1 = roads_data[crosses_data[i].road_id1].id; 
    crosses_data[i].road_id2 = roads_data[crosses_data[i].road_id2].id; 
    crosses_data[i].road_id3 = roads_data[crosses_data[i].road_id3].id; 
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
        tmp.id = count; //make id starts from 0
        cars_data.push_back(tmp);
        count++;
      }
    }
  }
  ifs.close();
}