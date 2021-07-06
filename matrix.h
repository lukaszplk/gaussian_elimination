#include<string>
#include <iostream>
#include<iomanip>

class fractial{
    public:
    int nominator, denominator;
    void show(){
        if(this->nominator == 0){
                std::cout<<0<<' ';
            }else if(this->denominator == 1){
                std::cout<<this->nominator<<' ';
            }else if(this->denominator == this->nominator){
                std::cout<<1<<' ';
            }else if(this->nominator % this->denominator == 0){
                std::cout<<this->nominator / this->denominator<<' ';
            }else if(-this->denominator == this->nominator || this->denominator == -this->nominator){
                std::cout<<-1<<' ';
            }else if(this->nominator > this->denominator){
                std::cout<<
                    this->nominator / this->denominator<<"("<<
                    this->nominator % this->denominator<<
                    '/'<<this->denominator<<") ";
            }else if(-this->nominator > this->denominator){
                std::cout<<
                    this->nominator / this->denominator<<"("<<
                    -this->nominator % this->denominator<<
                    '/'<<this->denominator<<") ";
            }else{
                std::cout<<this->nominator<<'/'<<this->denominator<<' ';
            }
    }
};

class Matrix
{   
    public:
    Matrix(int rows, int columns, float tab[]);
    Matrix(int rows, int columns); 
    void print() const;
    Matrix transpose();
    Matrix upper_triangular();
    Matrix unit_triangular();
    Matrix atomic_triangular();

    private:
    fractial * multiply_row(int, fractial);
    void swap_rows(int, int);
    void sum_rows(int, fractial tab[]);
    int find_non_zero_index_in_col(int col, int start);
    int find_zero_index_in_col(int, int);
    int find_non_zero_index_in_row(int);
    void short_frac(fractial *);
    
    int rows, columns;
    fractial **matrix;
};