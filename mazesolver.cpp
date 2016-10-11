#include "mazesolver.h"
#include "mazedisplay.h"
#include "visitedtracker.h"
#include "heap.h"
#include <QMessageBox>
#include <queue>
#include <iostream>
#include <stack>
#include <vector>
#include <cmath>
using namespace std;

MazeSolver::MazeSolver(Maze * m, MazeDisplay * md)
    : maze(m), display(md)
{}



void MazeSolver::solveByDFSRecursive(){
    int r, c;
    int numSquares = maze->numRows() * maze->numCols();
    VisitedTracker vt(maze->numRows(), maze->numCols());
    std::vector<Direction> parent(numSquares);
    int numExplored;
    vt.setVisited(maze->getStartRow(), maze->getStartCol());
    std::stack<std::pair<int, int>> q;
    q.push(std::pair<int,int>(maze->getStartRow(), maze->getStartCol()));
    std::pair<int, int> v = q.top();
    r = v.first;
    c = v.second;
    bool test = DFS_Helper(vt, numExplored, parent, q, r, c); 
}
bool MazeSolver::DFS_Helper(VisitedTracker vt, int numExplored, vector<Direction> parent, stack<pair<int,int>> q, int r, int c){
    numExplored++;
    if( r == maze->getGoalRow() && c == maze->getGoalCol() ){
        std::vector<Direction> path;
        std::stack<Direction> st;
        while( r != maze->getStartRow() || c != maze->getStartCol()) {
             st.push( parent[ squareNumber(r,c) ]);
                switch( st.top() )
                {
                case UP: r++; break; 
                case DOWN: r--; break;
                case LEFT: c++; break;
                case RIGHT: c--; break;
                }
        }
        while ( ! st.empty()){
            path.push_back(st.top());
            st.pop();
        }
        display->reportSolution(path, vt, numExplored);
    }
    else{
         if ( maze->canTravel(UP, r, c) && ! vt.isVisited(r-1,c)){
            parent[squareNumber(r-1, c)] = UP;
            vt.setVisited(r-1,c);
            q.push(std::pair<int,int>(r-1, c));
            bool check = DFS_Helper(vt,numExplored, parent, q, r-1, c);
        }
        if ( maze->canTravel(DOWN, r, c) && ! vt.isVisited(r+1,c) ){
            parent[squareNumber(r+1, c)] = DOWN;
            vt.setVisited(r+1, c);
            q.push(std::pair<int,int>(r+1, c));
            bool check = DFS_Helper(vt,numExplored, parent, q, r+1, c);
        }
        if ( maze->canTravel(LEFT, r, c) && ! vt.isVisited(r,c-1) ){
            parent[squareNumber(r, c-1)] = LEFT;
            vt.setVisited(r, c-1);
            q.push(std::pair<int,int>(r, c-1));
            bool check = DFS_Helper(vt,numExplored, parent, q, r, c-1);
        }
        if ( maze->canTravel(RIGHT, r, c) && ! vt.isVisited(r, c+1) ){
            parent[squareNumber(r, c+1)] = RIGHT;
            vt.setVisited(r, c+1);
            q.push(std::pair<int,int>(r, c+1));
            bool check = DFS_Helper(vt,numExplored, parent,q, r, c+1);
        }
        q.pop();
    }
    return true;
}
struct check{
    pair<int,int> p;
    int priority;
    check(int r, int c, int y){
        p.first = r;
        p.second = c;
        priority = y;
    };
};
struct compare{
    bool operator() (const check& a, const check& b){
        return (a.priority < b.priority); 
    }
};

