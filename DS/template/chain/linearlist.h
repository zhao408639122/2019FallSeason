#ifndef __LINEARLIST_H_
#define __LINEARLIST_H_

template<typename T>
class linearList
{
public:
    virtual ~linearList() {};
    virtual bool empty() const = 0;
    virtual int size() const = 0;
    virtual int find(const T&) const = 0;
    virtual void erase (int) = 0;
    virtual void insert(int, const T&) = 0;
    virtual void clear() = 0;
    virtual void push_back(const T&) = 0;

    virtual T&  operator[](int) = 0;
    virtual const T& operator[](int) const = 0;
};

#endif //__LINEARLIST_H_

