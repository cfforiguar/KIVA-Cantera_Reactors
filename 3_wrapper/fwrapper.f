      integer function wrapper_f(n) result (dummy)
      IMPLICIT NONE
      integer, intent(in) :: n
      integer :: i=1,j=1,numcellsa=2000,ct_nsp=29
      double precision, dimension(:,:), allocatable:: Y,TempY
      double precision, dimension(:), allocatable:: temperatura
      double precision, dimension(:), allocatable:: CellVolume
      double precision, dimension(:), allocatable:: CellPressure
      double precision :: tfinal
    ! Cómo escribir salidas con formato variable http://www.eng-tips.com/viewthread.cfm?qid=37205
      character*50 :: varFormat
      
      dummy=0
      
      allocate (Y(numcellsa,ct_nsp))
      allocate (TempY(ct_nsp,numcellsa))
      allocate (temperatura(numcellsa))
      allocate (CellVolume(numcellsa))
      allocate (CellPressure(numcellsa))
      !Inicializo variables de los reactores
      tfinal=1.2991316826994651d-05
      DO i = 1, numcellsa
        temperatura(i)=1477.76999999999998d0
        CellVolume(i)=5.892793593225408d-13
        CellPressure(i)=2680000.0000000009d0
        Y(i,1)=0.16d0
        Y(i,2)=0.21d0
        Y(i,3)=0.79d0        
        DO j = 4, ct_nsp
          Y(i,j) = 0.0d0
        END DO
      END DO    
      TempY=transpose(Y)
      write(varFormat,*) '(',numcellsa*ct_nsp,'(es26.20", "))'
      call wrapper_c(temperatura,CellVolume,CellPressure,tfinal
     &                ,TempY, numcellsa,ct_nsp)
!      print *, 'Fortran wrapper'
!      print *,Y
      open(unit=10,file='datos_f90.csv',position='append',status='old')
!      write(*,varFormat) Y
      write(10,varFormat) transpose(TempY)
      close (10)
!      print *,varFormat
!	Para verificar que los dos archivos de salida son idénticos desde
!	el shell
!1.	Ejecutar el programa
!	./test
!2.	Reemplazar las E mayúsculas por minúsculas
!	sed -i 's/E/e/g' datos_f90.csv
!3.	Usar diff para que avise si son iguales
!	diff -s  datos_f90.csv datos_cpp.csv

      end function wrapper_f
