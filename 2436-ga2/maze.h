#include <fstream>
#include <climits>
#include <cstdlib>
#include <iomanip>
#include <stack>
#include "disjointedset.h"
using namespace std;

class Maze {
  public:
    Maze();
    Maze(int);
    void GenerateMaze();
    
    void Kruskal();
    void PrintKruskal();
    void SortKruskalVertices();
    void SolveKruskal();
    bool SolveKruskal(Vertex current, Vertex end, stack<Vertex> solution, queue<Vertex> neighbors);

    void Prim(Vertex **maze, int xValue, int yValue);
    bool IsInMaze(vector<Vertex> verticesInMaze, Vertex vert);
    void PrintPrim(Vertex **maze);
    void DFS(Vertex **maze, Vertex current);
    bool unvisitedNeighbors(Vertex **maze, Vertex &current, stack<Vertex> &unvisitedVertices);
    void Path(Vertex **maze, stack<Vertex> &unvisitedVertices);

  private:
    int count = 4;
    int setCount = 0;
    int seedNum = 10; //change after
    int pathCount = 0;
    int vertexCount = count*count;
    vector<Vertex> tree; //saves vertices in order from kruskal
    vector<Edge> edgeTree;
    vector<Vertex> vertices; //random order vertices

    //Vertex **maze;
    vector<Vertex> frontierVertices;
    stack<Vertex> unvisitedVertices;
    vector<Vertex> visitedVertices;

};

Maze::Maze() {
  int count = 30; 
  int setCount = 0; 
  srand(seedNum); 
  
}

Maze::Maze(int c) {
  int count = c;
  int setCount = 0;
  srand(seedNum);
  
}

void Maze::GenerateMaze(){
  int **maze = new int *[count];
  for (int i = 0; i < count; ++i) {
    maze[i] = new int[count];
    for(int j = 0; j < count; ++j) {
      maze[i][j] = setCount;
      setCount++; 
    }
  }
  
}

void Maze::Kruskal() {

  queue<Edge> q;
  vector<int> edgeIndex;
 
  DisjointSet maze(vertexCount, count); //cells
  vector<Edge> edges = maze.ListOfEdges(); //list of all edges
  
  while(edgeIndex.size() != edges.size()) {
    int index = rand()%(edges.size());
    if(find(edgeIndex.begin(), edgeIndex.end(), index) == edgeIndex.end()) {//not present in the list
      q.push(edges[index]);
      edgeIndex.push_back(index);
    }
  }
  
  while(!q.empty()) {
    
    Edge e = q.front();
    Vertex x = e.a;
    Vertex y = e.b;
    char direction = e.direction;
  
    if(maze.Union(e)) {
      //cout << "merged: " << x.name << " " << y.name << endl;
      if(e.direction == 'R') {
        //cout << "R" << endl;
        x.rightEdge = true;
        
        int index = findIndex(vertices, x); //if not -1
        if(index == -1) { //not in vector
          vertices.push_back(x);
        } else {
          vertices[index].rightEdge = true;
        } 
        index = findIndex(vertices, y); //if not -1
        if(index == -1) {
          vertices.push_back(y);
        }
      }
        
      if(e.direction == 'D') {
        //cout << "D" << endl;
        x.downEdge = true;
        
        int index = findIndex(vertices, x); //if not -1
        if(index == -1) {
          vertices.push_back(x);
        } else {
          vertices[index].downEdge = true;
        } 
        index = findIndex(vertices, y); //if not -1
        if(index == -1) {
          vertices.push_back(y);
        }
      }

      int index = findIndex(edgeTree, e);
      if(index == -1) {
          edgeTree.push_back(e);
      }
      
      //cout << edgeCount << " ";
    }
    q.pop();
  }

  SortKruskalVertices();

  PrintKruskal();
  
  SolveKruskal();

  PrintKruskal();
  
}

void Maze::SortKruskalVertices() {

  for(int i = 0; i < count; ++i) {
    for(int j = 0; j < count; ++j) {
      Vertex a = Vertex();
      a.x = i;
      a.y = j;
      int index = findIndex(vertices, a); //if not -1
      tree.push_back(vertices[index]);
    }
  }
}

