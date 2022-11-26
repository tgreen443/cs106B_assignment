#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 5;
const int NONE = -1; // used as sentinel index

PQHeap::PQHeap() {
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[_numAllocated](); // allocated zero'd memory
    _numFilled = 0;
}

PQHeap::~PQHeap() {
    delete[] _elements;
}

void PQHeap::expandArray(){
    _numAllocated *=2;
    auto _oldElements=_elements;
    _elements = new DataPoint[_numAllocated];
    for(int i=0;i<_numFilled;i++){
        _elements[i] = _oldElements[i];
    }
    delete[] _oldElements;
}

void PQHeap::enqueue(DataPoint elem) {

    if(_numAllocated == _numFilled){
        expandArray();
    }
    _elements[_numFilled]= elem;
    int i =_numFilled;
    while(i>0){
        int p=getParentIndex(i);
        if(_elements[p].priority>_elements[i].priority){
            swap(i,getParentIndex(i));
        }
        i=p;
    }
    _numFilled++;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
DataPoint PQHeap::peek() const {
    if(isEmpty()){
        error("the PQHeap is empty!");
    }
    return _elements[0];
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
void PQHeap::reconstructHeap(int node){
    int ln=getLeftChildIndex(node);
    int rn=getRightChildIndex(node);
    //左侧为0，说明不含子节点
    if(ln==NONE){
        return ;
    }
    else if(rn == NONE){
        if(_elements[ln].priority<_elements[node].priority){
            swap(node,ln);
        }
        return ;
    }
    int minnode =(_elements[ln].priority < _elements[rn].priority) ? ln : rn;
    if(_elements[minnode].priority<_elements[node].priority){
        swap(minnode,node);
    }
    reconstructHeap(minnode);
}
DataPoint PQHeap::dequeue() {
    DataPoint maxDatapoint = peek();
    _elements[0]=_elements[_numFilled-1];
    _numFilled--;
    reconstructHeap(0);
    return maxDatapoint;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
bool PQHeap::isEmpty() const {
    return size()==0;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
int PQHeap::size() const {
    return _numFilled;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
void PQHeap::clear() {
    _numFilled=0;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
void PQHeap::printDebugInfo(string msg) const {
    cout << msg << endl;
    for (int i = 0; i < size(); i++) {
        cout << "[" << i << "] = " << _elements[i] << endl;
    }
}

/*
 * We strongly recommend implementing this helper function, which
 * should traverse the heap array and ensure that the heap property
 * holds for all elements in the array. If elements are found that
 * violate the heap property, an error should be thrown.
 */
void PQHeap::validateInternalState() const {
   if (_numFilled > _numAllocated) {
       error("Too many elements in not enough space!");
    }
   validateHelper(0);
}
void PQHeap::validateHelper(int parent) const {
    int left = getLeftChildIndex(parent);
    int right = getRightChildIndex(parent);
    // No child node
    if (left == NONE) {
        return;
    }
    // Only left child node
    if (right == NONE) {
        if (_elements[parent].priority > _elements[left].priority) {
            printDebugInfo("validateInternalState");
            error("PQHeap is not valid at index " + integerToString(parent));
        }
        return;
    } else {
        // Two child nodes
        if (_elements[parent].priority > _elements[left].priority ||
            _elements[parent].priority > _elements[right].priority) {
            printDebugInfo("validateInternalState");
            error("PQHeap is not valid at index " + integerToString(parent));
        }
        validateHelper(left);
        validateHelper(right);
    }
}

// 随机交换一个节点，用于验证 validateInternalState 接口
void PQHeap::validateHelperTest() {
    while (true) {
        int parent = randomInteger(0, this->size());
        int minChild = randomBool() ? getLeftChildIndex(parent) : getRightChildIndex(parent);
        if (minChild != NONE && _elements[parent].priority < _elements[minChild].priority) {
            swap(parent, minChild);
            break;
        }
    }
}

int PQHeap::getParentIndex(int child) const {   
    int p=(child-1)/2;
    return child>=1 ? p:NONE;
}


int PQHeap::getLeftChildIndex(int parent) const {
    int lc=2*parent+1;
    return lc<_numFilled ? lc:NONE;
}

int PQHeap::getRightChildIndex(int parent) const {
    int rc=2*parent+2;
    return rc<_numFilled ? rc:NONE;
}
void PQHeap::swap(int indexA, int indexB) {
    DataPoint tmp = _elements[indexA];
    _elements[indexA] = _elements[indexB];
    _elements[indexB] = tmp;
}
/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Add your own custom tests here! */


/* * * * * Provided Tests Below This Point * * * * */
void fillVector(PQHeap& pq, int n) {
    pq.clear();
    for (int i = 0; i < n; i++) {
        DataPoint pt = {"", (double)randomInteger(0, 100)};
        pq.enqueue(pt);
    }
}
STUDENT_TEST("PQHeap, 测试 validateInternalState") {
    for (int i = 0; i < 5; i++) {
        PQHeap pq;
        fillVector(pq, 10);
        pq.validateHelperTest();
        EXPECT_ERROR(pq.validateInternalState());
    }
}

STUDENT_TEST("PQHeap, enqueue only, validate at every step") {
    PQHeap pq;
    pq.enqueue({"e", 2.718});
    pq.validateInternalState();
    pq.enqueue({"pi", 3.14});
    pq.validateInternalState();
    pq.enqueue({"phi", 1.618});
    pq.validateInternalState();
    EXPECT_EQUAL(pq.size(), 3);
    pq.dequeue();
    pq.validateInternalState();
    pq.dequeue();
    pq.validateInternalState();
    pq.dequeue();
    pq.validateInternalState();
    EXPECT_EQUAL(pq.size(), 0);
}

STUDENT_TEST("PQHeap, enqueue random value and validate it") {
    PQHeap pq;
    fillVector(pq, 30);
    pq.validateInternalState();
    EXPECT_EQUAL(pq.size(), 30);
    for (int i = 30; i > 0; i--) {
        pq.dequeue();
    }
    pq.validateInternalState();
    EXPECT_EQUAL(pq.size(), 0);
}
PROVIDED_TEST("PQHeap example from writeup, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {{"R", 4}, {"A", 5}, {"B", 3}, {"K", 7}, {"G", 2},
                               {"V", 9}, {"T", 1}, {"O", 8}, {"S", 6}};

    pq.validateInternalState();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }
    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
}

PROVIDED_TEST("PQHeap example from writeup") {
    PQHeap pq;

    pq.enqueue({"Zoe", -3});
    pq.enqueue({"Elmo", 10});
    pq.enqueue({"Bert", 6});
    EXPECT_EQUAL(pq.size(), 3);
    pq.printDebugInfo("After enqueue 3 elements");

    pq.enqueue({"Kermit", 5});
    EXPECT_EQUAL(pq.size(), 4);
    pq.printDebugInfo("After enqueue one more");

    DataPoint removed = pq.dequeue();
    DataPoint expected = {"Zoe", -3};
    EXPECT_EQUAL(removed, expected);
    pq.printDebugInfo("After dequeue one");
}

PROVIDED_TEST("PQHeap, enqueue only, validate at every step") {
    PQHeap pq;

    pq.enqueue({"e", 2.718});
    pq.validateInternalState();
    pq.enqueue({"pi", 3.14});
    pq.validateInternalState();
    pq.enqueue({"phi", 1.618});
    pq.validateInternalState();
    EXPECT_EQUAL(pq.size(), 3);
}

PROVIDED_TEST("PQHeap: operations size/isEmpty/clear") {
    PQHeap pq;

    EXPECT(pq.isEmpty());
    pq.clear();
    EXPECT_EQUAL(pq.isEmpty(), pq.size() == 0);
    pq.enqueue({"", 7});
    EXPECT_EQUAL(pq.size(), 1);
    pq.enqueue({"", 5});
    EXPECT_EQUAL(pq.size(), 2);
    pq.enqueue({"", 5});
    EXPECT_EQUAL(pq.size(), 3);
    pq.clear();
    pq.validateInternalState();
    EXPECT(pq.isEmpty());
    EXPECT_EQUAL(pq.size(), 0);
}

PROVIDED_TEST("PQHeap: dequeue or peek on empty queue raises error") {
    PQHeap pq;
    DataPoint point = {"Programming Abstractions", 106};

    EXPECT(pq.isEmpty());
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());

    pq.enqueue(point);
    pq.dequeue();
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());

    pq.enqueue(point);
    pq.clear();
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());
}

PROVIDED_TEST("PQHeap, dequeue, validate at every step") {
    PQHeap pq;

    pq.enqueue({"e", 2.718});
    pq.enqueue({"pi", 3.14});
    pq.enqueue({"phi", 1.618});

    for (int i = 0; i < 3; i++) {
        pq.dequeue();
        pq.validateInternalState();
    }
}

PROVIDED_TEST("PQHeap, test enlarge array memory") {
    for (int size = 5; size <= 500; size *= 5) {
        PQHeap pq;

        for (int i = 1; i <= size; i++) {
            pq.enqueue({"", double(i)});
        }
        pq.validateInternalState();

        for (int i = 1; i <= size; i++) {
            DataPoint expected = {"", double(i)};
            EXPECT_EQUAL(pq.dequeue(), expected);
        }
    }
}

PROVIDED_TEST("PQHeap, sequence of mixed operations") {
    PQHeap pq;
    int size = 30;
    double val = 0;

    for (int i = 0; i < size; i++) {
        pq.enqueue({"", --val});
    }
    val = 0;
    for (int i = 0; i < pq.size(); i++) {
        DataPoint front = pq.peek();
        EXPECT_EQUAL(pq.dequeue(), front);
        pq.enqueue({"", ++val});
    }
    EXPECT_EQUAL(pq.size(), size);
    val = 0;
    while (!pq.isEmpty()) {
        DataPoint expected = {"", ++val};
        EXPECT_EQUAL(pq.dequeue(), expected);
    }
}

PROVIDED_TEST("PQHeap stress test, cycle many random elements in and out") {
    PQHeap pq;
    int n = 0, maxEnqueues = 1000;
    double sumEnqueued = 0, sumDequeued = 0;

    setRandomSeed(42);  // make test behavior deterministic

    DataPoint mostUrgent = {"", 0};
    pq.enqueue(mostUrgent);
    while (true) {
        if (++n < maxEnqueues && randomChance(0.9)) {
            DataPoint elem = {"", randomInteger(-10, 10) + 0.5};
            if (elem.priority < mostUrgent.priority) {
                mostUrgent = elem;
            }
            sumEnqueued += elem.priority;
            pq.enqueue(elem);
        } else {
            DataPoint elem = pq.dequeue();
            sumDequeued += elem.priority;
            EXPECT_EQUAL(elem, mostUrgent);
            if (pq.isEmpty())
                break;
            mostUrgent = pq.peek();
        }
    }
    EXPECT_EQUAL(sumEnqueued, sumDequeued);
}

void fillQueue(PQHeap& pq, int n) {
    pq.clear();  // start with empty queue
    for (int i = 0; i < n; i++) {
        pq.enqueue({"", randomReal(0, 10)});
    }
}

void emptyQueue(PQHeap& pq, int n) {
    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
}

PROVIDED_TEST("PQHeap timing test, fillQueue and emptyQueue") {
    PQHeap pq;
    int n = 20000;

    TIME_OPERATION(n, fillQueue(pq, n));
    TIME_OPERATION(n, emptyQueue(pq, n));
}
