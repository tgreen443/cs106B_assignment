/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include <algorithm>
#include <map>
#include "vector.h"
using namespace std;

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 * WARNING: The provided code is buggy!
 *
 * Use unit tests to identify which inputs to this function
 * are incorrectly handled. Then, remove this comment and
 * replace it with a description of the bug you fixed.
 */
//除去所有非字母的字符
string removeNonLetters(string s) {
    int head =0;
    while(!isalpha(s.at(head))){
        head++;
        if(head==s.length()){
            return "";
        }
    }
    int length=s.length();
    string result = charToString(s.at(head));
    for (int i = head+1; i < length; i++) {
        if (isalpha(s.at(i))) {
            result += s.at(i);
        }
    }
    return result;
}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */

string soundex(string s) {

    string m;
    m=removeNonLetters(s);
    transform(m.begin(),m.end(),m.begin(),::toupper);
    char head=m.at(0);
    map<char,char> mymap={
        {'A','0'},
        {'E','0'},
        {'I','0'},
        {'O','0'},
        {'U','0'},
        {'H','0'},
        {'W','0'},
        {'Y','0'},
        {'B','1'},
        {'F','1'},
        {'P','1'},
        {'V','1'},
        {'C','2'},
        {'G','2'},
        {'J','2'},
        {'K','2'},
        {'Q','2'},
        {'S','2'},
        {'X','2'},
        {'Z','2'},
        {'D','3'},
        {'T','3'},
        {'L','4'},
        {'M','5'},
        {'N','5'},
        {'R','6'}

    };
    //转换成数字
    //cout << m <<endl;
    int m_length=m.size();
    for(int i=0;i<m_length;i++){
       m.at(i)=mymap.at(m.at(i));
    }
    //cout << "transform to numbers:"<<m <<endl;
    //合并重复数字
    for(int i=0;i<m_length-1;){
            if(m.at(i)==m.at(i+1)){
               m.erase(i,1);
               m_length--;
               }
            else{
                i++;
            }
    }
    int temp_len=m.size();
    if(temp_len>1&&m.at(temp_len-2)==m.at(temp_len-1)){
        m.erase(temp_len-1);
    }
    //cout <<"delete same numbers:" <<m <<endl;
    //除去0替换首字母
    m.at(0)=head;
    //cout << "transform the first char into letter:"<<m <<endl;
    int semi_length=m.size();
    for(int i=0;i<semi_length;){
            if(m.at(i)=='0'){
               m.erase(i,1);
               semi_length--;
            }
            else{
                i++;
            }
    }
    //cout <<"delete the 0:"<< m << endl;

    //整理成四位
    int final_length=m.size();
    if(final_length>4){
        m.erase(4);
    }
    else {
        for(int i=final_length;i<4;i++){
            m+='0';
        }
    }
    //cout<<"make its length 4:"<< m << endl;
    return m;

}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    while(true){

        cout << "Read file " << filepath << ", "
             << databaseNames.size() << " names found." << endl;

        // The names in the database are now stored in the provided
        // vector named databaseNames
        string line=getLine("Please enter a surname(RETURN to quit):");
        string soudex_code=soundex(line);
        cout <<"Soundex code is "<<soudex_code<<endl;
        Vector<string> tempDatabase;
        for(string name:databaseNames){
            if (soundex(name)==soudex_code){
                tempDatabase.add(name);
            }
        }
        tempDatabase.sort();
        cout << "Matches from database: "<<tempDatabase<<endl;
   }

    /* TODO: Fill in the remainder of this function.*/
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}



PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here
STUDENT_TEST("removeNonLetters"){
    EXPECT_EQUAL(removeNonLetters("q2c456ko's"),"qckos");
    EXPECT_EQUAL(removeNonLetters("2222222"),"");
    EXPECT_EQUAL(removeNonLetters("wH  s356"),"wHs");
}

