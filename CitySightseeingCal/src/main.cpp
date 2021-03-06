#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <set>
#include <string.h>
#include "Graph.h"
#include "graphviewer.h"
#include "Person.h"
#include "MapReading.h"
#include "Bus.h"
#include "StringAlgorithms.h"

Graph<int> createGraphUsingPois(vector<int>& poisV, vector<vector<int> >& W);
long int calcDistOfPath(vector<int> path, vector<vector<int> >& W);
vector<int> getAllPath(vector<int>& path, Graph<int>& g);
vector<int> calculatePath(vector<int>& pois, vector<vector<int> >& W);
vector<vector<int> > constructPaths(MapReading& mr, GraphViewer *gv);
vector<vector<int> > getPathsFromUser(MapReading& mr);
vector<int> getPathFromUser(int pathId, MapReading& mr);
vector<Bus> constructBuses(MapReading& mr, vector<vector<int> >& paths);
void printPath(vector<int>& path);
void printColorEdges(GraphViewer *gv, map<int, pair<int,int> >& edges, map<int, pair<double,bool> >& edgesProperties, vector<int> allPath, int val);
void printColorVertex(GraphViewer *gv, vector<int>& path);
void addTourists(vector<Bus>& buses);
void addTourist(vector<Bus>& buses, bool isTheFirstTourist);
void chooseByPOI(vector<Bus>& buses, Person& tourist);
void chooseByPersons(vector<Bus>& buses, Person& tourist);
void printTourists(vector<Bus>& buses);
void showTheTouristsInBuses(vector<Bus>& buses);

int main() {
	MapReading mr;
	mr.makeManualGraph();

	GraphViewer *gv = new GraphViewer(900, 600, false);
	gv->createWindow(600, 600);
	gv->defineEdgeCurved(false);
	mr.sendDataToGraphViewerManual(gv);
	gv->rearrange();

	vector<vector<int> > paths = constructPaths(mr, gv);

	mr.sendVertexLabelsToGraphViewer(gv);

	vector<Bus> buses = constructBuses(mr, paths);

	addTourists(buses);

	return 0;
}

void showTheTouristsInBuses(vector<Bus>& buses){
	for(size_t i = 0;i < buses.size();i++){
		cout << "Turistas no autocarro " << i+1 << endl;
		Bus bus = buses[i];
		vector<Person> touristsInBus = bus.getTourists();
		for(size_t j = 0;j < touristsInBus.size();j++){
			cout << touristsInBus[j].getName() << endl;
		}
		cout << endl;
	}

	cout << "Pressione qualquer tecla para sair" << endl;
	getchar();
}

void addTourists(vector<Bus>& buses){
	system("cls");
	string ans;
	bool isTheFirstTourist = true;
	while(1){
		cout << "1 - Inserir turista" << endl;
		cout << "2 - Mostrar autocarros" << endl;
		cout << "3 - Exit" << endl;
		cout << "Indique a sua opcao: " ;
		getline(cin, ans);

		if(ans == "1"){
			addTourist(buses, isTheFirstTourist);
			isTheFirstTourist = false;
		}
		else if(ans == "2")
			showTheTouristsInBuses(buses);
		else if(ans == "3")
			break;
		else
			cout << "Opcao invalida, tente de novo." << endl;
	}
}

void addTourist(vector<Bus>& buses, bool isTheFirstTourist){
	system("cls");
	string ans;
	cout << "Indique o nome : ";
	getline(cin, ans);
	Person p = Person(ans);

	if(isTheFirstTourist)
		chooseByPOI(buses, p);
	else{
		while(1){
			cout << "Modo de escolha do autocarro" << endl;
			cout << "1 - Escolher por pessoas conhecidas" << endl;
			cout << "2 - Escolher por POIS" << endl;
			cout << "Indique a sua opcao: ";
			getline(cin,ans);
			if(ans == "1"){
				chooseByPersons(buses, p);
				break;
			}
			else if(ans == "2"){
				chooseByPOI(buses, p);
				break;
			}
			else{
				cout << "Opcao invalida, tente de novo." << endl;
			}
		}
	}
}

