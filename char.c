#include<stdio.h>
#include<string.h>
int str[1000];
int k=0;
int m;
void convert(char b[],int leng) 
{
	
	//char b[2]={'B','A'};
	int p=leng;
	int z=0;
	for(z=0;z<p;z++)
	{
	
	
  char a=b[z];
  int i;
  
  
  for (i = 1; i < 8; i++) {
  	int j=!!((a << i) & 0x80);
  	str[k++]=j;
      //printf("%d", !!((a << i) & 0x80));
      //printf("%d",j);
      j=0;
  }
}
}
int main()
{
	//	char b[]={'B','A','C','D','E'};
	//int p=strlen(b);
	//printf("%d\n",p);
	//convert(b,p);
	int len=0;
	int h=0;
	FILE *fp;
	char ch;
    
    
    fp=fopen("decrypt.txt","r");
    
    if(fp==NULL)
    {
      printf("file can't open\n");
      return 0;
    }
    
    
    char store[1000];
    
    while((ch=fgetc(fp))!=EOF)
    {
      store[len++]=ch;	// stroring the string read from file in a character array
      
    }
    h=strlen(store);
    convert(store,h);
    
    
   fclose(fp);

  printf("\n");
  for(m=0;m<k;m++)
  {
  
  printf("%d",str[m]);
   }
  return 0;
}
