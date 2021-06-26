#include <iostream>
#include <vector>
#include <cstdlib>
#include <functional>
#include <limits>
#include <algorithm>
#include <iomanip> //change precision of double
#include <omp.h>
#include "InputData.h"
#include "Random.h"
#include "SA.h"


using namespace std;

// CLASS::CONSTRUCTOR (outline) for SA
SA::SA(InputData *inputobj)  
{
    //Initializes seed for the Random Class
    mersenne_obj.SeedInput();

    // ---- Simmulated Annealing parameters
    alpha = 0.92; Temp0 = 1.0; Temp = Temp0;
    y0 = 0.0; yk = y0; ykp1 = y0;
    yk_best = 0.0; dY = 0.0;

    pInputData = inputobj; //copy input pointer

    //cout << "Number of iterations = " << pInputData->iterations << endl; 
    // ---- Resizing vectors
    x0.resize(inputobj->nr_independent_variables);
    xk.resize(inputobj->nr_independent_variables);
    xkp1.resize(inputobj->nr_independent_variables);
    xk_best.resize(inputobj->nr_independent_variables);
    xMax.resize(inputobj->nr_independent_variables);   
    xMin.resize(inputobj->nr_independent_variables);   

    // ---- Boolean var indicates that global solution have been updated at least once in relation to its initial value at k == 0
    globalsolutionflag = false;


    // Copies xMax e xMin from the input Object
    for (int j = 0; j < inputobj->nr_independent_variables; j++)         
    {                                                  
		xMax[j] = inputobj->xMax;
		xMin[j] = inputobj->xMin;
    }
    // ---- Initializing vectors xk_best, xkp1, xk, x0.
    // ... x0 is the initial random position in the search space
    for (int i = 0; i < inputobj->nr_independent_variables; i++)
    {
        xkp1[i] = 0.0;       
        xk_best[i] = 0.0;
        x0[i] = xMin[i] + (xMax[i] - xMin[i])*mersenne_obj.dRN(0.0,1.0);
        xk[i] = x0[i]; 
    }

}

// CLASS::MEMBER.FUNCTION(ARGs) 
// ...Optimize() is the member function that implements the optimization method 
//
// double f(vector<double>, InputData*) is a prototype declaration of the objective function declared in the global scope (main.cpp). 
// The compiler converts this prototype declaration to the appropiate pointer type.
// It is equvalent to: void SA::Optimize(double (*f)(vector<double>, DadosEntrada&), string  &ArquivoSaida)
void SA::Optimize(double f(vector<double>, InputData*))
{

    //nr_activation -> counts the number of times certain heuristics are activated
    int nr_activation = 0; 

    cout << "Initializing Simmulated Anealing Algorithm..." << endl << endl;
    for (int k = 0; k < pInputData->iterations; k++)
    {
        // -- Cooling law simulates a temperature descrease as function of time.
        Temp = alpha*Temp;
        TempRatio = Temp/Temp0;

        yk = f(xk, pInputData);
        
        //Using OpenMP for CPU parallelization
        #pragma omp parallel for 
        for (int i = 0; i < pInputData->nr_independent_variables; i++)
        {
            //std::cout << "xk[" << i << "] : " << xk[i] << std::endl;

            // Updates the "Position" of the solution in searchspace, x_k+1 = x_k + Temperature_contribution_term*(f(RandomNr, xMin, xMax))
            xkp1[i] = xk[i] + TempRatio*( ( xMax[i] - xk[i] )*mersenne_obj.dRN(0.0,1.0) - (xk[i] - xMin[i])*mersenne_obj.dRN(0.0,1.0) );

            // Imposes minimum and maximum bounds on the independent variables 
            if (xkp1[i] > xMax[i] )   
		    {
			    xkp1[i] = xMax[i];
		    }
		    else if (xkp1[i] < xMin[i])
		    {
				xkp1[i] = xMin[i];  
		    }
        }

        // evaluates function at point xk+1
        ykp1 = f(xkp1, pInputData);

        // The first function evaluation at k = 0 is set as yk_best
        if (k == 0) 
        {
            yk_best = ykp1;
        }

        double deltayk = ykp1 - yk; 
        double local_random_num1 = mersenne_obj.dRN(0.0,1.0);

        // This heuristic moves back the solution to enhance exploration at point xk 
        //if ((deltayk < (1 + TempRatio*local_random_num1 )) && (k != 0) )
        if ( local_random_num1 < exp( -deltayk/Temp ) && k != 0 ) 
        {
            xk = xkp1;
            yk = ykp1;
            nr_activation++;
        }

        // Updates global solution 
        if (ykp1 < yk_best)
        {
            xk = xkp1;
            yk = ykp1;

            yk_best = yk;
            xk_best = xk;

            globalsolutionflag = true;
        }

        // Iteration output
        cout << "Iter : " << k << "\tObj.Func. : " << yk_best << "\tTemperature.Coefficient : " << TempRatio << "  "<< endl;
 
    }


    // Prints the results to the user
    cout << endl << "----------------------------------------------------------------------" << endl << endl;
    std::cout << "Number of times heuristic was activated : " << nr_activation << " of total of iterations :" << pInputData->iterations << std::endl;
    cout << endl << "Minimum Objective Function: " << yk_best << endl;     
	cout << "Solution (indepent variables at optimum):" << endl << endl;

    for (int i = 0; i < pInputData->nr_independent_variables; i++)
    {  
		cout << fixed << setprecision(4) << "x_opt[" << i << "] : " << xk_best[i] << " ";    //solucao global (lider)
    }
    cout << endl;

    if  (globalsolutionflag == false) { cout << "Warning! Possible Bad solution" << endl;}
}
