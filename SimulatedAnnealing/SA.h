#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H
#include <iostream>
#include <vector>
#include <cstdlib>
#include <functional>
#include <limits>
#include <algorithm>
#include "InputData.h"
#include "Random.h"
#include <time.h>


using namespace std;

class SA
{
    public:
        SA(InputData*);  
        Random mersenne_obj;
        void Optimize(double (vector<double>, InputData*)); // "()main" da classe
        InputData *pInputData;

        /// -------- SA ------------
        double alpha; // Cooling law constant Tkp1 = Tk*alpha
        double Temp; // Current Temperature
        double Temp0; // Initial Temperature
        double TempRatio; // Timevarying weight that multiples stochastic component in position update equation
        double dY; // evaluates the different between two solutions (xkp1 - xk)

        // Position Vectors
        vector<double> x0; // Initial position
        vector<double> xk;
        vector<double> xkp1;
        vector<double> xk_best; // global solution
        vector<double> xMin;    // minumum bound restriction 
        vector<double> xMax;    // maximum bound restriction 

        // objective function values
        double y0, yk, ykp1, yk_best;
        bool globalsolutionflag;
      
};

#endif // SIMULATEDANNEALING_H
