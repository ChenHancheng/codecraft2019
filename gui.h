#include <vector>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "road.h"
#include "cross.h"
#include "car.h"

void StateShow(const vector<Road>& roads, vector<Cross>& crosses, const vector<Car> cars);
void WriteMapImg(const vector<Road>& roads, vector<Cross>& crosses, const vector<Car>& cars);
