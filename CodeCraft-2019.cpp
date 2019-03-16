#include "iostream"
#include "graph.h"
#include "dijkstra.h"
#include <iomanip>
#include <unordered_map>

using std::unordered_map;
bool ReadRoad(const string& road_path, vector<Road>& roads);
bool ReadCross(const string& cross_path, vector<Cross>& cross);
bool ReadCar(const string& car_path, vector<Car>& cars);

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

  unordered_map<int, vector<int>> read_car;


  Graph graph(roads, crosses.size());
  vector<vector<int>> cost_matrix(crosses.size()+1, vector<int>(crosses.size()+1, INF));
  vector<vector<int>> parent_maxtrix(crosses.size()+1, vector<int>(crosses.size()+1, INF));
  for(int i = 1; i<=crosses.size(); i++){
    Dijkstra(graph, i, cost_matrix, parent_maxtrix);
  }

  for(int i=0; i<=crosses.size(); i++){
    for(int j = 0; j<=crosses.size(); j++){
      if(cost_matrix[i][j] == INF){
        std::cout<<std::setw(3)<<-1<<' ';
      }
      else std::cout<<std::setw(3)<<cost_matrix[i][j]<<' ';
    }
    std::cout<<std::endl;
  }
  std::cout<<"parent matrix"<<std::endl;
  for(int i=0; i<=crosses.size(); i++){
    for(int j = 0; j<=crosses.size(); j++){
      if(parent_maxtrix[i][j] == INF){
        std::cout<<std::setw(3)<<-1<<' ';
      }
      else std::cout<<std::setw(3)<<parent_maxtrix[i][j]<<' ';
    }
    std::cout<<std::endl;
  }
	// TODO:process
	// TODO:write output file
	
	return 0;
}

bool ReadRoad(const string& road_path, vector<Road>& roads){
  ifstream ifs(road_path, std::ifstream::in);
  if(ifs.is_open() == false){
    std::cerr<<"Open road file failed!"<<std::endl;
  }

  Road tmp;
  char a;
  while(ifs.good()){
    ifs>>a;
    if(a == '#') {
      string discard_line;
      getline(ifs, discard_line);
    }
    else{
      if(ifs>>tmp) roads.push_back(tmp);
    }
  }
}

bool ReadCross(const string& cross_path, vector<Cross>& cross){
  ifstream ifs(cross_path, std::ifstream::in);
  if(ifs.is_open() == false){
    std::cerr<<"Open cross file failed!"<<std::endl;
  }

  Cross tmp;
  char a;
  while(ifs.good()){
    ifs>>a;
    if(a == '#') {
      string discard_line;
      getline(ifs, discard_line);
    }
    else{
      if(ifs>>tmp) cross.push_back(tmp);
    }
  }
}

bool ReadCar(const string& car_path, vector<Car>& cars){
  ifstream ifs(car_path, std::ifstream::in);
  if(ifs.is_open() == false){
    std::cerr<<"Open car file failed!"<<std::endl;
  }

  Car tmp;
  char a;
  while(ifs.good()){
    ifs>>a;
    if(a == '#') {
      string discard_line;
      getline(ifs, discard_line);
    }
    else{
      if(ifs>>tmp) cars.push_back(tmp);
    }
  }
}