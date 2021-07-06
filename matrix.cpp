#include "matrix.h"


Matrix::Matrix(int rows, int columns, float tab[]){
    
    this->rows = rows;
    this->columns = columns;
        
    this->matrix = new fractial * [rows];
    for (int i=0; i<rows; i++){
        matrix[i] = new fractial[columns];
    }
    
    for(int i=0;i<this->rows;i++){
        for(int j=0;j<this->columns;j++){
            this->matrix[i][j].nominator = tab[i*columns + j];
            this->matrix[i][j].denominator = 1;
        }
    }
    
}

Matrix::Matrix(int rows, int columns){
    this->rows = rows;
    this->columns = columns;
        
    this->matrix = new fractial * [rows];
    for (int i=0; i<rows; i++){
        matrix[i] = new fractial[columns];
    }
    
    for(int i=0;i<this->rows;i++){
        for(int j=0;j<this->columns;j++){
            this->matrix[i][j].nominator = 0;
            this->matrix[i][j].denominator = 1;
        }
    }
    
}

void Matrix::print() const{
    for(int i=0;i<this->rows;i++){
        for(int j=0;j<this->columns;j++){
            std::cout<<std::setw(10);
            this->matrix[i][j].show();    
        }
        std::cout<<std::endl;
    }
}

int Matrix::find_non_zero_index_in_col(int col, int start = 0){
    #if defined(debug_all) || defined(debug_find_non_zero_in_col)
        std::cout<<"Start find non zero in col function."<<std::endl;
    #endif
    int index = -1;
    for (int i=start; i<this->rows;i++){
        if(this->matrix[i][col].nominator != 0 ){
            index = i;
            break;
        } 
    }
    #if defined(debug_all) || defined(debug_find_non_zero_in_col)
        std::cout<<"Non zero value in col at index "<<index<<std::endl;
        std::cout<<"End of find non zero in col function."<<std::endl<<std::endl;
    #endif
    return index;
    
}

int Matrix::find_zero_index_in_col(int col, int start = 0){
    int index = -1;
    #if defined(debug_all) || defined(debug_find_zero_in_col)
        std::cout<<"Start find zero in col function."<<std::endl;
    #endif
    for (int i=start; i<this->rows;i++){
        if(this->matrix[i][col].nominator == 0 ){
            index = i;
            break;
        } 
    }
    #if defined(debug_all) || defined(debug_find_zero_in_col)
        std::cout<<"Zero value in col at index "<<index<<std::endl;
        std::cout<<"End of find zero in col function."<<std::endl<<std::endl;
    #endif
    return index;
}

Matrix Matrix::upper_triangular(){
    #if defined(debug_all) || defined(debug_upper)
        std::cout<<"Start upper triangular function"<<std::endl;
    #endif
    Matrix *T = new Matrix(this->rows, this->columns);
    fractial mul;
    int index;

    for(int i=0;i<this->rows;i++){
        for(int j=0;j<this->columns;j++){
            T->matrix[i][j] = this->matrix[i][j];
        }
    }

    bool prev_row = false;
    for (int i=0; i<T->columns;i++){
        if(T->find_non_zero_index_in_col(i,i) >= 0){
            if(T->find_non_zero_index_in_col(i,i)>i){
                T->swap_rows(i, T->find_non_zero_index_in_col(i,i));
                std::cout<<"Swap row "<<i<<" with "<<T->find_non_zero_index_in_col(i,i)<<std::endl;       
            }

            if(prev_row){
                T->swap_rows(i, T->find_zero_index_in_col(i,i-1));
                std::cout<<"Swap row "<<i<<" with "<<T->find_non_zero_index_in_col(i,i-1)<<std::endl;
                prev_row = false;
            }
            
            for(int j=i+1;j<T->rows;j++){
                index = T->find_non_zero_index_in_col(i,j);
                if(index<0){
                    break;
                }
                
                mul.nominator = -T->matrix[index][i].nominator * T->matrix[i][i].denominator;
                mul.denominator = T->matrix[index][i].denominator * T->matrix[i][i].nominator;
                short_frac(&mul);
                std::cout<<"Summing "<<j<<" row and "<< i <<" row multiplied by ";
                mul.show();

                T->sum_rows(index, T->multiply_row(i,mul));

                std::cout<<std::endl;
                T->print();
                std::cout<<std::endl; 
            }
        }else{
            prev_row = true;
        }
    }
    return *T;
    delete T;
    #if defined(debug_all) || defined(debug_upper)
        std::cout<<"End of upper function"<<std::endl<<std::endl;
    #endif
}

