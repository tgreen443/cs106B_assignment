/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "console.h"
#include <cmath>
#include <iostream>
#include "testing/SimpleTest.h"
using namespace std;

/* This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
long smarterSum(long n) {
    long total = 0;
    long m;
    double a=sqrt(n);
    m= (long)(a);
    for (long divisor = 1; divisor < m+1; divisor++) {
        if (n % divisor == 0) {
            total = total + divisor+n/divisor;
        }
    }
    total-=n;
    if(m*m==n){
        total-=m;
    }
    return total;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
bool isPerfectSmarter(long n) {
    return (n != 0) && (n == smarterSum(n));
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void findPerfectsSmarter(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
long findNthPerfectEuclid(long n) {
    long k;
    long m;
    long count =0;
    for(k=1;;k++){
        m=(long)pow(2,k)-1;
        if(smarterSum(m)==1){
            count++;
            if(count==n){
                long p;
                p=m*(m+1)*0.5;
                return p;
            }
        }
    }
}


/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(10000, findPerfects(10000));
    TIME_OPERATION(20000, findPerfects(20000));
    TIME_OPERATION(40000, findPerfects(40000));
}

// TODO: add your student test cases here
STUDENT_TEST("Q1") {
    TIME_OPERATION(10, findPerfects(10));
    TIME_OPERATION(100, findPerfects(100));
    TIME_OPERATION(1000, findPerfects(1000));
    //TIME_OPERATION(300000, findPerfects(300000));
    //EXPECT(isPerfect(-1));


}
STUDENT_TEST("smarterSum"){
    EXPECT_EQUAL(smarterSum(17),divisorSum(17));
    EXPECT_EQUAL(smarterSum(25),divisorSum(25));
    EXPECT_EQUAL(smarterSum(120),divisorSum(120));
    EXPECT_EQUAL(smarterSum(10000),divisorSum(10000));
}
STUDENT_TEST("Q7"){
    TIME_OPERATION(10, findPerfectsSmarter(10));
    TIME_OPERATION(100, findPerfectsSmarter(100));
    TIME_OPERATION(1000, findPerfectsSmarter(1000));
    //TIME_OPERATION(12000000, findPerfectsSmarter(12000000));
}
STUDENT_TEST("Euclid"){
    EXPECT(findNthPerfectEuclid(1)==6);
    EXPECT(findNthPerfectEuclid(2)==28);
    EXPECT(findNthPerfectEuclid(3)==496);
    EXPECT(findNthPerfectEuclid(4)==8128);
    TIME_OPERATION(5,findNthPerfectEuclid(5));
    TIME_OPERATION(6,findNthPerfectEuclid(6));
}
