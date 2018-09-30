#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#define MAX_TREE_HT 100


char hcodes1[26][MAX_TREE_HT],hcodes2[26][MAX_TREE_HT],hcodes3[32][MAX_TREE_HT];
int len1[26],len2[26];
char encrypt[100],decrypt[100];
char realfile[1000];
char storage[1000];
int iterate=0;
struct MinHeapNode* Root;


//huffman node
struct MinHeapNode
{
    char data;  
    unsigned freq; 
    struct MinHeapNode *left, *right; 
};

// min heap nodes
struct MinHeap
{
    unsigned size;    
    unsigned capacity;   
    struct MinHeapNode **array;  // Array of minheap node pointers
};

//function allocate a new min heap node with character and frequency of the character
struct MinHeapNode* newNode(char data, unsigned freq)
{
    struct MinHeapNode* temp =
          (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

// function to create a min heap of given capacity
struct MinHeap* createMinHeap(unsigned capacity)
{
    struct MinHeap* minHeap =
         (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->size = 0;  
    minHeap->capacity = capacity;
    minHeap->array =
     (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

//function to swap two min heap nodes
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

//minHeapify function.
void minHeapify(struct MinHeap* minHeap, int idx)
{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size &&
        minHeap->array[left]->freq < minHeap->array[smallest]->freq)
      smallest = left;

    if (right < minHeap->size &&
        minHeap->array[right]->freq < minHeap->array[smallest]->freq)
      smallest = right;

    if (smallest != idx)
    {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}


int isSizeOne(struct MinHeap* minHeap)
{
    return (minHeap->size == 1);
}

//extract minimum value node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

//insert a new node to Min Heap
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode)
{
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && minHeapNode->freq < minHeap->array[(i - 1)/2]->freq)
    {
        minHeap->array[i] = minHeap->array[(i - 1)/2];
        i = (i - 1)/2;
    }
    minHeap->array[i] = minHeapNode;
}

//build min heap
void buildMinHeap(struct MinHeap* minHeap)
{
    int n = minHeap->size - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

//function to print an array of size n
void printArr(char ch,int arr[], int n)
{
    int i;
    for (i = 0; i < n; ++i){
        printf("%d", arr[i]);
	if(ch>=97 && ch<=122)	hcodes1[ch-'a'][len1[ch-'a']++]=arr[i]+'0';
	else	if(ch>=65 && ch<=90)	hcodes2[ch-'A'][len2[ch-'A']++]=arr[i]+'0';
	else	if(ch>=32 && ch<=64)	hcodes2[ch-' '][len2[ch-' ']++]=arr[i]+'0';
    }
    printf("\n");
}

//function to check if this node is leaf
int isLeaf(struct MinHeapNode* root)
{
    return !(root->left) && !(root->right) ;
}

// Creates a min heap of capacity equal to size and inserts all character 
struct MinHeap* createAndBuildMinHeap(char data[], char freq[], int size)
{
	int i=0;
    struct MinHeap* minHeap = createMinHeap(size);
     for(i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);
    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}

//function that builds Huffman tree
struct MinHeapNode* buildHuffmanTree(char data[], char freq[], int size)
{
    struct MinHeapNode *left, *right, *top;

    // Step 1: Create a min heap of capacity equal to size.  Initially, there are
    // modes equal to size.
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    // Iterate while size of heap doesn't become 1
    while (!isSizeOne(minHeap))
    {
        // Step 2: Extract the two minimum freq items from min heap
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        // Step 3:  Create a new internal node with frequency equal to the
        // sum of the two nodes frequencies. Make the two extracted node as
        // left and right children of this new node. Add this node to the min heap
        // '$' is a special value for internal nodes, not used
        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }

    // Step 4: The remaining node is the root node and the tree is complete.
    return extractMin(minHeap);
}

// Prints huffman codes from the root of Huffman Tree.  It uses arr[] to
// store codes
void printCodes(struct MinHeapNode* root, int arr[], int top)
{
   
    if (root->left)
    {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }

    
    if (root->right)
    {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    // If this is a leaf node, then it contains one of the input
    // characters, print the character and its code from arr[]
    if (isLeaf(root))
    {
        printf("%c: ", root->data);
        printArr(root->data,arr, top);
    }
}

// The main function that builds a Huffman Tree and print codes by traversing
// the built Huffman Tree
void HuffmanCodes(char data[], char freq[], int size)
{
   //  Construct Huffman Tree
   struct MinHeapNode* root = buildHuffmanTree(data, freq, size);
   Root=root;

   // Print Huffman codes using the Huffman tree built above
   int arr[MAX_TREE_HT], top = 0;
   printCodes(root, arr, top);
}

void decode_file(struct MinHeapNode* root, char s[],int actual)
{
    
    struct MinHeapNode* curr = root;
    int i=0;
    int lenn=actual;
    for ( i=0;i<lenn;i++)
    {
        if (s[i] == '0')
           curr = curr->left;
        else
           curr = curr->right;
 
        // reached leaf node
        if (curr->left==NULL && curr->right==NULL)
        {
            realfile[iterate++]= curr->data;
            curr = root;
        }
    }
    // cout<<ans<<endl;
   // return ans+'\0';
}





int binaryToDecimal(char n[],int x)
{
	  int i;
	  //int len=strlen(n);
    int dec_value = 0;
     
    // Initializing base value to 1, i.e 2^0
    int base = 1;
     
    //int len = num.length();
    for (i=x-1;i>=0;i--)
    {
        if (n[i] == '0')
        {
            dec_value += 0*base;
            base = base * 2;
        }
        else
        {
            dec_value += 1*base;
            base = base * 2;
        }
    }
     
    return dec_value;
}
 
 
 
 
 //Encryption  
int Encrypt(char arr[],int len)
{
	int i;
		int key='P';
	for(i=0;i<len;i++)
	{
		encrypt[i]=arr[i]^key;
 }
}
//Decryption
int Decrypt(char arr[],int len)
{
	int i;
		int key='P';
	for(i=0;i<len;i++)
	{
		decrypt[i]=arr[i]^key;
 }
}

void printbinchar(char character[],int leng)
{
	int i=leng;
	int j=0;
    char output[1000];
    for(j=0;j<i;j++)
	{
    
    itoa(character[j], output, 2);
    } 
    printf("\n%s", output);
}

  



// Driver program to test above functions
int main()
{
 
    int i,j,k=0,m,count=0;
    FILE *fp,*filepointer,*fp5;
    char ch;
    char ch2,ch3;
    
    fp=fopen("file.txt","r");
    
    if(fp==NULL)
    {
      printf("file can't open\n");
      return 0;
    }
    
    char freq1[26]={0};
    char freq2[26]={0};
    char freq3[32]={0};
    char data1[26]={0};
    char data2[32]={0};
     char data3[26]={0};
    char str[10000]={0};
    int len=0;
    char store[1000];
    
    while((ch=fgetc(fp))!=EOF)
    {
      str[len++]=ch;	// stroring the string read from file in a character array
      if(ch>=97 && ch<=122)
      freq1[ch-'a']++;
      
      else if(ch>=65 && ch<=90)
      freq2[ch-'A']++;
      
      else if(ch>=32 && ch<=64)
      freq3 [ch-' ']++;
    }
    
   fclose(fp);
    
   for(j=0;j<26;j++)
   {
     if(freq1[j])
     {
     	count++;
        data1[j]=(j+'a');
        printf("%c:",data1[j]);
        printf("%d\n",freq1[j]);
      
     }
     if(freq2[j])
     {
     	count++;
     	data2[j]=(j+'A');
        printf("%c:",data2[j]);
        printf("%d\n",freq2[j]);
     	
     }
      if(freq3[j])
     {
     	count++;
     	data3[j]=(j+' ');
        printf("%c:",data3[j]);
        printf("%d\n",freq3[j]);
     	
     }
   }
   
   char values[count];
   char occur[count];
   
   for(j=0;j<26;j++)
   {
     if(freq1[j])
     {
     	
        data1[j]=(j+'a');
        values[k]=data1[j];
        occur[k]=freq1[j];
        k++;
      
     }
     if(freq2[j])
     {
     	data2[j]=(j+'A');
        values[k]=data2[j];
        occur[k]=freq2[j];
        k++;
     	
     }
     
     
     if(freq3[j])
     {
     	data3[j]=(j+' ');
        values[k]=data3[j];
        occur[k]=freq3[j];
        k++;
     	
     }

   }
   
   printf("Total Different characters Present in File =%d\n",count);
     
   int size = sizeof(values)/sizeof(values[0]);
   
   printf("\nHuffman Code For Different Characters in File.\n");
   
   HuffmanCodes(values, occur , size);
   
  
   
   //rewriting file
   fp=fopen("file1.txt","w");
   for(int i=0;i<len;i++)
   {
	if(str[i]=='\n')	break;
	if(str[i]>=97 && fp!=NULL)
		fputs(hcodes1[str[i]-'a'],fp);	
	else if(fp!=NULL)
		fputs(hcodes2[str[i]-'A'],fp);
		else if(fp!=NULL)
		fputs(hcodes3[str[i]-' '],fp);
   }
   fclose(fp); // closing conn
   
   
   
   
    char arr[7];
    
    
  fp=fopen("file1.txt","r");
    
    if(fp==NULL)
    {
      printf("file can't open\n");
      return 0;
    }
    
   int l;
    FILE *fp1,*fp2;
    fp2=fopen("file1.txt","r");
    fp1=fopen("output.txt","a");
    char ch1=fgetc(fp2);
     printf("\n\nbinary no\n");
    while((ch=fgetc(fp))!=EOF)
    {
    	 //printf("%c",ch);
      arr[i]=ch;
      i++;
       ch1=fgetc(fp2);
     
      if(i==7 || ch1==EOF)
      {
      count=binaryToDecimal(arr,i);
        
       // printf("%d",i);
       
        printf("%d ",count);
        fputc(count,fp1);
        i=0;
        for(l=0;l<7;l++)
          arr[l]='0';
      }     
    }

   fclose(fp);	
   fclose(fp1);	
   
   
   char str1[1000]={0};
   int z=0;
   fp1=fopen("output.txt","r");
   while((ch=fgetc(fp))!=EOF)
    {
      str1[z++]=ch;	
    }
   fclose(fp1);  
  
  Encrypt(str1,z);
  int length=strlen(encrypt);
   fp=fopen("encrypt.txt","w");
   
   for(i=0;i<=length;i++)
   {
     fputc((char)encrypt[i],fp);
   }
  
   fclose(fp);
   
   
   
   
   
   
   Decrypt(encrypt,length);
   int length2=strlen(decrypt);
   fp=fopen("decrypt.txt","w");
  
   for(i=0;i<=length2;i++)
   {
     fputc((char)decrypt[i],fp);
   }
   fclose(fp);
   char test[1]={59};
   int w=1;
   /*int w=0;
   fp=fopen("decrypt.txt","r");
    while((ch2=fgetc(fp))!=EOF)
    {
      store[w++]=ch2;	// stroring the string read from file in a character array
      
    }
    int v=strlen(store);
    printf("\n%d",v);
    printbinchar(store,v);
    fclose(fp);*/
    printbinchar(test,w);
    
    int take=0,take2;
    
    fp5=fopen("file1.txt","r");
    while((ch3=fgetc(fp5))!=EOF)
    {
      storage[take++]=ch3;	// stroring the string read from file in a character array
      
    }
    take2=strlen(storage);
    
    
    
    
    decode_file(Root,storage,take2);
    
    
    
    filepointer=fopen("real.txt","w");
    fputs(realfile,filepointer);
    fclose(filepointer);
    
    
    
    
    
   
   
 
    
    
    
    
  return 0; 
}
