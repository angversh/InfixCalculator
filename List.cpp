
#include <iostream> 

using namespace std;

class ListEl {
public:
	virtual ~ListEl() {}
	virtual bool isEqual(const ListEl* other) const = 0;
};
class DoubleEl : public ListEl {
private:
	double element;
public:
	DoubleEl(double el) {
		element = el;
	}
	bool isEqual(const ListEl* other) const override {
		const DoubleEl* doubleEl = dynamic_cast<const DoubleEl*>(other);
		return doubleEl != NULL && doubleEl->element == element;
	}
	double getElement() const {
		return element;
	}
};

class CharEl : public ListEl {
private:
	char element;
public:
	CharEl(char el) {
		element = el;
	}
	bool isEqual(const ListEl* other) const override {
		const CharEl* charEl = dynamic_cast<const CharEl*>(other);
		return charEl != NULL && charEl->element == element;
	}
	char getElement() const {
		return element;
	}
};

class Node {
public:
	ListEl* data;
	Node* next;
	Node(ListEl* data) {
		this->data = data;
		this->next = NULL;
	}
};

class MyList {
public:
	Node* HeadNode;
	MyList() {
		HeadNode = NULL;
	}
	~MyList() {
		while (HeadNode != NULL) {
			DeletePosition(0, HeadNode);
		}
	}
	int IsEmpty() {
		if (HeadNode == NULL) {
			return 1;
		}
		else {
			return 0;
		}
	}
	void AddEnd(ListEl* a, Node*& HeadNode) {
		if (IsEmpty()) {
			Node* node = new Node(a);
			HeadNode = node;
		}
		else {
			Node* node = new Node(a);
			Node* tmp = HeadNode;
			while (1) {
				if ((*tmp).next != NULL) {
					tmp = (*tmp).next;
				}
				else {
					(*tmp).next = node;
					break;
				}
			}
		}
	}
	void AddBegin(ListEl* a, Node*& HeadNode) {
		if (IsEmpty()) {
			Node* node = new Node(a);
			HeadNode = node;
		}
		else {
			Node* node = new Node(a);
			node->next = HeadNode;
			HeadNode = node;
		}
	}
	void AddPosition(ListEl* a, int pos) {
		if (pos == 0) {
			AddBegin(a, HeadNode);
		}
		if (pos < 0) {
			return;
		}
		if (pos > 0) {
			if (IsEmpty()) {
				return;
			}
			else {
				Node* tmp = HeadNode;
				int iter = -1;
				while (iter < pos - 2) {
					if ((*tmp).next == NULL) {
						return;
					}
					iter++;
					tmp = (*tmp).next;
				}
				if ((*tmp).next == NULL) {
					AddEnd(a, HeadNode);
					return;
				}
				else {
					Node* node = new Node(a);
					node->next = (*tmp).next;
					(*tmp).next = node;
				}
			}
		}
	}
	void DeletePosition(int pos, Node*& HeadNode) {
		if ((pos < 0) || IsEmpty()) {
			return;
		}
		if (pos == 0) {
			Node* tmp = (*HeadNode).next;
			delete HeadNode;
			HeadNode = tmp;
		}
		if (pos > 0) {
			Node* tmp = HeadNode;
			int iter = -1;
			while (iter < pos - 2) {
				if ((*tmp).next == NULL) {
					return;
				}
				iter++;
				tmp = (*tmp).next;
			}
			if ((*tmp).next == NULL) {
				return;
			}
			else {
				Node* tmp1 = (*((*tmp).next)).next;
				delete (*tmp).next;
				(*tmp).next = tmp1;
			}
		}
	}
	void DeleteValue(ListEl* value, Node*& HeadNode) {
		if (IsEmpty()) {
			return;
		}
		Node* tmp = HeadNode;
		int iter = -1;
		int deleted = 0;
		while (tmp != NULL) {
			if (tmp->data->isEqual(value)) {
				tmp = (*tmp).next;
				DeletePosition(iter + 1 - deleted, HeadNode);
				deleted++;
				iter++;
			}
			else {
				tmp = (*tmp).next;
				iter++;
			}
		}
	}
	ListEl* FindPosition(int pos) {
		Node* tmp = HeadNode;
		int iter = -1;
		while (tmp != NULL) {
			if (iter + 1 != pos) {
				iter++;
				tmp = (*tmp).next;
			}
			else {
				return (*tmp).data;
			}
		}
		return NULL;
	}
	int ListSize() {
		if (IsEmpty()) {
			return 0;
		}
		else {
			int size = 0;
			Node* tmp = HeadNode;
			while (tmp != NULL) {
				tmp = (*tmp).next;
				size++;
			}
			return size;
		}
	}
	void print() {
		double d;
		Node* tmp = HeadNode;
		if (tmp == NULL) {
			cout << "EmptyList" << endl;
		}
		else {
			while (tmp != NULL) {
				if (dynamic_cast<DoubleEl*>(tmp->data) != NULL) {
					d = dynamic_cast<DoubleEl*>(tmp->data)->getElement();
					if (d == static_cast<int>(d)) {
						cout << static_cast<int>(d) << " ";
					}
					else {
						cout << d << " ";
					}
				}
				else {
					cout << dynamic_cast<CharEl*>(tmp -> data) -> getElement() << " ";
				}
				tmp = (*tmp).next;
			}
			cout << endl;
		}
	}
	void DeleteList() {
		while (HeadNode != NULL) {
			DeletePosition(0, HeadNode);
		}
	}
};