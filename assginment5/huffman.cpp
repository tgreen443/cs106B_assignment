#include "bits.h"
#include "treenode.h"
#include "huffman.h"
#include "map.h"
#include "vector.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
using namespace std;

/**
 * Given a Queue<Bit> containing the compressed message bits and the encoding tree
 * used to encode those bits, decode the bits back to the original message text.
 *
 * You can assume that tree is a well-formed non-empty encoding tree and
 * messageBits queue contains a valid sequence of encoded bits.
 *
 * Your implementation may change the messageBits queue however you like. There
 * are no requirements about what it should look like after this function
 * returns. The encoding tree should be unchanged.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits) {
    string text="";
    while(!messageBits.isEmpty()){
        char temp;
        Bit tempNum;
        auto tempNode=tree;
        while(!tempNode->isLeaf()){
            tempNum=messageBits.dequeue();
            if(tempNum==false){
                tempNode=tempNode->zero;
            }
            else{
                tempNode=tempNode->one;
            }
        }
        temp=tempNode->getChar();
        text += temp;
    }

    return text;
}

/**
 * Reconstruct encoding tree from flattened form Queue<Bit> and Queue<char>.
 *
 * You can assume that the queues are well-formed and represent
 * a valid encoding tree.
 *
 * Your implementation may change the queue parameters however you like. There
 * are no requirements about what they should look like after this function
 * returns.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */



EncodingTreeNode* unflattenTree(Queue<Bit>& treeShape, Queue<char>& treeLeaves) {    
    Bit tempNode=treeShape.dequeue();
    //basecase
    if(tempNode==0){
        EncodingTreeNode* noCharTree=new EncodingTreeNode(treeLeaves.dequeue());
        return noCharTree;
    }
    else{
        EncodingTreeNode* noCharTree=new EncodingTreeNode(unflattenTree(treeShape,treeLeaves),unflattenTree(treeShape,treeLeaves));
        return noCharTree;
    }
}

/**
 * Decompress the given EncodedData and return the original text.
 *
 * You can assume the input data is well-formed and was created by a correct
 * implementation of compress.
 *
 * Your implementation may change the data parameter however you like. There
 * are no requirements about what it should look like after this function
 * returns.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
string decompress(EncodedData& data) {

    //生成树
    EncodingTreeNode* tree=unflattenTree(data.treeShape,data.treeLeaves);
    //解压
    string initStr=decodeText(tree,data.messageBits);
    deallocateTree(tree);
    return initStr;
}

/**
 * Constructs an optimal Huffman coding tree for the given text, using
 * the algorithm described in lecture.
 *
 * Reports an error if the input text does not contain at least
 * two distinct characters.
 *
 * When assembling larger trees out of smaller ones, make sure to set the first
 * tree dequeued from the queue to be the zero subtree of the new tree and the
 * second tree as the one subtree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodingTreeNode* buildHuffmanTree(string text) {
    //生成优先队列
    Map<char,int> map;
    for(char i:text){
        map[i]++;
    }
    PriorityQueue<EncodingTreeNode*> pq;
    for(char i :map){
        auto temp=new EncodingTreeNode(i);
        pq.enqueue(temp,map[i]);
    }
    //
    auto temp=pq.peek();
    for(int i=0;i<map.size()-1;i++){
        auto minP=pq.peekPriority();
        auto min=pq.dequeue();
        auto secondP=pq.peekPriority();
        auto secondMin=pq.dequeue();
        temp=new EncodingTreeNode(min,secondMin);
        pq.enqueue(temp,minP+secondP);
    }

    return temp;
}

/**
 * Given a string and an encoding tree, encode the text using the tree
 * and return a Queue<Bit> of the encoded bit sequence.
 *
 * You can assume tree is a valid non-empty encoding tree and contains an
 * encoding for every character in the text.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
//辅助函数生成映射
void generateTreeMap(EncodingTreeNode* tree,Map<char,string> &treeMap,string &temp){
    //basecase
    if(tree->isLeaf()){
        treeMap.put(tree->getChar(),temp);
        return;
    }
    else{
        temp+='0';
        generateTreeMap(tree->zero,treeMap,temp);
        temp.pop_back();
        temp+='1';
        generateTreeMap(tree->one,treeMap,temp);
        temp.pop_back();
        return;
    }
}

Queue<Bit> encodeText(EncodingTreeNode* tree, string text) {
    //建立映射
    Map<char,string> treeMap;
    string temp="";
    generateTreeMap(tree,treeMap,temp);
    //压缩
    Queue<Bit> encodeText ;
    for(char i : text){
       string temp=treeMap[i];
       for(char c :temp){
           Bit t;
           t=int(c-'0');
           encodeText.enqueue(t);
       }
    }
    return encodeText;
}

/**
 * Flatten the given tree into a Queue<Bit> and Queue<char> in the manner
 * specified in the assignment writeup.
 *
 * You can assume the input queues are empty on entry to this function.
 *
 * You can assume tree is a valid well-formed encoding tree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    //basecase
    if(tree->isLeaf()){
        treeShape.enqueue(0);
        treeLeaves.enqueue(tree->getChar());
    }
    else{
        treeShape.enqueue(1);
        flattenTree(tree->zero,treeShape,treeLeaves);
        flattenTree(tree->one,treeShape,treeLeaves);
    }
}

/**
 * Compress the input text using Huffman coding, producing as output
 * an EncodedData containing the encoded message and flattened
 * encoding tree used.
 *
 * Reports an error if the message text does not contain at least
 * two distinct characters.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodedData compress(string messageText) {
    //1.生成哈夫曼树
    EncodingTreeNode* huffmanTree = buildHuffmanTree(messageText);
    //2.生成扁平树
    Queue<Bit> treeShape={};
    Queue<char> treeLeaves={};
    flattenTree(huffmanTree,treeShape,treeLeaves);
    //3.压缩文本
    Queue<Bit>messageBits=encodeText(huffmanTree,messageText);
    //4.打包
    EncodedData data;
    data.messageBits=messageBits;
    data.treeLeaves=treeLeaves;
    data.treeShape=treeShape;
    deallocateTree(huffmanTree);
    return data;
}

/* * * * * * Testing Helper Functions Below This Point * * * * * */

