#include <iostream>
#include <cstring>

struct Grid {
    int gridRow;
    int gridColumn;       
    Grid(int x, int y): gridColumn(x), gridRow(y) {};
};

struct Robot {
    int xRobot;
    int yRobot;
    Robot(int x, int y): xRobot(x), yRobot(y) {};
};

struct Destination {
    int xDestination;
    int yDestination;
    Destination(int x, int y): xDestination(x), yDestination(y) {};
};

struct OccupiedCells {
    int xLowerLeft;
    int yLowerLeft;
    int xUpperRight;
    int yUpperRight;
    OccupiedCells(int xLL, int yLL, int xUR, int yUR): xLowerLeft(xLL), yLowerLeft(yLL), xUpperRight(xUR), yUpperRight(yUR) {};
};



struct OccupancyMap{
    Grid grid;
    Robot robot; 
    Destination destination;
    OccupiedCells occupiedCells;
    OccupancyMap(Grid g, Robot r, Destination d, OccupiedCells oc): grid(g), robot(r), destination(d), occupiedCells(oc) {};
    

    const char*** renderGrid(){
        
        // Create dynamic 2D Array
        // Note that 🤖, 🚩, ⬛ and 🔳 are "constant char*" type, thus 2D array becomes "pointer to pointer to pointers"
        int col = grid.gridColumn;
        int rows = grid.gridRow;
        const char*** gridMap = new const char**[rows];
        for (int r=rows-1; r >= 0; r--) {
            gridMap[r] = new const char*[col];
        }
        
        // Populate the dynamic 2D Array while rendering the Occupancy Map (robot + destination + blockage)
        for (int r=grid.gridRow-1; r >= 0; r--) {
            for (int c = 0; c < grid.gridColumn; c++) {

                if(c == robot.xRobot && r == robot.yRobot) {
                    std::cout << "🤖"; 
                    gridMap[c][r] = "🤖";
                }
                else if(c == destination.xDestination && r == destination.yDestination) {
                    std::cout << "🚩";
                    gridMap[c][r] =  "🚩";
                }
                else if ((unsigned)(c-occupiedCells.xLowerLeft) <= (occupiedCells.xUpperRight - occupiedCells.xLowerLeft) &&
                    (unsigned)(r-occupiedCells.yLowerLeft) <= (occupiedCells.yUpperRight - occupiedCells.yLowerLeft)) {
                    std::cout << "⬛"; 
                    gridMap[c][r] =  "⬛";
                }
                else {
                    std::cout << "🔳";
                    gridMap[c][r] =  "🔳";
                }
            }
            std::cout << "\n";
        }

        
        
        // std::cout << "\n" << "Symbols that can be used in the map: ";
        // std::cout << "\n";
        // std::cout << "🔳"; 
        // std::cout << "⬛"; 
        // std::cout << "❌"; 
        // std::cout << "⚓"; 
        // std::cout << "🚩"; 
        // std::cout << "⭐"; 
        // std::cout << "▶️"; 
        // std::cout << "◀️"; 
        // std::cout << "🔼"; 
        // std::cout << "🔽"; 
        // std::cout << "🔻"; 
        // std::cout << "🤖"; 
        // std::cout << "➡️​"; 
        // std::cout << "⬅️​"; 
        // std::cout << "⬆️​"; 
        // std::cout << "⬇️"; 

        return gridMap; 
    }

};


class Solution {
    
    public:
        const char*** theGrid;
        Robot robot;
        Destination destination;
        Solution(const char*** tG, Robot r, Destination d): theGrid(tG), robot(r), destination(d)  {};
        
        int xDirection;
        int yDirection;
        //move the robot
        int xMove;
        int yMove;
        int step = 1;
    
