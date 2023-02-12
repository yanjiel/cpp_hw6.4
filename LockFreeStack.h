#ifndef LOCK_FREE_STACK_H
#  define LOCK_FREE_STACK_H
#include<atomic>
#include<memory>
#include<iostream>
using std::atomic;

namespace mpcs51044 {
    // Linked list of integers
    struct StackItem {
        StackItem(int val) : next(nullptr), value(val) {} ///////////next(0)
        StackItem* next; // Next item, 0 if this is last
        int value;
        void print() { std::cout << "val: " << value << "; next: " << next << std::endl;  }
    };

    struct StackHead {
        StackItem* link;      // First item, 0 if list empty
        unsigned count;      // How many times the list has changed (see lecture notes)
    };

    struct Stack {
        Stack();
        int pop();
        void push(int);
    private:
        atomic<StackHead> head;
    };

    Stack::Stack()
    {
        StackHead init; // create a StackHead, with the link pointed to nullptr, count = 0
        init.link = nullptr;
        init.count = 0;
        head.store(init); //head atomic<StackHead> will be initialized to a StackHead that's pullptr w count 0
    }

    // Pop value off list
    int
        Stack::pop()
    {
        // What the head will be if nothing messed with it
        StackHead expected = head.load(); //get current StackHead
        StackHead newHead;
        bool succeeded = false;
        while (!succeeded) {
            if (expected.link == nullptr) { /////// == 0
                std::cout << "list empty" << std::endl;
                return 0; // List is empty
            }
            // What the head will be after the pop:
            std::cout << "list not empty" << std::endl;
            newHead.link = expected.link->next;
            newHead.count = expected.count + 1;
            // Even if the compare_exchange fails, it updates expected.
            succeeded = head.compare_exchange_weak(expected, newHead); // compare and then exchange, update expected, and store back into head
        }
        int value = expected.link->value;
        delete expected.link;
        return value;
    }

    // Push an item onto the list with the given head
    void
        Stack::push(int val)
    {
        //StackHead expected = head.load(); //get current StackHead
        //StackHead newHead;
        //bool succeeded = false;
        //while (!succeeded) {
        //    while (item != nullptr) {
        //       item  = expected.link->next;
        //    }
        //    item = new StackItem(val);
        //    newHead.link = expected.link;
        //    newHead.count = expected.count + 1;
        //    //// Even if the compare_exchange fails, it updates expected.
        //    succeeded = head.compare_exchange_weak(expected, newHead); // compare and then exchange, update expected, and store back into head
        //}
        //delete expected.link;

        StackHead expected = head.load(); //get current StackHead
        std::cout << "is head pointer null? : " << (expected.link == nullptr)<< std::endl;
        if (expected.link == nullptr) {
            expected.link = new StackItem(val);
        }
        else {
            StackItem* item = expected.link;
            while (item->next != nullptr) {
                item = expected.link->next;
                //(*item).print();
            }
            item->next = new StackItem(val);
            (*item->next).print();   
        }
        expected.count = expected.count + 1;
        head.store(expected);
        
    }
    
}
#endif