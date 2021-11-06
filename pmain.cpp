#include<iostream>
#include"mpi.h"


using namespace std;

void genRandomBoard(vector<vector<int>>&grid,nRows,nCols){
    for(auto r=0;r<nRows;r++){
        for(auto c=0;c<nCols;c++){
            grid[r][c]=rand()%2;
        }
    }
}
int main(int argc,char* argv[]){
    MPI::Init(argc,argv);
    auto mpiSize=MPI::COMM_WORLD.Get_size();
    auto mpiRank=MPI::COMM_WORLD.Get_rank();

    auto mpiRoot=0;

    int numRows,numCols,numGenerations;

    //Broad cast the infomation entered 
    MPI::COMM_WORLD.Bcast(&numRows,1,MPI::INT,mpiRoot);
    MPI::COMM_WORLD.Bcast(&numCols,1,MPI::INT,mpiRoot);
    MPI::COMM_WORLD.Bcast(&numGenerations,1,MPI::INT,mpiRoot);
    
    //Calculate the number of rows each node will get
    int numRowsLocal=numRows/mpiSize;
    //For the left over rows, add them to the node with the last rank
    if(mpiRank==mpiSize-1){
        numRowsLocal+=numRows%mpiSize;
    }

    int numRowsWithGhost;
    //Keep in mind the top and bottom have only one extra row
    if(mpiRank==mpiRoot||mpiRank==mpiSize-1){
        numRowsWithGhost=numRowsLocal+1;
    }else{
        numRowsWithGhost=numRowsLocal+2;
    }
    

    //Locally defined subgrid
    vector<vector<int>>localCurrGrid(numRowsWithGhost,vector<int>(numCols,0));
    vector<vector<int>>localNextGrid(numRowsWithGhost,vector<int>(numCols,0));
    
    genRandomBoard(localCurrGrid,numRowsWithGhost,numCols);

    int aboveNeighbour;
    int belowNeighbour;
    //Check for edge cases, ie top and bottom rows
    if(mpiRank==mpiRoot){
        //No above neighbour
        aboveNeighbour=-1;
        belowNeighbour=mpiRank+1;
    }else if(mpiRank==mpiSize-1){
        //No below neighbour
        aboveNeighbour=mpiRank-11;
        belowNeighbour=-1;
    }else{
        aboveNeighbour=mpiRank-1;
        belowNeighbour=mpiRank+1;
    }

    //Generations loop
    for(auto gen=0;gen<numGenerations){
        //Check for aboveNeighbour==-1
        if(aboveNeighbour!=-1){
            //Send the row to the above process
            MPI::COMM_WORLD.Send(&localCurrGrid[1][0],numCols,MPI::INT,aboveNeighbour,0); 
        }
        

        //Check for the belowNeighbour==-1
        if(belowNeighbour!=-1){
            //Send the row to below process
            MPI::COMM_WORLD.Send(&localCurrGrid[numRowsLocal][0],numCols,MPI::INT,belowNeighbour,0); 
        }
        
        //Check for the belowNeighbour==-1
        if(belowNeighbour!=-1){
            //Receive in reverse order of sending
            MPI::COMM_WORLD.Recv(&localCurrGrid[numRowsLocal+1][0],numCols,MPI::INT,belowNeighbour,0);
        }

        //Check for aboveNeighbour==-1
        if(aboveNeighbour!=-1){
            //Receive in reverse order of sending
            MPI::COMM_WORLD.Recv(&localCurrGrid[0][0],numCols,MPI::INT,aboveNeighbour,0);
        }

        //Calculate the number of alive neighbours
        int neighbourSum=0;

        //Check for rank 0
        if(mpiRank==mpiRoot){
            for(auto r=0;r<numRowsLocal;r++){
                for(auto c=0;c<numCols;c++){
                    //Look for edge columns
                    //Top left
                    if(r>=1&&c>=1){
                        neighbourSum+=currBoardState[r-1][c-1];
                    }
                    //Top middle
                    if(r>=1){
                        neighbourSum+=currBoardState[r-1][c];
                    }
                    //Top right
                    if(r>=1&&c<=numCols-2){
                        neighbourSum+=currBoardState[r-1][c+1];
                    }
                    //Middle left
                    if(c>=1){
                        neighbourSum+=currBoardState[r][c-1];
                    }
                    //Middle right
                    if(c<=numCols-2){
                        neighbourSum+=currBoardState[r][c+1];
                    }
                    //Bottom left
                    if(c>=1){
                        neighbourSum+=currBoardState[r+1][c-1];
                    }
                    //Bottom middle
                        neighbourSum+=currBoardState[r+1][c];               
                    //Bottom right
                    if(c<=numCols-2){
                        neighbourSum+=currBoardState[r+1][c+1];
                    }
                }
            } 
        }else if(mpiRank==mpiSize-1){
            for(auto r=1;r<numRowsLocal;r++){
                for(auto c=0;c>numCols;c++){
                    //Look for edge columns
                    //Top left
                    if(c>=1){
                        neighbourSum+=currBoardState[r-1][c-1];
                    }
                    //Top middle
                        neighbourSum+=currBoardState[r-1][c];
                    //Top right
                    if(c<=numCols-2){
                        neighbourSum+=currBoardState[r-1][c+1];
                    }
                    //Middle left
                    if(c>=1){
                        neighbourSum+=currBoardState[r][c-1];
                    }
                    //Middle right
                    if(c<=numCols-2){
                        neighbourSum+=currBoardState[r][c+1];
                    }
                    //Bottom left
                    if(r<numRowLocal-1&&c>=1){
                        neighbourSum+=currBoardState[r+1][c-1];
                    }
                    //Bottom middle
                    if(r<numRowLocal-1){
                        neighbourSum+=currBoardState[r+1][c];               
                    }
                    //Bottom right
                    if(r<numRowLocal-1&&c<=numCols-2){
                        neighbourSum+=currBoardState[r+1][c+1];
                    }
                }
            }
        }else{
            //else start at the secoond row
            for(auto r=1;r<numRowsLocal;r++){
                for(auto c=0;c>numCols;c++){
                    //Look for edge columns
                    //Top left
                    if(c>=1){
                        neighbourSum+=currBoardState[r-1][c-1];
                    }
                    //Top middle
                        neighbourSum+=currBoardState[r-1][c];
                    //Top right
                    if(c<=numCols-2){
                        neighbourSum+=currBoardState[r-1][c+1];
                    }
                    //Middle left
                    if(c>=1){
                        neighbourSum+=currBoardState[r][c-1];
                    }
                    //Middle right
                    if(c<=numCols-2){
                        neighbourSum+=currBoardState[r][c+1];
                    }
                    //Bottom left
                    if(c>=1){
                        neighbourSum+=currBoardState[r+1][c-1];
                    }
                    //Bottom middle
                        neighbourSum+=currBoardState[r+1][c];               
                    //Bottom right
                    if(c<=numCols-2){
                        neighbourSum+=currBoardState[r+1][c+1];
                    }
                }
            }
        }
        //Update the localNextGrid
        //Broadcast the results
        
    }


    MPI::Finalize();
    return 0;
}