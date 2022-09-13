#include <iostream>
#include "console.h"
#define v __cplusplus
using namespace std;


void print_cpp_version();

int main() {
    print_cpp_version();
    return 0;
}


void print_cpp_version(){
    if(v==199711){
        cout << " The newest cplusplus standard version that your compiler support is C++98 " <<endl;
    }
    else{
        if(v==201103){
        cout << " The newest cplusplus standard version that your compiler support is C++11 " <<endl;
        }
        else{
            if(v==201402){
                cout << " The newest cplusplus standard version that your compiler support is C++14 " <<endl;
            }
            else{
                if(v==201703){
                    cout << " The newest cplusplus standard version that your compiler support is C++17 " <<endl;
                }
                else{
                    if(v==202002){
                        cout << " The newest cplusplus standard version that your compiler support is C++20 " <<endl;
                    }
                    else{
                        cout << " No imformation about the cpp version of your compiler";
                    }
                }
            }

        }
    }
}
