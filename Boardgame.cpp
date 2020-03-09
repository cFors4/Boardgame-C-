#include <iostream>
#include <string>
#include <list>
#include <sstream>
#include <fstream>
#include<bits/stdc++.h>


using namespace std;

void print(vector<int> const &input)
{
  for (int i = 0; i < input.size(); i++) {
    cout << input.at(i) << ' ';
  }
}

class Graph
{
  int V;    // No. of vertices
  vector<int> *adj;
  int traverseColors(int v, bool visited[],vector<int> colors);

public:
  Graph(int V);   // Constructor
  void addEdge(int v, int w);
  vector<int> findSame(vector<int> colors);
  vector<int> adjacency(int v);
  void makeEdges(int N);
};

vector<int> Graph::findSame(vector<int> colors)
{
  // Mark all the vertices as not visited
  bool *visited = new bool[V];
  vector<int> change;
  for(int v = 0; v < V; v++)
    visited[v] = false;

  for (int v=0; v<V; v++)
  {
    if (visited[v] == false)
    {
      // print all reachable vertices
      // from v
      int x = traverseColors(v, visited,colors);
      change.push_back(x);
    }
  }
  return change;
}

int Graph::traverseColors(int v, bool visited[],vector<int> colors)
{
  // Mark the current node as visited
  visited[v] = true;

  vector<int>::iterator i;
  for(i = adj[v].begin(); i != adj[v].end(); ++i){
    if(!visited[*i] && colors[*i]!=colors[0]){
      traverseColors(*i, visited,colors);
    }
  }return v;
}

Graph::Graph(int V)
{
  this->V = V;
  adj = new vector<int>[V];
}

// method to add an undirected edge
void Graph::addEdge(int v, int w)
{
  adj[v].push_back(w);
  adj[w].push_back(v);
}


vector<int> Graph::adjacency(int v)
{
  return(adj[v]);
}

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



void openFile(string fileName, int elements[]){

  ifstream ufile(fileName);
  int data;
  // gets first element of list
  ufile >> data;
  int N = data;
  for(int i = 0; i < N*N;i++){
    ufile >> data;
    elements[i] = data;
  }
}

void outElements(vector<int> colors,int N){
  int len = N*N;
  for(int i = 0;i <len;i++){
    cout << colors[i];
    if(i % N-(N-1) == 0){
      cout << "\n";
    }
  }
}

vector<int> changeSame(int color,vector<int> colors,vector<int> same){
  for (int i = 0; i < same.size(); i++) {
    colors.at(same[i]) =color;
  }
  return colors;
}

vector<int> turn(int color,vector<int> colors,Graph g,int numVertices){
  //take a turn
  vector<int> same = g.findSame(colors);
  vector<int> colored = changeSame(color,colors,same);
  return colored;
}

bool check(vector<int> colors){
  if ( equal(colors.begin() + 1, colors.end(), colors.begin()) )
  {
    cout << "All elements are equal each other" << endl;
    return true;
  }
  else{
    return false;
  }
}


// Drive program to test above
int main()
{
  //input framework
  //reading from a file
  string fileNam = "example.txt";
  //opens file
  ifstream ufile(fileNam);
  int data;
  // gets first element of list
  ufile >> data;
  int N = data;
  ufile.close();

  int numVertices = N*N;
  int elements[numVertices];
  // returns list of elements
  openFile(fileNam,elements);
  //convert int array to vector
  vector<int> colors;
  for (int i: elements) {
    colors.push_back(i);
  }
  //Create graph
  Graph g(N*N);
  //make relationships between positions on matrix
  g.makeEdges(N);

  int steps=0;
  bool finished = false;
  string Q= "";
  vector<int> gen;
  while (true){
    //print out from a list of colors
    steps+=1;
    cout<<"STEP: "<<steps;
    cout << "\n";
    outElements(colors,N);
    cout << "\n";

    int color;
    cout << "change pivot to a color represented from 1-6: ";
    //TODO add check for 1-6
    cin >> color;
    colors = turn(color,colors,g,numVertices);
    //check if done
    finished = check(colors);



    if (finished){
      cout << "\n##########CONGRAT###########\n ";
      cout << "Q to quit: ";
      //TODO add check for 1-6
      cin >> Q;
      if(Q=="Q"||Q=="q"){
        exit(1);
      }else{
        cout << "Pick N for an N*N graph from 3 to 9 and randomly generate: ";
        // TODO: check N 3 to 9
        cin >> N;

        numVertices = N*N;
        for (int i =0; i < numVertices; i++){
          int ran = rand() % 6 + 1;
          gen.push_back(ran);
          //cout << gen[i] << endl;
          }
        colors = gen;
        steps=0;
        finished=false;
        times+=1;
        }
      }
    }
    return 0;
  }
