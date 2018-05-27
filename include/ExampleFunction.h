#ifndef EXAMPLEFUNCTION_H
#define EXAMPLEFUNCTION_H

#define _GLIBCXX_USE_CXX11_ABI 0

#include "NumericalOptimizerInterface.h"
#include "GlobalPlacer.h"

class ExampleFunction : public NumericalOptimizerInterface
{
public:
    ExampleFunction(unsigned dimension, double gamma, double lambda);

    void evaluateFG(const vector<double> &x, double &f, vector<double> &g);
    void evaluateF(const vector<double> &x, double &f);
    unsigned dimension();
    unsigned _dimension;
    double _gamma;
    double _lambda;
};
#endif // EXAMPLEFUNCTION_H
