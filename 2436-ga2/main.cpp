#include "maze.h"

int main() {

  ifstream fin("input1.txt");

  int seedNum;
  string method;
  seedNum = 10; 
  srand(seedNum);
  method = "kruskal"; // "prim";
  
  Maze m;
  m.Kruskal();


  /*
  int count = 5; //this will probably have to be cleaned up at the end. and added into the maze class
  int vertexCount = count*count;
  int setCount = 0;

  queue<Vertex> frontierVertices; // would be used to check vertices next to the ones already added in maze
  // fin >> seedNum >> method;
        // comment out later
  

  Vertex **maze = new Vertex *[count];
  for (int i = 0; i < count; ++i) {
    maze[i] = new Vertex[count];
    for(int j = 0; j < count; ++j) {
      maze[i][j].name = setCount;
      maze[i][j].x = i;
      maze[i][j].y = j;
      setCount++; 
    }
  }

  
  if (method == "kruskal") {
    //Kruskal(count, maze, edges, setCount);
    m.Kruskal();
  }
  
  //m.PrintPrim(maze);
  //m.Prim(maze, 0, 0);
  //m.PrintPrim(maze);
*/


  return 0;
}
