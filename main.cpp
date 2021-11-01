#include<iostream>
#include<vector>
#include<string>

using namespace std;

class Cell{
    public:
        int rowLocation;
        int colLocation;
        int neighbourCount;
        string state; 

        Cell(int r,int c,string s){
            rowLocation=r;
            colLocation=c;
            neighbourCount=0;
            state=s;
        }   
};

int main(){
    int rows=4;
    int cols=4;
    int generations=5;
    string line;
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
            currBoardState[r].push_back(Cell(r,c,string(1,vecLines[r][c])));
        }
    }

    //print the board
    for(int r=0;r<rows;r++){
        for(int c=0;c<cols;c++){
            cout<<currBoardState[r][c].state<<" ";
        }
        cout<<endl;
    }

    //Neighbour count

    //Generate the next board state
        //Neighbour checker
        //Top-left
        //Top middle
        //Top right
        //Middle left
        //Middle right
        //Bottom left
        //Bottom middle
        //Bottom right

    //Display the next board state
    //Continue this loop for a few generations

    return 0;
}
