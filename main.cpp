

#include <vector>
#include <math.h>
#include <mutex>
#include <stdlib.h>

#include "omp.h"
#include "Coord.h"
#include "Dot.h"

#include <iostream>

enum Type{
    all,
    find,
};

bool solved = false;
int solveCounter = 0;

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

void findSolution(int table_size,int row,std::vector<Coord> solution,Type type){
    if (solution.size() < table_size && row < table_size){
        if(solved)
            return;

        #pragma omp parallel for private(solution)
        for(int i = 0 ; i < table_size ; i++){
            if (isFree(table_size,solution,Coord(i,row))){
                std::vector<Coord> path;
                path = solution;
                path.push_back(Coord(i,row));
                findSolution(table_size,row+1,path,type);
            }
        }
    }else if (solution.size() == table_size){
        #pragma omp critical
        {
            if (type == all){
                solveCounter++;
                std::string line =  std::to_string(solveCounter) + " " + toString(solution) + '\n' ;
                output.write(line.c_str(),line.length());
            }
            if (type == find){
                if (!solved){
                    solveCounter++;
                    std::string line =  std::to_string(solveCounter) + " " + toString(solution) + '\n' ;
                    output.write(line.c_str(),line.length());
                    queensFile("graph.dot",solution,table_size);
                    solved = true;
                }
            }
        }
    }
}

void queens(Type type,int n_queens){
    std::vector<Coord> rootsol;
    findSolution(n_queens,0,rootsol,type);
}

int main(int argc, char* argv[]) {
    std::string beginFile = "---------------------------------------------------------------------\nBEGIN FILE solutions.txt\n___________________________________________\n";
    std::string endFile = "---------------------------------------------------------------------\nEND FILE solutions.txt\n___________________________________________";

    std::cout << omp_get_max_threads() << std::endl;
    output.write(beginFile.c_str(),beginFile.length());

    if (argc != 5 )
        return 1;

    if (std::string(argv[2]) == "all")
        queens(all,std::atoi(argv[4]));
    if (std::string(argv[2]) == "find")
        queens(find,std::atoi(argv[4]));

    output.write(endFile.c_str(),endFile.length());
    output.close();
}
