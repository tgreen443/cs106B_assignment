// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include <algorithm>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
string cleanToken(string s)
{
    if(s.size()==0){
        return "";
    }
    string str=s;
    int initialLen=s.length();
    int head =0;
    int tail =initialLen-1;
    while(ispunct(s[head])){
        head++;
        if(head==initialLen){
            return "";
         }
    }
    while(ispunct(s[tail])){
        tail--;
    }
    string cutStr=str.substr(head,tail-head+1);
    int alphaNum = 0;
    for(int num : cutStr){
        if(isalpha(num)){
            alphaNum++;
        }
    }
    if(alphaNum==0){
        return "";
    }

    transform(cutStr.begin(),cutStr.end(),cutStr.begin(),::tolower);
    return cutStr;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> gatherTokens(string text)
{
    Vector<string> preToken = stringSplit(text,' ');
    Set<string> tokens;
    for(string str:preToken){
        string s=cleanToken(str);
        if(s!=""){
            tokens.add(s);
        }
    }
    return tokens;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
int buildIndex(string dbfile, Map<string, Set<string>>& index){
    ifstream in;
    int webCount=0;
    if (!openFile(in, dbfile)){
        error("Cannot open file ");
    }
    Vector<string> lines;
    readEntireFile(in, lines);
    int linLen=lines.size();
    for(int i=0;i<linLen;i+=2){
        string url=lines[i];
        Set<string> tokens = gatherTokens(lines[i+1]);
        for(string str:tokens){
            if(index.containsKey(str)){
                index[str].add(url);
            }
            else{
                Set<string> iniVal ;
                iniVal.add(url);
                index.put(str,iniVal);
            }
        }
        webCount++;
    }

    return webCount;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;
    Vector<string> queryVec = stringSplit(query,' ');
    for(string strQuery:queryVec){
        string tempQuery=cleanToken(strQuery);
        if(tempQuery.size()==0){
            continue;
        }
        if(strQuery[0]=='-'){
            result-=index[tempQuery];
        }
        else{
            if(strQuery[0]=='+'){
                result.intersect(index[tempQuery]);
            }
            else{
                result+=index[tempQuery];
            }
        }

    }
    return result;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
void searchEngine(string dbfile)
{
    Map<string, Set<string>> index;
    cout << "Stand by while building index..." <<endl;
    int mapNum =buildIndex(dbfile,index);
    cout << "Indexed ";cout <<mapNum;cout << " pages containing ";cout <<index.size();cout <<" unique terms "<< endl;
    cout <<endl;

    string query;
    while(true){
        query = getLine("Enter query sentence (RETURN/ENTER to quit):");
        if(query==""){
            break;
        }
        Set<string> matchResults =findQueryMatches(index,query);
        cout <<"Found "; cout << matchResults.size(); cout <<" matching pages"<<endl;
        cout <<matchResults<<endl;
    }
    cout<<"All done!"<<endl;
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


// TODO: add your test cases here
STUDENT_TEST("cleanToken test"){
    EXPECT_EQUAL(cleanToken(""), "");
    EXPECT_EQUAL(cleanToken("!u!"), "u");
    EXPECT_EQUAL(cleanToken("**g!!O$$"), "g!!o");
    EXPECT_EQUAL(cleanToken("**!!$$"), "");
    EXPECT_EQUAL(cleanToken("BA111D%%%%"), "ba111d");
}
STUDENT_TEST("buildIndex timeTest"){
    Map<string, Set<string>> index;
    for(int i=0;i<5;i++){
        for(int j=0;j<i;j++){
           TIME_OPERATION(index.size(),buildIndex("res/tiny.txt", index));
        }
    }

}

STUDENT_TEST("search"){
    searchEngine("res/website.txt");
}
