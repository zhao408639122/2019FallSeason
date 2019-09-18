#include<cstdio>
#include<iostream>
#include<cstring>
#include<string>
#include <algorithm>

using namespace std;
template <class T>
class ArrayList{
protected:
    T* _Ele;
    int Arr_len;
    int list_size;
public:
    ArrayList(int init_L = 10);
    ArrayList(const ArrayList<T>& );
    void push_back(const T &x);
    void ch_sort();
    void rank_sort();
    void Bubble_sort();
    void insert_sort();
    void output();
};
template <class T>
ArrayList<T> :: ArrayList(int init_L){
    list_size = init_L;
    _Ele = new T[init_L];
    Arr_len = 0;
};
template <class T>
ArrayList<T> ::ArrayList(const ArrayList<T> &t) {
    list_size = t.list_size;
    _Ele = new T[list_size];
    Arr_len = t.Arr_len;
    copy(t._Ele, t._Ele + t.Arr_len, _Ele);
}
template <class T>
void ArrayList<T> ::push_back(const T &x) {
    if (list_size == Arr_len) {
        ArrayList<T>A(*this);
        list_size = list_size * 2;
        delete[] _Ele;
        _Ele = new T[list_size];
        copy(A._Ele + 1, A._Ele + Arr_len, _Ele);
    }
    _Ele[Arr_len++] = x;
}
template <class T>
void ArrayList<T> ::ch_sort() {
    bool sorted = false;
    for (int size = Arr_len; !sorted && (size > 1); size--){
        int ind = 0;
        sorted = true;
        for (int i = 1; i < size; ++i)
            if (_Ele[ind] <= _Ele[i]) ind = i;
            else sorted = false;
        swap(_Ele[ind], _Ele[size - 1]);
    }
}
template <class T>
void ArrayList<T> ::Bubble_sort() {
    bool swapd = true;
    for (int i = Arr_len; i > 1 && swapd; i--){
        swapd = false;
        for (int j = 0; j < i - 1; j++)
            if (_Ele[j] > _Ele[j + 1]){
                swap(_Ele[j], _Ele[j+1]);
                swapd = 1;
            }
    }
}
template <class T>
void ArrayList<T> ::insert_sort() {
    for (int i = 1; i < Arr_len; ++i){
        T t = _Ele[i];
        int j;
        for (j = i - 1; j >= 0 && t < _Ele[j]; --j)
            _Ele[j + 1] = _Ele[j];
        _Ele[j + 1] = t;
    }
}
template <class T>
void ArrayList<T> ::rank_sort() {
    int *r;
    r = new int[list_size];
    memset(r, 0, list_size * 4);
    for (int i = 0; i < Arr_len; ++i){
        for (int j = i + 1; j < Arr_len; ++j){
            if (_Ele[i] > _Ele[j])r[i]++;
            else r[j]++;
        }
    }
//    for (int i = 0; i < Arr_len; ++i)cout<<r[i]<<' '<<endl;
    for (int i = 0; i < Arr_len; ++i){
        while(r[i] != i){
            swap(_Ele[i], _Ele[r[i]]);
            swap(r[i], r[r[i]]);
        }
    }
}
template <class T>
void ArrayList<T> ::output() {
    for (int i = 0; i < Arr_len - 1; ++i)cout<<_Ele[i]<<' ';
    cout<<_Ele[Arr_len - 1];
//    cout<<endl;
}
int main()
{

    ArrayList<int> _L(1010);
    int n, a;
//    int t[1010];
//    ios::sync_with_stdio(false);
    cin>>n;
    for (int i = 1; i <= n; ++i){
        scanf("%d", &a);
        _L.push_back(a);
    }
    _L.ch_sort();
    _L.output();
    return 0;
}