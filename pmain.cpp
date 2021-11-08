#include<iostream>
#include<vector>
#include<mpi.h>


using namespace std;

void genRandomBoard(vector<vector<int>>&grid,int myRank,int mpiSize,int nRows,int nCols){

    if(myRank!=0){
        for(auto r=1;r<=nRows;r++){
            for(auto c=0;c<nCols;c++){
                grid[r][c]=rand()%2;//FIXME: Make the seed change
                
            }
        }
    }else if(myRank==0){
        for(auto r=0;r<nRows-1;r++){
            for(auto c=0;c<nCols;c++){
                grid[r][c]=rand()%2;
            }
        }
    }
}

int main(int argc,char* argv[]){
    MPI_Init(&argc,&argv);
    int mpiSize,mpiRank;
    MPI_Comm_size(MPI_COMM_WORLD,&mpiSize);
    MPI_Comm_rank(MPI_COMM_WORLD,&mpiRank);

    auto mpiRoot=0;

    int numRows,numCols,numGenerations;
    if(mpiRank==mpiRoot){
        if(argc!=4){
            cout<<"4 arguments required"<<endl;
            exit(1);
        }
    }
    numRows = atoi(argv[1]);
    numCols = atoi(argv[2]);
    numGenerations = atoi(argv[3]);

    //Broad cast the infomation entered 
    MPI_Bcast(&numRows,1,MPI_INT,mpiRoot,MPI_COMM_WORLD);
    MPI_Bcast(&numCols,1,MPI_INT,mpiRoot,MPI_COMM_WORLD);
    MPI_Bcast(&numGenerations,1,MPI_INT,mpiRoot,MPI_COMM_WORLD);
    
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
    
    genRandomBoard(localCurrGrid,mpiRank,mpiSize,numRowsLocal,numCols);

    int aboveNeighbour;
    int belowNeighbour;
    //Check for edge cases, ie top and bottom rows
    if(mpiRank==mpiRoot){
        //No above neighbour
        aboveNeighbour=-1;
        belowNeighbour=mpiRank+1;
    }else if(mpiRank==mpiSize-1){
        //No below neighbour
        aboveNeighbour=mpiRank-1;
        belowNeighbour=-1;
    }else{
        aboveNeighbour=mpiRank-1;
        belowNeighbour=mpiRank+1;
    }

    //Generations loop
    for(auto gen=0;gen<numGenerations;gen++){
        //Check not rank 0
        if(mpiRank!=mpiRoot){
            //Send the row to the above process
            MPI_Send(&localCurrGrid[1][0],numCols,MPI_INT,aboveNeighbour,0,MPI_COMM_WORLD); 
        }
        
        //Check not last rank
        if(mpiRank==mpiRoot){
            //Send the row to below process
            MPI_Send(&localCurrGrid[numRowsLocal-1][0],numCols,MPI_INT,belowNeighbour,0,MPI_COMM_WORLD); 
        }else if(mpiRank!=mpiSize-1){
            //Send the row to below process
            MPI_Send(&localCurrGrid[numRowsLocal][0],numCols,MPI_INT,belowNeighbour,0,MPI_COMM_WORLD); 
        }
        
        //Check for last rank
        if(mpiRank!=mpiSize-1)
        {
            //Receive in from below process
            if(mpiRank!=mpiRoot){
                MPI_Recv(&localCurrGrid[numRowsLocal+1][0],numCols,MPI_INT,belowNeighbour,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            }else{
                MPI_Recv(&localCurrGrid[numRowsLocal][0],numCols,MPI_INT,belowNeighbour,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            }
        }


        //Check for rank 0
        if(mpiRank!=mpiRoot){
            //Receive from above process
            MPI_Recv(&localCurrGrid[0][0],numCols,MPI_INT,aboveNeighbour,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        }

        // cout<<"Local current grid for rank: "<<mpiRank<<endl;
        // for(int r=0;r<numRowsWithGhost;r++){
        //     for(int c=0;c<numCols;c++){
        //         cout<<localCurrGrid[r][c]<<" ";
        //     }
        //     cout<<endl;
        // }

        //Display the grid
        if(mpiRank!=mpiRoot){//FIXME: Uncomment to print

            for(int row=1;row<=numRowsLocal;row++){
                MPI_Send(&localCurrGrid[row][0],numCols,MPI_INT,mpiRoot,0,MPI_COMM_WORLD);
            }
        }else{
            //Print local grid
            cout<<"Generation: "<<gen<<endl;
            for(int row=0;row<numRowsLocal;row++){
                for(int col=0;col<numCols;col++){
                    cout<<localCurrGrid[row][col]<<" ";
                }
                cout<<endl;
            }
            //Print received grids
            for(int otherRanks=1;otherRanks<mpiSize;otherRanks++){
                auto numReceived=numRows/mpiSize;//Check this
                // cout<<"Num received: "<<numReceived<<" from rank "<<otherRanks<<endl;
                if(otherRanks==mpiSize-1){
                    numReceived+=numRows%mpiSize;
                }
                //Vector to save the input
                vector<int>vecTemp(numCols,0);
                //Perform a loop for the receives
                for(int i=0;i<numReceived;i++){
                    MPI_Recv(&vecTemp[0],numCols,MPI_INT,otherRanks,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                    for(auto x:vecTemp){
                        cout<<x<<" ";
                    }
                    cout<<endl;
                }
            }
        }
       

        //Check for rank 0
        if(mpiRank==mpiRoot){
            for(auto r=0;r<numRowsLocal;r++){
                for(auto c=0;c<numCols;c++){
                    int neighbourSum=0;
                    //Look for edge columns
                    //Top left
                    if(r>=1&&c>=1){
                        neighbourSum+=localCurrGrid[r-1][c-1];
                    }
                    //Top middle
                    if(r>=1){
                        neighbourSum+=localCurrGrid[r-1][c];
                    }
                    //Top right
                    if(r>=1&&c<=numCols-2){
                        neighbourSum+=localCurrGrid[r-1][c+1];
                    }
                    //Middle left
                    if(c>=1){
                        neighbourSum+=localCurrGrid[r][c-1];
                    }
                    //Middle right
                    if(c<=numCols-2){
                        neighbourSum+=localCurrGrid[r][c+1];
                    }
                    //Bottom left
                    if(c>=1){
                        neighbourSum+=localCurrGrid[r+1][c-1];
                    }
                    //Bottom middle
                    neighbourSum+=localCurrGrid[r+1][c];               
                    //Bottom right
                    if(c<=numCols-2){
                        neighbourSum+=localCurrGrid[r+1][c+1];
                    }
                    

                    //Update the localNextGrid
                    if(localCurrGrid[r][c]==1){
                        if(neighbourSum<2||neighbourSum>3){
                            localNextGrid[r][c]=0;
                        }else{
                            localNextGrid[r][c]=localCurrGrid[r][c];
                        }
                    }else if(neighbourSum==3){
                        localNextGrid[r][c]=1;
                        
                    }
                }
            } 
        }else if(mpiRank==mpiSize-1){
            for(auto r=1;r<=numRowsLocal;r++){
                for(auto c=0;c<numCols;c++){
                    int neighbourSum=0;
                    //Look for edge columns
                    //Top left
                    if(c>=1){
                        neighbourSum+=localCurrGrid[r-1][c-1];
                    }
                    //Top middle
                        neighbourSum+=localCurrGrid[r-1][c];
                    //Top right
                    if(c<=numCols-2){
                        neighbourSum+=localCurrGrid[r-1][c+1];
                    }
                    //Middle left
                    if(c>=1){
                        neighbourSum+=localCurrGrid[r][c-1];
                    }
                    //Middle right
                    if(c<=numCols-2){
                        neighbourSum+=localCurrGrid[r][c+1];
                    }
                    //Bottom left
                    if(r<numRowsLocal&&c>=1){
                        neighbourSum+=localCurrGrid[r+1][c-1];
                    }
                    //Bottom middle
                    if(r<numRowsLocal){
                        neighbourSum+=localCurrGrid[r+1][c];               
                    }
                    //Bottom right
                    if(r<numRowsLocal&&c<=numCols-2){
                        neighbourSum+=localCurrGrid[r+1][c+1];
                    }
                    // cout<<"Neighbour sum is: "<<neighbourSum<<" for row: "<<r<<" col: "<<c<<endl;
                    //Update the localNextGrid
                    if(localCurrGrid[r][c]==1){
                        if(neighbourSum<2||neighbourSum>3){
                            localNextGrid[r][c]=0;
                        }else{
                            localNextGrid[r][c]=localCurrGrid[r][c];
                        }
                    }else if(neighbourSum==3){
                        localNextGrid[r][c]=1;
                    }
                }
            }
        }else{
            //else start at the secoond row
            for(auto r=1;r<=numRowsLocal;r++){
                for(auto c=0;c<numCols;c++){
                    int neighbourSum=0;
                    //Look for edge columns
                    //Top left
                    if(c>=1){
                        neighbourSum+=localCurrGrid[r-1][c-1];
                    }
                    //Top middle
                        neighbourSum+=localCurrGrid[r-1][c];
                    //Top right
                    if(c<=numCols-2){
                        neighbourSum+=localCurrGrid[r-1][c+1];
                    }
                    //Middle left
                    if(c>=1){
                        neighbourSum+=localCurrGrid[r][c-1];
                    }
                    //Middle right
                    if(c<=numCols-2){
                        neighbourSum+=localCurrGrid[r][c+1];
                    }
                    //Bottom left
                    if(c>=1){
                        neighbourSum+=localCurrGrid[r+1][c-1];
                    }
                    //Bottom middle
                        neighbourSum+=localCurrGrid[r+1][c];               
                    //Bottom right
                    if(c<=numCols-2){
                        neighbourSum+=localCurrGrid[r+1][c+1];
                    }

                    //Update the localNextGrid
                    if(localCurrGrid[r][c]==1){
                        if(neighbourSum<2||neighbourSum>3){
                            localNextGrid[r][c]=0;
                        }else{
                            localNextGrid[r][c]=localCurrGrid[r][c];
                        }
                    }else if(neighbourSum==3){
                        localNextGrid[r][c]=1;
                    }
                }
            }
        }

        //Copy next grid to current grid
        if(mpiRank!=0){
            for(auto r=1;r<=numRowsLocal;r++){
                for(auto c=0;c<numCols;c++){
                    localCurrGrid[r][c]=localNextGrid[r][c];
                }
               
            }
        }else if(mpiRank==0){
            for(auto r=0;r<numRowsLocal;r++){
                for(auto c=0;c<numCols;c++){
                    localCurrGrid[r][c]=localNextGrid[r][c];
                }
            }
        }
    }


    MPI_Finalize();
    return 0;
}