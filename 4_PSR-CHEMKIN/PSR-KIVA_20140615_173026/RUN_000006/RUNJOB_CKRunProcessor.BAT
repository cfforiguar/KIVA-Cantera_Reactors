REM Chemkin-Generated Batch Job Command File

REM get the Chemkin runtime environment defined

CALL "C:\Archivos de programa\Reaction\chemkin15083_pc\bin\run_chemkin_env_setup.bat"
SET JAVA_ID=304094661

REM DELETE ANY INTERMEDIATE FILES LEFTOVER FROM PREVIOUS RUN

IF EXIST "C:\Documents and Settings\SalaCAD\chemkin\PSR-KIVA\PSR-KIVA_20140615_173026\RUN_000006\XMLdata.zip" DEL /F /Q "C:\Documents and Settings\SalaCAD\chemkin\PSR-KIVA\PSR-KIVA_20140615_173026\RUN_000006\XMLdata.zip"

REM COPY FILES TO WORKING DIR

COPY "C:\Archivos de programa\Reaction\chemkin15083_pc\data\chemkindata.dtd" "C:\Documents and Settings\SalaCAD\chemkin\PSR-KIVA\PSR-KIVA_20140615_173026\RUN_000006"

REM RUN THE JOB AND CAPTURE EXIT STATUS

CKReactorGenericPSR -i "C:\Documents and Settings\SalaCAD\chemkin\PSR-KIVA\PSR-KIVA_20140615_173026\RUN_000006\PSR-KIVA.inp" -o "C:\Documents and Settings\SalaCAD\chemkin\PSR-KIVA\PSR-KIVA_20140615_173026\RUN_000006\PSR-KIVA.out" -m "PSR-KIVA.out.mon" -c "C:\Documents and Settings\SalaCAD\chemkin\PSR-KIVA\PSR-KIVA-GriMech_gas.asc"
exit %errorlevel%
