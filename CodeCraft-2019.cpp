#include "iostream"
#include "read_data.h"
#include "dijkstra.h"
#include <iomanip>
#include <unordered_map>
#include <algorithm>
#include "codecraft_util.h"
#include "dispatch.h"
#include <vector>
#include <fstream>

using std::ofstream;
using std::vector;
using std::priority_queue;
using std::unordered_map;
void WriteAnswerFile(const vector<Car>& cars, const string& answer_path);
int main(int argc, char *argv[])
{
    std::cout << "Begin" << std::endl;
	
	if(argc < 5){
		std::cout << "please input args: carPath, roadPath, crossPath, answerPath" << std::endl;
		exit(1);
	}
	
	std::string carPath(argv[1]);
	std::string roadPath(argv[2]);
	std::string crossPath(argv[3]);
	std::string answerPath(argv[4]);
	
	std::cout << "carPath is " << carPath << std::endl;
	std::cout << "roadPath is " << roadPath << std::endl;
	std::cout << "crossPath is " << crossPath << std::endl;
	std::cout << "answerPath is " << answerPath << std::endl;
	
	// TODO:read input filebuf
	vector<Road> roads;
  vector<Cross> crosses;
  vector<Car> cars;
  unordered_map<int, priority_queue<int, vector<int>, std::greater<int>>> ready_cars;

  ReadData(carPath, crossPath, roadPath, cars, crosses, roads, ready_cars);

  Graph graph(roads, crosses.size());
  vector<vector<int>> cost_matrix(crosses.size(), vector<int>(crosses.size()+1, INF));
  vector<vector<int>> parent_maxtrix(crosses.size(), vector<int>(crosses.size()+1, INF));
  for(int i = 0; i<crosses.size(); i++){
    graph.Dijkstra(roads, i);
  }

  Dispatch(cars, roads, crosses, graph, ready_cars);

  WriteAnswerFile(cars, answerPath);
  #ifdef DEBUG
    for(int i=0; i<crosses.size(); i++){
      for(int j = 0; j<crosses.size(); j++){
        if(graph.cost_matrix[i][j] == INF){
          std::cout<<std::setw(3)<<-1<<' ';
        }
        else std::cout<<std::setw(3)<<graph.cost_matrix[i][j]<<' ';
      }
      std::cout<<std::endl;
    }
    std::cout<<"parent matrix"<<std::endl;
    for(int i=0; i<crosses.size(); i++){
      for(int j = 0; j<crosses.size(); j++){
        if(graph.parent_matrix[i][j] == INF){
          std::cout<<std::setw(3)<<-1<<' ';
        }
        else std::cout<<std::setw(3)<<graph.parent_matrix[i][j]<<' ';
      }
      std::cout<<std::endl;
    }
  #endif
	// TODO:process
	// TODO:write output file
	
	return 0;
}

void WriteAnswerFile(const vector<Car>& cars, const string& answer_path){
  ofstream ofs(answer_path);
  if(!ofs.good()){
    std::cerr<<"open answer file failed"<<std::endl;
  }
  for(int i=0; i<cars.size(); i++){
    ofs<<'('<<carcount_id[cars[i].id]<<','<<cars[i].true_start_time<<',';//<<cars[i].end_time-cars[i].true_start_time<<','; 
    for(int j=0; j<cars[i].path.size(); j++){
      ofs<<roadcount_id[cars[i].path[j]];
      if(j != cars[i].path.size()-1) ofs<<','; 
    }
    ofs<<')'<<std::endl;
  }
  ofs.close();
}