bool Maze::SolveKruskal(Vertex current, Vertex end, stack<Vertex> solution, queue<Vertex> neighbors) {
    if (current == end) {
        return true;
    }
    bool B = false;
    neighbors = findAllEdges(edgeTree, current);

    int index = findUnvisitIndexA(edgeTree, current);


    while (!neighbors.empty()) {


        cout << "Current Vertex is now ";
        PrintVertex(neighbors.front());
        cout << endl;

        current = neighbors.front();
        index = findUnvisitIndexA(edgeTree, current);
        cout << "Index: " << index << endl;


        if (index == -1) { //does not exist OR all have been visited so go to the b half
            index = findUnvisitIndexB(edgeTree, current);
            cout << "Index: " << index << endl;

            if (index == -1) { //b half all visited so ?
                solution.pop();
                current = solution.top();
                return SolveKruskal(current, end, solution, neighbors);
            }

        }
        else {
            B = false;
        }


        if (current == edgeTree[index].a && !edgeTree[index].visited) {
            solution.push(current); //push 0,0
            edgeTree[index].visited = true;
            current = edgeTree[index].b; //current is now 0,1
            PrintVertex(current);
            cout << endl;
            return SolveKruskal(current, end, solution, neighbors);
        }
        else {
            solution.pop();
            current = solution.top();
        }
        cout << "IF statement failed" << endl;
        neighbors.pop();
    }

    return false;
}

void Maze::SolveKruskal() {
 
  ofstream fout("output.txt");
  bool B = false;
  int ErrorCount = 0;
  int index = 0;

  stack<Vertex> solution;
  queue<Edge> possiblePaths;
  queue<Vertex> neighbors;
  Vertex current = tree[0];
  Vertex end = tree[vertexCount-1];

  for(int i = 0; i < edgeTree.size(); ++i){
    PrintEdge(fout, edgeTree[i]);
    fout << endl;
  }
  //cout << "finished printing all combos" << endl;

  //vertices[0].path = true;

  if (current == end) {
      return;
  }

  neighbors = findAllEdges(edgeTree, current);

  //int index = findUnvisitIndexA(edgeTree, current); // 0,0

  //add all possible edge paths into queue 

  if (SolveKruskal(current, end, solution, neighbors)) {
      cout << "Path found" << endl;
  }
  else {
      cout << "Got a false" << endl;
  }



  /*
  while(!(current == end) || ErrorCount < 10) {
    cout << "1st Passed " << index << endl;

    
    index = findUnvisitIndexA(edgeTree, current); //finds 0,0 edge
    B = true;

    if(index == -1) { //does not exist OR all have been visited so go to the b half
      index = findUnvisitIndexB(edgeTree, current);

      if (index == -1) { //b half all visited so ?
         solution.pop();
         current = solution.top();
         index = findUnvisitIndexA(edgeTree, current);
      }
      
    } else {
      B = false;
    }
    cout << "2nd Passed : " << index << endl;
    
    if(current == edgeTree[index].a && !B){
      solution.push(current); //push 0,0
      edgeTree[index].visited = true;
      current = edgeTree[index].b; //current is now 0,1
      PrintVertex(current);
      cout << endl;
    } 
    
    else if (current == edgeTree[index].a && B) {
      solution.push(current); //push 0,0
      edgeTree[index].visited = true;
      current = edgeTree[findUnvisitIndexB(edgeTree, current)].a; //current is now 0,1
      PrintVertex(current);
      cout << endl;
    } 
      
    else if (current == edgeTree[index].b) {
      solution.push(current); //push 0,0
      edgeTree[index].visited = true;
      current = edgeTree[index].a; //current is now 0,1
      PrintVertex(current);
      cout << endl;
      
    } 
    
    else if(B && !edgeTree[index].visited) { //it found a B but it didnt match current
      cout << "???" << endl;
      
      //solution.pop();
      edgeTree[index].visited = true;
      current = solution.top();
      cout << "New current ";
      PrintVertex(current);
      cout << endl;
      
    }
    
    else {

      cout << "Error" << endl;
      ErrorCount++;
      solution.pop();
      edgeTree[findIndex(edgeTree, current)].visited = false;
      current = solution.top();
      
      cout << "New current ";
      PrintVertex(current);
      cout << endl;
      B = false;
      
      //break;
      
    }
    cout << "Current solution is : ";
    PrintVertex(solution.top());
    cout << endl;
    cout << "3rd Passed : " << index << endl;
  }*/
  
  //solution complete
  solution.push(current);
  cout << "Done" << endl;
  
  while(!solution.empty()) {
    current = solution.top();
    //PrintVertex(current);
    //cout << endl;
    int index = findIndex(vertices, current);
    vertices[index].path = true;
    pathCount++;
    solution.pop();
    
  }
  
}

