/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
string operatorsFrom(string str) {
    Set<char> Operators{'{','}','[',']','(',')'};
    if(str.empty()){
        return "";
    }
    if(Operators.contains(str[0])){
        return str[0]+operatorsFrom(str.substr(1));
    }
    else{
        return operatorsFrom(str.substr(1));
    }
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
bool operatorsAreMatched(string ops) {
    /*
    Stack<char> parens;
    for (int i = 0; i < (int) ops.length(); i++) {
        char c = ops[i];
        if (c == '(' || c == '{'|| c == '[') {
        parens.push(c);
        } else if (c == ')' || c == '}' || c == ']') {
            if (parens.isEmpty()) {
                return false;
            }
            char top = parens.pop();
            if ((top == '(' && c != ')') || (top == '{' && c != '}') || (top == '[' && c != ']')) {
                return false;
            }
        }
    }

    if (parens.isEmpty()) {
        return true; // balanced
    } else {
        return false;
    }
*/
    //递归
    int Len= ops.size();
    if(Len==0){
        return true;
    }
    bool flag = false;
    if(stringContains(ops,"()")){
        int bracketIndex = stringIndexOf(ops,"()");
        ops.erase(bracketIndex,2);
        flag = true;
    }
    if(stringContains(ops,"[]")){
        int bracketIndex = stringIndexOf(ops,"[]");
        ops.erase(bracketIndex,2);
        flag = true;
    }
    if(stringContains(ops,"{}")){
        int bracketIndex = stringIndexOf(ops,"{}");
        ops.erase(bracketIndex,2);
        flag = true;
    }
    if(!flag){
        return false;
    }
    return operatorsAreMatched(ops);
}

/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}
STUDENT_TEST("operatorsAreMatched test") {
    EXPECT(operatorsAreMatched("{}"));
    EXPECT(operatorsAreMatched("{[()]}"));
    EXPECT(operatorsAreMatched("{[{}]{}}"));
    EXPECT(!operatorsAreMatched("{{{}]}"));
    EXPECT(!operatorsAreMatched("{([])}]"));
}
