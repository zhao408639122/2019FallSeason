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
	int height, size;
	binaryTreeNode<T>* leftChild;
	binaryTreeNode<T>* rightChild;
	binaryTreeNode<T>* father;

	binaryTreeNode() {height = 0, size = 0;}
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
	linkedBinaryTree(binaryTreeNode<T>* node = nullptr) {root = node;}
	virtual ~linkedBinaryTree() {}

	bool operator <(const linkedBinaryTree<T>& x) const{
		return root ->element < x.root->element;
	}
	
	void size() const {
		visit = linkedBinaryTree<T>::size;
		postOrder(root);
	}
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

	//求树高
	void height() const {
		visit = linkedBinaryTree<T>::height;
		postOrder(root);
	}

	binaryTreeNode<T>* makeTree(int*, int*, int);
protected:
	int tmp;
	int tmpArray[100010];
	T *tmp1;
	binaryTreeNode<T> *root;
	static void (*visit)(binaryTreeNode<T>*);
	static void preOrder(binaryTreeNode<T>*);
	static void inOrder(binaryTreeNode<T>*);
	static void postOrder(binaryTreeNode<T>*);
	static void levelOrder(binaryTreeNode<T>*);
	static void dispose(binaryTreeNode<T>* t) {delete t;}
	static void size(binaryTreeNode<T>*);
	static void height(binaryTreeNode<T>*);
	static void output(binaryTreeNode<T>* t) {cout << t -> element << ' ';}
};
template<typename T>
void (*linkedBinaryTree<T>::visit)(binaryTreeNode<T>*) = linkedBinaryTree<T>::output;
template<typename T>
void linkedBinaryTree<T>::preOrder(binaryTreeNode<T>* t){
	if (t == nullptr) return;
	linkedBinaryTree<T>::visit(t);
	preOrder(t ->leftChild);
	preOrder(t ->rightChild);
}

template<typename T>
void linkedBinaryTree<T>::inOrder(binaryTreeNode<T>* t){
	if (t == nullptr) return;
	inOrder(t ->leftChild);
	linkedBinaryTree<T>::visit(t);
	inOrder(t ->rightChild);
}

template<typename T>
void linkedBinaryTree<T>::postOrder(binaryTreeNode<T>* t) {
	if (t == nullptr) return;
	postOrder(t ->leftChild);
	postOrder(t ->rightChild);
	linkedBinaryTree<T>::visit(t);
}

template<typename T>
void linkedBinaryTree<T>::levelOrder(binaryTreeNode<T>* t) {
	queue<binaryTreeNode<T>* >q;
	q.push(t);
	while(!q.empty()){
		binaryTreeNode<T>* p = q.front(); q.pop();
		if (p == nullptr) continue;
		linkedBinaryTree<T>::visit(p);
		q.push(p ->leftChild);
		q.push(p ->rightChild);
	}
}

template<typename T>
void linkedBinaryTree<T>::height(binaryTreeNode<T>* t){
	t ->height = 1;
	if (t->leftChild) t->height = max(t->height, t->leftChild->height + 1);
	if (t->rightChild) t->height = max(t->height, t->rightChild->height + 1);	
}
template<typename T>
void linkedBinaryTree<T>::size(binaryTreeNode<T>* t){
	t ->size = 1;
	if (t ->leftChild) t ->size += t ->leftChild ->size;
	if (t ->rightChild) t ->size += t ->rightChild ->size;
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
int n;
binaryTreeNode<int>* tr;
linkedBinaryTree<int> init(){
	int lson, rson;
	scanf("%d", &n);
	tr = new binaryTreeNode<int>[n + 1];
	for (int i = 1; i <= n; ++i){
		tr[i].element = i;
		scanf("%d%d", &lson, &rson);
		if (lson == -1) tr[i].leftChild = nullptr;
		else tr[i].leftChild = tr + lson, tr[lson].father = tr + i;
		if (rson == -1) tr[i].rightChild = nullptr;
		else tr[i].rightChild = tr + rson, tr[rson].father = tr + i; 
	}
	linkedBinaryTree<int> res(tr + 1);
	return res;
}

int main(){
	linkedBinaryTree<int> tree = init();
	int *a;
	a = new int[10];
	// delete (a + 2);
	tree.levelOrder();
	tree.height();
	tree.size();
	cout<<endl;
	for (int i = 1; i <= n; ++i)
		printf("%d ", tr[i].size);
	cout<<endl;
	for (int i = 1; i <= n; ++i)
		printf("%d ", tr[i].height);
	cout<<endl;

}