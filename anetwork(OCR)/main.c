#include"network.h"
#include"backprop.h"

int main()
{
	Network net = loadNetwork("aybe");/*
	int a[] = {1024,30,10};
	int *sizes = a;
	Network net = initNetwork(sizes);*/
	for(int i = 0; i < 1; ++i)
	{
		//train(net, "data_set");
		
		int num_char = 946;
		printf("Errors: %i / %i\n", num_errors(net, "data_set"), num_char);
		/*
		char *s;
		s = malloc(sizeof(char) * num_char);
		readtxt(net, "data_set", s, num_char);
	   printf(s);
	   free(s);
	   printf("\n");*/

	}
	
	freenet(net);
	return 0;
}
