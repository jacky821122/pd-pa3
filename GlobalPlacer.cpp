#include "GlobalPlacer.h"
#include "ExampleFunction.h"
#include "NumericalOptimizer.h"
#include <map>
#include <cstdlib>
#include <cmath>

GlobalPlacer::GlobalPlacer(Placement &placement)
	:_placement(placement)
{

}


void GlobalPlacer::place()
{
	///////////////////////////////////////////////////////////////////
	// The following example is only for analytical methods.
	// if you use other methods, you can skip and delete it directly.
    //////////////////////////////////////////////////////////////////
    bool RANDOM_SOLUTION = false;

    if(!RANDOM_SOLUTION)
    {   
        srand(19931122);
        for(unsigned i = 0; i < _placement.numModules(); ++i)
        {            
            int height = abs(_placement.boundryTop() - _placement.boundryBottom());
            int width = abs(_placement.boundryRight() - _placement.boundryLeft());
            
            int position_x = rand() % width - abs(_placement.boundryLeft());
            int position_y = rand() % height - abs(_placement.boundryBottom());

            _placement.module(i).setPosition(position_x, position_y);
        }

        double gamma = 100;
        double lambda = 0;
        for(unsigned i = 0; i < _placement.numNets(); ++i)
        {
            map<unsigned, bool> modulesThisIteration;
            unsigned numPins = _placement.net(i).numPins();
            ExampleFunction ef(numPins * 2, gamma, lambda);
            vector<double> x(numPins * 2);
            vector<double> deltaPinBlock(numPins * 2);

            for(unsigned j = 0; j < numPins; ++j)
            {
                if(modulesThisIteration.find(_placement.net(i).pin(j).moduleId()) == modulesThisIteration.end())
                {
                    modulesThisIteration.insert(pair<unsigned, bool>(_placement.net(i).pin(j).moduleId(), true));
                }
                x[2 * j + 0] = _placement.net(i).pin(j).x();
                x[2 * j + 1] = _placement.net(i).pin(j).y();
                // x[2 * j + 0] = _placement.module(_placement.net(i).pin(j).moduleId()).x();
                // x[2 * j + 1] = _placement.module(_placement.net(i).pin(j).moduleId()).y();
                deltaPinBlock[2 * j + 0] = _placement.net(i).pin(j).x() - _placement.module(_placement.net(i).pin(j).moduleId()).x();
                deltaPinBlock[2 * j + 1] = _placement.net(i).pin(j).y() - _placement.module(_placement.net(i).pin(j).moduleId()).y();
                cout << x[2 * j + 0] << " ";
                cout << x[2 * j + 1] << " ";
            }
            cout << endl;

            NumericalOptimizer no(ef);
            no.setX(x);
            no.setNumIteration(30);
            no.setStepSizeBound(80);
            no.solve();

            for(unsigned j = 0; j < numPins; ++j)
            {
                double position_x = no.x(2 * j + 0) - deltaPinBlock[2 * j + 0];
                double position_y = no.x(2 * j + 1) - deltaPinBlock[2 * j + 1];
                _placement.module(_placement.net(i).pin(j).moduleId()).setPosition(position_x, position_y);
            }

            cout << "Block number: " << this->_placement.numModules() << endl;
            cout << "Current solution:" << endl;
            for (unsigned i = 0; i < no.dimension(); i++) {
                cout << "x[" << i << "] = " << no.x(i) << endl;
            }
            cout << "Objective: " << no.objective() << endl;
        }

        for(unsigned i = 0; i < _placement.numModules(); ++i)
        {
            cout << "(" << _placement.module(i).x() << "," << _placement.module(i).y() << ")" << endl;
        }
    }

    else
    {
        srand(19931122);
        for(unsigned i = 0; i < _placement.numModules(); ++i)
        {            
            int height = abs(_placement.boundryTop() - _placement.boundryBottom());
            int width = abs(_placement.boundryRight() - _placement.boundryLeft());
            
            int position_x = rand() % width - abs(_placement.boundryLeft());
            int position_y = rand() % height - abs(_placement.boundryBottom());

            _placement.module(i).setPosition(position_x, position_y);
        }
    }

    // ExampleFunction ef(_placement, gamma); // require to define the object function and gradient function

 //    vector<double> x(_placement.numModules() * 2); // solution vector, size: num_blocks*2 
 //                                                   // each 2 variables represent the X and Y dimensions of a block
 //    for(unsigned i = 0; i < x.size(); ++i) // initialize the solution vector
 //    {
 //        x[i] = 0.005;
 //    }
 //    for(unsigned i = 0; i < 5000; ++i)
 //    {
 //        x[i] = 0.002;
 //    }

 //    NumericalOptimizer no(ef);
 //    no.setX(x); // set initial solution
 //    no.setNumIteration(35); // user-specified parameter
 //    no.setStepSizeBound(1); // user-specified parameter
 //    no.solve(); // Conjugate Gradient solver

 //    // cout << "Block number: " << _placement.numModules() << endl;
 //    cout << "Current solution:" << endl;
 //    for (unsigned i = 0; i < no.dimension(); i++) {
 //        cout << "x[" << i << "] = " << no.x(i) << endl;
 //    }
 //    cout << "Objective: " << no.objective() << endl;
	// ////////////////////////////////////////////////////////////////


}


void GlobalPlacer::plotPlacementResult( const string outfilename, bool isPrompt )
{
    ofstream outfile( outfilename.c_str() , ios::out );
    outfile << " " << endl;
    outfile << "set title \"wirelength = " << _placement.computeHpwl() << "\"" << endl;
    outfile << "set size ratio 1" << endl;
    outfile << "set nokey" << endl << endl;
    outfile << "plot[:][:] '-' w l lt 3 lw 2, '-' w l lt 1" << endl << endl;
    outfile << "# bounding box" << endl;
    plotBoxPLT( outfile, _placement.boundryLeft(), _placement.boundryBottom(), _placement.boundryRight(), _placement.boundryTop() );
    outfile << "EOF" << endl;
    outfile << "# modules" << endl << "0.00, 0.00" << endl << endl;
    for( size_t i = 0; i < _placement.numModules(); ++i ){
        Module &module = _placement.module(i);
        plotBoxPLT( outfile, module.x(), module.y(), module.x() + module.width(), module.y() + module.height() );
    }
    outfile << "EOF" << endl;
    outfile << "pause -1 'Press any key to close.'" << endl;
    outfile.close();

    if( isPrompt ){
        char cmd[ 200 ];
        sprintf( cmd, "gnuplot %s", outfilename.c_str() );
        if( !system( cmd ) ) { cout << "Fail to execute: \"" << cmd << "\"." << endl; }
    }
}

void GlobalPlacer::plotBoxPLT( ofstream& stream, double x1, double y1, double x2, double y2 )
{
    stream << x1 << ", " << y1 << endl << x2 << ", " << y1 << endl
           << x2 << ", " << y2 << endl << x1 << ", " << y2 << endl
           << x1 << ", " << y1 << endl << endl;
}
