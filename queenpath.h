
#ifndef QUEENPATH_H
#define QUEENPATH_H

#include "Coord.h"

class QueenPath{
public:
    QueenPath(){
    }

    QueenPath(std::vector<Coord> _queens){
        queens = _queens;
    }
    QueenPath(Coord pos){
        queens.push_back(pos);
    }

    void push_back(Coord pos){
        queens.push_back(pos);
    }

    std::vector<Coord> queens;  
};


#endif