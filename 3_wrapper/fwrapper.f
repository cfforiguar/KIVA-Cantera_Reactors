      integer function wrapper_f(n) result (dummy)
      use, intrinsic :: iso_c_binding
      use mpi
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
    !Variables usadas por MPI
      integer ( kind = 4 ) ierr, id,p  
      integer ( c_int ) comm
      real ( kind = 8 ) wtime_diff
      real ( kind = 8 ) wtime_end
      real ( kind = 8 ) wtime_start
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
      !
      !  Initialize MPI.
      !
        call MPI_Init ( ierr )
      !
      !  Get this process's ID.
      !
        comm = MPI_COMM_WORLD
        call MPI_Comm_rank ( comm, id, ierr )
      !
      !  Find out how many processes are available.
      !
        call MPI_Comm_size ( comm, p, ierr )

        if ( id == 0 ) then
          write ( *, '(a,i8)' ) '  The number of processes is ', p
          wtime_start = MPI_Wtime ( )
        end if

        write ( *, '(a)' ) ' '
        write ( *, '(a,i8,a)' ) 'Process ', id, ' is active.'
      !
      !  Here we call the C function.
      !

!        call MPI_SCATTERV(sbuf, scounts, displs, stype, rbuf, rcount, rtype,
!     &       root, comm, ierr)
	            
      call wrapper_c(temperatura,CellVolume,CellPressure,tfinal
     &                ,TempY, numcellsa,ct_nsp,comm,ierr)



      if ( id == 0 ) then
        wtime_end = MPI_Wtime ( )
      end if


      if ( id == 0 ) then
        wtime_end = MPI_Wtime ( )
      end if
    !
    !  Finish up.
    !
      call MPI_Finalize ( ierr )
    !
    !  Single process execution.
    !

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