EncodingTreeNode* createExampleTree() {
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */
    auto t=new EncodingTreeNode('T');
    auto e=new EncodingTreeNode('E');
    auto r=new EncodingTreeNode('R');
    auto s=new EncodingTreeNode('S');
    auto rs=new EncodingTreeNode(r,s);
    auto rse =new EncodingTreeNode(rs,e);
    auto trse =new EncodingTreeNode(t,rse);
    return trse;
}


void deallocateTree(EncodingTreeNode* t) {
    if(t->isLeaf()){
        delete t;
    }
    else{
        deallocateTree(t->zero);
        deallocateTree(t->one);
        delete t;
    }
}

bool areEqual(EncodingTreeNode* a, EncodingTreeNode* b) {
    //basecase
    if(a==nullptr&&b==nullptr){
        return true;
    }
    else{
        if(a->isLeaf()){
            if(b->isLeaf()){
                return a->getChar()==b->getChar();
            }
            else{
                return false;
            }
        }
        else{
            return areEqual(a->zero,b->zero)&&areEqual(a->one,b->one);
        }
    }
}




/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Write your own student tests. */
STUDENT_TEST("测试dellocateTree"){
    auto t=createExampleTree();
    deallocateTree(t);
}
STUDENT_TEST("测试areEqual"){
    auto singleNode=new EncodingTreeNode('A');
    auto zero= new EncodingTreeNode(nullptr,nullptr);
    EXPECT_EQUAL(false,areEqual(singleNode,zero));
    deallocateTree(singleNode);
    deallocateTree(zero);
    auto t=createExampleTree();
    auto tc=createExampleTree();
    auto o=t->one;
    EXPECT_EQUAL(true,areEqual(t,tc));
    EXPECT_EQUAL(false,areEqual(t,o));
    deallocateTree(t);
    deallocateTree(tc);
}
STUDENT_TEST("decodeText测试") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    messageBits = { }; // 空
    EXPECT_EQUAL(decodeText(tree, messageBits), "");

    messageBits = { 0,0,0,0,0,0}; // TTTTTT
    EXPECT_EQUAL(decodeText(tree, messageBits), "TTTTTT");
    Queue<Bit> longMessageBits;
    for(int i=0;i<100;i++){
        longMessageBits.enqueue(1);
        longMessageBits.enqueue(0);
        longMessageBits.enqueue(1);
        longMessageBits.enqueue(1);
        longMessageBits.enqueue(1);
        longMessageBits.enqueue(0);
    }
    string longString;
    for(int i =0;i<100;i++){
        longString+="SET";
    }
    EXPECT_EQUAL(decodeText(tree, longMessageBits), longString);//long set

    deallocateTree(tree);
}
STUDENT_TEST("unflattenTree测试") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  treeShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeLeaves = { 'T', 'R', 'S', 'E' };
    EncodingTreeNode* tree = unflattenTree(treeShape, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

STUDENT_TEST("decompress测试long") {
    EncodedData data = {
        { 1, 0, 1, 1, 0, 0, 0 }, // treeShape
        { 'T', 'R', 'S', 'E' },  // treeLeaves
        { } // messageBits
    };
    Queue<Bit> longMessageBits;
    for(int i=0;i<100;i++){
        longMessageBits.enqueue(1);
        longMessageBits.enqueue(0);
        longMessageBits.enqueue(1);
        longMessageBits.enqueue(1);
        longMessageBits.enqueue(1);
        longMessageBits.enqueue(0);
    }
    string longString;
    for(int i =0;i<100;i++){
        longString+="SET";
    }
    data.messageBits=longMessageBits;

    EXPECT_EQUAL(decompress(data), longString);
}
STUDENT_TEST("encodeText long测试") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit> longMessageBits;
    for(int i=0;i<100;i++){
        longMessageBits.enqueue(1);
        longMessageBits.enqueue(0);
        longMessageBits.enqueue(1);
        longMessageBits.enqueue(1);
        longMessageBits.enqueue(1);
        longMessageBits.enqueue(0);
    }
    string longString;
    for(int i =0;i<100;i++){
        longString+="SET";
    }
    EXPECT_EQUAL(encodeText(reference, longString), longMessageBits);

    deallocateTree(reference);
}
STUDENT_TEST("flattenTree测试") {
    EncodingTreeNode* reference = createExampleTree();
    Queue<Bit>  expectedShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'T', 'R', 'S', 'E' };

    Queue<Bit>  treeShape;
    Queue<char> treeLeaves;
    flattenTree(reference, treeShape, treeLeaves);

    EXPECT_EQUAL(treeShape,  expectedShape);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);
    //反转
    auto tree=unflattenTree(treeShape,treeLeaves);
    EXPECT_EQUAL(true,areEqual(tree,reference));

    deallocateTree(reference);
    deallocateTree(tree);
}