void Matrix::sum_rows(int row, fractial secRow[]){
    #if defined(debug_all) || defined(debug_sumrows)
        std::cout<<"Start sum rows function."<<std::endl;
    #endif
    fractial *tab = new fractial[this->columns];
    #if defined(debug_all) || defined(debug_sumrows)
        std::cout<<"Before summing"<<std::endl;
        for(int i=0;i<this->columns;i++){
            std::cout<<this->matrix[row][i].nominator<<"/"<<this->matrix[row][i].denominator<<" ";
        }
        std::cout<<std::endl;
    #endif
    for(int i=0;i<this->columns;i++){
        tab[i].nominator = this->matrix[row][i].nominator * secRow[i].denominator
                                      + this->matrix[row][i].denominator * secRow[i].nominator;
        tab[i].denominator = this->matrix[row][i].denominator * secRow[i].denominator;
        
        short_frac(tab + i);
    }
    #if defined(debug_all) || defined(debug_sumrows)
        std::cout<<"After summing and cleaning"<<std::endl;
        for(int i=0;i<this->columns;i++){
            std::cout<<this->matrix[row][i].nominator<<"/"<<this->matrix[row][i].denominator<<" ";
        }
        std::cout<<std::endl;
    #endif

    for(int i=0;i<this->columns;i++){
        this->matrix[row][i] = tab[i];
    }
    #if defined(debug_all) || defined(debug_sumrows)
        std::cout<<"End of sum rows function."<<std::endl<<std::endl;
    #endif
    delete [] tab;
}

fractial * Matrix::multiply_row(int row, fractial num){
    fractial *tab = new fractial[this->columns];
    #if defined(debug_all) || defined(debug_mulrow)
        std::cout<<"Start multiply row function."<<std::endl;
        std::cout<<"Nominator "<<num.nominator<<" Denominator "<<num.denominator<<std::endl;
    #endif

    for(int i=0;i<this->columns;i++){
        if(this->matrix[row][i].nominator==0){
            #if defined(debug_all) || defined(debug_mulrow)
                std::cout<<"Skipping "<<this->matrix[row][i].nominator<<"/"<<this->matrix[row][i].denominator<<std::endl;
            #endif
            tab[i].nominator = this->matrix[row][i].nominator;
            tab[i].denominator = this->matrix[row][i].denominator;
            continue;
        }
        #if defined(debug_all) || defined(debug_mulrow)
            std::cout<<"Multipling "<<this->matrix[row][i].nominator<<"/"<<this->matrix[row][i].denominator<<" and "
            <<num.nominator<<"/"<<num.denominator<<std::endl;
        #endif
        tab[i].nominator = this->matrix[row][i].nominator * num.nominator;
        tab[i].denominator = this->matrix[row][i].denominator * num.denominator;
    }

    #if defined(debug_all) || defined(debug_mulrow)
        std::cout<<"Multiplied row: "<<row<<std::endl;
        std::cout<<"Row: "<<row<<"     ";
        for(int i=0;i<this->columns;i++){
            std::cout<<this->matrix[row][i].nominator<<"/"<<this->matrix[row][i].denominator<<" ";
        }
        std::cout<<"\nMutiplied: ";
        for(int i=0;i<this->columns;i++){
            std::cout<<tab[i].nominator<<"/"<<tab[i].denominator<<" ";
        }
        std::cout<<std::endl;
        std::cout<<"End of multiply function."<<std::endl<<std::endl;
    #endif

    return tab;
    delete [] tab;    
}

void Matrix::swap_rows(int row, int secRow){
    fractial *tab = new fractial[this->columns];
    #if defined(debug_all) || defined(debug_swaprows)
        std::cout<<"In swap rows."<<std::endl;
    #endif
    for(int i=0;i<this->columns;i++){
        tab[i] = this->matrix[row][i];
    }
    for(int i=0;i<this->columns;i++){
        this->matrix[row][i] = this->matrix[secRow][i];
    }
    for(int i=0;i<this->columns;i++){
        this->matrix[secRow][i] = tab[i];
    }
    delete [] tab;
    #if defined(debug_all) || defined(debug_swaprows)
        std::cout<<"End of swap row function."<<std::endl<<std::endl;
    #endif
}

