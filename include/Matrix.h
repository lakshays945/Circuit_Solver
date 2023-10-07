#pragma once

#include<iostream>
#include<vector>
#include<math.h>

class Matrix {
public:
	std::vector<std::vector<long double>> Array;
	int Rows;
	int Columns;
	Matrix(){

	}
	Matrix(int rows, int columns, long double initial = 0){
		Rows = rows;
		Columns = columns;
		for(int i=0; i<rows; i++)
			Array.push_back(std::vector<long double>(columns,initial));
	}
	void SetArray(std::vector<std::vector<long double>> &array){
		for(int i=0; i<array.size(); i++){
			Array.push_back(array[i]);
		}
		Rows = array.size();
		Columns = array[0].size();
	}
	void RowOperation(int r1, int r2, long double mul1, long double mul2){ // r1 = mul1*r1 + mul2*r23
		if(r1 >= Rows || r2 >= Rows){
			std::cout<<"Invalid Row !!\n";
			return;
		}
		for(int i=0; i<Columns; i++){
			Array[r1][i] = mul1*Array[r1][i] + mul2*Array[r2][i];
		}
	}
	void SwapRow(int r1, int r2){
		if(r1 >= Rows || r2 >= Rows){
			std::cout<<"Invalid Row !!\n";
			return;
		}
		for(int i=0; i<Columns; i++){
			long double temp = Array[r1][i];
			Array[r1][i] = Array[r2][i];
			Array[r2][i] = temp;
		}
	}
	void ColumnOperation(int c1, int c2, long double mul1, long double mul2){ //c1 = mul1*c1 + mul2*c2
		if(c1 >= Columns || c2 >= Columns){
			std::cout<<"Invalid Column !!\n";
			return;
		}
		for(int i=0; i<Rows; i++){
			Array[i][c1] = mul1*Array[i][c1] + mul2*Array[i][c2];
		}
	}
	void SwapColumn(int c1, int c2){
		if(c1 >= Columns || c2 >= Columns){
			std::cout<<"Invalid Column !!\n";
			return;
		}
		for(int i=0; i<Rows; i++){
			long double temp = Array[i][c1];
			Array[i][c1] = Array[i][c2];
			Array[i][c2] = temp;
		}
	}
	void PrintMatrix(){
	    for(int i=0; i<Rows; i++){
	        for(int j=0; j<Columns; j++){
	            std::cout<<Array[i][j]<<' ';
	        }
	        std::cout<<'\n';
	    }
	    std::cout<<'\n';
	}
	Matrix operator+(Matrix& other){
		if(Rows != other.Rows || Columns != other.Columns){
			std::cout<<"Cant Add Differnt size matrix\n";
			return *this;
		}
		Matrix m = Matrix(Rows,Columns);
		for(int i=0; i<Rows; i++){
			for(int j=0; j<Columns; j++){
				m.Array[i][j] = Array[i][j] + other.Array[i][j];
			}
		}
		return m;
	}
	Matrix operator-(Matrix& other){
		if(Rows != other.Rows || Columns != other.Columns){
			std::cout<<"Cant Add Differnt size matrix\n";
			return *this;
		}
		Matrix m(Rows,Columns);
		for(int i=0; i<Rows; i++){
			for(int j=0; j<Columns; j++){
				m.Array[i][j] = Array[i][j] - other.Array[i][j];
			}
		}
		return m;
	}
	Matrix operator*(Matrix& other){
		if(Columns != other.Rows){
			std::cout<<"Cant Multiply Matrix\n";
			return *this;
		}
		Matrix m(Rows,other.Columns);
		for(int i=0; i<other.Columns; i++){
			for(int j=0; j<Rows; j++){
				for(int k=0; k<Columns; k++){
					m.Array[j][i] += Array[j][k] * other.Array[k][i];
				}
			}
		}
		return m;
	}
	void operator*(long double mul){
		for(int i=0; i<Rows; i++){
			for(int j=0; j<Columns; j++){
				Array[i][j] *= mul;
			}
		}
	}
	Matrix GetTranspose(){
		Matrix m(Columns,Rows);
		for(int i=0; i<Rows; i++){
			for(int j=0; j<Columns; j++){
				m.Array[j][i] = Array[i][j];
			}
		}
		return m;
	}
};

class SquareMatrix : public Matrix {
public:
	int Size;
	SquareMatrix(int size, long double initial=0){
		Rows = size;
		Columns = size;
		Size = size;
		for(int i=0; i<size; i++)
			Array.push_back(std::vector<long double>(size,initial));
	}
	SquareMatrix () {}

	void SetArray(std::vector<std::vector<long double>> &array){
		for(int i=0; i<array.size(); i++){
			Array.push_back(array[i]);
		}
		Rows = array.size();
		Columns = array[0].size();
		Size = Rows;
	}
	Matrix Solve(Matrix B){
		std::vector <long double> ans;
		Matrix Ans(Size,1);
		if(B.Rows != Size){
			std::cout<<"Invalid Matrix\n";
			return Ans;
		}
		Matrix A = *this;
		for (int i = 0; i < A.Rows; i++) {
			for (int j = i + 1; j < A.Rows; j++) {
				if (fabs(A.Array[i][i]) < 1e-8) {
					for (int k = i + 1; k < A.Rows; k++) {
						if (fabs(A.Array[k][i]) >= 1e-8) {
							A.SwapRow(i, k);
							B.SwapRow(i, k);
							break;
						}
						if (k == A.Rows - 1) {
							std::cout << "No Solution Possible\n";
							A.SwapRow(i, k);
							B.SwapRow(i, k);
							break;
						}
					}
				}
				long double mul1 = A.Array[i][i];
				long double mul2 = -1 * A.Array[j][i];
				if (fabs(mul2) >= 1e-8) {
					A.RowOperation(j, i, mul1/mul2, 1);
					B.RowOperation(j, i, mul1/mul2, 1);
				}
			}
		}
		for (int i = A.Rows - 1; i >= 0; i--) {
			long double extra = 0;
			for (int j = 0; j < ans.size(); j++) {
				extra += A.Array[i][A.Rows-j-1] * ans[j];
			}
			long double a = (B.Array[i][0]-extra) / A.Array[i][i];
			ans.push_back(a);
		}
		for (int i = 0; i < Size; i++) {
			Ans.Array[i][0] = ans[Size-i-1];
		}
		return Ans;
	}
};

class IdentityMatrix : public SquareMatrix{
public:
	IdentityMatrix(int size){
		Size = size;
		Rows = size;
		Columns = size;
		for(int i=0; i<size; i++){
			Array.push_back(std::vector<long double>(size,0));
			Array[i][i] = 1;
		}
	}
};