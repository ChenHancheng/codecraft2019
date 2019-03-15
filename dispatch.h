
#include <vector>

#include "road.h"
#include "cross.h"

using std::vector;

void Dispath(vector<Car> cars, vector<Road> roads, vector<Cross> crosses){
  int roads_num = roads.size();
  int crosses_num = crosses.size();
  int cars_num = cars.size();
  while(){
    for(int i=0; i<roads_num; i++){
      roads[i].RoadRun(cars);
    }
  }




}