Matrix Matrix::unit_triangular(){
    #if defined(debug_all) || defined(debug_unit)
        std::cout<<"In unit triangular function."<<std::endl;
    #endif
    Matrix *T = new Matrix(this->rows, this->columns);
    for(int i=0;i<this->rows;i++){
        for(int j=0;j<this->columns;j++){
            T->matrix[i][j] = this->matrix[i][j];
        }
    }
    fractial divider;
    for(int i=0;i<this->rows;i++){
        divider = T->matrix[i][T->find_non_zero_index_in_row(i)];
        #if defined(debug_all) || defined(debug_unit)
            std::cout<<"Divider: "<<divider.nominator<<"/"<<divider.denominator<<std::endl;
            std::cout<<"Before calculations"<<std::endl;
            T->print();
            std::cout<<std::endl;
        #endif 
        std::cout<<"Uniting row "<<i<<std::endl;
        std::cout<<"Divider ";
        divider.show();
        std::cout<<std::endl;
        for(int j=0;j<this->columns;j++){
            if(T->matrix[i][j].nominator == 0){
                continue;
            }
            
            T->matrix[i][j].nominator *= divider.denominator; 
            T->matrix[i][j].denominator *= divider.nominator;       
        }
        
        #if defined(debug_all) || defined(debug_unit)
            std::cout<<"After calculations"<<std::endl;
        #endif
        T->print();
    }
    #if defined(debug_all) || defined(debug_unit)
        std::cout<<"End of unit function"<<std::endl<<std::endl;
    #endif
    
    return *T;
    delete T;
}

int Matrix::find_non_zero_index_in_row(int row){
    #if defined(debug_all) || defined(debug_find_non_zero_in_row)
        std::cout<<"In find non zero in row function."<<std::endl;
    #endif
    int index = -1;
    for (int i=0; i<this->columns;i++){
        if(this->matrix[row][i].nominator != 0 ){
            index = i;
            break;
        } 
    }
    #if defined(debug_all) || defined(debug_find_non_zero_in_row)
        std::cout<<"Non zero value in row at index "<<index<<std::endl;
        std::cout<<"End of find non zero in row function."<<std::endl<<std::endl;
    #endif
    return index; 
}

void Matrix::short_frac(fractial * frac){
    #if defined(debug_all) || defined(debug_short_frac)
        std::cout<<"Start short frac function."<<std::endl;
    #endif
    if(frac->nominator==0){
        frac->denominator = 1;
        #if defined(debug_all) || defined(debug_short_frac)
            std::cout<<"Nominator = 0 -> denominator "<<frac->denominator<<std::endl;
            std::cout<<"End short frac function"<<std::endl<<std::endl;
        #endif
        return;
    }
    int a, b;
    if(frac->nominator < 0){
        a = -frac->nominator;
    }else{
        a = frac->nominator;
    }
    

    if(frac->denominator < 0){
        b = -frac->denominator;
    }else{
        b = frac->denominator;
    }
    

    #if defined(debug_all) || defined(debug_short_frac)
        std::cout<<"Before NWD"<<std::endl;
        std::cout<<"Frac "<<a<<"/"<<b<<std::endl;
    #endif
    while (a != b){
        if (a < b)
        b -= a;
        else
        a -= b;
    }
    #if defined(debug_all) || defined(debug_short_frac)
        std::cout<<"After NWD.\nNWD = "<<a<<std::endl;
        std::cout<<"Before shortening "<<frac->nominator<<"/"<<frac->denominator<<std::endl;
    #endif
    frac->nominator /= a;
    frac->denominator /= a;
    #if defined(debug_all) || defined(debug_short_frac)
        std::cout<<"After shortening "<<frac->nominator<<"/"<<frac->denominator<<std::endl;
        std::cout<<"End of short frac function."<<std::endl<<std::endl;
    #endif
}


Matrix Matrix::atomic_triangular(){
    #if defined(debug_all) || defined(debug_atomic)
        std::cout<<"Start atomic function."<<std::endl;
    #endif
    Matrix *T = new Matrix(this->rows, this->columns);
    for(int i=0;i<this->rows;i++){
        for(int j=0;j<this->columns;j++){
            T->matrix[i][j] = this->matrix[i][j];
        }
    }
    int index;
    fractial mul;
    for (int i=T->rows-1; i>=0;i--){
        index = T->find_non_zero_index_in_row(i);
        if(index < 0){
            continue;
        }
        #if defined(debug_all) || defined(debug_atomic)
            std::cout<<"Non zero value at index "<<index<<std::endl;
        #endif
        for(int j=i-1;j>=0;j--){
            mul.nominator = -T->matrix[j][index].nominator * T->matrix[i][index].denominator;
            mul.denominator = T->matrix[j][index].denominator * T->matrix[i][index].nominator;
            #if defined(debug_all) || defined(debug_atomic)
                std::cout<<"Divider "<<mul.nominator<<"/"<<mul.denominator<<std::endl;
            #endif
            T->sum_rows(j,T->multiply_row(i,mul));
            short_frac(&mul);
            std::cout<<"Summing "<<j<<" row and "<< i <<" row multiplied by ";
            mul.show();
            
            std::cout<<std::endl;
            T->print();
            std::cout<<std::endl;
        }
        
    }
    #if defined(debug_all) || defined(debug_atomic)
        std::cout<<"End of atomic function"<<std::endl<<std::endl;
    #endif
    return *T;
    delete T;
}