string minName(string name, vector<Person> tourists){

	string minNome = "";
	int min = 4000;
	int value;
	for(size_t i = 0; i < tourists.size(); i++){
		value = editDistance(name,tourists[i].getName());

		if(value < min){
			min = value;
			minNome = tourists[i].getName();
		}
	}

	return minNome;
}


void chooseByPersons(vector<Bus>& buses, Person& tourist){
	system("cls");
	string name;
	while(1){
		printTourists(buses);
		cout << endl;
		cout << "Escolha o turista com quem quer ir: ";
		getline(cin, name);

		for(size_t i = 0;i < buses.size();i++){
			Bus bus = buses[i];
			vector<Person> touristsInBus = bus.getTourists();
			for(size_t j = 0;j < touristsInBus.size();j++){
				if(touristsInBus[j].getName() == name){
					buses[i].addTourist(tourist);
					return;
				}
			}
		}
		vector<int> values;
		for(size_t j = 0 ; j < buses.size(); j++){
			Bus b = buses[j];
			vector<Person> tourists = b.getTourists();
			string nome = minName(name, tourists);
			boolean state = false;
			while(1){
				cout << "Sera que se estava a referir a : " << nome << endl;
				cout <<"(s/n): ";
				string ans;
				getline(cin, ans);

				if(ans == "s"){
					buses[j].addTourist(tourist);
					return;
				}
				else if(ans == "n"){
					state = true;
					break;
				}
				else{
					cout << "resposta invalida, por favor, volte a tentar" << endl;
				}

			}
			if(state){
				break;
			}
		}
	}



	cout << "Nao foi encontrado nenhum turista com esse nome. Tente de novo." << endl;
}




void chooseByPOI(vector<Bus>& buses, Person& tourist){
	system("cls");
	while(1){
		cout << "Indique o ponto de interesse favorito do turista: ";
		string favoritePoi;
		getline(cin, favoritePoi);
		for(size_t i = 0;i < buses.size();i++){
			string pois = buses[i].getPois();
			if(kmp_matcher(pois, favoritePoi) > 0){
				buses[i].addTourist(tourist);
				return;
			}
		}
		cout << "Nao foi encontrado nenhum caminho que contenha um POI com esse nome. Tente de novo." << endl;
	}
}

void printTourists(vector<Bus>& buses){
	cout << "Turistas ja escolhidos:" << endl;
	for(size_t i = 0;i < buses.size();i++){
		vector<Person> personsInBus = buses[i].getTourists();
		for(size_t j = 0;j < personsInBus.size();j++)
			cout << personsInBus[j].getName() << endl;
	}
}

vector<Bus> constructBuses(MapReading& mr, vector<vector<int> >& paths){
	vector<Bus> buses;
	map<int, string> nameOfNodes = mr.getNameOfNodes();

	for(size_t i = 0;i < paths.size();i++){
		vector<int> path = paths[i];
		Bus bus(path);
		for(size_t j = 0;j < path.size();j++){
			string poiName = nameOfNodes[path[j]];
			bus.addPoi(poiName);
		}
		buses.push_back(bus);
	}

	return buses;
}

vector<vector<int> > constructPaths(MapReading& mr, GraphViewer *gv){
	Graph<int> g = mr.getGraph();
	g.floydWarshallShortestPath();
	vector<vector<int> > W = g.getWeightBetweenAllVertexs();
	vector<vector<int> > paths = getPathsFromUser(mr);

	for(size_t i = 0;i < paths.size();i++){
		vector<int> path = calculatePath(paths[i], W);

		cout << "Caminho " << i+1 << endl;
		vector<int> allPath = getAllPath(path, g);
		printPath(allPath);
		printColorEdges(gv, mr.getEdges(), mr.getEdgesProperties(), allPath, i);
		printColorVertex(gv, path);
		gv->rearrange();

		paths[i] = allPath;
	}
	return paths;
}

vector<vector<int> > getPathsFromUser(MapReading& mr){
	vector<vector<int> > paths;
	int pathId = 0;
	string s;

	cout << "Gerador de autocarros" << endl;
	while(1){
		if(pathId > 0){
			cout << "Pretende adicionar outro autocarro(y/n)? ";
			getline(cin, s);
			if(s == "n")
				break;
		}
		vector<int> path = getPathFromUser(pathId, mr);
		paths.push_back(path);
		pathId++;
	}
	return paths;
}

