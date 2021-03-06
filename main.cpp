#include<iostream>
#include<vector>
#include<string>
#include<stack>
#include<sstream>
#include <algorithm>

using namespace std;

vector<vector<int>> serialConway(int rows, int cols, int generations, vector<vector<int>>currBS){
    vector<vector<int>>currBoardState=currBS;
    vector<vector<int>>nextBoardState(rows,vector<int>(cols));
    int currNeighbourCount;

    cout<<endl;
    for(int gen=0;gen<generations;gen++){
        for (auto &i : nextBoardState){
            fill(i.begin(), i.end(), 0);
        }

        for(int row=0;row<rows;row++){
            for(int col=0;col<cols;col++){
                //Inistialize neighbour count
                currNeighbourCount=0;
                //Top left
                if(row>=1&&col>=1){
                    currNeighbourCount+=currBoardState[row-1][col-1];
                }
                //Top middle
                if(row>=1){
                    currNeighbourCount+=currBoardState[row-1][col];
                }
                //Top right
                if(row>=1&&col<=cols-2){
                    currNeighbourCount+=currBoardState[row-1][col+1];
                }
                //Middle left
                if(col>=1){
                    currNeighbourCount+=currBoardState[row][col-1];
                }
                //Middle right
                if(col<=cols-2){
                    currNeighbourCount+=currBoardState[row][col+1];
                }
                //Bottom left
                if(row<=rows-2&&col>=1){
                    currNeighbourCount+=currBoardState[row+1][col-1];
                }
                //Bottom middle
                if(row<=rows-2){
                    currNeighbourCount+=currBoardState[row+1][col];
                }
                //Bottom right
                if(row<=rows-2&&col<=cols-2){
                    currNeighbourCount+=currBoardState[row+1][col+1];
                }

                if(currBoardState[row][col]==1){
                    if(currNeighbourCount<2||currNeighbourCount>3){
                        nextBoardState[row][col]=0;
                    }else{
                        nextBoardState[row][col]=currBoardState[row][col];
                    }
                }else if(currNeighbourCount==3){
                    nextBoardState[row][col]=1;
                }
                else{
                    nextBoardState[row][col]=currBoardState[row][col];
                }
            }
        }

        currBoardState=nextBoardState;
    }

    return currBoardState;
}
int main(){
    int rows=4;
    int cols=4;
    int generations=2;
    int state;

    string line;
    stringstream str;

    vector<vector<int>>currBoardState(rows,vector<int>(cols));
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
            currBoardState[r][c]=state;
            str.clear();

        }
    }

    currBoardState=serialConway(rows,cols,generations,currBoardState);

         cout<<endl;
        //Display the next board state
        for(int r=0;r<rows;r++){
            for(int c=0;c<cols;c++){
                cout<<currBoardState[r][c];
            }
            cout<<endl;
        }

    return 0;
}
