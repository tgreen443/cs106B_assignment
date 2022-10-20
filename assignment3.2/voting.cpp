// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "voting.h"

#include "testing/SimpleTest.h"
using namespace std;

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior

int blocksSum(Vector<int>&blocks){
    int sum=0;
    for (int n:blocks){
        sum+=n;
    }
    return sum;
}
void subBlocksRec(Vector<int> &blocks,int head,Vector<int> soFar,const int &winSum,Vector<int> &preResult,int soFarSum,Vector<int> &leastWinNum){
    if(head==blocks.size()){
        if(soFarSum>=winSum){
            for(auto n:soFar){
                //如果除去这个区域选票失败
                if((soFarSum-blocks[n])<winSum){
                    preResult[n]++;
                }
            }

        }
        return;
    }
    else{
        //判断是否是无效分支
        if(soFarSum<leastWinNum[head]){
            return;
        }
        //递归
        subBlocksRec(blocks,head+1,soFar,winSum,preResult,soFarSum,leastWinNum);
        //添加
        soFar.add(head);
        //递归
        subBlocksRec(blocks,head+1,soFar,winSum,preResult,soFarSum+blocks[head],leastWinNum);
    }
}
Vector<int> subBlocks(Vector<int> &blocks){
    int head=0;
    int allSum=blocksSum(blocks);
    int winSum =allSum/2+1;
    Vector<int> preResult(blocks.size(),0);
    Vector<int> soFar;
    Vector<int> leastWinNum(blocks.size(),0);
    for(int i=0;i<blocks.size();i++){
        int tempNum=0;
        for(int j=i;j<blocks.size();j++){
            tempNum+=blocks[j];
        }
        leastWinNum[i]=winSum-tempNum;
    }
    int soFarSum=0;
    subBlocksRec(blocks,head,soFar,winSum,preResult,soFarSum,leastWinNum);
    return preResult;
}
Vector<int> computePowerIndexes(Vector<int>&blocks)
{
   
    Vector<int> preResult=subBlocks(blocks);
    Vector<int> result(preResult.size(),0);
    int winCounts = blocksSum(preResult);
    for(int i=0;i<preResult.size();i++){
        if(i>0&&blocks[i]==blocks[i-1]){
            result[i]=result[i-1];
        }
        else{
            result[i]=(preResult[i]*100)/winCounts;
        }
    }
    return result;
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Test power index, blocks 50-49-1") {
    Vector<int> blocks = {50, 49, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks Hempshead 1-1-3-7-9-9") {
    Vector<int> blocks = {1, 1, 3, 7, 9, 9};
    Vector<int> expected = {0, 0, 0, 33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-NY 55-38-39") {
    Vector<int> blocks = {55, 38, 29};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-GA 55-38-16") {
    Vector<int> blocks = {55, 38, 16};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks EU post-Nice") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int i = 0; i < 15; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}


