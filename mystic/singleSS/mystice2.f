*deck beam
      subroutine beam
      implicit real*8 (a-h,o-z)
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/dtwo/itwo,ndof,idim
      common/tapcom/nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlin
c
      common a(1)
      real a
c
      nfirst=n6e+4*itwo
      n101=nfirst
      n102=11*nummat*itwo+n101
      n103=4*nume*itwo+n102
      n104=nume+n103
      n105=6*nume+n104
      nlast=3*nummat+n105
      midest=nlast-nfirst+1
      min=nmax+nlast-n1l+1
      if(ind.eq.0)write(6,20)min
      if(min.le.nmax) go to 10
      stop 4
   10 call
     . beamp(a(n2),a(n2l),a(n1l),a(n2),a(n3),a(n101),a(n102),a(n103),
     .a(n104),a(n105),nume,nummat,ndof,ind,kpri,nt11)
      if(ind.eq.0)call ewrite(a(n6e),ng)
   20 format (1h0,39htorage required to input element data =,i10)
   30 format (1h0,21herror nmax less than ,i10)
      return
      end
*deck beamp
      subroutine beamp(a,id,mht,x,y,fmat,xy,mat,lm,iprop,nume,
     . nummat,ndof,ind,kpri,nt11)
      implicit real*8 (a-h,o-z)
      common/comtyp/itypee,ngroup,ngin,nlen
c
      common /list/ data(64),ndum
      common/switch/ipsm,ipkbar,ipinpt,ipreac,ieldat,ipsdm,ipcnpf
     . ,ibigw
c
      dimension x(1),y(1),fmat(11,1),mat(1),lm(6,1),stiff(21),xmass(21),
     . damp(21),st(4,6),fk(6,6),fc(6,6),fm(6,6),xy(2,2,1),nonum(2),
     . id(ndof,1),mht(1),iprop(3,1),
     . ur(6),ui(6),sigr(4),sigi(4),cz(4),sl(6,6)
c
      if (ind.ne.0) go to 200
c
c         read and store material data
c
      do 115 m=1,nummat
      call fld
      do 110 n=1,11
  110 fmat(n,m)=data(n+3)
      do 115 n=1,3
  115 iprop(n,m)=data(n)
c
c         read and store element information
c
      if(ibigw.eq.0)go to 33
      if(ieldat.eq.0)go to 33
      write(6,140)
   33 continue
      do 150 n=1,nume
      call fld
      num=data(1)
      mat(n)=data(2)
      do 135 i=1,2
      node=data(i+2)
      nonum(i)=node
      lm(3*i-2,n)=id(1,node)
      lm(3*i-1,n)=id(2,node)
      lm(3*i,n)=id(3,node)
      xy(1,i,n)=x(node)
  135 xy(2,i,n)=y(node)
c
      call colht(mht,6,lm(1,n))
