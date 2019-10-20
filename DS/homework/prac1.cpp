#include<cstdio>
#include<iostream>
#include<cstring>
#include<string>
#include<algorithm>
#include<stdexcept>
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

template<typename T>
struct chainNode
{
    T element;
    chainNode<T>* _next;
    chainNode(const T& _element, chainNode<T>* _next = NULL) : element(_element), _next(_next) {}
    chainNode(const chainNode<T>*& c) : element(c->element), _next(c->_next) {}
};

template<typename T>
class chain{
public:
    chain(int = 10);
    chain(const chain<T>&);
    ~chain();

    bool empty() const;
    int size() const;
    int find(const T&) const;
    void erase(int);
    void insert(int, const T&);

    void clear();
    void push_back(const T&);
    void reverse();
    void unique();
    void merge(chain<T> &a, chain<T> &b);
    chain<T>& operator=(const chain<T>&);
    void print();

    T& operator[](int);
    const T& operator[](int) const;

    class iterator;
    class const_iterator;

    iterator begin() {return iterator(pHead -> _next);}
    iterator end() {return iterator(NULL);}
    void revprint(iterator a);  
    const_iterator begin() const {return const_iterator(pHead -> _next);}
    const iterator end() const {return const_iterator(NULL);}
    class iterator{
    public:
        typedef forward_iterator_tag iterator_category;
        typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef T* pointer;
		typedef T& reference;

        iterator(chainNode<T>* theNode = NULL) :node(theNode) {}

        T& operator*() {return node -> element;}
        T* operator->() {return &node->element; }

        iterator& operator++(){
            node = node -> _next;
            return *this;
        }
        iterator operator++(int){
            iterator old = *this;
            node = node->_next;
            return old;
        }
    
        bool operator==(const iterator right) const {return node == right.node;}
        bool operator!=(const iterator right) const {return node != right.node;}
    protected:
        chainNode<T>* node;
    };
    class const_iterator{
    public:
    	typedef forward_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef T* pointer;
		typedef T& reference;

        const_iterator(chainNode<T>* theNode) :node(theNode) {};

		const T& operator*() { return node->element; }
		const T* operator->() { return &node->element; }

		const_iterator& operator++()
		{
			node = node->_next;
			return *this;
		}
		const_iterator operator++(int)
		{
			const_iterator old = *this;
			node = node->_next;
			return old;
		}

		bool operator==(const const_iterator right) const { return node == right.node; }
		bool operator!=(const const_iterator right) const { return node != right.node; }

    protected:
        chainNode<T>* node;
    };
protected:
    chainNode<T>* pHead;
    chainNode<T>* pTail;
    int listSize;
    void checkIndex(int) const;
};

template<typename T>
chain<T>::chain(int initialCapacity){
    if (initialCapacity < 1) throw out_of_range("the initial Capacity of arrayList must > 0");
    listSize = 0;
    pHead = new chainNode<T>(T());
    pTail = pHead;
}
template<typename T>
chain<T>::chain(const chain<T>& c){
    pHead = new chainNode<T>(c.pHead->element);
    pTail = pHead;
    chainNode<T>* sourceNode = c.pHead ->_next;
    chainNode<T>* currentNode = pHead;
    while(sourceNode != NULL){
        pTail = currentNode->_next;
        sourceNode = sourceNode->_next;
    }
    listSize = c.listSize;
}

template<typename T>
chain<T>::~chain(){
    chainNode<T>* currentNode = pHead ->_next;
    chainNode<T>* deleteNode;
    while(currentNode != NULL){
        deleteNode = currentNode;
        currentNode = currentNode ->_next;
        delete deleteNode;
    }
    delete pHead;
}

template<typename T>
bool chain<T>::empty() const {return listSize == 0;}

template<typename T>
int chain<T>::size() const {return listSize;}

template<typename T>
int chain<T>::find(const T& theElement) const{
    int index = 0;
    chainNode<T>* currentNode = pHead ->_next;
    while(currentNode != NULL){
        if (currentNode->element == theElement) return index;
        currentNode = currentNode->_next;
        ++index;
    }
    return -1;
}
template<typename T>
void chain<T>::erase(int theIndex){
    checkIndex(theIndex);
    chainNode<T>* deleteNode;
    chainNode<T>* pre = pHead;
    for (int i = 0; i < theIndex; ++i) pre = pre->_next;
    if (theIndex == listSize - 1) pTail = pre;
    deleteNode = pre->_next;
    pre->_next = pre->_next->_next;
    --listSize;
    delete deleteNode;
}

template<typename T>
void chain<T>::insert(int theIndex, const T& theElement){
    if (theIndex < 0 || theIndex > listSize) throw out_of_range("illegalIndex");
    chainNode<T>* pre = pHead;
    for (int i = 0; i < theIndex; ++i) pre = pre->_next;
    pre->_next = new chainNode<T>(theElement, pre->_next);
    if (theIndex == listSize) pTail = pre->_next;
    ++listSize;
}
template<typename T>
void chain<T>::clear(){
    chainNode<T>* currentNode = pHead->_next;
    chainNode<T>* deleteNode;
    while(currentNode != NULL){
        deleteNode = currentNode;
        currentNode = currentNode->_next;
        delete deleteNode;
    }
    listSize = 0;
    pHead ->_next = NULL;
    pTail = pHead;
}
template<typename T>
void chain<T>::push_back(const T& theElement){
    pTail->_next = new chainNode<T>(theElement, pTail->_next);
    pTail = pTail->_next;
    listSize++;
}
template<typename T>
chain<T>& chain<T>::operator=(const chain<T>& c){
    if (this == &c) return *this;
    clear();
    chainNode<T>* currentNode = pHead;
    chainNode<T>* sourceNode = c.pHead->_next;
    while (sourceNode != NULL)
    {
        pTail = currentNode->_next = new chainNode<T>(sourceNode->element);
        currentNode = currentNode->_next;
        sourceNode = sourceNode->_next;
    }
    listSize = c.listSize;
    return *this;
}
template<typename T>
T& chain<T>::operator[](int index){
    checkIndex(index);
    chainNode<T>* currentNode = pHead->_next;
    for (int i = 0; i < index; ++i) currentNode = currentNode->_next;
    return currentNode->element;
}
template<typename T>
const T& chain<T>::operator[](int index) const{
    checkIndex(index);
    chainNode<T> *currentNode = pHead->_next;
    for (int i = 0; i < index ; ++i) currentNode = currentNode->_next;
    return currentNode->element;
}

