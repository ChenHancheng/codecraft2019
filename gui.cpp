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
Mat map_img(1000, 1000, CV_8UC3, Scalar(255, 255, 255));
#define RED Scalar(0, 50, 250)
#define GREEN Scalar(20, 200, 20)
#define ORANGE Scalar(200, 200, 100)
#define BLUE Scalar(200, 20, 20)

void DrawCross(const vector<Cross>& crosses);
void DrawRoads(const vector<Road>& roads);
void UpdateRoads(const vector<Road>& roads, vector<Cross>& crosses, const vector<Car>& cars);
void WriteMapImg(const vector<Road>& roads, vector<Cross>& crosses, const vector<Car>& cars){
  UpdateRoads(roads, crosses, cars);
  imwrite("../dead_lock.png", map_img);
}

void StateShow(const vector<Road>& roads, vector<Cross>& crosses, const vector<Car> cars){
  if(first_time == 0){
    // map_img = Mat::ones(1000, 1000, CV_8UC3);
    DrawRoads(roads);
    DrawCross(crosses);
    first_time++;
  }
  UpdateRoads(roads, crosses, cars);
  imshow("map", map_img);
  waitKey(0);
}

void DrawCross(const vector<Cross>& crosses){
  for(int i=0; i<WIDTH; i++){
    for(int j=0; j<WIDTH; j++){
      cv::Point point; 
      point.y = i*100 + 100;
      point.x = j*100 + 100;
      circle(map_img, point, 10, ORANGE, FILLED);
      string idx = std::to_string(i*WIDTH+j);
      point.y = i*100 + 105;
      point.x = j*100 + 92;
      putText(map_img, idx, point, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
    }
  }
}

void DrawRoads(const vector<Road>& roads){
  for(int i=0; i<roads.size(); i++){
      int start = roads[i].start;
      int end = roads[i].end;
      cv::Point p1, p2; 
      p1.x = (start%WIDTH)*100 + 100;
      p1.y = (start/WIDTH)*100 + 100;

      p2.x = (end%WIDTH)* 100 + 100;
      p2.y = (end/WIDTH)*100 + 100;
      line(map_img, p1, p2, Scalar(00, 0, 00));

      int rec_height = 3;
      for(int j = 0; j<roads[i].channel; j++){
        int rec_len = 80/roads[i].length;
        for(int k=0; k<roads[i].length; k++){
          if(roads[i].start+1 == roads[i].end){
            Point rec_p1, rec_p2;
            rec_p1.x = p2.x-k*rec_len-10;
            rec_p1.y = p2.y - (roads[i].channel-j)*rec_height;
            rec_p2.x = p2.x-(k+1)*rec_len-10;
            rec_p2.y = p2.y - (roads[i].channel-j-1)*rec_height;
            rectangle(map_img, rec_p1, rec_p2, Scalar(0, 0, 0));
            if(roads[i].bidirectional == true){
              rec_p1.x = p1.x+k*rec_len+10;
              rec_p1.y = p1.y+(roads[i].channel-j)*rec_height;
              rec_p2.x = p1.x+(k+1)*rec_len+10;
              rec_p2.y = p1.y+(roads[i].channel-j-1)*rec_height;
              rectangle(map_img, rec_p1, rec_p2, Scalar(255, 0, 0));
            }
          }
          else{
            Point rec_p1, rec_p2;
            rec_p1.y = p2.y-k*rec_len-10;
            rec_p1.x = p2.x - (roads[i].channel-j)*rec_height;
            rec_p2.y = p2.y-(k+1)*rec_len-10;
            rec_p2.x = p2.x - (roads[i].channel-j-1)*rec_height;
            rectangle(map_img, rec_p1, rec_p2, Scalar(0, 0, 0));
            if(roads[i].bidirectional == true){
              rec_p1.y = p1.y+k*rec_len+10;
              rec_p1.x = p1.x+(roads[i].channel-j)*rec_height;
              rec_p2.y = p1.y+(k+1)*rec_len+10;
              rec_p2.x = p1.x+(roads[i].channel-j-1)*rec_height;
              rectangle(map_img, rec_p1, rec_p2, Scalar(255, 0, 0));
            }
          }
        }
      }
  }
}

void UpdateRoads(const vector<Road>& roads, vector<Cross>& crosses, const vector<Car>& cars){
  for(int i=0; i<roads.size(); i++){
    int start = roads[i].start;
    int end = roads[i].end;
    cv::Point p1, p2; 
    p1.x = (start%WIDTH)*100 + 100;
    p1.y = (start/WIDTH)*100 + 100;

    p2.x = (end%WIDTH)* 100 + 100;
    p2.y = (end/WIDTH)*100 + 100;
    // line(map_img, p1, p2, Scalar(00, 0, 00));

    int rec_height = 3;
    for(int j = 0; j<roads[i].channel; j++){
      int rec_len = 80/roads[i].length;
      for(int k=0; k<roads[i].length; k++){
        if(roads[i].start+1 == roads[i].end){
          Point rec_p1, rec_p2;
          rec_p1.x = p2.x-k*rec_len-10;
          rec_p1.y = p2.y - (roads[i].channel-j)*rec_height;
          rec_p2.x = p2.x-(k+1)*rec_len-10;
          rec_p2.y = p2.y - (roads[i].channel-j-1)*rec_height;
          if(roads[i].lane0[j][k] != -1) {
            int car_id = roads[i].lane0[j][k];
            if(cars[car_id].state == STOP) rectangle(map_img, rec_p1, rec_p2, GREEN,FILLED);
            else{
              int next_road_id = cars[car_id].next_road_id;
              int cross_id;
              if(roads[i].end == roads[next_road_id].start || roads[i].end == roads[next_road_id].end){
                cross_id = roads[i].end;
              }
              else if(roads[i].start == roads[next_road_id].start || roads[i].start == roads[next_road_id].end){
                cross_id = roads[i].start;
              }
              int direction = crosses[cross_id].turn_direction[{i, next_road_id}];
              if(direction == GO_STRAIGHT){
                rectangle(map_img, rec_p1, rec_p2, ORANGE,FILLED);
              }
              else if (direction == TURN_LEFT){
                rectangle(map_img, rec_p1, rec_p2, BLUE,FILLED);
              }
              else{
                rectangle(map_img, rec_p1, rec_p2, RED,FILLED);
              }
              
            }
              // rectangle(map_img, rec_p1, rec_p2, Scalar(255, 0, 0));
          }
          else 
          {
            rectangle(map_img, rec_p1, rec_p2, Scalar(255, 255, 255),FILLED);
            rectangle(map_img, rec_p1, rec_p2, Scalar(0, 0, 0));
          }
          if(roads[i].bidirectional == true){
            rec_p1.x = p1.x+k*rec_len+10;
            rec_p1.y = p1.y+(roads[i].channel-j)*rec_height;
            rec_p2.x = p1.x+(k+1)*rec_len+10;
            rec_p2.y = p1.y+(roads[i].channel-j-1)*rec_height;
            if(roads[i].lane1[j][k] != -1) {
                          int car_id = roads[i].lane1[j][k];
            if(cars[car_id].state == STOP) rectangle(map_img, rec_p1, rec_p2, GREEN,FILLED);
            else{
              int next_road_id = cars[car_id].next_road_id;
              int cross_id;
              if(roads[i].end == roads[next_road_id].start || roads[i].end == roads[next_road_id].end){
                cross_id = roads[i].end;
              }
              else if(roads[i].start == roads[next_road_id].start || roads[i].start == roads[next_road_id].end){
                cross_id = roads[i].start;
              }
              int direction = crosses[cross_id].turn_direction[{i, next_road_id}];
              if(direction == GO_STRAIGHT){
                rectangle(map_img, rec_p1, rec_p2, ORANGE,FILLED);
              }
              else if (direction == TURN_LEFT){
                rectangle(map_img, rec_p1, rec_p2, BLUE,FILLED);
              }
              else{
                rectangle(map_img, rec_p1, rec_p2, RED,FILLED);
              }
            }
            }
            else 
            {
              rectangle(map_img, rec_p1, rec_p2, Scalar(255, 255, 255),FILLED);
              rectangle(map_img, rec_p1, rec_p2, Scalar(255, 0, 0));
            }
          }
        }
        else{
          Point rec_p1, rec_p2;
          rec_p1.y = p2.y-k*rec_len-10;
          rec_p1.x = p2.x - (roads[i].channel-j)*rec_height;
          rec_p2.y = p2.y-(k+1)*rec_len-10;
          rec_p2.x = p2.x - (roads[i].channel-j-1)*rec_height;
          if(roads[i].lane0[j][k] != -1) {
                          int car_id = roads[i].lane0[j][k];
            if(cars[car_id].state == STOP) rectangle(map_img, rec_p1, rec_p2, GREEN,FILLED);
            else{
              int next_road_id = cars[car_id].next_road_id;
              int cross_id;
              if(roads[i].end == roads[next_road_id].start || roads[i].end == roads[next_road_id].end){
                cross_id = roads[i].end;
              }
              else if(roads[i].start == roads[next_road_id].start || roads[i].start == roads[next_road_id].end){
                cross_id = roads[i].start;
              }
              int direction = crosses[cross_id].turn_direction[{i, next_road_id}];
              if(direction == GO_STRAIGHT){
                rectangle(map_img, rec_p1, rec_p2, ORANGE,FILLED);
              }
              else if (direction == TURN_LEFT){
                rectangle(map_img, rec_p1, rec_p2, BLUE,FILLED);
              }
              else{
                rectangle(map_img, rec_p1, rec_p2, RED,FILLED);
              }
            }
          }
          else 
          {
            rectangle(map_img, rec_p1, rec_p2, Scalar(255, 255, 255),FILLED);
            rectangle(map_img, rec_p1, rec_p2, Scalar(0, 0, 0));
          }
          if(roads[i].bidirectional == true){
            rec_p1.y = p1.y+k*rec_len+10;
            rec_p1.x = p1.x+(roads[i].channel-j)*rec_height;
            rec_p2.y = p1.y+(k+1)*rec_len+10;
            rec_p2.x = p1.x+(roads[i].channel-j-1)*rec_height;
            if(roads[i].lane1[j][k] != -1) {
                          int car_id = roads[i].lane1[j][k];
            if(cars[car_id].state == STOP) rectangle(map_img, rec_p1, rec_p2, GREEN,FILLED);
            else{
              int next_road_id = cars[car_id].next_road_id;
              int cross_id;
              if(roads[i].end == roads[next_road_id].start || roads[i].end == roads[next_road_id].end){
                cross_id = roads[i].end;
              }
              else if(roads[i].start == roads[next_road_id].start || roads[i].start == roads[next_road_id].end){
                cross_id = roads[i].start;
              }
              int direction = crosses[cross_id].turn_direction[{i, next_road_id}];
              if(direction == GO_STRAIGHT){
                rectangle(map_img, rec_p1, rec_p2, ORANGE,FILLED);
              }
              else if (direction == TURN_LEFT){
                rectangle(map_img, rec_p1, rec_p2, BLUE,FILLED);
              }
              else{
                rectangle(map_img, rec_p1, rec_p2, RED,FILLED);
              }
            }
            }
            else 
            {
              rectangle(map_img, rec_p1, rec_p2, Scalar(255, 255, 255),FILLED);
              rectangle(map_img, rec_p1, rec_p2, Scalar(255, 0, 0));
            }
          }
        }
      }
    }
    }
}