void MazeSolver::solveByAStar(int choice){  
    int numSquares = maze->numRows() * maze->numCols();
    VisitedTracker vt(maze->numRows(), maze->numCols());
    pair<int,int> x = pair<int,int>(maze->getStartRow(), maze->getStartCol());
    pair<int,int> y = pair<int,int>(maze->getGoalRow(), maze->getGoalCol());
    priority_queue<check, vector<check> , compare> qq;
    double value = aStar_Helper(x,y,1);
    
    // so here is where i would impliment A*. Depending on the choice
    // given call helper function with that choice(2/Manhattan) would
    // return abs(x1-x2) + abs(y1-y2) and Euclidean would use the 
    // pythagorean theorem. using that and isVisited to traverse the 
    // map while comparing priorities to determine the best path.


    //qq.push(value);
    do{
        //qq.push(value);
        ++numExplored;
        if(choice == 1){
            //heuristic 0
            //A* with a heuristic of 0 is the same as BFS
            int r, c;
            int numSquares = maze->numRows() * maze->numCols();
            VisitedTracker vt(maze->numRows(), maze->numCols());
            std::vector<Direction> parent( numSquares ); // what was my immediate prior direction to get here?
            int numExplored = 0;
            vt.setVisited(maze->getStartRow(), maze->getStartCol());
            std::queue<std::pair<int, int>> q;
            q.push(std::pair<int,int>(maze->getStartRow(), maze->getStartCol()));
            while( ! q.empty() ){
                std::pair<int, int> v = q.front();
                q.pop();
                numExplored++;
                r = v.first;
                c = v.second;
                if( r == maze->getGoalRow() && c == maze->getGoalCol() ){
                    std::vector<Direction> path;
                    std::stack<Direction> st;
                    while( r != maze->getStartRow() || c != maze->getStartCol()){
                        st.push( parent[ squareNumber(r,c) ]);
                        switch( st.top() )
                        {
                        case UP: r++; break; // yes, r++.  I went up to get here...
                        case DOWN: r--; break;
                        case LEFT: c++; break;
                        case RIGHT: c--; break;
                        }
                    }
                    while ( ! st.empty() ){
                        path.push_back(st.top());
                        st.pop();
                    }
                    display->reportSolution(path, vt, numExplored);
                    return;
                }
                if ( maze->canTravel(UP, r, c) && ! vt.isVisited(r-1,c)){
                    parent[squareNumber(r-1, c)] = UP;
                    vt.setVisited(r-1,c);
                    q.push(std::pair<int,int>(r-1, c));
                }
                if ( maze->canTravel(DOWN, r, c) && ! vt.isVisited(r+1,c) ){
                    parent[squareNumber(r+1, c)] = DOWN;
                    vt.setVisited(r+1, c);
                    q.push(std::pair<int,int>(r+1, c));
                }
                if ( maze->canTravel(LEFT, r, c) && ! vt.isVisited(r,c-1) ){
                    parent[squareNumber(r, c-1)] = LEFT;
                    vt.setVisited(r, c-1);
                    q.push(std::pair<int,int>(r, c-1));
                }
                if ( maze->canTravel(RIGHT, r, c) && ! vt.isVisited(r, c+1) ){
                    parent[squareNumber(r, c+1)] = RIGHT;
                    vt.setVisited(r, c+1);
                    q.push(std::pair<int,int>(r, c+1));
                }
            }


        }
        else if(choice == 2){
            //heuristic of Manhattan Distance
            //path size should be the same across all A*
            //calls. this verifies their validity
        }
        else if(choice == 3){
            //heuristic of Euclidean Distance

        }
        else{



        }
    }while(!qq.empty());
}
double MazeSolver::aStar_Helper(pair<int,int> x, pair<int,int> y, int c){
    int x1 = x.first;
    int y1 = x.second;
    int x2 = y.first;
    int y2 = y.second;
    if(c == 1){
        return 0;
    }
    else if(c == 2){
        double dist =  (abs(x1-x2) + abs(y1-y2));
        return dist;
    }
    if(c == 3){
    double x = x1-x2;
    double y = y1-y2;
    double dist = pow(x, 2) + pow(y, 2);
    dist = sqrt(dist);
    return dist;
    }
}
void MazeSolver::solveByDFSIterative()
{
    int r, c;
    int numSquares = maze->numRows() * maze->numCols();
    VisitedTracker vt(maze->numRows(), maze->numCols());

    std::vector<Direction> parent( numSquares ); // what was my immediate prior direction to get here?
    int numExplored = 0;
    vt.setVisited(maze->getStartRow(), maze->getStartCol());
    std::stack<std::pair<int, int>> q;
    q.push(std::pair<int,int>(maze->getStartRow(), maze->getStartCol()));

    while( ! q.empty() )
    {
        std::pair<int, int> v = q.top();
        q.pop();
        numExplored++;

        r = v.first;
        c = v.second;

        /* This one if statement is different from the pseudo-code provided
           in lecture, because we want to stop when we've reached the goal.
           The code provided in lecture was for if you wanted to do a BFS
           that explored the entire graph.
        */
        if( r == maze->getGoalRow() && c == maze->getGoalCol() )
        {
            std::vector<Direction> path;
            std::stack<Direction> st;

            while( r != maze->getStartRow() || c != maze->getStartCol())
            {
                st.push( parent[ squareNumber(r,c) ]);
                switch( st.top() )
                {
                case UP: r++; break; // yes, r++.  I went up to get here...
                case DOWN: r--; break;
                case LEFT: c++; break;
                case RIGHT: c--; break;
                }
            }
            while ( ! st.empty() )
            {
                path.push_back(st.top());
                st.pop();
            }
            display->reportSolution(path, vt, numExplored);
            return;
        }

        /*
         * Now we're back to code that looks like the pseudo-code you've seen.
         * The difference here is that we aren't keeping track of distances;
           that's similar to the difference above.  You could add, and ignore,
           the vector that would result if you wanted to do so.
         */
        if ( maze->canTravel(UP, r, c) && ! vt.isVisited(r-1,c))
        {
            parent[squareNumber(r-1, c)] = UP;
            vt.setVisited(r-1,c);
            q.push(std::pair<int,int>(r-1, c));
        }
        // Note:  this is NOT "else if" ...
        if ( maze->canTravel(DOWN, r, c) && ! vt.isVisited(r+1,c) )
        {
            parent[squareNumber(r+1, c)] = DOWN;
            vt.setVisited(r+1, c);
            q.push(std::pair<int,int>(r+1, c));
        }
        if ( maze->canTravel(LEFT, r, c) && ! vt.isVisited(r,c-1) )
        {
            parent[squareNumber(r, c-1)] = LEFT;
            vt.setVisited(r, c-1);
            q.push(std::pair<int,int>(r, c-1));
        }
        if ( maze->canTravel(RIGHT, r, c) && ! vt.isVisited(r, c+1) )
        {
            parent[squareNumber(r, c+1)] = RIGHT;
            vt.setVisited(r, c+1);
            q.push(std::pair<int,int>(r, c+1));
        }
    }
}

