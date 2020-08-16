#include "stdio.h"

int a [5] = {0, 1, 2, 3, 4};


int add(int a[], int n){
    int sum = 0;
    for(int i = 0; i < n; i++){
        sum += a[i];
    }
    return sum;
}


int main(){
    int val = add(a, 5);
    printf("valu: %d\n", val);
    return 0;
}

