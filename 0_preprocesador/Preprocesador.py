script para preprocesar manualmente las cosas de cantera a KIVA

#Haría falta alguna forma para que modificara el mecanismo y que deje las 12 especies de KIVA en el orden que es---> que el usuario lo haga
#Este script supone que el usuario ya organizó las 12 especies en el mecanismo

#Este procedimiento está regado en las notas del cacharreo cantera
python3
import cantera as ct
import numpy as np
#ct.add_directory('/home/carlos/Maestria/Materias/Tesis/Mecanismos')
gas1 = ct.Solution('Mech_KIVA_Cantera.cti')
gas1.TP=298.15,ct.one_atm
#http://homepages.cae.wisc.edu/~hessel/manualsAndEpilogues/kiva4itape5inputs/htform.html
hAdim_R=gas1.standard_enthalpies_RT*ct.gas_constant
htf=hAdim_R*298.15
#Convierto de J/kmol a kcal/mol
htf=htf*0.000239005736/1e3
#Ahora se imprime en el formato de KIVA ej:
#      o2   mw2     32.000  htf2     0.0
#Que corresponde al formato según rinput.f
#  730 format(a8,2x,2(a6,f10.5))
#a8: 8 espacios para letras
#2x: 2 espacios vacios
#2(a6,f10.5): 2 veces(6 letras y real en forma decinal de 10 espacios y 5 para los decimales -10 espacios contandl el punto-)
#Acá empieza lo nuevo
mw=gas1.molecular_weights
names=gas1.species_names
#Pasa a minúsculas
#gas1.species_names[1].lower()
nsp=gas1.n_species
cont=np.arange(12,nsp,dtype=np.int16)
#Para imprimir hay 2 alternativas:
#https://docs.python.org/3.1/tutorial/inputoutput.html
#Para formatear la salida/entrada véase
#https://docs.python.org/3.1/library/string.html#formatexamples
#Para el manejo de strings véase:
#https://docs.python.org/3.1/library/stdtypes.html#string-methods}
#[-6:] sirve para coger sólo 6 caracteres del string en caso de que haya más de 1000 especies
for value in cont:
	print(gas1.species_names[value].lower().rjust(8),
	'   '+'mw{0}'.format(value+1).ljust(6)[-6:]+'{0:10.5f}'.format(mw[value]),
	'htf{0}'.format(value+1).ljust(6)[-6:]+'{0:10.5f}'.format(htf[value]),
	sep='')

#A continuación se pone la parte de llenar los mfrac:
#mfracc5h11co,     0.000000E+00
#que lee rinput como:
#         read ( 5,*)      id(1),(mfrac(n,isp),n=1,nregions)
for value in cont:
	print(('mfrac'+gas1.species_names[value].lower()+',').ljust(18)
	+'{0:12.6E}'.format(0))

#A continuación se hace la parte de las entalpías para el archivo datahk
#http://homepages.cae.wisc.edu/~hessel/manualsAndEpilogues/kiva4itape5inputs/datahk.html


#Ahora se imprime en el formato de KIVA:
#Que corresponde al formato según rinput.f
#read(37,"(a8)") ispecies
#read(37,*) (ek(i,j+nspl),i=1,51)
#read(37,*) tcriti(j+nspl),pcriti(j+nspl),acentric(j+nspl)
T100=np.arange(0,5100,100,dtype=np.int16)
T100[0]=1#Cantera only accepts T>0
tCriti=np.zeros((nsp), dtype=np.float64)
pCriti=np.zeros((nsp), dtype=np.float64)
acentric=np.zeros((nsp), dtype=np.float64)
impCont=np.arange(0,51,1,dtype=np.int16)
Enth=np.ones((nsp,51), dtype=np.float64)
f = open('datahkNew', 'w')
for value in cont:
  for i in impCont:
    gas1.TP=T100[i],ct.one_atm
    Enth[:,i]=gas1.standard_enthalpies_RT*ct.gas_constant*T100[i]#in J/Kmol

for value in cont:
  f.write(''.join(gas1.species_names[value].lower().rjust(8)))
  f.write('\n')
  f.write(''.join(['{0:12.6E},'.format(Enth[value,i]*0.000239005736/1e3) for i in impCont]))#0.000239005736/1e3 J/kmol a kcal/mol
  f.write('\n')
  f.write(''.join('{0:12.6E},{1:12.6E},{2:12.6E}'.format(tCriti[value],pCriti[value],acentric[value])))
  f.write('\n')

f.close()

