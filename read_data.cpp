#include <vector>
#include <fstream>
#include <string>
#include <iostream>

#include "data_type.h"
#include "read_data.h"
#include "road.h"
#include "car.h"
#include "cross.h"

using std::string;
using std::ifstream;
using std::vector;

bool ReadRoad(const string& road_path, vector<Road>& roads){
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
        tmp.id = count; //make id starts from 0
        tmp.start--;
        tmp.end--;
        Road road_tmp(tmp);
        roads.push_back(road_tmp);
        count++;
      }
    }
  }
  ifs.close();
}

bool ReadCross(const string& cross_path, vector<Cross>& cross){
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
        tmp.id = count;//make id starts from 0
        Cross cross_tmp(tmp);
        cross.push_back(cross_tmp);
        count++;
      }
    }
  }
  ifs.close();
}

bool ReadCar(const string& car_path, vector<Car>& cars){
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
        tmp.id = count; //make id starts from 0
        Car car_tmp(tmp);
        cars.push_back(car_tmp);
        count++;
      }
    }
  }
  ifs.close();
}