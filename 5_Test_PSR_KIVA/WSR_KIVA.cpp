#include <iostream>
#include "cantera/zerodim.h"
#include "cantera/IdealGasMix.h"
#include "cantera/zeroD/IdealGasReactor.h"
#include "cantera/equilibrium.h"

#include <fstream>

using namespace Cantera;

void runexample(double temperatura,double CellVolume,double CellPressure,double tfinal,double fuel_mdot, double * Y)
{
/*
std::cout << "Temperatura I "<< temperatura << std::endl;
std::cout << "Volumen I "<< CellVolume << std::endl;
std::cout << "Presión I "<< CellPressure << std::endl;
std::cout << "Tiempo "<< tfinal << std::endl;
std::cout << "Flujo másico "<< fuel_mdot << std::endl;
*/
    IdealGasMix gasIn("Mech_KIVA_Cantera.xml","gas");
    IdealGasMix gasComb("Mech_KIVA_Cantera.xml","gas");

//FIXME
    //Averiguar el tipo gas para pasarlo a la función
    int nsp = gasIn.nSpecies(), k;
    double MwI=0.0,MwF=0.0,EI=0.0,EF=0.0,EKIVA=0.0,
    VwF=0.0,VwI=0.0,PwF=0.0,PwI=0.0,TwF=0.0,TwI=0.0;//unit test:Comprobación de la masa inicial y final
    
//end FIXME
    double *compositionI,*PrintIt,*nonDimSSEnthalpy_RT,Htf_i;
    compositionI=new double[nsp];
    PrintIt=new double[nsp];
    nonDimSSEnthalpy_RT=new double[nsp];
    /*
    for (k = 0; k < nsp; k++) {
        compositionI[k] = Y[k];
    }
    */
    
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
    combustor.setEnergy(1);//0 = Energy off
//Unit test
    ThermoPhase& cI = combustor.contents();
    cI.getMassFractions(compositionI);
    VwI=combustor.volume();
    PwI=combustor.pressure();
    TwI=combustor.temperature();
    MwI=combustor.density()*VwI;
//END Unit test
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

    // put a  mass flow controller on the exhaust line to regulate the mass
    MassFlowController m2;
    m2.install(combustor, exhaust);
    m2.setMassFlowRate(fuel_mdot);

    // the simulation only contains one reactor
    ReactorNet sim;
    sim.addReactor(&combustor);
    sim.setTolerances(1e-50,1.0e-9);

    // take single steps to 6 s, writing the results to a CSV file
    // for later plotting.
    double tnow = 0.0;
//    double tres;



	sim.advance(tfinal);//Avanza internamente y bota respuesta
//	tnow = tfinal; //Como ya avanzó, el tiempo final es el mismo tnow
//        tres = combustor.mass()/v.massFlowRate();
	
       ThermoPhase& c = combustor.contents();
       c.getMassFractions(Y);   
        
        VwF=combustor.volume();
        PwF=combustor.pressure();
        TwF=combustor.temperature();
        MwF=c.density()*VwF;
	std::cout << "milestone" << std::endl;
//	std::cout << "Temperatura Final:" << combustor.temperature() << std::endl;
//	std::cout << "Presión Final:" << combustor.pressure() << std::endl;
//	std::cout << "Masa inicial:" << MwI << std::endl;
//	std::cout << "Masa final:" << MwF << std::endl;
	std::cout << "Masa final-Masa inicial:" << MwF-MwI << std::endl;
	std::cout << "P final-P inicial:" << PwF-PwI << std::endl;
	std::cout << "Vol final-Vol inicial:" << VwF-VwI << std::endl;
	std::cout << "T final-T inicial:" << TwF-TwI << std::endl;
	std::cout << "Mfrac final-Mfrac inicial:"  << std::endl;
        for (k = 0; k < nsp; k++) {
            std::cout << Y[k]-compositionI[k] <<", ";
        }
        std::cout<< std::endl;
	//P*V+u*MasaInic:
	EI=CellPressure*CellVolume+gasIn.intEnergy_mass()*MwI+gasIn.cv_mass()*MwI*temperatura;
	std::cout << "Energía total inicial:" << EI << std::endl;
	//P*V+u*MasaFin:
	EF=combustor.pressure()*CellVolume+c.intEnergy_mass()*MwF+c.cv_mass()*MwF*combustor.temperature();
	std::cout << "Energía final:" << EF << std::endl;
	std::cout << "Energía final-Energía inicial:" << EF-EI 
	<< std::endl;
//https://groups.yahoo.com/neo/groups/cantera/conversations/topics/1428	
        std::cout << "rhoI ct:" << gasIn.density() << std::endl;
        std::cout << "rhoF ct:" << c.density() << std::endl;
        std::cout << "Delta rho KIVA:" 
        << (c.density()-gasIn.density())*1.0e-3 << std::endl;        
        std::cout << "Delta sie ct:" 
        << c.intEnergy_mass()-gasIn.intEnergy_mass() << 
        " Delta sie KIVA:" << 
        (c.intEnergy_mass()-gasIn.intEnergy_mass())*(1.0e7/1.0e3) 
        << std::endl;
//        std::cout << "spd ct:" << c.intEnergy_mass() << "spd KIVA:" << << std::endl;
        gasIn.setState_TPY(298.0, OneAtm, compositionI);	
	gasIn.getEnthalpy_RT_ref(nonDimSSEnthalpy_RT);
	for (k = 0; k < nsp; k++) {
	    //[R]=J( K)−1(k mol)−1,(k mol)−1= (1000 mol)−1
       	    Htf_i= nonDimSSEnthalpy_RT[k]*GasConstant*298.15;
       	    //std::cout << Htf_i << std::endl;
       	    //std::cout << (Y[k]-compositionI[k])/tfinal << std::endl;
            EKIVA=EKIVA-((Y[k]-compositionI[k])/tfinal)*(Htf_i/
	    c.molecularWeight(k));
        }
	std::cout << "sie según KIVA:" << EKIVA*(1.0e7/(1.0e3)) << std::endl;
        gasIn.setState_TPY(temperatura, CellPressure, compositionI);
        std::cout << "Ratas de prod. instantánea:"  << std::endl;
        gasIn.getNetProductionRates(PrintIt);
        for (k = 0; k < nsp; k++) {
            std::cout << PrintIt[k] <<", ";
        }
        std::cout << std::endl;       
        std::cout << "Mfrac en equilibrio:"  << std::endl;
        gasIn.setState_TPY(temperatura, CellPressure, compositionI);
        equilibrate(gasIn, "TP");
        gasIn.getMassFractions(PrintIt);
        for (k = 0; k < nsp; k++) {
            std::cout << PrintIt[k] <<", ";
        }
        std::cout << std::endl;
//    salida = composition;

//Salidas:
    //Las salidas se pasan a las unidades de KIVA en la interfase.
    //Densidades: Y->densidades(SI)

    //c.massFraction()*rho

    //Delta Energía: (SI)//¿Entalpía ó energía interna?->depende del reactor

    //H_Inic(gas)-H_Fin(gas)
    
    delete [] compositionI;
    delete [] PrintIt;
    delete [] nonDimSSEnthalpy_RT;

    
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
    IdealGasMix gas("Mech_KIVA_Cantera.xml","gas");

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

	for (k = 0; k < nsp; k++) {
            salida[k]= 0.0;
        }
	salida[0]=0.16;//2.4343383649779241e-18;//c7h16
	salida[1]=0.21;//O2
	salida[2]=0.79;//N2
	
	double *PrintIt,*ct_Mw,meanMw=0.0,nMol,fTMol=0.0,*X,*mol;
	PrintIt = new double[nsp];
	ct_Mw = new double[nsp];
	X = new double[nsp];
	mol= new double[nsp];
//	salida = new double[nsp];
        gas.setState_TPY(ins[0], ins[2], salida);
        gas.getNetProductionRates(PrintIt);//Species net production rates [kmol/m^3/s]. 
        std::cout << "Ratas de prod. neta instantánea:"  << std::endl;
        for (k = 0; k < nsp; k++) {
            std::cout << PrintIt[k] <<", ";
        }
        std::cout << std::endl;
        gas.getMolecularWeights(ct_Mw);//Output array of molecular weights (kg/kmol) 
        std::cout << "Masas molares: "  << std::endl;
        for (k = 0; k < nsp; k++) {
            std::cout << ct_Mw[k] <<", ";
        }
        std::cout << std::endl;
        gas.getMoleFractions(X);
        std::cout << "Fracción molar inicial: "  << std::endl;
        for (k = 0; k < nsp; k++) {
            std::cout << X[k] <<", ";
        }
        std::cout << std::endl;
        nMol=ins[1]*ins[2]/(GasConstant*ins[0]);//[kmol]=PV/(RT), [GasConstant]=J/kmo-K
        std::cout << "Moles iniciales: "<< nMol  << std::endl;
        std::cout << "Moles nuevas: "  << std::endl;        
	for (k = 0; k < nsp; k++) {
	   mol[k]=X[k]*nMol+ins[3]*PrintIt[k]*ins[1];//y-y0=(x-x0)*m, [mol]=kmol
           std::cout << mol[k] << ", ";
	   fTMol=fTMol+mol[k];//kmol
           meanMw=meanMw+ct_Mw[k]*X[k];//[meanMw]=kg/kmol
        }
        std::cout << std::endl;
        std::cout << "Total de moles nuevas: "<< fTMol  << std::endl;
        std::cout << "Masa molar media : "<< meanMw  << std::endl;
        std::cout << "Fracciónes másicas nuevas: "<< std::endl;
	for (k = 0; k < nsp; k++) {
	   X[k]=mol[k]/fTMol;
	   salida[k]=X[k]*ct_Mw[k]/meanMw;
           std::cout << salida[k] <<", ";
        }

	std::cout << "milestone2" << std::endl;	
//runexample(double temperatura,double CellVolume,double CellPressure,double tfinal,double fuel_mdot, double * Y)
// Unit test for the interface comunication
runexample(ins[0], ins[1], ins[2], ins[3],ins[4], salida);
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
	delete [] ct_Mw;
	delete [] X;
	delete [] mol;
	delete [] salida;
	delete [] ins;
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
