#include "cantera/zerodim.h"
#include "cantera/IdealGasMix.h"
#include "cantera/equilibrium.h" 

#include <fstream>

/*
PSR reactor execise Python to C++ of Ray's example in https://groups.google.com/forum/#!searchin/cantera-users/psr/cantera-users/dMUhi5kVVDk/L1ThqdXUPIYJ

It wil be used to integrate a CFD code with small timesteps so, mass conservation is important. So no H at the inlet as suggested by Charles.

Also the inlet is already mixed. You could "separate" the mix in 2 tanks but it looses the objective of the execise.

*/

using namespace Cantera;
/*
double mdot_out(double mass, double residence_time) 
{
	double r=mass/residence_time;
	return r;
}
*/
void runexample()
{

    // use reaction mechanism GRI-Mech 3.0
    IdealGasMix gas("gri30.cti", "gri30");
    int nsp = gas.nSpecies();
    gas.setState_TPX(300.0, OneAtm, "CH4:1.0, O2:0.21");
	//Creo reservorios aguas arriba y abajo para el reactor
    Reservoir upstream;
    Reservoir downstream;
	//Pone al objeto gas en estado de equilibrio para llenar el reactor en su 
	//estado incial (conserva la masa)
	equilibrate(gas,"HP");
	//Crea el reactor y lo llama r1
	//r1 = ct.IdealGasReactor(contents=gas)
	//energy='off' apaga la ecuación de energía al crear el reactor
    Reactor r1;
	r1.insert(gas);
    r1.setInitialVolume(1.0);

	double residence_time = 0.8e-4;
	

	// FIXME esta parte python->c++
	//original de Python
		//def mdot_out(t):
    	//	return r1.mass / residence_time
	//Q&D:
	double mdot_out;
	mdot_out=r1.mass()/residence_time;
	//C++C++2011 no deja definir funciones internas ó "lambda/closures" como 	int func = [] () { cout << "Hello world"; };
	// end FIXME
	//Poner una válvula a la entrada del reactor r1 para mantener P=cte y evitar 
	// que se devuelva el flujo. Véase  help(ct.Valve)
	//Nótese el uso del objeto upstream que es un tipo Reservoir devinido arriba
    Valve inlet;
    inlet.install(r1, downstream);
    double Kv = 100.0;
    inlet.setParameters(1, &Kv);
	//Pone un objeto MassFlowController a la salidad de r1 que mantiene el flujo 
	//másico constante, véase help(ct.MassFlowController)
	//Nótese el uso del objeto downstream que es un tipo Reservoir devinido arriba
	//outlet = ct.MassFlowController(r1, downstream)
	//fija el flujo másico a la salida del reactor
    MassFlowController outlet;
    outlet.install(downstream, r1);
	//FIXME fuel_mdor->mdot_out(t):
    outlet.setMassFlowRate(mdot_out);
	//END FIXME

    ReactorNet net;
    net.addReactor(&r1);

	//Creo los "pasos de tiempo" en los que quiero ver el progreso de la solución
	//El solver de cantera resuelve de forma automática y escoge su paso de tiempo 
	//de forma automática así t sea 2, 20 o 2000 puntos
	//FIXME
	//	t = np.linspace(0, residence_time, 2)
	//end FIXME
	//
	//¿Por qué el código original de Ray multiplica por 5 el tiempo de residencia?
	//t = np.linspace(0, 5*residence_time, 2)
	//

    // take single steps to 6 s, writing the results to a CSV file
    // for later plotting.
    double tfinal = 1.0e-7;
    double tnow = 0.0;
    double tres;
    int k;

    std::ofstream f("combustor_cxx.csv");
	net.advance(tfinal);
/*
    while (tnow < tfinal) {
        tnow = net.step(tfinal);
		//net.advance(tfinal);//Paso automático hasta tfinal
        tres = r1.mass()/inlet.massFlowRate();
        f << tnow << ", "
          << r1.temperature() << ", "
          << tres << ", ";
        ThermoPhase& c = r1.contents();
        for (k = 0; k < nsp; k++) {
            f << c.moleFraction(k) << ", ";
        }
        f << std::endl;
    }
    f.close();
*/
}

int main()
{

    try {
        runexample();
        return 0;
    }
    // handle exceptions thrown by Cantera
    catch (CanteraError& err) {
        std::cout << err.what() << std::endl;
        std::cout << " terminating... " << std::endl;
        appdelete();
        return 1;
    }
}
