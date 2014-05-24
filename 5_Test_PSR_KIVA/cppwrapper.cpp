#include <iostream>
#include "cantera/zerodim.h"
#include "cantera/IdealGasMix.h"

#include <fstream>

using namespace Cantera;
void runexample(double temperatura,double CellVolume,double CellPressure,double tfinal,double fuel_mdot, double * Y)
{
	//TODO: Cambiar el nombre del mecanismo por uno estático.
	IdealGasMix gas("Mech_KIVA_Cantera.cti");
    //IdealGasMix gas("gri30.cti", "gri30");//Da lo mismo que la línea anterior
//FIXME
    //Averiguar el tipo gas para pasarlo a la función
    int nsp = gas.nSpecies();
//end FIXME
    //double composition(nsp)//

    //create a reservoir for the fuel inlet, and set to cell's composition.
    Reservoir fuel_in;
    gas.setState_TPY(temperatura,CellPressure, Y);
    fuel_in.insert(gas);
    double fuel_mw = gas.meanMolecularWeight();


    // create the combustor, and fill it in initially with N2

    //gas.setState_TPX(300.0, OneAtm, "N2:1.0");//Interesante resultado, el reactor tarda en sacar el nitrógeno OJO KIVA
    gas.setState_TPY(temperatura, CellPressure, Y);
    Reactor combustor;
    combustor.insert(gas);
    combustor.setInitialVolume(CellVolume);


    // create a reservoir for the exhaust. The initial composition
    // doesn't matter.
    Reservoir exhaust;
    exhaust.insert(gas);

    // create and install the mass flow controllers. Controllers
    // m1 and m2 provide constant mass flow rates, and m3 provides
    // a short Gaussian pulse only to ignite the mixture
    MassFlowController m1;
    m1.install(fuel_in, combustor);
    m1.setMassFlowRate(fuel_mdot);

    // put a valve on the exhaust line to regulate the pressure
    Valve v;
    v.install(combustor, exhaust);
    double Kv = 1.0;
    v.setParameters(1, &Kv);

    // the simulation only contains one reactor
    ReactorNet sim;
    sim.addReactor(&combustor);

    // take single steps to 6 s, writing the results to a CSV file
    // for later plotting.
    double tnow = 0.0;
    double tres;
    int k;


	sim.advance(tfinal);//Avanza internamente y bota respuesta
	tnow = tfinal; //Como ya avanzó, el tiempo final es el mismo tnow
        tres = combustor.mass()/v.massFlowRate();

       ThermoPhase& c = combustor.contents();

       c.getMassFractions(Y);


//    salida = composition;

//Salidas:
    //Las salidas se pasan a las unidades de KIVA en la interfase.
    //Densidades: Y->densidades(SI)

    //c.massFraction()*rho

    //Delta Energía: (SI)//¿Entalpía ó energía interna?->depende del reactor

    //H_Inic(gas)-H_Fin(gas)
}

extern "C"
int wrapper_c_(double *ins, double *salida)
{
    try {
/*
	ins[0]=temperatura;
	ins[1]=CellVolume;
	ins[2]=CellPressure;
	ins[3]=tfinal;
	ins[4]=fuel_mdot;
*/
	//FIXME: -Inicializar el gas en otro lado
	//       -Ver si el gas cambia para sólo usar uno para todos los reactores
	//	 -usar una ct_nsp para manejar aparte el nsp de kiva
	IdealGasMix gas("Mech_KIVA_Cantera.cti","gas");
	int k,nsp=gas.nSpecies();
	//end FIXME

	/*Comandos para saber a qué indice coresponde un compuesto
	python3
	import cantera as ct
	gas1=ct.Solution("gri30.cti")
	gas1.species_index('AR')
	*/

        runexample(ins[0], ins[1], ins[2], ins[3],ins[4], salida);
/* 
// Unit test for the interface comunication
   	std::ofstream f("datos_cpp.csv",std::ios_base::app);
	f.setf(std::ios_base::scientific);
	f.precision(20);
        for (k = 0; k < nsp; k++) {
            f << salida[k] << ", ";
        }
        f << std::endl;
	f.close();
// END Unit test for the interface comunication
*/	
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