void Maze::PrintKruskal() {
  //cout << count << endl;
  cout << "Vertex Count " << vertexCount << endl;
  int score = pathCount%vertexCount;
  
  cout << "+";
  for (int i = 0; i < count; i++) {
    cout << "--+";
  }
  cout << endl;
  
  for(int i = 0; i < count; ++i) {
    cout << "|";
    for(int j = 0; j < count; ++j) {
      Vertex a = Vertex();
      a.x = i;
      a.y = j;
      int index = findIndex(vertices, a); //if not -1
      if(vertices[index].path && vertices[index].rightEdge) {
        cout << setw(3) << "o ";
      }
       else if(vertices[index].path && !vertices[index].rightEdge) {
        //PrintVertex(a.x, a.y);
        cout << setw(3) << "o|";
      } 
       else if (!vertices[index].path && vertices[index].rightEdge) {
        //PrintVertex(a.x, a.y);
        cout << setw(3) << "";
      } 
       else {
        //PrintVertex(a.x, a.y);
        cout << setw(3) << "|";
      }
    }
    
    cout << endl;
    cout << "+";
    for (int j = 0; j < count; j++) {
        Vertex a = Vertex();
        a.x = i;
        a.y = j;
        int index = findIndex(vertices, a);
        //PrintVertex(vertices[index].x, vertices[index].y);
        //cout << endl;
        if(vertices[index].downEdge){
          //cout << solution[index].distance
          cout << setw(3) << "+";
        } else {
            cout << "--+";
        }
      }
    cout << endl;
  }
  cout << "Score: " << pathCount << " (" << setprecision(2) << score << "%)" << endl;
}

void Maze::Prim(Vertex **maze, int xValue, int yValue) { 
  vector<Vertex> verticesInMaze;  
  if(!maze[0][0].visited){
    verticesInMaze.push_back(maze[0][0]);
    maze[0][0].visited = true;
    maze[0][0].frontier = true;   
  } 

  int x = xValue, y = yValue, x2, y2; 

  while(verticesInMaze.size() < (count * count)) { 
    if(x < count - 1 && !IsInMaze(verticesInMaze, maze[x+1][y]) && !maze[x+1][y].frontier) {
      maze[x+1][y].frontier = true;  
      frontierVertices.push_back(maze[x+1][y]);
    }    
    if(x >= 1 && !IsInMaze(verticesInMaze, maze[x-1][y]) && !maze[x-1][y].frontier){
      maze[x-1][y].frontier = true;
      frontierVertices.push_back(maze[x-1][y]);
    }
    if(y < count - 1  && !IsInMaze(verticesInMaze, maze[x][y+1]) && !maze[x][y+1].frontier){
      maze[x][y+1].frontier = true;
      frontierVertices.push_back(maze[x][y+1]);
    }
    if(y >= 1 && !IsInMaze(verticesInMaze, maze[x][y-1]) && !maze[x][y-1].frontier){
      maze[x][y-1].frontier = true;
      frontierVertices.push_back(maze[x][y-1]);
    } 
 
    if(frontierVertices.size() > 0) { 
      int pos = rand() % frontierVertices.size(); 
      x2 = frontierVertices[pos].x;
      y2 = frontierVertices[pos].y;
        
      bool valid = true; 
      while(valid) {
        switch(rand()% 4) { 
          case 0:
            if(x2 >= 1 && IsInMaze(verticesInMaze, maze[x2-1][y2])) {
              maze[x2-1][y2].downEdge = true;
              //cout << x2-1 << y2 << "->" << x2 << y2 << endl;
              valid = false;
              break;
            }
          case 1:
            if(x2 < count - 1 && IsInMaze(verticesInMaze, maze[x2+1][y2])){
              maze[x2][y2].downEdge = true;  
              //out << x2+1 << y2 << "->" << x2 << y2 << endl;
              valid = false;
              break;
            }
          case 2:
            if(y2 < count - 1 && IsInMaze(verticesInMaze, maze[x2][y2+1])){ 
        
              maze[x2][y2].rightEdge = true;  
              //cout << x2 << y2 << "->" << x2 << y2+1 << endl;
              valid = false;
              break;
            }
          case 3:
            if(y2 >= 1 && IsInMaze(verticesInMaze, maze[x2][y2-1])){

              maze[x2][y2-1].rightEdge = true;
              //cout << x2 << y2-1 << "->" << x2 << y2 << endl;
              valid = false;
              break;
              }
          default:
            break;
        }
      }
      
      y = frontierVertices[pos].y;
      x = frontierVertices[pos].x; 
      
      verticesInMaze.push_back(maze[frontierVertices[pos].x][frontierVertices[pos].y]);
      maze[frontierVertices[pos].x][frontierVertices[pos].y].visited = true;
      frontierVertices.erase(frontierVertices.begin()+pos);    
    }
  }
  //PrintPrim(maze);
  DFS(maze, maze[0][0]);
  PrintPrim(maze);
  return;
}

