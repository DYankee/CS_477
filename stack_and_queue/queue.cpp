#include <cstddef>

struct Node{
    int data;
    Node *next;
    
    Node(int data){
        this->data = data;
        this->next = nullptr;
    }
};

struct Queue{
private:
    Node *first;
    Node *last;

public:
    Queue(){
        first = last = nullptr;
    }

    bool isEmpty(){
        return first == nullptr;
    }

    void enqueue(int data){
        Node *oldLast = last;
        last = new Node(data);
        if (isEmpty()){
            first = last;
        }
        else{
            oldLast->next = last;
        }
    }

    int dequeue(){
        int data = first->data;
        first = first->next;

        if (isEmpty()){
            last = nullptr;
        }
        return data;
    }
};