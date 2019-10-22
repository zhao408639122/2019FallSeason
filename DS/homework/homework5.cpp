#include<cstdio>
#include<iostream>
#include<stdexcept>
using namespace std;
//P212.7
template<typename T>
class arrayQueue{
public:
    arrayQueue(int = 10);
    arrayQueue(const arrayQueue<T>&);
    ~arrayQueue() {delete[] queue;}

    int queueBack() const {return ((queueFront + queueSize) % listSize);}
    bool empty() const {return queueSize == 0;}
    int size() const {return queueSize;}
    T front() const;
    T back() const;
    void pop();
    void push(const T&);
    void clear();
    arrayQueue<T>& operator=(const arrayQueue<T>&);

protected:
    T* queue;
    int queueFront;
    int queueSize;
    int listSize;
};
template<typename T>
arrayQueue<T>::arrayQueue(int initialCapcity){
    if (initialCapcity < 1) throw invalid_argument("Queue size must > 0");
    listSize = initialCapcity;
    queueFront = queueSize = 0;
    queue = new T[listSize];
}

template<typename T>
arrayQueue<T>::arrayQueue(const arrayQueue<T>& q){
    queueSize = q.queueSize;
    listSize = q.listSize;
    queueFront = q.queueFront - 1;
    queue = new T[listSize];
    for (int i = 0; i < q.size(); ++i) queue[i] = q.queue[(i + q.queueFront + 1) % listSize];
}

template<typename T>
T arrayQueue<T>::front() const{
    if (empty()) throw out_of_range("queue is empty");
    return queue[(queueFront + listSize + 1) % listSize];
}

template<typename T>
T arrayQueue<T>::back() const{
    if (empty()) throw out_of_range("queue is empty");
    return queue[queueBack()];
}

template<typename T>
void arrayQueue<T>::pop(){
    if (empty()) throw out_of_range("queue is empty");
    queueFront++;
    if (queueFront >= listSize) queueFront -= listSize;
    queue[queueFront].~T();
}

template<typename T>
void arrayQueue<T>::push(const T& Ele){
    if ((queueBack() + 1) % listSize == (queueFront + listSize) % listSize){
        T* newQueue = new T[2 * listSize];
        int start = (queueFront + 1) % listSize;
        if (start < 2)
            copy(queue + start, queue + start + listSize - 1, newQueue);
        else {
            copy(queue + start, queue + listSize, newQueue);
            copy(queue, queue + queueBack() + 1, newQueue + listSize - start);
        }
        queueFront = 2 * listSize - 1;
        queueSize *= 2;
        delete[] queue;
        queue = newQueue;
    }
    queueSize++;
    queue[queueBack()] = Ele;
}
template<typename T>
void arrayQueue<T>::clear(){
    queueFront = queueSize = listSize = 0;
    for (int i = 0; i < listSize; ++i) queue[i].~T();
}
template<typename T>
arrayQueue<T>& arrayQueue<T>::operator=(const arrayQueue<T>& q){
    if (this == &q) return *this;
    queueSize = q.queueSize;
    queueFront = q.listSize - 1;
    listSize = q.listSize;
    delete[] queue;
    queue = new T[queueSize];
    for (int i = 0; i < q.size(); ++i) queue[i] = q.queue[(i + q.queueFront + 1) % listSize];
    return *this;
}
//212.8
//lastOp = 0 代表队列可能为空， lastOp = 1代表队列可能为满
//checkOp 返回lastOp 的值
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
    int checkOp() {return lastOp;}
    arrayQueue<T>& operator=(const arrayQueue<T>&);

protected:
    T* queue;
    int queueFront;
    int queueTail;
    int queueSize;
    int lastOp;
};
template<typename T>
arrayQueue<T>::arrayQueue(int initialCapcity){
    if (initialCapcity < 1) throw invalid_argument("Queue size must > 0");
    queueSize = initialCapcity;
    queueFront = queueTail = 0;
    queue = new T[queueSize];
    lastOp = 0;
}

template<typename T>
arrayQueue<T>::arrayQueue(const arrayQueue<T>& q){
    queueSize = q.queueSize;
    queueFront = queueSize - 1;//??
    queueTail  = q.size() - 1;
    queue = new T[queueSize];
    for (int i = 0; i < q.size(); ++i) queue[i] = q.queue[(i + q.queueFront + 1) % queueSize];
    lastOp = 0;
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
    lastOp = 0;
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
    lastOp = 1;
}
template<typename T>
void arrayQueue<T>::clear(){
    queueFront = queueTail = 0;
    for (int i = 0; i < queueSize; ++i) queue[i].~T();
    lastOp = 0;
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
    lastOp = 0;
}
//P239.5
template<class K, class E>
class sortedArrayList{
public:
    ~sortedArrayList();
    bool empty() const{return _Ele.size() == 0;}
    int size() const {return _Ele.size();}

    pair<const K, E>* find(const K&);

    void erase(const K&);

    void insert(const pair<const K, E> &);
    
protected:
    arrayList<pair<K, E> > _Ele;
};

template<class K, class E>
pair <const K, E>* sortedArrayList<K, E>::find(const K& k){
    if (empty()) return NULL;
    int l = 0, r = size() - 1;
    while(l <= r){
        int mid = (l + r) >> 1;
        if (k == _Ele[mid].first) return &_Ele[mid];
        if (k < _Ele[mid].first) r = mid - 1;
        else l = mid + 1;  
    }
}
template<class K, class E>
void sortedArrayList<K, E>::erase(const K& k){
    int l = 0, r = size() - 1, res = -1;
    while(l <= r){
        int mid = (l + r) >> 1;
        if (k == _Ele[mid].first) res = mid;
        if (k < _Ele[mid].first) r = mid - 1;
        else l = mid + 1;  
    }
    if (res != -1) _Ele.erase(res);
}

template<class K, class E>
void sortedArrayList<K, E>::insert(const pair<const K, E> & pr){
    pair<K, E>* node = find(pr.first);
    if (node != NULL){
        node -> second = pr.second;
        return;
    }
    int l = 0, r = size() - 1, res = size();
    while(l <= r){
        int mid = (l + r) >> 1;
        if (pr.first < _Ele[mid].first) res = mid, r = mid - 1;
        else  l = mid + 1;
    }
    _Ele.insert(res, pr);
}
