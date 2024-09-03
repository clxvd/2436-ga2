#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <queue>
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
    //void SolveKruskal();

    void Prim(Vertex** maze, int xValue, int yValue);
    bool IsInMaze(vector<Vertex> verticesInMaze, Vertex vert);
    void PrintPrim(Vertex** maze);
    void DFS(Vertex** maze, Vertex current, bool kruskal);
    bool unvisitedNeighbors(Vertex** maze, Vertex& current, stack<Vertex>& unvisitedVertices);
    void Path(Vertex** maze, stack<Vertex>& unvisitedVertices, bool);
    int GetPathCount() { return pathCount; }

private:
    int count; //n x n
    int setCount = 0;
    int pathCount = 0;
    int vertexCount = 0;
    double score; //holds the percent
    vector<Vertex> tree; //saves vertices in order from kruskal
    vector<Edge> edgeTree;
    vector<Vertex> vertices; //random order vertices

    Vertex** maze;
    vector<Vertex> frontierVertices;
    stack<Vertex> unvisitedVertices;
    vector<Vertex> visitedVertices;

    bool method;


};

Maze::Maze() {
    count = 30;
    setCount = 0;

}

Maze::Maze(int c) {
    count = c;
    setCount = 0;

    int** maze;
}

void Maze::GenerateMaze() {
    int** maze = new int* [count];
    for (int i = 0; i < count; ++i) {
        maze[i] = new int[count];
        for (int j = 0; j < count; ++j) {
            maze[i][j] = setCount;
            setCount++;
        }
    }

}

void Maze::Kruskal() {

    queue<Edge> q;
    vector<int> edgeIndex;
    vertexCount = count * count;
    //cout << "Count and Vertex Count is : " << count << " " << vertexCount << endl;

    DisjointSet maze(vertexCount, count); //cells
    vector<Edge> edges = maze.ListOfEdges(); //list of all edges

    while (edgeIndex.size() != edges.size()) {
        int index = rand() % (edges.size());
        if (find(edgeIndex.begin(), edgeIndex.end(), index) == edgeIndex.end()) {//not present in the list
            q.push(edges[index]);
            edgeIndex.push_back(index);
        }
    }

    while (!q.empty()) {

        Edge e = q.front();
        Vertex x = e.a;
        Vertex y = e.b;
        char direction = e.direction;

        if (maze.Union(e)) {
            //cout << "merged: " << x.name << " " << y.name << endl;
            if (e.direction == 'R') {
                //cout << "R" << endl;
                x.rightEdge = true;

                int index = findIndex(vertices, x); //if not -1
                if (index == -1) { //not in vector
                    vertices.push_back(x);
                }
                else {
                    vertices[index].rightEdge = true;
                }
                index = findIndex(vertices, y); //if not -1
                if (index == -1) {
                    vertices.push_back(y);
                }
            }

            if (e.direction == 'D') {
                //cout << "D" << endl;
                x.downEdge = true;

                int index = findIndex(vertices, x); //if not -1
                if (index == -1) {
                    vertices.push_back(x);
                }
                else {
                    vertices[index].downEdge = true;
                }
                index = findIndex(vertices, y); //if not -1
                if (index == -1) {
                    vertices.push_back(y);
                }
            }

            int index = findIndex(edgeTree, e);
            if (index == -1) {
                edgeTree.push_back(e);
            }

            //cout << edgeCount << " ";
        }
        q.pop();
    }

    SortKruskalVertices();

    int k = 0;
    Vertex** matrix = new Vertex * [count];
    for (int i = 0; i < count; ++i) {
        matrix[i] = new Vertex[count];
        for (int j = 0; j < count; ++j) {

            matrix[i][j] = tree[k];
            k++;
        }
    }

    DFS(matrix, matrix[count - 1][count - 1], true);

}

void Maze::SortKruskalVertices() {

    for (int i = 0; i < count; ++i) {
        for (int j = 0; j < count; ++j) {
            Vertex a = Vertex();
            a.x = i;
            a.y = j;
            int index = findIndex(vertices, a); //if not -1
            tree.push_back(vertices[index]);
        }
    }


}

