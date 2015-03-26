#include <iostream>
#include <fstream>


extern "C"
int wrapper_c_(double *salida)
{
	double test;
//**	test=*ins+*ins;
	std::cout << "C++ wrapper" << std::endl;
  int k;


////**      double temperatura=2900.0;
////**      ins[4]=fuel_mdot;
//	//FIXME: -Inicializar el gas en otro lado
//	//       -Ver si el gas cambia para sólo usar uno para todos los reactores
//	//	 -usar una ct_nsp para manejar aparte el nsp de kiva


////**	salida[48]=0.0;//AR:0.0

////        runexample(ins[0], CellVolume, CellPressure, tfinal,fuel_mdot, * & salida);
  for (k = 0; k < 21*20; k++) {
    std::cout << salida[k]<<" "; //<< std::endl;
  }
 	std::ofstream f("datos_cpp.csv",std::ios_base::app);
	f.setf(std::ios_base::scientific);
	f.precision(20);
	//cout.precision(10);
	//f << std::scientific <<;
        for (k = 0; k < 21*20; k++) {
            f << salida[k] << ", ";
        }
        f << std::endl;
	f.close();


        return 0;

}
