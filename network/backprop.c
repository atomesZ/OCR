#include "backprop.h"
#include "network.h"
#include <math.h>

double cost(double out,double expected)
{
    return 0.5*((out - expected) * (out - expected));
}

double derive_output(Network N,int i,double derivesuivante)
{
    double sum = 0;
    int n = N.sizes[i];
    for (int k = 0; k < n; ++k)
    {
        sum += sigmoid_prime(N.n_outputs[(i+1)*n+k]) * N.weights[k*i+k];
    }

    //sum = sigmoid_prime(outputsuivante) * weight;

    return sum * derivesuivante;

}

double derive_weight(double derivesuivante,double outputsuivante,double output)
{
    return derivesuivante * sigmoid_prime(outputsuivante) * output;
}

/*double sum_prime_weight(int n)
{
    return 1;
}

//double cost_prime_weight(double out,double expected,double n)
double cost_prime_weight(double out,double n,double a)           //XOR
{
    return a*out_prime_sum(out)*n;
}

double cost_prime_bias(double out,double expected)
{
    return cost_prime_out(out,expected)*out_prime_sum(out);
}

double cost_prime_sum(double out,double expected,double weight)
{
    return cost_prime_out(out,expected)*out_prime_sum(out)*weight;
}

//double total_cost(Matrix out,Matrix in)
double total_cost(double out,double expected)         //XOR
{
    double total_cost = 0;
    //int n = out.columns*out.rows;
    int n = 1;                                          //XOR
    for (int i = 0; i < n; ++i)
    {
        //total_cost += cost(out.m[i],in.m[1]);
        total_cost += cost(out,expected);                         //XOR
    }

    return total_cost;
}

double new_weight(double weight,double LR)
{
    return weight -LR*cost_prime_weight();
}

double new_bias(double bias,double LR)
{
    return bias -LR*2;
}*/



//int backprop(Network N,double *outpt,Matrix in)
void backprop(Network N,double out,double expected,double LR)                    // XOR
{
    //double cost = total_cost(out,expected);

    double E;
    double DO;
    double DW1;
    double DW2;
    int n = N.sizes[1];
    double DO1[n];

    E = cost(out,expected);

    for (int k = 0; k <= N.sizes[N.num_layers-1]; ++k)
    {
        printf("k = %d\n",k);
        N.weights[3+k] -= LR * derive_weight(E,out,N.n_outputs[1+k]);
    }

    printf(" ---------- \n");


    DO1[0] = (out - expected) * out * ( 1 - out) * N.weights[4];

    for (int i = N.num_layers-2; i > 0; --i)
    {
        printf("i = %d\n",i);
        for (int j = 0; j < N.sizes[i]; ++j)
        {
            printf(" j = %d\n",j);
            DO = derive_output(N,i,DO1[j]);
            DW1 = derive_weight(DO,N.n_outputs[2+j],N.n_outputs[0]);
            DW2 = derive_weight(DO,N.n_outputs[2+j],N.n_outputs[1]);
            N.weights[j*2] -= LR * DW1;
            N.weights[(j*2)+1] -= LR * DW2;
            DO1[j] = DO;
        }
    }


                                                                                                //XOR :

    /*double a = cost_prime_sum(out,expected,N.weights[8]) + cost_prime_sum(out,expected,N.weights[7]) + cost_prime_sum(out,expected,N.weights[6]);

    if (1) {

        N.weights[8] = N.weights[8] - 0.2 * cost_prime_weight(out, N.n_outputs[1], cost_prime_out(out, expected));
        N.weights[7] = N.weights[7] - 0.2 * cost_prime_weight(out, N.n_outputs[1], cost_prime_out(out, expected));
        N.weights[6] = N.weights[6] - 0.2 * cost_prime_weight(out, N.n_outputs[1], cost_prime_out(out, expected));

        N.biases[0] = N.biases[0] - 0.1*cost_prime_bias(out,expected);
        N.biases[1] = N.biases[1] - 0.1*cost_prime_bias(out,expected);
        N.biases[2] = N.biases[2] - 0.1*cost_prime_bias(out,expected);

        N.weights[5] = N.weights[5] - 0.2 * cost_prime_weight(out, N.n_outputs[0], a);
        N.weights[4] = N.weights[3] - 0.2 * cost_prime_weight(out, N.n_outputs[0], a);
        N.weights[3] = N.weights[1] - 0.2 * cost_prime_weight(out, N.n_outputs[0], a);
        N.weights[2] = N.weights[4] - 0.2 * cost_prime_weight(out, N.n_outputs[2], a);
        N.weights[1] = N.weights[2] - 0.2 * cost_prime_weight(out, N.n_outputs[2], a);
        N.weights[0] = N.weights[0] - 0.2 * cost_prime_weight(out, N.n_outputs[2], a);

    }*/

}

void XOR(Network N,double out,double expected)
{
    double cost = 0.5*((expected - out) * (expected - out));

    /*double a = -(expected - out);
    double b = out*(1 - out);
    double c = N.n_outputs[];*/

    N.weights[5] = N.weights[5] - 0.5 * ( (-1*(expected - out)) * (out * ( 1 - out )) * N.n_outputs[2] );
    N.weights[4] = N.weights[4] - 0.5 * ( (-1*(expected - out)) * (out * ( 1 - out )) * N.n_outputs[3] );

    /*double d1 = a * b;
    double d2 = N.weights[]; // couche suivante
    double d = d1 * d2;
    double e = N.n_outputs[]*(1 - N.n_outputs[]); // neurone apres
    double f = N.n_outputs[] ; // neurone avant*/

    /*N.weights[5] = N.weights[5] - 0.5 * ( ( ( (-1*(expected - out)) * (out * ( 1 - out )) )* N.weights[8] ) * ( N.n_outputs[4] * (1 - N.n_outputs[4] ) ) * N.n_outputs[1] );
    N.weights[4] = N.weights[4] - 0.5 * ( ( ( (-1*(expected - out)) * (out * ( 1 - out )) )* N.weights[8] ) * ( N.n_outputs[4] * (1 - N.n_outputs[4] ) ) * N.n_outputs[0] );*/

    N.weights[3] = N.weights[3] - 0.5 * ( ( ( (-1*(expected - out)) * (out * ( 1 - out )) )* N.weights[5] ) * ( N.n_outputs[3] * (1 - N.n_outputs[3] ) ) * N.n_outputs[1] );
    N.weights[2] = N.weights[2] - 0.5 * ( ( ( (-1*(expected - out)) * (out * ( 1 - out )) )* N.weights[5] ) * ( N.n_outputs[3] * (1 - N.n_outputs[3] ) ) * N.n_outputs[0] );

    N.weights[1] = N.weights[1] - 0.5 * ( ( ( (-1*(expected - out)) * (out * ( 1 - out )) )* N.weights[4] ) * ( N.n_outputs[2] * (1 - N.n_outputs[2] ) ) * N.n_outputs[1] );
    N.weights[0] = N.weights[0] - 0.5 * ( ( ( (-1*(expected - out)) * (out * ( 1 - out )) )* N.weights[4] ) * ( N.n_outputs[2] * (1 - N.n_outputs[2] ) ) * N.n_outputs[0] );














}


