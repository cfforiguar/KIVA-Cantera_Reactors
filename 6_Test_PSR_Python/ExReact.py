"""
Test of a case of combustion of nc7h16 in air.
"""

import math
import csv

import cantera as ct

# use Patel's  reaction mechanism with NO+N2

gasIn1 = ct.Solution('Mech_KIVA_Cantera.cti','gas')
gasIn2 = ct.Solution('Mech_KIVA_Cantera.cti','gas')
gasComb = ct.Solution('Mech_KIVA_Cantera.cti','gas')
gasEx = ct.Solution('Mech_KIVA_Cantera.cti','gas')

# create a reservoir for the fuel inlet, and set to mixture.
gasIn1.TPY = 1500.0, 2026500.00000003, 'nc7h16:0.063, o2:0.218,n2:0.71899999999999997'
fuel_in = ct.Reservoir(gasIn1)
fuel_mw = gasIn1.mean_molecular_weight

# create a reservoir for the air inlet, and set to mixture.
gasIn2.TPY = 1500.0, 2026500.00000003, 'nc7h16:0.063, o2:0.218,n2:0.71899999999999997'
air_in = ct.Reservoir(gasIn2)
air_mw = gasIn2.mean_molecular_weight

# create the combustor, and fill it in initially with mixture
combustor = ct.IdealGasReactor(gasComb)
combustor.volume = 5.892793593225408e-13

# create a reservoir for the exhaust
exhaust = ct.Reservoir(gasEx)

# lean combustion, phi = 0.5
equiv_ratio = 0.5

# compute fuel and air mass flow rates
factor = 5.6761327713201428e-07
air_mdot = factor/2.0
fuel_mdot = factor/2.0

# create and install the mass flow controllers. Controllers m1 and m2 provide
# constant mass flow rates, and m3 provides a short Gaussian pulse only to
# ignite the mixture
m1 = ct.MassFlowController(fuel_in, combustor, mdot=fuel_mdot)

# note that this connects two reactors with different reaction mechanisms and
# different numbers of species. Downstream and upstream species are matched by
# name.
m2 = ct.MassFlowController(air_in, combustor, mdot=air_mdot)

# put a valve on the exhaust line to regulate the pressure
v = ct.Valve(combustor, exhaust, K=1.0)

# the simulation only contains one reactor
sim = ct.ReactorNet([combustor])

# take single steps to 6 s, writing the results to a CSV file for later
# plotting.
tfinal = 1.2991316826994651e-05
tnow = 0.0
Tprev = combustor.T
tprev = tnow
outfile = open('combustor.csv','w')
csvwriter = csv.writer(outfile)

while tnow < tfinal:
    tnow = sim.step(tfinal)
    Tnow = combustor.T
    if abs(Tnow - Tprev) > 1.0 or tnow-tprev > 2e-2:
        tprev = tnow
        Tprev = Tnow
        csvwriter.writerow([tnow, combustor.T,] +
                            list(combustor.thermo.Y))
outfile.close()
