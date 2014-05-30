#include <iostream>
#include "cantera/zerodim.h"
#include "cantera/IdealGasMix.h"
#include "cantera/zeroD/IdealGasReactor.h"

#include <fstream>

using namespace Cantera;

void runexample(double temperatura,double CellVolume,double CellPressure,double tfinal,double fuel_mdot, double * Y)
{
std::cout << temperatura << std::endl;
std::cout << CellVolume << std::endl;
std::cout << CellPressure << std::endl;
std::cout << tfinal << std::endl;
std::cout << fuel_mdot << std::endl;

    IdealGasMix gasIn("Mech_KIVA_Cantera.cti","gas");
    IdealGasMix gasComb("Mech_KIVA_Cantera.cti","gas");

//FIXME
    //Averiguar el tipo gas para pasarlo a la función
    int nsp = gasIn.nSpecies();
//end FIXME
    //double composition(nsp)//

    //create a reservoir for the fuel inlet, and set to cell's composition.
    Reservoir fuel_in;
    gasIn.setState_TPY(temperatura,CellPressure, Y);
    fuel_in.insert(gasIn);
    double fuel_mw = gasIn.meanMolecularWeight();


    // create the combustor, and fill it in initially with N2

    //gas.setState_TPX(300.0, OneAtm, "N2:1.0");//Interesante resultado, el reactor tarda en sacar el nitrógeno OJO KIVA
    gasComb.setState_TPY(temperatura, CellPressure, Y);
    IdealGasReactor combustor;
    combustor.insert(gasComb);
    combustor.setInitialVolume(CellVolume);


    // create a reservoir for the exhaust. The initial composition
    // doesn't matter.
    Reservoir exhaust;
    exhaust.insert(gasComb);

  
  // create and install the mass flow controllers. Controllers
    // m1 and m2 provide constant mass flow rates, and m3 provides
    // a short Gaussian pulse only to ignite the mixture
    MassFlowController m1;
    m1.install(fuel_in, combustor);
    m1.setMassFlowRate(fuel_mdot);

    // put a valve on the exhaust line to regulate the pressure
    Valve v;
    v.install(combustor, exhaust);
    double Kv = 1.0e-0;
    v.setParameters(1, &Kv);

    // the simulation only contains one reactor
    ReactorNet sim;
    sim.addReactor(&combustor);
    sim.setTolerances(1e-50,1.0e-9);

    // take single steps to 6 s, writing the results to a CSV file
    // for later plotting.
    double tnow = 0.0;
//    double tres;
//    int k;


	sim.advance(tfinal);//Avanza internamente y bota respuesta
//	tnow = tfinal; //Como ya avanzó, el tiempo final es el mismo tnow
//        tres = combustor.mass()/v.massFlowRate();

       ThermoPhase& c = combustor.contents();
       c.getMassFractions(Y);
	std::cout << "milestone" << std::endl;
        std::cout << combustor.temperature() << std::endl;
        std::cout << combustor.pressure() << std::endl;

//    salida = composition;

//Salidas:
    //Las salidas se pasan a las unidades de KIVA en la interfase.
    //Densidades: Y->densidades(SI)

    //c.massFraction()*rho

    //Delta Energía: (SI)//¿Entalpía ó energía interna?->depende del reactor

    //H_Inic(gas)-H_Fin(gas)
}

/*


Reactor "OK", con valores negativos
  ins
$8 = {0.063000000000000014, 0.218, 0.71899999999999997, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
(idb) 
$9 = {291.94999999999999, 5.8927935932253858e-13, 7500000, 9.9999999999999995e-07, 5.4998796853503451e-07, 0, 0, 0, 0, 0}
  outs
$10 = {0.063000000000000014, 0.218, 0.71899999999999997, 1.1264513407326086e-113, 2.9894174119354749e-40, 3.9521855473664602e-56, -2.4196569671246096e-56, 1.6485501151690446e-62, 0, 5.8280186724042078e-42, 5.9956370057393033e-82, 0, 1.7805029807059307e-41, 8.9086495883540873e-32, 7.5924780792461835e-115, 8.1782834608248607e-62, 7.0463517242564463e-95, 9.8967704799945667e-117, 3.2051534329617427e-88, 7.0927047650188194e-114, 3.6371125007923386e-95, 1.3497854996925895e-51, 1.2543698768718194e-54, 8.722732759869291e-147, 7.2446628745138333e-142, 1.0132472734189619e-51, 9.2231272923659291e-82, 2.726000624400925e-34, 3.5373968507255294e-31, 2.4758801725795698e-39, 2.7006316254876312e-61}

Reactor Ok

*/


extern "C"
int wrapper_c_()
{
    try {
	double *ins,*salida;

	//FIXME: -Inicializar el gas en otro lado
	//       -Ver si el gas cambia para sólo usar uno para todos los reactores
	//	 -usar una ct_nsp para manejar aparte el nsp de kiva
    IdealGasMix gas("Mech_KIVA_Cantera.cti","gas");
	int k,nsp=gas.nSpecies();
	//end FIXME
	salida = new double[nsp];
        ins = new double[4];
	/*Comandos para saber a qué indice coresponde un compuesto
	python3
	import cantera as ct
	gas1=ct.Solution("gri30.cti")
	gas1.species_index('AR')
	*/
/*
If you leave:
        ins[1]=5.892793593225408e-02;//CellVolume[m3]
You can stress test everything and it works 
But if if you use:
        ins[1]=5.892793593225408e-13;//CellVolume[m3]
Everything breaks and then "CanteraError thrown by CVodesIntegrator:
 CVodes error encountered. Error code: -3"
*/

//The values i need to test are not commented
	ins[0]=797.19180707280918;//temperatura [k];
                //tested from 1.900e2 to  4.0 e3
	ins[1]=6.2635104999482856e-13;//CellVolume[m3];
                //ok from 5.892793593225408e10 to 5.892793593225408e-07
                //fails from 5.892793593225408e-08 and smaller
	ins[2]=2952763.2293912154;//CellPressure[Pa]; 
                //tested from 2.02650000000003e-6 to  2.02650000000003e16
	ins[3]=3.9076705530412464e-06;//tfinal[s];
	ins[4]=2.0600368693814045e-06;//fuel_mdot[kg/s];
                //tested from 5.6761327713201428e-20 to 5.6761327713201428e10

	for (k = 0; k < nsp; k++) {
            salida[k]= 0.0;
        }
	salida[0]=2.4343383649779241e-18;//c7h16
	salida[1]=0.2320000000000001;//O2
	salida[2]=0.7679999999999999;//N2
        runexample(ins[0], ins[1], ins[2], ins[3],ins[4], salida);
	std::cout << "milestone2" << std::endl;
//runexample(double temperatura,double CellVolume,double CellPressure,double tfinal,double fuel_mdot, double * Y)
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

int main()
{
	int dummy;
	dummy=wrapper_c_();
	return 0;
}
