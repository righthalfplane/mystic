      program main
      OPEN(UNIT=11,FILE='TAPE11',FORM='UNFORMATTED',STATUS='UNKNOWN')
      do 10 i=1,1000
      call fld
   10 continue
      stop
      end
      SUBROUTINE FLD
      IMPLICIT REAL*8 (A-H,O-Z)
      COMMON/LIST/DATA(64),NWORD
      character*8 cdata
      common/clist/cdata(64)
      DATA NT11/11/
      READ(11)NW,(DATA(I),I=1,NW),(cdata(I),I=1,NW)
c     READ(11)NW,(DATA(I),I=1,NW)
c     READ(11)NW
      write(*,*)'nw = ',NW,cdata(1)
      NWP=NW+1
      DO 10 I=NWP,64
      DATA(I)=0.
      cdata(I)=' '
   10 CONTINUE
      RETURN
      END
