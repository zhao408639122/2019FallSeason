#include<cstdio>
#include<iostream>
#include<stdexcept>

template<typename T>
class arrayQueue{
public:
    arrayQueue(int = 10);
    arrayQueue(const arrayQueue<T>&);
    ~arrayQueue() {delete[] queue;}

    int queueBack() const {return ((queueFront + queueSize) % listSize);}
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
    int queueSize;
    int listSize;
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
    if (empyt()) throw out_of_range("queue is empty");
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
