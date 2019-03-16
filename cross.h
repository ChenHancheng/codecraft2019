#ifndef CROSS_H_
#define CROSS_H_

#include <set>
#include <vector>
#include <utility>
#include <algorithm>

#include "car.h"
#include "road.h"
#include "data_type.h"

using std::set;
using std::vector;
using std::pair;

class Cross{
 public:
  int id;
  Cross() = default;
  Cross(const CrossData& cross_data);

  vector<int> dispatch_seq;
  set<pair<int, int>> go_straight;
  set<pair<int, int>> turn_left;
  set<pair<int, int>> turn_right;
  int valid_roads_num;

  void CalCost();
  void init();
  void InitialValue(vector<Road>& roads, vector<Car>& cars, const vector<vector<int>> cost_matrix);
};

#endif //CROSS_H_
