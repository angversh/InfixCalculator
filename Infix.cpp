#pragma once
#include <iostream>
#include <string> 
#include <limits> 
#include "List.cpp"
#include "Postfix.cpp"
using namespace std;

class InfixCalc {
public:
    bool IsOverflow, IsBadSeq, IsBadElem, IsCorrect; //indictator of incorrect
    double num;
    bool flag;
    char tmp;
    InfixCalc() {
        this->IsOverflow = false;
        this->IsBadSeq = false;
        this->IsBadElem = false;
        this->IsCorrect = true;
        this->num = 0;
        this->flag = false;
        this->tmp = '\0';
    }
    int OpPriority(char Op) {
        if (Op == '+' || Op == '-') {
            return 1;
        }
        else {
            return 2;
        }
    }
    void IsGood(string s) {
        IsCorrect = true;
        IsBadElem = false;
        if (s.size() == 0) {
            IsCorrect = false;
            return;
        }
        for (int i = 0; i < s.size(); i++) {
            if (!isspace(s[i])) {
                break;
            }
            if (isspace(s[i]) && i == s.size() - 1) { //spaces as seq
                IsCorrect = false;
                return;
            }
        }
        int ind = 0;
        int indLastDig = -1, indLastOp = -1, indLastOpenBr = -1, indLastClosBr = -1;
        while (isspace(s[ind])) {
            ind++;
        }
        if (s[ind] == '+' || s[ind] == '-' || s[ind] == '*' || s[ind] == '/') { // operators start seq
            IsCorrect = false;
            return;
        }
        ind = s.size() - 1;
        while (isspace(s[ind])) {
            ind--;
        }
        if (s[ind] == '+' || s[ind] == '-' || s[ind] == '*' || s[ind] == '/') {
            IsCorrect = false;
            return;
        }
        for (int i = 0; i < s.size(); i++) {
            if (!isdigit(s[i]) && !isspace(s[i]) && s[i] != '(' && s[i] != ')' && s[i] != '+' && s[i] != '-' && s[i] != '*' && s[i] != '/') {
                IsCorrect = false;
                IsBadElem = true;
                return;
            }
            if ((s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/') && indLastOp >= 0 && !(indLastDig > indLastOp && indLastDig < i)) {
                IsCorrect = false;
                return;
            }
            if (isdigit(s[i]) && indLastDig >= 0 && indLastDig != i - 1 && !(indLastOp > indLastDig && indLastOp < i)) {
                IsCorrect = false;
                return;
            }
            if ((s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/') && indLastOpenBr >= 0) {
                if (!(indLastClosBr > indLastOpenBr && indLastClosBr < i) && !(indLastOp > indLastOpenBr && indLastOp < i) && !(indLastDig > indLastOpenBr && indLastDig < i)) {
                    IsCorrect = false;
                    return;
                }
            }
            if (s[i] == ')' && indLastOp >= 0) {
                if (!(indLastOpenBr > indLastOp && indLastOpenBr < i) && !(indLastClosBr > indLastOp && indLastClosBr < i) && !(indLastDig > indLastOp && indLastDig < i)) {
                    IsCorrect = false;
                    return;
                }
            }
            if (s[i] == ')' && indLastOpenBr >= 0) {
                if (!(indLastClosBr > indLastOpenBr && indLastClosBr < i) && !(indLastDig > indLastOpenBr && indLastDig < i) && !(indLastOp > indLastOpenBr && indLastOp < i)) {
                    IsCorrect = false;
                    return;
                }
            }
            if (s[i] == '(' && indLastClosBr >= 0) {
                if (!(indLastOpenBr > indLastClosBr && indLastOpenBr < i) && !(indLastDig > indLastClosBr && indLastDig < i) && !(indLastOp > indLastClosBr && indLastOp < i)) {
                    IsCorrect = false;
                    return;
                }
            }
            if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/') {
                indLastOp = i;
            }
            else if (s[i] == '(') {
                indLastOpenBr = i;
            }
            else if (s[i] == ')') {
                indLastClosBr = i;
            }
            else if (isdigit(s[i])) {
                indLastDig = i; // renew last el
            }
        }
    }
    void InfToPostf(string expr, MyStack& Stack, MyQueue& Queue) {
        num = 0;
        tmp = '\0';
        flag = false;
        IsOverflow = false;
        IsBadSeq = false;  // согласованность скобок
        Stack.Clear();
        Queue.Clear();
        for (int j = 0; j < expr.size(); j++) { // read seq
            if (isdigit(expr[j])) {
                if (num <= std::numeric_limits<int>::max() / 10) {
                    num *= 10;
                }
                else {
                    IsOverflow = true;
                    return;
                }
                if (num <= std::numeric_limits<int>::max() - (expr[j] - '0')) {
                    num += (expr[j] - '0');
                }
                else {
                    IsOverflow = true;
                    return;
                }
                flag = true;
                if (j == expr.size() - 1) {
                    Queue.Push(new DoubleEl(num), Queue.HeadNode);
                    num = 0;
                }
            }
            else if (isspace(expr[j])) {
                if (flag) {
                    Queue.Push(new DoubleEl(num), Queue.HeadNode);
                    num = 0;
                    flag = false;
                }
            }
            else if (expr[j] == '+' || expr[j] == '-' || expr[j] == '*' || expr[j] == '/') {
                if (flag) {
                    Queue.Push(new DoubleEl(num), Queue.HeadNode);
                    num = 0;
                    flag = false;
                }
                if (Stack.EmptyStack() || dynamic_cast<CharEl*>(Stack.Top())->getElement() == '(') {
                    Stack.Push(new CharEl(expr[j]), Stack.HeadNode);
                }
                else if (OpPriority(expr[j]) > OpPriority(dynamic_cast<CharEl*>(Stack.Top())->getElement())) {
                    Stack.Push(new CharEl(expr[j]), Stack.HeadNode);
                }
                else if (OpPriority(expr[j]) <= OpPriority(dynamic_cast<CharEl*>(Stack.Top())->getElement())) {
                    while (!Stack.EmptyStack() && dynamic_cast<CharEl*>(Stack.Top())->getElement() != '(' && OpPriority(dynamic_cast<CharEl*>(Stack.Top())->getElement()) >= OpPriority(expr[j])) {
                        tmp = dynamic_cast<CharEl*>(Stack.Top())->getElement();
                        Queue.Push(new CharEl(tmp), Queue.HeadNode);
                        Stack.Pop();
                    }
                    Stack.Push(new CharEl(expr[j]), Stack.HeadNode); // while stack not empty or till operation of a less prioritized is found
                }
            }
            else if (expr[j] == '(') {
                if (flag) {
                    Queue.Push(new DoubleEl(num), Queue.HeadNode);
                    num = 0;
                    flag = false;
                }
                Stack.Push(new CharEl(expr[j]), Stack.HeadNode);
            }
            else if (expr[j] == ')') {
                if (flag) {
                    Queue.Push(new DoubleEl(num), Queue.HeadNode);
                    num = 0;
                    flag = false;
                }
                while (!Stack.EmptyStack() && dynamic_cast<CharEl*>(Stack.Top())->getElement() != '(') {
                    tmp = dynamic_cast<CharEl*>(Stack.Top())->getElement();
                    Queue.Push(new CharEl(tmp), Queue.HeadNode);
                    Stack.Pop();
                }
                if (Stack.EmptyStack()) {
                    IsBadSeq = true;
                    return;
                }
                else {
                    Stack.Pop();
                }
            }
        }
        while (!Stack.EmptyStack()) {
            if (dynamic_cast<CharEl*>(Stack.Top())->getElement() == '(') {
                IsBadSeq = true;
                return;
            }
            else {
                tmp = dynamic_cast<CharEl*>(Stack.Top())->getElement();
                Queue.Push(new CharEl(tmp), Queue.HeadNode);
                Stack.Pop();
            }
        }
    }
};
int input() {
    long long input;
    while (1) {
        cin >> input;
        if (cin.fail() || cin.peek() != '\n' || abs(input) > std::numeric_limits<int>::max()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            break;
        }
    }
    return int(input);
}
int main() {
    MyStack Stack;
    MyQueue Queue;
    InfixCalc Inf;
    PostfixCalc Postf;
    string expr;
    int cmd;
    cout << "Infix Calculator" << endl;
    cout << "Menu:" << endl;
    cout << "1. Count the expression" << endl;
    cout << "2. Exit" << endl;
    while (1) {
        cmd = input();
        if (cmd == 1) {
            cout << "Enter the expression: " << endl;
            cin.ignore();
            getline(cin, expr);
            cout << "Your expression is: " << expr << endl;
            Inf.IsGood(expr);
            if (!Inf.IsCorrect) {
                cout << "Incorrect expression!" << endl;
                if (Inf.IsBadElem) {
                    cout << "Unsupported element detected!" << endl;
                }
            }
            else {
                Inf.InfToPostf(expr, Stack, Queue);
                if (Inf.IsBadSeq == 0 && Inf.IsOverflow == 0) {
                    cout << "Postfix expression: " << endl;
                    Queue.print();
                    Stack.Clear();
                    Postf.CountExpr(Stack, Queue);
                    if (Postf.IsCorrect == 0) {
                        cout << "Incorrect expression!" << endl;
                    }
                    else if (Postf.IsOverflow) {
                        cout << "Oveflow result!" << endl;
                    }
                    else if (Postf.IsDBZ) {
                        cout << "Division by zero!" << endl;
                    }
                    else {
                        if (Postf.ans == static_cast<int>(Postf.ans)) {
                            cout << "The answer is: " << fixed << static_cast<int>(Postf.ans) << endl;
                        }
                        else {
                            cout << "The answer is: " << defaultfloat << Postf.ans << endl;
                        }
                    }
                    Postf.ClearCalc();
                }
                else {
                    cout << "Invalid expression!" << endl;
                    if (Inf.IsBadSeq == 1) {
                        cout << "Mismatched brackets!" << endl;
                    }
                    if (Inf.IsOverflow == 1) {
                        cout << "Operand oveflow!" << endl;
                    }
                }
            }
        }
        else if (cmd == 2) {
            return 0;
        }
        else {
            cout << "Incorrect menu command!" << endl;
        }
    }
}