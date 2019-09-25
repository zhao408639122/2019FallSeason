#include <cstdio>
#include <iostream>
#include <string>

using namespace std;
struct node{
    string name;
    long long tele;
    long long _class;
    long long _domi;
};
template <class T>
class ArrayList{
protected:
    T* _Ele;
    int Arr_len;
    int list_size;
public:
    class iterator;
    iterator begin(){
        return iterator(_Ele);
    }
    iterator end() {
        return iterator(_Ele + Arr_len);
    }
    class iterator{
    protected:
        T* position;
    public:
        typedef bidirectional_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t  difference_type;
        typedef T* pointer;
        typedef T& reference;
        iterator(T* thePosition = nullptr){position = thePosition;}

        T& operator*() const{return *position;}
        T* operator->() const{return &*position;}
        iterator &operator++(){
            ++position;return *this;
        }
        iterator operator++(int){
            iterator old = *this;
            ++position;
            return old;
        }
        iterator& operator--(){
            --position; return *this;
        }
        iterator operator--(int){
            iterator old = *this;
            --position;
            return old;
        }
    };

    ArrayList(int init_L = 10);
    ArrayList(const ArrayList<T>&);
    void push_back(const T &x);
    void output();
    int checkIndex(int x){
        return x < Arr_len;
    }
    int del(int x){
        if (!checkIndex(x)) return 0;
        for (int i = x + 1; i < Arr_len; ++i)
            _Ele[i - 1] = _Ele[i];
        Arr_len--;
        return 1;
    }
    T operator [](const int &t){return *(_Ele + t);}
    int size(){
        return Arr_len;
    }
    void change(int x, T a){
        _Ele[x] = a;
    }
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
void ArrayList<T> ::output() {
    for (int i = 0; i < Arr_len - 1; ++i)cout<<_Ele[i]<<' ';
    cout<<_Ele[Arr_len - 1];
//    cout<<endl;
}
int del_name(ArrayList<node> &A, const string &t) {
    int n = A.size(), _x = -1;
    for (int i = 0; i < n; ++i)
        if (A[i].name == t){
            _x = i;
            break;
        }
    if (_x == -1) return 0;
    else return A.del(_x);
}
void _modify(ArrayList<node> &A, const string &t, int x, long long val){
    int n = A.size();
    for (int i = 0; i < n; ++i){
        if (A[i].name == t){
            node a = A[i];
            if (x == 1) a.tele = val;
            else if (x == 2) a._class = val;
            else a._domi = val;
            A.change(i, a);
            return;
        }
    }
}
int _find(ArrayList<node> &A, const string &t){
    int n = A.size();
    for (int i = 0; i < n; ++i)
        if (A[i].name == t) return 1;
    return 0;
}
long long _class_xor(ArrayList<node> &A, const long long &val){
    int n = A.size();
    long long ans = 0;
    for (int i = 0; i < n; ++i){
        if (A[i]._class == val)
            ans ^= A[i]._domi;
    }
    return ans;
}
int main()
{
    ArrayList<node> _L(20010);
    int n, a;
    string name;
    long long _class, _domi, _tele;
    ios::sync_with_stdio(false);
    cin>>n;
    for (int i = 1; i <= n; ++i){
        cin>>a;
        if (a == 0){
            cin>>name>>_tele>>_class>>_domi;
            _L.push_back((node){name, _tele, _class, _domi});
        } else if (a == 1){
            cin>>name;
            del_name(_L, name);
        } else if (a == 2){
            int x;
            long long val;
            cin>>name>>x>>val;
            _modify(_L, name, x, val);
        } else if (a == 3) {
            cin>>name;
            printf("%d\n", _find(_L, name));
        } else {
            cin>>_class;
            printf("%lld\n", _class_xor(_L, _class));
        }
    };
    return 0;
}