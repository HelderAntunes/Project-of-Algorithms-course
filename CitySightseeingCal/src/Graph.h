/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include <algorithm>
#include <climits>
#include <stack>
#include <set>


using namespace std;

template <class T> class Edge;
template <class T> class Graph;

const int NOT_VISITED = 0;
const int BEING_VISITED = 1;
const int DONE_VISITED = 2;
const int INT_INFINITY = INT_MAX;

/*
 * ================================================================================================
 * Class Vertex
 * ================================================================================================
 */
template <class T>
class Vertex {
	T info;
	vector<Edge<T>  > adj;
	bool visited;
	bool processing;
	bool addedToHeap;
	int indegree;
	int dist;
	int low;
	int num;
	Vertex<T>* parent = NULL;
public:

	Vertex(T in);
	friend class Graph<T>;

	void addEdge(Vertex<T> *dest, double w);
	bool removeEdgeTo(Vertex<T> *d);

	T getInfo() const;
	void setInfo(T info);

	int getDist() const;
	int getIndegree() const;

	Vertex* path;
};


template <class T>
struct vertex_greater_than {
	bool operator()(Vertex<T> * a, Vertex<T> * b) const {
		return a->getDist() > b->getDist();
	}
};


template <class T>
bool Vertex<T>::removeEdgeTo(Vertex<T> *d) {
	d->indegree--; //adicionado do exercicio 5
	typename vector<Edge<T> >::iterator it= adj.begin();
	typename vector<Edge<T> >::iterator ite= adj.end();
	while (it!=ite) {
		if (it->dest == d) {
			adj.erase(it);
			return true;
		}
		else it++;
	}
	return false;
}

//atualizado pelo exerc�cio 5
template <class T>
Vertex<T>::Vertex(T in): info(in), visited(false), processing(false), indegree(0), dist(0) {
	path = NULL;
	addedToHeap = false;
}


template <class T>
void Vertex<T>::addEdge(Vertex<T> *dest, double w) {
	Edge<T> edgeD(dest,w);
	adj.push_back(edgeD);
}


template <class T>
T Vertex<T>::getInfo() const {
	return this->info;
}

template <class T>
int Vertex<T>::getDist() const {
	return this->dist;
}


template <class T>
void Vertex<T>::setInfo(T info) {
	this->info = info;
}

template <class T>
int Vertex<T>::getIndegree() const {
	return this->indegree;
}




/* ================================================================================================
 * Class Edge
 * ================================================================================================
 */
template <class T>
class Edge {
	Vertex<T> * dest;
	double weight;
	string roadName;
public:
	Edge(Vertex<T> *d, double w);
	friend class Graph<T>;
	friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w){}





/* ================================================================================================
 * Class Graph
 * ================================================================================================
 */
template <class T>
class Graph {
	vector<Vertex<T> *> vertexSet;
	void dfs(Vertex<T> *v, vector<T> &res) const;
	vector<vector<int> > W;
	vector<vector<int> > P;
	int counter;

	//exercicio 5
	int numCycles;
	void dfsVisit(Vertex<T> *v);
	void dfsVisit();
	void getPathTo(Vertex<T> *origin, list<T> &res);

	void putInStackByPosOrder_SCC(Vertex<T>* v, stack<T>& stack);
	void printOneComponent_SCC(Vertex<T>* v, set<T>& component);

public:
	bool addVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, double w);
	bool removeVertex(const T &in);
	bool removeEdge(const T &sourc, const T &dest);
	vector<T> dfs() const;
	vector<T> bfs(Vertex<T> *v) const;
	int maxNewChildren(Vertex<T> *v, T &inf) const;
	vector<Vertex<T> * > getVertexSet() const;
	int getNumVertex() const;

	//exercicio 5
	Vertex<T>* getVertex(const T &v) const;
	void resetIndegrees();
	vector<Vertex<T>*> getSources() const;
	int getNumCycles();
	vector<T> topologicalOrder();
	vector<T> getPath(const T &origin, const T &dest);
	void unweightedShortestPath(const T &v);
	bool isDAG();

	void bellmanFordShortestPath(const T &s);
	void dijkstraShortestPath(const T &s);
	void floydWarshallShortestPath();
	int edgeCost(int i, int j);
	vector<T> getfloydWarshallPath(const T &origin, const T &dest);

	vector<vector<T> > getWeightBetweenAllVertexs();
	vector<T> getPathSalesmanProblem(T idStart,T idEnd);
	void salesmanProblemAux(Vertex<T> *vertexToProcess,Vertex<T> *endVertex, vector<T> &res, int numberEdgesVisited);

	bool isConnected();
	vector<set<T> > getStrongestConnectedComponents();
	Graph<T> getReversedGraph();

	void findArt(T info, vector<T>& artNodes);
	void findArtAux(Vertex<T>* v, vector<T>& artNodes);

};

