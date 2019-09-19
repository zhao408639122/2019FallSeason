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
    void insert_sort();
    void output();
};
template <class T> ArrayList<T> :: ArrayList(int init_L){
    list_size = init_L;
    _Ele = new T[init_L];
    Arr_len = 0;
};
template <class T> ArrayList<T> ::ArrayList(const ArrayList<T> &t) {
    list_size = t.list_size;
    _Ele = new T[list_size];
    Arr_len = t.Arr_len;
    copy(t._Ele, t._Ele + t.Arr_len, _Ele);
}
template <class T> void ArrayList<T> ::push_back(const T &x) {
    if (list_size == Arr_len) {
        T *k = new T[list_size * 2];
        list_size = list_size * 2;
        copy(_Ele, _Ele + Arr_len, k);
        delete[] _Ele;
        _Ele = k;
    }
    _Ele[Arr_len++] = x;
}
template <class T> void ArrayList<T> ::insert_sort() {
    for (int i = 1; i < Arr_len; ++i){
        T t = _Ele[i];
        int j;
        for (j = i - 1; j >= 0 && t < _Ele[j]; --j)
            _Ele[j + 1] = _Ele[j];
        _Ele[j + 1] = t;
    }
}
template <class T> void ArrayList<T> ::output() {
    for (int i = 0; i < Arr_len - 1; ++i)cout<<_Ele[i]<<' ';
    cout<<_Ele[Arr_len - 1];
//    cout<<endl;
}
int main()
{

    ArrayList<int> _L(1);
    int n, a;
//    int t[1010];
//    ios::sync_with_stdio(false);
    cin>>n;
    for (int i = 1; i <= n; ++i){
        scanf("%d", &a);
        _L.push_back(a);
    }
    _L.insert_sort();
    _L.output();
    return 0;
}