        void solve() {

            while (robot.yRobot != destination.yDestination || robot.xRobot != destination.xDestination) {    

                if (robot.xRobot < destination.xDestination) {
                    xDirection = 1;             //going right
                } else if (robot.xRobot > destination.xDestination)  { 
                    xDirection = -1;            //going left
                } else { xDirection = 0; }      //stay
                    
                if (robot.yRobot < destination.yDestination) {
                    yDirection = 1;             //going up
                } else if (robot.yRobot > destination.yDestination) {
                    yDirection = -1;            //going down
                } else  { yDirection = 0; }     //stay
                

                if (xDirection == 0) {
                    divert(0);
                }
                
                else if (xDirection == 1) {
                    //check cell at the right
                    robot.xRobot += step;
                    if(strncmp((theGrid[robot.xRobot][robot.yRobot]), "🔳", 1) == 0) {
                        std::cout << "\n Moving to the right.";
                        theGrid[robot.xRobot][robot.yRobot] = "▶️";                        
                    } 
                    else {
                        // std::cout << "\n Robot was blocked on the right... try y-direction.";
                        robot.xRobot -= step;
                        divert(1);
                    }

                }

                else if (xDirection == -1) {
                    //check cell at the right
                    robot.xRobot -= step;
                    if(strncmp((theGrid[robot.xRobot][robot.yRobot]), "🔳", 1) == 0) {
                        std::cout << "\n Moving to the left.";
                        theGrid[robot.xRobot][robot.yRobot] = "◀️";
                    } 
                    else {
                        std::cout << "\n Robot was blocked on the left... try y-direction.";
                        robot.xRobot += step;
                        divert(-1);
                    }
                }




            }

            std::cout << "\n Robot reached the target.";

        }


        void divert(int blocked) {

            if(blocked != 0) {
                if (yDirection == 1) {
                    //while right side is still blocked, continue going up
                    while(theGrid[robot.xRobot+blocked][robot.yRobot] == "⬛") {
                        divertUp();
                    }
                }

                else if (yDirection <= 0) {
                    //while right side is still blocked, continue going down
                    while(theGrid[robot.xRobot+blocked][robot.yRobot] == "⬛") {
                        divertDown();
                    }
                }
            }

            else {
                if (yDirection == 1) {
                    divertUp();
                }
                else if (yDirection == -1) {
                    divertDown();
                }
            }
        }

        void divertUp() {
            robot.yRobot += step;
            if(strncmp((theGrid[robot.xRobot][robot.yRobot]), "🔳", 1) == 0  ||
            strncmp((theGrid[robot.xRobot][robot.yRobot]), "🚩", 1) == 0) {
                std::cout << "\n Moving up.";
                theGrid[robot.xRobot][robot.yRobot] = "🔼";
            } 
            else {
                std::cout << "\n Robot was blocked up... reversing y-direction.";
                robot.yRobot -= step;
            }
        }
        

        void divertDown() {
            robot.yRobot -= step;
            if(strncmp((theGrid[robot.xRobot][robot.yRobot]), "🔳", 1) == 0 || 
            strncmp((theGrid[robot.xRobot][robot.yRobot]), "🚩", 1) == 0) {
                std::cout << "\n Moving down.";
                theGrid[robot.xRobot][robot.yRobot] = "🔽";
            } 
            else {
                std::cout << "\n Robot was blocked up... reversing y-direction.";
                robot.yRobot += step;
            }
        }

        void printMap(Grid grid) {

            int col = grid.gridColumn;
            int row = grid.gridRow;

            std::cout << "\n\n Printing the Solution. \n\n";
            
            for (int r=row-1; r >= 0; r--) {
                for (int c = 0; c < col; c++) {
                    std::cout << theGrid[c][r];
                }
                std::cout << "\n";
            }

        }
        
};

int main() {
    
    Grid grid(9, 9);                          //Grid (column, row)  Note:  cell 0,0 is at lower left corner
    Robot robot(7,4);                           //Robot (x, y coordinates)
    Destination destination(1, 4);              //Destination (x, y coordinates)
    OccupiedCells occupiedCells(3, 2, 4, 7);    //Occupied Cells (xLL, yLL, xUR, yUR - LowerLeft and UpperRight corner of the blocked cells)
    OccupancyMap occupancyMap(grid, robot, destination, occupiedCells);

    const char*** theGrid = occupancyMap.renderGrid();    //Create Occupancy Map


    //check if robot and destination is at the specified cells as indicated in the robot and destination objects
    std::cout << "\n The Robot: " << theGrid[robot.xRobot][robot.yRobot] << " at column: " << robot.xRobot << ", row: " << robot.yRobot ;
    std::cout << "\n The Destination: " << theGrid[destination.xDestination][destination.yDestination] << " at column: " << destination.xDestination << ", row: " << destination.yDestination ;
    


    Solution solution(theGrid, robot, destination);
    solution.solve();
    solution.printMap(grid);



    return 0;
}