void MazeSolver::solveByBFS()
{
    /* In lecture on Tuesday March 22, we had a graph
       with vertices numbered 0 to n-1, inclusive.
       Instead, we have vertices with two numbers,
       row and col, in the range:
       [0, maze->numRows()-1], [0, maze->numCols() -1 ]
       to assign each a unique number [0, maze->numRows() * maze->numCols() -1]
       we will say that maze square (r,c) is really number
       r * maze->numCols() + c
    */
    int r, c;
    int numSquares = maze->numRows() * maze->numCols();
    VisitedTracker vt(maze->numRows(), maze->numCols());

    std::vector<Direction> parent( numSquares ); // what was my immediate prior direction to get here?
    int numExplored = 0;
    vt.setVisited(maze->getStartRow(), maze->getStartCol());
    std::queue<std::pair<int, int>> q;
    q.push(std::pair<int,int>(maze->getStartRow(), maze->getStartCol()));

    while( ! q.empty() )
    {
        std::pair<int, int> v = q.front();
        q.pop();
        numExplored++;

        r = v.first;
        c = v.second;

        /* This one if statement is different from the pseudo-code provided
           in lecture, because we want to stop when we've reached the goal.
           The code provided in lecture was for if you wanted to do a BFS
           that explored the entire graph.
        */
        if( r == maze->getGoalRow() && c == maze->getGoalCol() )
        {
            std::vector<Direction> path;
            std::stack<Direction> st;

            while( r != maze->getStartRow() || c != maze->getStartCol())
            {
                st.push( parent[ squareNumber(r,c) ]);
                switch( st.top() )
                {
                case UP: r++; break; // yes, r++.  I went up to get here...
                case DOWN: r--; break;
                case LEFT: c++; break;
                case RIGHT: c--; break;
                }
            }
            while ( ! st.empty() )
            {
                path.push_back(st.top());
                st.pop();
            }
            display->reportSolution(path, vt, numExplored);
            return;
        }

        /*
         * Now we're back to code that looks like the pseudo-code you've seen.
         * The difference here is that we aren't keeping track of distances;
           that's similar to the difference above.  You could add, and ignore,
           the vector that would result if you wanted to do so.
         */
        if ( maze->canTravel(UP, r, c) && ! vt.isVisited(r-1,c))
        {
            parent[squareNumber(r-1, c)] = UP;
            vt.setVisited(r-1,c);
            q.push(std::pair<int,int>(r-1, c));
        }
        // Note:  this is NOT "else if" ...
        if ( maze->canTravel(DOWN, r, c) && ! vt.isVisited(r+1,c) )
        {
            parent[squareNumber(r+1, c)] = DOWN;
            vt.setVisited(r+1, c);
            q.push(std::pair<int,int>(r+1, c));
        }
        if ( maze->canTravel(LEFT, r, c) && ! vt.isVisited(r,c-1) )
        {
            parent[squareNumber(r, c-1)] = LEFT;
            vt.setVisited(r, c-1);
            q.push(std::pair<int,int>(r, c-1));
        }
        if ( maze->canTravel(RIGHT, r, c) && ! vt.isVisited(r, c+1) )
        {
            parent[squareNumber(r, c+1)] = RIGHT;
            vt.setVisited(r, c+1);
            q.push(std::pair<int,int>(r, c+1));
        }
    }
}


int MazeSolver::squareNumber(int r, int c) const
{
    return maze->numCols() * r + c;
}


