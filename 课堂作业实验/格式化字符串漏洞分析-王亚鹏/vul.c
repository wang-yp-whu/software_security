#include <stdio.h>

void main(int argc, char ** argv)
{
   char buff[36] = {0};
   int a = 0;
   int b = 0;
   FILE *fp;
   char ch;

   if( (fp = fopen(argv[1], "r")) != NULL )
	fread(buff, 32, 1, fp);

   printf("the addresses of a b buf are %x,%x,%x\n", &a, &b, buff);

   a = 0;
   printf(buff);
   printf("a= %d\n", a);
   if(fp) 
     fclose(fp);
}
   