template <class T>
void Graph<T>::findArt(T info, vector<T>& artNodes){
	for(size_t i = 0;i < vertexSet.size();i++)
		vertexSet[i]->visited = false;
	Vertex<T>* v = this->getVertex(info);
	counter = 1;
	findArtAux(v, artNodes);
}

template <class T>
void Graph<T>::findArtAux(Vertex<T>* v, vector<T>& artNodes){
	v->visited = true;
	v->num = v->low = counter++;
	for(size_t j = 0;j < v->adj.size();j++){
		Vertex<T>* w = v->adj[j].dest;
		if(w->visited == false){
			w->parent = v;
			findArtAux(w, artNodes);
			if(w->low >= v->num){
				artNodes.push_back(v->info);
			}
			v->low = min(v->low, w->low);
		}
		else{
			if(v->parent != w){
				v->low = min(v->low, w->num);
			}
		}
	}

}




template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
}
template <class T>
vector<Vertex<T> * > Graph<T>::getVertexSet() const {
	return vertexSet;
}

template <class T>
int Graph<T>::getNumCycles() {
	numCycles = 0;
	dfsVisit();
	return this->numCycles;
}

template <class T>
bool Graph<T>::isDAG() {
	return (getNumCycles() == 0);
}

template <class T>
bool Graph<T>::addVertex(const T &in) {
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	for (; it!=ite; it++)
		if ((*it)->info == in) return false;
	Vertex<T> *v1 = new Vertex<T>(in);
	vertexSet.push_back(v1);
	return true;
}

template <class T>
bool Graph<T>::removeVertex(const T &in) {
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	for (; it!=ite; it++) {
		if ((*it)->info == in) {
			Vertex<T> * v= *it;
			vertexSet.erase(it);
			typename vector<Vertex<T>*>::iterator it1= vertexSet.begin();
			typename vector<Vertex<T>*>::iterator it1e= vertexSet.end();
			for (; it1!=it1e; it1++) {
				(*it1)->removeEdgeTo(v);
			}

			typename vector<Edge<T> >::iterator itAdj= v->adj.begin();
			typename vector<Edge<T> >::iterator itAdje= v->adj.end();
			for (; itAdj!=itAdje; itAdj++) {
				itAdj->dest->indegree--;
			}
			delete v;
			return true;
		}
	}
	return false;
}

template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	int found=0;
	Vertex<T> *vS, *vD;
	while (found!=2 && it!=ite ) {
		if ( (*it)->info == sourc )
		{ vS=*it; found++;}
		if ( (*it)->info == dest )
		{ vD=*it; found++;}
		it ++;
	}
	if (found!=2) return false;
	vD->indegree++;
	vS->addEdge(vD,w);

	return true;
}

template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	int found=0;
	Vertex<T> *vS, *vD;
	while (found!=2 && it!=ite ) {
		if ( (*it)->info == sourc )
		{ vS=*it; found++;}
		if ( (*it)->info == dest )
		{ vD=*it; found++;}
		it ++;
	}
	if (found!=2)
		return false;

	vD->indegree--;

	return vS->removeEdgeTo(vD);
}

template <class T>
bool Graph<T>::isConnected(){
	typename vector<Vertex<T>*>::const_iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::const_iterator ite= vertexSet.end();
	for (; it !=ite; it++)
		(*it)->visited=false;
	Vertex<T> *v = vertexSet[0];
	vector<T> nodesVisited;
	dfs(v, nodesVisited);

	if(nodesVisited.size() == vertexSet.size())
		return true;
	else
		return false;
}

template <class T>
vector<T> Graph<T>::dfs() const {
	typename vector<Vertex<T>*>::const_iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::const_iterator ite= vertexSet.end();
	for (; it !=ite; it++)
		(*it)->visited=false;
	vector<T> res;
	it=vertexSet.begin();
	for (; it !=ite; it++)
		if ( (*it)->visited==false )
			dfs(*it,res);
	return res;
}

template <class T>
void Graph<T>::dfs(Vertex<T> *v,vector<T> &res) const {
	v->visited = true;
	res.push_back(v->info);
	typename vector<Edge<T> >::iterator it= (v->adj).begin();
	typename vector<Edge<T> >::iterator ite= (v->adj).end();
	for (; it !=ite; it++)
		if ( it->dest->visited == false ){
			dfs(it->dest, res);
		}
}

