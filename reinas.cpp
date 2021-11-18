#include "omp.h"
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <map>
#include <tuple>

#include "Coord.h"
#include "Dot.h"

using namespace std;

int n;
int n_solutions = 0;
int operation = 1;
string result = "";

std::ofstream output("solutions.txt");

int between(int x,int a,int b){
    return (x >= a && x<= b);
}

void process(string task,int& mycounter,string& myssol,string* s){

    if ((myssol != "" && operation == 0) || (result != "" && operation == 0))
        return;

    if (task.size() == n){
        for (int i = 0 ; i < task.length() ; i++)
            myssol += s[int(task[i])-97];
        if ( operation == 0 ){
            std::vector<Coord> solution;
            for (int i = 0 ; i < task.length() ; i++)
                solution.push_back(Coord(int(task[i])-97,0));
            queensFile("graph.dot",solution,n);
        }
        myssol += '\n';
        mycounter++;
        /*if ( myssol.length() > 100000){
            #pragma omp critical
            {
                output.write(myssol.c_str(),myssol.length());
                myssol="";
            }
        }*/
    }
        
    string solutions;
    string genPath = task;

    int bloqued = 0;
    int counter = 1;
    int max = ( (1 << (n-1) ) | ((1 << (n-1) ) - 1) );

    for (int i = genPath.size()-1 ; i >= 0 ; i-- ){
        int pos = static_cast<int>(task[i]) - 96;
        int bit_repr = 1;
        bit_repr = bit_repr << (n - pos);
        bit_repr = bit_repr | (bit_repr << counter)  | (bit_repr >> counter);
        bit_repr = (bit_repr & max );
        bloqued = (bloqued | bit_repr);
        ++counter;
    }
    
    int limit = 1 << n;
    for (int i = 0 ; i < n ; i++ ){
        if ( !( (bloqued << (i+1) ) & limit )){
            process(genPath + string(1,97+i),mycounter,myssol,s);
        }
    }
    
    /*
    int arr[n] = {0};
    int counter = 1;



    for (int i = genPath.size()-1 ; i >= 0 ; i-- ){
        int valor = static_cast<int>(task[i]) - 97;
        arr[valor] = 1;
        if (between(valor+counter,0,n-1))
            arr[valor+counter] = 1;
        if (between(valor-counter,0,n-1))
            arr[valor-counter] = 1;
        ++counter;
    }
    
    for (int i = 0 ; i < n ; i++ ){
        if (arr[i] == 0){
            process(genPath + string(1,97+i),mycounter,myssol,s);
        }
    }
*/

}

void parallelrunRecursive(string* map){
    #pragma omp parallel for
    for(int i = 0 ; i < n ; i++ ){        
        int mycounter = 0;
        string segsol = "";
        process(string(1,char(97+i)),mycounter,segsol,map);
        

        if (operation == 0 ){
            n_solutions = 1;
            #pragma omp critical
            {
                if (result == "")
                    result += segsol;
            }
        }
        else{
            #pragma omp atomic
            n_solutions += mycounter;
            #pragma omp critical
            result += segsol;
        }
    }  
}

int main(int argc, char* argv[]){

    std::string beginFile = "---------------------------------------------------------------------\nBEGIN FILE solutions.txt\n___________________________________________\n";
    std::string endFile = "---------------------------------------------------------------------\nEND FILE solutions.txt\n___________________________________________";

    output.write(beginFile.c_str(),beginFile.length());
    
    
    if (argc != 5 )
        return 1;

    if (std::string(argv[2]) == "all")
        operation = 1;
    if (std::string(argv[2]) == "find")
        operation = 0;

    n = std::atoi(argv[4]);
    
    string arr[n];

    for (int i = 0 ; i < n ; i++ )
        arr[i] = to_string(i+1)+' ';

    parallelrunRecursive(arr);

    cout << "calculos terminados" << endl;

    std::string line =  std::to_string(n_solutions) + '\n' ;
    output.write(line.c_str(),line.length());
    output.write(result.c_str(),result.length());

    output.write(endFile.c_str(),endFile.length());
    output.close();
    
}