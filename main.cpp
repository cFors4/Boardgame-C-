#include "head.h"
#include <iostream>
#include <string>

/**
* search vector of integers for a particular int
*
* auxillary function used for testing if a vector contains
*
* @param any vector of integers to be searched
* @param integer to be looked for in the vector<int>
* @return true or false if an integer is contained in the vector
*/
bool contains(vector<int> const &input,int key){
  if (count(input.begin(), input.end(), key))
  return true;
  else
  return false;
}

//open file
vector<int> openFile(string fileName, vector<int> elements){

  ifstream ufile(fileName);
  int data;
  // gets first element of list
  ufile >> data;
  int N = data;
  for(int i = 0; i < N*N;i++){
    ufile >> data;
    elements.push_back(data);
  }
  return elements;
}

//output Elements of grid in the grid format
void outElements(vector<int> colors,int N){
  int len = N*N;
  for(int i = 0;i <len;i++){
    cout << " "<<colors[i];
    if(i % N-(N-1) == 0){
      cout << "\n";
    }
  }
}

//changeSame colors based on same vector to the int color
vector<int> changeSame(int color,vector<int> colors,vector<int> same){
  for (int i = 0; i < same.size(); i++) {
    colors.at(same[i]) =color;
  }
  return colors;
}

//take a turn
vector<int> turn(int color,vector<int> colors,Graph g,int numVertices){
  //find same colors to be changed
  vector<int> same = g.findSame(colors);
  //change same colors based on same and given color input
  vector<int> colored = changeSame(color,colors,same);
  //return changed colors after turn taken
  return colored;
}
//check if the game is finished
bool check(vector<int> colors){
  //return true if all colors in colors are the same
  if ( equal(colors.begin() + 1, colors.end(), colors.begin()) )
  {
    return true;
  }
  else{
    return false;
  }
}

//used for memory intensive search N>8 as is time optimal
vector<int> greedySequence(vector<int> colors,Graph g){
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
      vector<int> closed;
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
    for(int i=0;i < impact.size();i++){
      if(impact[i]==max){
        vector<int> same = g.findSame(colors);
        colors = changeSame(i,colors,same);
        finished = check(colors);
        moves.push_back(i);
      }
    }
  }
  return moves;
}

int h(vector<int> colors){
  int count = 0;
  for (int i=1;i<7;i++){
    if(contains(colors,i))
    count += 1;
  }
  return count;
}

tuple<int, vector<vector<int>>> search(vector<vector<int>> path,int g, int bound, int previous,Graph graph){
  vector<int> node = path.back();
  //estimated cost of the cheapest path (root..node..goal)
  int f = g + h(node);
  if (f>bound){
    return make_tuple(f,path);
  }
  //check if done so dont ned to explore any further
  if (check(node)){
    return make_tuple(g,path);
  }
  //min e.g. infinity
  int min = 100;
  int numColors = 7;

  for (int next = 1; next < numColors; next++) {
    vector<int> newColors = turn(next,node,graph,node.size());
    bool breaks = false;
    for(int i=0;i<path.size();i++)
    if(newColors==path[i])
    breaks = true;
    //if color move already done
    if (breaks==true)
    continue;
    //if color same as last move
    if(next == previous)
    continue;
    //if color makes no difference
    vector<int> same = graph.findSame(node);
    vector<int> newSame = graph.findSame(newColors);
    if (same == newSame)
    continue;

    //push move to path
    path.push_back(newColors);
    int t;
    tie(t, path) = search(path,g+1,bound,next,graph);
    //if path at finished state return
    if (check(path.back()))
    return make_tuple(t,path);
    //min=current shortest
    if(t<min)
      min=t;
    //get rid of evaluated state
    path.pop_back();
  }
  //return current min
  return make_tuple(min,path);
}


int ida_star(vector<int> colors,Graph graph){
  //estimated cost of the cheapest path (node..goal)
  int bound=h(colors);
  vector<vector<int>> path;
  path.push_back(colors);

  while(true){
    int t;
    //search possible unexplored
    tie(t,path) = search(path,0,bound,0,graph);
    // when last path state is finished
    if (check(path.back())){
      return t;
    }
    //asign t to bound as new maximum
    bound = t;
  }
}
using namespace std;
int main()
{
  
  //initialisation for graph and game state
  Graph g(25);
  int steps=0;
  bool finished = false;
  string Q;
  

  //initialisation for shape and contents of vector for the game grid
  int N;
  int numVertices;
  vector<int> colors;
  vector<int> colorsFile;
  

  //initialisation of int variables
  int optimal;
  int ran;
  

  //reading from a file
  int data;
  string fileNam = "example.txt";
  
  //opens file
  ifstream ufile(fileNam);
  //gets first element of list
  ufile >> data;
  N = data;
  ufile.close();
  numVertices = N*N;
  vector<int> elements;
  
  //returns list of elements
  elements = openFile(fileNam,elements);
  //convert int array to vector
  
  while (true){
    
    string RorF;
    //check input is r or f
    while (cout << "Randomly generated or read from a file: [R/F] " && !(cin >> RorF)  || !(RorF == "R" || RorF =="r" || RorF == "F" || RorF =="f")) {
      cin.clear(); //clear bad input flag
      cin.ignore(numeric_limits<streamsize>::max(), '\n'); //discard input
      cout << "Invalid input; please re-enter.\n";
    }


    if(RorF == "R" || RorF =="r"){

      //check N is 3 to 9
      while (cout << "Pick N for an N*N graph from 3 to 9 and randomly generate: " && !(cin >> N)  || N < 3 || N > 9) {
        cin.clear(); //clear bad input flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //discard input
        cout << "Invalid input; please re-enter.\n";
      }
      //generate random new graph based on size N given in input
      vector<int> gen;
      numVertices = N*N;
      for (int i =0; i < numVertices; i++){
        ran = rand() % 6 + 1;
        gen.push_back(ran);
      }
      //make newly generated colors new colors
      colors = gen;
    }
    if(RorF == "F" || RorF =="f"){
      //set to details read in from file
      N = data;
      numVertices = N*N;
      colors = elements;
    }

    //change graph
    g.change(numVertices);
    g.makeEdges(N);
    //if N<8 use iterative deepening A* algorithm to find optimal
    if (N<8){
      optimal = ida_star(colors,g);
    }
    //if greater than depth 8 use greedy path finder as is optimal in time and runs in an adequate time
    else{
      vector<int> greedyPath = greedySequence(colors,g);
      optimal = greedyPath.size();
    }

    //play game until finished
    while(!finished){
      steps+=1;
      cout<<"STEP: "<<steps;
      cout << "\n";
      outElements(colors,N);
      cout << "\n";

      //check color 1 to 6
      int color;
      while (cout << "change pivot to a color represented from 1-6: " && !(cin >> color)  || color < 1 || color > 6) {
        cin.clear(); //clear bad input flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //discard input
        cout << "Invalid input; please re-enter.\n";
      }
      //take turn
      colors = turn(color,colors,g,numVertices);
      //check if done
      finished = check(colors);
    }

    //when done details and option to stop
    if (finished){
      cout << "\n##########CONGRATS###########\n ";
      cout<<"Optimal steps are "<<optimal<<"."<<endl;
      cout<<"You did it in "<<steps<<" steps."<<endl;

      //Q to Quit
      cout << "Q to quit: ";
      cin >> Q;
      if(Q=="Q"||Q=="q"){
        exit(1);
      }
      //reset game state to be played again
      steps=0;
      finished=false;
    }
  }

  return 0;
}
