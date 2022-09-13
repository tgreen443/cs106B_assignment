
#include <iostream>
#include <string>
#include "console.h"


using namespace std;
void print_triangle(int layer);

int main() {
    int l;
    cout <<"Please input the layer number:"<< endl;
    cin >>l;
    print_triangle(l);
    return 0;
}
void print_triangle(int layer){
//print the top layers
    for(int j=1;j<layer;j++){
      for(int i=1-layer;i<=layer-1;i++){
         if(i==j-1||i==1-j){
             cout <<j;
          }
          else{
                 cout <<" ";
          }
       }
        cout << endl;
    }

//print the last layer
     if(layer%2==0){
        for(int i=1-layer;i<=layer-1;i++){
            if(i%2==0){
                cout<<" ";
            }
            else{
                cout<<layer;
            }
        }
    }
     else{

        for(int i=1-layer;i<=layer-1;i++){
            if(i%2==0){
                cout<<layer;
            }
            else{
                cout<<" ";
            }
        }
    }
    cout <<endl;
}
