#include "maze.h"

//seedNum = 83878787; Winner!

int main() {

    //ifstream fin("winnerwinner.txt"); 
    ifstream fin("input.txt");

    int seedNum;
    string method;

    fin >> seedNum >> method;

    int count = 30;
    int setCount = 0;
    srand(seedNum);

    Maze m(count);

    if (method == "kruskal") {

        m.Kruskal();
        m.PrintKruskal();

    }
    else {
        Vertex** maze = new Vertex * [count];
        for (int i = 0; i < count; ++i) {
            maze[i] = new Vertex[count];
            for (int j = 0; j < count; ++j) {
                maze[i][j].name = setCount;
                maze[i][j].x = i;
                maze[i][j].y = j;
                setCount++;
            }
        }
        int x = rand() % 30;
        int y = rand() % 30;
        m.Prim(maze, x, y);
        m.PrintPrim(maze);
    }
    cout << "Seed: " << seedNum << endl;
    return 0;
}
