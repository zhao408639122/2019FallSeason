#include<cstdio>
#include<iostream>
#include<stdexcept>
#include<cstdlib>
#include<algorithm>
#include<cstring>
#include<functional>
#include<queue>

std::ostream& operator << (std::ostream &os, std::pair<int, int> k) {
	os << k.second;
	return os;
}
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

	//求树高
	int height() const {return height(root);}

protected:
	static int tmp;
	static T* tmp1;
	binaryTreeNode<T>* root;
	static void (*visit)(binaryTreeNode<T>*);
	static void preOrder(binaryTreeNode<T>*);
	static void inOrder(binaryTreeNode<T>*);
	static void postOrder(binaryTreeNode<T>*);
	static void levelOrder(binaryTreeNode<T>*);
	static void dispose(binaryTreeNode<T>* t) {delete t;}
	static int size(binaryTreeNode<T>*);
	static int height(binaryTreeNode<T>*);
	static void output(binaryTreeNode<T>* t) {std::cout << t -> element << ' ';}
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
	std::queue<binaryTreeNode<T>* >q;
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
template<typename T>
int linkedBinaryTree<T>::tmp = 0;
template<typename T>
T* linkedBinaryTree<T>::tmp1 = nullptr;
template<typename T>
void (*linkedBinaryTree<T>::visit)(binaryTreeNode<T>*) = linkedBinaryTree<T>::output;

template<typename T>
class binarySearchTree : virtual public linkedBinaryTree<std::pair<int, T> > {
public:
	typedef binaryTreeNode<std::pair<int, T> > node;

	binarySearchTree() {this ->root = nullptr;}
	~binarySearchTree() {}

	bool empty() const {return linkedBinaryTree<std::pair<int, T> >::empty();}
	int size() const {return linkedBinaryTree<std::pair<int, T> >::size();}
	int height() const {return linkedBinaryTree<std::pair<int, T> >::height();}

	int find(const T&);
	int find_by_rank(int);

	void insert(const T& element) {insert(this ->root, element); }

	void erase(const T&);
	void erase_by_rank(int);

	void ascend() {this ->inOrder(output); std::cout << std::endl;}
	void Aascend() {this ->preOrder(output); std::cout << std::endl;}
protected:
	static int insert(node*&, const T&);
	static node* erase(node*, const T&);
	static node* erase_by_rank(node*, int);
	static void output(node* t) { std::cout << t ->element.second << ' '; }
};

template<typename T>
int binarySearchTree<T>::find(const T& element) {
	node* rt = this ->root;
	int ans = 0;
	while(rt != nullptr){
		ans ^= rt ->element.second;
		// std::cout << rt ->element.second << ' ';
		if (rt ->element.second < element) rt = rt ->rightChild;
		else if (rt ->element.second > element) rt = rt ->leftChild;
		else return ans;
	}
	return -1;
}

template<typename T>
int binarySearchTree<T>::find_by_rank(int rank) {
	node *rt = this ->root;
	int ans = 0;
	while(rt != nullptr){
		ans ^= rt ->element.second;
		// std::cout << rt ->element.second<< ' ';
		if (rt ->element.first + 1 < rank) {
			rank -= rt ->element.first + 1;
			rt = rt ->rightChild;
		}
		else if (rt ->element.first + 1 > rank) rt = rt ->leftChild;
		else return ans;
	}
	return -1;
}

template<typename T>
int binarySearchTree<T>::insert(node*& rt, const T& element) {
	if (rt == nullptr) {
		rt = new node(std::make_pair(0, element));
		return 1;
	}
	if (rt ->element.second < element) {
		if (rt ->rightChild == nullptr) {
			node* new_node = new node(std::make_pair(0, element));
			new_node ->father = rt;
			rt ->rightChild = new_node;
			return 1;
		}
		return insert(rt ->rightChild, element);
	} else if (rt ->element.second > element) {
		if (rt ->leftChild == nullptr) {
			node* new_node = new node(std::make_pair(0, element));
			new_node ->father = rt;
			rt ->leftChild = new_node;
			++rt ->element.first; 
			return 1;
		}
		int add = insert(rt ->leftChild, element);
		rt ->element.first += add;
		return add;
	}
	return 0;
}

template<typename T>
void binarySearchTree<T>::erase(const T& element){
	node *p = erase(this ->root, element);
	node *pp = nullptr;
	if (p != nullptr) {
		if (p ->rightChild != nullptr && p ->leftChild != nullptr) {//两个孩子
			pp = p ->rightChild;
			while(pp ->leftChild != nullptr) {
				--pp ->element.first;
				pp = pp ->leftChild;
			}

			if (pp ->father == p){
				pp ->father ->rightChild = pp ->rightChild;
				if (pp ->rightChild != nullptr) pp ->rightChild ->father = pp ->father;
			} else {
				pp ->father ->leftChild = pp ->rightChild;
				if (pp ->rightChild != nullptr) pp ->rightChild ->father = pp ->father;
			}

			pp ->element.first = p ->element.first;

			pp ->leftChild = p ->leftChild;
			if (pp ->leftChild != nullptr) pp ->leftChild ->father = pp;

			pp ->rightChild = p ->rightChild;
			if (pp ->rightChild != nullptr) pp ->rightChild ->father = pp;

			pp ->father = p ->father;
		}
		//只有一个儿子
		else if (p ->leftChild != nullptr) {
			if (p != this ->root) {
				if (p ->father ->leftChild == p) p ->father ->leftChild = p ->leftChild;
				else p ->father ->rightChild = p ->leftChild;
				if (p ->leftChild != nullptr) p ->leftChild ->father = p ->father;
			} else {
				this ->root = p ->leftChild;
			}
			delete p;
			return;
		} else if (p ->rightChild != nullptr) {
			if (p != this ->root) {
				if (p ->father ->leftChild == p) p ->father ->leftChild = p ->rightChild;
				else p ->father ->rightChild = p ->rightChild;
				if (p ->rightChild != nullptr) p ->rightChild ->father = p ->father; 
			} else {
				this ->root = p ->rightChild;
			}
			delete p;
			return;
		}
		if (p == this ->root) {
			this ->root = pp;
		} else {
			if (p ->father ->leftChild == p) p ->father ->leftChild = pp;
			else p ->father ->rightChild = pp;
		}
		delete p;
	}
}