bool Maze::IsInMaze(vector<Vertex> verticesInMaze, Vertex vert){
  bool inMaze = false;
  for(int i = 0; i < verticesInMaze.size(); ++i){
    if(vert.x == verticesInMaze[i].x && vert.y == verticesInMaze[i].y){
      return true;
    }
  }
  return false;

}

void Maze::PrintPrim(Vertex **maze) {

  cout << "+";
  for(int i = 0; i < count; ++i){
    cout << "--+";
  }
  cout << endl;
  
  for(int i = 0; i < count; ++i){
    cout << "|";
    for(int j = 0; j < count; ++j){
      if(maze[i][j].path && maze[i][j].rightEdge){
        cout << setw(3) << "* ";
      }
      else if(maze[i][j].path && !maze[i][j].rightEdge){
        cout << setw(3) << "* |";
      }
      else if(!maze[i][j].path && maze[i][j].rightEdge) {
        cout << setw(3) <<""; 
      }
      else {
        cout << setw(3) <<"|";
      }
    }
    cout << endl;
    cout << "+";
    for(int j = 0; j < count; ++j){
      if(maze[i][j].downEdge) {
        cout << setw(3) <<"+";
      }
      else {
        cout << setw(3) <<"--+";
      }
    }
    cout << endl;
  } 
  cout << "Count: " << pathCount << endl;
}

void Maze::DFS(Vertex **maze, Vertex current) {
  unvisitedVertices.push(maze[0][0]); // starting point is (0,0)
  maze[0][0].visitedSol = true;
  current = maze[0][0];
  int x = current.x;
  int y = current.y;
  //cout << "(" << current.x << ", " << current.y << ")" << endl; // cool
  Vertex end = maze[count - 1][count - 1];
  
  while(!(current == end)) { // the current needs to reach the end point
    //cout << "(" << current.x << ", " << current.y << ")" << endl;
    if(unvisitedNeighbors(maze, current, unvisitedVertices)){
      current = unvisitedVertices.top();
      current.visitedSol = true;
      unvisitedVertices.push(current);
    }
    else{
      unvisitedVertices.pop();
      current = unvisitedVertices.top();
    }
    //check if the vertx has neighbors
    // need adjacent vertices...again
  }
  
  // this is where we will need to do backtracking and stuff
  // depth first search
  /*
  Start by putting any one of the graph's vertices on top of a stack.
  Take the top item of the stack and add it to the visited list.
  Create a list of that vertex's adjacent nodes. Add the ones which aren't in the visited list to the top of the stack.
  Keep repeating steps 2 and 3 until the stack is empty.
  */
  Path(maze, unvisitedVertices);
  return;
}

bool Maze::unvisitedNeighbors(Vertex **maze, Vertex &current, stack<Vertex> &unvisitedVertices){
  int x = current.x;
  int y = current.y;
  // down/up  left/right
  // have to check edges omg duh

  if((x >= 1) && !maze[x-1][y].visitedSol && maze[x-1][y].downEdge){
    //cout << "in if 1. (" << x-1 << ", " << y << ")" << endl;
    maze[x-1][y].visitedSol = true;
    unvisitedVertices.push(maze[x-1][y]);
    return true;
  }
  else if(x < count - 1 && !maze[x+1][y].visitedSol && maze[x][y].downEdge){
   // cout << "elif 2. (" << x+1 << ", " << y << ")" << endl;
    maze[x+1][y].visitedSol = true;
  unvisitedVertices.push(maze[x+1][y]);
    return true;
  }
  else if(y >= 1 && !maze[x][y-1].visitedSol && maze[x][y-1].rightEdge){
    //cout << "elif 3. (" << x << ", " << y-1 << ")" << endl;
    maze[x][y-1].visitedSol = true;
    unvisitedVertices.push(maze[x][y-1]);
    return true;
  }
  else if(x < count - 1 && !maze[x][y+1].visitedSol && maze[x][y].rightEdge){
   // cout << "elif 4. (" << x << ", " << y+1 << ")" << endl;
    //cout << "(" << x << ", " << y+1 << ")" << endl;
    maze[x][y+1].visitedSol = true;
    unvisitedVertices.push(maze[x][y+1]);
    return true;
  }
  else{
    //cout << "no neighbors " << endl;
  }
  return false;
}

void Maze::Path(Vertex **maze, stack<Vertex> &unvisitedVertices){
  Vertex current;
  while(!unvisitedVertices.empty()){
    current = unvisitedVertices.top();
    maze[current.x][current.y].path = true;
    pathCount++;
    unvisitedVertices.pop();
  }
}
