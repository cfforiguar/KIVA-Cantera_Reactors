program test
  use mpi
  use cantera
  type(phase_t)::mix
  integer ierr
!  mix=importPhase('gri30.xml','gri30')
!  write(*,*),'number of sp:',nSpecies(mix)
  call MPI_init(ierr)
  call MPI_finalize(ierr)
end program
