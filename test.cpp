#include "omp.h"
#include <iostream>
#include <unistd.h>

using namespace std;

int arr[10] = {0};

void ga(int n){
        cout << omp_get_thread_num() << endl;
        if (n < 10){
            arr[omp_get_thread_num()] += 1;
            #pragma omp parallel sections
            {
                #pragma omp section
                {
                    ga(n+1);
                }
            }
        }
}

int main(){
    ga(5);


    for (int i = 0 ; i < 10 ; i++ )
        cout << arr[i] << " ";
    cout << endl;

}