template <class T>
vector<set<T> > Graph<T>::getStrongestConnectedComponents(){
	vector<set<T> > ans;
	stack<T> stack;

	typename vector<Vertex<T>*>::const_iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::const_iterator ite= vertexSet.end();
	for (; it !=ite; it++)
		(*it)->visited=false;

	for(it = vertexSet.begin();it != ite;it++){
		if((*it)->visited == false){
			putInStackByPosOrder_SCC((*it), stack);
		}
	}

	Graph<T> gr = getReversedGraph();

	vector<Vertex<T> *> vertexSetGr = gr.getVertexSet();
	for(int i = 0;i < vertexSetGr.size();i++){
		vertexSetGr[i]->visited = false;
	}

	while(stack.empty() == false){
		T v = stack.top();
		stack.pop();

		if(gr.getVertex(v)->visited == false){
			set<T> component;
			gr.printOneComponent_SCC(gr.getVertex(v), component);
			ans.push_back(component);
		}
	}
	return ans;
}

template <class T>
void Graph<T>::printOneComponent_SCC(Vertex<T>* v, set<T>& component){
	v->visited = true;
	component.insert(v->info);

	for(int i = 0;i < v->adj.size();i++){
		if(v->adj[i].dest->visited == false){
			printOneComponent_SCC(v->adj[i].dest, component);
		}
	}
}

template <class T>
Graph<T> Graph<T>::getReversedGraph(){
	Graph<T> gr;

	for(int i = 0;i < vertexSet.size();i++){
		gr.addVertex(vertexSet[i]->info);
	}

	for(int i = 0;i < vertexSet.size();i++){
		for(int j = 0;j < vertexSet[i]->adj.size();j++){
			Vertex<T>* v = gr.getVertex(vertexSet[i]->info);
			Vertex<T>* w = gr.getVertex(vertexSet[i]->adj[j].dest->info);
			w->addEdge(v, vertexSet[i]->adj[j].weight);
		}
	}
	return gr;
}


template <class T>
void Graph<T>::putInStackByPosOrder_SCC(Vertex<T>* v, stack<T>& stack){
	v->visited = true;

	for(int i = 0;i < v->adj.size();i++){
		if(v->adj[i].dest->visited == false){
			putInStackByPosOrder_SCC(v->adj[i].dest, stack);
		}
	}

	stack.push(v->info);
}

template <class T>
vector<T> Graph<T>::getPathSalesmanProblem(T idStart,T idEnd){
	typename vector<Vertex<T>*>::const_iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::const_iterator ite= vertexSet.end();
	for (; it !=ite; it++)
		(*it)->visited=false;

	vector<T> res;
	Vertex<T> *v = getVertex(idStart);
	Vertex<T> *x = getVertex(idEnd);
	salesmanProblemAux(v, x, res, 0);

	return res;
}

template <class T>
void Graph<T>::salesmanProblemAux(Vertex<T> *vertexToProcess,Vertex<T> *endVertex, vector<T> &res, int numberEdgesVisited){
	vertexToProcess->visited = true;
	numberEdgesVisited++;
	res.push_back(vertexToProcess->info);

	Vertex<T> *dest = NULL;
	int weight = INT_INFINITY;
	typename vector<Edge<T> >::iterator it= (vertexToProcess->adj).begin();
	typename vector<Edge<T> >::iterator ite= (vertexToProcess->adj).end();

	for (; it !=ite; it++){
		if((size_t)numberEdgesVisited >= vertexSet.size()-1){
			res.push_back(endVertex->info);
			return;
		}
		else if ( it->dest->visited == false && it->dest != endVertex){
			if(it->weight < weight){
				dest = it->dest;
				weight = it->weight;
			}
		}
	}

	salesmanProblemAux(dest, endVertex, res, numberEdgesVisited);
}

template <class T>
vector<T> Graph<T>::bfs(Vertex<T> *v) const {
	vector<T> res;
	queue<Vertex<T> *> q;
	q.push(v);
	v->visited = true;
	while (!q.empty()) {
		Vertex<T> *v1 = q.front();
		q.pop();
		res.push_back(v1->info);
		typename vector<Edge<T> >::iterator it=v1->adj.begin();
		typename vector<Edge<T> >::iterator ite=v1->adj.end();
		for (; it!=ite; it++) {
			Vertex<T> *d = it->dest;
			if (d->visited==false) {
				d->visited=true;
				q.push(d);
			}
		}
	}
	return res;
}

