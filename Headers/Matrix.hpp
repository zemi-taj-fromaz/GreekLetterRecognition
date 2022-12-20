#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <vector>
#include <string>
#include <numeric>


using namespace std;

class Matrix {
    private:
        int rows;
        int columns;
        vector<vector<double>> matrix;
        vector<vector<double>> A;
        vector<int> P;
    public:
        double epsilon = 1e-9;
        Matrix(int,int);
        Matrix(string);
        Matrix(vector<vector<double>>);
        ~Matrix() {};
        void toFile(string);
        void Print();

        void LU_Decompose();
        void LUP_Decompose();
        
        double get(int,int);
        Matrix* set(double,int,int);

        vector<vector<double>>& getMatrix();
        vector<vector<double>>& getDecompMatrix();
        
        Matrix operator+(Matrix&);
        Matrix operator-(Matrix&);
        Matrix operator*(Matrix);
        void operator*=(double);
        void operator+=(Matrix&);
        void operator-=(Matrix&);
        bool operator==(Matrix&);
        void operator=(Matrix&);

        void T();
        void size();

        Matrix* forward_substitution(Matrix b);
        Matrix* backward_substitution(Matrix x);
        Matrix* inverse();

        static void swap_rows(Matrix&,int,int);
        static void swap_columns(Matrix&,int,int);

        
        static double det(vector<vector<double>>);
        static vector<string> split(string,string);
        
};

#endif