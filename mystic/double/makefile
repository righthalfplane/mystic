# Makefile

SHELL = /bin/sh

CC = gcc

CFLAGS = -DSUN 

Use_COMPILER := $(shell echo $(COMPILER) | tr "[a-z]" "[A-Z]")

      
  ifeq (G95, $(findstring G95,$(Use_COMPILER)))
	FFLAGS = -fstatic  -fzero -Wno=121,108,155 -d8 
	FC = g95
	EX=g95
	AX=
    ifeq "$(STUBS)" "yes"
		LFLAGS = -lSystemStubs
    else
		LFLAGS =
    endif
  else
	FFLAGS = -fno-automatic -fdefault-real-8 -fdefault-integer-8  -fdefault-double-8 -std=legacy  
	FC = gfortran
	EX=GF
	AX=G
	LFLAGS =
  endif
  
  FOPT= -O3
     
  ifeq "$(DEBUG)" "yes"
     FFLAGS +=  -g 
     CFLAGS +=  -g
     FOPT= -O0
  endif

  ifeq "$(OPT)" "-O0"
     FOPT= -O0
  endif

  ifeq "$(OPT)" "-O1"
     FOPT= -O1
  endif

  ifeq "$(OPT)" "-O2"
     FOPT= -O2
  endif

  ifeq "$(OPT)" "-O3"
     FOPT= -O3
  endif

  
  FFLAGS +=  ${FOPT}
  CFLAGS +=  ${FOPT}
  
  

  ifeq "$(EXE)" "yes"
     X = .exe
  else
     X =  .x
  endif

  ifeq "$(MMX)" "yes"
     FFLAGS +=  -mfpmath=sse -msse -msse2 
     CFLAGS +=  -mfpmath=sse -msse -msse2 
  endif


COBJS	= ciolib.o

FOBJS	=   mysticd.o mystice1.o mystice2.o

FOBJS2	=   dstatgs.o

OBJS	= ${FOBJS} ${COBJS} 

	
build: clean ${FOBJS} ${COBJS} ${FOBJS2}
	  ${FC} ${FFLAGS} -o mysticdD$(AX)$(X) ${OBJS} ${LFLAGS}
	  mv mysticdD$(AX)$(X) ~/utlib/
	  ${FC} ${FFLAGS} -o dstatgsD$(AX)$(X) ${FOBJS2}
	  mv dstatgsD$(AX)$(X) ~/utlib/
	  rm -f *.o
	
.PHONY : clean
     
clean:
	rm -f *.o mysticdD$(AX)$(X) dstatgsD$(AX)$(X)
	
.PHONY : rm
     
rm:
	rm -f ~/utlib/dstatgsD$(AX)$(X)
	rm -f ~/utlib/mysticdD$(AX)$(X)