template<typename T>
void chain<T>::checkIndex(int theIndex) const{
    if (theIndex < 0 || theIndex >= listSize)
        throw out_of_range("the index is out of range");
}

//12.(1) 把get方法写成了[]运算符 
//时间复杂度为O(N^2)
template<typename T>
inline ArrayList<T> transform1(chain<T> c){
    ArrayList<T> res(c.size());
    for (int i = 0; i < c.size(); ++i){
        res.push_back(c[i]);
    } 
    return res;
}
//(2). 使用迭代器，时间复杂度为O(N)
template<typename T>
inline ArrayList<T> transform2(chain<T> c){
    ArrayList<T> res(c.size());
    typename chain<T>::iterator a = c.begin();
    for (int i = 1; i <= c.size(); ++i){
        res.push_back(*a.element);
        a++;
    }
    return res;
}
//15.因为不是双向链表，只能做到O(N^2)复杂度
//但是其实可以通过颠倒位置来交换顺序只需要将所有指针倒过来
//时间复杂度O(N)
template<typename T>
inline void chain<T>::reverse(){
    chainNode<T>* pre = pHead;
    chainNode<T>* currentNode = pHead->_next, tmp = NULL;
    for (int i = 1; i < listSize; ++i){
        tmp = currentNode ->_next;
        currentNode->_next = pre;
        pre = currentNode;
        currentNode = tmp;
    } 
    pHead ->_next = NULL;
    swap(pHead, pTail);
}
//16.使用复杂度为O(N^2)的方法来解决这道问题
template<typename T>
inline void reverse(chain<T> &c){
    int l = 0, r = c.size() - 1;
    while(l < r){
        swap(c[l], c[r]);
    }
} 
//19.chain类里已经封装了Extendedchain的ADT，所以下面依然使用chain表示
template<typename T>
inline chain<T> merge(chain<T> &a, chain<T> &b){
    chain<T> res;
    typename chain<T>::iterator ita = a.begin(), itb = b.begin();
    while(ita != a.end() && itb != b.end()){
        // cout<< *ita << ' ' << *itb << ' ' << (*ita > *itb) << endl;
        if (*ita > *itb)
            res.push_back(*itb), itb++;
        else 
            res.push_back(*ita), ita++;
    }
    while(ita != a.end()) res.push_back(*ita), ita++;
    while(itb != b.end()) res.push_back(*itb), itb++;
    return res;
}
//20.和上题写法基本相同，最后clear一下
template<typename T>
void chain<T>::merge(chain<T> &a, chain<T> &b){
    typename chain<T>::iterator ita = a.begin(), itb = b.begin();
    while(ita != a.end() && itb != b.end()){
        if (*ita > *itb) 
            push_back(*itb), itb++;
        else 
            push_back(*ita), ita++;
    }
    while(ita != a.end()) push_back(*ita), ita++;
    while(itb != b.end()) push_back(*itb), itb++;
    a.clear(), b.clear();
}
template<typename T>
void chain<T>::print(){
    typename chain<T>::iterator it = begin();
    for (int i = 0; i < listSize; ++i){
        printf("%d ",*it);
        it++;
    }
    printf("\n");
}
//1.
template<typename T>
void chain<T>::unique(){
    for (chainNode<T>* it = pHead; it != end(); it = it -> _next){
        while (it -> _next != NULL && it -> element == it -> _next -> element){
            chainNode<T> *del = it;
            it -> _next = it -> _next -> _next;
            delete del;
        }
    }
}
//2.
template<typename T>
chain<T> get_union(const chain<T> &a, const chain<T> &b){
    chain<T> res(a);
    for (typename chain<T> :: iterator it = b.begin(); it != b.end(); ++it){
        int flag = 1;
        for (typename chain<T> :: iterator th = a.begin(); th != a.end(); ++th)
            if (*th == *it) flag = 0;
        if (flag) res.push_back(*it);
    }
}

template<typename T>
void chain<T>::revprint(iterator a){
    if (a != end()){
        T v = *a;
        revprint(++a);
        printf("%d ", v);
    }
}
int main()
{
    chain<int> a;
    int T, op, v;
    cin>>T;
    while(T--){
        scanf("%d", &op);
        if (op == 1) scanf("%d", &v), a.insert(0, v);
        else if (op == 2) scanf("%d", &v), a.erase(a.find(v));
        else if (op == 3) scanf("%d", &v), printf("%d\n", a.find(v));
        else if (op == 4) a.print();
        else if (op == 5) a.revprint(a.begin()), printf("\n");
        else {
            int n;
            chain<int> b, c;
            scanf("%d", &n);
            for (int i = 1; i <= n; ++i) scanf("%d", &v), b.push_back(v);
            scanf("%d", &n);
            for (int i = 1; i <= n; ++i) scanf("%d", &v), c.push_back(v);
            merge(b,c).print();
        }
    }
}