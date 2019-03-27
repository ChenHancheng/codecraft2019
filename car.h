#ifndef CAR_H_
#define CAR_H_

#include <vector>

#include "data_type.h"
#include "codecraft_util.h"

using std::vector;

class Car {
 public:
  const int id;
  const int start;
  const int end;
  const int speed;
  const int plan_time;

  Car(const CarData& car_data)
      : id(car_data.id),
        start(car_data.start),
        end(car_data.end),
        speed(car_data.speed),
        plan_time(car_data.plan_time) {
    state = IN_GARAGE;
        }
  int true_start_time;
  int end_time;
  int pos;
  int state;
  int next_road_id;
  vector<int> path;
  int current_road_id;
  int channel;
  int direction;
};

#endif  // CAR_H_
