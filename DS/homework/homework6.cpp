#include<cstdio>
#include<iostream>
#include<cstring>
#include<string>
#include<algorithm>
#include<cmath>
#include<stdexcept>

using namespace std;
template<class K, class E>
struct skipNode{
	typedef pair<const K, E> pairType;
	
	pairType element;
	skipNode<K, E> **nxt;
	
	skipNode(const pairType& thePair, int size) : element(thePair) {
		nxt = new skipNode<K, E>* [size];
	}
};
template<class K, class E>
class skipList{
	skipList(K, int, float);

	int level() const;
	skipNode<K, E>* search(const K&) const;

	pair<const K, E>* find(const K&) const;	
	void insert(const pair<const K, E>& thePair);
	void erase(const K&);
protected:
	float cutOff;
	int levels;
	int dSize;
	int maxLevel;
	K tailKey;
	skipNode<K, E>* headerNode;
	skipNode<K, E>* tailNode;
	skipNode<K, E>** last;
};
template<class K, class E>
skipList<K, E>::skipList(K largeKey, int maxPairs, float prob){
	cutOff = prob * RAND_MAX;
	maxLevel = (int) ceil(logf((float) maxPairs) / logf (1 / prob)) - 1;
	levels = 0;
	dSize = 0;
	tailKey = largeKey;

	pair<K, E> tailPair;
	tailPair.first = tailKey;
	headerNode = new skipNode<K, E> (tailPair, maxLevel + 1);
	tailNode = new skipNode<K, E> (tailPair, 0);
	last = new skipNode<K, E> *[maxLevel + 1];

	for (int i = 0; i <= maxLevel; ++i)
		headerNode ->nxt[i] = tailNode;
}
template<class K, class E>
pair<const K, E>* skipList<K, E>::find(const K& theKey) const{
	if (theKey >= tailKey) return NULL;
	skipNode<K, E>* beforeNode = headerNode;
	for (int i = levels; i >= 0; i--)
		while(beforeNode -> nxt[i] -> element.first < theKey)
			beforeNode = beforeNode ->nxt[i];
	if (beforeNode->nxt[0]->element.first == theKey)
		return &beforeNode->nxt[0]->element;
	return NULL;
}
template<class K, class E>
int skipList<K, E>::level() const{
	int lev = 0;
	while(rand() <= cutOff)
		lev++;
	return (lev <= maxLevel) ? lev : maxLevel;
}
template<class K, class E>
skipNode<K, E>* skipList<K, E>::search(const K& theKey) const{
	skipNode<K, E>* beforeNode = headerNode;
	for (int i = levels; i >= 0; i--){
		while(beforeNode ->nxt[i] ->element.first < theKey)
			beforeNode = beforeNode ->nxt[i];
		last[i] = beforeNode;
	}
	return beforeNode->nxt[0];
}
template<class K, class E>
void skipList<K, E>::insert(const pair<const K, E>& thePair){
	if (thePair.first >= tailKey){
		throw invalid_argument("error");
	}

	skipNode<K, E>* theNode = search(thePair.first);
	if (theNode ->element.first == thePair.first){
		theNode ->element.second = thePair.second;
		return;
	}

	int theLevel = level();
	if (theLevel > levels){
		theLevel = ++levels;
		last[theLevel] = headerNode;
	}

	skipNode<K, E>* newNode = new skipNode<K, E>(thePair, theLevel + 1);
	for (int i = 0; i <= theLevel; ++i){
		newNode ->nxt[i] = last[i]->nxt[i];
		last[i] ->nxt[i] = newNode;
	}

	dSize++;
	return;
}
template<class K, class E>
void skipList<K, E>::erase(const K& theKey){
	if (theKey >= tailKey) return;

	skipNode<K, E>* theNode = search(theKey);
	if (theNode ->element.first != theKey) return;

	for (int i = 0; i <= levels && last[i] ->nxt[i] == theNode; ++i)
		last[i]->nxt[i] = theNode->nxt[i];

	while(levels > 0 && headerNode ->nxt[levels] == tailNode)
		levels--;

	delete theNode;
	dSize--;
}


int main()
{

}