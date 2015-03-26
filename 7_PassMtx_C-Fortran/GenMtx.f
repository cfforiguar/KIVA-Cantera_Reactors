      program wrapper_f
      IMPLICIT NONE
      integer :: i,j,numcellsa=20,ct_nsp=21
      double precision, dimension(:,:), allocatable:: SpMtx!**
    ! Cómo escribir salidas con formato variable http://www.eng-tips.com/viewthread.cfm?qid=37205
      character*50 :: varFormat
      allocate (SpMtx(numcellsa,ct_nsp))!**
      DO i = 1, numcellsa
        DO j = 1, ct_nsp
          SpMtx(i,j) = i+j
        END DO
      END DO
      write(varFormat,*) '(',numcellsa*ct_nsp,'(es26.20", "))'
      call wrapper_c(SpMtx)
      print *, 'Fortran wrapper'
      print *,SpMtx
      open(unit=10,file='datos_f90.csv',position='append',status='old')
      write(*,varFormat) SpMtx
      write(10,varFormat) SpMtx
      close (10)
      print *,varFormat
!	Para verificar que los dos archivos de salida son idénticos desde
!	el shell
!1.	Ejecutar el programa
!	./test
!2.	Reemplazar las E mayúsculas por minúsculas
!	sed -i 's/E/e/g' datos_f90.csv
!3.	Usar diff para que avise si son iguales
!	diff -s  datos_f90.csv datos_cpp.csv

      end program
