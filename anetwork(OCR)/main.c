#include"network.h"
#include"backprop.h"

int main()
{
	Network net = loadNetwork("aybe");
	train(net);
	printf("Errors: %i / 946\n", num_errors(net));
	
	freenet(net);
	return 0;
}
