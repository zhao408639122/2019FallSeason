#include<cstdio>
#include<iostream>
#include<stdexcept>
#include<cstdlib>
#include<algorithm>
#include<cstring>
#include<functional>

using namespace std;

template<typename T, typename Pr = std::greater<T> >
class Heap {
public:
    Heap(int = 10);
    Heap(const Heap<T, Pr>&);
    ~Heap() {
        delete[] heap;
    }

    bool empty() const {return heapSize == 0;}
    int size() const {return heapSize;}

    const T& top() {
        if (heapSize < 1) throw out_of_range("heap is empty");
        return heap[1];
    }
    void pop();
    void push(const T&);
    void init(T*, T*);
    void erase(const T&);

    void output(std::ostream&, char = '\n');
protected:
    int heapSize;
    int arrayLength;
    T* heap;
    Pr cmp;
};

template<typename T, typename cmp>
Heap<T, cmp>::Heap(int initialCapacity) : cmp() {
    if (initialCapacity < 1) throw out_of_range("the initial capacity of heap must > 0");
    arrayLength = initialCapacity + 1;
    heap = new T[arrayLength];
    heapSize = 0;
}
template<typename T, typename Pr>
Heap<T, Pr>::Heap(const Heap<T, Pr>& theHeap) {
    arrayLength = theHeap.arrayLength;
    heapSize = theHeap.heapSize;
    heap = new T[arrayLength];
    copy(theHeap.heap, theHeap.heap + heapSize + 1, heap);
}

template<typename T, typename Pr>
void Heap<T, Pr>::pop(){
    heap[1] = heap[heapSize--];
    int k = 1, c = 2;
    while(c <= heapSize){
        if (c < heapSize && !cmp(heap[c], heap[c | 1])) c |= 1;
        if (cmp(heap[k], heap[c])) break;
        swap(heap[k], heap[c]);
        k = c;
        c <<= 1;
    }
}

template<typename T, typename Pr>
void Heap<T, Pr>::push(const T& element) {
    if (heapSize >= arrayLength - 1) {
        T* temp = new T[arrayLength * 2];
        for (int i = 1; i <= heapSize; ++i) temp[i] = heap[i];
        delete[] heap;
        heap = temp;
        arrayLength *= 2;
    }
    heap[++heapSize] = element;
    int k = heapSize;
    while(k != 1) {
        if (cmp(heap[k], heap[k >> 1])) swap(heap[k], heap[k >> 1]);
        k >>= 1;
    }
}
template<typename T, typename Pr>
void Heap<T, Pr>::init(T* begin, T* end) {
    delete[] heap;
    heapSize = end - begin;
    arrayLength = heapSize;
    heap = new T[arrayLength + 1];
    for (int i = 1; i <= heapSize; ++i) heap[i] = *(begin + i - 1);
    for (int rt = heapSize / 2; rt; --rt) {
        int k = rt;
        int c = k << 1;
        while(c <= heapSize) {
            if (c < heapSize && !cmp(heap[c], heap[c | 1])) c |= 1;
            if (cmp(heap[k], heap[c])) break;
            swap(heap[k], heap[c]);
            k = c; 
            c <<= 1;
        }
    }
}
template<typename T, typename Pr>
void Heap<T, Pr>::erase(const T& element) {
    int k = heapSize;
    for (int i = 1; i <= heapSize; ++i) {
        if (heap[i] == element) {
            heap[i] = heap[heapSize];
            heap[heapSize--].~T();
            k = i;
            break;
        }
    }
    int c = k << 1;
    while(c <= heapSize) {
        if (c < heapSize && !cmp(heap[c], heap[c << 1])) c |= 1;
        if (cmp(heap[k], heap[c])) break;
        swap(heap[k], heap[c]);
        k = c;
        c <<= 1;
    }
}

template<typename T, typename Pr>
void Heap<T, Pr>::output(ostream& out, char ends) {
    for (int i = 1; i <= heapSize; ++i) out << heap[i] << ' ';
    out<<ends;
}
int a[10010];
int main(){
    Heap<int, less<int> >H;
    int n, m;
    cin>>n;
    for (int i = 1; i <= n; ++i) scanf("%d", &a[i]);
    cin>>m;
    H.init(a + 1, a + n + 1);
    printf("%d\n", H.top());
    while(m--){
        int op, num;
        scanf("%d", &op);
        if (op == 1) {
            scanf("%d", &num);
            H.push(num);
            printf("%d\n", H.top());
        } else if (op == 2) {
            H.pop();
            // H.output(cout);
            printf("%d\n", H.top());
        } else {
            scanf("%d", &n);
            for (int i = 1; i <= n; ++i) scanf("%d", &a[i]);
                H.init(a + 1, a + n + 1);
            for (int i = 1; i <= n; ++i){
                printf("%d ", H.top());
                H.pop();
            }
        }
    }
}