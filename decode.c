#include <stdio.h>
#include <sys/types.h>
#include <dlfcn.h>
#include<string.h>

typedef void (*enc_func)(char*);
int main (int argc, char* argv[])
{
	void* dl_handler;
	enc_func func;
	char *str;
	
	if(argc<3)
	{
		return 1;
	}
	
	if(strcmp(argv[1] ,"codecA") == 0)
	{
		if ((dl_handler = dlopen("codecA.so", RTLD_LAZY)) == NULL)
			return -1;
		//printf("hello world\n");
		if ((func = (enc_func)dlsym(dl_handler, "codecA_decode")) == NULL)
			goto exit;
		//printf("hello world\n");
		
	}
	else if(strcmp(argv[1] ,"codecB") == 0)
	{
		if ((dl_handler = dlopen("codecB.so", RTLD_LAZY)) == NULL)
			return -1;
		//printf("hello world\n");
		if ((func = (enc_func)dlsym(dl_handler, "codecB_decode")) == NULL)
			goto exit;
		//printf("hello world\n");	
	}
	
	
	str = argv[2];
	func(str);
	
	printf("%s\n",str);


	exit:
	dlclose(dl_handler);
	return 0;
}

