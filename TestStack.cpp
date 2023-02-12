#include"LockFreeStack.h"
#include<iostream>
#include<thread>
#include<mutex>
using namespace std;
using namespace mpcs51044;

int main() {
	Stack s;
	s.push(10);
	s.push(8);
	cout << s.pop();
	return 0;
}

