#include<cstdio>
#include<iostream>
#include<cstdlib>
#include<stdexcept>
#include<queue>
#include<functional>

using namespace std;

template<typename T>
struct binaryTreeNode{
	T element;
	binaryTreeNode<T>* leftChild;
	binaryTreeNode<T>* rightChild;
	binaryTreeNode<T>* father;

	binaryTreeNode() {}
	binaryTreeNode(const T& theElement) : element(theElement) {
		leftChild = rightChild = father = nullptr;
	}
	bool operator<(const binaryTreeNode<T>& x) const {
		return element < x.element;
	}
};
template<typename T>
class linkedBinaryTree{
public:
	linkedBinaryTree() {root = nullptr;}
	virtual ~linkedBinaryTree() {erase();}

	bool operator <(const linkedBinaryTree<T>& x) const{
		return root ->element < x.root->element;
	}
	
	bool empty() const {return size(root) == 0;}
	int size() const {return size(root);}
	//求树根
	binaryTreeNode<T>*& getRoot(){
		return root;
	}
	//先序遍历
	void preOrder(void(*_visit)(binaryTreeNode<T>*) = linkedBinaryTree<T>::output) {
		visit = _visit;
		preOrder(root);
	}
	//中序遍历
	void inOrder(void(*_visit)(binaryTreeNode<T>*) = linkedBinaryTree<T>::output) {
		visit = _visit;
		inOrder(root);
	}
	//后序遍历
	void postOrder(void(*_visit)(binaryTreeNode<T>*) = linkedBinaryTree<T>::output) {
		visit = _visit;
		postOrder(root);
	}
	//层次遍历
	void levelOrder(void(*_visit)(binaryTreeNode<T>*) = linkedBinaryTree<T>::output) {
		visit = _visit;
		levelOrder(root);
	}
	//清空树
	void erase(){
		postOrder(linkedBinaryTree<T>::dispose);
		root = nullptr;
	}
	//建树
	void makeTree(const T&, linkedBinaryTree<T>&, linkedBinaryTree<T>&);

	//求树高
	int height() const {return height(root);}

	int levelMax(binaryTreeNode<T>*);
	binaryTreeNode<T>* makeTree(int*, int*, int);
	
	bool compare(linkedBinaryTree<T> x);
protected:
	static int tmp;
	static T* tmp1;
	binaryTreeNode<T>* root;
	static void* visit(binaryTreeNode<T>*);
	static void preOrder(binaryTreeNode<T>*);
	static void inOrder(binaryTreeNode<T>*);
	static void postOrder(binaryTreeNode<T>*);
	static void levelOrder(binaryTreeNode<T>*);
	static void dispose(binaryTreeNode<T>* t) {delete t;}
	static int size(binaryTreeNode<T>*);
	static int height(binaryTreeNode<T>*);
	static void output(binaryTreeNode<T>* t) {cout << t -> element << ' ';}
};

template<typename T>
void linkedBinaryTree<T>::preOrder(binaryTreeNode<T>* t){
	if (t == nullptr) return;
	visit(t);
	preOrder(t ->leftChild);
	preOrder(t ->rightChild);
}

template<typename T>
void linkedBinaryTree<T>::inOrder(binaryTreeNode<T>* t){
	if (t == nullptr) return;
	inOrder(t ->leftChild);
	visit(t);
	inOrder(t ->rightChild);
}

template<typename T>
void linkedBinaryTree<T>::postOrder(binaryTreeNode<T>* t) {
	if (t == nullptr) return;
	postOrder(t ->leftChild);
	postOrder(t ->rightChild);
	visit(t);
}

template<typename T>
void linkedBinaryTree<T>::levelOrder(binaryTreeNode<T>* t) {
	queue<binaryTreeNode<T>* >q;
	q.push(t);
	while(!q.empty()){
		binaryTreeNode<T>* p = q.front(); q.pop();
		if (p == nullptr) continue;
		visit(p);
		q.push(p ->leftChild);
		q.push(p ->rightChild);
	}
}
template<typename T>
void linkedBinaryTree<T>::makeTree(const T& theElement, linkedBinaryTree<T>& left, linkedBinaryTree<T>& right){
	root = new binaryTreeNode<T>(theElement);
	root ->leftChild = left.root;
	root ->rightChild = right.root;

	if (root->leftChild != nullptr) root ->leftChild ->father = root;
	if (root->rightChild != nullptr) root ->rightChild ->father = root;

	left.root = right.root = nullptr;
}
//P280.20 程序复杂度为O(last)
// void preOrder(binaryTree k){
// 	visit(k);
// 	if (a[k << 1].first) preOrder(k << 1);
// 	if (a[k << 1 | 1].first) preOrder(k << 1 | 1);
// }
//P281.27 时间复杂度为O(N)
template<typename T>
int linkedBinaryTree<T>::height(binaryTreeNode<T>* t){
	if (t == nullptr) return 0;
	int hl = height(t ->leftChild);
	int hr = height(t ->rightChild);
	
	return (hl > hr ? hl : hr) + 1;
}
//P281.28 时间复杂度为O(N)
template<typename T>
int linkedBinaryTree<T>::size(binaryTreeNode<T>* t){
	if (t == nullptr) return 0;
	return size(t ->leftChild) + size(t ->rightChild) + 1;
}
//P281.29 时间复杂度为O(N)
template<typename T>
int linkedBinaryTree<T>::levelMax(binaryTreeNode<T>* t) {
	int cnt[height(root)];
	queue<pair<binaryTreeNode<T>*, int> >q;
	int ans = 0;
	q.push(t);
	while(!q.empty()){
		pair<binaryTreeNode<T>*, int> p = q.front(); 
		cnt[p.second]++;
		if (cnt[ans] <= cnt[p.second]) ans = p.second; 
		q.pop();
		if (p == nullptr) continue;
		q.push(make_pair<p.first ->leftChild, p.second + 1>);
		q.push(make_pair<p.first ->rightChild, p.second + 1>);
	}
	return ans;
}
//P281.33 时间复杂度为O(N)
template<typename T>
binaryTreeNode<T>* linkedBinaryTree<T>::makeTree(int *preOrderArray, int *inOrderArray, int sz){
	if (sz == 0) return nullptr;
	int pos = 0;
	binaryTreeNode<T> *node = new binaryTreeNode<T>;
	if (sz == 1) {return node;}
	while(pos < sz && preOrderArray[0] != inOrderArray[pos]) pos++;
	node ->leftChild = makeTree(preOrderArray, inOrderArray, pos);
	node ->rightChild = makeTree(preOrderArray + pos + 1, inOrderArray + pos + 1, sz - pos);
	
	if (node ->leftChild != nullptr) node ->leftChild ->father = node;
	if (node ->rightChild != nullptr) node ->rightChild ->father = node;

	return node;
}

//P284.45 时间复杂度为O(N)，使用了lambda表达式，根据先序遍历顺序依次比较
template<typename T>
bool linkedBinaryTree<T>::compare(linkedBinaryTree<T> x){
	tmp = 0;
	std::function<int(binaryTreeNode<T>*, binaryTreeNode<T>*)> f = [&](binaryTreeNode<T>* x, binaryTreeNode<T>* y){
		int k = x->element == y->element;
		if ((x->leftChild && y->leftChild)|| x->leftChild == y->leftChild) k &= f(x->leftChild, y->leftChild);
		else k = 0;
		if ((x->rightChild && y->rightChild) || x->rightChild == y->rightChild) k &= f(x->rightChild, y->rightChild);
		else k = 0;
		return k;
	};
	return f(root, x);
}
int main(){

}