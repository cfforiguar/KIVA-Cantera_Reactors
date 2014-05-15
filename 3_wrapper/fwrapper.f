      program wrapper_f
      double precision :: aNumber
      double precision :: bNumber
      double precision :: ins(10), outs(10)
      double precision, allocatable:: Y(:)
!     TODO: consultar el número de especies internamente
      integer :: i,ct_nsp=53
! Cómo escribir salidas con formato variable http://www.eng-tips.com/viewthread.cfm?qid=37205
      character*50 :: varFormat
      allocate (Y(ct_nsp))
      ins(1) = 1
      ins(2) = 4
      write(varFormat,*) '(',ct_nsp,'(es26.20", "))'
      call wrapper_c(ins, Y)
      print *, 'Fortran wrapper'
      print *, ins
      print *,Y
      open(unit=10,file='datos_f90.csv',position='append',status='old')
      write(*,varFormat) Y
      write(10,varFormat) Y
      close (10)
      print *,varFormat}
!	Para verificar que los dos archivos de salida son idénticos desde
!	el shell
!1.	Ejecutar el programa
!	./test
!2.	Reemplazar las E mayúsculas por minúsculas
!	sed -i 's/E/e/g' datos_f90.csv
!3.	Usar diff para que avise si son iguales
!	diff -s  datos_f90.csv datos_cpp.csv

      end program
