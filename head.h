#ifndef HEAD_H
#define HEAD_H

#include <iostream>
#include <string>
#include <list>
#include <sstream>
#include <fstream>
#include<bits/stdc++.h>
#include <tuple>
using namespace std;

class Graph
{
  int V;    // No. of vertices
  vector<int> *adj; //adjacent vector determining structure of graph and positions
  vector<int> sameCol(int v, bool visited[],vector<int> colors,vector<int> change); //auxillary function used in findSame

public:
  Graph(int V);   // Constructor
  void addEdge(int v, int w); //add edge connection between nodes
  vector<int> findSame(vector<int> colors); //find same colors in the colors vector and return position of the same
  vector<int> adjacency(int v); //storing adjacent node relationships as vector<int>
  void makeEdges(int N); //
  void change(int V);
};

/**
* search vector of integers for a particular int
*
* auxillary function used for testing if a vector contains
*
* @param any vector of integers to be searched
* @param integer to be looked for in the vector<int>
* @return true or false if an integer is contained in the vector
*/
bool contains(vector<int> const &input,int key);

#endif
