// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include <cmath>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    int row=cur.row;
    int column=cur.col;

    for(int i=row-1;i<=row+1;i++){
        for(int j=column-1;j<=column+1;j++){
            if((abs(i+j-column-row)==1)&&(i>=0)&&(i<maze.numRows())&&(j>=0)&&(j<maze.numCols())){
                if(maze[i][j]){
                    GridLocation temp;
                    temp.row=i;
                    temp.col=j;
                    neighbors.add(temp);
                }

            }
        }
    }
    return neighbors;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void validatePath(Grid<bool>& maze, Stack<GridLocation> path) {

    //判断终点正确
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};

    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }
    //判断起点正确
    GridLocation mazeEntrance = {0,0};
    int pathLen = path.size();
    Stack<GridLocation> tempPath = path;
    GridLocation start;
    for(int i=0;i<pathLen;i++){
        start=tempPath.pop();
    }
    if(start!=mazeEntrance){
        error("Path does not start at maze entrance");
    }

    //判断是否符合移动规则
    Stack<GridLocation> tempMovePath = path;
    for(int i=0;i<pathLen-1;i++){
        GridLocation head =tempMovePath.pop();
        GridLocation tail =tempMovePath.peek();
        Set<GridLocation> neighbor = generateValidMoves(maze,head);
        if(!(neighbor.contains(tail))){
            error("there are invalid moves in this path");
        }

    }
    //判断是否循环了两次
    Stack<GridLocation> tempCountPath = path;
    Set<GridLocation> gridSet;
    for(int i=0;i<pathLen;i++){
        gridSet.add(tempCountPath.pop());
    }
    int gridNum=gridSet.size();
    if(gridNum!=pathLen){
        error("there are looping paths in this path");
    }
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    Stack<GridLocation> path;
    Stack<GridLocation> entrancePath ={{0, 0}};
    GridLocation exit ={maze.numRows()-1, maze.numCols()-1};
    Queue<Stack<GridLocation>> pathQueue;
    pathQueue.enqueue(entrancePath);
    Set<GridLocation> visitedGrid;
    visitedGrid.add({0,0});
    while(pathQueue.size()>0){
        path=pathQueue.dequeue();
        MazeGraphics::highlightPath(path,"green",2);
        GridLocation tempGrid=path.peek();
        if(tempGrid==exit){
            return path;
        }
        else{
            Set<GridLocation> nextValidGrids = generateValidMoves(maze,tempGrid);
            for(GridLocation vaildNeighbor:nextValidGrids){
                if(!(visitedGrid.contains(vaildNeighbor))){
                    visitedGrid.add(vaildNeighbor);
                    Stack<GridLocation> nextPath=path;
                    nextPath.push(vaildNeighbor);
                    pathQueue.enqueue(nextPath);
                }
            }
        }
    }



    return path;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

// TODO: add your test cases here
STUDENT_TEST("test the genetateValidMoves"){
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 0};
    Set<GridLocation> expected = {{0,1}, {1, 0}};


    EXPECT_EQUAL(generateValidMoves(maze, side), expected);


}
STUDENT_TEST("test1 the genetateValidMoves"){
    Grid<bool> maze = {{true, false, true},
                       {false, true, true},
                       {true, true, true}};
    GridLocation side = {0, 2};
    Set<GridLocation> expected = {{1, 2}};
    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}
STUDENT_TEST("test2 the genetateValidMoves"){


    Grid<bool> maze = {{true, false, true},
                       {false, true, false},
                       {true, true, true}};
    GridLocation side = {1, 1};
    Set<GridLocation> expected = {{2, 1}};
    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}
STUDENT_TEST("validatePath test") {
    Grid<bool> maze = {{true, false,true},
                       {true, true,true},
                       {true,false, true}};
    Stack<GridLocation> not_end_at_exit = { {0, 0}, {1, 0},{2, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1},{1, 2},{2, 2} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {0, 2},{1, 2},{2, 2} };
    Stack<GridLocation> teleport = { {0 ,0}, {2, 2} };
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {2, 0}, {1, 0}, {1, 1},{1, 2},{2, 2} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}
STUDENT_TEST("solveMaze on file 25x15") {
    Grid<bool> maze;
    readMazeFile("res/25x15.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}
STUDENT_TEST("solveMaze on file 33x41") {
    Grid<bool> maze;
    readMazeFile("res/33x41.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}
