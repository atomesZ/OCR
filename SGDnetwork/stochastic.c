#include "backprop.h"
#include "network.h"
#include <math.h>

double loss(double out,double expected)
{
    return (expected - out) * (expected - out);
}

double derive_output(Network N, int n)
{
    return sigmoid_prime(N.n_outputs[n]);
}

double derive_weight(double derivesuivante,double outputsuivante,double output)
{
    return derivesuivante * sigmoid_prime(outputsuivante) * output;
}

void SGD(Network N, double s[]/*data_set*/, int set_size, double LR, double epochs)
{
    printf(" set_size = %d\n",set_size);
    printf(" LR = %lf\n",LR);
    printf(" epochs = %f\n",epochs);

    double delta_w[N.num_links ];
		
    double delta_b[N.num_biases];

    ////double LR = 0.1;


    double epoch_n = 0;

    int a = 0;
    int b = 0;

    int z = 0;   

    while (epoch_n <= epochs)
    {
	printf(" epoch n :%lf\n",epoch_n);

	++epoch_n;

	//bash
	
	int data_i = rand(); // < TAILLE DATA_SET 

	// parcourir element i
	
            //XOR

	a = rand();
        b = rand();
        if(a < RAND_MAX / 2)
            a = 0;
        else
            a = 1;

        if(b < RAND_MAX / 2)
            b = 0;
        else
            b = 1;

	//feedforward

        N.n_outputs[0] = a;
	N.n_outputs[1] = b;

	printf(" a = %d\n",a);
	printf(" b = %d\n",b);

        feedforward(N);

	//backprop
	
	    //XOR
	
	double out[] = { N.n_outputs[N.num_neurons - 1]};
        double expected[] = { a + b == 1};

        double delta2[N.sizes[2]];

        for(int i = 0; i < N.sizes[2]; ++i)
        {
            delta2[i] = (out[i] - expected[i]) *  derive_output(N, N.num_neurons - N.sizes[2] + i);
        }
        double delta1[N.sizes[1]];

        for(int i = 0; i < N.sizes[1]; ++i)
        {
           double r = 0;
           for(int j = 0; j < N.sizes[2] ; ++j)
           {
               r += delta2[j] * N.weights[N.sizes[0]*N.sizes[1] + j + i*N.sizes[2]];
           }
           delta1[i] = r * derive_output(N, N.sizes[0] + i);
        }

        //maj B2
        for(int i = 0; i < N.sizes[2]; ++i)
           delta_b[N.sizes[1] + i] -= LR * delta2[i];

        //maj B1
        for(int i = 0; i < N.sizes[1] ; ++i)
            delta_b[i] -= LR * delta1[i];

       //maj W1
       for(int i = 0; i < N.sizes[0]; ++i)
       {
           for(int j = 0; j < N.sizes[1]; ++j)
           {
               delta_w[j*N.sizes[0] + i] -= LR * delta1[j] * N.n_outputs[i];
	       /*printf(" delta of :  %d\n",j*N.sizes[0] + i);
	       printf(" delta -= %lf\n ",LR * delta1[j] * N.n_outputs[i]);*/
           }
       }

        //maj W2
        for(int i = 0; i < N.sizes[1]; ++i)
        {
                for(int j = 0; j < N.sizes[2]; ++j)
                {
                        delta_w[j*N.sizes[1] + i + N.sizes[0] * N.sizes[1]] -= LR * delta2[j] * N.n_outputs[i + N.sizes[0]];
                }
        }


       ++z;
       
       printf("%d\n",z);    
    
    
    }

    //UPDATE

        //weights

    for (int w_n = 0; w_n < N.num_links; ++w_n)
    {
        N.weights[w_n] -= LR * (delta_w[w_n]/epochs);
        printf(" POID : %lf\n",delta_w[w_n]/epochs);	
    }

        //bias
    
    for (int b_n = 0; b_n < N.num_biases; ++b_n)
    {
        N.biases[b_n] -= LR * (delta_b[b_n]/epochs);
	printf(" BIAIS : %lf\n",delta_b[b_n]/epochs);
    }
 
    if (loss(N.n_outputs[N.num_neurons - 1], a + b == 1) > 0.01)
    {
         SGD(N,s,4,0.01,10.0);    
    }


}