template <class T>
int Graph<T>::maxNewChildren(Vertex<T> *v, T &inf) const {
	vector<T> res;
	queue<Vertex<T> *> q;
	queue<int> level;
	int maxChildren=0;
	inf =v->info;
	q.push(v);
	level.push(0);
	v->visited = true;
	while (!q.empty()) {
		Vertex<T> *v1 = q.front();
		q.pop();
		res.push_back(v1->info);
		int l=level.front();
		level.pop(); l++;
		int nChildren=0;
		typename vector<Edge<T> >::iterator it=v1->adj.begin();
		typename vector<Edge<T> >::iterator ite=v1->adj.end();
		for (; it!=ite; it++) {
			Vertex<T> *d = it->dest;
			if (d->visited==false) {
				d->visited=true;
				q.push(d);
				level.push(l);
				nChildren++;
			}
		}
		if (nChildren>maxChildren) {
			maxChildren=nChildren;
			inf = v1->info;
		}
	}
	return maxChildren;
}


template <class T>
Vertex<T>* Graph<T>::getVertex(const T &v) const {
	for(unsigned int i = 0; i < vertexSet.size(); i++)
		if (vertexSet[i]->info == v) return vertexSet[i];
	return NULL;
}

template<class T>
void Graph<T>::resetIndegrees() {
	//colocar todos os indegree em 0;
	for(unsigned int i = 0; i < vertexSet.size(); i++) vertexSet[i]->indegree = 0;

	//actualizar os indegree
	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		//percorrer o vector de Edges, e actualizar indegree
		for(unsigned int j = 0; j < vertexSet[i]->adj.size(); j++) {
			vertexSet[i]->adj[j].dest->indegree++;
		}
	}
}


template<class T>
vector<Vertex<T>*> Graph<T>::getSources() const {
	vector< Vertex<T>* > buffer;
	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		if( vertexSet[i]->indegree == 0 ) buffer.push_back( vertexSet[i] );
	}
	return buffer;
}


template <class T>
void Graph<T>::dfsVisit() {
	typename vector<Vertex<T>*>::const_iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::const_iterator ite= vertexSet.end();
	for (; it !=ite; it++)
		(*it)->visited=false;
	it=vertexSet.begin();
	for (; it !=ite; it++)
		if ( (*it)->visited==false )
			dfsVisit(*it);
}

template <class T>
void Graph<T>::dfsVisit(Vertex<T> *v) {
	v->processing = true;
	v->visited = true;
	typename vector<Edge<T> >::iterator it= (v->adj).begin();
	typename vector<Edge<T> >::iterator ite= (v->adj).end();
	for (; it !=ite; it++) {
		if ( it->dest->processing == true) numCycles++;
		if ( it->dest->visited == false ){
			dfsVisit(it->dest);
		}
	}
	v->processing = false;
}

template<class T>
vector<T> Graph<T>::topologicalOrder() {
	//vector com o resultado da ordenacao
	vector<T> res;

	//verificar se � um DAG
	if( getNumCycles() > 0 ) {
		cout << "Ordenacao Impossivel!" << endl;
		return res;
	}

	//garantir que os "indegree" estao inicializados corretamente
	this->resetIndegrees();

	queue<Vertex<T>*> q;

	vector<Vertex<T>*> sources = getSources();
	while( !sources.empty() ) {
		q.push( sources.back() );
		sources.pop_back();
	}

	//processar fontes
	while( !q.empty() ) {
		Vertex<T>* v = q.front();
		q.pop();

		res.push_back(v->info);

		for(unsigned int i = 0; i < v->adj.size(); i++) {
			v->adj[i].dest->indegree--;
			if( v->adj[i].dest->indegree == 0) q.push( v->adj[i].dest );
		}
	}

	//testar se o procedimento foi bem sucedido
	if ( res.size() != vertexSet.size() ) {
		while( !res.empty() ) res.pop_back();
	}

	//garantir que os "indegree" ficam atualizados ao final
	this->resetIndegrees();

	return res;
}



template<class T>
vector<T> Graph<T>::getPath(const T &origin, const T &dest){

	list<T> buffer;
	Vertex<T>* v = getVertex(dest);

	//cout << v->info << " ";
	buffer.push_front(v->info);
	while ( v->path != NULL &&  v->path->info != origin) {
		v = v->path;
		buffer.push_front(v->info);
	}
	if( v->path != NULL )
		buffer.push_front(v->path->info);


	vector<T> res;
	while( !buffer.empty() ) {
		res.push_back( buffer.front() );
		buffer.pop_front();
	}
	return res;
}


