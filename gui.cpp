#include <vector>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "road.h"
#include "cross.h"
#include "car.h"

#define WIDTH 8
#define CROSS_RADIUS 10
using std::vector;
int first_time = 0;

using namespace cv;
Mat map_img;

void DrawCross(const vector<Cross>& crosses);

void StateShow(const vector<Road>& roads, const vector<Cross>& crosses, const vector<Car> cars){
  if(first_time == 0){
    map_img = Mat::zeros(1080, 1080, CV_8UC3);
    DrawCross(crosses);
    // DrawRoads(roads);
    first_time++;
  }
  // UpdateRoads(roads, crosses, cars);

}

void DrawCross(const vector<Cross>& crosses){
  for(int i=0; i<WIDTH; i++){
    for(int j=0; j<WIDTH; j++){
      cv::Point point; 
      point.x = i*50 + 100;
      point.y = j*50 + 100;
      circle(map_img, point, 10, Scalar(0, 0, 255), FILLED);
      string idx = std::to_string(i*WIDTH+j);
      putText(map_img, idx, point, FONT_HERSHEY_COMPLEX_SMALL, 1.2, Scalar(255, 0, 0));
      // putText(map_img, "1,", point, font,)
    }
  }
}

// void DrawRoads(const vector<Road>& roads){
//   for(int i=0; i<WIDTH; i++){
//     for(int j=0; j<WIDTH; j++){
//       cv::Point point; 
//       point.x = i*50 + 100;
//       point.y = j*50 + 100;
//       cv::circle(, point, CROSS_RADIUS);
//       put
//     }
//   }
// }

// void UpdateRoads(const vector<Road>& roads, const vector<Cross>& crosses, const vector<Car> cars){
//   for(int i=0; i<roads.size(); i++){
//     for(int j=0; j<roads[i].channel; j++){
//       for(int k = 0; k<roads[i].length; k++){

//       }
//     }
//   }
// }