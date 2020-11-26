#include <stdio.h>
#include <stdlib.h>
int main(int argc,char *argv[])
{
	char *buf = (char*)malloc(4*sizeof(char));
	char *pbuf=buf;
	int i,j;
	for(i=0;i<8;i++)
	{
		*pbuf = 's';
		pbuf++;
	}
	// *pbuf = '\0';
	printf("%s\n",buf);
	return 0;
}
