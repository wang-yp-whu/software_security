# 1 "swap.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "swap.c"
extern int buf[];

int * bufp0 = &buf[0];
static int *bufp1;

void swap()
{
    int temp;
    bufp1 = &buf[1];
    temp = *bufp0;
    *bufp0 = *bufp1;
    *bufp1 = temp;
}
