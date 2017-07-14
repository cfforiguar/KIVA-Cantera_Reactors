#! /home/carlos/opt/anaconda3/bin/python3

#############################
#Guarda los datos de las 12 especies que se deben añadir a KIVA en caso de que el mecanismo que se usa no las tenga
#############################

O2=["O2                121386O   2               G  0300.00   5000.00  1000.00      1",
    " 0.03697578E+02 0.06135197E-02-0.01258842E-05 0.01775281E-09-0.01136435E-13    2",
    "-0.01233930E+05 0.03189166E+02 0.03212936E+02 0.01127486E-01-0.05756150E-05    3",
    " 0.01313877E-07-0.08768554E-11-0.01005249E+05 0.06034738E+02                   4"]

N2=["N2                121286N   2               G  0300.00   5000.00  1000.00      1",
    " 0.02926640E+02 0.01487977E-01-0.05684761E-05 0.01009704E-08-0.06753351E-13    2",
    "-0.09227977E+04 0.05980528E+02 0.03298677E+02 0.01408240E-01-0.03963222E-04    3",
    " 0.05641515E-07-0.02444855E-10-0.01020900E+05 0.03950372E+02                   4"]


CO2=["CO2               121286C   1O   2          G  0300.00   5000.00  1000.00      1",
     " 0.04453623E+02 0.03140169E-01-0.01278411E-04 0.02393997E-08-0.01669033E-12    2",
     "-0.04896696E+06-0.09553959E+01 0.02275725E+02 0.09922072E-01-0.01040911E-03    3",
     " 0.06866687E-07-0.02117280E-10-0.04837314E+06 0.01018849E+03                   4"]


H2O=["H2O                20387H   2O   1          G  0300.00   5000.00  1000.00      1",
     " 0.02672146E+02 0.03056293E-01-0.08730260E-05 0.01200996E-08-0.06391618E-13    2",
     "-0.02989921E+06 0.06862817E+02 0.03386842E+02 0.03474982E-01-0.06354696E-04    3",
     " 0.06968581E-07-0.02506588E-10-0.03020811E+06 0.02590233E+02                   4"]


H=["H          8/12/99 THERMH   1O   0    0    0G   300.000  5000.000 1000.000    01",
   " 2.50104422E+00 0.00000000E+00 0.00000000E+00 0.00000000E+00 0.00000000E+00    2",
   " 2.54747466E+04-4.65341317E-01 2.50104422E+00 0.00000000E+00 0.00000000E+00    3",
   " 0.00000000E+00 0.00000000E+00 2.54747466E+04-4.65341317E-01                   4"]


H2=["H2                121286H   2               G  0300.00   5000.00  1000.00      1",
    " 0.02991423E+02 0.07000644E-02-0.05633829E-06-0.09231578E-10 0.01582752E-13    2",
    "-0.08350340E+04-0.01355110E+02 0.03298124E+02 0.08249442E-02-0.08143015E-05    3",
    "-0.09475434E-09 0.04134872E-11-0.01012521E+05-0.03294094E+02                   4"]


O=["O                 120186O   1               G  0300.00   5000.00  1000.00      1",
   " 0.02542060E+02-0.02755062E-03-0.03102803E-07 0.04551067E-10-0.04368052E-14    2",
   " 0.02923080E+06 0.04920308E+02 0.02946429E+02-0.01638166E-01 0.02421032E-04    3",
   "-0.01602843E-07 0.03890696E-11 0.02914764E+06 0.02963995E+02                   4"]


N=["N                 120186N   1               G  0300.00   5000.00  1000.00      1",
   " 0.02450268E+02 0.10661458E-03-0.07465337E-06 0.01879652E-09-0.10259839E-14    2",
   " 0.05611604E+06 0.04448758E+02 0.02503071E+02-0.02180018E-03 0.05420529E-06    3",
   "-0.05647560E-09 0.02099904E-12 0.05609890E+06 0.04167566E+02                   4"]


