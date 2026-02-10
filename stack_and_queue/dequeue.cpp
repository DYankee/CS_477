#include <iostream>

// My implementation of a double ended queue using a doubly linked list structure

struct DLNode {
    public:
        int data;
        DLNode *next;
        DLNode *previous;

        DLNode(int data){
            this->data = data;
            this->next = nullptr;
            this->next = nullptr;
        }
};

struct DeQueue{
private:
    DLNode *front;
    DLNode *rear;

public:
    // Constructor
    DeQueue() {
        front = rear = nullptr;
    }

    // Check if queue is empty
    bool isEmpty() {return front == nullptr;}

    // Add item to front of the queue
    void insertFront(int data){
        DLNode *newNode = new DLNode(data);

        if (isEmpty()){front = rear = newNode;}
        else {
            newNode->next = front;
            front->previous = newNode;
            front = newNode;
        }
    }

    // Add item to rear of the queue
    void insertRear(int data){
         DLNode *newNode = new DLNode(data);

        if (isEmpty()){front = rear = newNode;}
        else {
            newNode->next = rear;
            rear->next = newNode;
            rear = newNode;
        }       
    }

    // remove item from front of the queue
    void deleteFront(){
        if (isEmpty()){std::cout << "Already empty!\n";}
        else {
            front = front->next;
            
            if (front) front->previous = nullptr;
            else rear = nullptr;
        }
    }
    
    // remove item from rear of the queue
    void deleteRear(){
        if (isEmpty()){std::cout << "Already empty!\n";}
        else {
            rear = rear->previous;
            if (rear) rear->next = nullptr;
            else front = nullptr;
        }
    }


    int getFront(){
        if(isEmpty()){
            return -1;
        }
        else {
            return front->data;
        }
    }
    
    int getRear(){
        if(isEmpty()){
            return -1;
        }
        else {
            return rear->data;
        }
    }
    
    int popFront(int) {
        int data = getFront();
        deleteFront();
        return data;
    }

    int popRear(int){
        int data = getRear();
        deleteRear();
        return data;
    }
};