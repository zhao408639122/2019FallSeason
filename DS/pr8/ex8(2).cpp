#include<cstdio>
#include<iostream>
#include<cstring>
#include<string>
#include<algorithm>
#include<utility>
#include<stdexcept>

using namespace std;

template<typename T> class Hash;
template<>
class Hash<string>{
public:
    size_t operator() (const string key) const{
        unsigned long long value = 0;
        int n = key.length();
        for (int i = 0; i < n; ++i) 
            value = value * 128 + key.at(i);
        return size_t(value);
    }
};
template<>
class Hash<int>{
public:
    size_t operator() (const int key) const {
        return size_t(key);
    }
};
template<typename K, typename E>
class hashTable{
public:
    hashTable(int = 10);
    ~hashTable(){
        for (int i = 0; i < divisor; ++i) delete table[i];
        delete[] table;
    }

    bool empty() const {return dsize == 0;}
    int size() const {return dsize;};

    int find(const K&) const;
    int erase(const K&);
    int insert(const K&, const E&);

    void output(ostream&) const;
protected:
    Hash<K> hash;
    pair<const K, E>** table;
    int divisor;
    int dsize;
};

template<typename K, typename E>
hashTable<K, E>::hashTable(int _divisor){
    if (_divisor < 1) throw logic_error("the divisor must greater than 0.");
    divisor = _divisor;
    dsize = 0;
    table = new pair<const K, E>*[divisor];
    for (int i = 0; i < divisor; ++i) table[i] = nullptr;
}

template<typename K, typename E>
int hashTable<K, E>::find(const K& key) const {
    int o = hash(key) % divisor;
    int i = o;
    while(1){
        if (table[i] == nullptr) return -1;
        else if (table[i]->first == key) return i;
        i = (i + 1) % divisor;
        if (i == o) break;
    }
    return -1;
}
template<typename K, typename E>
int hashTable<K, E>::erase(const K& key) {
    int o = hash(key) % divisor;
    int i = o;
    bool find = false;
    while (1) {
        if (table[i] == nullptr) break;
        else if (table[i] ->first == key) {
            find = true;
            delete table[i];
            table[i] = nullptr;
            dsize--;
            break;
        }
        i = (i + 1) % divisor;
        if (i == o) break;
    }
    if (!find) return -1;
    int ans = 0;
    if (find) {
        o = i; 
        int Next = (i + 1) % divisor;
        while(Next != o && table[Next] != nullptr) {
            int ash = hash(table[Next] ->first) % divisor;
            if (Next > i) {
                if (ash > Next || ash <= i) {
                    pair<const K, E>* tmp = table[Next];
                    table[Next] = table[i];
                    table[i] = tmp;
                    i = Next;
                    ans++;
                }
            } else {
                if (ash > Next && ash <= i){
                    pair<const K, E>* tmp = table[Next];
                    table[Next] = table[i];
                    table[i] = tmp;
                    i = Next;
                    ans++;
                }
            }
            Next = (Next + 1) % divisor;
        }
    }
    return ans;
}
template<typename K, typename E>
int hashTable<K, E>::insert(const K& key, const E& value){
    int o = hash(key) % divisor;
    int i = o;
    while(true){
        if (table[i] == nullptr) {
            table[i] = new pair<const K, E>(key, value);
            dsize++;
            return i;
        } else if (table[i] ->first == key) {
            table[i]->second = value;
            return -1;
        }
        i = (i + 1) % divisor;
        if (i == o) throw overflow_error("dictionary is full");
    }
}
int main(){
    int D, m;
    cin>>D>>m;
    hashTable<int, int>mp(D);
    while(m--){
        int op, ans, x;
        scanf("%d%d", &op, &x);
        if (op == 0){
            ans = mp.insert(x, 1);
            if (ans == -1) printf("Existed\n");
            else printf("%d\n", ans);
        } else if (op == 1) 
            printf("%d\n", mp.find(x));
        else {
            int ans = mp.erase(x);
            if (ans == -1) printf("Not Found\n");
            else printf("%d\n", ans);
        }
    }
}