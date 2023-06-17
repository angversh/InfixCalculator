#pragma once
#include <iostream> 
#include <string>
#include <limits>
#include "C:\Users\User\source\repos\List\List\List.cpp"

using namespace std;

class MyStack : public MyList {
public:
	MyStack() {
		HeadNode = NULL;
	}
	~MyStack() {
		Clear();
	}
	int EmptyStack() {
		return IsEmpty();
	}
	int StackSize() {
		return ListSize();
	}
	void Clear() {
		DeleteList();
	}
	void Push(ListEl* a, Node*& HeadNode) {
		AddEnd(a, HeadNode);
	}
	void Pop() {
		DeletePosition(StackSize() - 1, HeadNode);
	}
	ListEl* Top() {
		return FindPosition(StackSize() - 1);
	}

};

class MyQueue : public MyList {
public:
	MyQueue() {
		HeadNode = NULL;
	}
	~MyQueue() {
		Clear();
	}
	void Clear() {
		DeleteList();
	}
	int EmptyQueue() {
		return IsEmpty();
	}
	int QueueSize() {
		return ListSize();
	}
	void Push(ListEl* a, Node*& HeadNode) {
		AddEnd(a, HeadNode);
	}
	void Pop() {
		DeletePosition(0, HeadNode);
	}
	ListEl* Head() {
		return FindPosition(0);
	}
	ListEl* Tail() {
		return FindPosition(QueueSize() - 1);
	}
};

class PostfixCalc {
public:
	double res, ans;
	int ErrCode;
	bool IsCorrect, IsOverflow, IsDBZ;
	PostfixCalc() {
		this->IsCorrect = true;
		this->IsOverflow = false;
		this->IsDBZ = false;
		this->res = 0;
		this->ans = 0;
		this->ErrCode = 0;
	}
	void MakeOp(double a, double b, char op) {
		switch (op) {
		case '+':
			if ((b > 0 && a > std::numeric_limits<int>::max() - b) || (b < 0 && a < std::numeric_limits<int>::min() - b)) {
				this->ErrCode = 1;
			}
			else {
				this->res = a + b;
			}
			break;
		case '-':
			if ((b < 0 && a > std::numeric_limits<int>::max() + b) || (b > 0 && a < std::numeric_limits<int>::min() + b)) {
				this->ErrCode = 1;
			}
			else {
				this->res = a - b;
			}
			break;
		case '*':
			if (a == 0 || b == 0) {
				this->res = 0;
			}
			else if ((b > 0) && ((a > std::numeric_limits<int>::max() / b) || (a < std::numeric_limits<int>::min() / b))) {
				this->ErrCode = 1;
			}
			else if ((b < 0) && ((a < std::numeric_limits<int>::max() / b) || (a > std::numeric_limits<int>::min() / b))) {
				this->ErrCode = 1;
			}
			else {
				this->res = a * b;
			}
			break;
		case '/':
			if (b == 0) {
				this->ErrCode = 2;
			}
			else if ((b > 0) && ((a > std::numeric_limits<int>::max() * b) || (a < std::numeric_limits<int>::min() * b))) {
				this->ErrCode = 1;
			}
			else if ((b < 0) && ((a < std::numeric_limits<int>::max() * b) || (a > std::numeric_limits<int>::min() * b))) {
				this->ErrCode = 1;
			}
			else {
				this->res = a / b;
			}
			break;
		}
	}
	void ClearOps() {
		this->res = 0;
		this->ErrCode = 0;
	}
	void CountExpr(MyStack& Stack, MyQueue& Queue) {
		double num1, num2;
		Stack.Clear();
		ClearOps();
		while (!Queue.EmptyQueue()) {
			if (dynamic_cast<DoubleEl*>(Queue.Head()) != NULL) {
				Stack.Push(dynamic_cast<DoubleEl*>(Queue.Head()), Stack.HeadNode);
				Queue.Pop();
			}
			else {
				if (Stack.EmptyStack()) {
					IsCorrect = false;
					return;
				}
				else {
					num2 = dynamic_cast<DoubleEl*>(Stack.Top())->getElement();
					Stack.Pop();
				}
				if (Stack.EmptyStack()) {
					IsCorrect = false;
					return;
				}
				else {
					num1 = dynamic_cast<DoubleEl*>(Stack.Top())->getElement();
					Stack.Pop();
				}
				MakeOp(num1, num2, dynamic_cast<CharEl*>(Queue.Head())->getElement());
				if (ErrCode == 0) {
					Stack.Push(new DoubleEl(res), Stack.HeadNode);
					ClearOps();
					Queue.Pop();
				}
				else if (ErrCode == 1) {
					IsOverflow = true;
					ClearOps();
					return;
				}
				else {
					IsDBZ = true;
					ClearOps();
					return;
				}
			}
		}
		if (Stack.StackSize() != 1) {
			IsCorrect = 0;
		}
		else {
			ans = dynamic_cast<DoubleEl*>(Stack.Top())->getElement();
		}
	}
	void ClearCalc() {
		this->IsCorrect = true;
		this->IsOverflow = false;
		this->IsDBZ = false;
		this->res = 0;
		this->ans = 0;
		this->ErrCode = 0;
	}
};