template<typename T>
binaryTreeNode<std::pair<int, T> >* binarySearchTree<T>::erase(node *rt, const T& element){
	if (rt == nullptr) return nullptr;
	if (rt ->element.second < element) return erase(rt ->rightChild, element);
	else if (rt ->element.second > element) {
		node *tmp = erase(rt ->leftChild, element);
		if (tmp != nullptr) --rt->element.first;
		return tmp;
	} else return rt;
}

template<typename T>
void binarySearchTree<T>::erase_by_rank(int rank) {
	node *p = erase_by_rank(this ->root, rank);
	node *pp = nullptr;
	if (p != nullptr) {
		if (p ->rightChild != nullptr && p ->leftChild != nullptr){ //两个孩子
			pp = p ->rightChild;
			while(pp ->leftChild != nullptr) {
				--pp ->element.first;
				pp = pp ->leftChild;
			}

			if (pp ->father == p) {
				pp ->father ->rightChild = pp ->rightChild;
				if (pp ->rightChild != nullptr) pp ->rightChild ->father = pp ->father;
			} else {
				pp ->father ->leftChild = pp ->rightChild;
				if (pp ->rightChild != nullptr) pp ->rightChild ->father = pp ->father;
			}

			pp ->element.first = p ->element.first;

			pp ->leftChild = p ->leftChild;
			if (pp ->leftChild != nullptr) pp ->leftChild ->father = pp;

			pp ->rightChild = p ->rightChild;
			if (pp ->rightChild != nullptr) pp ->rightChild ->father = pp;

			pp ->father = p ->father;
		}
		// 一个孩子
		else if (p ->leftChild != nullptr) {
			if (p != this ->root) {
				if (p ->father ->leftChild == p) p ->father ->leftChild = p ->leftChild;
				else p ->father ->rightChild = p ->leftChild;
				if (p ->leftChild != nullptr) p ->leftChild ->father = p ->father;
			} else {
				this ->root = p ->leftChild;
			}
			delete p;
			return;
		} else if (p ->rightChild != nullptr) {
			if (p != this ->root) {
				if (p ->father ->leftChild == p) p ->father ->leftChild = p ->rightChild;
				else p ->father ->rightChild = p ->rightChild;
				if (p ->rightChild != nullptr) p ->rightChild ->father = p ->father;
			} else {
				this ->root = p ->rightChild;
			}
			delete p;
			return;
		}
		if (p == this ->root) {
			this ->root = pp;
		} else {
			if (p ->father ->leftChild == p) p ->father ->leftChild = pp;
			else p ->father ->rightChild = pp;
		}
		delete p;
	}
}

template<typename T>
binaryTreeNode<std::pair<int, T> >* binarySearchTree<T>::erase_by_rank(node *rt, int rank) {
	if (rt == nullptr) return nullptr;
	if (rt ->element.first + 1 < rank) return erase_by_rank(rt ->rightChild, rank - 1 - rt ->element.first);
	else if (rt ->element.first + 1 > rank) {
		node* tmp = erase_by_rank(rt ->leftChild, rank);
		if (tmp != nullptr) --rt ->element.first;
		return tmp;
	} else return rt;
}

int main(){
	binarySearchTree<int> Tr;
	int m, ans = 0;
	std::cin>>m;
	while(m--){
		// printf("\n");
		// Tr.Aascend();
		// Tr.ascend();
		int op, x;
		scanf("%d%d", &op, &x);
		if (op == 0){
			ans = Tr.find(x);
			if (ans != -1) printf("0\n");
			else  {
				Tr.insert(x);
				ans = Tr.find(x);
				printf("%d\n", ans ^ x);
			}
		} else if (op == 1) {
			ans = Tr.find(x);
			if (ans == -1) printf("0\n");
			else printf("%d\n", ans);
		} else if (op == 2) {
			ans = Tr.find(x);
			if (ans == -1) printf("0\n");
			else Tr.erase(x), printf("%d\n", ans);
		} else if (op == 3) {
			ans = Tr.find_by_rank(x);
			if (ans == -1) printf("0\n");
			printf("%d\n", ans);
		} else {
			ans = Tr.find_by_rank(x);
			if (ans == -1) printf("0\n");
			else Tr.erase_by_rank(x), printf("%d\n", ans);
		}
	}
}
