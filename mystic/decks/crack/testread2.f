      program main
      call fld
      stop
      end
      SUBROUTINE FLD
      IMPLICIT REAL*8 (A-H,O-Z)
      COMMON/LIST/DATA(64),NWORD
      character*8 cdata
      common/clist/cdata(64)
      DATA NT11/11/
    5 continue
      READ(11)NW,(DATA(I),I=1,NW),(cdata(I),I=1,NW)
      write(*,*)(DATA(I),I=1,NW)
      if(DATA(1).eq.'STOP')goto 5
      NWP=NW+1
      DO 10 I=NWP,64
      DATA(I)=0.
      cdata(I)=' '
   10 CONTINUE
      RETURN
      END
