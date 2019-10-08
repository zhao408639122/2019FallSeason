#ifndef __ARRAYLIST_H_
#define __ARRAYLIST_H_

#include "linearlist.h"
#include <stdexcept>

using namespace std;

template<typename T>
class arrayList 
    :virtual public linearList<T>
{
public:
    arrayList(int = 10);
    arrayList(const arrayList<T>&);
    ~arrayList();

    bool empty() const;
    int size() const;
    int find(const T&) const;
    void erase(int);
    void insert(int, const T&);

    void clear();
    void push_back(const T&);

    arrayList<T>& operator=(const arrayList<T>&);
    T& operator[](int);
    const T& operator[](int) const;

    class iterator;
    class const_iterator; 

    const_iterator begin() const {return const_iterator(element);}
    const_iterator end() const {return const_iterator(element + listZize);}

    iterator begin() {return iterator(element);}
    iterator end() {return iterator(element + listSize);}
    class iterator{
    public:
        typedef bidirectional_iterator_tag __iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef T* pointer;
        typedef T& reference;

        iterator(T* thePosition = nullptr) : position(thePosition) {}
        T& operator*() {return *position; }
        T* operator->() {return &*position; }

        iterator& operator++(){
            ++position;
            return *this;
        }
        iterator operator++(int){
            iterator old = *this;
            ++position;
            return old;
        }
        iterator& operator--(){
            --position;
            return *this;
        }
        iterator operator--(int){
            iterator old = *this;
            --position;
            return old;
        }
    
        bool operator != (const iterator right) const {return position != right.position;}
        bool operator == (const iterator right) const {return position == right.position;}

    protected:
        T* position;
    };

    class const_iterator
	{
	public:
		typedef bidirectional_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef T* pointer;
		typedef T& reference;

		const_iterator(T* thePosition = nullptr) :position(thePosition) {};

		const T& operator*() { return *position; }
		const T* operator->() { return &*position; }

		const_iterator& operator++()
		{
			++position;
			return *this;
		}
		const_iterator operator++(int)
		{
			const_iterator old = *this;
			++position;
			return old;
		}

		const_iterator& operator--()
		{
			--position;
			return *this;
		}
		const_iterator operator--(int)
		{
			const_iterator old = *this;
			--position;
			return old;
		}

		bool operator==(const const_iterator right) const { return position == right.position; }
		bool operator!=(const const_iterator right) const { return position != right.position; }

	protected:
		T* position;
	};
    protected:
        int listSize;
        int arrayLength;
        T* element;
        void checkIndex(int theIndex) const;
};

template<typename T>
arrayList<T>::arrayList(int initialCapacity)
{
	if (initialCapacity < 1)
		throw out_of_range("the initial Capacity of arrayList mut > 0");
	arrayLength = initialCapacity;
	element = new T[arrayLength];
	listSize = 0;
}

template<typename T>
arrayList<T>::arrayList(const arrayList<T>& a){
    arrayLength = a.arrayLength;
    listSize = a.listSize;
    element = new = T[arrayLength];
    copy(a.element, a.element + a.listSize, element);
}
template<typename T>
arrayList<T>::~arrayList() {delete[] element;}

template<typename T>
bool arrayList<T>::empty() const {return listSize == 0;}

template<typename T>
int arrayList<T>::size() const {return listSize;}

template<typename T>
int arrayList<T>::find(const T& theElement) const{
    int i;
    for (i = 0; i < listSize && element[i] != theElement; ++i);
    if (i == listSize) return -1;
    return i;
}

template<typename T>
void arrayList<T>::erase(int theIndex){
    checkIndex(theIndex);
    copy(element + theIndex + 1, element + listSize, element + theIndex);
    element[--listSize].~T();
}

template<typename T>
void arrayList<T>::insert(int theIndex, const T& theElement){
    if (theIndex < 0 || theIndex > listSize)
        throw out_of_range("illegalIndex");
    if (listSize == arrayLength){
        arrayLength *= 2;
        T *tmp = new T[arrayLength];
        copy(element, element + listSize, tmp);
        delete[] element;
        element = tmp;
    }
    for (int i = listSize; i > theIndex; --i) element[i] = element[i - 1];
    element[theIndex] = theElement;
    listSize++;
}

template<typename T>
void arrayList<T>::clear()
{
	delete[]element;
	element = new T[arrayLength];
	listSize = 0;
}

template<typename T>
void arrayList<T>::push_back(const T& theElement)
{
	if (listSize == arrayLength)
	{
		arrayLength *= 2;
		T* tmp = new T[arrayLength];
		copy(element, element + listSize, tmp);
		delete[]element;
		element = tmp;
	}
	element[listSize++] = theElement;
}

template<typename T>
T& arrayList<T>::operator[](int theIndex)
{
	checkIndex(theIndex);
	return element[theIndex];
}

template<typename T>
arrayList<T>& arrayList<T>::operator=(const arrayList<T>& a)
{
	if (this == &a)	return *this;
	listSize = a.listSize;
	arrayLength = a.arrayLength;
	delete[]element;
	element = new T[arrayLength];
	copy(a.element, a.element + listSize, element);
	return *this;
}

template<typename T>
const T& arrayList<T>::operator[](int theIndex) const
{
	checkIndex(theIndex);
	return element[theIndex];
}

template<typename T>
void arrayList<T>::checkIndex(int theIndex) const
{
	if (theIndex < 0 || theIndex >= listSize)
		throw out_of_range("the index is out of range!");
}

#endif //__ARRAYLIST_H_