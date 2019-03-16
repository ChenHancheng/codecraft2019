#ifndef CAR_H_
#define CAR_H_

#include <vector>

#include "data_type.h"

using std::vector;

class Car {
 public:
  int id;
  int start;
  int end;
  int speed;
  int plan_time;

  Car(const CarData& car_data)
      : id(car_data.id),
        start(car_data.start),
        end(car_data.end),
        speed(car_data.speed),
        plan_time(car_data.plan_time) {}
  Car()=default;
  int pos;
  int state;
  vector<int> path;
};

#endif  // CAR_H_
