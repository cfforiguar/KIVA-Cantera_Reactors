      program wrapper_f
      real aNumber
      real bNumber
      real ins(10), outs(10)

      ins(1) = 1
      ins(2) = 3

      call wrapper_c(ins, outs)
      print *, 'Fortran wrapper'
      
      end program