c
      if(ibigw.eq.0)go to 150
      if(ieldat.eq.0)go to 150
      write(6,145) n,mat(n),(nonum(i),i=1,2)
  140 format(///10x,12helement data,/,2x,3helm,2x,3hmat,2x,5hnode1,
     . 2x,5hnode2//)
  145 format(2i5,4i7)
c
  150 continue
c
      return
c
c         ind=1 pass to form element matrices
c
  200 if (ind.ne.1.and.ind.ne.3) go to 300
      if (ind.eq.3)write(6,390)
  390 format(1x,43hcomplex uniaxial stresses for beam elements//,
     . 7h    elm,3x,2(8hsigma1-u,3x),2(8hsigma1-l,3x),
     . 2(8hsigma2-u,3x),2(8hsigma2-l,3x),/,7x,
     . 4(5x,4hreal,7x,4himag,2x)//)
      do 280 n=1,nume
      icode=0
      if (ind.eq.1) call echeck(lm(1,n),6,icode)
      if (icode.eq.1) go to 280
      m=mat(n)
      ilumps=iprop(1,m)
      idamps=iprop(2,m)
      istres=iprop(3,m)
      if(ind.eq.3.and.istres.eq.0)go to 280
      if(kpri.eq.3.and.idamps.eq.0)go to 280
      area=fmat(1,m)
      e=fmat(2,m)
      pr=fmat(3,m)
      fnu1=fmat(4,m)
      rho=fmat(5,m)
      fksy=fmat(6,m)
      fiz=fmat(7,m)
      do 10 i=1,4
   10 cz(i)=fmat(i+7,m)
      kprip=kpri
      if(ind.eq.3)kprip=4
      call slbm(e,pr,area,xy(1,1,n),fiz,fksy,sl)
      call sgbm(sl,xy(1,1,n),rho,area,fiz,fnu1,ilumps,kprip,cz,st,
     . fk,fc,fm)
      if (ind.eq.3) go to 255
c
      m=0
      do 220 i=1,6
      do 220 j=i,6
      m=m+1
      stiff(m)=fk(i,j)
      damp(m)=fc(i,j)
  220 xmass(m)=fm(i,j)
c     print of element stiffness damping and mass matrices
      if(ibigw.eq.0)go to 950
      if(ipsdm.eq.0)go to 950
      if(kpri.ne.1)go to 702
      write(6,956)n
  956 format(1x//////,1x,30h stiffness for beam element n=,i5,//)
      write(6,953)((fk(ii,jj),jj=1,6),ii=1,6)
  953 format(1x,1p6e11.3)
  702 if(kpri.ne.3)go to 703
      write(6,957)n
  957 format(1x,//////,1x,35h damping matrix for beam element n=,i5//)
      write(6,953)((fc(ii,jj),jj=1,6),ii=1,6)
  703 if(kpri.ne.2)go to 950
      write(6,960)n
  960 format(1x,//////,32h mass matrix for beam element n=,i5//)
      write(6,953)((fm(ii,jj),jj=1,6),ii=1,6)
  950 continue
c
      go to (230,240,250), kpri
c
c         assemble stiffness matrix
c
  230 continue
      call addban(a,mht,stiff,lm(1,n),6)
      go to 280
c
c         assemble mass matrix
c
  240 call addban(a,mht,xmass,lm(1,n),6)
      go to 280
c
c         assemble damping matrix
c
  250 call addban(a,mht,damp,lm(1,n),6)
      go to 280
c
c    ind =3 pass
c
  255 do 320 i=1,6
c
c       expand solution displ vector (fill in constraint zeros)
c
      idof=lm(i,n)
      ur(i)=0.0
      ui(i)=0.0
      if(idof.eq.0) go to 320
      ur(i)=x(idof)
      ui(i)=y(idof)
  320 continue
c
c compute stresses
c     multiply stress transformation matrix times displacement vector
c
      do 340 i=1,4
      sigr(i)=0.0
      sigi(i)=0.0
      do 340 j=1,6
      sigr(i)=sigr(i)+st(i,j)*ur(j)
  340 sigi(i)=sigi(i)+st(i,j)*ui(j)
c
c      print stresses
c
      write(6,360)n,(sigr(i),sigi(i),i=1,4)
  360 format(1x,i5,1x,1p8e11.3)
c
  280 continue
      return
c
c         ind=3 and ind=4 passes
c
  300 if(ind.ne.4)return
      do 315 n=1,nume
      call echeck(lm(1,n),6,icode)
      if(icode)315,310,315
  310 continue
      ngin=1
      return
  315 continue
      return
      end
c.
c........................................................
c.  subroutine to calculate the local stiffness matrix for
c.  a beam element
c.
c.- - input variables- -
c.  e = young"s modulus
c.  a = cross sectional area
c.        xx(2,2)= x:i=1 and y:i=2 coordinates of the two endpoints
c. fiz= moment of inertia about the z axis
c.           cross section is y and z axis
c.        fksy= area factor for shear
c. pr = poisson"s ratio
c.- - output variables- -
c.      phiy= shear deflection correction factor
c.      g   = shear modulus of elasticity
c. sl(6,6)= local stiffness matrix
c.........................................................
*deck slbm
      subroutine slbm(e,pr,a,xx,fiz,fksy,sl)
      implicit real*8 (a-h,o-z)
      dimension sl(6,6),xx(2,2)
      fle=dsqrt((xx(1,2)-xx(1,1))**2+(xx(2,2)-xx(2,1))**2)
      asy=fksy*a
      g=e/(2.0*(1.0 + pr))
      if(asy.eq.0.0)go to 310
      phiy=12.0*e*fiz/(g*asy*fle**2)
  310 continue
      if(asy.eq.0.0)phiy=0.0
c.
c. local stiffness matrix
c.
      sl(1,1)=e*a/fle
      sl(4,1)=-sl(1,1)
      sl(2,2)=12.0*e*fiz/(fle**3*(1.0 +phiy))
      sl(3,2)=sl(2,2)*fle/2.0
      sl(5,2)=-sl(2,2)
      sl(6,2)=sl(3,2)
      sl(3,3)=(4.0 + phiy)*e*fiz/(fle*(1.0 + phiy))
      sl(5,3)=-sl(3,2)
      sl(6,3)=(2.0-phiy)*e*fiz/(fle*(1.0 + phiy))
      sl(4,4)=sl(1,1)
      sl(5,5)=sl(2,2)
      sl(6,5)=sl(5,3)
      sl(6,6)=sl(3,3)
c.
c. using symmetry fill in other half of matrix
c.
      do 60 i=1,6
      do 60 j=i,6
   60 sl(i,j)=sl(j,i)
c     write(6,205)((sl(i,j),j=1,6),i=1,6)
c 205 format(1x,"local stiffness matrix for beam element=",
c    1/,1x,6(6(1pe12.5,1x),/,1x))
      return
      end
c.
c.  subroutine to transform local stiffness matrix into global
c   matrix.
c.  double precision
c.
c.
*deck sgbm
      subroutine sgbm(sl,xx,rho,a,fiz,fnu1,ilumps,kprip,cz,st,
     1fk,fc,fm)
      implicit real*8 (a-h,o-z)
      dimension fl(6,6),sl(6,6),flsl(6),xx(2,2),c(2,2)
      dimension fc(6,6),fm(6,6),fk(6,6),st(4,6),cz(4),fkt(6,6)
     .,fml(6,6),fmll(6,6)
c.
c.
c.- - input variables- -
c.      fl(6,6)= direction cosine matrix,lambda
c.      sl(6,6)= local stiffness matrix to be transformed into
c.               global matrix
c.
c.      rho= material density
c. a= cross sectional area
c.      fnu1= damping constant
c.      ilumps= switch controling mass matrix: 0-consistant mass
c.  matrix; 1-lumped mass matrix
c.      idamps= switch controling damping matrix: 0-no damping
c.  matrix; 1-damping matrix
c.   istres= switch controling stress transformation matrix:
c.  0-no stress calculation; 1-stress calculation
c.
c.- - output variables- -
c.      fk(6,6)= global stiffness matrix
c.      fc(6,6)= damping matrix
c.      fm(6,6)= mass matrix
c. st(4,6)= stress transformation matrix
c.
c.
c.  calculation of the direction cosine matrix fl
c.
      x=xx(1,2)-xx(1,1)
      y=xx(2,2)-xx(2,1)
      fll= dsqrt(x**2+y**2)
c.
c.  calculate the direction cosines using the formula for the\
c.  angle between two vectors, a and b: cos<>=(a1*b1+a2*b2)/a*b
c.
c.  c(1,1): a1=1.0, a2=0.0, b1=x, b2=y
c.  c(1,2): a1=0.0, a2=1.0, b1=x, b2=y
c.  c(2,1): a1=1.0, a2=0.0, b1=-y,b2=x
c.  c(2,2): a1=0.0, a2=1.0, b1=-y,b2=x
c.
      do 5 i=1,2
      if(i.eq.2)go to 15
      b1=x
      b2=y
      go to 25
   15 b1=-y
      b2=x
   25 do 5 j=1,2
      if(j.eq.2)go to 35
      a1=1.0
      a2=0.0
      go to 5
   35 a1=0.0
      a2=1.0
    5 c(i,j)=(a1*b1 + a2*b2)/fll
c.
c.
c.  formation of the lambda matrix from the c matrix
c.
c.
      do 62 i=1,6
      do 62 j=1,6
   62 fl(i,j)=0.0
      do 55 i=1,2
      do 55 j=1,2
   55 fl(i,j)=c(i,j)
      fl(3,3)=1.0
      do 65 i=4,6
      do 65 j=4,6
   65 fl(i,j)=fl(i-3,j-3)
c.
c.
c.  transformation of the local matrix to the global matrix
c.  using the relation: s=fl(transpose)*sl*fl
c.
      do 10 i=1,6
      do 10 j=1,6
      fm(i,j)=0.0
      fc(i,j)=0.0
   10 fk(i,j)=0.0
      if(ilumps.eq.0)go to 240
      do 70 j=1,6
      do 40 k=1,6
      flsl(k)=0.0
      do 40 l=1,6
   40 flsl(k)=flsl(k) + sl(k,l)*fl(l,j)
      do 60 i=1,6
      stiff=0.0
      do 50 l=1,6
      fkt(l,j)=flsl(l)
   50 stiff=stiff + fl(l,i)*flsl(l)
   60 fk(i,j)=fk(i,j) + stiff
   70 continue
      go to (230,240,250,260)kprip
  230 return
  250 continue
c.
c.    c a l c u l a t i o n  o f  t h e  d a m p i n g  m a t r i x
c.
      do 102 i=1,6
      do 102 j=1,6
  102 fc(i,j)=fnu1*fk(i,j)
      return
  260 continue
c.
c.    c a l c u l a t i o n  o f  t h e  s t r e s s  t r a n s -
c.    f o r m a t i o n  m a t r i x
c.
      do 131 i=1,2
      do 131 j=1,6
  131 st(i,j)=-1.0/a*fkt(1,j) + (-1.0)**i*cz(i)/fiz*fkt(3,j)
      do 137 i=3,4
      do 137 j=1,6
  137 st(i,j)=1.0/a*fkt(4,j) + (-1.0)**i*cz(i)/fiz*fkt(6,j)
      return
  240 continue
c.
c. c a l c u l a t i o n  o f  t h e  l u m p e d  m a s s  m a t r i x
c.
      w=rho*a*fll
      if(ilumps.eq.0)go to 96
      do 97 i=1,6
      fm(i,i)=w/2.0
      if(i.eq.3.or.i.eq.6)fm(i,i)=w/24*fll**2
   97 continue
      return
c.
c. c a l c u l a t i o n  o f  t h e  c o n s i s t a n t  m a s s
c.   m a t r i x
c.
   96 continue
      do 99 i=1,6
   99 fml(i,i)=w/3.0
      fml(1,4)=w/6.0
      fml(2,5)=fml(1,4)
      fml(3,6)=fml(1,4)
      fml(6,3)=fml(3,6)
      fml(5,2)=fml(2,5)
      fml(4,1)=fml(1,4)
c.
c.    t r a n s f o r m  l o c a l  m a s s  m a t r i x  t o
c.      g l o b a l  m a s s  m a t r i x
c.
      do 43 i=1,6
      do 43 j=1,6
      sum=0.0
      do 44 l=1,6
   44 sum=sum + fml(i,l)*fl(l,j)
   43 fmll(i,j)=sum
      do 45 i=1,6
      do 45 j=1,6
      sum=0.0
      do 46 l=1,6
   46 sum=sum + fmll(l,j)*fl(l,i)
   45 fm(i,j)=sum
      return
      end
*deck beam3d
      subroutine beam3d
      implicit real*8 (a-h,o-z)
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/dtwo/itwo,ndof,idim
      common/tapcom/nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlin
c
      common a(1)
      real a
c
      nfirst=n6e+4*itwo
      n101=nfirst
      n102=21*nummat*itwo+n101
      n103=6*nume*itwo+n102
      n104=nume+n103
      n105=12*nume+n104
      nlast=3*nummat+n105
      midest=nlast-nfirst+1
      min=nmax+nlast-n1l+1
      if(ind.eq.0)write(6,20)min
      if(min.le.nmax) go to 10
      stop 4
   10 call
     . beam3dp(a(n2),a(n2l),a(n1l),a(n2),a(n3),a(n4),a(n101),a(n102),
     .a(n103),a(n104),a(n105),nume,nummat,ndof,ind,kpri,nt11)
      if(ind.eq.0)call ewrite(a(n6e),ng)
   20 format (1h0,39htorage required to input element data =,i10)
   30 format (1h0,21herror nmax less than ,i10)
      return
      end
*deck beam3dp
      subroutine beam3dp(a,id,mht,x,y,z,fmat,xy,mat,lm,iprop,nume,
     . nummat,ndof,ind,kpri,nt11)
      implicit real*8 (a-h,o-z)
      common/comtyp/itypee,ngroup,ngin,nlen
c
      common /list/ data(64),ndum
      common/switch/ipsm,ipkbar,ipinpt,ipreac,ieldat,ipsdm,ipcnpf
     . ,ibigw
c
      dimension x(1),y(1),z(1),fmat(21,1),mat(1),lm(12,1),stiff(78),
     . damp(78),st(8,12),fk(12,12),fc(12,12),fm(12,12),xy(3,2,1),
     . id(ndof,1),mht(1),iprop(3,1),xmass(78),nonum(2),
     . ur(12),ui(12),sigr(8),sigi(8),cz(8),sl(12,12),ft(6,12)
     . ,forcer(8),forcei(8)
c
      if (ind.ne.0) go to 200
c
c         read and store material data
c
      do 115 m=1,nummat
      call fld
      do 110 n=1,21
  110 fmat(n,m)=data(n+3)
      do 115 n=1,3
  115 iprop(n,m)=data(n)
c
c         read and store element information
c
      if(ibigw.eq.0)go to 33
      if(ieldat.eq.0)go to 33
      write(6,140)
   33 continue
      do 150 n=1,nume
      call fld
      num=data(1)
      mat(n)=data(2)
      do 135 i=1,2
      node=data(i+2)
      nonum(i)=node
      lm(6*i,n)=id(6,node)
      lm(6*i-1,n)=id(5,node)
      lm(6*i-2,n)=id(4,node)
      lm(6*i-3,n)=id(3,node)
      lm(6*i-4,n)=id(2,node)
      lm(6*i-5,n)=id(1,node)
      xy(1,i,n)=x(node)
      xy(2,i,n)=y(node)
  135 xy(3,i,n)=z(node)
c
      call colht(mht,12,lm(1,n))
c
      if(ibigw.eq.0)go to 150
      if(ieldat.eq.0)go to 150
      write(6,145) n,mat(n),(nonum(i),i=1,2)
  140 format(///10x,12helement data,/,2x,3helm,2x,3hmat,2x,5hnode1,
     . 2x,5hnode2//)
  145 format(2i5,4i7)
c
  150 continue
c
      return
c
c         ind=1 pass to form element matrices
c
  200 if (ind.ne.1.and.ind.ne.3) go to 300
      if (ind.eq.3)write(6,390)
      if (ind.eq.3)write(6,391)
  390 format(1x,16x,71hc o m p l e x   s t r e s s e s   i n   3 d   b e
     . a m   e l e m e n t s,/,1x,47x,16h(real/imaginary),//,1x,7helemen
     .t,21x,4(8hlocation,7x),5x,7haverage,/,1x,2x,3hid.,27x,1h1,14x,1h2,
     .14x,1h3,14x,1h4,12x,12haxial stress/)
  391 format(1x,16x,67hc o m p l e x   f o r c e s   i n   3 d   b e a m
     .   e l e m e n t s,/,1x,46x,16h(real/imaginary),//,1x,7helement
     .,9x,17hbend-moment-end-a,12x,17hbend-moment-end-b,14x,
     .9h- shear -,13x,5haxial,/,1x,2x,3hid.,9x,7hplane 1,7x,
     .7hplane 2,8x,7hplane 1,7x,7hplane 2,6x,7hplane 1,6x,7hplane 2,
     .8x,5hforce,9x,6htorque//)
      do 280 n=1,nume
      icode=0
      if (ind.eq.1) call echeck(lm(1,n),12,icode)
      if (icode.eq.1) go to 280
      m=mat(n)
      ilumps=iprop(1,m)
      idamps=iprop(2,m)
      istres=iprop(3,m)
      if(ind.eq.3.and.istres.eq.0)go to 280
      if(kpri.eq.3.and.idamps.eq.0)go to 280
      area=fmat(1,m)
      e=fmat(2,m)
      pr=fmat(3,m)
      fnu1=fmat(4,m)
      rho=fmat(5,m)
      fksy=fmat(6,m)
      fiz=fmat(7,m)
      fiy=fmat(8,m)
      fksz=fmat(9,m)
      fj=fmat(10,m)
      vx=fmat(11,m)
      vy=fmat(12,m)
      vz=fmat(13,m)
      do 10 i=1,8
   10 cz(i)=fmat(i+13,m)
      kprip=kpri
      if(ind.eq.3)kprip=4
      if(kprip.eq.2)go to 91
      call slbm3d(e,pr,area,xy(1,1,n),fiy,fiz,fksy,fksz,fj,sl)
   91 call sgbm3d(sl,xy(1,1,n),vx,vy,vz,rho,area,fiy,fiz,fnu1,
     . ilumps,kprip,cz,fll,st,ft,fk,fc,fm)
      if (ind.eq.3) go to 255
c
      m=0
      do 220 i=1,12
      do 220 j=i,12
      m=m+1
      stiff(m)=fk(i,j)
      damp(m)=fc(i,j)
  220 xmass(m)=fm(i,j)
c     print of element stiffness damping and mass matrices
      if(ibigw.eq.0)go to 950
      if(ipsdm.eq.0)go to 950
      if(kpri.ne.1)go to 702
      write(6,956)n
  956 format(1x//////,1x,33h stiffness for 3d beam element n=
     .,i5,//)
      write(6,953)((fk(ii,jj),jj=1,12),ii=1,12)
  953 format(1x,2(1p6e11.3,/,1x))
  702 if(kpri.ne.3)go to 703
      write(6,957)n
  957 format(1x,//////,1x,38h damping matrix for 3d beam element n=
     .,i5,//)
      write(6,953)((fc(ii,jj),jj=1,12),ii=1,12)
  703 if(kpri.ne.2)go to 950
      write(6,960)n
  960 format(1x,//////,35h mass matrix for 3d beam element n=
     .,i5//)
      write(6,953)((fm(ii,jj),jj=1,12),ii=1,12)
  950 continue
c
      go to (230,240,250), kpri
c
c         assemble stiffness matrix
c
  230 continue
      call addban(a,mht,stiff,lm(1,n),12)
      go to 280
c
c         assemble mass matrix
c
  240 call addban(a,mht,xmass,lm(1,n),12)
      go to 280
c
c         assemble damping matrix
c
  250 call addban(a,mht,damp,lm(1,n),12)
      go to 280
c
c    ind =3 pass
c
  255 do 320 i=1,12
c
c       expand solution displ vector (fill in constraint zeros)
c
      idof=lm(i,n)
      ur(i)=0.0
      ui(i)=0.0
      if(idof.eq.0) go to 320
      ur(i)=x(idof)
      ui(i)=y(idof)
  320 continue
c
c compute stresses and forces
c     multiply stress transformation matrix times displacement vector
c     multiply force transformation matrix times displacement vector
c
      axstrr=-forcer(i)/area
      axstri=-forcei(i)/area
      do 340 i=1,8
      sigr(i)=0.0
      sigi(i)=0.0
      do 340 j=1,12
      sigr(i)=sigr(i)+st(i,j)*ur(j)
  340 sigi(i)=sigi(i)+st(i,j)*ui(j)
      do 341 i=1,6
      forcer(i)=0.0
      forcei(i)=0.0
      do 341 j=1,12
      forcer(i)=forcer(i) + ft(i,j)*ur(j)
  341 forcei(i)=forcei(i) + ft(i,j)*ui(j)
      forcer(7)=forcer(5)-forcer(2)*fll
      forcer(8)=forcer(6)-forcer(3)*fll
      forcei(7)=forcei(5)-forcei(2)*fll
      forcei(8)=forcei(6)-forcei(3)*fll
c
c      print stresses
c
      write(6,360)n,(sigr(i),i=1,4),axstrr,
     .(sigi(i),i=1,4),axstri,(sigr(i),i=5,8),
     .(sigi(i),i=5,8)
  360 format(1x,i6,5x,5hend-a,10x,4(e13.6,2x),5x,e13.6,/,
     .1x,26x,4(e13.6,2x),5x,e13.6,//,1x,11x,5hend-b,
     .10x,4(e13.6,2x),/,1x,26x,4(e13.6,2x)//)
c
c      print forces
c
      write(6,361)n,(forcer(i),i=5,8),forcer(2),forcer(3)
     .,forcer(1),forcer(4),(forcei(i),i=5,8),forcei(2),forcei(3)
     .,forcei(1),forcei(4)
  361 format(1x,i6,5x,2(e13.6,1x),1x,2(e13.6,1x),3(e12.5,1x),
     .e12.5,/,1x,11x,2(e13.6,1x),1x,2(e13.6,1x),3(e12.5,1x),
     .e12.5,//)
  280 continue
      return
c
c         ind=3 and ind=4 passes
c
  300 if(ind.ne.4)return
      do 315 n=1,nume
      call echeck(lm(1,n),12,icode)
      if(icode)315,310,315
  310 continue
      ngin=1
      return
  315 continue
      return
      end
c.
c........................................................
c.  subroutine to calculate the local stiffness matrix for
c.  a 3-d beam element
c.
c.- - input variables- -
c.  e = young"s modulus
c.  a = cross sectional area
c.      xx(3,2)= x:i=1 and y:i=2 coordinates of the two endpoints
c.  z:i=3
c. fiz= moment of inertia about the z axis
c.           cross section is y and z axis
c. fiy= moment of inertial about the y-axis
c.      fksy= area factor for shear
c. fksz = area factor for shear
c. pr = poisson"s ratio
c. fj = torsional constant
c.- - output variables- -
c.      phiy= shear deflection correction factor
c. phiz= shear delfection correction factor
c.      g   = shear modulus of elasticity
c. sl(12,12)= local stiffness matrix
c.........................................................
*deck slbm3d
      subroutine slbm3d(e,pr,a,xx,fiy,fiz,fksy,fksz,fj,sl)
      implicit real*8 (a-h,o-z)
      dimension sl(12,12),xx(3,2)
      fle=dsqrt((xx(1,2)-xx(1,1))**2+(xx(2,2)-xx(2,1))**2
     1+ (xx(3,2)-xx(3,1))**2)
      asy=fksy*a
      g=e/(2.0*(1.0 + pr))
      if(asy.eq.0.0)go to 310
      phiy=12.0*e*fiz/(g*asy*fle**2)
  310 continue
      if(asy.eq.0.0)phiy=0.0
      phiz=0.0
      asz=fksz*a
      if(asz.eq.0.)go to 510
      phiz=12.0*e*fiy/(g*asz*fle**2)
  510 continue
c.
c. local stiffness matrix
c.
      do 49 i=1,12
      do 49 j=1,12
   49 sl(i,j)=0.0
      sl(1,1)=e*a/fle
      sl(7,1)=-sl(1,1)
      sl(2,2)=12.0*e*fiz/(fle**3*(1.0 + phiy))
      sl(6,2)=sl(2,2)*fle/2.0
      sl(8,2)=-sl(2,2)
      sl(12,2)=sl(6,2)
      sl(3,3)=12.0*e*fiy/(fle**3*(1.0 + phiz))
      sl(5,3)=-sl(3,3)*fle/2.0
      sl(9,3)=-sl(3,3)
      sl(11,3)=sl(5,3)
      sl(4,4)=g*fj/fle
      sl(10,4)=-sl(4,4)
      sl(5,5)=(4.0 + phiz)*e*fiy/(fle*(1.0 + phiz))
      sl(9,5)=-sl(5,3)
      sl(11,5)=(2.0 - phiz)*e*fiy/(fle*(1.0 + phiz))
      sl(6,6)=(4.0 + phiy)*e*fiz/(fle*(1.0 + phiy))
      sl(8,6)=-sl(6,2)
      sl(12,6)=(2.0 - phiy)*e*fiz/(fle*(1.0 + phiy))
      sl(7,7)=sl(1,1)
      sl(8,8)=sl(2,2)
      sl(12,8)=sl(8,6)
      sl(9,9)=sl(3,3)
      sl(11,9)=sl(9,5)
      sl(10,10)=sl(4,4)
      sl(11,11)=sl(5,5)
      sl(12,12)=sl(6,6)
c.
c. using symmetry fill in other half of matrix
c.
      do 60 i=1,12
      do 60 j=i,12
   60 sl(i,j)=sl(j,i)
c     write(6,205)((sl(i,j),j=1,12),i=1,12)
  205 format(1x,"local stiffness matrix for beam element=",
     1/,1x,24(6(1pe12.5,1x),/,1x))
      return
      end
c.
c.  subroutine to transform local stiffness matrix into global
c   matrix.
c.
c.
*deck sgbm3d
      subroutine sgbm3d(sl,xx,vx,vy,vz,rho,a,fiy,fiz,fnu1
     1,ilumps,kprip,cz,fll,st,ft,fk,fc,fm)
      implicit real*8 (a-h,o-z)
      dimension fl(12,12),sl(12,12),flsl(12),xx(3,2),fml(12,12)
      dimension fc(12,12),fm(12,12),fk(12,12),st(8,12),cz(8),fkt(12,12)
      dimension flfml(12),ft(6,12),qf(6,6),q(8,6)
c.
c.
c.- - input variables- -
c.      fl(12,12)= direction cosine matrix,lambda
c.      sl(12,12)= local stiffness matrix to be transformed into
c.               global matrix
c.
c.      rho= material density
c. a= cross sectional area
c.      fnu1= damping constant
c.      ilumps= switch controling mass matrix: 0-consistant mass
c.  matrix; 1-lumped mass matrix
c.      idamps= switch controling damping matrix: 0-no damping
c.  matrix; 1-damping matrix
c.   istres= switch controling stress transformation matrix:
c.  0-no stress calculation; 1-stress calculation
c.
c.- - output variables- -
c.      fk(12,12)= global stiffness matrix
c.      fc(12,12)= damping matrix
c.      fm(12,12)= mass matrix
c. st(8,12)= stress transformation matrix
c.
c.
c.  calculation of the direction cosine matrix fl
c.
      x=xx(1,2)-xx(1,1)
      y=xx(2,2)-xx(2,1)
      z=xx(3,2)-xx(3,1)
      fll= dsqrt(x**2+y**2+z**2)
      if(kprip.eq.2.and.ilumps.eq.1)go to 240
      flx=x/fll
      fmx=y/fll
      fnx=z/fll
      flv=dsqrt(vx**2+vy**2+vz**2)
      vl=vx/flv
      vm=vy/flv
      vn=vz/flv
      flz=fmx*vn-fnx*vm
      fmz=fnx*vl-flx*vn
      fnz=flx*vm-fmx*vl
      fly=fmz*fnx-fnz*fmx
      fmy=fnz*flx-flz*fnx
      fny=flz*fmx-fmz*flx
      yl=dsqrt(fly**2+fmy**2+fny**2)
      zl=dsqrt(flz**2+fmz**2+fnz**2)
      do 29 i=1,10,3
      fl(i,i)=flx
      fl(i+1,i)=fly/yl
      fl(i+2,i)=flz/zl
      fl(i,i+1)=fmx
      fl(i+1,i+1)=fmy/yl
      fl(i+2,i+1)=fmz/zl
      fl(i,i+2)=fnx
      fl(i+1,i+2)=fny/yl
      fl(i+2,i+2)=fnz/zl
   29 continue
c     write(6,203)((fl(i,j),j=1,12),i=1,12)
  203 format(1x,"lambda,fl, matrix=",/,1x,24(6(1pe12.5,1x),/,1x))
      go to (230,240,230,230)kprip
240   continue
c.
c. c a l c u l a t i o n  o f  t h e  l u m p e d  m a s s  m a t r i x
c.
      w=rho*a*fll
      if(ilumps.eq.0)go to 96
      do 97 i=1,3
   97 fm(i,i)=w/2.0
      do 190 i=7,9
  190 fm(i,i)=fm(i-6,i-6)
      do 191 i=4,6
  191 fm(i,i)=w*fll**2/24.0
      do 192 i=10,12
  192 fm(i,i)=fm(i-6,i-6)
      return
c.
c. c a l c u l a t i o n  o f  t h e  c o n s i s t a n t  m a s s
c.   m a t r i x
c.
   96 continue
      do 99 i=1,12
      do 99 j=1,12
  99  fml(i,j)=0.0
      wc=w/420.
      fml(1,1)=wc*175.
      fml(2,2)=wc*156.
      fml(3,3)=fml(2,2)
      fml(5,5)=wc*4.*fll**2
      fml(6,6)=fml(5,5)
      do 98 i=7,12
   98 fml(i,i)=fml(i-6,i-6)
      fml(1,7)=wc*35.
      fml(2,6)=wc*22.*fll
      fml(2,8)=wc*54.
      fml(2,12)=-wc*13.*fll
      fml(3,5)=-fml(2,6)
      fml(3,9)=fml(2,8)
      fml(3,11)=-fml(2,12)
      fml(5,9)=fml(2,12)
      fml(5,11)=-wc*3.*fll**2
      fml(6,8)=fml(3,11)
      fml(6,12)=fml(5,11)
      fml(8,12)=fml(3,5)
      fml(9,11)=fml(2,6)
      do 199 j=1,12
      do 199 i=j,12
  199 fml(i,j)=fml(j,i)
  230 continue
c.
c.
c.  transformation of the local matrix to the global matrix
c.  using the relation: s=fl(transpose)*sl*fl
c.
      do 10 i=1,12
      do 10 j=1,12
      fm(i,j)=0.0
   10 fk(i,j)=0.0
      do 70 j=1,12
      do 40 k=1,12
      flsl(k)=0.0
      flfml(k)=0.0
      do 40 l=1,12
      flfml(k)=flfml(k) + fml(k,l)*fl(l,j)
   40 flsl(k)=flsl(k) + sl(k,l)*fl(l,j)
      do 60 i=1,12
      stiff=0.0
      xmass=0.0
      do 50 l=1,12
      fkt(l,j)=flsl(l)
      xmass=xmass + fl(l,i)*flfml(l)
   50 stiff=stiff + fl(l,i)*flsl(l)
      fm(i,j)=fm(i,j) + xmass
   60 fk(i,j)=fk(i,j) + stiff
   70 continue
      if(kprip.eq.1.or.kprip.eq.2)return
      if(kprip.eq.4)go to 260
c.
c. c a l c u l a t i o n  o f  t h e  d a m p i n g  m a t r i x
c.
      do 102 i=1,12
      do 102 j=1,12
  102 fc(i,j)=fnu1*fk(i,j)
      return
c.
c. c a l c u l a t i o n  o f  t h e  s t r e s s  t r a n s -
c. f o r m a t i o n  m a t r i x
c.
  260 continue
c     write(6,68)(cz(i),i=1,4)
   68 format(1x,"cz(i)=",/,2x,8(1pe12.5,/,1x))
      do 79 i=1,4
      q(i,5)=-cz(2*i)/fiy
      q(i,6)=cz(2*i-1)/fiz
      j=i+4
      q(j,2)=-fll*q(i,6)
      q(j,3)=fll*q(i,5)
      q(j,5)=q(i,5)
      q(j,6)=q(i,6)
   79 continue
      do 26 i=1,6
      do 26 j=1,6
  26  qf(i,j)=0.0
      qf(1,1)=1.
      qf(2,2)=-1.
      qf(3,3)=-1.
      qf(4,4)=1.
      qf(5,6)=-1.
      qf(6,5)=1.
      do 39 i=1,6
      do 39 j=1,12
      do 39 l=1,6
   39 ft(i,j)=ft(i,j) + qf(i,l)*fkt(l,j)
      do 19 i=1,8
      do 19 j=1,12
      do 19 l=1,6
  19  st(i,j)=st(i,j) + q(i,l)*fkt(l,j)
  507 continue
      return
      end
*deck axsh
      subroutine axsh
      implicit real*8 (a-h,o-z)
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/dtwo/itwo,ndof,idim
      common/tapcom/nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlin
c
      common a(1)
      real a
c
      nfirst=n6e+4*itwo
      n101=nfirst
      n102=6*nummat*itwo+n101
      n103=4*nume*itwo+n102
      n104=nume+n103
      n105=6*nume+n104
      nlast=4*nummat+n105
      midest=nlast-nfirst+1
      min=nmax+nlast-n1l+1
      if(ind.eq.0)write(6,20)min
      if(min.le.nmax) go to 10
      stop 4
   10 call
     . axshp(a(n2),a(n2l),a(n1l),a(n2),a(n3),a(n101),a(n102),a(n103),
     .a(n104),a(n105),nume,nummat,ndof,ind,kpri,nt11)
      if(ind.eq.0)call ewrite(a(n6e),ng)
   20 format (1h0,39htorage required to input element data =,i10)
   30 format (1h0,21herror nmax less than ,i10)
      return
      end
*deck axshp
      subroutine axshp(a,id,mht,x,y,fmat,xy,mat,lm,iprop,nume,
     . nummat,ndof,ind,kpri,nt11)
      implicit real*8 (a-h,o-z)
      common/comtyp/itypee,ngroup,ngin,nlen
c
      common /list/ data(64),ndum
      common/switch/ipsm,ipkbar,ipinpt,ipreac,ieldat,ipsdm,ipcnpf
     . ,ibigw
c
      dimension x(1),y(1),fmat(6,1),mat(1),lm(6,1),stiff(21),xmass(21),
     . damp(21),ft(8,6),fk(6,6),fc(6,6),fm(6,6),xy(2,2,1),nonum(2),
     . id(ndof,1),mht(1),iprop(4,1),c(15),d(5,5),
     . ur(6),ui(6),forcer(8),forcei(8),sigi(8),sigr(8),st(8,6)
c
      if (ind.ne.0) go to 200
c
c         read and store material data
c
      do 115 m=1,nummat
      call fld
      do 110 n=1,6
  110 fmat(n,m)=data(n+4)
      do 115 n=1,4
  115 iprop(n,m)=data(n)
c
c         read and store element information
c
      if(ibigw.eq.0)go to 33
      if(ieldat.eq.0)go to 33
      write(6,140)
   33 continue
      do 150 n=1,nume
      call fld
      num=data(1)
      mat(n)=data(2)
      do 135 i=1,2
      node=data(i+2)
      nonum(i)=node
      lm(3*i-2,n)=id(1,node)
      lm(3*i-1,n)=id(2,node)
      lm(3*i,n)=id(3,node)
      xy(1,i,n)=x(node)
  135 xy(2,i,n)=y(node)
c
      call colht(mht,6,lm(1,n))
c
      if(ibigw.eq.0)go to 150
      if(ieldat.eq.0)go to 150
      write(6,145) n,mat(n),(nonum(i),i=1,2)
  140 format(///10x,12helement data,/,2x,3helm,2x,3hmat,2x,5hnode1,
     . 2x,5hnode2//)
  145 format(2i5,4i7)
c
  150 continue
c
      return
c
c         ind=1 pass to form element matrices
c
  200 if (ind.ne.1.and.ind.ne.3) go to 300
      if (ind.eq.3)write(6,391)
      if (ind.eq.3)write(6,390)
  390 format(1x,34hcomplex stresses for axsh elements,
     .31h(second two lines of print out)//,5h  elm,5x,
     . 4hnode,2x,2(1x,10hsigma-xx-u),2(1x,10hsigma-xx-l),
     . 2(1x,10hsigma-yy-u),2(1x,10hsigma-yy-l),/,8x,6hnumber
     . ,4(5x,4hreal,7x,4himag,2x)//)
  391 format(1x,44hcomplex forces and moments for axsh elements,
     .30h(first two lines of print out)//,
     . 5h  elm,5x,4hnode,2x,2(4x,3hn-s,4x),2(3x,4hn-th,4x),2(4x,3hm-s
     . ,4x), 2(3x,4hm-th,4x), /,8x,6hnumber,4(5x,4hreal,7x,4himag,2x)//)
      do 280 n=1,nume
      icode=0
      if (ind.eq.1) call echeck(lm(1,n),6,icode)
      if (icode.eq.1) go to 280
      m=mat(n)
      ilumps=iprop(1,m)
      idamps=iprop(2,m)
      istres=iprop(3,m)
      inshel=iprop(4,m)
      if(ind.eq.3.and.istres.eq.0)go to 280
      if(kpri.eq.3.and.idamps.eq.0)go to 280
      thic=fmat(1,m)
      e=fmat(2,m)
      pr=fmat(3,m)
      fnu1=fmat(4,m)
      rho=fmat(5,m)
      fkshr=fmat(6,m)
      nstr=5
      f=e*thic/(1.0-pr**2)
      fn=pr*f
      g=f*thic**2/12.0
      gn=pr*g
      h=e*thic*fkshr/(2.0*(1.0+pr))
      c(1)=f
      c(2)=fn
      c(3)=0.0
      c(4)=0.0
      c(5)=0.0
      c(6)=f
      c(7)=0.0
      c(8)=0.0
      c(9)=0.0
      c(10)=g
      c(11)=gn
      c(12)=0.0
      c(13)=g
      c(14)=0.0
      c(15)=h
c
c         fill in full 5x5 stress strain matrix
c
      ic=0
      do 212 i=1,5
      do 212 j=i,5
      ic=ic+1
      d(i,j)=c(ic)
  212 d(j,i)=c(ic)
      kprip=kpri
      if(ind.eq.3)kprip=4
      if(inshel.eq.1)
     .call shelln(nstr,thic,e,pr,ilumps,xy(1,1,n),d,fnu1,rho,kprip,
     . fk,fc,fm,st,ft)
      if(inshel.eq.0)
     .call shellax(nstr,thic,e,pr,ilumps,xy(1,1,n),d,fnu1,rho,kprip,
     . fk,fc,fm,st,ft)
      if (ind.eq.3) go to 255
c
      m=0
      do 220 i=1,6
      do 220 j=i,6
      m=m+1
      stiff(m)=fk(i,j)
      damp(m)=fc(i,j)
  220 xmass(m)=fm(i,j)
c     print of element stiffness damping and mass matrices
      if(ibigw.eq.0)go to 950
      if(ipsdm.eq.0)go to 950
      if(kpri.ne.1)go to 702
      write(6,956)n
  956 format(1x//////,1x,30h stiffness for axsh element n=,i5,//)
      write(6,953)((fk(ii,jj),jj=1,6),ii=1,6)
  953 format(1x,1p6e11.3)
  702 if(kpri.ne.3)go to 703
      write(6,957)n
  957 format(1x,//////,1x,35h damping matrix for axsh element n=,i5//)
      write(6,953)((fc(ii,jj),jj=1,6),ii=1,6)
  703 if(kpri.ne.2)go to 950
      write(6,960)n
  960 format(1x,//////,32h mass matrix for axsh element n=,i5//)
      write(6,953)((fm(ii,jj),jj=1,6),ii=1,6)
  950 continue
c
      go to (230,240,250), kpri
c
c         assemble stiffness matrix
c
  230 continue
      call addban(a,mht,stiff,lm(1,n),6)
      go to 280
c
c         assemble mass matrix
c
  240 call addban(a,mht,xmass,lm(1,n),6)
      go to 280
c
c         assemble damping matrix
c
  250 call addban(a,mht,damp,lm(1,n),6)
      go to 280
c
c    ind =3 pass
c
  255 do 320 i=1,6
c
c       expand solution displ vector (fill in constraint zeros)
c
      idof=lm(i,n)
      ur(i)=0.0
      ui(i)=0.0
      if(idof.eq.0) go to 320
      ur(i)=x(idof)
      ui(i)=y(idof)
  320 continue
c
c compute forces
c     multiply force transformation matrix times displacement vector
c
      do 340 i=1,8
      forcer(i)=0.0
      forcei(i)=0.0
      do 340 j=1,6
      forcer(i)=forcer(i)-ft(i,j)*ur(j)
  340 forcei(i)=forcei(i)-ft(i,j)*ui(j)
c
c      print forces and moments
c
      write(6,362)n,(forcer(i),forcei(i),i=1,4)
      write(6,363)n,(forcer(i),forcei(i),i=5,8)
c
c     compute stresses
c        multiply stress transformation matrix times displacement
c        vector
      do 341 i=1,8
      sigr(i)=0.0
      sigi(i)=0.0
      do 341 j=1,6
      sigr(i)=sigr(i) + st(i,j)*ur(j)
  341 sigi(i)=sigi(i) + st(i,j)*ui(j)
c
c     print stresses
c
      write(6,362)n,(sigr(i),sigi(i),i=1,4)
      write(6,363)n,(sigr(i),sigi(i),i=5,8)
  362 format(1x,/,1x,i5,3x,5hnode1,1p8e11.3)
  363 format(1x,i5,3x,5hnode2,1p8e11.3)
  280 continue
      return
c
c         ind=3 and ind=4 passes
c
  300 if(ind.ne.4)return
      do 315 n=1,nume
      call echeck(lm(1,n),6,icode)
      if(icode)315,310,315
  310 continue
      ngin=1
      return
  315 continue
      return
      end
*deck shellax
       subroutine shellax(nstr,thic,e,pr,ilumps,xx,d,fnu1,rho,kpri,
     . fk,fc,fm,st,ft)
      implicit real*8 (a-h,o-z)
c
c         subroutine to compute stiffness, damping and mass matrix
c   for a two node axisymmetric shell element
c
      dimension xx(2,2),d(5,5),b(5,6),fk(6,6),st(8,6),
     . fc(6,6),fm(6,6),ishape(2),dt(6,6),stf(6,6),flm(6,6)
     . ,fkl(8,6),fcl(6,6),ft(8,6),q(8,8)
c
      data ndof/6/,  pi/3.14159265/, ishape/-1,1/
      dx=xx(1,2)-xx(1,1)
      dy=xx(2,2)-xx(2,1)
      fl=dsqrt(dx**2 + dy**2)
      sinphi=dx/fl
      cosphi=dy/fl
c
c     form global to local transformation matrix
c     [u(local)]=[flm]*[u(global)]
c
      do 41 i=1,6
      do 41 j=1,6
   41 flm(i,j)=0.0
      flm(1,1)=cosphi
      flm(1,2)=sinphi
      flm(2,1)=-sinphi
      flm(2,2)=cosphi
      flm(3,3)=1.0
      do 42 i=4,6
      do 42 j=4,6
   42 flm(i,j)=flm(i-3,j-3)
      go to (230,240,230,230)kpri
 230  continue
      trm=xx(1,1)+xx(1,2)
      b24sin=sinphi/trm
      b4cos=cosphi/trm
      do 10 i=1,5
      do 10 j=1,6
   10 b(i,j)=0.0
c
c         form displacement transformation matrix
c
      do 20 i=1,2
      iz=3*i
      iy=iz-1
      ix=iy-1
      bshape=dfloat(ishape(i))/fl
      b(1,ix)=bshape
      b(2,iy)=b4cos
      b(2,ix)=b24sin
      b(3,iz)=-bshape
      b(4,iz)=-b24sin
      b(5,iy)=bshape
   20 b(5,iz)=-.5
c
c         form stiffness and damping matrices
c
      fac=trm*pi*fl
      do 40 i=1,nstr
      do 40 j=1,ndof
      sum=0.0
      do 30 l=1,nstr
   30 sum=sum+d(i,l)*b(l,j)
   40 stf(i,j)=sum
      do 60 i=1,ndof
      do 60 j=1,ndof
      sum=0.0
      do 50 l=1,nstr
   50 sum=sum+b(l,i)*stf(l,j)
      fkl(i,j)=sum*fac
   60 fcl(i,j)=fac*fnu1*sum
      if(kpri.eq.4)go to 250
c
c     transform local stiffness, damping and mass  matrices
c     into global matrices
c
      do 43 i=1,ndof
      do 43 j=1,ndof
      sum=0.0
      tum=0.0
      do 44 l=1,ndof
      tum=tum+fcl(i,l)*flm(l,j)
   44 sum=sum+fkl(i,l)*flm(l,j)
      dt(i,j)=tum
   43 stf(i,j)=sum
      do 45 i=1,ndof
      do 45 j=1,ndof
      tum=0.0
      sum=0.0
      do 46 l=1,ndof
      tum=tum+flm(l,i)*dt(l,j)
   46 sum=sum+flm(l,i)*stf(l,j)
      fc(i,j)=tum
   45 fk(i,j)=sum
      return
  240 continue
c
c         lumped mass matrix
c
      do 12 i=1,6
      do 12 j=1,6
   12 fm(i,j)=0.0
      facm=rho*thic*2.0*pi*fl/3.0
      fm(1,1)=facm*(xx(1,2)+.5*xx(1,1))
      fm(2,2)=fm(1,1)
      fm(4,4)=facm*(xx(1,1)+.5*xx(1,2))
      fm(5,5)=fm(4,4)
      return
  250 continue
c
c          force transformation matrix
c
      facf=e*thic
      facm=facf*thic**2/12.0
      do 31 i=5,7,2
      do 31 j=1,6
   31 fkl(i,j)=fkl(i-1,j)
      do 53 i=2,4,2
      k=i+4
      do 53 j=1,6
      fkl(k,j)=pr*fkl(i+3,j)
   53 fkl(i,j)=pr*fkl(i-1,j)
      fkl(2,1)=fkl(2,1) - facf*sinphi*2.0*pi
      fkl(2,2)=fkl(2,2) - facf*cosphi*2.0*pi
      fkl(4,3)=fkl(4,3) - facm*sinphi*2.0*pi
      fkl(6,4)=fkl(6,4) + facf*sinphi*2.0*pi
      fkl(6,5)=fkl(6,5) + facf*cosphi*2.0*pi
      fkl(8,6)=fkl(8,6) + facm*sinphi*2.0*pi
      do 51 i=1,8
      do 51 j=1,6
      sum=0.0
      do 52 l=1,6
   52 sum=sum + fkl(i,l)*flm(l,j)
   51 ft(i,j)=sum
c
c         stress transformation matrix
c
c            [st]=[q]*[ft]
c
      q(1,1)= -1.0/(thic*2.0*pi*xx(1,1))
      q(1,3)=6.0*q(1,1)/thic
      q(2,1)=q(1,1)
      q(2,3)=-q(1,3)
      q(3,2)=-q(1,1)
      q(3,4)=q(1,3)
      q(4,2)=q(3,2)
      q(4,4)=q(2,3)
      q(5,5)=1.0/(thic*2.0*pi*xx(1,2))
      q(5,7)=6.0*q(5,5)/thic
      q(6,5)=q(5,5)
      q(6,7)=-q(5,7)
      q(7,6)=q(6,5)
      q(7,8)=q(5,7)
      q(8,6)=q(7,6)
      q(8,8)=-q(7,8)
      do 67 i=1,8
      do 67 j=1,6
      st(i,j)=0.0
      do 67 l=1,8
   67 st(i,j)=st(i,j) + q(i,l)*ft(l,j)
      return
      end
*deck shelln
       subroutine shelln(nstr,thic,e,pr,ilumps,xx,d,fnu1,rho,kpri,
     . fk,fc,fm,st,ft)
      implicit real*8 (a-h,o-z)
c
c         subroutine to compute stiffness, damping and mass matrix
c   for a two node axisymmetric shell element
c
      dimension xx(2,2),d(5,5),b(5,6),fk(6,6),st(8,6),
     . fc(6,6),fm(6,6),ishape(2),dt(6,6),stf(6,6),flm(6,6)
     . ,fkl(8,6),fcl(6,6),ft(8,6),q(8,8),fmt(6,6),fml(6,6)
c
      data ndof/6/,  pi/3.14159265/, ishape/-1,1/
      dx=xx(1,2)-xx(1,1)
      dy=xx(2,2)-xx(2,1)
      fl=dsqrt(dx**2 + dy**2)
      sinphi=dx/fl
      cosphi=dy/fl
c
c     form global to local transformation matrix
c     [u(local)]=[flm]*[u(global)]
c
      do 41 i=1,6
      do 41 j=1,6
   41 flm(i,j)=0.0
      flm(1,1)=cosphi
      flm(1,2)=sinphi
      flm(2,1)=-sinphi
      flm(2,2)=cosphi
      flm(3,3)=1.0
      do 42 i=4,6
      do 42 j=4,6
   42 flm(i,j)=flm(i-3,j-3)
      trm=xx(1,1)+xx(1,2)
      go to (230,240,230,230)kpri
 230  continue
      b24sin=sinphi/trm
      b4cos=cosphi/trm
      b2cos=cosphi*fl/(4.*trm)
      do 10 i=1,5
      do 10 j=1,6
   10 b(i,j)=0.0
c
c         form displacement transformation matrix
c
      do 20 i=1,2
      iz=3*i
      iy=iz-1
      ix=iy-1
      bshape=dfloat(ishape(i))/fl
      b(1,ix)=bshape
      b(2,iy)=b4cos
      b(2,ix)=b24sin
      b(2,iz)=-dfloat(ishape(i))*b2cos
      b(3,iz)=-bshape
      b(4,iz)=-b24sin
      b(5,iy)=bshape
   20 b(5,iz)=-.5
c
c         form stiffness and damping matrices
c
      fac=trm*pi*fl
      do 40 i=1,nstr
      do 40 j=1,ndof
      sum=0.0
      do 30 l=1,nstr
   30 sum=sum+d(i,l)*b(l,j)
   40 stf(i,j)=sum
      do 60 i=1,ndof
      do 60 j=1,ndof
      sum=0.0
      do 50 l=1,nstr
   50 sum=sum+b(l,i)*stf(l,j)
      fkl(i,j)=sum*fac
   60 fcl(i,j)=fac*fnu1*sum
      if(kpri.eq.4)go to 250
      if(kpri.ne.2)go to 241
  240 continue
c
c     c o n s i s t e n t   m a s s   m a t r i x
c
      do 12 i=1,6
      do 12 j=1,6
   12 fml(i,j)=0.0
      facm=pi*fl*rho*thic*trm
      do 101 i=1,2
      do 101 j=1,2
      fml(i,j)=.25
  101 fml(i+2,j+2)=.25
      do 102 i=3,4
      fml(i,5)=fl/16.
      fml(i,6)=-fml(i,5)
      fml(5,i)=fml(i,5)
  102 fml(6,i)=fml(i,6)
      fml(5,5)=fl**2/64. + thic**2/48.
      fml(6,6)=fml(5,5)
      fml(5,6)=-fml(5,5)
      fml(6,5)=fml(5,6)
  241 continue
c
c     transform local stiffness, damping and mass  matrices
c     into global matrices
c
      do 43 i=1,ndof
      do 43 j=1,ndof
      sum=0.0
      tum=0.0
      fmum=0.0
      do 44 l=1,ndof
      tum=tum+fcl(i,l)*flm(l,j)
      fmum=fmum+fml(i,l)*flm(l,j)
   44 sum=sum+fkl(i,l)*flm(l,j)
      dt(i,j)=tum
      fmt(i,j)=fmum
   43 stf(i,j)=sum
      do 45 i=1,ndof
      do 45 j=1,ndof
      tum=0.0
      fmum=0.0
      sum=0.0
      do 46 l=1,ndof
      tum=tum+flm(l,i)*dt(l,j)
      fmum=fmum+flm(l,i)*fmt(l,j)
   46 sum=sum+flm(l,i)*stf(l,j)
      fc(i,j)=tum
      fm(i,j)=fmum*facm
   45 fk(i,j)=sum
      return
  250 continue
c
c          force transformation matrix
c
      facf=e*thic
      facm=facf*thic**2/12.0
      do 31 i=5,7,2
      do 31 j=1,6
   31 fkl(i,j)=fkl(i-1,j)
      do 53 i=2,4,2
      k=i+4
      do 53 j=1,6
      fkl(k,j)=pr*fkl(i+3,j)
   53 fkl(i,j)=pr*fkl(i-1,j)
      fkl(2,1)=fkl(2,1) - facf*sinphi*2.0*pi
      fkl(2,2)=fkl(2,2) - facf*cosphi*2.0*pi
      fkl(4,3)=fkl(4,3) - facm*sinphi*2.0*pi
      fkl(6,4)=fkl(6,4) + facf*sinphi*2.0*pi
      fkl(6,5)=fkl(6,5) + facf*cosphi*2.0*pi
      fkl(8,6)=fkl(8,6) + facm*sinphi*2.0*pi
      do 51 i=1,8
      do 51 j=1,6
      sum=0.0
      do 52 l=1,6
   52 sum=sum + fkl(i,l)*flm(l,j)
   51 ft(i,j)=sum
c
c         stress transformation matrix
c
c            [st]=[q]*[ft]
c
      q(1,1)= -1.0/(thic*2.0*pi*xx(1,1))
      q(1,3)=6.0*q(1,1)/thic
      q(2,1)=q(1,1)
      q(2,3)=-q(1,3)
      q(3,2)=-q(1,1)
      q(3,4)=q(1,3)
      q(4,2)=q(3,2)
      q(4,4)=q(2,3)
      q(5,5)=1.0/(thic*2.0*pi*xx(1,2))
      q(5,7)=6.0*q(5,5)/thic
      q(6,5)=q(5,5)
      q(6,7)=-q(5,7)
      q(7,6)=q(6,5)
      q(7,8)=q(5,7)
      q(8,6)=q(7,6)
      q(8,8)=-q(7,8)
      do 67 i=1,8
      do 67 j=1,6
      st(i,j)=0.0
      do 67 l=1,8
   67 st(i,j)=st(i,j) + q(i,l)*ft(l,j)
      return
      end
*deck pzel
      subroutine pzel
      implicit real*8 (a-h,o-z)
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/dtwo/itwo,ndof,idim
      common/tapcom/nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlin
c
      common a(1)
      real a
       ncoeff=40
c      first 12 elements are the coeef. of cl matrix, .coeef of upper
c      left 3*3 submatrix and diagonal terms of 4,5 & 6th rows.
c      next 9 elements are coeeff. of epsl matrix ,next 18 elments
c      are coeef of d-matrix & last element is variable  "rho"
       nnlst=5
c      nnlst=total  number of integer variables.all  are dummy for
c      this ruotine
       nnodel=8
c      nnodel= number of nodes per element.=8 for this element
c      varible nn3=no of node pts/ele*dim of coord( 2 for 2-d,3 for 3-d)
       nn3=nnodel*idim
c      variable nn5=no of node pts per element*no of dof/node
       nn5=nnodel*ndof
      nfirst=n6e+4*itwo
      n101=nfirst
c     n102=9*nummat*itwo+n101
      n102=ncoeff*nummat*itwo+n101
c     n103=4*nume*itwo+n102
      n103=nn3*nume*itwo+n102
      n104=nume+n103
c     n105=12*nume+n104
      n105=nn5*nume+n104
c     nlast=4*nummat+n105
      nlast=nnlst*nummat+n105
      midest=nlast-nfirst+1
      min=nmax+nlast-n1l+1
c      *************  variables nfrc1 & nstr1 define the number=
c      total number of forces & stresses to be calculated at a point.
c      these numbers will differ with different elements.
c      nptsf= number of points at which forces and stresses are to be
c             calculated
       nfrc1=8
       nstr1=6
       nptsf=2
       nfrc=nfrc1*nptsf
       nstr=nstr1*nptsf
      if(ind.eq.0)write(6,20)min
      if(mod(nn5,2).eq.1) go to 50
      nnsz=(nn5+1)*nn5/2
      go to 60
  50  nnsz=nn5*(nn5-1)/2
      nnsz=nnsz+nn5
  60  continue
      if(min.le.nmax) go to 10
      stop 4
  10    continue
c     print c,n1,n2,n3,n4,n5,n6,n7,n8
c      print 255,n1l,n2l,n3l,n4l,nc1,nc2,nc3,nc4,n14
  255  format( 10x,"n1l,n2l,n3l, , , n14  ",9i5)
c      print 256,ind
  256  format(10x," ind in routine pzel =  ",i5)
c      print *,(a(ijk),ijk=1,50)
      call
     . pzelp(a(n2),a(n2l),a(n1l),a(n2),a(n3),a(n4),a(n101),a(n102),
     . a(n103),
     .a(n104),a(n105),nume,nummat,ndof,ind,kpri,nt11,ncoeff,nn5,nnsz
     .,nfrc,nstr,nptsf,idim,nnlst,numnp,nnodel)
c     print c,n1,n2,n3,n4,n5,n6,n7,n8
c      print 255,n1l,n2l,n3l,n4l,nc1,nc2,nc3,nc4,n14
c      print 256,ind
c      print *,(a(ijk),ijk=1,50)
      if(ind.eq.0)call ewrite(a(n6e),ng)
       if(ind.eq.0) write(6,40) nume,nummat,ndof,ind,kpri,nt11,ncoeff
     .,nn5,nnsz,nfrc,nstr,nptsf,idim,nnlst,numnp
  40   format(/10x,"nume,nummat,ndof,ind,kpri,nt11,ncoeff,nn5,nnsz,
     .,nfrc,nstr,nptsf,idim,nnlst,numnp"/10x,15i5)
   20 format (1h0,39htorage required to input element data =,i10)
   30 format (1h0,21herror nmax less than ,i10)
      return
      end
cdeck pzelp
*deck pzelp
      subroutine pzelp(a,id,mht,x,y,z,fmat,xy,mat,lm,iprop,nume,
     .nummat,ndof,ind,kpri,nt11,ncoeff,nn5,nnsz,nfrc,nstr,nptsf,
     .idim,nnlst,numnp,nnodel)
      implicit real*8 (a-h,o-z)
       common/comtyp/itypee,ngroup,ngin,nlin
       common/prop/cl,epsl,dl
      common /list/ data(64),ndum
      common/switch/ipsm,ipkbar,ipinpt,ipreac,ieldat,ipsdm,ipcnpf
     . ,ibigw
c
      dimension x(1),y(1),z(1),fmat(40,1),mat(1),lm(32,1),stiff(528),
     .xmass(528),fk(32,32),fm(32,32),xy(3,8,1),
     . nonum(8),id(ndof,1),mht(1),iprop(5,1),ur(32),
     .cl(6,6),epsl(3,3),dl(3,6),ui(32)
c     all the dimensions above should be equal to or bigger than
c     the dimensions given below interms of the variables.all
c     variable size are defined in the previous subroutine ashl
c     dimension x(1),y(1),fmat(ncoeff,1),mat(1),lm(nn5,1),stiff(nnsz),
c    .xmass(nnsz),damp(nnsz),ft(nfrc,nn5),fk(nn5,nn5),fc(nn5,nn5),
c    .fm(nn5,nn5),xy(idim,nnodel,1),nonum(nnodel),id(nnodel,1),mht(1),
c    .iprop(nnlst,1),c(15),d(10,10),ur(nn5),ui(nn5),forcer(nfrc),
c    .forcei(nfrc),sigr(nstr),sigi(nstr),st(nstr,nn5)
c     print 100
  100 format(//20x,"      pzelp   routine     ")
c      print 256,ind
  256  format( 10x,"   ind=   ",i5)
      if (ind.ne.0) go to 200
c
c         read and store material data
c
      do 115 m=1,nummat
      call fld
      do 110 n=1,ncoeff
  110 fmat(n,m)=data(n+nnlst)
      do 115 n=1,nnlst
  115 iprop(n,m)=data(n)
c      print *,(data(m),m=1,64)
c
c         read and store element information
c
      if(ibigw.eq.0)go to 33
      if(ieldat.eq.0)go to 33
c      write(6,140)
   33 continue
      do 150 n=1,nume
      call fld
c      print *,(data(ikj),ikj=1,15)
      num=data(1)
      mat(n)=data(2)
      do 135 i=1,nnodel
      node=data(i+2)
      nonum(i)=node
c     write(6,5) n,node
   5   format(10x,"******  element no=  ",i5,"  node no=  ",i5,"  **")
c       *********************************************************
c       vector lm is boundary condition code. lm(nnodel*ndof,nume)
c      where nume=total number of elements
c      **********************************************************
       do 134 k=1,ndof
       k1=ndof-k+1
       k2=ndof*i-k+1
       lm(k2,n)=id(k1,node)
  134  continue
       xy(1,i,n)=x(node)
       if(idim.gt.1) xy(2,i,n)=y(node)
       if(idim.gt.2) xy(3,i,n)=z(node)
  135   continue
      call colht(mht,nn5,lm(1,n))
c      print 100
c      print *,(x(i),y(i),z(i),i=1,8)
c
      if(ibigw.eq.0)go to 150
      if(ieldat.eq.0)go to 150
c      write(6,145) n,mat(n),(nonum(i),i=1,8)
  140 format(///10x,12helement data,/,2x,3helm,2x,3hmat,2x,5hnodes//)
  145 format(2i5,8i7)
c
  150 continue
c
      return
c
c         ind=1 pass to form element matrices
c
  200 if (ind.ne.1.and.ind.ne.3) go to 300
      do 280 n=1,nume
      icode=0
      if (ind.eq.1) call echeck(lm(1,n),nn5,icode)
      if (icode.eq.1) go to 280
      m=mat(n)
c      following five ineger varibles are dummy, not used here
      ilumps=iprop(1,m)
      idamps=iprop(2,m)
      istres=iprop(3,m)
      nft=iprop(4,m)
      inshel=iprop(5,m)
      if(ind.eq.3.and.istres.eq.0)go to 280
      if(kpri.eq.3.and.idamps.eq.0)go to 280
      ijj=0
      do 75 ij1=1,3
      do 75 ij2=1,3
      ijj=ijj+1
      cl(ij1,ij2)=fmat(ijj,m)
  75  continue
      do 78 ij1=4,6
      ijj=ijj+1
      cl(ij1,ij1)=fmat(ijj,m)
  78  continue
      do 76 ij1=1,3
      do 76 ij2=1,3
      ijj=ijj+1
      epsl(ij1,ij2)=fmat(ijj,m)
  76  continue
      do 77 ij1=1,3
      do 77 ij2=1,6
      ijj=ijj+1
      dl(ij1,ij2)=fmat(ijj,m)
  77  continue
      ijj=ijj+1
      rho=fmat(ijj,m)
      kprip=kpri
      if(ind.eq.3)kprip=4
c      print 1,((xy(ij1,ij2,n),ij1=1,3),ij2=1,8)
  1    format(5x,6e15.5)
      call pzelst(rho,xy,fk,fm,ur,kprip,n,ndof)
      if (ind.eq.3) go to 255
c
      m=0
      do 220 i=1,nn5
      do 220 j=i,nn5
      m=m+1
      stiff(m)=fk(i,j)
c     damp(m)=fc(i,j)
  220 xmass(m)=fm(i,j)
c     print of element stiffness  and mass matrices
      if(ibigw.eq.0)go to 950
      if(ipsdm.eq.0)go to 950
      if(kpri.ne.1)go to 702
c      write(6,956)n
  956 format(1x//////,1x,30h stiffness for pzel element n=,i5,//)
c      write(6,953)((fk(ii,jj),jj=1,nn5),ii=1,nn5)
  953 format(1x,1p8e11.3)
  702 if(kpri.ne.3)go to 703
c     write(6,957)n
c 957 format(1x,//////,1x,35h damping matrix for pzel element n=,i5//)
c     write(6,953)((fc(ii,jj),jj=1,nn5),ii=1,nn5)
  703 if(kpri.ne.2)go to 950
c      write(6,960)n
  960 format(1x,//////,32h mass matrix for pzel element n=,i5//)
c      write(6,953)((fm(ii,jj),jj=1,nn5),ii=1,nn5)
  950 continue
c
      go to (230,240,250), kpri
c
c         assemble stiffness matrix
c
  230 continue
      call addban(a,mht,stiff,lm(1,n),nn5)
      go to 280
c
c         assemble mass matrix
c
  240 call addban(a,mht,xmass,lm(1,n),nn5)
      go to 280
c
c         assemble damping matrix
c
  250 call addban(a,mht,damp,lm(1,n),nn5)
      go to 280
c
c    ind =3 pass
c
  255 do 320 i=1,nn5
c
c       expand solution displ vector (fill in constraint zeros)
c
      idof=lm(i,n)
      ur(i)=0.0
      ui(i)=0.0
      if(idof.eq.0) go to 320
      ur(i)=x(idof)
      ui(i)=y(idof)
  320 continue
  280  continue
      return
c
c         ind=3 and ind=4 passes
c
  300 if(ind.ne.4)return
      do 420 n=1,nume
      call echeck(lm(1,n),nn5,icode)
      if(icode)420,410,420
  410 continue
      ngin=1
      return
  420 continue
      return
      end
*deck pzelst
       subroutine pzelst(rho,xy,kmm,fm,ur,kprip,n,ndof)
       implicit real*8 (a-h,o-z)
       common/cord/x,y,z
       real*8 kuu,kuphi,kphi,km,kmm
       common/prop/cl,epsl,dl
       dimension x(8),y(8),z(8),epsl(3,3),
     1 dl(3,6),cl(6,6),kuu(24,24),kuphi(24,8),kphi(8,8)
       dimension km(32,32),kmm(32,32),ii(32),xy(3,8,1),fm(32,32)
       data ii/1,9,17,25,2,10,18,26,3,11,19,27,4,12,20,28,5,13,21,29,
     1 6,14,22,30,7,15,23,31,8,16,24,32/
c      print 100
  100  format(10x,"   x,y,z  coordinates in pzelst routine ")
       do 1 i=1,8
       x(i)=xy(1,i,n)
       y(i)=xy(2,i,n)
       z(i)=xy(3,i,n)
c      print 101,x(i),y(i),z(i)
  101  format(5x,6e15.8)
  1    continue
c      print 102
  102  format(10x,"  cl (6,6) matrix")
       do 2 i=1,6
c      read *,(cl(i,j),j=1,6)
c      print 101,(cl(i,j),j=1,6)
  2    continue
c      print 103
  103  format(10x,"epsl(3,3)  matrix")
       do 3 i=1,3
c      read *,(epsl(i,j),j=1,3)
c      print 101,(epsl(i,j),j=1,3)
  3    continue
c      print 104
  104  format(10x,"dl(6,6) matrix")
       do 4 i=1,3
c      read *,(dl(i,j),j=1,6)
c      print 101,(dl(i,j),j=1,6)
  4    continue
       call deriv(8,n,kuu,kuphi,kphi,volm)
c      print 106
  106  format(20x,"  pzelst routine  ")
c      print 108
  108  format(10x,"kuu(24,24) matrix")
       do 9 i=1,24
c      print 101,(kuu(i,j),j=1,24)
  9    continue
c      print 109
  109  format(10x,"kuphi(24,8)  matrix")
       do 10 i=1,24
c      print 101,(kuphi(i,j),j=1,8)
  10   continue
c      print  110
  110  format(10x,"kphi(8,8) matrix")
       do 111 i=1,8
c      print 101,(kphi(i,j),j=1,8)
  111  continue
       do 21 i=1,24
       do 21 j=1,24
  21   km(i,j)=kuu(i,j)
       do 22 i=25,32
       i1=i-24
       do 22 j=1,24
  22   km(i,j)=kuphi(j,i1)
       do 23 i=1,24
       do 23 j=25,32
       j1=j-24
  23   km(i,j)=kuphi(i,j1)
       do 24 i=25,32
       i1=i-24
       do 24 j=25,32
       j1=j-24
  24   km(i,j)=kphi(i1,j1)
       do 25 i=1,32
       i1=ii(i)
       do 25 j=1,32
       j1=ii(j)
       kmm(i,j)=km(i1,j1)
  25   continue
c      print 30
  30   format(////20x,"  untransformed stiffness matrix")
       do 31 i=1,32
c      print 101,(km(i,j),j=1,32)
  31   continue
c      print 32
  32   format(//// 20x,"transformed stiffness matrix")
       do 33 i=1,32
c      print 101,(kmm(i,j),j=1,32)
  33   continue
       r1=rho*volm/8.
       do 43 i=1,32
       do 43 j=1,32
       fm(i,j)=0.
       if(i.eq.j) fm(i,i)=r1
  43   continue
       do 44 i=4,32,4
  44   fm(i,i)=0.
       return
       end
*deck drcosn
       subroutine drcosn(w,q)
       implicit real*8 (a-h,o-z)
       common/cord/x,y,z
       dimension x(8),y(8),z(8),w(3,3),q(6,6)
       real*8 l1,l2,l3,m1,m2,m3,n1,n2,n3
c      print 104
  104  format(  //   20x,"   drcosn  routine  ")
c      print 102,(x(i),y(i),z(i),i=1,8)
c      go to 170
c      xl12=(x(2)-x(1))**2+(y(2)-y(1))**2+(z(2)-z(1))**2
c      xl22=(x(4)-x(1))**2+(y(4)-y(1))**2+(z(4)-z(1))**2
c      xl32=(x(5)-x(1))**2+(y(5)-y(1))**2+(z(5)-z(1))**2
c      xl1=dsqrt(xl12)
c      xl2=dsqrt(xl22)
c      xl3=dsqrt(xl32)
c      l1=(x(2)-x(1))/xl1
c      m1=(y(2)-y(1))/xl1
c      n1=(z(2)-z(1))/xl1
c      l3=(x(1)-x(5))/xl3
c      m3=(y(1)-y(5))/xl3
c      n3=(z(1)-z(5))/xl3
c      l2=(x(4)-x(1))/xl2
c      m2=(y(4)-y(1))/xl2
c      n2=(z(4)-z(1))/xl2
c      go to 150
  170  xl12=(x(2)-x(1))**2+(y(2)-y(1))**2+(z(2)-z(1))**2
       xl1=dsqrt(xl12)
       vn1x=(y(2)-y(1))*(z(4)-z(1))-(y(4)-y(1))*(z(2)-z(1))
       vn1y=(x(4)-x(1))*(z(2)-z(1))-(x(2)-x(1))*(z(4)-z(1))
       vn1z=(x(2)-x(1))*(y(4)-y(1))-(y(2)-y(1))*(x(4)-x(1))
       xl32=vn1x**2+vn1y**2+vn1z**2
       xl3=dsqrt(xl32)
       vy1x=vn1y*(z(2)-z(1))-vn1z*(y(2)-y(1))
       vy1y=vn1z*(x(2)-x(1))-vn1x*(z(2)-z(1))
       vy1z=vn1x*(y(2)-y(1))-vn1y*(x(2)-x(1))
       xl22=vy1x**2+vy1y**2+vy1z**2
       xl2=dsqrt(xl22)
       l1=(x(2)-x(1))/xl1
       m1=(y(2)-y(1))/xl1
       n1=(z(2)-z(1))/xl1
       l2=vy1x/xl2
       m2=vy1y/xl2
       n2=vy1z/xl2
       l3=vn1x/xl3
       m3=vn1y/xl3
       n3=vn1z/xl3
  150  continue
       w(1,1)=l1
       w(1,2)=l2
       w(1,3)=l3
       w(2,1)=m1
       w(2,2)=m2
       w(2,3)=m3
       w(3,1)=n1
       w(3,2)=n2
       w(3,3)=n3
       q(1,1)=l1*l1
       q(1,2)=l2*l2
       q(1,3)=l3*l3
       q(1,4)=l1*l2
       q(1,5)=l2*l3
       q(1,6)=l3*l1
       q(2,1)=m1*m1
       q(2,2)=m2*m2
       q(2,3)=m3*m3
       q(2,4)=m1*m2
       q(2,5)=m2*m3
       q(2,6)=m3*m1
       q(3,1)=n1*n1
       q(3,2)=n2*n2
       q(3,3)=n3*n3
       q(3,4)=n1*n2
       q(3,5)=n2*n3
       q(3,6)=n3*n1
       q(4,1)=2.*l1*m1
       q(4,2)=2.*l2*m2
       q(4,3)=2.*l3*m3
       q(4,4)=l1*m2+l2*m1
       q(4,5)=l2*m3+l3*m2
       q(4,6)=l3*m1+l1*m3
       q(5,1)=2.*m1*n1
       q(5,2)=2.*m2*n2
       q(5,3)=2.*m3*n3
       q(5,4)=m1*n2+m2*n1
       q(5,5)=m2*n3+m3*n2
       q(5,6)=m3*n1+m1*n3
       q(6,1)=2.*n1*l1
       q(6,2)=2.*n2*l2
       q(6,3)=2.*n3*l3
       q(6,4)=n1*l2+n2*l1
       q(6,5)=n2*l3+n3*l2
       q(6,6)=n3*l1+n1*l3
c      print 100
  100  format(10x,"w(3,3)  and  q(6,6)  matrices")
       do 101 i=1,3
c      print 102,(w(i,j),j=1,3)
  101  continue
c      do 103 i=1,6
c      print 102,(q(i,j),j=1,6)
  102  format(10x,6e16.6)
       return
       end
*deck coord
       subroutine coord(jmax,xi,eta,zeta,wt)
       implicit real*8 (a-h,o-z)
       dimension xi(27),eta(27),zeta(27),wt(27)
       x=0.57735
       y=0.7746
       a=0.55556
       b=0.88889
       a3=a**3
       a2b=a*a*b
       ab2=a*b*b
       b3=b**3
       if(jmax.eq.27) go to 100
       do 1 i=1,8
  1    wt(i)=1.
       xi(1)=-x
       xi(2)=x
       xi(3)=-x
       xi(4)=x
       xi(5)=-x
       xi(6)=x
       xi(7)=-x
       xi(8)=x
       eta(1)=-x
       eta(2)=-x
       eta(3)=x
       eta(4)=x
       eta(5)=-x
       eta(6)=-x
       eta(7)=x
       eta(8)=x
       do 2 i=1,8
       if(i.lt.5) zeta(i)=-x
       if(i.gt.4) zeta(i)=x
  2    continue
       return
  100  do 3 i=1,9
       i1=(i-1)*3+1
       i2=i1+1
       i3=i2+1
       xi(i1)=-y
       xi(i2)=0.
       xi(i3)=y
  3    continue
       do 4 i=1,27
       eta(i)=0.
       zeta(i)=0.
  4    continue
       do 5 i=1,3
       i1=(i-1)*9+1
       i2=i1+1
       i3=i2+1
       i4=i1+6
       i5=i2+6
       i6=i3+6
       eta(i1)=-y
       eta(i2)=-y
       eta(i3)=-y
       eta(i4)=y
       eta(i5)=y
       eta(i6)=y
  5    continue
       do 6 i=1,3
       do 6 j=1,9
       i1=(i-1)*9+j
       if(i.eq.1) zeta(i1)=-y
       if(i.eq.3) zeta(i1)=y
       if(i.eq.2) zeta(i1)=0.
  6    continue
       do 7 i=1,6
       i1=i*2
       wt(i1)=a2b
  7    continue
       wt(1)=a3
       wt(3)=a3
       wt(7)=a3
       wt(9)=a3
       wt(5)=ab2
       wt(11)=ab2
       wt(13)=ab2
       wt(14)=b3
       do 8 i=1,14
       i1=28-i
       wt(i1)=wt(i)
   8   continue
       return
       end
*deck deriv
       subroutine deriv(jmax,n,kuu,kuphi,kphi,volm)
       implicit real*8 (a-h,o-z)
       common/prop/cl,epsl,dl
       common/cord/x,y,z
       real*8 kuu,kuphi,kphi
       dimension xi(27),eta(27),zeta(27),dnxi(8),dneta(8),
     1 dnzeta(8),dnx(8),dny(8),dnz(8),x(8),y(8),z(8),
     2 xib(8),etab(8),zetab(8),cl(6,6),epsl(3,3),dl(3,6)
       dimension bj(3,3),bji(3,3),wt(27),egl(6,3),cgl(6,6),epsgl(3,3)
c
       dimension bu(24,24),bphi(8,8),kuu(24,24),kuphi(24,8),kphi(8,8)
       data xib/-1.,1.,1.,-1.,-1.,1.,1.,-1./
       data etab/-1.,-1.,1.,1.,-1.,-1.,1.,1./
       data zetab/1.,1.,1.,1.,-1.,-1.,-1.,-1./
       call propgl(cgl,egl,epsgl)
c      print *,((cgl(i,j),i=1,6),j=1,6)
       call coord(jmax,xi,eta,zeta,wt)
c      print 90
  90   format(  20x,"   deriv   routine   ")
c      print 101,(x(i),y(i),z(i),i=1,8)
       volm=0.
       do 1 j=1,jmax
       do 2 i=1,8
       dnxi(i)=(1.+etab(i)*eta(j))*(1.+zetab(i)*zeta(j))*xib(i)/8.
       dneta(i)=(1.+xib(i)*xi(j))*(1.+zetab(i)*zeta(j))*etab(i)/8.
       dnzeta(i)=(1.+xib(i)*xi(j))*(1.+etab(i)*eta(j))*zetab(i)/8.
   2   continue
       do 3 i=1,3
       do 3 k=1,3
  3    bj(i,k)=0.
       do 4 i=1,8
       bj(1,1)=bj(1,1)+dnxi(i)*x(i)
       bj(1,2)=bj(1,2)+dnxi(i)*y(i)
       bj(1,3)=bj(1,3)+dnxi(i)*z(i)
       bj(2,1)=bj(2,1)+dneta(i)*x(i)
       bj(2,2)=bj(2,2)+dneta(i)*y(i)
       bj(2,3)=bj(2,3)+dneta(i)*z(i)
       bj(3,1)=bj(3,1)+dnzeta(i)*x(i)
       bj(3,2)=bj(3,2)+dnzeta(i)*y(i)
       bj(3,3)=bj(3,3)+dnzeta(i)*z(i)
   4   continue
c      print 100
  100  format(20x,"jacobian and its inverse")
c      print 101,((bj(i,k),k=1,3),i=1,3)
  101  format(10x,3f20.8)
c***       d1=1.
c***       call linv3f(bj,bji,1,3,3,d1,d2,wk,ier)
c      print 101,((bj(i,k),k=1,3),i=1,3)
       call inv(bj,bji,bbj)
c***       do 22 i=1,3
c***       do 22 k=1,3
c***  22    bji(i,k)=bj(i,k)
c***       bbj=d1*(2.)**d2
       if(j.eq.1.and.bbj.eq.0.) write(6,810) n
  810  format(20x,"******  singular jacobian  ***** element no =",i5)
       if(bbj.eq.0.) stop
       if(j.eq.1.and.bbj.lt.0.) write(6,811) n,bbj
  811  format(20x," ***** negative jacobian  *****  element no= ",i5
     1 ,f20.4/20x,"jacobian is changed to +ve but you are advised"
     2 ," to change numbering of this element & rerun problem")
       if(bbj.lt.0.) bbj=-bbj
       volm=volm+bbj*wt(j)
c      print 101,bbj
       do 5 k=1,8
       dnx(k)=0.
       dny(k)=0.
       dnz(k)=0.
  5    continue
       do 6 k=1,8
       dnx(k)=bji(1,1)*dnxi(k)+bji(1,2)*dneta(k)+bji(1,3)*
     1 dnzeta(k)
       dny(k)=bji(2,1)*dnxi(k)+bji(2,2)*dneta(k)+bji(2,3)*
     1 dnzeta(k)
       dnz(k)=bji(3,1)*dnxi(k)+bji(3,2)*dneta(k)+bji(3,3)*
     1 dnzeta(k)
  6    continue
       do 50 m=1,6
       do 50 nn=1,24
  50   bu(m,nn)=0.
       do 51 nn=1,8
  51   bu(1,nn)=dnx(nn)
       do 52 nn=9,16
       n1=nn-8
  52   bu(2,nn)=dny(n1)
       do 53 nn=17,24
       n1=nn-16
  53   bu(3,nn)=dnz(n1)
       do 54 nn=9,24
       n1=nn-8
       n2=nn-16
       if(nn.lt.17)bu(4,nn)=dnz(n1)
       if(nn.gt.16) bu(4,nn)=dny(n2)
  54   continue
       do 55 nn=1,8
  55   bu(5,nn)=dnz(nn)
       do 56 nn=17,24
       n1=nn-16
  56   bu(5,nn)=dnx(n1)
       do 57 nn=1,16
       n1=nn-8
       if(nn.lt.9) bu(6,nn)=dny(nn)
       if(nn.gt.8) bu(6,nn)=dnx(n1)
  57   continue
       do 58 nn=1,8
       bphi(1,nn)=dnx(nn)
       bphi(2,nn)=dny(nn)
       bphi(3,nn)=dnz(nn)
  58   continue
c      print *,((bu(im,jm),im=1,6),jm=1,24)
c      print *,((cgl(im,jm),im=1,6),jm=1,6)
       do 70  im=1,24
       do 70 jm=1,24
       if(j.eq.1) kuu(im,jm)=0.
       do 70 mm=1,6
       do 70 nm=1,6
       kuu(im,jm)=kuu(im,jm)+bu(mm,im)*cgl(mm,nm)*bu(nm,jm)*wt(j)*bbj
  70   continue
c      print 999,j
  999  format(20x,"    kuu  matrix at j=   ",i5)
c      print *,((kuu(im,jm),jm=1,24),im=1,24)
       do 71 im=1,24
       do 71 jm=1,8
       if(j.eq.1) kuphi(im,jm)=0.
       do 71 mm=1,6
       do 71 nm=1,3
       kuphi(im,jm)=kuphi(im,jm)+
     1 bu(mm,im)*egl(mm,nm)*bphi(nm,jm)*wt(j)*bbj
  71   continue
       do 72 im=1,8
       do 72 jm=1,8
       if(j.eq.1) kphi(im,jm)=0.
       do 72 mm=1,3
       do 72 nm=1,3
       kphi(im,jm)=kphi(im,jm)+
     1 bphi(mm,im)*epsgl(mm,nm)*bphi(nm,jm)*wt(j)*bbj
  72   continue
   1   continue
       return
       end
*deck propgl
       subroutine propgl(cgl,egl,epsgl)
       implicit real*8 (a-h,o-z)
       common/cord/x,y,z
       dimension epsl(3,3),dl(3,6),cl(6,6),el(6,3)
       dimension cgl(6,6),egl(6,3),epsgl(3,3),w(3,3),q(6,6)
       dimension x(8),y(8),z(8)
       common/prop/cl,epsl,dl
c      call inverse routine to get inverse of sl=cl
c      print 100
  100  format(  //20x,"  propgl  routine  ")
c      print 101,(x(i),y(i),z(i),i=1,8)
  101   format(15x,3f20.8)
c      print 101,((cl(i,j),i=1,3),j=1,3)
       call drcosn(w,q)
       do 1 i=1,6
       do 1 j=1,3
       el(i,j)=0.
       do 1 k=1,6
       el(i,j)=el(i,j)+cl(i,k)*dl(j,k)
  1    continue
       do 2 i=1,6
       do 2 j=1,6
       cgl(i,j)=0.
       do 2 m=1,6
       do 2 n=1,6
       cgl(i,j)=cgl(i,j)+q(m,i)*cl(m,n)*q(n,j)
  2    continue
c      print 101,((cgl(i,j),i=1,6),j=1,6)
       do 3 i=1,6
       do 3 j=1,3
       egl(i,j)=0.
       do 3 m=1,6
       do 3 n=1,3
       egl(i,j)=egl(i,j)+q(m,i)*el(m,n)*w(n,j)
  3    continue
       do 4 i=1,3
       do 4 j=1,3
       epsgl(i,j)=0.
       do 4 m=1,3
       do 4 n=1,3
       epsgl(i,j)=epsgl(i,j)+w(m,i)*epsl(m,n)*w(n,j)
   4   continue
       return
       end
*deck inv
       subroutine inv(a,b,det)
       implicit real*8 (a-h,o-z)
       dimension a(3,3),b(3,3),c(3,3)
       a11=a(1,1)
       a12=a(1,2)
       a13=a(1,3)
       a21=a(2,1)
       a22=a(2,2)
       a23=a(2,3)
       a31=a(3,1)
       a32=a(3,2)
       a33=a(3,3)
       det=a11*(a22*a33-a32*a23)-a12*(a21*a33-a31*a23)+
     1 a13*(a21*a32-a31*a22)
       if(det.eq.0.) return
       b(1,1)=a22*a33-a32*a23
       b(1,2)=-(a12*a33-a13*a32)
       b(1,3)=a12*a23-a13*a22
       b(2,1)=-(a21*a33-a31*a23)
       b(2,2)=a11*a33-a13*a31
       b(2,3)=-(a11*a23-a13*a21)
       b(3,1)=a21*a32-a22*a31
       b(3,2)=-(a11*a32-a12*a31)
       b(3,3)=a11*a22-a12*a21
       do 1 i=1,3
       do 1 j=1,3
  1    b(i,j)=b(i,j)/det
       do 2 i=1,3
       do 2 j=1,3
       c(i,j)=0.
       do 2 k=1,3
       c(i,j)=c(i,j)+a(i,k)*b(k,j)
  2    continue
       return
       end
*deck datain
      subroutine datain(s,lm,nt)
      implicit real*8 (a-h,o-z)
      dimension s(1),lm(1)
      read(nt)k,(s(i),i=1,k),n,(lm(i),i=1,n)
      return
      end
*deck freac
      subroutine freac
      implicit real*8 (a-h,o-z)
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/dtwo/itwo,ndof,idim
      common/tapcom/nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlin
      common a(1)
      real a
      nfirst=n6e+4*itwo
      n101=nfirst
      n102=n101+nume*itwo
      n103=n102+2*nume
      nlast=n103+nume
      midest=nlast-(nfirst-1)
      min=nmax+nlast-n1l+1
      if(ind.eq.0)write(6,2020)min
      if(min.le.nmax)go to 10
      write(6,2010)nlast
      stop
   10 continue
      call freacp(a(n2l),a(n1l),a(n103),a(n102),a(n101),nume,ndof)
      if(ind.eq.0)call ewrite(a(n6e),ng)
      return
 2010 format(1h0,10x,20herror nmax less than  ,i10)
 2020 format(1h0,10x,41hstorage required to input element data = ,i10)
      end
*deck freacp
      subroutine freacp(id,mht,irtype,lm,value,nume,ndof)
      implicit real*8 (a-h,o-z)
      dimension id(ndof,1),irtype(1),lm(2,1),value(1)
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/comtyp/itype,ngroup,ngin,nlin
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat
      common a(1)
      real a
      character*8 cdata
      common/clist/cdata(64)
      common/list/data(64),ndum
      common/tapcom/nt11,nt31
      dimension ss(3),lmm(2)
      character*8 dtype
      dimension dtype(3)
      data dtype/'spf','maf','daf'/
      nd=2
      if(ind.ne.0)go to 20
      i=0
c***      if(kpelem.eq.0)go to 5
      write(6,2000)
      write(6,2010)nume
      write(6,2020)
    5 continue
      call fld
      it=1
      if(cdata(1).eq.dtype(2))it=2
      if(cdata(1).eq.dtype(3))it=3
      node1=data(2)
      node2=data(3)
      val=data(4)
      idof1=data(5)
      idof2=data(6)
      if(idof1.le.0)idof1=1
      if(idof2.le.0)idof2=1
      ntyme=data(7)
      if(ntyme.le.1)ntyme=1
      inc=data(8)
      if(inc.le.1)inc=1
      node1=node1-inc
      node2=node2-inc
      do 10 jj=1,ntyme
      node1=node1+inc
      node2=node2+inc
      i=i+1
      irtype(i)=it
      lm(1,i)=id(idof1,node1)
      lm(2,i)=id(idof2,node2)
      value(i)=val
      call colht(mht,nd,lm(1,i))
c***      if(kpelem.eq.0)go to 10
      write(6,2030)i,dtype(it),node1,node2,val,idof1,idof2
   10 continue
      if(i.lt.nume)go to 5
      return
   20 if(ind.ne.1.and.ind.ne.4)return
      ktyp=kpri-2
      do 70 n=1,nume
      call echeck(lm(1,n),nd,icode)
      if(icode.eq.1)go to 70
      ngin=1
      if(ind.eq.4)return
      if(irtype(n).ne.kpri)go to 70
c***      if(ktyp)30,40,40
   30 continue
c***      s=1./value(n)
c****     go to 50
   40 continue
      s=value(n)
   50 continue
      ss(1)=0.0
      ss(2)=+s
      ss(3)=0.0
      lmm(1)=lm(1,n)
      lmm(2)=lm(2,n)
      if(lmm(1).eq.lmm(2))lmm(2)=0
      call addban(a(n2),a(n1l),ss,lmm,nd)
   70 continue
      return
 2000 format(1h1)
 2010 format(39h r e a c t a n c e  d e f i n i t i o n ,//
     .       39h number of elements.................. = ,i5)
 2020 format(//,7x,1hn,1x,4htype,6x,2hn1,6x,2hn2,6x,3hval,5x,4hdof1,1x,
     .       4hdof2)
 2030 format(i8,2x,a3,2i8,1pe12.4,2i5)
      end
*deck react
      subroutine react
      implicit real*8 (a-h,o-z)
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/dtwo/itwo,ndof,idim
      common/tapcom/nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlin
      common a(1)
      real a
      nfirst=n6e+4*itwo
      n101=nfirst
      n102=n101+nume*itwo
      n103=n102+2*nume
      nlast=n103+nume
      midest=nlast-(nfirst-1)
      min=nmax+nlast-n1l+1
      if(ind.eq.0)write(6,2020)min
      if(min.le.nmax)go to 10
      write(6,2010)nlast
      stop
   10 continue
      call reactp(a(n2l),a(n1l),a(n103),a(n102),a(n101),nume,ndof)
      if(ind.eq.0)call ewrite(a(n6e),ng)
      return
 2010 format(1h0,10x,20herror nmax less than  ,i10)
 2020 format(1h0,10x,41hstorage required to input element data = ,i10)
      end
*deck reactp
      subroutine reactp(id,mht,irtype,lm,value,nume,ndof)
      implicit real*8 (a-h,o-z)
      dimension id(ndof,1),irtype(1),lm(2,1),value(1)
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/comtyp/itype,ngroup,ngin,nlin
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat
      common a(1)
      real a
      character*8 cdata
      common/clist/cdata(64)
      common/list/data(64),ndum
      common/tapcom/nt11,nt31
      dimension ss(3),lmm(2)
      character*8 dtype
      dimension dtype(6)
      data dtype/'cap','ind','res', 'k','m','d'/
      nd=2
      if(ind.ne.0)go to 20
      i=0
      if(kpelem.eq.0)go to 5
      write(6,2000)
      write(6,2010)nume
      write(6,2020)
    5 continue
      call fld
      it=1
      if(cdata(1).eq.dtype(2))it=2
      if(cdata(1).eq.dtype(3))it=3
      if(cdata(1).eq.dtype(4))it=4
      if(cdata(1).eq.dtype(5))it=5
      if(cdata(1).eq.dtype(6))it=6
c     write(6,*)'it ',it,cdata(1),dtype
      node1=data(2)
      node2=data(3)
      val=data(4)
      idof1=data(5)
      idof2=data(6)
      if(idof1.le.0)idof1=1
      if(idof2.le.0)idof2=1
      ntyme=data(7)
      if(ntyme.le.1)ntyme=1
      inc=data(8)
      if(inc.le.1)inc=1
      node1=node1-inc
      node2=node2-inc
      do 10 jj=1,ntyme
      node1=node1+inc
      node2=node2+inc
      i=i+1
      irtype(i)=it
      lm(1,i)=id(idof1,node1)
      lm(2,i)=id(idof2,node2)
      value(i)=val
      call colht(mht,nd,lm(1,i))
      if(kpelem.eq.0)go to 10
      write(6,2030)i,dtype(it),node1,node2,val,idof1,idof2
   10 continue
      if(i.lt.nume)go to 5
      return
   20 if(ind.ne.1.and.ind.ne.4)return
      ktyp=kpri-2
      do 70 n=1,nume
      call echeck(lm(1,n),nd,icode)
      if(icode.eq.1)go to 70
      ngin=1
      if(ind.eq.4)return
      if(irtype(n)-3.ne.kpri)go to 35
      go to 40
   35 continue
      if(irtype(n).ne.kpri)go to 70
      if(ktyp)30,40,40
   30 continue
      s=1./value(n)
      go to 50
   40 continue
      s=value(n)
   50 continue
      ss(1)=s
      ss(2)=-s
      ss(3)=s
      lmm(1)=lm(1,n)
      lmm(2)=lm(2,n)
      if(lmm(1).eq.lmm(2))lmm(2)=0
      call addban(a(n2),a(n1l),ss,lmm,nd)
   70 continue
      return
 2000 format(1h1)
 2010 format(39h r e a c t a n c e  d e f i n i t i o n ,//
     .       39h number of elements.................. = ,i5)
 2020 format(//,7x,1hn,1x,4htype,6x,2hn1,6x,2hn2,6x,3hval,5x,4hdof1,1x,
     .       4hdof2)
 2030 format(i8,2x,a3,2i8,1pe12.4,2i5)
      end
*deck quad4
      subroutine quad4
      implicit real*8 (a-h,o-z)
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/dtwo/itwo,ndof,idim
      common/tapcom/nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlin
c
      common a(1)
      real a
c
      nfirst=n6e+4*itwo
      n101=nfirst
      n102=25*nummat*itwo+n101
      n103=8*nume*itwo+n102
      n104=nume+n103
      n105=8*nume+n104
      nlast=5*nummat+n105
      midest=nlast-nfirst+1
      min=nmax+nlast-n1l+1
      if(ind.eq.0)write(6,20)min
      if(min.le.nmax) go to 10
      stop 4
   10 call
     .quad4p(a(n2),a(n2l),a(n1l),a(n2),a(n3),a(n101),a(n102),a(n103),
     .a(n104),a(n105),nume,nummat,ndof,ind,kpri,nt11)
      if(ind.eq.0)call ewrite(a(n6e),ng)
   20 format (1h0,40hstorage required to input element data =,i10)
   30 format (1h0,21herror nmax less than ,i10)
      return
      end
*deck quad4p
      subroutine quad4p(a,id,mht,x,y,fmat,xy,mat,lm,iprop,nume,nummat,
     . ndof,ind,kpri,nt11)
      implicit real*8 (a-h,o-z)
c
      common /list/ data(64),ndum
      common/switch/ipsm,ipkbar,ipinpt,ipreac,ieldat,ipsdm,ipcnpf
     . ,ibigw
      common/comtyp/itypee,ngroup,ngin,nlin
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat
c
      dimension x(1),y(1),fmat(25,1),mat(1),lm(8,1),stiff(36),xmass(36),
     . damp(36),st(4,8),fk(8,8),fc(8,8),fm(8,8),xy(2,4,1),nonum(4),
     . id(ndof,1),mht(1),iprop(5,1),c(10),ctil(10),d(4,4),dtil(4,4)
      dimension ur(8),ui(8),sigr(4),sigi(4)
c
      if (ind.ne.0) go to 200
c
c         read and store material data
c
      do 115 m=1,nummat
      call fld
      do 110 n=1,25
  110 fmat(n,m)=data(n+5)
      do 112 n=1,5
  112 iprop(n,m)=data(n)
      if(kpmat.eq.0)go to 115
      write(6,1000)m,(iprop(n,m),n=1,5)
      write(6,1010)(fmat(n,m),n=1,25)
  115 continue
c
c         read and store element information
c
      if(ibigw.eq.0)go to 146
      if(ieldat.eq.0)go to 146
      if(kpelem.eq.0)go to 146
      write(6,140)
  146 continue
      do 150 n=1,nume
      call fld
      num=data(1)
      if (num.ne.n) stop 5
      mat(n)=data(2)
      do 135 i=1,4
      node=data(i+2)
      nonum(i)=node
      lm(2*i-1,n)=id(1,node)
      lm(2*i,n)=id(2,node)
      xy(1,i,n)=x(node)
  135 xy(2,i,n)=y(node)
c
      call colht(mht,8,lm(1,n))
      if(ibigw.eq.0)go to 150
      if(ieldat.eq.0)go to 150
      if(kpelem.eq.0)go to 150
c
      write(6,145) n,mat(n),(nonum(i),i=1,4)
  140 format(///10x,12helement data,/,2x,3helm,2x,3hmat,2x,5hnode1,
     . 2x,5hnode2,2x,5hnode3,2x,5hnode4//)
  145 format(2i5,4i7)
c
  150 continue
c
      return
c
c         ind=1 pass to form element matrices
c
  200 if(ind.ne.1 .and. ind.ne.3) go to 300
      if(ind.eq.3.and.kpstre.eq.1)write(6,390)
  390 format(1h0,10x,33hcomplex stress for quad4 elements,//,1x,5h  elm,
     . 2(3x,8hsigma-xx),2(3x,8hsigma-yy),2(4x,6htau-xy,1x),
     . 2(3x,8hsigma-yy) ,/,5x,3(5x,4hreal,7x,4himag,2x),//)
      do 280 n=1,nume
      icode=0
      if (ind.eq.1) call echeck(lm(1,n),8,icode)
      if (icode.eq.1) go to 280
      m=mat(n)
      itype=iprop(1,m)
      nint=iprop(2,m)
      ilumps=iprop(3,m)
      idamps=iprop(4,m)
      istres=iprop(5,m)
      if(ind.eq.3.and.istres.eq.0)go to 280
      if(kpri.eq.3.and.idamps.eq.0)go to 280
      thic=fmat(1,m)
      e=fmat(2,m)
      pr=fmat(3,m)
      fnu1=fmat(4,m)
      rho=fmat(5,m)
      do 210 i=1,10
      c(i)=fmat(5+i,m)
  210 ctil(i)=fmat(15+i,m)
      if (thic.eq.0.0) thic=1.0d0
      nstr=3
      if (itype.eq.0) nstr=4
      if (e.eq.0.0) go to 211
      f=e/(1.0d0+pr)
      g=f*pr/(1.0d0-2.0d0*pr)
      h=f+g
      c(1)=h
      c(2)=g
      c(3)=0.0d0
      c(4)=g
      c(5)=h
      c(6)=0.0d0
      c(7)=g
      c(8)=f/2.0d0
      c(9)=0.0d0
      c(10)=h
c
c         fill in full 4x4 stress strain matrix
c
  211 ic=0
      do 212 i=1,4
      do 212 j=i,4
      ic=ic+1
      d(i,j)=c(ic)
      d(j,i)=c(ic)
      dtil(i,j)=ctil(ic)
  212 dtil(j,i)=ctil(ic)
c
c         modify for plane stress
c
      if (itype.ne.2) go to 219
      if (d(4,4).eq.0.0) go to 217
      do 215 i=1,3
      con=d(i,4)/d(4,4)
      do 215 j=1,3
  215 d(i,j)=d(i,j)-d(4,j)*con
c
  217 if (dtil(4,4).eq.0.0) go to 219
      do 218 i=1,3
      con=dtil(i,4)/dtil(4,4)
      do 218 j=1,3
  218 dtil(i,j)=dtil(i,j)-dtil(4,j)*con
  219 kprip=kpri
      if(ind.eq.3)kprip=4
      call sgqm1(n,itype,nstr,nint,thic,e,pr,xy(1,1,n),d,dtil,fnu1,
     . rho,ilumps,kprip,st,fk,fc,fm)
      if(ind.eq.3) go to 255
c
      m=0
      do 220 i=1,8
      do 220 j=i,8
      m=m+1
      stiff(m)=fk(i,j)
      damp(m)=fc(i,j)
  220 xmass(m)=fm(i,j)
c      print of element stiff damp and mass matrices and pointer array
      if(ibigw.eq.0)go to 950
      if(ipsdm.ne.1)go to 950
      write(6,951)n
  951 format(1h0,22hprint lm(i,n) array,n= ,i5//)
      write(6,952)(lm(ii,n),ii=1,8)
  952 format(1x,8i6//)
      if(kpri.ne.1)go to 702
      write(6,956)n
  956 format(1x//////,31h stiffness for quad4 element n= ,i5//)
      write(6,953)((fk(ii,jj),jj=1,8),ii=1,8)
  953 format(1x,8e13.5)
  702 if(kpri.ne.3)go to 703
      write(6,957)n
  957 format(1x,//////,36h damping matrix for quad4 element n= ,i5//)
      write(6,953)((fc(ii,jj),jj=1,8),ii=1,8)
  703 if(kpri.ne.2)go to 950
      write(6,960)n
  960 format(1x,//////,33h mass matrix for quad4 element n= ,i5//)
      write(6,953)((fm(ii,jj),jj=1,8),ii=1,8)
  950 continue
c
      go to (230,240,250), kpri
c
c         assemble stiffness matrix
c
  230 call addban(a,mht,stiff,lm(1,n),8)
      go to 280
c
c         assemble mass matrix
c
  240 call addban(a,mht,xmass,lm(1,n),8)
      go to 280
c
c         assemble damping matrix
c
  250 call addban(a,mht,damp,lm(1,n),8)
      go to 280
c
c    ind =3 pass
c
  255 do 320 i=1,8
c
c       expand solution displ vector (fill in constraint zeros)
c
      idof=lm(i,n)
      ur(i)=0.0d0
      ui(i)=0.0d0
      if(idof.eq.0) go to 320
      ur(i)=x(idof)
      ui(i)=y(idof)
  320 continue
c
c compute stresses
c     multiply stress transformation matrix times displacement vector
c
      do 340 i=1,nstr
      sigr(i)=0.0d0
      sigi(i)=0.0d0
      do 340 j=1,8
      sigr(i)=sigr(i)+st(i,j)*ur(j)
  340 sigi(i)=sigi(i)+st(i,j)*ui(j)
c
c      print stresses
c
      if(kpstre.eq.0)go to 280
      write(6,360)n,(sigr(i),sigi(i),i=1,nstr)
  360 format(i5,1p8e11.3)
c
  280 continue
      return
c
c      ind=2 and ind=4 passes
c
  300 if(ind.ne.4)return
      do 420 n=1,nume
      call echeck(lm(1,n),8,icode)
      if(icode)420,410,420
  410 continue
      ngin=1
      return
  420 continue
      return
 1000 format(1h0,10x,31hquad4 properties for mat number,i3,//,
     .2x,5hitype,3x,4hnint,1x,6hilumps,1x,6hidamps,1x,6histres,/,5i7,/)
 1010 format(1x,1p5e12.4)
      end
*deck tri
      subroutine tri
      implicit real*8 (a-h,o-z)
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/dtwo/itwo,ndof,idim
      common/tapcom/nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlin
c
      common a(1)
      real a
c
      nfirst=n6e+4*itwo
      n101=nfirst
      n102=25*nummat*itwo+n101
      n103=8*nume*itwo+n102
      n104=nume+n103
      n105=8*nume+n104
      nlast=5*nummat+n105
      midest=nlast-nfirst+1
      min=nmax+nlast-n1l+1
      if(ind.eq.0)write(6,20)min
      if(min.le.nmax) go to 10
      stop 4
   10 call
     . trip(a(n2),a(n2l),a(n1l),a(n2),a(n3),a(n101),a(n102),a(n103),
     .a(n104),a(n105),nume,nummat,ndof,ind,kpri,nt11)
c***
c***  if(ind.eq.0) write(nt31)
c*** .nume,nummat,itype,midest,(a(i),i=nfirst,nlast)
      if(ind.eq.0)call ewrite(a(n6e),ng)
   20 format (1h0,39htorage required to input element data =,i10)
   30 format (1h0,21herror nmax less than ,i10)
      return
      end
*deck trip
      subroutine trip(a,id,mht,x,y,fmat,xy,mat,lm,iprop,nume,nummat,
     . ndof,ind,kpri,nt11)
      implicit real*8 (a-h,o-z)
c
      common /list/ data(64),ndum
      common/switch/ipsm,ipkbar,ipinpt,ipreac,ieldat,ipsdm,ipcnpf
     . ,ibigw
      common/comtyp/itypee,ngroup,ngin,nlin
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat
c
      dimension x(1),y(1),fmat(25,1),mat(1),lm(8,1),stiff(21),xmass(21),
     . damp(21),st(4,6),fk(6,6),fc(6,6),fm(6,6),xy(2,4,1),nonum(4),
     . id(ndof,1),mht(1),iprop(5,1),c(10),ctil(10),d(4,4),dtil(4,4),
     . ur(6),ui(6),sigr(4),sigi(4)
c
      if (ind.ne.0) go to 200
c
c         read and store material data
c
      do 115 m=1,nummat
      call fld
      do 110 n=1,25
  110 fmat(n,m)=data(n+5)
      do 112 n=1,5
  112 iprop(n,m)=data(n)
      write(6,1000)m,(iprop(n,m),n=1,5)
      if(kpmat.eq.0)go to 115
      write(6,1010)(fmat(n,m),n=1,25)
  115 continue
c
c         read and store element information
c
      if(ibigw.eq.0)go to 33
      if(ieldat.eq.0)go to 33
      if(kpelem.eq.0)go to 33
      write(6,140)
   33 continue
      do 150 n=1,nume
      call fld
      num=data(1)
      if (num.ne.n) stop 5
      mat(n)=data(2)
      do 135 i=1,3
      node=data(i+2)
      nonum(i)=node
      lm(2*i-1,n)=id(1,node)
      lm(2*i,n)=id(2,node)
      xy(1,i,n)=x(node)
  135 xy(2,i,n)=y(node)
c
      call colht(mht,6,lm(1,n))
c
      if(ibigw.eq.0)go to 150
      if(ieldat.eq.0)go to 150
      if(kpelem.eq.0)go to 150
      write(6,145) n,mat(n),(nonum(i),i=1,3)
  140 format(///10x,12helement data,/,2x,3helm,2x,3hmat,2x,5hnode1,
     . 2x,5hnode2,2x,5hnode3//)
  145 format(2i5,4i7)
c
  150 continue
c
      return
c
c         ind=1 pass to form element matrices
c
  200 if (ind.ne.1.and.ind.ne.3) go to 300
      if(ind.eq.0.and.kpstre.eq.1)write(6,390)
  390 format(1h0,10x,33hcomplex stresses for tri elements//1x,5h  elm,
     . 2(3x,8hsigma-xx),2(3x,8hsigma-yy),2(4x,6htau-xy,1x)
     . ,2(3x,8hsigma-zz)/5x,3(5x,4hreal,7x,4himag,2x)//)
      do 280 n=1,nume
      icode=0
      if (ind.eq.1) call echeck(lm(1,n),6,icode)
      if (icode.eq.1) go to 280
      m=mat(n)
      itype=iprop(1,m)
      nint=iprop(2,m)
      ilumps=iprop(3,m)
      idamps=iprop(4,m)
      istres=iprop(5,m)
      if(ind.eq.3.and.istres.eq.0)go to 280
      if(kpri.eq.3.and.idamps.eq.0)go to 280
      thic=fmat(1,m)
      e=fmat(2,m)
      pr=fmat(3,m)
      fnu1=fmat(4,m)
      rho=fmat(5,m)
      do 210 i=1,10
      c(i)=fmat(5+i,m)
  210 ctil(i)=fmat(15+i,m)
c
      if (thic.eq.0.0) thic=1.0
      nstr=3
      if (itype.eq.0) nstr=4
      if (e.eq.0.0) go to 211
      f=e/(1.0+pr)
      g=f*pr/(1.0-2.0*pr)
      h=f+g
      c(1)=h
      c(2)=g
      c(3)=0.0
      c(4)=g
      c(5)=h
      c(6)=0.0
      c(7)=g
      c(8)=f/2.0
      c(9)=0.0
      c(10)=h
c
c         fill in full 4x4 stress strain matrix
c
  211 ic=0
      do 212 i=1,4
      do 212 j=i,4
      ic=ic+1
      d(i,j)=c(ic)
      d(j,i)=c(ic)
      dtil(i,j)=ctil(ic)
  212 dtil(j,i)=ctil(ic)
c
c         modify for plane stress
c
      if (itype.ne.2) go to 219
      if (d(4,4).eq.0.0) go to 217
      do 215 i=1,3
      con=d(i,4)/d(4,4)
      do 215 j=1,3
  215 d(i,j)=d(i,j)-d(4,j)*con
c
  217 if (dtil(4,4).eq.0.0) go to 219
      do 218 i=1,3
      con=dtil(i,4)/dtil(4,4)
      do 218 j=1,3
  218 dtil(i,j)=dtil(i,j)-dtil(4,j)*con
  219 kprip=kpri
      if(ind.eq.3)kprip=4
      call sgtrm(nstr,ilumps,xy(1,1,n),thic,d,fnu1,dtil,rho,kprip,
     . fk,fc,fm,st)
      if (ind.eq.3) go to 255
c
      m=0
      do 220 i=1,6
      do 220 j=i,6
      m=m+1
      stiff(m)=fk(i,j)
      damp(m)=fc(i,j)
  220 xmass(m)=fm(i,j)
c      print of element stiff damp and mass matrices and pointer array
      if(ibigw.eq.0)go to 950
      if(ipsdm.ne.1)go to 950
      write(6,951)n
  951 format(1h0,22hprint lm(i,n) array,n= ,i5//)
      write(6,952)(lm(ii,n),ii=1,6)
  952 format(1x,6i6//)
      if(kpri.ne.1)go to 702
      write(6,956)n
  956 format(1x//////,30h stiffness for tri element n= ,i5//)
      write(6,953)((fk(ii,jj),jj=1,6),ii=1,6)
  953 format(1x,6e13.5)
  702 if(kpri.ne.3)go to 703
      write(6,957)n
  957 format(1x//////,1x,28h damping for tri element n= ,i5//)
      write(6,953)((fc(ii,jj),jj=1,6),ii=1,6)
  703 if(kpri.ne.2)go to 950
      write(6,960)n
  960 format(1x,//////,32h mass matrix for tri element n= ,i5//)
      write(6,953)((fm(ii,jj),jj=1,6),ii=1,6)
  950 continue
c
      go to (230,240,250), kpri
c
c         assemble stiffness matrix
c
  230 call addban(a,mht,stiff,lm(1,n),6)
      go to 280
c
c         assemble mass matrix
c
  240 call addban(a,mht,xmass,lm(1,n),6)
      go to 280
c
c         assemble damping matrix
c
  250 call addban(a,mht,damp,lm(1,n),6)
      go to 280
c
c    ind =3 pass
c
  255 do 320 i=1,6
c
c       expand solution displ vector (fill in constraint zeros)
c
      idof=lm(i,n)
      ur(i)=0.0
      ui(i)=0.0
      if(idof.eq.0) go to 320
      ur(i)=x(idof)
      ui(i)=y(idof)
  320 continue
c
c compute stresses
c     multiply stress transformation matrix times displacement vector
c
      do 340 i=1,nstr
      sigr(i)=0.0
      sigi(i)=0.0
      do 340 j=1,6
      sigr(i)=sigr(i)+st(i,j)*ur(j)
  340 sigi(i)=sigi(i)+st(i,j)*ui(j)
c
c      print stresses
c
      if(kpstre.eq.0)go to 280
      write(6,360)n,(sigr(i),sigi(i),i=1,nstr)
  360 format(i5,1p8e11.3)
c
  280 continue
      return
c
c         ind=3 and ind=4 passes
c
  300 if(ind.ne.4)return
      do 420 n=1,nume
      call echeck(lm(1,n),6,icode)
      if(icode)420,410,420
  410 continue
      ngin=1
      return
  420 continue
      return
 1000 format(1h0,10x,31htrip properties for mat number  ,i3,//,
     .2x,5hitype,3x,4hnint,1x,6hilumps,1x,6hidamps,1x,6histres,/,5i7,/)
 1010 format(1x,1p5e12.4)
      end
*deck sgtrm
       subroutine sgtrm(nstr,ilumps,xx,thic,d,fnu1,dtil,rho,kpri,
     . fk,fc,fm,st)
      implicit real*8 (a-h,o-z)
c
c         subroutine to compute stiffness, damping and mass matrix
c         for a constant strain triangle
c
      dimension xx(2,3),d(4,4),b(4,6),fk(6,6),st(4,6),dtil(4,4),
     . fc(6,6),fm(6,6),dt(4,6),dx(3),dy(3),dxy(3),iperm(3)
c
      data ndof/6/, iperm/2,3,1/, pi/3.14159265/
c
c
c         form coordinate differences
c
c     write(6,108)((xx(i,j),i=1,2),j=1,3)
  108 format(1x,"xx(i,j)= ",/,1x,3(2(e12.4,1x),/,1x))
      ravg=0.0
      zavg=0.0
      do 10 i=1,3
      ip=iperm(i)
      jp=iperm(ip)
      ravg=ravg + xx(1,i)/3.0
      zavg=zavg + xx(2,i)/3.0
      dx(i)=xx(1,jp)-xx(1,ip)
      dy(i)=xx(2,ip)-xx(2,jp)
   10 dxy(i)=xx(1,ip)*xx(2,jp) - xx(1,jp)*xx(2,ip)
      ta=dx(1)*dy(3)-dx(3)*dy(1)
c
c         form displacement transformation matrix
c
      go to (230,240,230,230)kpri
  230 continue
      do 20 i=1,3
      iy=2*i
      ix=iy-1
      b(1,ix)=dy(i)/ta
      b(2,iy)=dx(i)/ta
      b(3,ix)=dx(i)/ta
      b(3,iy)=dy(i)/ta
   20 b(4,ix)=(dxy(i)+dy(i)*ravg+dx(i)*zavg)/(ta*ravg)
c
c         form stiffness and damping matrices
c
      fac=thic*ta/2.0
      if(nstr.eq.4)fac=pi*ravg*ta
      do 40 i=1,nstr
      do 40 j=1,ndof
      sum=0.0
      tum=0.0
      do 30 l=1,nstr
      sum=sum+d(i,l)*b(l,j)
   30 tum=tum+dtil(i,l)*b(l,j)
      st(i,j)=sum
   40 dt(i,j)=tum
      if(kpri.eq.4)return
      do 60 i=1,ndof
      do 60 j=1,ndof
      sum=0.0
      tum=0.0
      do 50 l=1,nstr
      sum=sum+b(l,i)*st(l,j)
   50 tum=tum+b(l,i)*dt(l,j)
      fk(i,j)=sum*fac
   60 fc(i,j)=fac*(tum+fnu1*sum)
      return
  240 continue
c
c         mass matrix
c
      w=thic*ta*rho/2.0
      if(nstr.eq.4)w=ta*pi*ravg*rho
c     write(6,109)ta,ravg,w,rho
  109 format(1x,"ta= ",e12.4," ravg= ",e12.4,
     x" w= ",e12.4," rho= ",e12.4)
      off=0.0
      diag=w/3.0
      if (ilumps.eq.0) off=w/12.0
      if (ilumps.eq.0) diag=w/6.0
      do 80 i=1,ndof
      do 70 j=1,ndof
   70 fm(i,j)=off
   80 fm(i,i)=diag
c
      return
      end
*deck bemd
      subroutine bemd
      implicit real*8 (a-h,o-z)
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/dtwo/itwo,ndof,idim
      common/tapcom/nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlin
c
      common a(1)
      real a
c
      nfirst=n6e+4*itwo
      n101=nfirst
      n102=11*nummat*itwo+n101
      n103=4*nume*itwo+n102
      n104=nume+n103
      n105=6*nume+n104
      nlast=3*nummat+n105
      midest=nlast-nfirst+1
      min=nmax+nlast-n1l+1
      if(ind.eq.0)write(6,20)min
      if(min.le.nmax) go to 10
      stop 4
   10 call
     .bemdp(a(n2),a(n2l),a(n1l),a(n2),a(n3),a(n101),a(n102),a(n103),
     .a(n104),a(n105),nume,nummat,ndof,ind,kpri,nt11)
      if(ind.eq.0)call ewrite(a(n6e),ng)
   20 format (1h0,39htorage required to input element data =,i10)
   30 format (1h0,21herror nmax less than ,i10)
      return
      end
*deck bemdp
      subroutine bemdp(a,id,mht,x,y,fmat,xy,mat,lm,iprop,nume,
     . nummat,ndof,ind,kpri,nt11)
      implicit real*8 (a-h,o-z)
      common/comtyp/itypee,ngroup,ngin,nlen
c
      common /list/ data(64),ndum
      common/switch/ipsm,ipkbar,ipinpt,ipreac,ieldat,ipsdm,ipcnpf
     . ,ibigw
c
      dimension x(1),y(1),fmat(11,1),mat(1),lm(6,1),stiff(21),xmass(21),
     . damp(21),st(4,6),fk(6,6),fc(6,6),fm(6,6),xy(2,2,1),nonum(2),
     . id(ndof,1),mht(1),iprop(3,1),
     . ur(6),ui(6),sigr(4),sigi(4),cz(4),sl(6,6)
c
      if (ind.ne.0) go to 200
c
c         read and store material data
c
      do 115 m=1,nummat
      call fld
      do 110 n=1,11
  110 fmat(n,m)=data(n+3)
      do 115 n=1,3
  115 iprop(n,m)=data(n)
c
c         read and store element information
c
      if(ibigw.eq.0)go to 33
      if(ieldat.eq.0)go to 33
      write(6,140)
   33 continue
      do 150 n=1,nume
      call fld
      num=data(1)
      mat(n)=data(2)
      do 135 i=1,2
      node=data(i+2)
      nonum(i)=node
      lm(3*i-2,n)=id(1,node)
      lm(3*i-1,n)=id(2,node)
      lm(3*i,n)=id(3,node)
      xy(1,i,n)=x(node)
  135 xy(2,i,n)=y(node)
c
      call colht(mht,6,lm(1,n))
c
      if(ibigw.eq.0)go to 150
      if(ieldat.eq.0)go to 150
      write(6,145) n,mat(n),(nonum(i),i=1,2)
  140 format(///10x,12helement data,/,2x,3helm,2x,3hmat,2x,5hnode1,
     . 2x,5hnode2//)
  145 format(2i5,4i7)
c
  150 continue
c
      return
c
c         ind=1 pass to form element matrices
c
  200 if (ind.ne.1.and.ind.ne.3) go to 300
      if (ind.eq.3)write(6,390)
  390 format(1x,43hcomplex uniaxial stresses for beam elements//,
     . 7h    elm,3x,2(8hsigma1-u,3x),2(8hsigma1-l,3x),
     . 2(8hsigma2-u,3x),2(8hsigma2-l,3x),/,7x,
     . 4(5x,4hreal,7x,4himag,2x)//)
      do 280 n=1,nume
      icode=0
      if (ind.eq.1) call echeck(lm(1,n),6,icode)
      if (icode.eq.1) go to 280
      m=mat(n)
      ilumps=iprop(1,m)
      idamps=iprop(2,m)
      istres=iprop(3,m)
      if(ind.eq.3.and.istres.eq.0)go to 280
      if(kpri.eq.3.and.idamps.eq.0)go to 280
      area=fmat(1,m)
      e=fmat(2,m)
      pr=fmat(3,m)
      fnu1=fmat(4,m)
      rho=fmat(5,m)
      fksy=fmat(6,m)
      fiz=fmat(7,m)
      do 10 i=1,4
   10 cz(i)=fmat(i+7,m)
      kprip=kpri
      if(ind.eq.3)kprip=4
      call slbd(e,pr,area,xy(1,1,n),fiz,fksy,sl)
      call sgbd(sl,xy(1,1,n),rho,area,fiz,fnu1,ilumps,kprip,cz,st,
     . fk,fc,fm)
      if (ind.eq.3) go to 255
c
      m=0
      do 220 i=1,6
      do 220 j=i,6
      m=m+1
      stiff(m)=fk(i,j)
      damp(m)=fc(i,j)
  220 xmass(m)=fm(i,j)
c     print of element stiffness damping and mass matrices
      if(ibigw.eq.0)go to 950
      if(ipsdm.eq.0)go to 950
      if(kpri.ne.1)go to 702
      write(6,956)n
  956 format(1x//////,1x,30h stiffness for beam element n=,i5,//)
      write(6,953)((fk(ii,jj),jj=1,6),ii=1,6)
  953 format(1x,1p6e11.3)
  702 if(kpri.ne.3)go to 703
      write(6,957)n
  957 format(1x,//////,1x,35h damping matrix for beam element n=,i5//)
      write(6,953)((fc(ii,jj),jj=1,6),ii=1,6)
  703 if(kpri.ne.2)go to 950
      write(6,960)n
  960 format(1x,//////,32h mass matrix for beam element n=,i5//)
      write(6,953)((fm(ii,jj),jj=1,6),ii=1,6)
  950 continue
c
      go to (230,240,250), kpri
c
c         assemble stiffness matrix
c
  230 continue
      call addban(a,mht,stiff,lm(1,n),6)
      go to 280
c
c         assemble mass matrix
c
  240 call addban(a,mht,xmass,lm(1,n),6)
      go to 280
c
c         assemble damping matrix
c
  250 call addban(a,mht,damp,lm(1,n),6)
      go to 280
c
c    ind =3 pass
c
  255 do 320 i=1,6
c
c       expand solution displ vector (fill in constraint zeros)
c
      idof=lm(i,n)
      ur(i)=0.0
      ui(i)=0.0
      if(idof.eq.0) go to 320
      ur(i)=x(idof)
      ui(i)=y(idof)
  320 continue
c
c compute stresses
c     multiply stress transformation matrix times displacement vector
c
      do 340 i=1,4
      sigr(i)=0.0
      sigi(i)=0.0
      do 340 j=1,6
      sigr(i)=sigr(i)+st(i,j)*ur(j)
  340 sigi(i)=sigi(i)+st(i,j)*ui(j)
c
c      print stresses
c
      write(6,360)n,(sigr(i),sigi(i),i=1,4)
  360 format(1x,i5,1x,1p8e11.3)
c
  280 continue
      return
c
c         ind=3 and ind=4 passes
c
  300 if(ind.ne.4)return
      do 315 n=1,nume
      call echeck(lm(1,n),6,icode)
      if(icode)315,310,315
  310 continue
      ngin=1
      return
  315 continue
      return
      end
*deck slbd
      subroutine slbd(e,pr,a,xx,fiz,fksy,sl)
      implicit real*8 (a-h,o-z)
c.
c........................................................
c.  subroutine to calculate the local stiffness matrix for
c.  a beam element
c.
c.- - input variables- -
c. e = young@s modulus
c. a = cross sectional area
c.        xx(2,2)= x i=1 and y i=2 coordinates of the two endpoints
c.fiz= moment of inertia about the z axis
c.           cross section is y and z axis
c.        fksy= area factor for shear
c.pr = poisson@s ratio
c.- - output variables- -
c.      phiy= shear deflection correction factor
c.      g   = shear modulus of elasticity
c.sl(6,6)= local stiffness matrix
c.........................................................
      dimension sl(6,6),xx(2,2)
      fle=dsqrt((xx(1,2)-xx(1,1))**2+(xx(2,2)-xx(2,1))**2)
      asy=fksy*a
      g=e/(2.0*(1.0 + pr))
      if(asy.eq.0.0)go to 310
      phiy=12.0*e*fiz/(g*asy*fle**2)
  310 continue
      if(asy.eq.0.0)phiy=0.0
c.
c. local stiffness matrix
c.
      sl(1,1)=e*a/fle
      sl(4,1)=-sl(1,1)
      sl(2,2)=12.0*e*fiz/(fle**3*(1.0 +phiy))
      sl(3,2)=sl(2,2)*fle/2.0
      sl(5,2)=-sl(2,2)
      sl(6,2)=sl(3,2)
      sl(3,3)=(4.0 + phiy)*e*fiz/(fle*(1.0 + phiy))
      sl(5,3)=-sl(3,2)
      sl(6,3)=(2.0-phiy)*e*fiz/(fle*(1.0 + phiy))
      sl(4,4)=sl(1,1)
      sl(5,5)=sl(2,2)
      sl(6,5)=sl(5,3)
      sl(6,6)=sl(3,3)
c.
c. using symmetry fill in other half of matrix
c.
      do 60 i=1,6
      do 60 j=i,6
   60 sl(i,j)=sl(j,i)
      write(6,205)((sl(i,j),j=1,6),i=1,6)
  205 format(1x,40hlocal stiffness matrix for beam element=,
     1/,1x,6(6(1pe12.5,1x),/,1x))
      return
      end
*deck sgbd
      subroutine sgbd(sl,xx,rho,a,fiz,fnu1,ilumps,idamps,istres,cz,st,
     1fk,fc,fm)
      implicit real*8 (a-h,o-z)
      dimension fl(6,6),sl(6,6),flsl(6),xx(2,2),c(2,2)
      dimension fc(6,6),fm(6,6),fk(6,6),st(4,6),cz(4),fkt(6,6)
c.
c.
c.- - input variables- -
c.      fl(6,6)= direction cosine matrix,lambda
c.      sl(6,6)= local stiffness matrix to be transformed into
c.               global matrix
c.
c.      rho= material density
c.a= cross sectional area
c.      fnu1= damping constant
c.      ilumps= switch controling mass matrix  0-consistant mass
c.matrix> 1-lumped mass matrix
c.      idamps= switch controling damping matrix  0-no damping
c.matrix> 1-damping matrix
c.  istres= switch controling stress transformation matrix
c.0-no stress calculation> 1-stress calculation
c.
c.- - output variables- -
c.      fk(6,6)= global stiffness matrix
c.      fc(6,6)= damping matrix
c.      fm(6,6)= mass matrix
c.st(4,6)= stress transformation matrix
c.
c.
c.  calculation of the direction cosine matrix fl
c.
      x=xx(1,2)-xx(1,1)
      y=xx(2,2)-xx(2,1)
      fll= dsqrt(x**2+y**2)
c.
c.  calculate the direction cosines using the formula for the
c.  angle between two vectors, a and b  cos '=(a1*b1+a2*b2)/a*b
c.
c.  c(1,1)  a1=1.0, a2=0.0, b1=x, b2=y
c.  c(1,2)  a1=0.0, a2=1.0, b1=x, b2=y
c.  c(2,1)  a1=1.0, a2=0.0, b1=-y,b2=x
c.  c(2,2)  a1=0.0, a2=1.0, b1=-y,b2=x
c.
      do 5 i=1,2
      if(i.eq.2)go to 15
      b1=x
      b2=y
      go to 25
   15 b1=-y
      b2=x
   25 do 5 j=1,2
      if(j.eq.2)go to 35
      a1=1.0
      a2=0.0
      go to 5
   35 a1=0.0
      a2=1.0
    5 c(i,j)=(a1*b1 + a2*b2)/fll
      write(6,202)((c(i,j),j=1,2),i=1,2)
  202 format(1x,9hc matrix=,/,1x,2(2(1pe12.5,1x),/,1x))
c.
c.
c.  formation of the lambda matrix from the c matrix
c.
c.
      do 45 i=1,6
      do 45 j=1,6
   45 fl(i,j)=0.0
      do 55 i=1,2
      do 55 j=1,2
   55 fl(i,j)=c(i,j)
      fl(3,3)=1.0
      do 65 i=4,6
      do 65 j=4,6
   65 fl(i,j)=fl(i-3,j-3)
      write(6,203)((fl(i,j),j=1,6),i=1,6)
  203 format(1x,18hlambda,fl, matrix=,/,1x,6(6(1pe12.5,1x),/,1x))
c.
c.
c.  transformation of the local matrix to the global matrix
c.  using the relation  s=fl(transpose)*sl*fl
c.
      do 10 i=1,6
      do 10 j=1,6
      fm(i,j)=0.0
      fc(i,j)=0.0
   10 fk(i,j)=0.0
      do 70 j=1,6
      do 40 k=1,6
      flsl(k)=0.0
      do 40 l=1,6
   40 flsl(k)=flsl(k) + sl(k,l)*fl(l,j)
      do 60 i=1,6
      stiff=0.0
      do 50 l=1,6
      if(istres.eq.0)go to 61
      fkt(l,j)=flsl(l)
   61 continue
   50 stiff=stiff + fl(l,i)*flsl(l)
   60 fk(i,j)=fk(i,j) + stiff
   70 continue
      write(6,71)((fkt(m,n),n=1,6),m=1,6)
   71 format(1x,4hfkt=,/,2x,6(6(1pe12.5,1x),/,1x))
c.
c. c a l c u l a t i o n  o f  t h e  l u m p e d  m a s s  m a t r i x
c.
      w=rho*a*fll
      if(ilumps.eq.0)go to 96
      do 97 i=1,6
      fm(i,i)=w/2.0
      if(i.eq.3.or.i.eq.6)fm(i,i)=w/24*fll**2
   97 continue
      go to 98
c.
c. c a l c u l a t i o n  o f  t h e  c o n s i s t a n t  m a s s
c.   m a t r i x
c.
   96 continue
      do 99 i=1,6
   99 fm(i,i)=w/3.0
      fm(1,4)=w/6.0
      fm(2,5)=fm(1,4)
      fm(3,6)=fm(1,4)
      fm(6,3)=fm(3,6)
      fm(5,2)=fm(2,5)
      fm(4,1)=fm(1,4)
   98 continue
c.
c. c a l c u l a t i o n  o f  t h e  d a m p i n g  m a t r i x
c.
      if(idamps.eq.0)go to 509
      do 102 i=1,6
      do 102 j=1,6
  102 fc(i,j)=fnu1*fk(i,j)
  509 continue
c.
c. c a l c u l a t i o n  o f  t h e  s t r e s s  t r a n s -
c. f o r m a t i o n  m a t r i x
c.
      write(6,68)(cz(i),i=1,4)
   68 format(1x,6hcz(i)=,/,2x,4(1pe12.5,/,1x))
      if(istres.eq.0)go to 507
      do 131 i=1,2
      do 131 j=1,6
  131 st(i,j)=-1.0/a*fkt(1,j) + (-1.0)**i*cz(i)/fiz*fkt(3,j)
      do 132 i=3,4
      do 132 j=1,6
  132 st(i,j)=1.0/a*fkt(4,j) + (-1.0)**i*cz(i)/fiz*fkt(6,j)
  507 continue
      return
      end
*deck sgqm1
      subroutine sgqm1(nel,itype,nstr,nint,thic,e,pr,xx,d,dtil,fnu1,
     1rho,ilumps,kpri,st,fk,fc,fm)
      implicit real*8 (a-h,o-z)
c
c..............................................................
c.
c.    p r o g r a m
c.    to calculate isoparametric quadrilateral element stiffness
c.    mass and damping matrices for plane stress and plane strain
c.    conditions
c.
c.- - input variables - -
c.      nel       = number of element
c.      itype     = element type
c.              eq.0 = axisymmetric
c.                     eq.1 = plane strain
c.                     eq.2 = plane stress
c.                   eq.3 = input matrix
c.c(6)  = input material property matrix
c.      ctil(6) = input damping matrix
c.      fnu1      = damping constant
c.      ilumps    = switch for mass matrix' 0-consistant mass matrix
c.       1-lumped mass matrix
c.      nint      = gauss numerical integration order
c.      thic      = thickness of element
c.      e        = young"s modulus
c.      pr        = poisson"s ratio
c.      xx(2,4)   = element node coordinates
c.      fk(8,8)    = stiffness matrix on solution exit
c.
c.- - output - -
c.      fk(8,8)    = stiffness matrix
c.      fm(8,8)   = mass matrix
c.      fc(8,8)    = damping matrix
c.st(3,8)= stress transformation matrix
c......................................................................
      dimension d(4,4),b(4,8),xx(2,4),fk(8,8),xg(4,4),wgt(4,4),db(4)
      dimension hh(2,8),fm(8,8)
      dimension dtil(4,4),st(4,8),fc(8,8),dtilb(4)
c
c
c
c
c     matrix xg stores gauss - legendre sampling points
c
      data xg/   0.d0,   0.d0,   0.d0,   0.d0,   -.5773502691896d0,
     1 .5773502691896d0,   0.d0,   0.d0,   -.7745966692415d0,   0.d0,
     2 .7745966692415d0,   0.d0,   -.8611363115941d0,
     3 -.3399810435849d0,   .3399810435849d0,   .8611363115941d0 /
c
c      matrix wgt stores gauss - legendre weighting factors
c
      data wgt /  2.d0,   0.d0,   0.d0,   0.d0,   1.d0,   1.d0,
     1 0.d0,   0.d0,   .5555555555556d0,   .8888888888889d0,
     2 .5555555555556d0,   0.d0,   .3478548451375d0,
     3 .6521451548625d0,   .6521451548625d0,   .3478548451375d0 /
      data ndof /8/,pi /3.141592653590d0/
      go to (230,240,230,260)kpri
  240 continue
      do 31 i=1,ndof
      do 31 j=1,ndof
   31 fm(i,j)=0.0d0
      if(ilumps.eq.1)go to 290
  230 continue
      if(kpri.eq.2)go to 76
c.    c a l c u l a t e  e l e m e n t  s t i f f n e s s ,
c.    e l e m e n t  m a s s ,  a n d  e l e m e n t
c.    d a m p i n g
c.
      do 30 i=1,ndof
      do 30 j=1,ndof
      fc(i,j)=0.0d0
      fk(i,j)=0.0d0
   30 continue
   76 continue
      do 80 lx=1,nint
      ri=xg(lx,nint)
      do 80 ly=1,nint
      si=xg(ly,nint)
c
c     evaluate derivative operator b and the jacobian determinant det
c     evaluate the displacement interpolation matrix h
c
      call stom(xx,b,det,hh,ri,si,xbar,nel,itype)
c
c     add contribution to element stiffness and mass
c
      if (itype.gt.0)xbar=thic
      wt=wgt(lx,nint)*wgt(ly,nint)*xbar*det
      if(itype.eq.0)wt=2.0d0*pi*wt
      if(kpri.eq.2)go to 72
      do 70 j=1,ndof
      do 40 k=1,nstr
      dtilb(k)=0.0d0
      db(k)=0.0d0
      do 40 l=1,nstr
      dtilb(k)=dtilb(k) + dtil(k,l)*b(l,j)
  40  db(k)=db(k)+d(k,l)*b(l,j)
      do 60 i=j,ndof
      damp=0.0d0
      stiff=0.0d0
      do 50 l=1,nstr
      damp=damp + b(l,i)*dtilb(l)
  50  stiff=stiff+b(l,i)*db(l)
      fc(i,j)=fc(i,j) + wt*(damp + fnu1*stiff)
  60  fk(i,j)=fk(i,j)+stiff*wt
  70  continue
      if(kpri.ne.2)go to 80
   72 continue
      do 27 j=1,ndof
      do 27 i=j,ndof
      fmass=0.0d0
      do 17 l=1,2
   17 fmass=fmass + hh(l,i)*hh(l,j)
   27 fm(i,j)=fm(i,j) + fmass*rho*wt
  80  continue
c
      do 90 j=1,ndof
      do 90 i=j,ndof
      fc(j,i)=fc(i,j)
      fm(j,i)=fm(i,j)
  90  fk(j,i)=fk(i,j)
      return
  290 continue
c.
c. c a l c u l a t i o n  o f  t h e   l u m p e d  m a s s  m a t r i x
c.
      area=((xx(1,2)-xx(1,3))*(xx(2,2)-xx(2,1)) + (xx(1,2)-xx(1,1))*
     1(xx(2,3)-xx(2,2)) + (xx(1,4)-xx(1,1))*(xx(2,4)-xx(2,3)) +
     2(xx(1,4)-xx(1,3))*(xx(2,1)-xx(2,4)))/2.0d0
      w=rho*thic*area
      ravg=(xx(1,1)+xx(1,2)+xx(1,3)+xx(1,4))/4.0d0
      if(itype.eq.0)w=2.0d0*pi*ravg*area*rho
      do 315 i=1,8
  315 fm(i,i)=w/4.0d0
      return
  260 continue
c.
c. c a l c u l a t i o n  o f  t h e  s t r e s s  t r a n s f o r m -
c. a t i o n  m a t r i x
c.
      do 411 i=1,nstr
      do 411 j=1,ndof
  411 st(i,j)=0.0d0
      ri=0.0d0
      si=0.0d0
      call stom(xx,b,det,hh,ri,si,xbar,nel,itype)
      do 413 i=1,nstr
      do 413 j=1,ndof
      do 413 l=1,nstr
  413 st(i,j)=st(i,j) + d(i,l)*b(l,j)
      return
c
      end
*deck stom
      subroutine stom(xx,b,det,hh,r,s,xbar,nel,itype)
      implicit real*8 (a-h,o-z)
c
c.................................................................
c.
c.    p r o g r a m
c.        to evaluate the strain-displacement transformation matrix
c.        b at point (r,s) for a quadrilateral element
c.
c..................................................................
      dimension xx(2,4),hh(2,8),b(4,8),h(4),p(2,4),xj(2,2),xji(2,2)
c
      rp= 1.0d0 + r
      sp= 1.0d0 + s
      rm= 1.0d0 - r
      sm= 1.0d0 - s
c
c     interpolation functions
c
      h(1)= .25d0*rp*sp
      h(2)= .25d0*rm*sp
      h(3)= .25d0*rm*sm
      h(4)= .25d0*rp*sm
c
c     displacement interpolation matrix for calculation of mass matrix
c
      do 45 i=1,2
      do 45 j=1,8
   45 hh(i,j)=0.0d0
      do 35 j=1,4
      hh(1,2*j-1)=h(j)
   35 hh(2,2*j)=h(j)
c
c     natural coordinate derivatives of the interpolation functions
c
c         1. with respect to r
c
      p(1,1)= .25d0*sp
      p(1,2)= -p(1,1)
      p(1,3)= -.25d0*sm
      p(1,4)= -p(1,3)
c
c         2. with respect to s
c
      p(2,1)= .25d0*rp
      p(2,2)= .25d0*rm
      p(2,3)= -p(2,2)
      p(2,4)= -p(2,1)
c
c     evaluate the jacobian matrix at the point (r,s)
c
  10  do 30 i=1,2
      do 30 j=1,2
      dum=0.0d0
      do 20 k=1,4
  20  dum=dum+p(i,k)*xx(j,k)
  30  xj(i,j)=dum
c
c     compute the determinant of the jacobian matrix at point (r,s)
c
      det=xj(1,1)*xj(2,2)-xj(2,1)*xj(1,2)
      if (det.gt.0.00000001d0)go to 40
      write (6,2000)nel
      stop
c
c     compute inverse jacobian matrix
c
  40  dum=1./det
      xji(1,1)=xj(2,2)*dum
      xji(1,2)=-xj(1,2)*dum
      xji(2,1)=-xj(2,1)*dum
      xji(2,2)= xj(1,1)*dum
c
c     evaluate global deivative operator b
c
      k2=0
      do 60 k=1,4
      k2=k2+2
      b(1,k2-1)=0.d0
      b(1,k2)=0.d0
      b(2,k2-1)=0.d0
      b(2,k2)=0.d0
      do 50 i=1,2
      b(1,k2-1)=b(1,k2-1)+ xji(1,i)*p(i,k)
  50  b(2,k2  )=b(2,k2  )+ xji(2,i)*p(i,k)
      b(3,k2  )=b(1,k2-1)
  60  b(3,k2-1)=b(2,k2  )
c
c     in case of plane strain or plane stress analysis do not include
c     the normal strain component
c
      if(itype.gt.0)return
c
c     compute the radius at point (r,s)
c
      xbar=0.0d0
      do 70 k=1,4
  70  xbar=xbar+h(k)*xx(1,k)
c
c     evaluate the hoop strain-displacement relation
c
      if(xbar.gt.0.00000001d0)go to 90
c
c     for the case of zero radius equate radial to hoop strain
c
      do 80 k=1,8
  80  b(4,k)=b(1,k)
      return
c
c     non zero radius
c
  90  dum=1./xbar
      k2=0
      do 100 k=1,4
      k2=k2+2
      b(4,k2)=0.d0
 100  b(4,k2-1)=h(k)*dum
      return
c
 2000 format (1h1,6herror>,/,1x,
     143hzero or negative jacobian det. for element ,i6)
c
c
      end
*deck tapein
      subroutine tapein
      implicit real*8 (a-h,o-z)
      return
      end
*deck user
      subroutine user
      implicit real*8 (a-h,o-z)
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/dtwo/itwo,ndof,idim
      common/tapcom/nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlin
      common a(1)
      real a
      if(ind.gt.1)return
c
c        data between nfirst and nlast will be
c        restored every time this routine is called
c
      nd=1
      nfirst=n6e+4*itwo
      n101=nfirst
      n102=n101+nume*itwo
      n103=n102+nume*nd
      nlast=n103
      midest=nlast-(nfirst-1)
      min=nmax+nlast-n1l+1
      if(ind.eq.0)write(6,2020)min
      if(min.le.nmax)go to 10
      write(6,2010)nlast
      stop
   10 continue
      call userp(a(n2l),a(n1l),a(n101),a(n102),nume,ndof,nd)
      if(ind.eq.0)call ewrite(a(n6e),ng)
      return
 2010 format(1h0,10x,20herror nmax less than  ,i10)
 2020 format(1h0,10x,41hstorage required to input element data = ,i10)
      end
*deck userp
      subroutine userp(id,mht,edat,lm,nume,ndof,nd)
      implicit real*8 (a-h,o-z)
      dimension id(ndof,1),mht(1),edat(1),lm(nd,1)
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/comtyp/itypee,ngroup,ngin,nlin
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/tapcom/nt11,nt31
      common/list/data(64),ndum
      common a(1)
      real a
      dimension ss(144)
      if(ind.ne.0)go to 20
c
c        read in and store element data
c
      call fld
      read(nt11)data
c
c        check column heights
c
      call colht(mht,nd,lm(1,i))
   10 continue
      return
   20 if(ind.ne.1)go to 80
c
c        form stiffness
c
      ktyp=kpri-2
      do 70 n=1,nume
c
c        check to see if the element  is in the current block
c
      call echeck(lm(1,n),nd,icode)
      if(icode.eq.1)go to 70
      if(ktyp)30,40,50
   30 continue
c
c        stiffness part of the total stiffness
c
      go to 60
   40 continue
c
c        mass part of total stiffness
c
      go to 60
   50 continue
c
c        damping part of total stiffness
c
   60 continue
c
c        the array ss should have the element stiffness in it
c
      call addban(a(n2),a(n1l),ss,lm(1,n),nd)
   70 continue
      return
   80 if(ind.ne.4)return
      do 100 n=1,nume
      call echeck(lm(1,n),nd,icode)
      if(icode)100,90,100
   90 continue
      ngin=1
      return
  100 continue
      return
      end
