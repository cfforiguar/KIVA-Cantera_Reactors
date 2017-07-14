#include <iostream>
#include "cantera/zerodim.h"
#include "cantera/IdealGasMix.h"
#include "cantera/zeroD/IdealGasReactor.h"
#include <vector>
#include <fstream>
#include <mpi.h>

using namespace Cantera;

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
  void SolveCVR(double temperatura,double CellVolume,double CellPressure,double tfinal, double * Y);
  void SetReactor();
  void SetReactorNetwork();
};
joinCVR::joinCVR()
    :gasComb("Mech_KIVA_Cantera.xml","gas")
    ,sim()
    ,combustor()
  //**Puedo inicializar el Reactor y el ReactorNetwork sin hacer gasComb.setState_TPY(...)
{
  nsp=gasComb.nSpecies();
  SetReactor();
  SetReactorNetwork();
}
void joinCVR::SetReactorNetwork(){//Reactor Network set up
  sim.addReactor(&combustor);  
  sim.setTolerances(1.0e-5,1.0e-9);//Review this carefully. It may need tune up
}

void joinCVR::SolveCVR(double temperatura,double CellVolume,double CellPressure,double tfinal, double * Y){//New solution
//sim.updateState(),  could be called to further optimize the code. But it is a low lever function. It should be used with care. I need more experience for that

  gasComb.setState_TPY(temperatura, CellPressure, Y);
  combustor.setInitialVolume(CellVolume);
  combustor.insert(gasComb);//Dejar esto acá que si no, no refresca el gas
  sim.setInitialTime (0.0);
	sim.advance(tfinal);//Avanza internamente y bota respuesta  
  ThermoPhase& c = combustor.contents();
  c.getMassFractions(Y);
}
void joinCVR::SetReactor(){//Reactor set up. It's Called once to set up the reactor.
  combustor.setEnergy(1);//0 = Energy eqn of the combustor is off 
}

void Paralelizar(double *temperatura,double *CellVolume,double *CellPressure,double *tfinal, double *Y,int *numcellsa, int *ct_nsp){
//  std::vector<joinCVR> ReacVect;
//  std::vector<joinCVR> ReacVect;
//  for(int i = 1; i<10 ++i){
//    ReacVect.push_back(joinCVR());
//  printf("Interfaz C++");
#pragma omp parallel
{
  joinCVR bicho;
  double Especies[ct_nsp[0]];
#pragma omp for //private(tfinal)
	for (int i = 0; i<numcellsa[0]; i++) {
//    std::cout << ct_nsp[0] <<"\n";
//    std::copy(Y+0, Y+ct_nsp[0],Especies);
    std::copy(Y+i*ct_nsp[0]
             ,Y+(i+1)*ct_nsp[0]
             ,Especies);
//  	for (int k = 0; k < ct_nsp[0]; k++) {
//      std::cout << Especies[k] <<", ";
//    }
//    std::cout << temperatura[i]<<", ";
//    std::cout << CellVolume[i]<<", ";
//    std::cout << CellPressure[i]<<", ";
//    std::cout << tfinal[0]<<", ";
//    std::cout << std::endl;
    bicho.SolveCVR(temperatura[i], CellVolume[i], CellPressure[i], tfinal[0], Especies);
//    std::cout << "\n";
//   	for (int k = 0; k < ct_nsp[0]; k++) {
//      std::cout << Especies[k] <<", ";
//    }
//    std::copy(Especies, Especies+ct_nsp[0],Y+0);
  std::copy(Especies, Especies+ct_nsp[0],Y+i*ct_nsp[0]);

 	std::ofstream f("datos_cpp.csv",std::ios_base::app);
	f.setf(std::ios_base::scientific);
	f.precision(20);
        for (int k = i*ct_nsp[0]; k < (i+1)*ct_nsp[0]; k++) {
          f << Y[k] << ", ";
        }
//        f << std::endl;
	f.close();
  };
}
};


extern "C"
int wrapper_c_(double *temperatura,double *CellVolume,double *CellPressure,double *tfinal, double *Y,int *numcellsa, int *ct_nsp, int *fComm )
{
// mpi vars
  MPI_Comm comm;
  int id;
  int ierr;
  int p;
  try {
  /*
    Convert Fortran MPI Communicator ID to C.
  */
    comm = MPI_Comm_f2c ( *fComm );
  /*
    Determine this processes's rank.
  */
    ierr = MPI_Comm_rank ( comm, &id );
  /*
    Determine the number of processes.
  */
    ierr = MPI_Comm_size ( comm, &p );

  //Cosas para borrar
//    std::cout << "Interfaz C" << std::endl;
//    std::cout << ct_nsp[0] << std::endl;
  //Fin: Cosas para borrar
    Paralelizar(temperatura,CellVolume,CellPressure,tfinal
                 ,Y,numcellsa, ct_nsp);
                 
    	for (int i = 0; i<numcellsa[0]; i++) {
 	std::ofstream f("datos_cpp.csv",std::ios_base::app);
	f.setf(std::ios_base::scientific);
	f.precision(20);
        for (int k = i*ct_nsp[0]; k < (i+1)*ct_nsp[0]; k++) {
          f << Y[k] << ", ";
        }
//        f << std::endl;
	f.close();
  };

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
