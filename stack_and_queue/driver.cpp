#include "dequeue.cpp"
#include "queue.cpp"

// I dont normally use namespace but
// this is just an example and it helps me
// prototype faster
using namespace std;

// Driver function
int main(){

    cout << "----------------------------------------------------" << endl;
    // single linked list queue implementation
    cout << "Single linked list queue implementation" << endl;
    cout << "----------------------------------------------------" << endl;

    Queue q;

    cout << "Confirm new Queue is empty" << endl;
    cout << "Empty: " << (q.isEmpty() ? "yes" : "no") << endl;
        
    cout << "Test front insertion into an empty queue" << endl;
    q.enqueue(5);
    cout << q.dequeue() << endl;
    cout << "Empty: " << (q.isEmpty() ? "yes" : "no") << endl;
    
    
    cout << "----------------------------------------------------" << endl;
    cout << endl;
    
    cout << "----------------------------------------------------" << endl;
    // Double linked list queue implementation
    cout << "Doubly linked list deQueue implementation" << endl;
    cout << "----------------------------------------------------" << endl;
    DeQueue dq;
    
    cout << "Confirm new DeQueue is empty" << endl;
    cout << "Empty: " << (dq.isEmpty() ? "yes" : "no") << endl;

    cout << "Test front insertion into an empty queue" << endl;
    dq.insertFront(5);
    cout << dq.getFront() << endl;
    cout << "Delete the item we just added and confirm queue is empty" << endl;
    dq.deleteFront();
    cout << "Empty: " << (dq.isEmpty() ? "yes" : "no") << endl;


    cout << "Test rear insertion into an empty queue" << endl;
    dq.insertRear(5);
    cout << dq.getRear() << endl;
    cout << "Delete the item we just added and confirm queue is empty" << endl;
    dq.deleteRear();
    cout << "Empty: " << (dq.isEmpty() ? "yes" : "no") << endl;
    cout << "----------------------------------------------------" << endl;
}