void Maze::PrintKruskal() {
    //cout << count << endl;
    //cout << "Vertex Count " << vertexCount << endl;

    cout << "+";
    for (int i = 0; i < count; i++) {
        cout << "--+";
    }
    cout << endl;

    for (int i = 0; i < count; ++i) {
        cout << "|";
        for (int j = 0; j < count; ++j) {
            Vertex a = Vertex();
            a.x = i;
            a.y = j;
            int index = findIndex(vertices, a); //if not -1
            if (vertices[index].path && vertices[index].rightEdge) {
                cout << setw(3) << "o ";
            }
            else if (vertices[index].path && !vertices[index].rightEdge) {
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
            if (vertices[index].downEdge) {
                //cout << solution[index].distance
                cout << setw(3) << "+";
            }
            else {
                cout << "--+";
            }
        }
        cout << endl;
    }
    cout << "Score: " << pathCount << " " << setprecision(7) << score * 100 << "% (" << score << ")" << endl;
}

void Maze::Prim(Vertex** maze, int xValue, int yValue) {
    vector<Vertex> verticesInMaze;
    vertexCount = count * count;

    if (!maze[xValue][yValue].visited) {
        verticesInMaze.push_back(maze[xValue][yValue]);
        maze[xValue][yValue].visited = true;
        maze[xValue][yValue].frontier = true;
    }

    int x = xValue, y = yValue, x2, y2;

    while (verticesInMaze.size() < (count * count)) {
        if (x < count - 1 && !IsInMaze(verticesInMaze, maze[x + 1][y]) && !maze[x + 1][y].frontier) {
            maze[x + 1][y].frontier = true;
            frontierVertices.push_back(maze[x + 1][y]);
        }
        if (x >= 1 && !IsInMaze(verticesInMaze, maze[x - 1][y]) && !maze[x - 1][y].frontier) {
            maze[x - 1][y].frontier = true;
            frontierVertices.push_back(maze[x - 1][y]);
        }
        if (y < count - 1 && !IsInMaze(verticesInMaze, maze[x][y + 1]) && !maze[x][y + 1].frontier) {
            maze[x][y + 1].frontier = true;
            frontierVertices.push_back(maze[x][y + 1]);
        }
        if (y >= 1 && !IsInMaze(verticesInMaze, maze[x][y - 1]) && !maze[x][y - 1].frontier) {
            maze[x][y - 1].frontier = true;
            frontierVertices.push_back(maze[x][y - 1]);
        }

        if (frontierVertices.size() > 0) {
            int pos = rand() % frontierVertices.size();
            x2 = frontierVertices[pos].x;
            y2 = frontierVertices[pos].y;

            bool valid = true;
            while (valid) {
                switch (rand() % 4) {
                case 0:
                    if (x2 >= 1 && IsInMaze(verticesInMaze, maze[x2 - 1][y2])) {
                        maze[x2 - 1][y2].downEdge = true;
                        //cout << x2-1 << y2 << "->" << x2 << y2 << endl;
                        valid = false;
                        break;
                    }
                case 1:
                    if (x2 < count - 1 && IsInMaze(verticesInMaze, maze[x2 + 1][y2])) {
                        maze[x2][y2].downEdge = true;
                        //out << x2+1 << y2 << "->" << x2 << y2 << endl;
                        valid = false;
                        break;
                    }
                case 2:
                    if (y2 < count - 1 && IsInMaze(verticesInMaze, maze[x2][y2 + 1])) {

                        maze[x2][y2].rightEdge = true;
                        //cout << x2 << y2 << "->" << x2 << y2+1 << endl;
                        valid = false;
                        break;
                    }
                case 3:
                    if (y2 >= 1 && IsInMaze(verticesInMaze, maze[x2][y2 - 1])) {

                        maze[x2][y2 - 1].rightEdge = true;
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
            frontierVertices.erase(frontierVertices.begin() + pos);
        }
    }
    //PrintPrim(maze);
    DFS(maze, maze[29][29], false);

    return;
}

bool Maze::IsInMaze(vector<Vertex> verticesInMaze, Vertex vert) {
    bool inMaze = false;
    for (int i = 0; i < verticesInMaze.size(); ++i) {
        if (vert.x == verticesInMaze[i].x && vert.y == verticesInMaze[i].y) {
            return true;
        }
    }
    return false;

}

void Maze::PrintPrim(Vertex** maze) {

    cout << "+";
    for (int i = 0; i < count; ++i) {
        cout << "--+";
    }
    cout << endl;

    for (int i = 0; i < count; ++i) {
        cout << "|";
        for (int j = 0; j < count; ++j) {
            if (maze[i][j].path && maze[i][j].rightEdge) {
                cout << setw(3) << " * ";
            }
            else if (maze[i][j].path && !maze[i][j].rightEdge) {
                cout << setw(3) << "*|";
            }
            else if (!maze[i][j].path && maze[i][j].rightEdge) {
                cout << setw(3) << "";
            }
            else {
                cout << setw(3) << "|";
            }
        }
        cout << endl;
        cout << "+";
        for (int j = 0; j < count; ++j) {
            if (maze[i][j].downEdge) {
                cout << setw(3) << "+";
            }
            else {
                cout << setw(3) << "--+";
            }
        }
        cout << endl;
    }
    cout << "Score: " << pathCount << " " << setprecision(7) << score * 100 << "% (" << score << ")" << endl;

}

void Maze::DFS(Vertex** maze, Vertex current, bool kruskal) {
    unvisitedVertices.push(maze[29][29]); // starting point is (0,0)
    maze[29][29].visitedSol = true;

    current = maze[count - 1][count - 1];

    int x = maze[29][29].x;
    int y = maze[29][29].y;

    while (!(current == maze[0][0])) { // the current needs to reach the end point
        //cout << "(" << current.x << ", " << current.y << ")" << endl;
        if (unvisitedNeighbors(maze, current, unvisitedVertices)) {
            current = unvisitedVertices.top();
            current.visitedSol = true;
            //unvisitedVertices.push(current);
        }
        else {
            unvisitedVertices.pop();
            current = unvisitedVertices.top();
        }
        //check if the vertx has neighbors
    }

    /*
    Start by putting any one of the graph's vertices on top of a stack.
    Take the top item of the stack and add it to the visited list.
    Create a list of that vertex's adjacent nodes. Add the ones which aren't in the visited list to the top of the stack.
    Keep repeating steps 2 and 3 until the stack is empty.
    */
    Path(maze, unvisitedVertices, kruskal);
    return;
}

bool Maze::unvisitedNeighbors(Vertex** maze, Vertex& current, stack<Vertex>& unvisitedVertices) {
    int x = current.x;
    int y = current.y;
    // down/up  left/right
    // have to check edges omg duh

    if ((x >= 1) && !maze[x - 1][y].visitedSol && maze[x - 1][y].downEdge) {
        //cout << "in if 1. (" << x-1 << ", " << y << ")" << endl;
        maze[x - 1][y].visitedSol = true;
        unvisitedVertices.push(maze[x - 1][y]);
        return true;
    }
    else if (x < count - 1 && !maze[x + 1][y].visitedSol && maze[x][y].downEdge) {
        // cout << "elif 2. (" << x+1 << ", " << y << ")" << endl;
        maze[x + 1][y].visitedSol = true;
        unvisitedVertices.push(maze[x + 1][y]);
        return true;
    }
    else if (y >= 1 && !maze[x][y - 1].visitedSol && maze[x][y - 1].rightEdge) {
        //cout << "elif 3. (" << x << ", " << y-1 << ")" << endl;
        maze[x][y - 1].visitedSol = true;
        unvisitedVertices.push(maze[x][y - 1]);
        return true;
    }
    else if (x < count - 1 && !maze[x][y + 1].visitedSol && maze[x][y].rightEdge) {
        // cout << "elif 4. (" << x << ", " << y+1 << ")" << endl;
         //cout << "(" << x << ", " << y+1 << ")" << endl;
        maze[x][y + 1].visitedSol = true;
        unvisitedVertices.push(maze[x][y + 1]);
        return true;
    }
    else {
        //cout << "no neighbors " << endl;
    }
    return false;
}

void Maze::Path(Vertex** maze, stack<Vertex>& unvisitedVertices, bool kruskal) {
    Vertex current;
    pathCount = 0;
    while (!unvisitedVertices.empty()) {

        current = unvisitedVertices.top();
        //cout << pathCount << " "<< current.x << ", " << current.y << endl;
        maze[current.x][current.y].path = true;

        if (kruskal) {
            int index = findIndex(vertices, current); //this is for kruskals.
            vertices[index].path = true;
        }

        pathCount += 1;
        //cout << pathCount << endl;
        unvisitedVertices.pop();
    }
    score = pathCount / double(vertexCount);
}