/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("decodeText, small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    deallocateTree(tree);
}

PROVIDED_TEST("unflattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  treeShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeLeaves = { 'T', 'R', 'S', 'E' };
    EncodingTreeNode* tree = unflattenTree(treeShape, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

PROVIDED_TEST("decompress, small example input") {
    EncodedData data = {
        { 1, 0, 1, 1, 0, 0, 0 }, // treeShape
        { 'T', 'R', 'S', 'E' },  // treeLeaves
        { 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1 } // messageBits
    };

    EXPECT_EQUAL(decompress(data), "TRESS");
}

PROVIDED_TEST("buildHuffmanTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    EncodingTreeNode* tree = buildHuffmanTree("STREETTEST");
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);
}

PROVIDED_TEST("encodeText, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(encodeText(reference, "E"), messageBits);

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(encodeText(reference, "SET"), messageBits);

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1 }; // STREETS
    EXPECT_EQUAL(encodeText(reference, "STREETS"), messageBits);

    deallocateTree(reference);
}

PROVIDED_TEST("flattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  expectedShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'T', 'R', 'S', 'E' };

    Queue<Bit>  treeShape;
    Queue<char> treeLeaves;
    flattenTree(reference, treeShape, treeLeaves);

    EXPECT_EQUAL(treeShape,  expectedShape);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    deallocateTree(reference);
}

PROVIDED_TEST("compress, small example input") {
    EncodedData data = compress("STREETTEST");
    Queue<Bit>  treeShape   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeChars   = { 'T', 'R', 'S', 'E' };
    Queue<Bit>  messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0 };

    EXPECT_EQUAL(data.treeShape, treeShape);
    EXPECT_EQUAL(data.treeLeaves, treeChars);
    EXPECT_EQUAL(data.messageBits, messageBits);
}

PROVIDED_TEST("Test end-to-end compress -> decompress") {
    Vector<string> inputs = {
        "HAPPY HIP HOP",
        "Nana Nana Nana Nana Nana Nana Nana Nana Batman"
        "Research is formalized curiosity. It is poking and prying with a purpose. – Zora Neale Hurston",
    };

    for (string input: inputs) {
        EncodedData data = compress(input);
        string output = decompress(data);

        EXPECT_EQUAL(input, output);
    }
}
