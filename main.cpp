#include<iostream>
#include<vector>
#include<string>
#include<stack> 
#include<sstream>

using namespace std;

class Cell{
    public:
        int rowLocation;
        int colLocation;
        int neighbourCount;
        int state; 
        bool stateChange;

        Cell(int r,int c,int s){
            rowLocation=r;
            colLocation=c;
            neighbourCount=0;
            state=s;
            stateChange=false;
        }   
};

int main(){
    int rows=4;
    int cols=4;
    int generations=5;
    int state;
    int currNeighbourCount;
    string line;
    stringstream str;
    stack<Cell*>AliveCells;
    stack<Cell*>neighbours;
    vector<vector<Cell>>currBoardState(rows);
    vector<vector<Cell>>nextBoardState;
    vector<string>vecLines;

    //Take in input for the current board state
    vecLines.resize(rows);
    // vecMovesToMake.resize(stoi(numInput));
    for(int k=0;k<rows;k++){
        getline(cin,line);
        vecLines[k]=line;
        
    }

    for(int r=0;r<rows;r++){
        for(int c=0;c<cols;c++){
            str<<vecLines[r][c];
            str>>state;
            currBoardState[r].push_back(Cell(r,c,state));
            str.clear();

        }
    }

    //print the board
    for(int r=0;r<rows;r++){
        for(int c=0;c<cols;c++){
            cout<<currBoardState[r][c].state<<" ";
        }
        cout<<endl;
    }
    //TODO: Loop for going throught the alive cells
    //Go through the entire board
    for(int row=0;row<rows;row++){
        for(int col=0;col<cols;col++){
            Cell *currCell=&currBoardState[row][col];
        //Neighbour checker
        //TODO: checks for edge of board neighbours
        //TODO: Actually update the next board state
            //Top-left
            currCell->neighbourCount+=currBoardState[row-1][col-1].state;
            neighbours.push(&currBoardState[row-1][col-1]);
            //Top middle
            currCell->neighbourCount+=currBoardState[row-1][col].state;
            neighbours.push(&currBoardState[row-1][col]);
            //Top right
            currCell->neighbourCount+=currBoardState[row-1][col+1].state;
            neighbours.push(&currBoardState[row-1][col+1]);
            //Middle left
            currCell->neighbourCount+=currBoardState[row][col-1].state;
            neighbours.push(&currBoardState[row][col-1]);
            //Middle right
            currCell->neighbourCount+=currBoardState[row-1][col+1].state;
            neighbours.push(&currBoardState[row-1][col+1]);
            //Bottom left
            currCell->neighbourCount+=currBoardState[row+1][col-1].state;
            neighbours.push(&currBoardState[row+1][col-1]);
            //Bottom middle
            currCell->neighbourCount+=currBoardState[row+1][col].state;
            neighbours.push(&currBoardState[row+1][col]);
            //Bottom right
            currCell->neighbourCount+=currBoardState[row+1][col+1].state;
            neighbours.push(&currBoardState[row+1][col+1]);

            currNeighbourCount=currCell->neighbourCount;
            //Do the rule check
            if(currCell->state==1){
                if(currNeighbourCount<2||currNeighbourCount>3){
                    currCell->state=0;
                    currCell->stateChange=true;
                }
            }else if(currNeighbourCount==3){
                currCell->state=1;
                currCell->stateChange=true;
            }

            state=currCell->state;
            //If there is a state change, update the neighbours
            if(currCell->stateChange){
                //Go through all the neighbours and update them
                while(!neighbours.empty()){
                    if(state==1){
                        neighbours.top()->neighbourCount+=1;
                        neighbours.pop();
                    }else{
                        neighbours.top()->neighbourCount-=1;
                        neighbours.pop();
                    }
                }
            }
                //Else move on to the next cell
        }
    }
    //Generate the next board state
        

    //Display the next board state
    //Continue this loop for a few generations


    //Check
    return 0;
}