vector<int> getPathFromUser(int pathId, MapReading& mr){
	vector<int> path;
	string s;
	int max = mr.getNodes().size();

	cout << "Indique os Pois do autocarro " << pathId+1 << endl;
	while(1){
		cout << "Poi de partida: ";
		getline(cin, s);
		int val = atoi(s.c_str());
		if(val > -1 && val < max){
			path.push_back(val);
			break;
		}
		else{
			cout << "Id invalido. Insira novamente" << endl;
		}
	}
	while(1){
		cout << "Poi de chegada: ";
		getline(cin, s);
		int val = atoi(s.c_str());
		if(val > -1 && val < max){
			path.push_back(val);
			break;
		}
		else{
			cout << "Id invalido. Insira novamente" << endl;
		}
	}

	while(1){
		cout << "Indique outro Poi(-1 para terminar): ";
		getline(cin, s);
		if(s == "-1")
			break;
		else{
			int val = atoi(s.c_str());
			if(val > -1 && val < max){
				path.push_back(val);

			}
			else{
				cout << "Id invalido. Insira novamente" << endl;
			}
		}
	}
	return path;
}

vector<int> getAllPath(vector<int>& path, Graph<int>& g){
	vector<int> allPath;
	for(size_t j = 0;j < path.size()-1;j++){
		vector<int> subPath = g.getfloydWarshallPath(path[j], path[j+1]);
		for(size_t k = 0;k < subPath.size();k++){
			if(j > 0 && k == 0)
				continue;
			allPath.push_back(subPath[k]);
		}

	}
	return allPath;
}

void printPath(vector<int>& path){
	for(size_t i = 0;i < path.size();i++){
		if(i%30 == 0 && i!= 0)
			cout << endl;
		cout << path[i] << " ";
	}
	cout << endl;
}

vector<int> calculatePath(vector<int>& pois, vector<vector<int> >& W){
	int idStart = pois[0];
	int idEnd = pois[1];
	Graph<int> graphWithPois = createGraphUsingPois(pois, W);
	return graphWithPois.getPathSalesmanProblem(idStart, idEnd);
}

long int calcDistOfPath(vector<int> path, vector<vector<int> >& W){
	long int d = 0;
	for(size_t i = 1;i < path.size();i++){
		d += W[path[i - 1]][path[i]];
	}
	return d;
}

Graph<int> createGraphUsingPois(vector<int>& poisV, vector<vector<int> >& W){
	Graph<int> g;
	for(size_t k = 0;k < poisV.size();k++)
		g.addVertex(poisV[k]);

	for(size_t k = 0;k < poisV.size();k++)
		for(size_t w = 0;w < poisV.size();w++){
			if(k == w)
				continue;
			int weight = W[poisV[k]][poisV[w]];
			if(weight != 0 && weight != INT_INFINITY)
				g.addEdge(poisV[k],poisV[w],weight);

		}
	return g;
}

void printColorEdges(GraphViewer *gv, map<int, pair<int,int> >& edges, map<int, pair<double,bool> >& edgesProperties, vector<int> allPath, int val){

	vector<string> cores;
	cores.push_back("RED");
	cores.push_back("GREEN");
	cores.push_back("ORANGE");
	cores.push_back("PURPLE");
	cores.push_back("BROWN");
	cores.push_back("YELLOW");
	cores.push_back("PINK");

	for (size_t i = 0; i < allPath.size() - 1; ++i)
		for(size_t j = 0; j < edges.size(); j++){
			if(edges[j].first == allPath[i] && edges[j].second == allPath[i + 1])
				gv->setEdgeColor(j, cores[val%7]);
			if(edgesProperties[j].second == true)
				if(edges[j].second == allPath[i] && edges[j].first == allPath[i + 1])
					gv->setEdgeColor(j, cores[val%7]);
		}
}

void printColorVertex(GraphViewer *gv, vector<int>& path){
	for(size_t i = 0;i<  path.size();i++)
		gv->setVertexColor(path[i], "BLUE");
}
