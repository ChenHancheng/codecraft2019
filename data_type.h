#ifndef DATATYPE_H_
#define DATATYPE_H_

#include <vector>
#include <fstream>
#include <string>
#include <iostream>

using std::string;
using std::ifstream;
using std::vector;

struct RoadData
{
  int id;
  int length;
  int speed_limit;
  int channel;
  int start;
  int end;
  bool bidirectional;
};
inline ifstream& operator>>(ifstream& is, RoadData& road_data){
  char a;
  is>>road_data.id>>a>>road_data.length>>a>>road_data.speed_limit>>a>>road_data.channel>>a>>road_data.start>>a>>road_data.end>>a>>road_data.bidirectional>>a;
  return is;
}


struct CrossData
{
  int id;
  int road_id0;
  int road_id1;
  int road_id2;
  int road_id3;
};
inline  ifstream& operator>>(ifstream& is, CrossData& cross_data){
  char a;
  is>>cross_data.id>>a>>cross_data.road_id0>>a>>cross_data.road_id1>>a>>cross_data.road_id2>>a>>cross_data.road_id3>>a;
  return is;
}


struct CarData
{
  int id;
  int start;
  int end;
  int speed;
  int plan_time;
};
inline ifstream& operator>>(ifstream& is, CarData& car_data){
  char a;
  is>>car_data.id>>a>>car_data.start>>a>>car_data.end>>a>>car_data.speed>>a>>car_data.plan_time>>a;
  return is;
}
#endif //DATATYPE_H_

