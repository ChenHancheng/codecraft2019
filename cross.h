#ifndef CROSS_H_
#define CROSS_H_

#include <set>
#include <vector>
#include <utility>
#include <algorithm>

#include "car.h"
#include "road.h"

using std::set;
using std::vector;
using std::pair;

class Cross{
 public:
  int id;
  int road_id0;
  int road_id1;
  int road_id2;
  int road_id3;
  
  vector<int> dispatch_seq;
  set<pair<int, int>> go_straight;
  set<pair<int, int>> turn_left;
  set<pair<int, int>> turn_right;
  int valid_roads;
  void CalCost();
  void Cross::init();
  void InitialValue();
};

#endif //CROSS_H_
