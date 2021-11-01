#include<iostream>
#include<vector>
#include<string>

using namespace std;

int main(){
    int rows=4;
    int cols=4;
    int generations=5;
    string line;
    vector<string>vecLine;
    vector<vector<string>>currBoardState;
    vector<vector<string>>nextBoardState(rows,vector<string>(cols,"0"));
    vector<string>vecLines;

    //Take in input for the current board state
    vecLines.resize(rows);
    // vecMovesToMake.resize(stoi(numInput));
    for(int k=0;k<rows;k++){
        getline(cin,line);
        vecLines[k]=line;
        
    }

    for(int r=0;r<rows;r++){
        for(auto c:vecLines[r]){
            vecLine.push_back(string(1,c));
        }
        currBoardState.push_back(vecLine);
        vecLine.clear();
    }

    //print the board
    for(int r=0;r<rows;r++){
        for(int c=0;c<cols;c++){
            cout<<currBoardState[r][c]<<" ";
        }
        cout<<endl;
    }

    //Generate the next board state
     
    //Display the next board state
    //Continue this loop for a few generations

    return 0;
}
