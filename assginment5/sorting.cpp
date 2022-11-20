#include "sorting.h"
#include "listnode.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment about your implementation of the function.
 */
//添加一个节点的函数
void addNode(ListNode*& initList,ListNode* &tail,ListNode* & tempNode){
    if(initList==nullptr){
        initList=tempNode;
        tail=tempNode;
    }
    else{
        tail->next=tempNode;
        tail=tempNode;
    }
}
//分割函数
void decomposePar(ListNode* &initList,ListNode* &smallerPar,ListNode* &biggerPar,ListNode*&midList){
    if(initList!=nullptr){
        int mid=initList->data;
        //标注尾节点
        ListNode* smallerTail = smallerPar;
        ListNode* biggerTail = biggerPar;
        ListNode* midTail = midList;
        //在三个头节点加入相应的元素
        while(initList!=nullptr){
            if(initList->data>mid){
                addNode(biggerPar,biggerTail,initList);
            }
            else if(initList->data<mid){
                addNode(smallerPar,smallerTail,initList);
            }
            else{
                addNode(midList,midTail,initList);
            }
            //移动
            initList=initList->next;
        }
        //尾节点的next置为空
        if(smallerTail!=nullptr){
            smallerTail->next=nullptr;
        }
        if(biggerTail!=nullptr){
            biggerTail->next=nullptr;
        }
        if(midTail!=nullptr){
            midTail->next=nullptr;
        }
    }
}
//两链表合并,左大右小
void mergeList(ListNode*& first,ListNode*& second){
    if(first==nullptr){
        first=second;
    }
    else{
        ListNode* flag=first;
        while(flag!=nullptr){
            if(flag->next==nullptr){
                flag->next=second;
                break;
            }
            else{
                flag=flag->next;
            }
        }
    }
}
//合并函数
void reconstructPar(ListNode* &initList,ListNode* &smallerPar,ListNode* &biggerPar,ListNode*&midList){

    //分别合并三路
    mergeList(initList,smallerPar);
    smallerPar=nullptr;
    mergeList(initList,midList);
    midList=nullptr;
    mergeList(initList,biggerPar);
    biggerPar=nullptr;
}

void quickSort(ListNode*& front) {
    //base case
    if (front==nullptr){
        return ;
    }

    //分割
    //初始化
    ListNode* smallerPar=nullptr,* biggerPar=nullptr,* midList=nullptr;
    decomposePar(front,smallerPar,biggerPar,midList);
    //递归
    quickSort(smallerPar);
    quickSort(biggerPar);
    //合并
    reconstructPar(front,smallerPar,biggerPar,midList);
}


/* * * * * * Test Code Below This Point * * * * * */

/* TODO: Write your own student tests below to test your sorting helper
 * functions and overall sorting algorithm correctness. We have provided
 * the skeleton of some tests below to help you get started with this
 * process.
 */

/*
 * We have provided this utility function as a convenience. This
 * function traverses the linked list represented by the provided
 * front pointer and prints out the data stored in the list along
 * the way in a convenient format. You may find it to be helpful to
 * use this function as a debugging aid.
 */
void printList(ListNode* front) {
   cout << "{";
   for (ListNode *cur = front; cur != nullptr; cur = cur->next) {
       cout << cur->data;
       if (cur->next != nullptr){
           cout << ", ";
       }
   }
   cout << "}" << endl;
}

/*
 * This utility function deallocates the memory for all the nodes in a
 * given linked list. It can be used to recycle the memory allocated
 * during a test case and avoid memory leaks.
 */
void deallocateList(ListNode* front) {
    while(front!=nullptr){
        ListNode* next=front->next;
        int ele=front->data;
        delete front;
        cout<<"the list of "<<ele<< " has been deleted!"<<endl;
        front=next;
    }

}

/*
 * This utility function is given a vector of values and constructs
 * an equivalent linked list containing the same values in the same order.
 * It can be used to construct a linked list to use as an input to
 * a function being tested.
 */
ListNode* createList(Vector<int> values){
    ListNode* front=nullptr;
    ListNode* flag=front;
    for(int ele:values){
        ListNode* tempNode = new ListNode(ele,nullptr);
        if(front==nullptr){
            front=tempNode;
        }
        else{
            flag->next=tempNode;
        }
        flag=tempNode;
    }
    return front;
}

/*
 * This utility function compares a linked list and a vector for
 * equivalence and returns true if both contain the same values in the
 * same order. It can be used to confirm a linked list's contents match
 * the expected.
 */
bool areEquivalent(ListNode* front, Vector<int> v){
    for(int i=0 ;i<v.size();i++){
        if(front==nullptr){
            return false;
        }
        if(front->data==v[i]){
            front=front->next;
        }
    }
    if(front==nullptr){
        return true;
    }
    return false;
}

STUDENT_TEST("端到端测试：空集") {
    /* Initialize vector to sequence of values */
    Vector<int> values = {};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("端到端测试：单元素集") {
    /* Initialize vector to sequence of values */
    Vector<int> values = {1};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("端到端测试：逆序") {
    /* Initialize vector to sequence of values */
    Vector<int> values;
    for (int i = 100; i > 0; i--) {
        values.add(i);
    }
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("端到端测试：正序") {
    /* Initialize vector to sequence of values */
    Vector<int> values;
    for (int i = 0; i < 100; i++) {
        values.add(i);
    }
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("端到端测试：重复值") {
    /* Initialize vector to sequence of values */
    Vector<int> values;
    for (int i = 0; i < 100; i++) {
        values.add(3);
    }
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("端到端测试：正负值") {
    /* Initialize vector to sequence of values */
    Vector<int> values;
    for (int i = 0; i < 100; i++) {
        values.add(randomInteger(-100, 100));
    }
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("端到端测试：随机值") {
    /* Initialize vector to sequence of values */
    Vector<int> values;
    for (int i = 0; i < 100; i++) {
        values.add(randomInteger(0, 100));
    }
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

PROVIDED_TEST("This tests some of the testing helper functions you are recommended to implement.") {
    /* Creates a small test list containing the values 1->2->3. */
    ListNode* testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    /* Test equality comparison function. */
    EXPECT(areEquivalent(testList, {1, 2, 3}));
    EXPECT(!areEquivalent(testList, {1, 2}));
    EXPECT(!areEquivalent(testList, {1, 2, 3, 4}));
    EXPECT(!areEquivalent(testList, {1, 3, 2}));

    /* Test create list function. */
    ListNode* studentList = createList({1, 2, 3, 4});
    printList(studentList);
    ListNode* cur = studentList;
    for (int i = 1; i <= 4; i++){
        EXPECT(cur != nullptr);
        EXPECT_EQUAL(cur->data, i);
        cur = cur->next;
    }

    /* Test deallocate list functions. There should be no memory leaks from this test. */
    deallocateList(studentList);
    deallocateList(testList);
}

/*
 * The provided test case demonstrates a comparsion between the same
 * algorithm (quicksort) operating on a linked list versus a vector.
 */
PROVIDED_TEST("Time linked list quicksort vs vector quicksort") {
    int startSize = 50;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */

        deallocateList(list);
    }
}


