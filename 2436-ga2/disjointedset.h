#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

struct Vertex { //or cell 
  int x;
  int y;
  int name; //setCount number
  bool visited = false;
  bool rightEdge = false; 
  bool downEdge = false;
  bool frontier = false;
  bool visitedSol = false;
  bool path = false;
  bool operator==(const Vertex& rhs) const {
    Vertex a = rhs;
    return (this->x == a.x && this->y == a.y);
  }
};

struct Edge {
  Vertex a;
  Vertex b;
  char direction; // up-down or side-side
  bool visited = false;
  
};

class DisjointSet {
public:
  DisjointSet(int numVertex, int);
  void MakeSet();
  vector<Edge> ListOfEdges();
  int Find(Vertex s);
  bool Union(Edge e);

private:
  int count = 4;
  int num;
  Vertex *parent;
  int *rank;
  vector<Edge> edges;
};

DisjointSet::DisjointSet(int numVertex, int c) {
  count = c;
  num = numVertex;
  parent = new Vertex[num];
  rank = new int[num];
  MakeSet();
}

void DisjointSet::MakeSet() {
  int k = 0;
  for (int i = 0; i < count; ++i) {
    for (int j = 0; j < count; ++j){
      Vertex a = Vertex();
      a.x = i;
      a.y = j;
      a.name = k;
      parent[k] = a;
      k++;
      //cout << "Vertex: " << a.name << " " << a.x << a.y << endl;
    }
  }
  //cout << "K: " << k << endl;
}

vector<Edge> DisjointSet::ListOfEdges() {
  int edgeCount = 2*count*count - count - count;
  int vertexCount = count*count;

  //cout << "Vertex count: " << vertexCount << endl;
  Edge e = Edge();
  int index = 0;
  
  for(int i = 0; i < vertexCount; ++i){
    Vertex a = parent[i];
    Vertex b;
    //cout << "A: "<< i << " " << a.x << a.y << endl;
    
    for(int j=0; j < vertexCount; ++j) {
      
      b = parent[j];
      //cout << "B: " << j << " " << b.x << b.y << endl;
      if(a.x == b.x && b.y == a.y+1) {
        e.a = a;
        e.b = b;
        e.direction = 'R';
        edges.push_back(e);
        //cout << "Vertices: " << a.x << a.y << " " <<  b.x << b.y << endl;
      }
      else if(a.x + 1 == b.x && a.y == b.y) {
        e.a = a;
        e.b = b;
        e.direction = 'D';
        edges.push_back(e);
        //cout << "Other Vertices: " << a.x << a.y << " " <<  b.x << b.y << endl;
      }
    }
  }
/*
  for(int i = 0; i < edges.size(); ++i){
    cout << "Edge " << i << ": " << edges[i].a.x << edges[i].a.y << " " << edges[i].b.x << edges[i].b.y << endl;
  }*/

  return edges;
  
}

int DisjointSet::Find(Vertex s) { // find the leader of the set
  if (parent[s.name].name != s.name)
    parent[s.name].name = Find(parent[s.name]);

  return parent[s.name].name;
}

bool DisjointSet::Union(Edge e) {
  int setX = Find(e.a);
  int setY = Find(e.b);

  if(setX == setY){
    return false;
  }

  if(rank[setX] > rank[setY]){
    //cout << "X is greater" << endl;
    parent[setY].name = setX;
  }
  else if(rank[setY] > rank[setX]){
    //cout << "Y is greater" << endl;
    parent[setX].name = setY;
  } else {
    //cout << "Increasing Y by 1" << endl;
    parent[setX].name = setY;
    rank[setY] = rank[setY] + 1;
  }
  return true;
}

void PrintVertex(Vertex a){
  cout << a.x << ", " << a.y;
}

void PrintEdge(Edge a){
  PrintVertex(a.a);
  cout << " -> ";
  PrintVertex(a.b);
}

void PrintVertex(ostream& fout, Vertex a){
  fout << a.x << ", " << a.y;
}

void PrintEdge(ostream& fout, Edge a){
  PrintVertex(fout, a.a);
  fout << " -> ";
  PrintVertex(fout, a.b);
}

int findIndex(vector<Vertex> v, Vertex a) {
  for(int i = 0; i < v.size(); ++i){
    if(v[i] == a){
      return i;
    }
  }
  return -1;
}
int findIndex(vector<Edge> e, Edge a){
  for(int i = 0; i < e.size(); ++i){
    if(e[i].a == a.a && e[i].b == a.b){
      return i;
    }
  }
  return -1;
}

int findIndex(vector<Edge> e, Vertex a){
  for(int i = 0; i < e.size(); ++i){
    if(e[i].a == a){
      return i;
    }
  }
  return -1;
}

int findUnvisitIndexA(vector<Edge> e, Vertex a){
  for(int i = 0; i < e.size(); ++i){
    if(e[i].a == a && !e[i].visited){
      return i;
    }
  }
  return -1;
}
int findUnvisitIndexB(vector<Edge> e, Vertex a){
  for(int i = 0; i < e.size(); ++i){
    if(e[i].b == a && !e[i].visited){
      return i;
    }
  }
  return -1;
}

queue<Vertex> findAllEdges(vector<Edge> e, Vertex a){
  queue<Vertex> v;
  for(int i = 0; i < e.size(); ++i){
    if(e[i].a == a) {
      v.push(e[i].a);
    }
    if (e[i].b == a) {
        v.push(e[i].b);
    }
  }
  return v;
}
