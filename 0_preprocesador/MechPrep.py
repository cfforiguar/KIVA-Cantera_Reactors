#! /home/carlos/opt/anaconda3/bin/python3
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

f=open('EBRIOchem.inp','w')
for line in lines:
  f.write(line)
f.closed

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
      print(lines1[i])
      print("ebrioLOCO "+speciesStr)


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

f1=open('EBRIOtherm.dat','w')
for line in lines1:
  f1.write(line)
f1.closed

#################################
#Mañana:
#**-Script que convierta el mecanismo
#-Script que genere el xml
#-Mod de preprocesador.py para que escriba directamente en el itape5
#  -nsp
#  -nrk
#  -trbchem
#-Mod de preprocesador.py para que escriba el datahk completo
#################################


#Separar líneas de las especies en dat.thermo

#Imprimir todo el dat.thermo, añadir las que no encuentre

#. generar un mecanismo en cantera usando los archivos de mecanismo y thermo de chemkin y ponerle como nombre "Mech_KIVA_Cantera.cti"
#. A la fase que se va a importar en dicho mecanismo ponerle como nombre 'gas'
#ck2cti --permissive  --input=EBRIOchem.inp --thermo=EBRIOtherm.dat --id=gas --output=Mech_KIVA_Cantera.cti

#. modificar el datahk de acuerdo con la salida mw/hft de Preprocesador.py
  #Escribirlo de cero a partir del dat.thermo

#. modificar el itape5 de acuerdo con la salida mfrac de Preprocesador.py
#. modificar nsp en el itape5 para que concuerde con el número de especies
#. modificar nrk en el itape5 para que concuerde con el número de reacciones
#. modificar trbchem en el itape5 para que sea 2.0

#. Verificar los 3 archivos necesarios para correr la prueba sean coherentes
#  -datahk ---->OJO: cambia cuando se cambia de mecanismo
#  -iprep/kiva4grid
#  -itape5 ---->OJO: cambia cuando se cambia de mecanismo
#    -Tener en cuenta que como se implementó el phiMax y phiMin que se debe modificar el itape5 de acuerdo con eso
#      -La línea a modificar es:   nrk    #Reacc   phiMax   phiMin
#  -Mech_KIVA_Cantera.cti ---->OJO: cambia cuando se cambia de mecanismo



