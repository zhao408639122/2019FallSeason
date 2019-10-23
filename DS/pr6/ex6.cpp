#include<cstdio>
#include<iostream>
#include<string>
#include<cstring>
#include<algorithm>
#include<stdexcept>

using namespace std;

template<typename T>
class arrayStack{
public:
    arrayStack(int = 10);
    arrayStack(const arrayStack<T>&);
    ~arrayStack() {delete[] stack;}

    bool empty() const {return stackTop == 0;}
    int size() const {return stackTop;}

    T top() const;

    void pop();
    void push(const T&);
    void clear();

    arrayStack<T>& operator=(const arrayStack<T>&);
protected:
    T* stack;
    int stackTop;
    int stackSize;
};

template<typename T>
arrayStack<T>::arrayStack(int initialCapacity){
    if (initialCapacity < 1) throw out_of_range("the initial capacity must > 0");
    stackSize = initialCapacity;
    stack = new T[stackSize];
    stackTop = 0;
}
template<typename T>
arrayStack<T>::arrayStack(const arrayStack<T>& s){
    stackSize = s.stackSize;
    stackTop = 0;
    stack = new T[stackSize];
    while(stackTop < s.stackTop){
        stack[stackTop] = s.stack[stackTop];
        stackTop++;
    }
}

template<typename T>
T arrayStack<T>::top() const{
    if (stackTop <= 0) throw out_of_range("stack is empty");
    return stack[stackTop - 1];
}

template<typename T>
void arrayStack<T>::pop(){
    if (stackTop <= 0) throw out_of_range("stack is empty");
    stack[--stackTop].~T();
}

template<typename T>
void arrayStack<T>::push(const T& _Ele){
    if (stackTop == stackSize){
        T* newStack = new T[stackSize * 2];
        copy(stack, stack + stackTop, newStack);
        delete stack;
        stack = newStack;
        stackSize *= 2;
    }
    stack[stackTop++] = _Ele;
} 
template<typename T>
void arrayStack<T>::clear(){
    while(stackTop > 0) stack[--stackTop].~T();
}

template<typename T>
arrayStack<T>& arrayStack<T>::operator=(const arrayStack<T>& s){
    if (this == &s) return *this;
    stackSize = s.stackSize;
    stackTop = 0;
    stack = new T[stackSize];
    while(stackTop < s.stackTop){
        stack[stackTop] = s.stack[stackTop];
        stackTop++;
    }
    return *this;
}
char s[2020];
int t[2020];
int getflag(char a){
    if (a == '+') return 1;
    if (a == '-') return 2;
    if (a == '*') return 3;
    if (a == '/') return 4;
    if (a == '(') return 5;
    else return 6;
}
// + 1, - 2, * 3, / 4, ( 5, ) 6
int getvalue(int a){
    if (a == 1 || a == 2) return 2;
    if (a == 3 || a == 4) return 3;
    else return 1;
}
arrayStack<int> flag;
int suffixSwichy(){
    int ind = 0;
    int n = strlen(s + 1);
    for (int i = 1; i <= n; ++i){
        if (s[i] >= '0' && s[i] <= '9') t[++ind] = s[i] - '0' + 10; 
        else {
            int f = getflag(s[i]);
            if (f == 5) flag.push(f);
            else if (f == 6) {
                while(flag.top() != 5){
                    t[++ind] = flag.top();
                    flag.pop();
                }
                flag.pop();
            } else {                    
                while(!flag.empty() && getvalue(flag.top()) >= getvalue(f)){
                    t[++ind] = flag.top();
                    flag.pop();
                }
                flag.push(f);
            }
        }
    }
    while(!flag.empty()) t[++ind] = flag.top(), flag.pop();
    return ind;
}
int main(){
    arrayStack<double> num;
    int T;
    cin>>T;
    while(T--){
        scanf("%s", s + 1);
        int res = suffixSwichy();
        for (int i = 1; i <= res; ++i){
            if (t[i] < 10){
                int a = num.top(); num.pop();
                int b = num.top(); num.pop();
                if (t[i] == 1) num.push(a + b);
                else if (t[i] == 2) num.push(b - a);
                else if (t[i] == 3) num.push(b * a);
                else num.push(b / a);
            }
        }
        
    }
}
// 1+2*3+(4*5+6)*7