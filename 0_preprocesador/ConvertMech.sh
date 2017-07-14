#!/bin/bash
export LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib/boost:/usr/local/include/boost:/usr/lib:/home/carlos/opt/sundials-intel/include:/home/carlos/opt/sundials-intel/lib:$LD_LIBRARY_PATH
source /home/carlos/.local/bin/setup_cantera
source /opt/intel/bin/compilervars.sh intel64

ck2cti --permissive  --input=chem.inp --thermo=therm.dat --id=gas --output=Mech_KIVA_Cantera.cti
