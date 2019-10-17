nclude<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
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
        iterator(T* thePosition = NULL){position = thePosition;}

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
    T& operator[] (int a){
        return _Ele[a];
    }
    void operator = (const ArrayList<T> &A){
        if (&A == this) return;
        delete[] _Ele;
        _Ele = new T[A.list_size];
        Arr_len = A.Arr_len;
        list_size = A.list_size;
        for (int i = 0; i < Arr_len; ++i) _Ele[i] = A._Ele[i];
    }
    ArrayList(int init_L = 10);
    ArrayList(const ArrayList<T>& );
    void push_back(const T &x);
    void output();
    int size(){
        return Arr_len;
    }
    void clear() {Arr_len = 0;}
    void ch_sort();
    void qsort(){sort(_Ele, _Ele + Arr_len);}
};

template <class T>
ArrayList<T> :: ArrayList(int init_L){
    list_size = init_L;
    _Ele = new T[init_L];
    Arr_len = 0;
}
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
        copy(A._Ele, A._Ele + Arr_len, _Ele);
    }
    _Ele[Arr_len++] = x;
}
template <class T>
void ArrayList<T> ::output() {
    for (int i = 0; i < Arr_len; ++i)cout<<_Ele[i]<<' ';
    cout<<endl;
}
template <class T>
void ArrayList<T> ::ch_sort() {
    bool sorted = false;
    for (int size = Arr_len; !sorted && (size > 1); size--){
        int ind = 0;
        sorted = true;
        for (int i = 1; i < size; ++i)
            if (_Ele[ind] < _Ele[i]) ind = i;
            else sorted = false;
        swap(_Ele[ind], _Ele[size - 1]);
    }
}
template<typename T>
struct matrixTerm{
    int index;
    T value;
    bool operator != (const matrixTerm<T> &x) const {return !(index == x.index && value == x.value); }
    bool operator < (const matrixTerm<T> &x) const {
        return index < x.index;
    }
    void operator = (const matrixTerm<T> &a) {
        index = a.index;
        value = a.value;
    }
    friend ostream & operator << (ostream & out, const matrixTerm &a){
        out<<a.value;
        return out;
    }
};
template <typename T>
class SparseMatrix{
public:
    SparseMatrix(int _row, int _col, int size) : term(size), row(_row), col(_col) {}
    void set(){
        scanf("%d%d", &row, &col);
        term.clear();
        int index = 1;
        for (int i = 1; i <= row; i++){
            for (int j = 1, v; j <= col; ++j){
                scanf("%d", &v);
                if (v) term.push_back((matrixTerm<T>){index, v});
                index++;
            }
        }
        // printf("11111\n");
        // term.output();
    }
    void change(int _row, int _col){
        row = _row;
        col = _col;
        term.clear();
    }
    void operator= (const SparseMatrix<T>& a){
        row = a.row, col = a.col;
        term = a.term;
    }
    T get(int, int) const;
    int get_row() {return row;}
    int get_col() {return col;}
    void insert(int, int, const T&);
    void print(ostream&);
    void add(int, int, T);
    SparseMatrix<T> transpose() const;
    SparseMatrix<T> operator+(SparseMatrix<T>&);
    SparseMatrix<T> operator*(SparseMatrix<T>&);
protected:
    int row, col;
    ArrayList<matrixTerm<T> > term;
};
template<typename T>
T SparseMatrix<T>::get(int _row, int _col) const{
    if (_row < 1 || _row > row || _col < 1 || _col > col) throw out_of_range("the index is out of range");
    int index = (_row - 1) * col + _col;
    int l = 0, r = term.size(), mid;
    while(l < r){
        mid = (l + r) / 2;
        if (index > term[mid].index) l = mid + 1;
        else r = mid;
    }
    if (l == term.size()) return 0;
    else if (term[l].index == index) return term[l].value;
    else return 0;
}
template<typename T>
void SparseMatrix<T>::print(ostream &out) {
    int n = term.size();
    int k = 0, index = 1;
    out << row << ' ' << col << endl;
    T value;
    for (int i = 1; i <= row; ++i){
        for (int j = 1; j <= col; ++j){
            if (k >= n || index < term[k].index) value = 0;
            else value = term[k++].value;
            out << value << ' ';
            index ++;
        }
        out << endl;
    }
}
template<typename T>
SparseMatrix<T> SparseMatrix<T>::transpose() const{
    SparseMatrix<T> res(col, row);
    for (int i = 0; i < this -> term.size(); ++i){
        int index = term[i].index;
        T value = term[i].value;
        int c = index % col;
        int r = index / col + 1;
        if (c == 0){
            c = col, r--;
        }
        res.insert(c, r, value);
    }
    return res;
}
template <typename T>
void SparseMatrix<T>::insert(int _row, int _col, const T& value){
    if (_row < 1 || _row > row || _col < 1 || _col > col) throw out_of_range("the index is out of range");
    int index = (_row - 1) * col + _col;
    term.push_back((matrixTerm<T>){index, value});
}

