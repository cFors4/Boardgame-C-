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

bool contains(vector<int> const &input,int key){
  if (count(input.begin(), input.end(), key))
  return true;
  else
  return false;
}

class Graph
{
  int V;    // No. of vertices
  vector<int> *adj;
  vector<int> sameCol(int v, bool visited[],vector<int> colors,vector<int> change);

public:
  Graph(int V);   // Constructor
  void addEdge(int v, int w);
  vector<int> findSame(vector<int> colors);
  vector<int> adjacency(int v);
  void makeEdges(int N);
  void change(int V);

  int G(vector<int> colors,int current,int adj);
  int H(vector<int> colors,int adj);
};

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

vector<int> Graph:: sameCol(int v, bool visited[],vector<int> colors,vector<int> change)
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

void Graph::change(int V){
  this->V = V;
  adj = new vector<int>[V];
}


int Graph::G(vector<int> colors,int current,int next){
  //impact to traverse // make turn
  if (colors[current] == colors[next]){
    return 0;
  }
  return 1;
}

int Graph::H(vector<int> colors,int next){
  //surrounding are same or not //impact
  int counter = 0;
  for(auto x:adj[next]){
    if(colors[x] == colors[next]){
      counter -=0.5;
    } else if(colors[x]!= colors[next]){
      counter +=0.5;
    }
  }
  return counter;
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
    return true;
  }
  else{
    return false;
  }
}


int all(vector<int> colors,int col){
  int count = 0;
  for(int x=0;x<colors.size();x++){
    if(colors[x]==col)
      count+=1;
  }
  return count;
}


vector<int> optimalSequence(vector<int> colors,Graph g){
  vector<int> closed;
  vector<int> moves;
  vector<int> impact(7,0);
  bool finished;
  while (!finished) {

    vector<int> same;
    finished = check(colors);

    // Try all colors but the last one used.
    fill(impact.begin(), impact.end(), 0);
    int numColors = 7;
    for (int next = 1; next < numColors; ++next) {
      //if color same as last move
      if(moves.size()>0)
      if (next == moves[moves.size()])
      continue;

      //if unchanged
      vector<int> newColors = turn(next,colors,g,colors.size());
      same = g.findSame(colors);
      vector<int> newSame = g.findSame(newColors);
      if (same == newSame){
        continue;
      }
      //check impact of change
      fill(closed.begin(), closed.end(), 0);
      for (int filled=0;filled<same.size();filled++){
        for (auto adjacent:g.adjacency(same[filled])){
          if(colors[adjacent]==next && !contains(closed,adjacent)){
            impact[next]+=1;
            closed.push_back(adjacent);
          }
        }
      }
    }
    //pick biggest impact
    int max = *max_element(impact.begin(), impact.end());
    bool move = false;
    if(!move){
    for(int i=1;i < impact.size();i++){

      if(impact[i]==all(colors,i) && impact[i]!=0){
        move = true;
        cout<<"all "<<i<<" quantitiy "<<impact[i]<<endl;
        vector<int> same = g.findSame(colors);
        colors = changeSame(i,colors,same);
        finished = check(colors);
        moves.push_back(i);
        break;

      }
    }
    if(!move){
    for(int i=1;i < impact.size();i++){
      if(impact[i]==max){
        cout<<"max"<<endl;
        vector<int> same = g.findSame(colors);
        colors = changeSame(i,colors,same);
        finished = check(colors);
        moves.push_back(i);
        break;
      }
    }
  }

    }
    cout<<endl;
    print(moves);
    cout<<endl;

  }
  return moves;
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
  //A*
  vector<int> optimalPath = optimalSequence(colors,g);
  int optimalSteps = optimalPath.size();
  cout<<"optimal steps are "<<optimalSteps<<endl;


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
      cout << "\n##########CONGRATS###########\n ";
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

        g.change(numVertices);
        g.makeEdges(N);
        colors = gen;
        steps=0;
        finished=false;

      }
    }
  }
  return 0;
}
