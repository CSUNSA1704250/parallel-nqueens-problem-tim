#ifndef COORD_H
#define COORD_H

#include <iostream>
#include <vector>

struct Coord{
    int x;
    int y;
    Coord(int _x,int _y){
        x = _x;
        y = _y;
    }
};

std::ostream& operator<<(std::ostream& o, std::vector<Coord> sol){
    for (int i = 0 ; i < sol.size() ; i++ ){
        o << sol[i].x + 1 << " "; 
    }
    return o;
}


std::string toString(std::vector<Coord> sol){
    std::string line;
    for (int i = 0 ; i < sol.size() ; i++ ){
        line += std::to_string(sol[i].x + 1) + " "; 
    }
    return line;
}


#endif