

#ifndef DOT_H
#define DOT_H

#include "Coord.h"
#include <string>
#include <fstream>

std::string openDigraph(){
    return "digraph D { node [shape=plaintext] some_node [";
}

std::string closeDigraph(){
    return "]; }";
}

std::string openTable(int border = 0,int cellborder = 1,int cellspacing = 0){
    return "<table border=\"0\" cellborder=\"1\" cellspacing=\"0\">";
}

std::string closeTable(){
    return "</table>";
}

std::string openRow(){
    return "<tr>";
}

std::string closeRow(){
    return "</tr>";
}

std::string openColum(){
    return "<td>";
}

std::string closeColum(){
    return "</td>";
}

std::string queen(){
    return "&#9813;";
}

std::string openLabel(){
    return "<label>=<";
}

std::string closeLabel(){
    return ">";
}

std::string queensDot(std::vector<Coord> queens, int table_size){
    std::string content;
    content = openDigraph() + openLabel() + openTable() ;

    for (int i = 0 ; i < queens.size() ; i++){
        content += openRow();
        for( int j = 0 ; j < table_size ; j++ ){
            content += openColum();
            if (queens[i].x == j )
                content += queen();
            content += closeColum();
        }
        content += closeRow();
    }
    content += closeTable() + closeLabel() + closeDigraph();
    return content;
}

void queensFile(std::string filename,std::vector<Coord> queens, int table_size){
    std::ofstream output(filename);
    std::string content =  queensDot(queens,table_size);
    output.write( content.c_str(), content.length() );
    output.close();
}

#endif