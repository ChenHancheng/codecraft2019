#include <set>
#include <vector>
#include <utility>
#include <algorithm>

#include "cross.h"
#include "car.h"
#include "road.h"

using std::set;
using std::vector;
using std::pair;

void Cross::init(){
  if(road_id0 != -1){
    dispatch_seq.push_back(road_id0);
  }
  if(road_id1 != -1){
    dispatch_seq.push_back(road_id1);
  }
  if(road_id2 != -1){
    dispatch_seq.push_back(road_id2);
  }
  if(road_id3 != -1){
    dispatch_seq.push_back(road_id3);
  }

  sort(dispatch_seq.begin(), dispatch_seq.end());
  valid_roads = dispatch_seq.size();

  go_straight.insert({road_id0, road_id2});
  go_straight.insert({road_id2, road_id0});
  go_straight.insert({road_id1, road_id3});
  go_straight.insert({road_id3, road_id1});

  turn_left.insert({road_id0, road_id1});
  turn_left.insert({road_id1, road_id2});
  turn_left.insert({road_id2, road_id3});
  turn_left.insert({road_id3, road_id0});
  
  turn_right.insert({road_id0, road_id3});
  turn_right.insert({road_id3, road_id2});
  turn_right.insert({road_id2, road_id1});
  turn_right.insert({road_id1, road_id0});
}

void Cross::InitialValue(vector<Road>& roads, vector<Car> cars){
  while()
  i = (i+1)%
  roads[dispatch_seq[i]].wait_end.pop();
}
