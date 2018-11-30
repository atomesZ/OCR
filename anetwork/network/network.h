# ifndef NETWORK_H_
# define NETWORK_H_

#include<time.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>

typedef struct Network Network;

struct Network
{
	int num_layers; //number of layers
	int *sizes; //number of neurons by layers
	double *biases; //list of biases for neurons
	double *weights; //list of weights for neurons
	int num_neurons; //number of neurons
	int num_links; //number of links
	int num_biases; //number of biases
	double *n_outputs; //list of outputs of each neurons
	double *expected; //list of expected outputs for last layer
};

Network initNetwork(int *sizes);
void saveNetwork(Network net, char* filename);
Network loadNetwork(char* filename);
void freenet(Network net);
double sigmoid(double z);
double sigmoid_prime(double z);
double dot(double *w, double *n_outputs, int len, int ifo, int ifw);
void feedforward(Network net);
void printNet(Network net);

#endif
