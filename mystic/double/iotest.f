      program iotest
c gfortran -fno-automatic -fdefault-real-8 -fdefault-integer-8 -fdefault-double-8 -std=legacy -o iotest iotest.f ciolib.c
c gfortran -D_FILE_OFFSET_BITS=64 -fno-automatic -fdefault-real-8 -fdefault-integer-8 -fdefault-double-8 -std=legacy -o iotest iotest.f ciolib.c
      integer cwrite
      external cwrite
      common a(50000000)
      nr=31
      nword=50000000
      npos=0;
      ier=ccreate(nr,"tape.31;")
      if(ier.ne.0)go to 40
      do 30 i=1,15
      write(6,*)i
      ierr=cwrite(nr,a,nword,npos)
      if(ierr.gt.0)go to 40
      npos=npos+nword
  30  continue
  40  continue
      stop
      end
