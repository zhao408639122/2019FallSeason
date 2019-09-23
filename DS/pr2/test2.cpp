#include<cstdio>
#include <iostream>
using namespace std;
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
    ArrayList(const ArrayList<T>& );
    void merge(ArrayList<T> a, ArrayList<T> b);
    void push_back(const T &x);
    void ch_sort();
    void output();
    void reverse();
    int size(){
        return Arr_len;
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
void ArrayList<T> ::output() {
    for (int i = 0; i < Arr_len - 1; ++i)cout<<_Ele[i]<<' ';
    cout<<_Ele[Arr_len - 1];
//    cout<<endl;
}
// 22.(1)
template <class T>
void ArrayList<T> ::reverse() {
    int n = Arr_len >> 1;
    T a;
    for (int i = 0; i < n; ++i){//其实可以直接用swap
        a = _Ele[i];
        _Ele[i] = _Ele[Arr_len - i - 1];
        _Ele[Arr_len - i - 1] = a;
    }
}
// 22.(2)
// 可以用程序步数计算只需在循环中加入 step++;

// 22.(4)
template <class T>
void reverse(ArrayList<T> &A){//可以使用get实现，此处使用迭代器实现
    T a;
    int n = A.size() >> 1;
    typename ArrayList<T>::iterator bg = A.begin();
    typename ArrayList<T>::iterator ed = A.end();
    for (int i = 0; i < n; ++i){
        a = *bg;
        ed--;
        *bg = *ed;
        *ed = a;
        bg++;
    }
}
// 22.(5)
// 时间复杂度为O(ListSize)，使用迭代器实现，常数方面比第一份代码更优
// 空间复杂度为O(1)，除了迭代器和函数地址循环变量没有额外开销

// 29.(1)
template <class T>
void ArrayList <T>::merge(ArrayList<T> a, ArrayList<T> b) {
    ArrayList<T> c = a, d = b;
    int len = a.size() + b.size();
    delete[] _Ele;
    _Ele = new T[len];
    Arr_len = 0;
    list_size = len;
    for (int i = 0; i < c.size(); ++i)push_back(c._Ele[i]);
    for (int i = 0; i < d.size(); ++i)push_back(d._Ele[i]);
}
// 29.(2)
// 时间复杂度为O(ListSizeA + ListSizeB)
// 空间复杂度也为O(ListSizeA + ListSizeB)，事实上本题空间复杂度可以优化为O(1)。
int main()
{
    ArrayList<int> _L(1010), _B(1010);
    int n, a;
//    int t[1010];
//    ios::sync_with_stdio(false);
    cin>>n;
    for (int i = 1; i <= n; ++i){
        scanf("%d", &a);
        _L.push_back(a);
    };
    cin>>n;
    for (int i = 1; i <= n; ++i){
        scanf("%d", &a);
        _B.push_back(a);
    }
    _B.merge(_L, _B);
//    _L.reverse();
    _B.output();
    return 0;
}