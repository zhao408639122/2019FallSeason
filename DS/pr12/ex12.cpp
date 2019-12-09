#include<cstdio>
#include<iostream>
#include<cstring>
#include<string>
#include<algorithm>
#include<stdexcept>

using namespace std;

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
     
    chain<T>& operator=(const chain<T>&);
    T& operator[](int);
    const T& operator[](int) const;

    class iterator;
    class const_iterator;

    iterator begin() {return iterator(pHead -> _next);}
    iterator end() {return iterator(NULL);}

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

template<typename T>
class arrayQueue{
public:
    arrayQueue(int = 10);
    arrayQueue(const arrayQueue<T>&);
    ~arrayQueue() {delete[]queue;}

    bool empty() const {return queueTail == queueFront;}
    int size() const {return (queueTail - queueFront + queueSize) % queueSize;}
    T front() const;
    T back() const;
    void pop();
    void push(const T&);
    void clear();

    arrayQueue<T>& operator=(const arrayQueue<T>&);

protected:
    T* queue;
    int queueFront;
    int queueTail;
    int queueSize;
};
template<typename T>
arrayQueue<T>::arrayQueue(int initialCapcity){
    if (initialCapcity < 1) throw invalid_argument("Queue size must > 0");
    queueSize = initialCapcity;
    queueFront = queueTail = 0;
    queue = new T[queueSize];
}

template<typename T>
arrayQueue<T>::arrayQueue(const arrayQueue<T>& q){
    queueSize = q.queueSize;
    queueFront = queueSize - 1;//??
    queueTail  = q.size() - 1;
    queue = new T[queueSize];
    for (int i = 0; i < q.size(); ++i) queue[i] = q.queue[(i + q.queueFront + 1) % queueSize];
}

template<typename T>
T arrayQueue<T>::front() const{
    if (empty()) throw out_of_range("queue is empty");
    return queue[(queueFront + 1) % queueSize];
}

template<typename T>
T arrayQueue<T>::back() const{
    if (empty()) throw out_of_range("queue is empty");
    return queue[queueTail];
}

template<typename T>
void arrayQueue<T>::pop(){
    if (empty()) throw out_of_range("queue is empty");
    queueFront++;
    if (queueFront >= queueSize) queueFront -= queueSize;
    queue[queueFront].~T();
}

template<typename T>
void arrayQueue<T>::push(const T& Ele){
    if ((queueTail + 1) % queueSize == queueFront){
        T* newQueue = new T[2 * queueSize];
        int start = (queueFront + 1) % queueSize;
        if (start < 2)
            copy(queue + start, queue + start + queueSize - 1, newQueue);
        else {
            copy(queue + start, queue + queueSize, newQueue);
            copy(queue, queue + queueTail + 1, newQueue + queueSize - start);
        }
        queueFront = 2 * queueSize - 1;
        queueTail = queueSize - 2;
        queueSize *= 2;
        delete[] queue;
        queue = newQueue;
    }
    queueTail++;
    if (queueTail >= queueSize) queueTail -= queueSize;
    queue[queueTail] = Ele;
}
template<typename T>
void arrayQueue<T>::clear(){
    queueFront = queueTail = 0;
    for (int i = 0; i < queueSize; ++i) queue[i].~T();
}
template<typename T>
arrayQueue<T>& arrayQueue<T>::operator=(const arrayQueue<T>& q){
    if (this == &q) return *this;
    queueSize = q.queueSize;
    queueFront = queueSize - 1;
    queueTail = q.size() - 1;
    delete[] queue;
    queue = new T[queueSize];
    for (int i = 0; i < q.size(); ++i) queue[i] = q.queue[(i + q.queueFront + 1) % queueSize];
    return *this;
}

template<typename T>
class linkedGraph {
    typedef pair<int, T> p;
protected:
    chain<p>* e;
    bool* vis;
    int n, m;
public:
    explicit linkedGraph(const int n) :n(n) {
        e = new chain<p> [n + 1];
        vis = new bool[n + 1];
        m = 0;
    }
    ~linkedGraph() {
        delete[] e;
        delete[] vis;
    }

    void add(int u, int v, const T& w) {
        e[u].push_back(make_pair(v, w));
        ++m;
    }
    
    void erase(int u, int v) {
        auto i = 0;
        for (auto it = e[u].begin(); it != e[u].end() && it ->first != v; ++it, ++i);
        e[u].erase(i);
        --m;
    }

    void bfs(int s) {
        arrayQueue<int> q;
        vis[s] = true;
        q.push(s);
        while(!q.empty()) {
            auto u = q.front(); q.pop();
            cout << u << ' ';
            for (auto it = e[u].begin(); it != e[u].end(); ++it) {
                auto v = it ->first;
                if (!vis[v]) {
                    q.push(v);
                    vis[v] = true;
                }
            }
        }
        cout<<endl;
    }
    void dfs(int u) {
        cout << u << endl;
        vis[u] = true;
        for (auto it = e[u].begin(); it != e[u].end(); ++it) {
            auto v = it ->first;
            if (!vis[v]) dfs(v);
        }
    }
    int dis(int s, int t) const {
        T* dis = new T[n + 1];
        int* vis = new int[n + 1];
        memset(vis, 0, n * 4);
        for (auto i = 1; i <= n; ++i) dis[i] = 0x3f3f3f3f;
        arrayQueue<int> q;
        q.push(s); 
        dis[s] = 0;
        while(!q.empty()) {
            int u = q.front();
            for (auto it = e[u].begin(); it != e[u].end(); ++i) {
                int v = it ->first;
                T& w = it ->second;
                if (dis[u] + w < dis[v]){
                    dis[v] = dis[u] + w;
                    if (!vis[v]){
                        vis[v] = 1;
                        q.push(v);
                    }
                } 
            }
        }
        auto ans = dis[t];
        delete[] dis;
        return ans;
    }
};