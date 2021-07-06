#include <iostream>
#include <string>
#include "matrix.cpp"

int main(){
    int n = 3, m = 4;

    float tab[] = {1,2,-1,5,3,4,1,9,2,-2,3,-1};

    Matrix M(n,m,tab), G(n,m);
    M.print();
    std::cout<<"----------------------------------------------------------------------"<<std::endl;
    G = M.upper_triangular().unit_triangular().atomic_triangular();
    std::cout<<"----------------------------------------------------------------------"<<std::endl;
    G.print();
}