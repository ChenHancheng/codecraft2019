#ifndef CAR_H_
#define CAR_H_

#include <vector> 

using std::vector;

class Car{
 public:
  int id;
  int start;
  int end;
  int speed;
  int plan_time;

  int pos;
  int state;
  vector<int> path;
};

#endif //CAR_H_
