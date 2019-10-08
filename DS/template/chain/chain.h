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