/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
int points(string str) {
    int len=str.size();
    if(len<4){
        return 0;
    }
    else{
        return (len-3);
    }
    return 0;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
Set<GridLocation> generateNeighbors (Grid<char> & board,GridLocation &loc,Set<GridLocation> visitedPoints){
    Set<GridLocation> neighbors;
    int i= loc.row;
    int j= loc.col;
    for(int m=i-1;m<=i+1;m++){
        for(int n=j-1;n<=j+1;n++){
            GridLocation temp(m,n);
            if(board.inBounds(temp)&&(!visitedPoints.contains(temp))&&isalpha(board[temp])&&temp!=loc){
                    neighbors.add(temp);
            }
        }
    }
    return neighbors;
}
void scoreBoardRec(Grid<char>& board, Lexicon& lex,string wordStack,GridLocation loc,Set <GridLocation> visitedPoints, Set<GridLocation> neighbors,Set<string> &words){
    visitedPoints.add(loc);
    wordStack+=board[loc];    
    if(!isalpha(board[loc])){
        return;
    }
    //basecase
    if(neighbors.isEmpty()){
        if(lex.contains(wordStack)){
            words.add(wordStack);
        }
        return;
    }
    else{
        if(!lex.containsPrefix(wordStack)){
            return;
        }
        //递归其中一个点
        GridLocation nextLoc=neighbors.last();
        auto nextNeighbors=generateNeighbors (board,nextLoc,visitedPoints);
        scoreBoardRec(board,lex,wordStack,nextLoc,visitedPoints,nextNeighbors,words);
        //或者平行的其他点
        neighbors.remove(nextLoc);
        wordStack.pop_back();
        scoreBoardRec(board,lex,wordStack,loc,visitedPoints,neighbors,words);
    }
}
int scoreBoard(Grid<char>& board,Lexicon &lex) {
    int row= board.numRows();
    int col= board.numCols();
    int score =0;

    Set<string> words;
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){

            GridLocation loc(i,j);
            if(isalpha(board[loc])){
                string wordStack;
                Set<GridLocation> visitedPoints;
                auto neighbors=generateNeighbors(board,loc,visitedPoints);
                scoreBoardRec(board,lex,wordStack,loc,visitedPoints,neighbors,words);
            }
        }
    }
    for(string n:words){
            score+=points(n);
    }
    return score;
}

/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}
PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring") {
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}
PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}
PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}