OH=["OH         8/12/99 THERMH   1O   1    0    0G   300.000  5000.000 1357.000    01",
    " 2.62599754E+00 1.31992406E-03-3.59724670E-07 4.25630800E-11-1.82048016E-15    2",
    " 4.12085374E+03 7.10667307E+00 3.43586219E+00 2.02235804E-04-1.13546412E-07    3",
    " 2.42445149E-10-7.43651031E-14 3.74321252E+03 2.45014127E+00                   4"]


CO=["CO                121286C   1O   1          G  0300.00   5000.00  1000.00      1",
    " 0.03025078E+02 0.01442689E-01-0.05630828E-05 0.01018581E-08-0.06910952E-13    2",
    "-0.01426835E+06 0.06108218E+02 0.03262452E+02 0.01511941E-01-0.03881755E-04    3",
    " 0.05581944E-07-0.02474951E-10-0.01431054E+06 0.04848897E+02                   4"]


NO=["NO                121286N   1O   1          G  0300.00   5000.00  1000.00      1",
    " 0.03245435E+02 0.12691383E-02-0.05015890E-05 0.09169283E-09-0.06275419E-13    2",
    " 0.09800840E+05 0.06417293E+02 0.03376541E+02 0.12530634E-02-0.03302750E-04    3",
    " 0.05217810E-07-0.02446262E-10 0.09817961E+05 0.05829590E+02                   4"]


H2O2=["H2O2              120186H   2O   2          G  0300.00   5000.00  1000.00      1",
      " 0.04573167E+02 0.04336136E-01-0.01474689E-04 0.02348904E-08-0.01431654E-12    2",
      "-0.01800696E+06 0.05011370E+01 0.03388754E+02 0.06569226E-01-0.01485013E-05    3",
      "-0.04625806E-07 0.02471515E-10-0.01766315E+06 0.06785363E+02                   4"]


o2=["o2                121386O   2               G  0300.00   5000.00  1000.00      1",
    " 0.03697578E+02 0.06135197E-02-0.01258842E-05 0.01775281E-09-0.01136435E-13    2",
    "-0.01233930E+05 0.03189166E+02 0.03212936E+02 0.01127486E-01-0.05756150E-05    3",
    " 0.01313877E-07-0.08768554E-11-0.01005249E+05 0.06034738E+02                   4"]

n2=["n2                121286N   2               G  0300.00   5000.00  1000.00      1",
    " 0.02926640E+02 0.01487977E-01-0.05684761E-05 0.01009704E-08-0.06753351E-13    2",
    "-0.09227977E+04 0.05980528E+02 0.03298677E+02 0.01408240E-01-0.03963222E-04    3",
    " 0.05641515E-07-0.02444855E-10-0.01020900E+05 0.03950372E+02                   4"]


co2=["co2               121286C   1O   2          G  0300.00   5000.00  1000.00      1",
     " 0.04453623E+02 0.03140169E-01-0.01278411E-04 0.02393997E-08-0.01669033E-12    2",
     "-0.04896696E+06-0.09553959E+01 0.02275725E+02 0.09922072E-01-0.01040911E-03    3",
     " 0.06866687E-07-0.02117280E-10-0.04837314E+06 0.01018849E+03                   4"]


h2o=["h2o                20387H   2O   1          G  0300.00   5000.00  1000.00      1",
     " 0.02672146E+02 0.03056293E-01-0.08730260E-05 0.01200996E-08-0.06391618E-13    2",
     "-0.02989921E+06 0.06862817E+02 0.03386842E+02 0.03474982E-01-0.06354696E-04    3",
     " 0.06968581E-07-0.02506588E-10-0.03020811E+06 0.02590233E+02                   4"]


h=["h          8/12/99 THERMH   1O   0    0    0G   300.000  5000.000 1000.000    01",
   " 2.50104422E+00 0.00000000E+00 0.00000000E+00 0.00000000E+00 0.00000000E+00    2",
   " 2.54747466E+04-4.65341317E-01 2.50104422E+00 0.00000000E+00 0.00000000E+00    3",
   " 0.00000000E+00 0.00000000E+00 2.54747466E+04-4.65341317E-01                   4"]


