#include <iostream>
#include "cantera/zerodim.h"
#include "cantera/IdealGasMix.h"
#include "cantera/zeroD/IdealGasReactor.h"

#include <fstream>

using namespace Cantera;

void UnitTests(){
/*
//Unit test: Variables
    int nsp = gasIn.nSpecies(),k;
    double MwI=0.0,MwF=0.0,EI=0.0,EF=0.0,EKIVA=0.0,
    VwF=0.0,VwI=0.0,PwF=0.0,PwI=0.0,TwF=0.0,TwI=0.0;
    double *compositionI,*PrintIt,*nonDimSSEnthalpy_RT,Htf_i;
    compositionI=new double[nsp];
    PrintIt=new double[nsp];
    nonDimSSEnthalpy_RT=new double[nsp];  
    for (k = 0; k < nsp; k++) {
        compositionI[k] = Y[k];
    }  
//END Unit test:  Variables
*/    

/*
//Unit test
    ThermoPhase& cI = combustor.contents();
    cI.getMassFractions(compositionI);
    VwI=combustor.volume();
    PwI=combustor.pressure();
    TwI=combustor.temperature();
    MwI=combustor.density()*VwI;
//END Unit test
*/


/*
//Unit test: Final values   
        VwF=combustor.volume();
        PwF=combustor.pressure();
        TwF=combustor.temperature();
        MwF=c.density()*VwF;
//END Unit test: Final values
	std::cout << "Masa final-Masa inicial:" << MwF-MwI << std::endl;
	std::cout << "P final-P inicial:" << PwF-PwI << std::endl;
	std::cout << "Vol final-Vol inicial:" << VwF-VwI << std::endl;
	std::cout << "T final-T inicial:" << TwF-TwI << std::endl;
	std::cout << "Mfrac final-Mfrac inicial:"  << std::endl;
        for (k = 0; k < nsp; k++) {
            std::cout << Y[k]-compositionI[k] <<", ";
        }
        std::cout<< std::endl;
        std::cout << "Delta rho KIVA:" 
        << (c.density()-gasIn.density())*1.0e-3 << std::endl;
        std::cout << "Delta sie ct:" 
        << c.intEnergy_mass()-gasIn.intEnergy_mass() << 
        " Delta sie KIVA:" << 
        (c.intEnergy_mass()-gasIn.intEnergy_mass())*(1.0e7/1.0e3) 
        << std::endl;
        gasIn.setState_TPY(298.0, OneAtm, compositionI);	
	gasIn.getEnthalpy_RT_ref(nonDimSSEnthalpy_RT);
	for (k = 0; k < nsp; k++) {
	    //[R]=J( K)−1(k mol)−1,(k mol)−1= (1000 mol)−1
       	    Htf_i= nonDimSSEnthalpy_RT[k]*GasConstant*298.15;
       	    //std::cout << Htf_i << std::endl;
            EKIVA=EKIVA-((Y[k]-compositionI[k])/tfinal)*(Htf_i/
	    c.molecularWeight(k));
        }
	std::cout << "sie según KIVA:" << EKIVA*(1.0e7/(1.0e3)) << std::endl;
        gasIn.setState_TPY(temperatura, CellPressure, compositionI);
//    salida = composition;
*/
}

