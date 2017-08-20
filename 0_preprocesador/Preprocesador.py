#! /home/carlos/opt/anaconda3/bin/python3
import re

#script para preprocesar manualmente las cosas de cantera a KIVA

#Haría falta alguna forma para que modificara el mecanismo y que deje las 12 especies de KIVA en el orden que es---> que el usuario lo haga
#Este script supone que el usuario ya organizó las 12 especies en el mecanismo

#Este procedimiento está regado en las notas del cacharreo cantera
import cantera as ct
import numpy as np
#ct.add_directory('/home/carlos/Maestria/Materias/Tesis/Mecanismos')
gas1 = ct.Solution('Mech_KIVA_Cantera.cti')
gas1.TP=298.15,ct.one_atm
nrk=gas1.n_reactions

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
#2(a6,f10.5): 2 veces(6 letras y real en forma decinal de 10 espacios y 5 para los decimales -10 espacios contando el punto-)
#Acá empieza lo nuevo
mw=gas1.molecular_weights
names=gas1.species_names
#Pasa a minúsculas
#gas1.species_names[1].lower()
nsp=gas1.n_species


isSpeciesBlock=False
lnSpeciesBlock=0
with open('itape5','r') as f:
    lines = f.readlines()
for i in range(0,len(lines)):
  if re.match("stoifuel",lines[i]):#Detectar Fin del bloque de especies
    break  
  if isSpeciesBlock:
#    print(lines[i])
    lnSpeciesBlock=lnSpeciesBlock+1
  if re.match("nsp",lines[i]):#Detectar bloque de especies
    nspLine=i
    isSpeciesBlock=True
    #Detectar número de especies líquidas
    nsplRegex=re.search("nsp\s(\d+)\s(\d+)(\s|$)?",lines[i])
    if nsplRegex:
      nspl=int(nsplRegex.group(2))#extrae número de especies líquidas
      #Sustituye el número de especies por el número de especies del mecanismo
      lines[i]=re.sub(nsplRegex.group(1),str(nsp),lines[i],1)
      print("sustitución: ",lines[i])

#Ciclo para eliminar las líneas con especies previas
for i in range(0,lnSpeciesBlock-nspl):
  del lines[nspLine+nspl+1]#Eliminar especies previas, excepto la especie líquida

cont=np.arange(0,nsp,dtype=np.int16)
#Ciclo para introducir las nuevas especies
for value in cont:
#  lines.insert(nspLine+cont+1,speciesRegEx)
  lines.insert(nspLine+value+nspl+1, gas1.species_names[value].lower().rjust(8)+
               '   '+'mw{0}'.format(value+1).ljust(6)[-6:]+'{0:10.5f}'.format(mw[value])+
               'htf{0}'.format(value+1).ljust(6)[-6:]+'{0:10.5f}'.format(htf[value])+"\n")

#Para imprimir hay 2 alternativas:
#https://docs.python.org/3.1/tutorial/inputoutput.html
#Para formatear la salida/entrada véase
#https://docs.python.org/3.1/library/string.html#formatexamples
#Para el manejo de strings véase:
#https://docs.python.org/3.1/library/stdtypes.html#string-methods}
#[-6:] sirve para coger sólo 6 caracteres del string en caso de que haya más de 1000 especies

isBlock=False
lnBlock=0
mfracLine=0
for i in range(0,len(lines)):
  nrkRegex=re.search("nrk\s(\d+)(\s|$)?",lines[i])#Detectar Fin del bloque de mfrac
  lines[i]=re.sub("trbchem\s\d+\.\d+","trbchem 2.0",lines[i],1)
  if nrkRegex:
    lines[i]=re.sub(nrkRegex.group(1),str(nrk),lines[i],1)
    print("sustitución: ",lines[i])
    break  
  if isBlock:
    lnBlock=lnBlock+1
  if re.match("er.\s\d+\.\d+",lines[i]):#Detectar bloque de mfrac
    print(lines[i])
    mfracLine=i
    isBlock=True

#Ciclo para eliminar las líneas con especies previas
for i in range(0,lnBlock-nspl):
  del lines[mfracLine+nspl+1]#Eliminar especies previas, excepto la especie líquida


#A continuación se pone la parte de llenar los mfrac:
#mfracc5h11co,     0.000000E+00
#que lee rinput como:
#         read ( 5,*)      id(1),(mfrac(n,isp),n=1,nregions)

for value in cont:
  lines.insert(mfracLine+value+nspl+1,('mfrac'+gas1.species_names[value].lower()+',').ljust(18)
	             +'{0:12.6E}'.format(0)+"\n")

f1=open('itape5','w')
for line in lines:
  f1.write(line)
f1.close()


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
f = open('datahk', 'w')
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
  
f.write( "c"+"\n"+
         "c"+"\n"+
         "c"+"\n"+
         "c  Automatically generated datahk by Qt-KIVA Universidad Nacional de Colombia"+"\n"+
         "c"+"\n"+
         "c     hk arrays are the enthalpies of the species, taken from the"+"\n"+
         "c     janaf thermochemical tables, which have the zero point at t=298k."+"\n"+
         "c     intervals are t=100(n-1), and units are kcal/mole (all enthalpy"+"\n"+
         "c     data is converted to sie in ergs/gm in subroutine rinput)."+"\n"+
         "c     species #1 is assumed to be the fuel, and its hk is loaded by"+"\n"+
         "c     subroutine fuel.     species #2-13 below are as follows:"+"\n"+
         "c     nspl+1=o2, nspl+2=n2, nspl+3=co2, nspl+4=h20, nspl+5=h, "+"\n"+
         "c     nspl+6=h2, nspl+7=o, nspl+8=n, nspl+9=oh,"+"\n"+
         "c     nspl+10=co, nspl+11=no, nspl+12=ho2.  "+"\n"+
         "c     note:  for h2 combustion, select h2 as the"+"\n"+
         "c     fuel (species #1), and do -->not<-- refer to species #7 in any"+"\n"+
         "c     chemical reactions.  species 13 enthalpy data below will need to"+"\n"+
         "c     be activated, and parameter lnsp must be increased to 13.")
  
f.close()
