#include <stdio.h>

void	swap(int a, int b);

int main() 
{ 
    printf("hello\n");
    swap(1, 2);
    return 0; 
}

// gcc -m32 test.c -o test32
// file test32