class joinCVR:
  public IdealGasPhase,
  public GasKinetics,
  public Reactor
{
public:
  IdealGasMix gasComb;
  IdealGasReactor combustor;// create the combustor
  ReactorNet sim;
  joinCVR();
  int nsp;
//  double temperatura;
//  double CellVolume;
//  double CellPressure;
//  double tfinal;
//  double fuel_mdot;
//  double * Y;
  void tester(double temperatura,double CellVolume,double CellPressure,double tfinal,double fuel_mdot, double * Y);
  void testerII(double CellVolume,double tfinal,double fuel_mdot, double * Y);
  void SetReactorNetwork();
};
joinCVR::joinCVR()
    :gasComb("Mech_KIVA_Cantera.xml","gas")
    ,sim()
    ,combustor()
{
  nsp=gasComb.nSpecies();
}
void joinCVR::SetReactorNetwork(){//New solution
//sim.updateState(),  sim.setInitialTime (0.0) and sim.advance() will be called somewere else several times over

//sim.updateState() is a low level function 
  //**Cuando se modifica el gas luego de meter el Reactor al ReactorNetwork, Cantera no ve los cambios
  //**Además puedo inicializar el Reactor y el ReactorNetwork sin hacer gasComb.setState_TPY(...)
  sim.addReactor(&combustor);  
  sim.setTolerances(1.0e-5,1.0e-9);//Review this carefully. It may need tune up
}
void joinCVR::tester(double temperatura,double CellVolume,double CellPressure,double tfinal,double fuel_mdot, double * Y){//Reactor Network set up
//This is going to become the reactor network.Called once to set up the reactor network. 
  testerII(CellVolume,tfinal,fuel_mdot,Y);
  SetReactorNetwork();
  
  gasComb.setState_TPY(temperatura, CellPressure, Y);
  combustor.setInitialVolume(CellVolume);
  combustor.insert(gasComb);
  sim.setInitialTime (0.0);
	sim.advance(tfinal);//Avanza internamente y bota respuesta  
  ThermoPhase& c = combustor.contents();
  c.getMassFractions(Y);
}
void joinCVR::testerII(double CellVolume,double tfinal,double fuel_mdot, double * Y){//Reactor set up
//This is going to become the reactor. Called once to set up the reactor.
  combustor.setEnergy(1);//0 = Energy eqn of the combustor off 
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
ins[4]=0.0;
	//FIXME: -Inicializar el gas en otro lado
	//       -Ver si el gas cambia para sólo usar uno para todos los reactores
	//	 -usar una ct_nsp para manejar aparte el nsp de kiva
	IdealGasMix gas("Mech_KIVA_Cantera.xml","gas");
	int k,nsp=gas.nSpecies();
	//end FIXME

	/*Comandos para saber a qué indice coresponde un compuesto
	python3
	import cantera as ct
	gas1=ct.Solution("gri30.cti")
	gas1.species_index('AR')
	*/
 

  joinCVR test;
	double *PrintIt;
	PrintIt = new double[nsp];
        gas.setState_TPY(ins[0], ins[2], salida);
        gas.getNetProductionRates(PrintIt);//Species net production rates [kmol/m^3/s]. 
	for (k = 0; k < nsp; k++) {
            if (PrintIt[k]!=0.0)
            {
              test.tester(ins[0], ins[1], ins[2], ins[3],ins[4], salida);
//              test.tester(ins[0], ins[1], salida);
              break;
            };
        }

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
	
    delete [] PrintIt;
    return 0;
    }
    // handle exceptions thrown by Cantera
    catch (CanteraError& err) {
        std::cout << "En la celda:"<< ins[5] << std::endl;
        std::cout << err.what() << std::endl;
        std::cout << " terminating... " << std::endl;
        appdelete();
        return 1;
    }
}
int main()
{
	joinCVR test;
	double *ins, *salida;
	int k;
	static int nsp=test.nsp;
	salida = new double[nsp];
	for (k = 0; k < nsp; k++) {
            salida[k]= 0.0;
        }
	salida[0]=0.16;//2.4343383649779241e-18;//c7h16
	salida[1]=0.21;//O2
	salida[2]=0.79;//N2
  ins = new double[4];
  //The values i need to test are not commented
	ins[0]=1477.76999999999998;//temperatura [k];
                //tested from 1.900e2 to  4.0 e3
	ins[1]=5.892793593225408e-13;//CellVolume[m3];
                //ok from 5.892793593225408e10 to 5.892793593225408e-07
                //fails from 5.892793593225408e-08 and smaller
	ins[2]=2680000.0000000009;//CellPressure[Pa]; 
                //tested from 2.02650000000003e-6 to  2.02650000000003e16
	ins[3]=1.2991316826994651e-05;//tfinal[s];
	ins[4]=0.0;//fuel_mdot[kg/s];
                //tested from 5.6761327713201428e-20 to 5.6761327713201428e10
  wrapper_c_(ins, salida);
  delete [] ins;
  delete [] salida;  
  return 0;
}
