
// A combustor. Two separate stream - one pure methane and the other
// air, both at 300 K and 1 atm flow into an adiabatic combustor where
// they mix. We are interested in the steady-state burning
// solution. Since at 300 K no reaction will occur between methane and
// air, we need to use an 'igniter' to initiate the chemistry. A simple
// igniter is a pulsed flow of atomic hydrogen. After the igniter is
// turned off, the system approaches the steady burning solution."""

#include "cantera/zerodim.h"
#include "cantera/IdealGasMix.h"

#include <fstream>

using namespace Cantera;

void runexample(double temperatura,double CellVolume,double CellPressure,double tfinal,double fuel_mdot, double * & salida)
{

//Entradas desde KIVA
    //KIVA temperatura
    //int temperatura=2900.0;
//FIXME
//Hacer que pueda usar cualquier nombre de mecanismo con una longitud arbitraria
    // use reaction mechanism GRI-Mech 3.0
//    char mechanism[]="gri30.cti\0"
//end FIXME
	IdealGasMix gas("gri30.cti");
    //IdealGasMix gas("gri30.cti", "gri30");//Da lo mismo que la línea anterior
//FIXME
    //Averiguar el tipo gas para pasarlo a la función
    int nsp = gas.nSpecies();
//end FIXME
    //double composition(nsp)//
    //create a reservoir for the fuel inlet, and set to pure methane.
    //double CellVolume=1;
    //double CellPressure=OneAtm;
    //double tfinal = 1.0e-6;//KIVA time step is reactor's final time
    //double fuel_mdot = 20.0; //fuel_mdot=CellDensity*CellVolume/tfinal
//FIXME
    //Pasar un array con la composición en número no una cadena
//end FIXME

    Reservoir fuel_in;
    gas.setState_TPX(temperatura,CellPressure, "CH4:1.0, N2:0.78, O2:0.21, AR:0.0");
    fuel_in.insert(gas);
    double fuel_mw = gas.meanMolecularWeight();


    // create the combustor, and fill it in initially with N2

    //gas.setState_TPX(300.0, OneAtm, "N2:1.0");//Interesante resultado, el reactor tarda en sacar el nitrógeno OJO KIVA
    gas.setState_TPX(temperatura, CellPressure, "CH4:1.0, N2:0.78, O2:0.21, AR:0.0");
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

    std::ofstream f("combustor_cxx.csv");

	sim.advance(tfinal);//Avanza internamente y bota respuesta
	tnow = tfinal; //Como ya avanzó, el tiempo final es el mismo tnow
        tres = combustor.mass()/v.massFlowRate();
        f << tnow << ", "
          << combustor.temperature() << ", "
          << tres << ", ";
        ThermoPhase& c = combustor.contents();
        for (k = 0; k < nsp; k++) {
            f << c.moleFraction(k) << ", ";
        }
        f << std::endl;
    f.close();
//    double composition;
//    salida = new double[nsp];
    for (k = 0; k < nsp; k++) {
         salida[k]=c.moleFraction(k);
    }

//    salida = composition;

//Salidas:
    //Las salidas se pasan a las unidades de KIVA en la interfase.
    //Densidades: Y->densidades(SI)

    //c.massFraction()*rho

    //Delta Energía: (SI)//¿Entalpía ó energía interna?->depende del reactor

    //H_Inic(gas)-H_Fin(gas)
}


int main()
{

    try {
        double temperatura=2900.0;
        double CellVolume=1;
        double CellPressure=OneAtm;
        double tfinal = 1.0e-6;//KIVA time step is reactor's final time
        double fuel_mdot = 20.0;//fuel_mdot=CellDensity*CellVolume/tfinal
	double *salida;
	IdealGasMix gas("gri30.cti");
	int k,nsp=gas.nSpecies();
	salida = new double[nsp];
        for (k = 0; k < nsp; k++) {
            salida [k]=0.0;
        }
	/*Comandos para saber a qué indice coresponde un compuesto
	python3
	import cantera as ct
	gas1=ct.Solution("gri30.cti")
	gas1.species_index('AR')
	*/
	//"CH4:1.0, N2:0.78, O2:0.21, AR:0.0");
	salida[13]=1.0;//CH4:1.0
	salida[47]=0.78;//N2:0.78
	salida[3]=0.21;//O2:0.2
	salida[48]=0.0;//AR:0.0

        runexample(temperatura, CellVolume, CellPressure, tfinal,fuel_mdot, * & salida);
        for (k = 0; k < nsp; k++) {
            std::cout << salida[k] << std::endl;
        }
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