template<class T>
void Graph<T>::unweightedShortestPath(const T &s) {

	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		vertexSet[i]->path = NULL;
		vertexSet[i]->dist = INT_INFINITY;
	}

	Vertex<T>* v = getVertex(s);
	v->dist = 0;
	queue< Vertex<T>* > q;
	q.push(v);

	while( !q.empty() ) {
		v = q.front(); q.pop();
		for(unsigned int i = 0; i < v->adj.size(); i++) {
			Vertex<T>* w = v->adj[i].dest;
			if( w->dist == INT_INFINITY ) {
				w->dist = v->dist + 1;
				w->path = v;
				q.push(w);
			}
		}
	}
}

template<class T>
void Graph<T>::bellmanFordShortestPath(const T &s){
	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		vertexSet[i]->path = NULL;
		vertexSet[i]->dist = INT_INFINITY;
	}

	Vertex<T>* v = getVertex(s);
	v->dist = 0;
	queue< Vertex<T>* > q;
	q.push(v);

	int V = vertexSet.size();
	while(--V){
		while( !q.empty() ) {
			v = q.front(); q.pop();
			for(unsigned int i = 0; i < v->adj.size(); i++) {
				Vertex<T>* w = v->adj[i].dest;
				if( v->dist + v->adj[i].weight < w->dist ) {
					w->dist = v->dist + v->adj[i].weight;
					w->path = v;
					q.push(w);
				}
			}
		}
	}
}

template<class T>
void Graph<T>::dijkstraShortestPath(const T &s){
	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		vertexSet[i]->path = NULL;
		vertexSet[i]->dist = INT_INFINITY;
		vertexSet[i]->visited = false;
		vertexSet[i]->processing = false;
	}

	Vertex<T>* v = getVertex(s);
	v->dist = 0;
	v->visited = true;

	vector<Vertex<T> * > pq;
	make_heap(pq.begin(),pq.end(),vertex_greater_than<T>());
	pq.push_back(v);push_heap(pq.begin(), pq.end());

	while(pq.size() > 0){
		v = pq.front();
		pop_heap(pq.begin(), pq.end());pq.pop_back();

		for(unsigned int i = 0; i < v->adj.size(); i++) {
			Vertex<T>* x = v->adj[i].dest;
			int newDist = v->dist + v->adj[i].weight;

			if(  newDist < x->dist && newDist > 0) {
				x->dist = v->dist + v->adj[i].weight;
				x->path = v;
				if(x->visited == false){
					x->visited = true;
					pq.push_back(x);push_heap(pq.begin(), pq.end());
				}
				else
					sort_heap(pq.begin(), pq.end());
			}
		}
	}
}

template<class T>
int Graph<T>::edgeCost(int i, int j){
	if(i == j)
		return 0;
	Vertex<T>* s = vertexSet[i];
	Vertex<T>* d = vertexSet[j];
	for(size_t i = 0;i < s->adj.size();i++)
		if(s->adj[i].dest == d)
			return s->adj[i].weight;
	return INT_INFINITY;
}

template<class T>
void Graph<T>::floydWarshallShortestPath(){
	size_t V = vertexSet.size();
	W = vector<vector<int> > (V, vector<int> (V));
	P = vector<vector<int> > (V, vector<int> (V));
	for(size_t i = 0;i < V;i++)
		for(size_t j = 0;j < V;j++){
			W[i][j] = edgeCost(i,j);
			P[i][j] = j;
		}
	for(size_t i = 0;i < V;i++)
		for(size_t j = 0;j < V;j++)
			for(size_t k = 0;k < V;k++){
				if(W[j][i] == INT_INFINITY || W[i][k] == INT_INFINITY)
					continue;
				int newDist = W[j][i]+W[i][k];
				if(newDist > 0)
					if(newDist < W[j][k]){
						W[j][k] = newDist;
						P[j][k] = P[j][i];
					}
			}
}

template<class T>
vector<vector<T> > Graph<T>::getWeightBetweenAllVertexs(){
	return W;
}

template<class T>
vector<T> Graph<T>::getfloydWarshallPath(const T &origin, const T &dest){
	vector<T> ans;
	Vertex<T>* s = getVertex(origin);
	Vertex<T>* d = getVertex(dest);
	int si, di;
	for(size_t i = 0;i < vertexSet.size();i++){
		if(vertexSet[i] == s){
			si = i;
		}
		if(vertexSet[i] == d){
			di = i;
		}
	}
	ans.push_back(vertexSet[si]->info);
	int ind = si;
	while(ind != di){
		ind = P[ind][di];
		ans.push_back(vertexSet[ind]->info);
	}
	return ans;
}



#endif /* GRAPH_H_ */
