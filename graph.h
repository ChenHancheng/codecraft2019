#ifndef GRAPH_H_
#define GRAPH_H_




#include <vector>
#include <fstream>
#include <string>

using std::string;
using std::ifstream;
using std::vector;

struct Road
{
  int id;
  int length;
  int speed_limit;
  int channel;
  int start;
  int end;
  bool bidirectional;
};
ifstream& operator>>(ifstream& is, Road& road){
  char a;
  is>>road.id>>a>>road.length>>a>>road.speed_limit>>a>>road.channel>>a>>road.start>>a>>road.end>>a>>road.bidirectional>>a;
  return is;
}



struct Cross
{
  int id;
  int road_id0;
  int road_id1;
  int road_id2;
  int road_id3;
};
ifstream& operator>>(ifstream& is, Cross& cross){
  char a;
  is>>cross.id>>a>>cross.road_id0>>a>>cross.road_id1>>a>>cross.road_id2>>a>>cross.road_id3>>a;
}

struct Car
{
  int id;
  int start;
  int end;
  int speed;
  int plan_time;
};
ifstream& operator>>(ifstream& is, Car& car){
  char a;
  is>>car.id>>a>>car.start>>a>>car.end>>a>>car.speed>>a>>car.plan_time>>a;
}
#endif //GRAPH_H_

