#! /home/carlos/opt/anaconda3/bin/python3
# encoding: utf-8
import os
import re
#######################################
#Modifica los mecanismos de Chemkin para que sigan el orden de las especies como se cargan en KIVA4
#######################################

# En el archivo de mecanismo de chemkin, poner/añadir las 12 especies originales de KIVA en el mismo orden que en el itape5


#########################################
#¿Cómo lidiar la especie combustible?
#  -Pedir la entrada y luego añadirla a la lista speciesLs
#########################################

#Insertar la línea con las especies que corresponda
speciesLs=["O2","N2","CO2","H2O","H","H2","O","N","OH","CO","NO","H2O2"]

with open('chem.inp','r') as f:
    lines = f.readlines()

isSpeciesBlock=False
for i in range(0,len(lines)):
  if re.match("SPECIES|species|SPEC|spec",lines[i]):#Detectar bloque de especies
    isSpeciesBlock=True
    #Detectar mayúsculas/minúsculas en las especies
    isLowerCase=re.match(r"([a-z])",lines[i])
    if (isLowerCase):
      speciesLs=[element.lower() for element in speciesLs]
    specsLine=i
    continue
  if isSpeciesBlock:
    print(lines[i])
    speciesRegEx="(^|\s)"+"($|\s)|(^|\s)".join(speciesLs)+"($|\s)"
    lines[i]=re.sub(speciesRegEx,"     ",lines[i],0)
    #Eliminar las especies que correspondan
    print(lines[i])
    if re.match("END|end",lines[i]):#Detectar Fin del bloque de especies
      break  

speciesRegEx="    ".join(speciesLs)+"\n"
lines.insert(specsLine+1,speciesRegEx)
print(lines[specsLine+1])

#print(', '.join(lines))
f.closed

f=open('MODchem.inp','w')
for line in lines:
  f.write(line)
f.close()

# En el archivo thermo de chemkin, poner/añadir las 12 especies originales de KIVA con los coeficientes de algún mecanismo probado en KIVA

###################################
# Hacer la comparación con la especie ya extraída y cambiar el booleano si lo encuentra
# Hacer conversión del mecanismo
# Generar mecanismo (probar que los mods son aceptados por cantera)
# añadir esto al script del preprocesador
# ----> mods itape5 (véase abajo) , (fusionar con preprocesador)
###################################

with open('therm.dat','r') as f1:
    lines1 = f1.readlines()

#Generar lista de booleana para las especies
isSpeciesLs=[False for i in range(len(speciesLs))]
for i in range(0,len(lines1)):
  #Filtrar las líneas para encontrar la especie
  #  Añadir las especies que hagan falta, usar mecanismo predeterminado
  for j in range(0,len(speciesLs)):
    #print("$|"+speciesLs[j]+"\s")
    speciesMatch="".join(speciesLs[j])
    speciesStr=re.findall("$|"+"^"+speciesMatch+"\s",lines1[i])[0]
    if (re.match(speciesMatch,speciesStr)):
      #print(lines1)
      isSpeciesLs[j]=True
      #print(lines1[i])
      #print("ebrioLOCO "+speciesStr)


#Crear base de datos de las 12 especies, si falta alguna, simplemente meta de la base de datos
from SpecsData import *
if (isLowerCase):
  RefillSpcs=[o2,n2,co2,h2o,h,h2,o,n,oh,co,no,h2o2]
else:
  RefillSpcs=[O2,N2,CO2,H2O,H,H2,O,N,OH,CO,NO,H2O2]

for i in range(0,len(isSpeciesLs)):
  if not isSpeciesLs[i]:
    lines1.insert(-1,"\n".join(RefillSpcs[i][0:4]))

f1.closed

f1=open('MODtherm.dat','w')
for line in lines1:
  f1.write(line)
f1.close()
os.system("ck2cti --permissive  --input=MODchem.inp --thermo=MODtherm.dat --id=gas --output=Mech_KIVA_Cantera.cti")

