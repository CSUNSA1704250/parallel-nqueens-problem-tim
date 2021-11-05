
#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>

#include "omp.h"
#include "Coord.h"
#include "Dot.h"
#include "queenpath.h"

using namespace std;

vector < std::vector<QueenPath> > solutions;
vector < std::vector<QueenPath> > nextSol;

std::ofstream output("solutions.txt");

bool isFree(int table_size,std::vector<Coord> setted,Coord pos){
    for (int i = 0 ; i < setted.size() ; i++){
        // vertical
        if (pos.y == setted[i].y)
            return false;
        // horizontal
        if (pos.x == setted[i].x)
            return false;
        // diag
        if (abs(pos.x-setted[i].x) == abs(pos.y-setted[i].y) )
            return false;
    }
    return true;
}

void discoverPathfino(int table_size, vector<Coord> path, int level,int casilla){
    if (isFree(table_size,path,Coord(level+1,casilla))){
            vector<Coord> childPath;
            childPath = path;
            childPath.push_back(Coord(level+1,casilla));

            nextSol[path[0].y].push_back(QueenPath(childPath));
    }
}

void discoverPath(int table_size, vector<Coord> path, int level){
    //cout << std::this_thread::get_id() << endl;
    for ( int i = 0 ; i < table_size ; i++ ){
        if (isFree(table_size,path,Coord(level+1,i))){
            vector<Coord> childPath;
            childPath = path;
            childPath.push_back(Coord(level+1,i));
            nextSol[path[0].y].push_back(QueenPath(childPath));
        }
    }
}

/*

0 / 12 = 0      0
1/ 12    0      1
2 / 12   0      2
3 / 12    0        3

12 / 12  1        0
15 / 12    1       3


*/

void solve(int n){

    solutions = vector < std::vector<QueenPath> >(n);
    nextSol = vector < std::vector<QueenPath> >(n);

    for( int i = 0 ; i < n ; i++ )
        solutions[i].push_back(QueenPath(Coord(0,i)));

    for ( int i = 0 ; i < n-1 ; i++ ){

        #pragma omp parallel num_threads(n)
        {
            int th = omp_get_thread_num();
            for ( int j = 0 ; j < solutions[th].size() ; j++ )
            {
                //cout << solutions.size() << endl;
                //discoverPathfino(n,solutions[j/n].queens,i,j%n);
                discoverPath(n,solutions[th][j].queens,i);
            }
        }
        

        #pragma omp barrier

        solutions.clear();
        solutions = nextSol;
        nextSol.clear();

        nextSol = vector < std::vector<QueenPath> >(n);
    }
}

int main(int argc, char* argv[]){
    
    std::string beginFile = "---------------------------------------------------------------------\nBEGIN FILE solutions.txt\n___________________________________________\n";
    std::string endFile = "---------------------------------------------------------------------\nEND FILE solutions.txt\n___________________________________________";

    output.write(beginFile.c_str(),beginFile.length());

    if (argc != 5 )
        return 1;

    if (std::string(argv[2]) == "all")
        solve(std::atoi(argv[4]));
    if (std::string(argv[2]) == "find")
        solve(std::atoi(argv[4]));

    cout << "end "<< endl;

    int total = 0;

    for (int  i = 0 ; i < solutions.size() ; i++ ){
        total += solutions[i].size();
    }

    std::string line =  std::to_string(total) + '\n' ;
    output.write(line.c_str(),line.length());

    int counter = 0;
    for (int  i = 0 ; i < solutions.size() ; i++ ){
        for (int  j = 0 ; j < solutions[i].size() ; j++ ){
            std::string line =  std::to_string(counter+1) + " " + toString(solutions[i][j].queens) + '\n' ;
            output.write(line.c_str(),line.length());
            counter++;
        }
    }
                    

    output.write(endFile.c_str(),endFile.length());
    output.close();

    
    /*for (int  i = 0 ; i < solutions.size() ; i++ ){
        cout << solutions[i].queens << endl;
    }*/
}