template<typename T>
SparseMatrix<T> SparseMatrix<T>::operator+(SparseMatrix<T> &a){
    if (row != a.row || col != a.col) throw logic_error("No matching of row and column");
    SparseMatrix<T> ans(row, col, 10010);
    matrixTerm<T> tmp;
    int i = 0, j = 0;
    while(i < term.size() && j < a.term.size()){
        if (term[i].index < a.term[j].index)
            tmp = term[i++];
        else if (term[i].index > a.term[j].index)
            tmp = a.term[j++];
        else {
            tmp = (matrixTerm<T>){term[i].index, term[i].value + a.term[j].value};
            i++, j++;
        }
        ans.term.push_back(tmp);
    }
    while(i < term.size()) ans.term.push_back(term[i++]);
    while(j < a.term.size()) ans.term.push_back(a.term[j++]);
    return ans;
}
template<typename T>
void SparseMatrix<T>::add(int _row, int _col, T value){
    int n = term.size(), res = -1;
    int index = (_row - 1) * col + _col;

    for (int i = n - 1; i >= 0; --i){
        if (index == term[i].index) res = i;
        // if ((term[i].index - 1) % col + 1 < _row) break;
    }
    if (res == -1)
        term.push_back((matrixTerm<T>){index, value});
    else term[res].value = term[res].value + value;
}
inline int calc(int r, int _c, int c){return (r - 1) * c + _c;}
template<typename T>
SparseMatrix<T> SparseMatrix<T>::operator*(SparseMatrix<T> &a) {
    if (col != a.row) throw logic_error("No matching of row and column");
    SparseMatrix<T> ans(row, a.col, 10010);
    T cnt[10010];
    memset(cnt, 0, sizeof(cnt));
    ArrayList<T> item(a.col + 1);
    int pos[101];
    memset(pos, -1, sizeof(pos));
    for (int i = 0; i < a.term.size(); ++i){
        int ar = a.term[i].index / col + 1;
        int ac = a.term[i].index % a.col;
        if (ac == 0) ar--;
        if (pos[ar] == -1) pos[ar] = i;
    }
    for (int i = 0; i < term.size(); ++i){
        int c = term[i].index % col;
        int r = term[i].index / col + 1;
        if (c == 0){
            c = col;
            r--;
        }
        if (pos[c] == -1) continue;
        for (int j = pos[c]; j < a.term.size(); ++j){
            int ac = a.term[j].index % a.col;
            int ar = a.term[j].index / col + 1;
            if (ac == 0){
                ac = a.col;
                ar--;
            }
            if (c == ar)  cnt[calc(r, ac, col)] = cnt[calc(r, ac, col)] +  term[i].value * a.term[j].value;
            else break;
        }
    }
    int tot = row * a.col;
    for (int i = 1; i <= tot; ++i)
        if (cnt[i] != 0)
            ans.term.push_back((matrixTerm<T>){i, cnt[i]});
    return ans;
}
int main(){
    int op, n;
    // ArrayList<int>k(2);
    // for (int i = 1; i <= 10; ++i) k.push_back(i), k.output();
    SparseMatrix<int> M(1, 1, 10010), M2(1, 1, 10010);
    cin>>n;
    for (int i = 1; i <= n; ++i){
        scanf("%d", &op);
        if (op == 1) M.set(); 
        // M.print(cout);
        else if (op == 2){
            M2.set();
            // M2.print(cout);
            if (M.get_col() == M2.get_row()) M = M * M2;
            else M = M2, printf("-1\n");
        } else if (op == 3){
            M2.set();
            if (M.get_row() == M2.get_row() && M.get_col() == M2.get_col()) M = M + M2;
            else M = M2, printf("-1\n");
        } else M.print(cout);
    }
} 
