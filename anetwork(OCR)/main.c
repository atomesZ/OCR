#include"network.h"
#include"backprop.h"

int main()
{
	Network net = loadNetwork("saybe");/*
	int a[] = {1024,30,10};
	int *sizes = a;
	Network net = initNetwork(sizes);*/
	for(int i = 0; i < 1; ++i)
	{
		//train(net);
		printf("Errors: %i / 946\n", num_errors(net));
	}
	
	freenet(net);
	return 0;
}