h2=["h2                121286H   2               G  0300.00   5000.00  1000.00      1",
    " 0.02991423E+02 0.07000644E-02-0.05633829E-06-0.09231578E-10 0.01582752E-13    2",
    "-0.08350340E+04-0.01355110E+02 0.03298124E+02 0.08249442E-02-0.08143015E-05    3",
    "-0.09475434E-09 0.04134872E-11-0.01012521E+05-0.03294094E+02                   4"]


o=["o                 120186O   1               G  0300.00   5000.00  1000.00      1",
   " 0.02542060E+02-0.02755062E-03-0.03102803E-07 0.04551067E-10-0.04368052E-14    2",
   " 0.02923080E+06 0.04920308E+02 0.02946429E+02-0.01638166E-01 0.02421032E-04    3",
   "-0.01602843E-07 0.03890696E-11 0.02914764E+06 0.02963995E+02                   4"]


n=["n                 120186N   1               G  0300.00   5000.00  1000.00      1",
   " 0.02450268E+02 0.10661458E-03-0.07465337E-06 0.01879652E-09-0.10259839E-14    2",
   " 0.05611604E+06 0.04448758E+02 0.02503071E+02-0.02180018E-03 0.05420529E-06    3",
   "-0.05647560E-09 0.02099904E-12 0.05609890E+06 0.04167566E+02                   4"]

oh=["oh         8/12/99 THERMH   1O   1    0    0G   300.000  5000.000 1357.000    01",
    " 2.62599754E+00 1.31992406E-03-3.59724670E-07 4.25630800E-11-1.82048016E-15    2",
    " 4.12085374E+03 7.10667307E+00 3.43586219E+00 2.02235804E-04-1.13546412E-07    3",
    " 2.42445149E-10-7.43651031E-14 3.74321252E+03 2.45014127E+00                   4"]


co=["co                121286C   1O   1          G  0300.00   5000.00  1000.00      1",
    " 0.03025078E+02 0.01442689E-01-0.05630828E-05 0.01018581E-08-0.06910952E-13    2",
    "-0.01426835E+06 0.06108218E+02 0.03262452E+02 0.01511941E-01-0.03881755E-04    3",
    " 0.05581944E-07-0.02474951E-10-0.01431054E+06 0.04848897E+02                   4"]


no=["no                121286N   1O   1          G  0300.00   5000.00  1000.00      1",
    " 0.03245435E+02 0.12691383E-02-0.05015890E-05 0.09169283E-09-0.06275419E-13    2",
    " 0.09800840E+05 0.06417293E+02 0.03376541E+02 0.12530634E-02-0.03302750E-04    3",
    " 0.05217810E-07-0.02446262E-10 0.09817961E+05 0.05829590E+02                   4"]


h2o2=["h2o2              120186H   2O   2          G  0300.00   5000.00  1000.00      1",
      " 0.04573167E+02 0.04336136E-01-0.01474689E-04 0.02348904E-08-0.01431654E-12    2",
      "-0.01800696E+06 0.05011370E+01 0.03388754E+02 0.06569226E-01-0.01485013E-05    3",
      "-0.04625806E-07 0.02471515E-10-0.01766315E+06 0.06785363E+02                   4"]



Final=["c  Automatically generated datahk by Qt-KIVA Universidad Nacional de Colombia",
       "c",
       "c     hk arrays are the enthalpies of the species, taken from the",
       "c     janaf thermochemical tables, which have the zero point at t=298k.",
       "c     intervals are t=100(n-1), and units are kcal/mole (all enthalpy",
       "c     data is converted to sie in ergs/gm in subroutine rinput).",
       "c     species #1 is assumed to be the fuel, and its hk is loaded by",
       "c     subroutine fuel.     species #2-13 below are as follows:",
       "c     nspl+1=o2, nspl+2=n2, nspl+3=co2, nspl+4=h20, nspl+5=h, ",
       "c     nspl+6=h2, nspl+7=o, nspl+8=n, nspl+9=oh,",
       "c     nspl+10=co, nspl+11=no, nspl+12=ho2.  ",
       "c     note:  for h2 combustion, select h2 as the",
       "c     fuel (species #1), and do -->not<-- refer to species #7 in any",
       "c     chemical reactions.  species 13 enthalpy data below will need to",
       "c     be activated, and parameter lnsp must be increased to 13."]

