#include<cstdio>
#include<iostream>
#include<cstring>
#include<string>
#include<algorithm>

using namespace std;
const int N = 1010;

template<typename T>
class LowTriMatrix{
public:
    LowTriMatrix(int _n = 0){row = _n;}
    const T& operator() (int, int){}
    void readin(){
        int tot = row * (row + 1) / 2;
        for (int i = 1; i <= tot; ++i) scanf("%d", &element[i]);
    }
protected:
    int row;
    T element[N];    
};
template<typename T>
class UpperTriMatrix{
public:
    UpperTriMatrix(int _n = 0){row = _n;}
    const T& operator() (int, int){}
    const T& operator[] (int a) {return element[a]};
    void readin(){
        int tot = row * (row + 1) / 2;
        for (int i = 1; i <= tot; ++i) scanf("%d", &element[i]);
    }
protected:
    int row;
    T element[N];    
};
template<typename T>
class matrix{
public:
    matrix(int, int);
    void print(){
        for (int i = 1; i <= row; ++i){
            for (int j = 1; j <= col; ++j) printf("%d ", element[i][j]);
            printf("\n");
        }
    }
    friend matrix<T> operator*(const LowTriMatrix<T> &, const UpperTriMatrix<T>&);
protected:
    int row, col;
    T element[100][100];    
};
template<typename T>
matrix<T>::matrix(int _row, int _col){row = _row, col = _col;}
template<typename T>
matrix<T> operator*(const LowTriMatrix<T>& A, const UpperTriMatrix<T>& B){
    int n = A.row, res;
    matrix <T> tmp(n, n);
    for (int i = 1; i <= n; ++i){
        for (int j = 1; j <= n; ++j){
            res = 0;
            int loi = i * (i - 1) / 2, loj = j, ge = n - 1; 
            for (int j = 1, lo = min(i, j); j <= lo; ++j){
                res += A[loi] * B[loj];
                loi++, loj += ge--;
            }
            tmp[i][j] = res;
        }
    }
    return tmp;
}
int main()
{
    int n;
    cin>>n;
    LowTriMatrix<int> A(n);
    UpperTriMatrix<int> B(n);
    int tot = n * (n + 1) / 2;
    A.readin(), B.readin();
    (A * B).print();
}