#include "iostream"
#include "read_data.h"
#include "dijkstra.h"
#include <iomanip>
#include <unordered_map>
#include "codecraft_util.h"
// #include "dispatch.h"
using std::unordered_map;

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
	ReadRoad(roadPath, roads);

  vector<Cross> crosses;
  ReadCross(crossPath, crosses);

  vector<Car> cars;
  ReadCar(carPath, cars);

  unordered_map<int, vector<int>> ready_car;

  Graph graph(roads, crosses.size());
  vector<vector<int>> cost_matrix(crosses.size(), vector<int>(crosses.size()+1, INF));
  vector<vector<int>> parent_maxtrix(crosses.size(), vector<int>(crosses.size()+1, INF));
  for(int i = 0; i<crosses.size(); i++){
    graph.Dijkstra(roads, i, cost_matrix, parent_maxtrix);
  }

  // Dispatch(cars, roads, crosses);

  #ifdef DEBUG
    for(int i=0; i<crosses.size(); i++){
      for(int j = 0; j<crosses.size(); j++){
        if(cost_matrix[i][j] == INF){
          std::cout<<std::setw(3)<<-1<<' ';
        }
        else std::cout<<std::setw(3)<<cost_matrix[i][j]<<' ';
      }
      std::cout<<std::endl;
    }
    std::cout<<"parent matrix"<<std::endl;
    for(int i=0; i<crosses.size(); i++){
      for(int j = 0; j<crosses.size(); j++){
        if(parent_maxtrix[i][j] == INF){
          std::cout<<std::setw(3)<<-1<<' ';
        }
        else std::cout<<std::setw(3)<<parent_maxtrix[i][j]<<' ';
      }
      std::cout<<std::endl;
    }
  #endif
	// TODO:process
	// TODO:write output file
	
	return 0;
}