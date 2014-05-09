import cantera as ct
import numpy as np

gas = ct.Solution('h2o2.xml')
gas.TPX = 300, ct.one_atm, 'H2:1.0, O2:2.0, AR:4.0'

#Miro el contenido del gas en fracciones másicas
gas.Y

#Creo reservorios aguas arriba y abajo para el reactor
#Éstos se convertirán en los objetos necesarios para "controlar" el reactor
upstream = ct.Reservoir(gas)
downstream = ct.Reservoir(gas)

#Pone al objeto gas en estado de equilibrio para llenar el reactor en su estado #incial (conserva la masa)
gas.equilibrate('HP')
#Crea el reactor y lo llama r1
#r1 = ct.IdealGasReactor(contents=gas)
#energy='off' apaga la ecuación de energía al crear el reactor
r1 = ct.IdealGasReactor(contents=gas, energy='off')

#Miro el contenido del reactor en fracciones másicas
r1.Y

residence_time = 0.8e-4

def mdot_out(t):
    return r1.mass / residence_time
#Poner una válvula a la entrada del reactor r1 para mantener P=cte y evitar que se #devuelva el flujo. Véase  help(ct.Valve)
#Nótese el uso del objeto upstream que es un tipo Reservoir devinido arriba
inlet = ct.Valve(upstream, r1, K=100)
#Pone un objeto MassFlowController a la salidad de r1 que mantiene el flujo másico 
#constante, véase help(ct.MassFlowController)
#Nótese el uso del objeto downstream que es un tipo Reservoir devinido arriba
outlet = ct.MassFlowController(r1, downstream)
#fija el flujo másico a la salida del reactor
outlet.set_mass_flow_rate(mdot_out)

net = ct.ReactorNet([r1])

#Creo los "pasos de tiempo" en los que quiero ver el progreso de la solución
#El solver de cantera resuelve de forma automática y escoge su paso de tiempo de #forma automática así t sea 2, 20 o 2000 puntos
t = np.linspace(0, residence_time, 2)
#
#¿Por qué el código original de Ray multiplica por 5 el tiempo de residencia?
#t = np.linspace(0, 5*residence_time, 2)
#

#Vector donde guardo las temperaturas
T = np.zeros(t.shape)
#Vector donde guardo los flujos másicos
mdot = np.zeros(t.shape)
for i,ti in enumerate(t):
    if ti:
        net.advance(ti)
    T[i] = r1.T
    mdot[i] = outlet.mdot(ti)

#Miro el contenido del reactor en fracciones másicas
r1.Y
