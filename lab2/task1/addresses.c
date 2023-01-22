#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int addr5;
int addr6;

int foo()
{
    return -1;
}
void point_at(void *p);
void foo1();
char g = 'g';
void foo2();

int main(int argc, char **argv)
{
    printf("long data size: %d\n", sizeof(long));
    int addr2;
    int addr3;
    char *yos = "ree";
    int *addr4 = (int *)(malloc(50));
    printf("- &addr2: %p\n", &addr2);
    printf("- &addr3: %p\n", &addr3);
    printf("- foo: %p\n", &foo);
    printf("- &addr5: %p\n", &addr5);

    point_at(&addr5);

    printf("- &addr6: %p\n", &addr6);
    printf("- yos: %p\n", yos);
    printf("- gg: %p\n", &g);
    printf("- addr4: %p\n", addr4);
    printf("- &addr4: %p\n", &addr4);

    printf("- &foo1: %p\n", &foo1);
    printf("- &foo2: %p\n", &foo2);
    printf("ld- &foo2 - &foo1: %ld\n", &foo2 - &foo1);
    printf("d- &foo2 - &foo1: %d\n", &foo2 - &foo1);

    int iarray[] = {1, 2, 3};
    float farray[3];
    double darray[3];
    char carray[] = {'a', 'b', 'c'};
    int *iarrayPtr;
    char *carrayPtr;
    iarrayPtr = iarray;
    carrayPtr = carray;
    int p[3];
    printf("***1b***\n");
    printf("iarray address: %p\n", iarray);
    printf("iarray+1 address: %p\n", iarray+1);
    printf("farray address: %p\n", farray);
    printf("farray+1 address: %p\n", farray+1);
    printf("darray address: %p\n", darray);
    printf("darray+1 address: %p\n", darray+1);
    printf("carray address: %p\n", carray);
    printf("carray+1 address: %p\n", carray+1);
    printf("********\n");
    printf("***1d***\n");
    for (int i = 0; i < 3; i++)
    {
        printf("%d", *iarrayPtr);
        iarrayPtr = iarrayPtr + 1;
    }
    printf("\n");
    for (int i = 0; i < 3; i++)
    {
        printf("%c", *carrayPtr);
        carrayPtr = carrayPtr + 1;
    }
    printf("\n");
    for (int i = 0; i < 3; i++)
    {
        printf("%p\n", p + i);
    }
    printf("********\n");
    return 0;
}

void point_at(void *p)
{
    int local;
    static int addr0 = 2;
    static int addr1;

    long dist1 = (size_t)&addr6 - (size_t)p;
    long dist2 = (size_t)&local - (size_t)p;
    long dist3 = (size_t)&foo - (size_t)p;

    printf("dist1: (size_t)&addr6 - (size_t)p: %ld\n", dist1);
    printf("dist2: (size_t)&local - (size_t)p: %ld\n", dist2);
    printf("dist3: (size_t)&foo - (size_t)p:  %ld\n", dist3);

    printf("- addr0: %p\n", &addr0);
    printf("- addr1: %p\n", &addr1);
}

void foo1()
{
    printf("foo1\n");
}

void foo2()
{
    printf("foo2\n");
}