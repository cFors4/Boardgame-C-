#include "head.h"

using namespace std;

//find all same in the colors vector and return all the same that can be changed
//used for changing colors of the graph each turn.
vector<int> Graph::findSame(vector<int> colors)
{
  // Mark all the vertices as not visited
  bool *visited = new bool[V];
  vector<int> change;
  for(int v = 0; v < V; v++)
  visited[v] = false;

  change =  sameCol(0, visited,colors,change);
  change.push_back(0);

  return change;
}

//takes in coloros and traveres all nodes that can be travelled to (same color)
vector<int> Graph::sameCol(int v, bool visited[],vector<int> colors,vector<int> change)
{
  // Mark the current node as visited
  visited[v] = true;

  //go through adjacent
  vector<int>::iterator i;
  for(auto i:adj[v]){
    if(colors[i] != colors[v] && !contains(change,v)){
      change.push_back(v);
    }
    else if(colors[i] == colors[v] && !visited[i]){
      change.push_back(i);
      change = sameCol(i,visited,colors,change);
    }
  }
  return change;
}

//constructor
Graph::Graph(int V)
{
  this->V = V;
  adj = new vector<int>[V];
}

// method to add an undirected edge between nodes
void Graph::addEdge(int v, int w)
{
  adj[v].push_back(w);
  adj[w].push_back(v);
}

//return adjacent relationships of particular node
vector<int> Graph::adjacency(int v)
{
  return(adj[v]);
}

//make edges using addEdge to be stored in adj based on size of grid N
void Graph::makeEdges(int N){
  int listLength = N*N;
  for(int i = 0; i < listLength ;i++){
    if(i+N <listLength){
      addEdge(i,i+N);
    }
    if((i+1) % N == 0) continue;
    if(i+1 < listLength){
      addEdge(i,i+1);
    }
  }
}

//change constructor values and create new adj
void Graph::change(int V){
  this->V = V;
  adj = new vector<int>[V];
}
