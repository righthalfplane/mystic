PCTDR,T20,IO60,UD.
ACCOUNT,RANTA,WDNA14KC-BA0,PCT,714/453-2530.
REWIND,OUTPUT.
FTN,A.
LGO.
^
      PROGRAM TEST(INPUT,OUTPUT,TAPE5=INPUT,TAPE6=OUTPUT)
      COMPLEX P,Q,R
    5 CONTINUE
      READ(5,100)A,B,C
      P=CMPLX(A,1.)
      Q=CMPLX(B,1.0)
      R=CMPLX(C,1.0)
      WRITE(6,200)P,Q,R
      IF (EOF(5))20,10
   10 CONTINUE
      CALL CUBICS(P,Q,R)
      GO TO 5
   20 CONTINUE
      CALL EXIT
  100 FORMAT(3F10.0)
  200 FORMAT(1H0,10X,6E14.6)
      END
C
      SUBROUTINE CUBICS(P,Q,R)
C
C        CUBIC SOLVES THE CUBIC EQUATION X**3+P*X**2+Q*X+R=0
C
      COMPLEX AS,BS,DET,A,B,P3,T,U,V
      COMPLEX P,Q,R
      COMPLEX F,X
      DIMENSION X(3),F(3)
      DIMENSION RR(3)
      P3=P/3.
      AS=(3.0*Q-P**2)/3.0
      BS=(2.0*P**3-9.0*P*Q+27.0*R)/27.0
      DET=BS*BS/4.0+AS*AS*AS/27.
      T=CSQRT(DET)
      U=-BS/2.+T
C
C        THE COMPLEX CUBE ROOT OF U IS EVALUATED
C
      R1=REAL(U)
      R2=AIMAG(U)
      A1=0.
      A2=0.
      S=SQRT(R1**2+R2**2)
      IF(S.LE.0.) GO TO 10
      S=EXP(ALOG(S)/3.)
      O=ATAN2(R2,R1)
      O=O/3.0
      A1=S*SIN(O)
      A2=S*COS(O)
   10 CONTINUE
      A=CMPLX(A2,A1)
      B=0.
      IF(CABS(A).GT.0.)B=-AS/(3.*A)
      U=-(A+B)/2.-P3
      V=CSQRT(CMPLX(-3.,0.))*(A-B)/2.
      X(1)=A+B-P3
      X(2)=U+V
      X(3)=U-V
      DO 20 I=1,3
      X1=REAL(X(I))
      X2=AIMAG(X(I))
      RR(I)=SQRT(X1**2+X2**2)
   20 CONTINUE
      WRITE(6,200)RR
      RETURN
      F(1)=X(1)**3+P*X(1)**2+Q*X(1)+R
      F(2)=X(2)**3+P*X(2)**2+Q*X(2)+R
      F(3)=X(3)**3+P*X(3)**2+Q*X(3)+R
      WRITE(6,200)X
      WRITE(6,200)F
      RETURN
  200 FORMAT(10X,1P8E14.6)
      END
^
    3.      3.        1.
   12.      40.       30.
  4.        5.        2.
    44.      33.      1.
   1.        1.       1.
   1.        5.         7.
   16.       3.        1.
    1.        9.        8.7
   1.        3.       -1.
     2.       -1.       3.
  -1.           2.        3.
    -1.       -2.      3.
   -2.       -3.      -1.
    -1.       5.       -3.
    5.        -3.      5.
     1.      -3.       -5.
   1.        0.        0.
~
