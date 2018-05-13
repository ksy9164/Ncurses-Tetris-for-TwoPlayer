#include<stdio.h>
#include<stdlib.h>

int main()
{
    char * a = (char *)malloc(sizeof(char) * 10000);
    for(int i =0 ; i< 10000;i++)
        a[i] = i;
    char * b = "123123121232131231233";
    printf("%d",(int)sizeof(a));
    printf("%d",(int)sizeof(b));
}
