*deck ashl
      subroutine ashl
      implicit real*8 (a-h,o-z)
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/dtwo/itwo,ndof,idim
      common/tapcom/nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlen
c
      common a(1)
      real a
c     here ncoeff=10 is used.ei.ten fixed point variables are allowed
c     for computations.first six have fixed meaning.last four are dummy
c     and can be used for other purposes.  **  1:thic=thickness
c     2: e=real part of young mod.=re(e1).   3:fnu1=im(e1)/re(e1).mult-
c     iplier when multiplied to re(e) gives imaginary comp. of young mod
c     4:pr=poissonn ratio.   5:rho=mass density/unit volume
c     6:fkshr=timo. shear factor.   7:fharm=harmonic number
c     8:ts=shear thickness     9:dummy     10:dummy variables
       ncoeff=10
c      nnlst=5  =total no. of integer variable switches which means
c      as follows.**** 1: =1 means consistent mass. =0 lumped mass
c      2: construct damping matrix.  =1  yes.   =0  no
c      3: stress recovery.   =1  yes.   =0  no
c      4:  dummy  switch
c      5:  dummy switch
       nnlst=5
c      nnodel= number of nodes per element.=2 for this element
       nnodel=2
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
   10 call
     . ashlp(a(n2),a(n2l),a(n1l),a(n2),a(n3),a(n101),a(n102),a(n103),
     .a(n104),a(n105),nume,nummat,ndof,ind,kpri,nt11,ncoeff,nn5,nnsz
     .,nfrc,nstr,nptsf,idim,nnlst,numnp,nnodel)
      if(ind.eq.0)call ewrite(a(n6e),ng)
       if(ind.eq.0) write(6,40) nume,nummat,ndof,ind,kpri,nt11,ncoeff
     .,nn5,nnsz,nfrc,nstr,nptsf,idim,nnlst,numnp
  40   format(/10x,"nume,nummat,ndof,ind,kpri,nt11,ncoeff,nn5,nnsz,
     .,nfrc,nstr,nptsf,idim,nnlst,numnp"/10x,15i5)
   20 format (1h0,39htorage required to input element data =,i10)
   30 format (1h0,21herror nmax less than ,i10)
      return
      end
*deck ashlp
      subroutine ashlp(a,id,mht,x,y,fmat,xy,mat,lm,iprop,nume,
     .nummat,ndof,ind,kpri,nt11,ncoeff,nn5,nnsz,nfrc,nstr,nptsf,
     .idim,nnlst,numnp,nnodel)
      implicit real*8 (a-h,o-z)
      common/comtyp/itypee,ngroup,ngin,nlen
c
      common /list/ data(64),ndum
      common/switch/ipsm,ipkbar,ipinpt,ipreac,ieldat,ipsdm,ipcnpf
     . ,ibigw
c
      dimension x(1),y(1),fmat(9,1),mat(1),lm(12,1),stiff(78),xmass(78),
     . damp(78),fk(12,12),fc(12,12),fm(12,12),xy(2,2,1),
     . nonum(2),id(ndof,1),mht(1),iprop(nnlst,1),
     . ur(12),ui(12),forcer(16),forcei(16),sigi(12),sigr(12)
c     all the dimensions above should be equal to or bigger than
c     the dimensions given below interms of the variables.all
c     variable size are defined in the previous subroutine ashl
c     dimension x(1),y(1),fmat(ncoeff,1),mat(1),lm(nn5,1),stiff(nnsz),
c    .xmass(nnsz),damp(nnsz),ft(nfrc,nn5),fk(nn5,nn5),fc(nn5,nn5),
c    .fm(nn5,nn5),xy(idim,ndof,1),nonum(nnodel),id(ndof,1),mht(1),
c    .iprop(nnlst,1),c(15),d(10,10),ur(nn5),ui(nn5),forcer(nfrc),
c    .forcei(nfrc),sigr(nstr),sigi(nstr),st(nstr,nn5)
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
c      if(idim.gt.2) xy(3,i,n)=z(node)
  135   continue
      call colht(mht,nn5,lm(1,n))
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
      if(ind.eq.3) write(6,392) fmat(7,1)
  392 format(//20x,"following results are for harmonic no= ",f5.1)
      if (ind.eq.3)write(6,391)
      if (ind.eq.3)write(6,390)
  390 format(1x,34hcomplex stresses for ashl elements,
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
      if (ind.eq.1) call echeck(lm(1,n),nn5,icode)
      if (icode.eq.1) go to 280
      m=mat(n)
      ilumps=iprop(1,m)
      idamps=iprop(2,m)
      istres=iprop(3,m)
      nft=iprop(4,m)
c     following property inshel  is a dummy variable. not used
      inshel=iprop(5,m)
      if(ind.eq.3.and.istres.eq.0)go to 280
      if(kpri.eq.3.and.idamps.eq.0)go to 280
      thic=fmat(1,m)
       ts=fmat(8,m)
       if(ts.eq.0.) ts=thic
      e=fmat(2,m)
      pr=fmat(4,m)
      fnu1=fmat(3,m)
      rho=fmat(5,m)
      fkshr=fmat(6,m)
      fharm=fmat(7,m)
c     nstr=5
c     f=e*thic/(1.0-pr**2)
c     fn=pr*f
c     g=f*thic**2/12.0
c     gn=pr*g
c     h=e*thic*fkshr/(2.0*(1.0+pr))
c     c(1)=f
c     c(2)=fn
c     c(3)=0.0
c     c(4)=0.0
c     c(5)=0.0
c     c(6)=f
c     c(7)=0.0
c     c(8)=0.0
c     c(9)=0.0
c     c(10)=g
c     c(11)=gn
c     c(12)=0.0
c     c(13)=g
c     c(14)=0.0
c     c(15)=h
c
c         fill in full 5x5 stress strain matrix
c
c     ic=0                                                            c
c     do 212 i=1,5
c     do 212 j=i,5
c     ic=ic+1
c     d(i,j)=c(ic)
c 212 d(j,i)=c(ic)
      kprip=kpri
      if(ind.eq.3)kprip=4
       print 1,thic,ts,e,pr,fnu1,rho,fkshr,((xy(ij1,ij2,n),ij1=1,2),
     1 ij2=1,2)
  1    format(5x,5e15.5)
      call ashlst(thic,ts,e,pr,fnu1,rho,fkshr,xy,fk,fc,fm
     1,ur,fharm,kprip,sigr,forcer,n,ndof)
      if (ind.eq.3) go to 255
c
      m=0
      do 220 i=1,nn5
      do 220 j=i,nn5
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
      write(6,953)((fk(ii,jj),jj=1,nn5),ii=1,nn5)
  953 format(1x,1p6e11.3)
  702 if(kpri.ne.3)go to 703
      write(6,957)n
  957 format(1x,//////,1x,35h damping matrix for axsh element n=,i5//)
      write(6,953)((fc(ii,jj),jj=1,nn5),ii=1,nn5)
  703 if(kpri.ne.2)go to 950
      write(6,960)n
  960 format(1x,//////,32h mass matrix for axsh element n=,i5//)
      write(6,953)((fm(ii,jj),jj=1,nn5),ii=1,nn5)
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
c
c compute forces
c
      call ashlst(thic,ts,e,pr,fnu1,rho,fkshp,xy,fk,fc,fm,
     1 ur,fharm,kprip,sigr,forcer,n,ndof)
      call ashlst(thic,ts,e,pr,fnu1,rho,fkshp,xy,fk,fc,fm,
     1 ui,fharm,kprip,sigi,forcei,n,ndof)
c
c      print forces and moments
c
      int1=nfrc/2
      int2=int1+1
      write(6,362)n,(forcer(i),forcei(i),i=1,int1)
      write(6,363)n,(forcer(i),forcei(i),i=int2,nfrc)
c
c     compute stresses
c        multiply stress transformation matrix times displacement
c        vector
c
c     print stresses
c
       int1=nstr/2
       int2=int1+1
      write(6,362)n,(sigr(i),sigi(i),i=1,int1)
      write(6,363)n,(sigr(i),sigi(i),i=int2,nstr)
  362 format(1x,/,1x,i5,3x,5hnode1,1p8e11.3)
  363 format(1x,i5,3x,5hnode2,1p8e11.3)
  280 continue
      return
c
c         ind=3 and ind=4 passes
c
  300 if(ind.ne.4)return
      do 315 n=1,nume
      call echeck(lm(1,n),nn5,icode)
      if(icode)315,310,315
  310 continue
      ngin=1
      return
  315 continue
      return
      end
*deck ashlst
       subroutine ashlst(t,ts,e,nu,fnu1,rho,fkshr,xy,xk,xc,xm
     1,ug,fharm,krip,stres,forc,n,ndof)
       implicit real*8 (a-h,o-z)
       dimension xk(12,12),x(2),y(2),xy(2,2,1),xc(12,12),xm(12,12)
     1,stress(1),force(1)
      common/prop7/e11,e22,e33,e12,d11,d22,d33,d12,g11,g22,dprop(8)
     1,dmpzel(100)
       real*8 nu,lem1,lem2
c      read *,e,nu,t,ts,rho,x(1),y(1),x(2),y(2),iharm
c      e=young modulus
c      e*=e(1+i*fnu1)  e is rel part of young mod. & e*fnu1 is
c      imaginary component of young modulus. imaginary comp. is
c      used to calculate damping matrix. xc(i,j)=fnu1*xk(i,j)
c      krip=1:calculate xk matrix.  =2:xm matrix.  =3 xc matrix
c      krip=4 calculate stresses in shell
c      nu=poissons ratio
c      t=stretching thickness
c      ts=shear thickness
c      rho=mass density/unit volume
c      x(i),y(i),i=1,2 are x & y coord. of pts 1 & 2 ,resply.
c      fharm=harmonic number
c      *****************************************************
c      note that   t,ts and t**3/12 have been taken care of thru
c      material matrices [e] ,[g]  & [d]
c      *********************************************************
       lem1=e*nu/((1.+nu)*(1.-2.*nu))
       x(1)=xy(1,1,n)
       y(1)=xy(2,1,n)
       x(2)=xy(1,2,n)
       y(2)=xy(2,2,n)
       lem2=e/(2.*(1.+nu))
       c11=lem1+2.*lem2
       c22=c11
       c33=c11
       c12=lem1
       c44=lem2
       c13=c12
       c23=c12
       e11=t*(c11-c13**2/c33)
       e12=t*(c12-c13*c23/c33)
       e22=t*(c22-c23**2/c33)
       e33=t*c44
       tt=t**2/12.
       d11=e11*tt
       d12=e12*tt
       d22=e22*tt
       d33=e33*tt
       g11=0.5*fkshr*ts*e/(1.+nu)
       g22=g11
       print 25,e11,e22,e33,e12,d11,d22,d33,d12,g11,g22
  25   format(10x,"e11,e22,e33,e12,d11,d22,d33,d12,g11,g22 ashlst"/
     1 (4e20.8))
       print 10,fharm,t,x(1),y(1),x(2),y(2)
       if(krip.eq.1) call const(fharm,ndof,x,y,xk)
       if(krip.eq.2) call conms(fharm,ndof,x,y,t,xm,rho)
       if(krip.eq.3) call const(fharm,ndof,x,y,xc)
       if(krip.ne.3) go to 12
       do 14 i=1,12
       do 14 j=1,12
       xc(i,j)=xc(i,j)*fnu1
  14   continue
  12   continue
       print 10,fharm,t,x(1),y(1),x(2),y(2)
  10   format(10x,"harm=  ",f5.0,"  shell stiffness matrix. thick=  "
     1,f6.3,"  x & y coord. pts 1 &2 are  ",4f10.2)
       call matprn(xk,12,12)
       print 11,fharm,t,x(1),y(1),x(2),y(2)
  11   format(11x,"harm=  ",f5.0,"  shell mass matrix. thick=  "
     1,f6.3,"  x & y coord. pts 1 &2 are  ",4f10.2)
       call matprn(xm,12,12)
       if(krip.eq.4) call shforce(fharm,x,y,ug,thic,stress,force)
       return
       end
*deck integrl
       subroutine integrl(r1,r2,ri)
       implicit real*8 (a-h,o-z)
       dimension ri(25),x(2),r(2)
       x(1)=0.57735027
       x(2)=-0.57735027
c      following redefinition of x(1) & x(2) gives single pt gauss integ
       x(1)=0.
       x(2)=0.
       a=(r2-r1)/2.
       b=(r2+r1)/2.
       r(1)=a*x(1)+b
       r(2)=a*x(2)+b
       print 10, r1,r2,r(1),r(2),a,b
  10   format(//20x,"********************************"/6f10.3/
     1 20x," above are r1,r2,r(1),r(2),a,b  *************")
       do 1 i=1,25
  1    ri(i)=0.
       do 2 i=1,2
       ri(5)=ri(5)+(1.-x(i))**2/r(i)
       ri(4)=ri(4)+(1.-x(i)**2)/r(i)
       ri(18)=ri(18)+r(i)
       ri(7)=ri(7)+(1.-x(i))*(1.-x(i)**2)/r(i)
       ri(6)=ri(6)+(1.+x(i))**2/r(i)
       ri(8)=ri(8)+(1.+x(i))*(1.-x(i)**2)/r(i)
       ri(9)=ri(9)+(1.-x(i)**2)**2/r(i)
       ri(1)=ri(1)+1./r(i)
       ri(11)=ri(11)+(1.-x(i))/r(i)**2
       ri(10)=ri(10)+(1.+x(i))/r(i)**2
       ri(2)=ri(2)+(1.-x(i))/r(i)
       ri(3)=ri(3)+(1.+x(i))/r(i)
       ri(15)=ri(15)+(1.-x(i))**2/r(i)**3
       ri(16)=ri(16)+(1.-x(i)**2)/r(i)**3
       ri(13)=ri(13)+(1.-x(i))**2/r(i)**2
       ri(12)=ri(12)+(1.-x(i)**2)/r(i)**2
       ri(17)=ri(17)+(1.+x(i))**2/r(i)**3
       ri(14)=ri(14)+(1.+x(i))**2/r(i)**2
       ri(19)=ri(19)+(1.-x(i))**2*r(i)
       ri(20)=ri(20)+(1.-x(i)**2)*r(i)
       ri(21)=ri(21)+(1.+x(i))**2*r(i)
       ri(22)=ri(22)+(1.-x(i)**2)**2*r(i)
       ri(23)=ri(23)+(1.-x(i))*(1.-x(i)**2)*r(i)
       ri(24)=ri(24)+(1.+x(i))*(1.-x(i)**2)*r(i)
  2    continue
       print 20,(ri(i),i=1,24)
  20   format(20x,"ri"/(6f18.6))
       return
       end
*deck const
       subroutine const(fharm,ndof,x,y,xk)
       implicit real*8 (a-h,o-z)
       common/prop8/e11,e22,e33,e12,d11,d22,d33,d12,g11,g22
       dimension xk(12,12),xk1(10,10),xk2(10,10),xk3(10,10),x(2),
     1 y(2),ri(25),t(10,10),c(12,12),r(12,12)
       real*8 n,n2
       print 45, e11,e22,e33,e12,d11,d22,d33,d12,g11,g22
  45   format(10x,"e11,e22,e33,e12,d11,d22,d33,d12,g11,g22"/
     1 (4e20.8))
       pi=3.1415926
       n=fharm
       n2=n*n
       xx=x(2)-x(1)
       yy=y(2)-y(1)
       xl2=xx**2+yy**2
       xl=dsqrt(xl2)
       snp=xx/xl
       csp=yy/xl
       scp=csp*snp
       csp2=csp**2
       snp2=snp**2
       snpn=snp+n
       do 1 i=1,10
       do 1 j=1,10
       xk1(i,j)=0.
       xk2(i,j)=0.
       xk3(i,j)=0.
       xk(i,j)=0.
   1   continue
       call integrl(x(1),x(2),ri)
       xk1(1,1)=(n2*e22+snp2*e33)*ri(5)/4.+e33*ri(18)/xl2
     1+2.*e33*snp/xl
       xk1(1,2)=(n2*e22+snp2*e33)*ri(4)/4.-e33*ri(18)/xl2
       xk1(1,3)=n*(e33-e12)/xl+n*snp*(e33+e22)*ri(5)/4.
       xk1(1,4)=n*(e12+e33)/xl+n*snp*(e22+e33)*ri(4)/4.
       xk1(1,5)=n*e22*csp*ri(5)/4.
       xk1(1,6)=n*e22*csp*ri(4)/4.
       xk1(1,7)=n*e22*xl*csp*ri(7)/16.
       xk1(1,8)=-n*e22*xl*csp*ri(7)/16.
       xk1(2,2)=(n2*e22+e33*snp2)*ri(6)/4.+e33*ri(18)/xl2
     1-2.*e33*snp/xl
       xk1(2,3)=-n*(e12+e33)/xl+n*snp*(e22+e33)*ri(4)/4.
       xk1(2,4)=n*(e12-e33)/xl+n*snp*(e22+e33)*ri(6)/4.
       xk1(2,5)=n*e22*csp*ri(4)/4.
       xk1(2,6)=n*e22*csp*ri(6)/4.
       xk1(2,7)=n*e22*xl*csp*ri(8)/16.
       xk1(2,8)=-n*e22*xl*csp*ri(8)/16.
       xk1(3,3)=e11*ri(18)/xl2-2.*e12*snp/xl+
     1 (n2*e33+e22*snp2)*ri(5)/4.
       xk1(3,4)=-e11*ri(18)/xl2+(n2*e33+e22*snp2)*ri(4)/4.
       xk1(3,5)=-e12*csp/xl+e22*scp*ri(5)/4.
       xk1(3,6)=-e12*csp/xl+e22*scp*ri(4)/4.
       xk1(3,7)=-e12*csp/6.+e22*scp*xl*ri(7)/16.
       xk1(3,8)=e12*csp/6.-e22*scp*xl*ri(7)/16.
       xk1(4,4)=2.*e12*snp/xl+e11*ri(18)/xl2
     1 +(n2*e33+e22*snp2)*ri(6)/4.
       xk1(4,5)=e12*csp/xl+e22*scp*ri(4)/4.
       xk1(4,6)=e12*csp/xl+e22*scp*ri(6)/4.
       xk1(4,7)=e12*csp/6.+e22*scp*xl*ri(8)/16.
       xk1(4,8)=-e12*csp/6.-e22*scp*xl*ri(8)/16.
       xk1(5,5)=e22*csp2*ri(5)/4.
       xk1(5,6)=e22*csp2*ri(4)/4.
       xk1(5,7)=e22*csp2*xl*ri(7)/16.
       xk1(5,8)=-xk1(5,7)
       xk1(6,6)=e22*csp2*ri(6)/4.
       xk1(6,7)=e22*csp2*xl*ri(8)/16.
       xk1(6,8)=-xk1(6,7)
       xk1(7,7)=e22*csp2*xl2*ri(9)/64.
       xk1(7,8)=-xk1(7,7)
       xk1(8,8)=xk1(7,7)
       xk2(1,1)=d33*ri(1)*(csp/(2.*xl))**2
       xk2(1,2)=-xk2(1,1)
       xk2(1,3)=-d33*csp2*snpn*ri(11)/(8.*xl)
       xk2(1,4)=-d33*csp2*snpn*ri(10)/(8.*xl)
       xk2(1,7)=-d33*n*csp*ri(2)/(4.*xl)
       xk2(1,8)=-d33*n*csp*ri(3)/(4.*xl)
       xk2(1,9)=d33*(csp/xl2+csp*snp*ri(2)/(4.*xl))
       xk2(1,10)=d33*(-csp/xl2+csp*snp*ri(3)/(4.*xl))
       xk2(2,2)=xk2(1,1)
       do 2 j=3,10
  2    xk2(2,j)=-xk2(1,j)
       xk2(3,3)=d33*ri(15)*(csp*snpn/4.)**2
       xk2(3,4)=d33*ri(16)*(csp*snpn/4.)**2
       xk2(3,7)=d33*ri(13)*n*csp*snpn/8.
       xk2(3,8)=d33*ri(12)*n*csp*snpn/8.
       xk2(3,9)=-d33*(ri(2)*csp*snpn/(4.*xl)
     1 +ri(13)*snp*csp*snpn/8.)
       xk2(3,10)=d33*(ri(2)*csp*snpn/(4.*xl)
     1 -ri(12)*snp*csp*snpn/8.)
       xk2(4,4)=d33*ri(17)*(csp*snpn/4.)**2
       xk2(4,7)=d33*ri(12)*n*csp*snpn/8.
       xk2(4,8)=d33*ri(14)*n*csp*snpn/8.
       xk2(4,9)=-d33*(ri(3)*csp*snpn/(4.*xl)
     1 +ri(12)*csp*snp*snpn/8.)
       xk2(4,10)=d33*(ri(3)*csp*snpn/(4.*xl)
     1 -ri(14)*csp*snp*snpn/8.)
       xk2(7,7)=d11*ri(18)/xl2-2.*d12*snp/xl
     1 +(d22*snp*snp+d33*n*n)*ri(5)/4.
       xk2(7,8)=-d11*ri(18)/xl2+(d22*snp*snp/4.
     1 +n*n*d33/4.)*ri(4)
       xk2(7,9)=n*(d12-d33)/xl-(d22+d33)*n*snp*ri(5)/4.
       xk2(7,10)=n*(d12+d33)/xl-n*snp*(d22+d33)*ri(4)/4.
       xk2(8,8)=d11*ri(18)/xl2+2.*d12*snp/xl
     1 +(d22*snp*snp+n*n*d33)*ri(6)/4.
       xk2(8,9)=-n*(d12+d33)/xl-n*snp*(d22+d33)*ri(4)/4.
       xk2(8,10)=n*(d33-d12)/xl-n*snp*(d22+d33)*ri(6)/4.
       xk2(9,9)=(n*n*d22+d33*snp*snp)*ri(5)/4.
     1 +2.*d33*snp/xl+d33*ri(18)/xl2
       xk2(9,10)=(n*n*d22+d33*snp*snp)*ri(4)/4.-d33*ri(18)/xl2
       xk2(10,10)=(n*n*d22+d33*snp*snp)*ri(6)/4.-d33*snp/xl
     1 +d33*ri(18)/xl2
       xk3(1,1)=g22*csp2*ri(5)/4.
       xk3(1,2)=g22*csp2*ri(4)/4.
       xk3(1,5)=n*g22*csp*ri(5)/4.
       xk3(1,6)=n*g22*csp*ri(4)/4.
       xk3(1,7)=n*xl*g22*csp*ri(7)/16.
       xk3(1,8)=-xk3(1,7)
       xk3(1,9)=-2.*g22*csp/3.
       xk3(1,10)=-g22*csp/3.
       xk3(2,2)=g22*csp2*ri(6)/4.
       xk3(2,5)=n*g22*csp*ri(4)/4.
       xk3(2,6)=n*g22*csp*ri(6)/4.
       xk3(2,7)=n*xl*g22*csp*ri(8)/16.
       xk3(2,8)=-n*xl*g22*csp*ri(8)/16.
       xk3(2,9)=-g22*csp/3.
       xk3(2,10)=-2.*g22*csp/3.
       xk3(5,5)=g11*ri(18)/xl2+n*n*g22*ri(5)/4.
       xk3(5,6)=-g11*ri(18)/xl2+n*n*g22*ri(4)/4.
       xk3(5,7)=g11*ri(18)/(2.*xl)+n*n*xl*g22*ri(7)/16.
       xk3(5,8)=g11*ri(18)/(2.*xl)-n*n*xl*g22*ri(7)/16.
       xk3(5,9)=-2.*n*g22/3.
       xk3(5,10)=-n*g22/3.

       xk3(6,6)=g11*ri(18)/xl2+n*n*g22*ri(6)/4.
       xk3(6,7)=-g11*ri(18)/(2.*xl)+n*n*xl*g22*ri(8)/16.
       xk3(6,8)=-g11*ri(18)/(2.*xl)-n*n*xl*g22*ri(8)/16.
       xk3(6,9)=-n*g22/3.
       xk3(6,10)=-2.*n*g22/3.
       xk3(7,7)=g11*ri(18)/4.+(n*xl/8.)**2*g22*ri(9)
       xk3(7,8)=g11*ri(18)/4.-(n*xl/8.)**2*g22*ri(9)
       xk3(7,9)=-n*xl*g22/12.
       xk3(7,10)=-n*xl*g22/12.
       xk3(8,8)=g11*ri(18)/4.+(n*xl/8.)**2*g22*ri(5)
       xk3(8,9)=n*xl*g22/12.
       xk3(8,10)=n*xl*g22/12.
       xk3(9,9)=g22*ri(19)/4.
       xk3(9,10)=g22*ri(20)/4.
       xk3(10,10)=g22*ri(21)/4.
       do 19 i=1,10
       do 19 j=i,10
       xk1(j,i)=xk1(i,j)
       xk2(j,i)=xk2(i,j)
  19   xk3(j,i)=xk3(i,j)
c      element stiffness is multiplied by 2 only when fharm=0
       xlhar=pi*xl/2.
c      factor  pi=3.1415926 is accounted for here.
c      factor  xl/2. is taken care of here because integrals ri
c      did not account for them
       if(fharm.eq.0) xlhar=xlhar*2.
       do 3 i=1,10
       do 3 j=1,10
       xk(i,j)=(xk1(i,j)+xk2(i,j)+xk3(i,j))*xlhar
  3    continue
       print 18
       call matprn(xk1,10,10)
       print 18
       call matprn(xk2,10,10)
       print 18
       call matprn(xk3,10,10)
       print 18
  18   format( 20x,"  ******   element stiffness matrix   ******")
       call matprn(xk,12,12)
       do 4 i=1,10
       do 4 j=1,10
       t(i,j)=0.
   4   continue
       t(1,1)=1.
       t(2,6)=1.
       t(3,2)=1.
       t(4,7)=1.
       t(5,3)=1.
       t(6,8)=1.
       t(7,4)=1.
       t(8,9)=1.
       t(9,5)=1.
       t(10,10)=1.
       print 21
  21   format(20x,"t-matrix printed")
       call matprn(t,10,10)
       call matmul(xk,10,10,12,12,t,10,10,10,10,c,12,12,0)
       call matmul(t,10,10,10,10,c,10,10,12,12,xk,12,12,1)
       print 22
  22   format(20x,"matrix  t-tran*k*t  printed")
       call matprn(xk,12,12)
       do 5 i=1,12
       do 5 j=1,12
  5    r(i,j)=0.
       r(1,1)=1.
       r(2,2)=csp
       r(2,3)=snp
       r(3,2)=-snp
       r(3,3)=csp
       r(4,4)=1.
       r(5,5)=csp
       r(5,6)=snp
       if(ndof.eq.5) r(5,6)=0.
       jj=6
       if(ndof.eq.5) jj=5
       do 6 i=1,5
       do 6 j=1,jj
       i1=i+5
       j1=j+6
       if(ndof.eq.5) j1=j+5
       r(i1,j1)=r(i,j)
  6    continue
       print 23
  23   format(20x,"r-matrix  printed")
       call matprn(r,12,12)
       if(ndof.eq.5) go to 25
       call matmul(xk,10,10,12,12,r,10,12,12,12,c,12,12,0)
       call matmul(r,10,12,12,12,c,10,12,12,12,xk,12,12,1)
       go to 26

   25  call matmul(xk,10,10,12,12,r,10,10,12,12,c,12,12,0)
       call matmul(r,10,10,12,12,c,10,10,12,12,xk,12,12,1)
   26  print 24
  24   format(20x,"r-tran*t-tran*k*t*r  =global stiffness matrix")
       call matprn(xk,12,12)
       return
       end
*deck matmul
       subroutine matmul(a,ia,ja,la,ma,b,ib,jb,lb,mb,c,lc,mc,iop)
       implicit real*8 (a-h,o-z)
       dimension a(la,ma),b(lb,mb),c(lc,mc)
c      if iop=0 :   a*b=c  is calculated
c      if iop=1 :   a-transpose*b=c  is calculated
c      if iop=2 :   a*b-transpose=c  is calculated
       if(iop.ne.0) go to 10
       if(ja.ne.ib) print 2
       if(ja.ne.ib) go to 100
       go to 40
  10   if(iop.ne.1) go to 20
       if(ia.ne.ib) print 2
       if(ia.ne.ib) go to 100
       go to 40
  20   if(ja.ne.jb) print 2
       if(ja.ne.jb) go to 100
  40   continue
  2    format(10x,"matrix multiplication of incompatible matrices")
       if(iop.eq.0) i1=ia
       if(iop.eq.0) j1=jb
       if(iop.eq.0) k1=ja
       if(iop.eq.1) i1=ja
       if(iop.eq.1) j1=jb
       if(iop.eq.1) k1=ia
       if(iop.eq.2) i1=ia
       if(iop.eq.2) j1=ib
       if(iop.eq.2) k1=ja
       do 1 i=1,i1
       do 1 j=1,j1
       c(i,j)=0.
       do 1 k=1,k1
       if(iop.eq.0) c(i,j)=c(i,j)+a(i,k)*b(k,j)
       if(iop.eq.1) c(i,j)=c(i,j)+a(k,i)*b(k,j)
       if(iop.eq.2) c(i,j)=c(i,j)+a(i,k)*b(j,k)
   1   continue
       return
  100  end
*deck matprn
       subroutine matprn(a,ia,ib)
       implicit real*8 (a-h,o-z)
       dimension a(ia,ib)
       do 1 i=1,ia
       print 10,(a(i,j),j=1,ib)
  1    continue
  10   format(5x,12(e9.3,"*"))
       return
       end
*deck conms
       subroutine conms(fharm,ndof,x,y,thick,xm,rho)
       implicit real*8 (a-h,o-z)
       real*8 n
       dimension xm(12,12),x(2),y(2),ri(25),t(10,10),c(12,12),r(12,12)
       n=fharm
       xx=x(2)-x(1)
       yy=y(2)-y(1)
       xl2=xx*xx+yy*yy
       xl=dsqrt(xl2)
       csp=yy/xl
       snp=xx/xl
       xl3=xl2*xl
       do 1 i=1,12
       do 1 j=1,12
  1    xm(i,j)=0.
       call integrl(x(1),x(2),ri)
       hl=thick*xl
       hl2=thick*xl2
       hl3=thick*xl3
       h2l=thick**2*xl
       h3l=thick**3*xl
       xm(1,1)=hl*ri(19)/8.
       xm(1,2)=hl*ri(20)/8.
       xm(1,9)=h3l*csp/36.
       xm(1,10)=h3l*csp/72.
       xm(2,2)=xl*ri(21)/8.
       xm(2,9)=h3l*csp/72.
       xm(2,10)=h3l*csp/36.
       xm(3,3)=hl*ri(19)/8.
       xm(3,4)=hl*ri(20)/8.
       xm(3,7)=h3l*csp/36.
       xm(3,8)=h3l*csp/72.
       xm(4,4)=hl*ri(21)/8.
       xm(4,7)=h3l*csp/72.
       xm(4,8)=h3l*csp/36.
       xm(5,5)=hl*ri(19)/8.
       xm(5,6)=hl*ri(20)/8.
       xm(5,7)=hl2*ri(23)/32.
       xm(5,8)=-hl2*ri(23)/32.
       xm(6,6)=hl*ri(21)/8.
       xm(6,7)=hl2*ri(24)/32.
       xm(6,8)=-hl2*ri(24)/32.
       xm(7,7)=hl3*ri(22)/128.+h3l*ri(19)/96.
       xm(7,8)=-hl3*ri(22)/128.+h3l*ri(20)/96.
       xm(8,8)=hl3*ri(22)/128.+h3l*ri(21)/96.
       xm(9,9)=h3l*ri(19)/96.
       xm(9,10)=h3l*ri(20)/96.
       xm(10,10)=h3l*ri(21)/96.
       do 2 i=1,12
       do 2 j=i,12
  2    xm(j,i)=xm(i,j)
       pi=3.1415926
       delt=rho*pi/2.
       if(fharm.eq.0) delt=2.*delt
       do 3 i=1,12
       do 3 j=1,12
c      note multiplication by 2 is required because original computa-
c      tions left it out
  3    xm(i,j)=2.*delt*xm(i,j)
       call matprn(xm,12,12)
       do 4 i=1,10
       do 4 j=1,10
       t(i,j)=0.
   4   continue
       t(1,1)=1.
       t(2,6)=1.
       t(3,2)=1.
       t(4,7)=1.
       t(5,3)=1.
       t(6,8)=1.
       t(7,4)=1.
       t(8,9)=1.
       t(9,5)=1.
       t(10,10)=1.
       print 21
  21   format(20x,"t-matrix printed")
       call matprn(t,10,10)
       call matmul(xm,10,10,12,12,t,10,10,10,10,c,12,12,0)
       call matmul(t,10,10,10,10,c,10,10,12,12,xm,12,12,1)
       print 22
  22   format(20x,"matrix  t-tran*m*t  printed")
       call matprn(xm,12,12)
       do 5 i=1,12
       do 5 j=1,12
  5    r(i,j)=0.
       r(1,1)=1.
       r(2,2)=csp
       r(2,3)=snp
       r(3,2)=-snp
       r(3,3)=csp
       r(4,4)=1.
       r(5,5)=csp
       r(5,6)=snp
       if(ndof.eq.5) r(5,6)=0.
       jj=6
       if(ndof.eq.5) jj=5
       do 6 i=1,5
       do 6 j=1,jj
       i1=i+5
       j1=j+6
       if(ndof.eq.5) j1=j+5
       r(i1,j1)=r(i,j)
  6    continue
       print 23
  23   format(20x,"r-matrix  printed")
       call matprn(r,12,12)
       if(ndof.eq.5) go to 25
       call matmul(xm,10,10,12,12,r,10,12,12,12,c,12,12,0)
       call matmul(r,10,12,12,12,c,10,12,12,12,xm,12,12,1)
       go to 26
  25   call matmul(xm,10,10,12,12,r,10,10,12,12,c,12,12,0)
       call matmul(r,10,10,12,12,c,10,10,12,12,xm,12,12,1)
   26  print 24
  24   format(20x,"r-tran*t-tran*m*t*r  =global mass matrix")
       call matprn(xm,12,12)
       return
       end
*deck shforce
       subroutine shforce(fharm,x,y,ug,thick,stress,forc)
       implicit real*8 (a-h,o-z)
       dimension tr(10,12),x(2),y(2),eft(3,10),c(3,12),d(3,12),
     1 e(2,12),force(3),moment(3),shear(2),ug(1),stress(1)
     2 ,dxt(3,12),ght(2,12),forc(1)
       common/prop8/e11,e22,e33,e12,d11,d22,d33,d12,g11,g22
       real*8 n,moment
       n=fharm
       xx=x(2)-x(1)
       yy=y(2)-y(1)
       xl2=xx*xx+yy*yy
       xl=dsqrt(xl2)
       snp=xx/xl
       csp=yy/xl
       tr(1,1)=1.
       tr(2,7)=1.
       tr(3,2)=csp
       tr(3,3)=snp
       tr(4,8)=csp
       tr(4,9)=snp
       tr(5,2)=-snp
       tr(5,3)=csp
       tr(6,8)=-snp
       tr(6,9)=csp
       tr(7,4)=1.
       tr(8,10)=1.
       tr(9,5)=csp
       tr(9,6)=snp
       tr(10,11)=csp
       tr(10,12)=snp
       do 1 i=1,3
       do 1 j=1,12
       eft(i,j)=0.
       dxt(i,j)=0.
       if(i.lt.3) ght(i,j)=0.
  1    continue
       do 2 i=1,2
       xs=-1.
       if(i.eq.2) xs=1.
       xkm=1.-xs
       xkp=1.+xs
       r2n=n/(2.*y(i))
       r2sn=snp/(2.*y(i))
       r2cs=csp/(2.*y(i))
       eft(1,1)=e12*r2n*xkm
       eft(2,1)=e22*r2n*xkm
       eft(3,1)=-e33*(1./xl+r2sn*xkm)
       eft(2,1)=e12*r2n*xkp
       eft(2,2)=e22*r2n*xkp
       eft(2,3)=e33*(1./xl-r2sn*xkp)
       eft(3,1)=-e11/xl+e12*r2sn*xkm
       eft(3,2)=-e12/xl+e22*r2sn*xkm
       eft(3,3)=-e33*r2n*xkm
       eft(4,1)=e11/xl+e12*r2sn*xkp
       eft(4,2)=e12/xl+e22*r2sn*xkp
       eft(4,3)=-e33*r2n*xkp
       eft(5,1)=e12*r2cs*xkm
       eft(5,2)=e22*r2cs*xkm
       eft(6,1)=e12*r2cs*xkp
       eft(6,2)=e22*r2cs*xkp
       call matmul(eft,3,12,3,12,tr,10,12,10,12,c,3,12,0)
       do 3 k=1,3
       force(k)=0.
       do 3 j=1,12
       force(k)=force(k)+c(k,j)*ug(j)
  3    continue
       dxt(1,3)=d33*r2cs/xl
       dxt(2,3)=-d33*r2cs/xl
       dxt(3,3)=-d33*r2cs*(snp+n)*xkm/(2.*y(i))
       dxt(4,3)=-d33*r2cs*(snp+n)*xkp/(2.*y(i))
       dxt(7,1)=-d11/xl+d12*r2sn*xkm
       dxt(7,2)=-d12/xl+d22*r2sn*xkm
       dxt(7,3)=-r2n*d33*xkm
       dxt(8,1)=d11/xl+d12*r2sn*xkp
       dxt(8,2)=d12/xl+d22*r2sn*xkp
       dxt(8,3)=-d33*r2n*xkp
       dxt(9,1)=-d12*r2n*xkm
       dxt(9,2)=-d22*r2n*xkm
       dxt(9,3)=d33*(1./xl+r2sn*xkm)
       dxt(10,1)=-d12*r2n*xkp
       dxt(10,2)=-d22*r2n*xkp
       dxt(10,3)=d33*(-1./xl+r2sn*xkp)
       call matmul(dxt,3,10,3,10,tr,10,12,10,12,d,3,12,0)
       do 4 k=1,3
       moment(k)=0.
       do 4 j=1,12
       moment(k)=moment(k)+d(k,j)*ug(j)
  4    continue
       ght(1,2)=-g22*r2cs*xkm
       ght(2,2)=-g22*r2cs*xkp
       ght(5,1)=-g11/xl
       ght(5,2)=-g22*r2n*xkm
       ght(6,1)=g11/xl
       ght(6,2)=-g22*r2n*xkp
       ght(7,1)=-g11/2.
       ght(8,1)=-g11/2.
       ght(9,2)=g22*xkm/2.
       ght(10,2)=g22*xkp/2.
       call matmul(ght,2,10,2,10,tr,10,12,10,12,e,2,12,0)
       do 5 k=1,2
       shear(k)=0.
       do 5 j=1,12
       shear(k)=shear(k)+e(k,j)*ug(j)
  5    continue
       do 6 ii=1,8
       iii=ii+i-1
       if(ii.le.3) forc(iii)=force(ii)
       if(ii.gt.3.and.ii.le.6) forc(iii)=moment(ii-3)
       if(ii.gt.6) forc(iii)=shear(ii-6)
  6    continue
       print 10,fharm,i,(force(k1),k1=1,3),(moment(k2),k2=1,3),
     1 (shear(k3),k3=1,2)
  10   format(1h1 /// 10x,"for harmonic=   ",f5.0," forces at end=  ",
     1 i5,"  are given in local coordinates.  "/
     2 10x,"forces along generator,circumference & inplane shear",
     3 5x,"moments about generator,circumference & twist",/,10x,
     4 "transverse shears along generator & circumference are"//
     5 5x,8e16.8)
       tt=thick**2/6.
       i1=(i-1)*6
       stress(i1+1)=force(1)/thick+moment(1)/tt
       stress(i1+2)=force(2)/thick+moment(2)/tt
       stress(i1+3)=moment(3)/tt
       stress(i1+4)=force(1)/thick-moment(1)/tt
       stress(i1+5)=force(2)/thick-moment(2)/tt
       stress(i1+6)=moment(3)/tt
       print 11,(stress(i1+k1),k1=1,6)
  11   format(10x,"stress along generator,circumference & shear"
     1 ,2x,"stress due to twist on top & bottom fibres are  "
     2 //10x,6e16.6)
  2    continue
       return
       end
*deck astf
      subroutine astf
      implicit real*8 (a-h,o-z)
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/dtwo/itwo,ndof,idim
      common/tapcom/nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlen
c
      common a(1)
      real a
c     ncoeff=13 is used.ie.thirteen fixed point variables are used for
c     computations.
c     1: af=x-sectional area of stiffener
c     2:e= real part of young modulus
c     3:fnu1=this fnu1 factor multiplied by above "e" yield
c            imaginary part of young modulus
c     4:pr=poissons ratio
c     5:rho=mass density per unit volume
c     6:fkshr=timo. shear factor
c     7:fharm=harmonic number
c     8:ixx=m.i. for bending in plane of stiffener
c     9:izz=m.i. for bending in plane normal to stiffener plane.
c     10:ec= distance of centroid of stiffener from reference surface
c            poitive sign of ec means centroidal circle of stiffener
c            is within reference circle.ie.stiffener is within shell
c            negative sign of ec means stiffener is outside of shell
c     11:ct=torsional coeff. similar to "j" of circular section
c     12,13 :y1,y2=distance along radial direction where stresses
c                  in circumfetential direction are to be computed
c                  this distance is with reference to centroid
       ncoeff=13
c      nnlst=5  =total no. of integer variable switches which means
c      as follows.**** 1: =1 means consistent mass. =0 lumped mass
c      2: construct damping matrix.  =1  yes.   =0  no
c      3: stress recovery.   =1  yes.   =0  no
c      4: nft=total number of elemental forces.
c      5:  dummy switch
       nnlst=5
c      nnodel=number of nodes per element. =1 for this element
       nnodel=1
c      varible nn3=no of node pts/ele*dim of coord( 2 for 2-d,3 for 3-d)
       nn3=nnodel*idim
c      variable nn5=no of node  pts per element*no of dof/node
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
       nfrc1=2
       nstr1=2
       nptsf=1
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
   10 call
     . astfp(a(n2),a(n2l),a(n1l),a(n2),a(n3),a(n101),a(n102),a(n103),
     .a(n104),a(n105),nume,nummat,ndof,ind,kpri,nt11,ncoeff,nn5,nnsz
     .,nfrc,nstr,nptsf,idim,nnlst,numnp,nnodel)
      if(ind.eq.0)call ewrite(a(n6e),ng)
       if(ind.eq.0) write(6,40) nume,nummat,ndof,ind,kpri,nt11,ncoeff
     .,nn5,nnsz,nfrc,nstr,nptsf,idim,nnlst,numnp
  40   format(/10x,"nume,nummat,ndof,ind,kpri,nt11,ncoeff,nn5,nnsz,
     .,nfrc,nstr,nptsf,idim,nnlst,numnp"/10x,15i5)
   20 format (1h0,39htorage required to input element data =,i10)
   30 format (1h0,21herror nmax less than ,i10)
      return
      end
*deck astfp
      subroutine astfp(a,id,mht,x,y,fmat,xy,mat,lm,iprop,nume,
     .nummat,ndof,ind,kpri,nt11,ncoeff,nn5,nnsz,nfrc,nstr,nptsf,
     .idim,nnlst,numnp,nnodel)
      implicit real*8 (a-h,o-z)
      common/comtyp/itypee,ngroup,ngin,nlen
c
      common /list/ data(64),ndum
      common/switch/ipsm,ipkbar,ipinpt,ipreac,ieldat,ipsdm,ipcnpf
     . ,ibigw
c
      dimension x(1),y(1),fmat(9,1),mat(1),lm(12,1),stiff(78),xmass(78),
     . damp(78),fk(12,12),fc(12,12),fm(12,12),xy(2,2,1),
     . nonum(2),id(ndof,1),mht(1),iprop(4,1),
     . ur(12),ui(12),forcer(16),forcei(16),sigi(12),sigr(12)
c     all the dimensions above should be equal to or bigger than
c     the dimensions given below interms of the variables.all
c     variable size are defined in the previous subroutine ashl
c     dimension x(1),y(1),fmat(ncoeff,1),mat(1),lm(nn5,1),stiff(nnsz),
c    .xmass(nnsz),damp(nnsz),ft(nfrc,nn5),fk(nn5,nn5),fc(nn5,nn5),
c    .fm(nn5,nn5),xy(idim,ndof,1),nonum(nnodel),id(ndof,1),mht(1),
c    .iprop(nnlst,1),c(15),d(10,10),ur(nn5),ui(nn5),forcer(nfrc),
c    .forcei(nfrc),sigr(nstr),sigi(nstr),st(nstr,nn5)
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
c      if(idim.gt.2) xy(3,i,n)=z(node)
  135   continue
      call colht(mht,nn5,lm(1,n))
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
      if(ind.eq.3) write(6,392) fmat(7,1)
  392 format(//20x,"following results are for harmonic no= ",f5.1)
      if (ind.eq.3)write(6,391)
      if (ind.eq.3)write(6,390)
  390 format(1x,34hcomplex stresses for ashl elements,
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
      if (ind.eq.1) call echeck(lm(1,n),nn5,icode)
      if (icode.eq.1) go to 280
      m=mat(n)
      ilumps=iprop(1,m)
      idamps=iprop(2,m)
      istres=iprop(3,m)
      nft=iprop(4,m)
c     following property inshel  is a dummy variable. not used
      inshel=iprop(5,m)
      if(ind.eq.3.and.istres.eq.0)go to 280
      if(kpri.eq.3.and.idamps.eq.0)go to 280
      af=fmat(1,m)
      e=fmat(2,m)
      fnu1=fmat(3,m)
      pr=fmat(4,m)
      rho=fmat(5,m)
      fkshr=fmat(6,m)
      fharm=fmat(7,m)
      xix=fmat(8,m)
      ziz=fmat(9,m)
      ec=fmat(10,m)
      ct=fmat(11,m)
      y1=fmat(12,m)
      y2=fmat(13,m)
      kprip=kpri
      if(ind.eq.3)kprip=4
      print 1,af,e,fnu1,pr,rho,fkshr,fharm,xix,ziz,ec,ct,y1,y2
  1   format(10x,"af,e,fnu1,pr,rho,fkshr,fharm,xix,ziz,ec,ct,y1,y2"/
     15x,f8.3,e10.1,11f10.2)
       call astfst(e,fnu1,af,pr,rho,ec,xix,ziz,fkshr,ct,fharm,xy,fk,fc,
     1fm,ur,kprip,sigr,forcer,n,y1,y2)
      if (ind.eq.3) go to 255
c
      m=0
      do 220 i=1,nn5
      do 220 j=i,nn5
      m=m+1
      stiff(m)=fk(i,j)
      damp(m)=fc(i,j)
  220 xmass(m)=fm(i,j)
c     print of element stiffness damping and mass matrices
      if(ibigw.eq.0)go to 950
      if(ipsdm.eq.0)go to 950
      if(kpri.ne.1)go to 702
      write(6,956)n
  956 format(1x//////,1x,30h stiffness for astf element n=,i5,//)
      write(6,953)((fk(ii,jj),jj=1,nn5),ii=1,nn5)
  953 format(1x,1p6e11.3)
  702 if(kpri.ne.3)go to 703
      write(6,957)n
  957 format(1x,//////,1x,35h damping matrix for astf element n=,i5//)
      write(6,953)((fc(ii,jj),jj=1,nn5),ii=1,nn5)
  703 if(kpri.ne.2)go to 950
      write(6,960)n
  960 format(1x,//////,32h mass matrix for astf element n=,i5//)
      write(6,953)((fm(ii,jj),jj=1,nn5),ii=1,nn5)
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
  240   call addban(a,mht,xmass,lm(1,n),nn5)
      go to 280
c
c         assemble damping matrix
c
  250   call addban(a,mht,damp,lm(1,n),nn5)
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
c
c compute forces
c
       call astfst(e,fnu1,af,pr,rho,ec,xix,ziz,fkshr,ct,fharm,xy,fk,fc,
     1fm,ur,kprip,sigr,forcer,n,y1,y2)
       call astfst(e,fnu1,af,pr,rho,ec,xix,ziz,fkshr,ct,fharm,xy,fk,fc,
     1fm,ui,kprip,sigi,forcei,n,y1,y2)
c
c      print forces and moments
c
      int1=nfrc/2
      int2=int1+1
      write(6,362)n,(forcer(i),forcei(i),i=1,int1)
      write(6,363)n,(forcer(i),forcei(i),i=int2,nfrc)
c
c     compute stresses
c        multiply stress transformation matrix times displacement
c        vector
c
c     print stresses
c
       int1=nstr/2
       int2=int1+1
      write(6,362)n,(sigr(i),sigi(i),i=1,int1)
      write(6,363)n,(sigr(i),sigi(i),i=int2,nstr)
  362 format(1x,/,1x,i5,3x,5hnode1,1p8e11.3)
  363 format(1x,i5,3x,5hnode2,1p8e11.3)
  280 continue
      return
c
c         ind=3 and ind=4 passes
c
  300 if(ind.ne.4)return
      do 315 n=1,nume
      call echeck(lm(1,n),nn5,icode)
      if(icode)315,310,315
  310 continue
      ngin=1
      return
  315 continue
      return
      end
*deck astfst
      subroutine astfst(e,fnu1,af,pr,rho,ec,ixx,izz,fkshr,ct,fharm,xy,fk
     1,fc,fm,ug,kprip,sigr,forcer,nelt,y1,y2)
       implicit real*8 (a-h,o-z)
       real*8 nu,ixx,izz,n,n2,k3,moment
       dimension fk(12,12),fc(12,12),fm(12,12),ug(12),forcer(*),
     1 sigr(*),xy(2,2,1)
c      read *,e,af,nu,rho,ec,ixx,izz,k3,ct,iharm,y,x,y1,y2
c      read *,(ug(i),i=1,5)
c      ec=eccentricity of stiff. centroid w.r.t. ref. pt.
c      ec pos. if stiffener is inside shell.
c      y & x are radial and long. coord. of ref. pt.
c      centroid ,ref pt. are in plane normal to axis of rotation
c      stiffener if symmetric w.r.t. above plane.
c      af= x-sectional area of stiffener
c      ixx = m.i. of stiffener related to bending in plane of stiff.
c      izz= m.i. if stiffener related to bending in plane normal.
c      ct= torsional coeff.
c      k3=shear factor. note :this k3= (timo shear factor)**2=5/6
c      rho=mass density/unit volume
c      y1 & y2 are radial dist. of top & bottom fibre of stiffener
c      from centroid of stiffener
c      ug is the displacement vector which is used to calculate stesses
c      note y variable inthis routine is radial distance which is in
c      contradiction to the definition of x-y coord. used in mistic.
       y=xy(1,1,nelt)
       n=fharm
       n2=n*n
       g=e/(2.*(1.+nu))
       e1=e/2.
       yec=y-ec
       yec2=yec**2
       yec3=yec**3
       aa1=1.-ec/y
       aa2=aa1**2
       pi=3.1415926
       ea1=e1*af/yec
       eix=e1*ixx/yec3
       eiz=e1*izz/yec
       gk=g*k3*(af/yec+ixx/yec3)/2.
       gc=g*ct/(2.*yec3)
       do 3 i=1,6
       do 3 j=1,6
       fk(i,j)=0.
       fc(i,j)=0.
  3    fm(i,j)=0.
       fk(1,1)=ea1*aa2*n2
       fk(1,3)=ea1*aa1*n
       fk(1,5)=-ea1*ec*n2*aa1
       fk(2,2)=eiz*(n2/yec)**2+gc*n2
       fk(2,4)=-eiz*n2*(1.+ec*n2/yec)/yec-gc*y*n2
       fk(3,3)=ea1+eix+gk*n2
       fk(3,5)=-n*(ea1*ec+eix*y+gk*y)
       fk(4,4)=eiz*(1.+ec*n2/yec)**2+gc*y**2*n2
       fk(5,5)=ea1*(ec*n)**2+eix*(y*n)**2+gk*y**2
       fk(3,1)=fk(1,3)
       fk(5,1)=fk(1,5)
       fk(4,2)=fk(2,4)
       fk(5,3)=fk(3,5)
       do 1 i=1,5
       do 1 j=1,5
       fk(i,j)=pi*fk(i,j)
       if(iharm.eq.0) fk(i,j)=2.*fk(i,j)
  1    continue
       print 12
       print 10,((fk(i,j),i=1,6),j=1,6)
  10   format(2x,6e20.6)
       fm(1,1)=yec*aa2*af+3.*ixx*yec/y**2
       fm(2,2)=yec*af
       fm(3,3)=yec*af
       fm(4,4)=yec*ec**2*af+yec*izz+(y-3.*ec)*ixx
       fm(5,5)=yec*ec**2*af+(y-3.*ec)*ixx
       fm(2,4)=ixx-ec*yec*af
       fm(1,5)=(2.*y-3.*ec)*ixx/y-ec*aa1*yec*af
       fm(4,2)=fm(2,4)
       fm(5,1)=fm(1,5)
       do 2 i=1,5
       do 2 j=1,5
       fm(i,j)=rho*pi*fm(i,j)
       if(iharm.eq.0) fm(i,j)=2.*fm(i,j)
       fc(i,j)=fk(i,j)*fnu1
  2    continue
       print 11
       print 10,((fm(i,j),i=1,6),j=1,6)
  11   format(20x,"   mass matrix of stiffener    ")
  12   format(////20x,"   stiffness matrix of stiffener   "//////)
       ea=e*af
       eaa=ea/y
       eaae=ea/(y-ec)
       eixx=e*ixx/(y-ec)**2
       ftheta=eaa*n*(ug(1)+y*ug(5))+eaae*(ug(3)-y*n*ug(5))
       moment=-eixx*(ug(3)-y*n*ug(5))
       forcer(1)=ftheta
       forcer(2)=moment
       print 14,ftheta,moment
  14   format(1h1,///10x,"ftheta=  ",e20.6,5x,"  moment=  ",e20.6)
       sigr(1)=ftheta/af+moment*y1/ixx
       sigr(2)=ftheta/af+moment*y2/ixx
       print 15, sigr(1),sigr(2)
  15   format(20x,"  stresses @ top fibre=  ",e20.6,5x," bottom fibre",
     1 " =  ",e20.6)
       return
       end
*deck atri
      subroutine atri
      implicit real*8 (a-h,o-z)
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/dtwo/itwo,ndof,idim
      common/tapcom/nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlen
c
      common a(1)
      real a
c     here ncoeff=15 is used.ei.twelve fixed point variables are allowed
c     1.   prpo=1 material  prpo. given by e,g,nu etc.
c          prpo=2  material      prpo given by cij,11,12,13,22,23,33.
c     2.   rho= mass density per unit volume
c     3.   fharm=  harmonic number
c     4.   e = real part of young modulus.
c     5.   fnu1*e=  imaginary part og young modulus.
c     6.   pr=  poissons ratio
c
c     7. to  15.  coeef. cij  where ij=11,12,13,22,23,33,44,55,66
c     note:   damping matrix= stiffness matrix*fnu1
       ncoeff=15
c      nnlst=5  =total no. of integer variable switches which means
c      as follows.**** 1: =1 means consistent mass. =0 lumped mass
c      2: construct damping matrix.  =1  yes.   =0  no
c      3: stress recovery.   =1  yes.   =0  no
c      integer variables  4  &  5  are dummy variables.:
       nnlst=5
c      nnodel=number of nodes per element. = 3 for this element
       nnodel=3
c      varible nn3=no of node pts*dim of coord( 2 for 2-d,3 for 3-d)
       nn3=nnodel*idim
c      variable nn5=no of node pts*no of dof/node
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
       nptsf=1
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
   10 call
     . atrip(a(n2),a(n2l),a(n1l),a(n2),a(n3),a(n101),a(n102),a(n103),
     .a(n104),a(n105),nume,nummat,ndof,ind,kpri,nt11,ncoeff,nn5,nnsz
     .,nfrc,nstr,nptsf,idim,nnlst,numnp,nnodel)
      if(ind.eq.0)call ewrite(a(n6e),ng)
       if(ind.eq.0) write(6,40) nume,nummat,ndof,ind,kpri,nt11,ncoeff
     .,nn5,nnsz,nfrc,nstr,nptsf,idim,nnlst,numnp
  40   format(/10x,"nume,nummat,ndof,ind,kpri,nt11,ncoeff,nn5,nnsz,
     .,nfrc,nstr,nptsf,idim,nnlst,numnp"/10x,15i5)
   20 format (1h0,39htorage required to input element data =,i10)
   30 format (1h0,21herror nmax less than ,i10)
      return
      end
*deck atrip
      subroutine atrip(a,id,mht,x,y,fmat,xy,mat,lm,iprop,nume,
     .nummat,ndof,ind,kpri,nt11,ncoeff,nn5,nnsz,nfrc,nstr,nptsf,
     .idim,nnlst,numnp,nnodel)
      implicit real*8 (a-h,o-z)
      common/comtyp/itypee,ngroup,ngin,nlen
c
      common /list/ data(64),ndum
       common/prop/prpo,rho,fharm,ee,fnu1,pr,c11,
     1 c12,c13,c22,c23,c33,c44,c55,c66,ilumps,idamps,istres
      common/switch/ipsm,ipkbar,ipinpt,ipreac,ieldat,ipsdm,ipcnpf
     . ,ibigw
c
      dimension x(1),y(1),fmat(15,1),mat(1),lm(9,1),stiff(45),xmass(45),
     . damp(45),fk(9,9),fc(9,9),fm(9,9),xy(2,3,1),
     . nonum(2),id(ndof,1),mht(1),iprop(5,1),
     . ur(9),ui(9),forcer(16),forcei(16),sigi(9),sigr(9)
c     all the dimensions above should be equal to or bigger than
c     the dimensions given below interms of the variables.all
c     variable size are defined in the previous subroutine ashl
c     dimension x(1),y(1),fmat(ncoeff,1),mat(1),lm(nn5,1),stiff(nnsz),
c    .xmass(nnsz),damp(nnsz),ft(nfrc,nn5),fk(nn5,nn5),fc(nn5,nn5),
c    .fm(nn5,nn5),xy(idim,ndof,1),nonum(nnodel),id(ndof,1),mht(1),
c    .iprop(nnlst,1),c(15),d(10,10),ur(nn5),ui(nn5),forcer(nfrc),
c    .forcei(nfrc),sigr(nstr),sigi(nstr),st(nstr,nn5)
      if (ind.ne.0) go to 200
c
c         read and store material data
c       first  nnlst=5  data are integer variable switches and
c       the following ncoeff=15 data are material properties
c
      do 115 m=1,nummat
      call fld
      do 110 n=1,ncoeff
  110 fmat(n,m)=data(n+nnlst)
      do 115 n=1,nnlst
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
c      if(idim.gt.2) xy(3,i,n)=z(node)
  135   continue
      call colht(mht,nn5,lm(1,n))
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
      if(ind.eq.3) write(6,392) fmat(3,1)
  392 format(//20x,"following results are for harmonic no= ",f5.1)
      if (ind.eq.3)write(6,391)
      if (ind.eq.3)write(6,390)
  390 format(1x,34hcomplex stresses for atri elements,
     .31h(second two lines of print out)//,5h  elm,5x,
     . 4hnode,2x,2(1x,10hsigma-xx-u),2(1x,10hsigma-xx-l),
     . 2(1x,10hsigma-yy-u),2(1x,10hsigma-yy-l),/,8x,6hnumber
     . ,4(5x,4hreal,7x,4himag,2x)//)
  391 format(1x,44hcomplex forces and moments for atri elements,
     .30h(first two lines of print out)//,
     . 5h  elm,5x,4hnode,2x,2(4x,3hn-s,4x),2(3x,4hn-th,4x),2(4x,3hm-s
     . ,4x), 2(3x,4hm-th,4x), /,8x,6hnumber,4(5x,4hreal,7x,4himag,2x)//)
      do 280 n=1,nume
      icode=0
      if (ind.eq.1) call echeck(lm(1,n),nn5,icode)
      if (icode.eq.1) go to 280
      m=mat(n)
      ilumps=iprop(1,m)
      idamps=iprop(2,m)
      istres=iprop(3,m)
c      following two variables nft & inshel  are dummy variables.
      nft=iprop(4,m)
      inshel=iprop(5,m)
      if(ind.eq.3.and.istres.eq.0)go to 280
      if(kpri.eq.3.and.idamps.eq.0)go to 280
       prpo=fmat(1,m)
       rho=fmat(2,m)
       fharm=fmat(3,m)
       ee=fmat(4,m)
       fnu1=fmat(5,m)
       pr= fmat(6,m)
       c11=fmat(7,m)
       c12=fmat(8,m)
       c13=fmat(9,m)
       c22=fmat(10,m)
       c23=fmat(11,m)
       c33=fmat(12,m)
       c44=fmat(13,m)
       c55=fmat(14,m)
       c66=fmat(15,m)
       if(prpo.eq.2.) go to 50
       xlem=ee*pr/((1.+pr)*(1.-2.*pr))
       xmu=ee/(2.*(1.+pr))
       c11=xlem+2.*xmu
       c22=c11
       c33=c11
       c12=xlem
       c13=c12
       c23=c12
       c44=xmu
       c55=xmu
       c66=xmu
  50   continue
      kprip=kpri
      if(ind.eq.3)kprip=4
       print 1,ee,pr,fnu1,rho,((xy(ij1,ij2,n),ij1=1,2),
     1 ij2=1,3)
  1    format(5x,5e15.5)
       call atristf(xy,fk,fc,n)
       call atrims(xy,fm,n)
      if (ind.eq.3) go to 255
c
      m=0
      do 220 i=1,nn5
      do 220 j=i,nn5
      m=m+1
      stiff(m)=fk(i,j)
      if(idamps.eq.1) damp(m)=fc(i,j)
  220 xmass(m)=fm(i,j)
c     print of element stiffness damping and mass matrices
      if(ibigw.eq.0)go to 950
      if(ipsdm.eq.0)go to 950
      if(kpri.ne.1)go to 702
      write(6,956)n
  956 format(1x//////,1x,30h stiffness for axsh element n=,i5,//)
      write(6,953)((fk(ii,jj),jj=1,nn5),ii=1,nn5)
  953 format(1x,1p6e11.3)
  702 if(kpri.ne.3)go to 703
      write(6,957)n
  957 format(1x,//////,1x,35h damping matrix for axsh element n=,i5//)
      write(6,953)((fc(ii,jj),jj=1,nn5),ii=1,nn5)
  703 if(kpri.ne.2)go to 950
      write(6,960)n
  960 format(1x,//////,32h mass matrix for axsh element n=,i5//)
      write(6,953)((fm(ii,jj),jj=1,nn5),ii=1,nn5)
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
c
c compute forces
c
c     call ashlst(thic,ts,e,pr,fnu1,rho,fkshr,xy,fk,fc,fm,
c    1,ur,iharm,kprip,sigr,forcer,n,ndof)
c     call ashlst(thic,ts,e,pr,fnu1,rho,fkshr,xy,fk,fc,fm,
c    1,ui,iharm,kprip,sigi,forcei,n,ndof)
c
c      print forces and moments
c
      int1=nfrc/2
      int2=int1+1
      write(6,362)n,(forcer(i),forcei(i),i=1,int1)
      write(6,363)n,(forcer(i),forcei(i),i=int2,nfrc)
c
c     compute stresses
c        multiply stress transformation matrix times displacement
c        vector
c
c     print stresses
c
       int1=nstr/2
       int2=int1+1
      write(6,362)n,(sigr(i),sigi(i),i=1,int1)
      write(6,363)n,(sigr(i),sigi(i),i=int2,nstr)
  362 format(1x,/,1x,i5,3x,5hnode1,1p8e11.3)
  363 format(1x,i5,3x,5hnode2,1p8e11.3)
  280 continue
      return
c
c         ind=3 and ind=4 passes
c
  300 if(ind.ne.4)return
      do 315 n=1,nume
      call echeck(lm(1,n),nn5,icode)
      if(icode)315,310,315
  310 continue
      ngin=1
      return
  315 continue
      return
      end
*deck masmat
      subroutine masmat(n,emass,ndgree,nlink,nfour,ix,r,z,ro,mtype)
      implicit real*8 (a-h,o-z)
c
c        computes mass matrix for axisymmetric elements with
c        harmonic variations in theta direction
c
      dimension ix(5,1),r(1),z(1),ro(1),emass(1)
      dimension rn(4),zn(4),xint(4),f(4)
c
c        dimensions below must correspond to ngauss
      dimension xord(4),wate(4)
      data
     1 ngauss/4/,
     2 wate/  0.1739274, 0.3260725, 0.3260725, 0.1739274 /,
     3 xord/  0.06943185, 0.3300095, 0.6699905, 0.9305682 /
      do 10 i=1,4
      j=ix(i,n)
      rn(i)=r(j)
   10 zn(i)=z(j)
      a1=rn(2)-rn(1)
      a2=rn(4)-rn(1)
      a3=rn(3)+rn(1)-rn(2)-rn(4)
      a4=rn(3)-rn(1)
      b1=zn(2)-zn(1)
      b2=zn(4)-zn(1)
      b3=zn(3)+zn(1)-zn(2)-zn(4)
      if(nlink.gt.3) go to 11
      xjac=a1*b2-a2*b1
      if (xjac.lt.0.0) go to 900
c
c        lumped masses for triangle
c
      xint(1)=xjac*(rn(1)/6.0+(a1+a4)/24.0)
      xint(2)=xjac*(rn(1)/6.+(2.*a1+a4)/24.0)
      xint(3)=xjac*(rn(1)/6.+(2.*a4+a1)/24.0)
      go to 80
c
c        lumped masses for quadrilateral
c
   11 do 12 i=1,4
   12 xint(i)=0.0
c
c***     integration loop
c
      do 20 i=1,ngauss
      psi=xord(i)
      do 20 j=1,ngauss
      eta=xord(j)
      xjac=(a1+a3*eta)*(b2+b3*psi)-(b1+b3*eta)*(a2+a3*psi)
      if (xjac.lt.0.0) go to 900
      rr=rn(1)+a1*psi+a2*eta+a3*psi*eta
c
c        functions to be integrated
c
      f(1)=rr*(1.0-psi-eta+psi*eta)
      f(2)=rr*(psi-psi*eta)
      f(3)=rr*psi*eta
      f(4)=rr*(eta-eta*psi)
c
c        add to integrands
c
      do 18 k=1,nlink
   18 xint(k)=xint(k)+xjac*f(k)*wate(i)*wate(j)
   20 continue
c
c        multiply by density and insert in emass
c
   80 k=1
      do 90 i=1,nlink
      do 90 j=1,ndgree
      emass(k)=ro(mtype)*xint(i)
   90 k=k+1
      return
  900 write(6,901) n
  901 format (1h0,5x,"negative area in element",i4)
      return
      end
*deck atristf
      subroutine atristf(xy,fk,fc,n)
      implicit real*8 (a-h,o-z)
c
c*****    this subroutine forms the triangular element stiffness matrix
c*****    and body force vector
c
c      0<&=n<=n  where n is harmonic number & n =highest harmonic
c      to be used. in the following analysis n=fharm
c     common /qadtri/
c    1 r(6), z(6), bf(9), xi(10), s, a(3,3), e(15)
       dimension r(6),z(6),bf(9),xi(10),a(3,3)
c     common/fourer/
c    1 numfou,nfour
       common/prop/prpo,rho,fharm,ee,fnu1,pr,c11,
     1 c12,c13,c22,c23,c33,c44,c55,c66,ilumps,idamps,istres
      dimension
     1  suu(3,3), suw(3,3), sut(3,3), sww(3,3), swt(3,3),
     2 stt(3,3), tuu(3,3), tuw(3,3), tut(3,3), tww(3,3), twt(3,3),
     3 ttt(3,3), btfr(3), btfz(3), btft(3),s(9,9),fk(9,9)
      dimension psi(7,9),fcf(7,7),xy(2,3,1),ii(10),fc(9,9)
c     equivalence
c    1 (e(1),c11), (e(2),c12), (e(3),c13), (e(4),c22), (e(5),c23),
c    2 (e(6),c33), (e(7),c44), (e(8),c55), (e(9),c66), (e(10),tlr),
c    c (e(11),tlz), (e(12),tlt), (e(13),bfr), (e(14),bfc),
c    4 (e(15),bfz), (s(1,1),suu), (s(1,7),suw), (s(1,8),sut),
c    5 (s(1,2),sww), (s(1,9),swt), (s(1,3),stt), (s(7,4),btfr),
c    6 (s(7,5),btfz), (s(7,6),btft)
      real*8
     1 nf, nf2
       nf=fharm
       nf2=nf*nf
       nfour=nf+1
       z(1)=xy(1,1,n)
       r(1)=xy(2,1,n)
       z(2)=xy(1,2,n)
       r(2)=xy(2,2,n)
       z(3)=xy(1,3,n)
       r(3)=xy(2,3,n)
c
c*****    form the volume integrals
c
      do 3 i=1,9
    3 bf(i)=0.0
c
      if (nfour.gt.2) go to 100
c
c        for nfour.le.2 , special stiffness matrices are n
c     needed for all elements with nodes on the z-axis
c
c     count number of nodes on z-axis
c
      kount=0
      zmax=-1.e+25
      do 5 i=1,3
      if (abs(r(i)).gt.1.e-4) go to 5
      key=i
      kount=kount+1
      if (z(i).lt.zmax) go to 5
      zmax=z(i)
    5 continue
      if (kount.eq.0) go to 100
      do 4 j=1,3
    4 z(j)=z(j)-zmax
      if (kount.lt.3) go to 10
    6 xi(2)=-1.e+10
      print 7,n
  7   format(10x,"element no. ",i5," has all nodes along axis of rot."
     1 ," run stopped at statement 7 in atristf routine")
      stop
   10 do 12 j=1,7
      do 11 i=1,9
   11 psi(j,i)=0.0
      do 12 i=1,7
   12 fcf(j,i)=0.0
      call integr(r,z,xi)
      if(kount.eq.2) go to 60
c
c     one node on z-axis
c
      iii=key
      ij=key+1
      if(ij.gt.3) ij=ij-3
      ik=key+2
      if(ik.gt.3) ik=ik-3
      rj=r(ij)
      zj=z(ij)
      rk=r(ik)
      zk=z(ik)
      det=rj*zk-rk*zj
c
c        special computation of xi(6)  , integral of z*z/r
      aa=zj
      bb=zk-zj
      cc=rj
      dd=rk-rj
      tlog=log(rk/rj)
      if(abs(dd).lt.1.e-3) go to 210
      xi(6)= det*(tlog*(aa**2-2.*aa*bb*cc/dd+(bb*cc/dd)**2) +
     + (2.*aa*bb-cc*bb**2/dd+0.5*bb**2))/(3.*dd)
      go to 211
  210 xi(6)= det/(3.*rj)*(zk*zj+(zk-zj)**2/3.0)
  211 continue
      if (nfour.eq.1) go to 239
c
c        psi inverse for nfour = 2,  one node on z axis
      do 200 i=1,3
      if(i.ne.key) go to 201
      psi(1,i)=1.0
      psi(2,i)=(zj-zk)/det
      psi(3,i)=(rk-rj)/det
      psi(6,i)=(zk-zj)/det
      psi(7,i)=(rj-rk)/det
      go to 200
  201 if(i.ne.ij) go to 202
c
c     then i=ij
c
      do 203 l=1,3
      m=i+3*l-3
      psi(2*l,m)=zk/det
  203 psi(2*l+1,m)=-rk/det
      go to 200
  202 continue
c
c     then i=ik
c
      do 204 l=1,3
      m=i+3*l-3
      psi(2*l,m)=-zj/det
  204 psi(2*l+1,m)=rj/det
  200 continue
c
c        fcf matrix for nfour=2, one node on z-axis
c
      fcf(2,2)=(c11+2.*c13+c33+c55)*xi(1)
      fcf(2,3)=(c13+c33+c55)*xi(4)
      fcf(2,5)=(c12+c23)*xi(1)
      fcf(2,6)=(c13+c33)*xi(1)
      fcf(2,7)=fcf(2,3)
      fcf(3,3)=c44*xi(1)+(c33+c55)*xi(6)
      fcf(3,4)=c44*xi(1)
      fcf(3,5)=c23*xi(4)
      fcf(3,6)=c33*xi(4)
      fcf(3,7)=(c33+c55)*xi(6)
      fcf(4,4)=(c44+c66)*xi(1)
      fcf(4,5)=c66*xi(4)
      fcf(4,7)=-c66*xi(1)
      fcf(5,5)=c22*xi(1)+c66*xi(6)
      fcf(5,6)=c23*xi(1)
      fcf(5,7)=(c23-c66)*xi(4)
      fcf(6,6)=c33*xi(1)
      fcf(6,7)=c33*xi(4)
      fcf(7,7)=(c33+c55)*xi(6)+c66*xi(1)
c
c     compute stiffness matrix s(9,9)
c
c     s=psi(trans)*fcf*psi
      call transf(fcf,7,7,psi,9,9,s)
      go to 20
c
c        psi inverse for nfour=1, one node on z axis
  239 do 240 i=1,3
      i3=i+3
      if (i.ne.key) go to 241
c
      psi(3,i3) = 1.0
      psi(4,i3) = (zj-zk)/det
      psi(5,i3) = (rk-rj)/det
      go to 240
c
  241 if (i.ne.ij) go to 242
c
      psi(1,i)=zk/det
      psi(2,i)=-rk/det
      psi(4,i3)=zk/det
      psi(5,i3)=-rk/det
      go to 240
c
  242 psi(1,i)=-zj/det
      psi(2,i)=rj/det
      psi(4,i3)=-zj/det
      psi(5,i3)=rj/det
  240 continue
c
c        fcf matrix for nfour=1, one node on z-axis
c
      fcf(1,1)=(c11+2.0*c12+c22)*xi(1)
      fcf(1,2)=(c12+c22)*xi(4)
      fcf(1,5)=(c13+c23)*xi(1)
      fcf(2,2)=c44*xi(1)+c22*xi(6)
      fcf(2,4)=c44*xi(1)
      fcf(2,5)=c23*xi(4)
      fcf(4,4)=c44*xi(1)
      fcf(5,5)=c33*xi(1)
      call transf(fcf,5,7,psi,9,9,s)
      go to 20
c
c     two nodes on z-axis
c
   60 do 64 i=1,3
      if ( abs(r(i)).le.1.e-4.and.abs(z(i)).le.1.e-4) go to 64
      if (abs(r(i)).le.1.e-4) go to 63
      z3=z(i)
      r3=r(i)
      go to 64
   63 z2=z(i)
   64 continue
      if (nfour.eq.1) go to 84
c
c     set psi(which is psi-inverse)
c
      do 65 i=1,3
      if ( abs(r(i)).le.1.e-4. and .abs(z(i)).le.1.e-4 ) go to 66
      if ( abs(r(i)).le.1.e-4 ) go to 67
      psi(2,i)=1.0/r3
      psi(4,i+3)=1.0/r3
      psi(5,i+6)=1.0/r3
      go to 65
   66 psi(1,i)=1.0
      psi(2,i)=(z3-z2)/(r3*z2)
      psi(3,i)=-1.0/z2
      psi(5,i)=(1.0-z3/z2)/r3
      go to 65
   67 psi(2,i)=-z3/(r3*z2)
      psi(3,i)=1.0/z2
      psi(5,i)=z3/(r3*z2)
   65 continue
c
c     set fcf matrix
c
      fcf(2,2)=c11+2.0*c13+c33+c55
      fcf(2,5)=c13+c33
      fcf(3,3)=c44+c66
      fcf(3,4)=c44+c66
      fcf(4,4)=c44+c66
      fcf(5,5)=c33
      do 75 i=1,5
      do 75 j=1,i
      fcf(i,j)=fcf(j,i)*xi(1)
   75 fcf(j,i)=fcf(i,j)
c     compute stiffness matrix =
c     (psi)trans*(fcf)*(psi)
c
      call transf(fcf,5,7,psi,9,9,s)
      go to 20
c
c        psi inverse for nfour=1, 2 nodes on z axis
c
   84 do 85 i=1,3
      i3=i+3
      if ( abs(r(i)).le.1.e-4.and. abs(z(i)).le.1.e-4) go to 86
      if (abs(r(i)).le.1.e-4) go to 87
c
c        i is node ik  or 3
      psi(1,i)=1.0/r3
      psi(3,i3)=1.0/r3
      go to 85
c
c        i is node key or  1
   86 psi(2,i3)=1.0
      psi(3,i3)=(z3-z2)/(r3*z2)
      psi(4,i3)=-1.0/z2
      go to 85
c
c        i is node ij or  2
   87 psi(3,i3)=-z3/(r3*z2)
      psi(4,i3)=1.0/z2
   85 continue
c
c        fcf for nfour=1, 2 nodes on z axis
      fcf(1,1)=(c11+2.0*c12+c22)*xi(1)
      fcf(1,4)=(c13+c23)*xi(1)
      fcf(3,3)=c44*xi(1)
      fcf(4,4)=c33*xi(1)
      call transf(fcf,4,7,psi,9,9,s)
      go to 20
  100 continue
c*****    form the displacement transformation matrix
      call integr(r,z,xi)
c
      if (xi(2).le.0.0) return
      c=0.5/xi(2)
      a(1,1)=c*(r(2)*z(3)-r(3)*z(2))
      a(1,2)=c*(r(3)*z(1)-r(1)*z(3))
      a(1,3)=c*(r(1)*z(2)-r(2)*z(1))
      a(2,1)=c*(z(2)-z(3))
      a(2,2)=c*(z(3)-z(1))
      a(2,3)=c*(z(1)-z(2))
      a(3,1)=c*(r(3)-r(2))
      a(3,2)=c*(r(1)-r(3))
      a(3,3)=c*(r(2)-r(1))
c
c*****    form the stiffness matricies in the generalized coordinates
c
      x=c33+nf2*c55
      suu(1,1)=x*xi(3)
      suu(1,2)=(c13+x)*xi(2)
      suu(1,3)=x*xi(5)
      suu(2,1)=suu(1,2)
      suu(2,2)=(c11+2.0*c13+x)*xi(1)
      suu(2,3)=(c13+x)*xi(4)
      suu(3,1)=suu(1,3)
      suu(3,2)=suu(2,3)
      suu(3,3)=x*xi(6)+c44*xi(1)
      y=nf2*c66
      sww(1,1)=y*xi(3)
      sww(1,2)=y*xi(2)
      sww(1,3)=y*xi(5)
      sww(2,1)=sww(1,2)
      sww(2,2)=(c44+y)*xi(1)
      sww(2,3)=y*xi(4)
      sww(3,1)=sww(1,3)
      sww(3,2)=sww(2,3)
      sww(3,3)=y*xi(6)+c22*xi(1)
      y=c33*nf2
      x=c55+y
      stt(1,1)=x*xi(3)
      stt(1,2)=y*xi(2)
      stt(1,3)=x*xi(5)
      stt(2,1)=stt(1,2)
      stt(2,2)=y*xi(1)
      stt(2,3)=y*xi(4)
      stt(3,1)=stt(1,3)
      stt(3,2)=stt(2,3)
      stt(3,3)=x*xi(6)+c66*xi(1)
      x=c33*nf
      c=c55*nf
      y=x+c13*nf
      sut(2,2)=y*xi(1)
      y=y+c
      c=x+c
      sut(1,1)=c*xi(3)
      sut(1,2)=x*xi(2)
      sut(1,3)=c*xi(5)
      sut(2,1)=y*xi(2)
      sut(2,3)=y*xi(4)
      sut(3,1)=c*xi(5)
      sut(3,2)=x*xi(4)
      sut(3,3)=c*xi(6)
      suw(1,1)=0.0
      suw(1,2)=0.0
      suw(1,3)=c23*xi(2)
      suw(2,1)=0.0
      suw(2,2)=0.0
      suw(2,3)=(c12+c23)*xi(1)
      suw(3,1)=0.0
      suw(3,2)=c44*xi(1)
      suw(3,3)=c23*xi(4)
      x=c66*nf
      y=c23*nf
      swt(1,1)=0.0
      swt(1,2)=0.0
      swt(1,3)=-x*xi(2)
      swt(2,1)=0.0
      swt(2,2)=0.0
      swt(2,3)=-x*xi(1)
      swt(3,1)=y*xi(2)
      swt(3,2)=y*xi(1)
      swt(3,3)=(y-x)*xi(4)
       print 172,((suu(i,j),j=1,3),i=1,3)
       print 172,((sut(i,j),j=1,3),i=1,3)
       print 172,((suw(i,j),j=1,3),i=1,3)
       print 172,((stt(i,j),j=1,3),i=1,3)
       print 172,((swt(i,j),j=1,3),i=1,3)
       print 172,((sww(i,j),j=1,3),i=1,3)
  172  format(5x,3e16.6)
c
c*****    form body force vector
c
      btfr(1)=tlt*xi(2)+bfr*xi(7)+bfc*xi(1)
      btfr(2)=(tlr+tlt)*xi(1)+bfr*xi(9)+bfc*xi(7)
      btfr(3)=tlt*xi(4)+bfr*xi(10)+bfc*xi(8)
      btfz(1)=bfz*xi(1)
      btfz(2)=bfz*xi(7)
      btfz(3)=tlz*xi(1)+bfz*xi(8)
      x=tlt*nf
      btft(1)=x*xi(2)+bfc*xi(1)
      btft(2)=x*xi(1)+bfc*xi(7)
      btft(3)=x*xi(4)+bfc*xi(8)
c
c*****    transform the stiffness matrix and body force vector to
c*****    global coordinates
c
      do 130 i=1,3
      do 130 j=1,3
      tuu(i,j)=0.0
      tuw(i,j)=0.0
      tut(i,j)=0.0
      tww(i,j)=0.0
      twt(i,j)=0.0
      ttt(i,j)=0.0
      do 130 k=1,3
      c=a(k,j)
      tuu(i,j)=tuu(i,j)+suu(i,k)*c
      tuw(i,j)=tuw(i,j)+suw(i,k)*c
      tut(i,j)=tut(i,j)+sut(i,k)*c
      tww(i,j)=tww(i,j)+sww(i,k)*c
      twt(i,j)=twt(i,j)+swt(i,k)*c
  130 ttt(i,j)=ttt(i,j)+stt(i,k)*c
      do 140 i=1,3
      iw=i+3
      it=iw+3
      bf(i)=0.0
      bf(iw)=0.0
      bf(it)=0.0
      do 140 j=1,3
      s(i,j)=0.0
      jw=j+3
      jt=jw+3
      s(i,jw)=0.0
      s(i,jt)=0.0
      s(iw,jw)=0.0
      s(iw,jt)=0.0
      s(it,jt)=0.0
      c=a(j,i)
      bf(i)=bf(i)+c*btfr(j)
      bf(iw)=bf(iw)+c*btfz(j)
      bf(it)=bf(it)+c*btft(j)
      do 140 k=1,3
      c=a(k,i)
      s(i,j)=s(i,j)+c*tuu(k,j)
      s(i,jw)=s(i,jw)+c*tuw(k,j)
      s(i,jt)=s(i,jt)+c*tut(k,j)
      s(iw,jw)=s(iw,jw)+c*tww(k,j)
      s(iw,jt)=s(iw,jt)+c*twt(k,j)
  140 s(it,jt)=s(it,jt)+c*ttt(k,j)
  20   continue
       do 22 i=1,9
       do 22 j=i,9
       s(j,i)=s(i,j)
  22   continue
c      ***************************************************************
c      following steps transforms matrix s to matrix fk.
c      matrix "s" is related to displacement vector
c      (ur1,ur2,ur3,uz1,uz2,uz3,ut1,ut2,ut3)   whereas matrix "fk"
c      is realated to vector (ut1,uz1,ur1,ut2,uz2,ur2,ut3,uz3,ur3)
c      this second vector is used in mistic
c      whereas dynax program uses first vector
c      ***************************************************************
       ii(1)=7
       ii(2)=4
       ii(3)=1
       ii(4)=8
       ii(5)=5
       ii(6)=2
       ii(7)=9
       ii(8)=6
       ii(9)=3
c      **************************************************************
c      dynax program factored out the multiplying factor  pi for n>0
c      & 2.*pi for n=0   where pi=3.1415926.
c      in the following steps those factors are reintroduced
c      ***************************************************************
        pi2=3.1415926
        if(nf.eq.0.) pi2=2.*3.1415926
       do 21 i=1,9
       i1=ii(i)
       do 21 j=1,9
       j1=ii(j)
       fk(i,j)=s(i1,j1)*pi2
       fc(i,j)=fk(i,j)*fnu1
  21   continue
        print 170
        print 171,((s(i,j),j=1,9),i=1,9)
       print 170
       print 171,((fk(i,j),j=1,9),i=1,9)
       print 170
       print 171,((fc(i,j),j=1,9),i=1,9)
  170  format(20x,"****************************************")
  171  format(5x,9e12.6)
       return
       end
*deck integr
      subroutine integr(r,z,xi)
      implicit real*8 (a-h,o-z)
c
c*****    this subroutine calculates the triangular volume integrals
c*****    for the stiffness matrix and body force vector.
c
c     common /qadtri/
c    1 r(6), z(6), bf(9), xi(10), s(9,9), a(3,3), e(15)
      dimension
     1 xm(6),r(6),z(6),xi(10)
      do 100 i=3,10
  100 xi(i)=0.0
c
c*****    form the area and volume integrals using the exact expression
c
      xi(2)=0.5*(r(1)*(z(2)-z(3))+r(2)*(z(3)-z(1))+r(3)*(z(1)-z(2)))
      c=xi(2)/3.0
      xi(1)=c*(r(1)+r(2)+r(3))
      xi(4)=c*(z(1)+z(2)+z(3))
      c=0.25*c
      r(4)=0.5*(r(1)+r(2))
      r(5)=0.5*(r(2)+r(3))
      r(6)=0.5*(r(3)+r(1))
      z(4)=0.5*(z(1)+z(2))
      z(5)=0.5*(z(2)+z(3))
      z(6)=0.5*(z(3)+z(1))
      d=3.0*c
      do 200 i=1,3
      j=i+3
      xm(i)=c*r(i)
  200 xm(j)=d*r(j)
      do 300 i=1,6
      q=xm(i)
      b=r(i)
      c=z(i)
      d=q*b
      xi(7)=xi(7)+d
      xi(8)=xi(8)+q*c
      xi(9)=xi(9)+d*b
      xi(10)=xi(10)+d*c
      if (b.le.1.e-4) b=0.0001
      d=q/(b*b)
      xi(3)=xi(3)+d
      xi(5)=xi(5)+d*c
      xi(6)=xi(6)+d*c*c
  300 continue
      return
      end
*deck transf
      subroutine transf(a,n,ndim,b,m,mdim,c)
      implicit real*8 (a-h,o-z)
      dimension a(ndim,ndim),b(ndim,mdim),c(mdim,mdim)
      do 2 i=1,n
      do 2 j=i,n
    2 a(j,i)=a(i,j)
      do 1 i=1,m
      do 1 j=1,m
      c(i,j)=0.0
      do 1 l=1,n
      do 1 k=1,n
    1 c(i,j)=c(i,j)+b(l,i)*a(l,k)*b(k,j)
      return
      end
*deck stress1
       subroutine stress1(n,sig,ord,kode,mtype)
      implicit real*8 (a-h,o-z)
      complex ud,udx,ur,uk,wk,tk,ui,uj,wi,wj,ti,tj,trr,tzz,trz,tzt,trt,
     *err,ezz,erz,ert,ezt,       er,ez,et,sig,forcd
      common/stiter/
     1 ud(690),ud1(690),ud2(690),forcd(690),r(230),z(230),
     2 ro(10),e(12,10),noddis(50),nodfrc(20),
     3 numnp,numel,ndgree,mud,numdis,numfrc,ix(5,355)
      common/tempfd/
     1 to,t(230)
      common/fourer/
     1 numfou,nfour,nf,nf2
      dimension
     1 ee(12),sig(6),ur(1),ord(2)
      equivalence
     1 (ud(1),ur(1))
      real*8
     1 nf, nf2
      dimension udx(3,230)
      equivalence (udx(1,1),ud(1))
c
c     set u-theta= -u-r for all nodes on z-axis if nfour=2
c
      if(nfour.ne.2) go to 55
   50 do 54 i=1,4
      j=ix(i,n)
      if ( abs(r(j)).lt.1.e-4) udx(3,j)=-udx(1,j)
   54 continue
   55 mprint=0
c
c*****    calculation of center node variables
c
      i=ix(1,n)
      j=ix(2,n)
      k=ix(3,n)
      l=ix(4,n)
      ii=3*i
      jj=3*j
      kk=3*k
      ll=3*l
      if(k.eq.l) go to 60
      nm=4
      c=0.25
      uk=c*(ur(ii-2)+ur(jj-2)+ur(kk-2)+ur(ll-2))
      wk=c*(ur(ii-1)+ur(jj-1)+ur(kk-1)+ur(ll-1))
      tk=c*(ur(ii)+ur(jj)+ur(kk)+ur(ll))
      temp=c*(t(i)+t(j)+t(k)+t(l))
      rk=c*(r(i)+r(j)+r(k)+r(l))
      zk=c*(z(i)+z(j)+z(k)+z(l))
      go to 70
   60 c=1./3.
      nm=1
      uk=ur(kk-2)
      wk=ur(kk-1)
      tk=ur(kk)
      temp=c*(t(i)+t(j)+t(k))
      rk=r(k)
      zk=z(k)
      c=1.
   70 continue
c
c*****    calculation of element properties
c
      do 90 m=1,9
   90 ee(m)=e(m,mtype)
      temp=temp-to
      do 100 m=10,12
  100 ee(m)=e(m,mtype)*temp
c
c*****    calculation of stresses at center np
c
      err=(0.0,0.0)
      ezz=(0.0,0.0)
      ett=(0.0,0.0)
      erz=(0.0,0.0)
      ert=(0.0,0.0)
      ezt=(0.0,0.0)
c
c*****    compute stress at node k of triangle
c
      do 200 m=1,nm
      mm=m+1
      if(m.eq.4) mm=1
      i=ix(m,n)
      j=ix(mm,n)
      ii=3*i
      jj=3*j
      ui=ur(ii-2)
      uj=ur(jj-2)
      wi=ur(ii-1)
      wj=ur(jj-1)
      ti=ur(ii)
      tj=ur(jj)
      ri=r(i)
      rj=r(j)
      zi=z(i)
      zj=z(j)
      aj=rj-ri
      ak=rk-ri
      bj=zj-zi
      bk=zk-zi
      x=aj-ak
      y=bj-bk
      d=aj*bk-ak*bj
      trr=y*ui+bk*uj-bj*uk
      tzz=-x*wi-ak*wj+aj*wk
      trz=-x*ui+y*wi-ak*uj+bk*wj+aj*uk-bj*wk
      tzt=-x*ti-ak*tj+aj*tk
      if(abs(rk).lt.1.e-4)trt=+zj*ti-zi*tj
c     bypass oirgin singularity
      if(abs(rk).lt.1.e-4) go to 355
      trt=-((rj*zk-rk*zj)*ti+(rk*zi-ri*zk)*tj+(ri*zj-rj*zi)*tk+zk*tzt)/
     1 rk
  355 continue
      err=trr/d+err
      ezz=tzz/d+ezz
      erz=trz/d+erz
      ert=trt/d+ert
      ezt=tzt/d+ezt
  200 continue
c
c        for computation of ett in triangles, let uk,tk,rk
c        take on average values
      if (nm.ne.1) go to 205
      cc=1./3.
      rk=cc*(ri+rj+rk)
      tk=cc*(ti+tj+tk)
      uk=cc*(ui+uj+uk)
  205 continue
      err=c*err
      ezz=c*ezz
      ett=(uk+nf*tk)/rk
      erz=c*erz
      ert=c*ert-nf*uk/rk
      ezt=c*ezt-nf*wk/rk
c
c*****    calculate stresses
c
      er=err-ee(10)
      ez=ezz-ee(11)
      et=ett-ee(12)
c
c    kode .eq. 0 or 2 calculate stresses
c      kode.ne.0 or 2 calculate strains
c
      if(kode.eq.0.or.kode.eq.2) go to 300
        do 310 i=1,9
         ee(i)=1.
  310 if(i.eq.2.or.i.eq.3.or.i.eq.5) ee(i)=0.
  300  continue
      sig(1)=ee(1)*er+ee(2)*ez+ee(3)*et
      sig(2)=ee(2)*er+ee(4)*ez+ee(5)*et
      sig(3)=ee(3)*er+ee(5)*ez+ee(6)*et
      sig(4)=ee(7)*erz
      sig(5)=ee(8)*ert
      sig(6)=ee(9)*ezt
      ord(1)=rk
      ord(2)=zk
      return
      end
*deck atrims
       subroutine atrims(xy,trms,n)
       implicit real*8 (a-h,o-z)
       common/prop/prpo,rho,fharm,ee,fnu1,pr,c11,
     1 c12,c13,c22,c23,c33,c44,c55,c66,ilumps,idamps,istres
       dimension  r(3),z(3),trms(9,9),xlmpms(9),xy(2,3,1)
       dimension xi(3,3),phij(9,9),xmi(9,9),trims(9,9),psi(9,9)
       z(1)=xy(2,1,n)
       r(1)=xy(1,1,n)
       z(2)=xy(2,2,n)
       r(2)=xy(1,2,n)
       z(3)=xy(2,3,n)
       r(3)=xy(1,3,n)
       print 100,fharm,rho,(r(i),z(i),i=1,3)
  100  format(20x,"harmonic no. =   ",f5.1,"  mass density/unit vol=  "
     1 ,f10.6//20x,"   r        &        z    coord of pts of triangle "
     2  / 5x,2f20.5/5x,2f20.5/5x,2f20.5)
       pi=3.1415926
       deln=pi
       if(fharm.eq.0.) deln=2.*pi
       r1=r(1)
       r2=r(2)
       r3=r(3)
       z1=z(1)
       z2=z(2)
       z3=z(3)
       if(ilumps.eq.0) go to 30
       if(z1.eq.z2) b1=1000.
       if(z1.ne.z2) b1=(r2-r1)/(z2-z1)
       a1=r1-b1*z1
       if(z2.eq.z3) b2=1000.
       if(z2.ne.z3) b2=(r3-r2)/(z3-z2)
       a2=r2-b2*z2
       if(z1.ne.z3) b3=(r1-r3)/(z1-z3)
       if(z1.eq.z3) b3=1000.
       a3=r3-b3*z3
       a12=a1**2
       a22=a2**2
       a32=a3**2
       a13=a1**3
       a23=a2**3
       a33=a3**3
       b12=b1**2
       b22=b2**2
       b32=b3**2
       a1b1=a1*b1
       a2b2=a2*b2
       a3b3=a3*b3
       a1a2=a1*a2
       a1a3=a1*a3
       a2a3=a2*a3
       b1b2=b1*b2
       b1b3=b1*b3
       b2b3=b2*b3
       z12=z1**2
       z13=z1**3
       z14=z1**4
       z15=z1**5
       z22=z2**2
       z23=z2**3
       z24=z2**4
       z25=z2**5
       z32=z3**2
       z33=z3**3
       z34=z3**4
       z35=z3**5
       xi(1,1)=0.5*(a32*(z3-z1)+a3b3*(z32-z12)+b32*(z33-z13)/3.
     1 -a12*(z2-z1)-a1b1*(z22-z12)-b12*(z23-z13)/3.
     2 -a22*(z3-z2)-a2b2*(z32-z22)-b22*(z33-z23)/3.)
       if(b3.ne.0.) c1=((a3+b3*z3)**4-(a3+b3*z1)**4)/b3
       if(b3.eq.0.) c1=4.*(z3-z1)*a33
       if(b1.ne.0.) c2=((a1+b1*z2)**4-(a1+b1*z1)**4)/b1
       if(b1.eq.0.) c2=4.*(z2-z1)*a13
       if(b2.ne.0.) c3=((a2+b2*z3)**4-(a2+b2*z2)**4)/b2
       if(b2.eq.0.) c3=4.*(z3-z2)*a23
       xi(1,2)=(c1-c2-c3)/12.
       xi(1,3)=0.5*(a32*(z32-z12)/2.+2.*a3b3*(z33-z13)/3.
     1 +b32*(z34-z14)/4.-a12*(z22-z12)/2.-2.*a1b1*(z23-z13)/3.
     2 -b12*(z24-z14)/4.-a22*(z32-z22)/2.-2.*a2b2*(z33-z23)/3.
     3 -b22*(z34-z24)/4.)
c      xi(2,3)=(1./3.)*(((a3+b3*z3)**5-(a3+b3*z1)**5)/(5.*b32)
c    1 -(a3/(4.*b32))*((a3+b3*z3)**4-(a3+b3*z1)**4)
c    2 -(0.2/b12)*((a1+b1*z2)**5-(a1+b1*z1)**5)
c    3 +(a1/(4.*b12))*((a1+b1*z2)**4-(a1+b1*z1)**4)
c    4 -(0.2/b22)*((a2+b2*z3)**5-(a2+b2*z2)**5)
c    5 +(a2/(4.*b22))*((a2+b2*z3)**4-(a2+b2*z2)**4))
       t1=a3+b3*z3
       t2=a3+b3*z1
       t3=a1+b1*z2
       t4=a1+b1*z1
       t5=a2+b2*z3
       t6=a2+b2*z2
       if(b3.eq.0.) d1=a33*(z32-z12)/2.
       if(b3.ne.0.) d1=(t1**5-t2**5)/(5.*b32)-(t1**4-t2**4)*a3/(4.*b32)
       if(b1.eq.0.) d2=a13*(z22-z12)/2.
       if(b1.ne.0.)d2=(t3**5-t4**5)/(5.*b12)-(t3**4-t4**4)*a1/(4.*b12)
       if(b2.eq.0.) d3=a23*(z32-z22)/2.
       if(b2.ne.0.)d3=(t5**5-t6**5)/(5.*b22)-(t5**4-t6**4)*a2/(4.*b22)
       xi(2,3)=(d1-d2-d3)/3.
       if(b3.eq.0.) e1=5.*a32*a32*(z3-z1)
       if(b3.ne.0.) e1=(t1**5-t2**5)/b3
       if(b1.eq.0.) e2=5.*a12*a12*(z2-z1)
       if(b1.ne.0.) e2=(t3**5-t4**5)/b1
       if(b2.eq.0.) e3=5.*a22*a22*(z3-z2)
       if(b2.ne.0.) e3=(t5**5-t6**5)/b2
       xi(2,2)=(e1-e2-e3)/20.
       xi(3,3)=0.5*(a32*(z33-z13)/3.+0.5*a3b3*(z34-z14)
     1 +b32*(z35-z15)/5.-a12*(z23-z13)/3.-0.5*a1b1*(z24-z14)
     2 -b12*(z25-z15)/5.-a22*(z33-z23)/3.-0.5*a2b2*(z34-z24)
     3 -b22*(z35-z25)/5.)
c      xi(2,2)=(((a3+b3*z3)**5-(a3+b3*z1)**5)/b3
c    1 -((a1+b1*z2)**5-(a1+b1*z1)**5)/b1
c    2 -((a2+b2*z3)**5-(a2+b2*z2)**5)/b2)/20.
       do 10 i=1,9
       do 10 j=1,9
       phij(i,j)=0.
  10   continue
       do 11 i=1,3
       do 11 j=i,3
       phij(i,j)=-xi(i,j)
c      this negative sign is required for consistent definition of
c      coordinates for stiffness and mass matrices.
  11   continue
       do 12 i=1,3
       do 12 j=i,3
       phij(j,i)=phij(i,j)
  12   continue
       do 13 i=1,3
       do 13 j=1,3
       i1=i+3
       j1=j+3
       i2=i+6
       j2=j+6
       phij(i1,j1)=phij(i,j)
       phij(i2,j2)=phij(i,j)
  13   continue
       dd=(r2-r1)*(z3-z2)-(r3-r2)*(z2-z1)
       xmi(1,1)=(r2*z3-r3*z2)/dd
       xmi(1,2)=(r3*z1-r1*z3)/dd
       xmi(1,3)=(r1*z2-r2*z1)/dd
       xmi(2,1)=(z2-z3)/dd
       xmi(2,2)=(z3-z1)/dd
       xmi(2,3)=(z1-z2)/dd
       xmi(3,1)=(r3-r2)/dd
       xmi(3,2)=(r1-r3)/dd
       xmi(3,3)=(r2-r1)/dd
       do 14 i=1,3
       do 14 j=1,3
       i1=i+3
       j1=j+3
       i2=i+6
       j2=j+6
       psi(i,j)=xmi(i,j)
       psi(i1,j1)=xmi(i,j)
       psi(i2,j2)=xmi(i,j)
  14   continue
       call transf(phij,9,9,psi,9,9,trims)
       do 15 i=1,9
       do 15 j=1,9
       trims(i,j)=trims(i,j)*rho*deln
  15   continue
       call tran(trims,trms)
       go to 31
c      above subroutine tran transforms matrix trims from dynax coord.
c      to matrix trms in mistic coordinates
c      *************************************************************
c      computations for lumped mass matrix
  30   r4=(r1+r2)/2.
       z4=(z1+z2)/2.
       r5=(r2+r3)/2.
       z5=(z2+z3)/2.
       r6=(r1+r3)/2.
       z6=(z1+z3)/2.
       r7=(r1+r2+r3)/3.
       z7=(z1+z2+z3)/3.
       r1a1=((r4-r1)*(z7-z1)-(r7-r1)*(z4-z1))*(r1+r4+r7)/6.+
     1 ((r7-r1)*(z6-z1)-(r6-r1)*(z7-z1))*(r1+r6+r7)/6.
       r2a2=((r4-r7)*(z2-z7)-(r2-r7)*(z4-z7))*(r2+r4+r7)/6.+
     1 ((r2-r7)*(z5-z7)-(r5-r7)*(z2-z7))*(r2+r5+r7)/6.
       r3a3=((r5-r7)*(z3-z7)-(r3-r7)*(z5-z7))*(r5+r7+r3)/6.+
     1 ((r3-r7)*(z6-z7)-(r6-r7)*(z3-z7))*(r7+r3+r6)/6.
        print 16,r1a1,r2a2,r3a3,rho,deln
       r1a1=r1a1*rho*deln
       r2a2=r2a2*rho*deln
       r3a3=r3a3*rho*deln
       xlmpms(1)=r1a1
       xlmpms(4)=r2a2
       xlmpms(7)=r3a3
       do 110 i=1,3
       i1=(i-1)*3+1
       i2=i1+1
       i3=i1+2
       xlmpms(i2)=xlmpms(i1)
       xlmpms(i3)=xlmpms(i1)
  110  continue
       do 32 i=1,9
       do 32 j=1,9
  32   trms(i,j)=0.
       do 33 i=1,9
  33   trms(i,i)=xlmpms(i)
  31   continue
       print 17
       print 16,(xlmpms(i),i=1,9)
       print 17
       print 16,((trms(i,j),j=1,9),i=1,9)
       print 17
       print 16,((trims(i,j),j=1,9),i=1,9)
  16   format(9(" *",1x,e11.5))
       print 17
       print 16,((phij(i,j),j=1,9),i=1,9)
       print 17
       print 16,((psi(i,j),j=1,9),i=1,9)
  17   format(  20x,"*********************************")
       return
       end
*deck tran
       subroutine tran(b,e)
       implicit real*8 (a-h,o-z)
       dimension b(9,9),e(9,9),ii(9)
       ii(1)=7
       ii(2)=4
       ii(3)=1
       ii(4)=8
       ii(5)=5
       ii(6)=2
       ii(7)=9
       ii(8)=6
       ii(9)=3
       do 21 i=1,9
       i1=ii(i)
       do 21 j=1,9
       j1=ii(j)
       e(i,j)=b(i1,j1)
  21   continue
       return
       end
*deck hex8
      subroutine hex8
      implicit real*8 (a-h,o-z)
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/dtwo/itwo,ndof,idim
      common/tapcom/nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlen
c
      common a(1)
      real a
c
      nfirst=n6e+4*itwo
      n101=nfirst
      n102=46*nummat*itwo+n101
      n103=24*nume*itwo+n102
      n104=nume+n103
      n105=24*nume+n104
      nlast=5*nummat+n105
      midest=nlast-nfirst+1
      min=nmax+nlast-n1l+1
      if(ind.eq.0)write(6,20)min
      if(min.le.nmax) go to 10
      stop 4
   10 call
     .hex8p(a(n2),a(n2l),a(n1l),a(n2),a(n3),a(n4),a(n101),a(n102),
     .a(n103),a(n104),a(n105),nume,nummat,ndof,ind,kpri,nt11)
      if(ind.eq.0)call ewrite(a(n6e),ng)
   20 format (1h0,40hstorage required to input element data =,i10)
   30 format (1h0,21herror nmax less than ,i10)
      return
      end
*deck hex8p
      subroutine hex8p(a,id,mht,x,y,z,fmat,xy,mat,lm,iprop,nume,nummat,
     . ndof,ind,kpri,nt11)
      implicit real*8 (a-h,o-z)
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat,kpplot      
      common/comtyp/itypee,ngroup,ngin,nlen
c
      common /list/ data(64),ndum
      common/switch/ipsm,ipkbar,ipinpt,ipreac,ieldat,ipsdm,ipcnpf
     . ,ibigw
c
      dimension x(1),y(1),z(1),fmat(46,1),mat(1),lm(24,1),stiff(300),
     . xmass(300),damp(300),st(6,24),fk(24,24),fc(24,24),fm(24,24),
     . id(ndof,1),mht(1),iprop(5,1),c(21),ctil(21),d(6,6),dtil(6,6)
     .,xy(3,8,1),nonum(8)
      dimension ur(24),ui(24),sigr(6),sigi(6)
c
      if (ind.ne.0) go to 200
c
c         read and store material data
c
      do 115 m=1,nummat
      call fld
      do 110 n=1,46
  110 fmat(n,m)=data(n+5)
c *** print mat properties ****
      write(6,222)m
  222 format(1x//,1x,30hhex8 properties for mat number,i3//)
      write(6,223)(fmat(in,m),in=1,46)
  223 format(1x,5e12.5)
      do 115 n=1,5
  115 iprop(n,m)=data(n)
c
c         read and store element information
c
      if(ibigw.eq.0)go to 146
      if(ieldat.eq.0)go to 146
      write(6,140)
  146 continue
      do 150 n=1,nume
      call fld
      num=data(1)
      if (num.ne.n) stop 5
      mat(n)=data(2)
      do 135 i=1,8
      node=data(i+2)
      nonum(i)=node
      lm(3*i-2,n)=id(1,node)
      lm(3*i-1,n)=id(2,node)
      lm(3*i,n)=id(3,node)
      xy(1,i,n)=x(node)
      xy(2,i,n)=y(node)
  135 xy(3,i,n)=z(node)
c
      call colht(mht,24,lm(1,n))
      call eselm2(mat(n),nonum,8)
      if(ibigw.eq.0)go to 150
      if(ieldat.eq.0)go to 150
c
      write(6,145) n,mat(n),(nonum(i),i=1,8)
  140 format(///10x,12helement data,/,2x,3helm,2x,3hmat,2x,5hnode1,
     . 2x,5hnode2,2x,5hnode3,2x,5hnode4,2x,5hnode5,2x,5hnode6
     .,2x,5hnode7,2x,5hnode8//)
  145 format(2i5,8i7)
c
  150 continue
c
      return
c
c         ind=1 pass to form element matrices
c
  200 if(ind.ne.1 .and. ind.ne.3) go to 300
      if(ind.eq.3.and.kpstre.ne.0)write(6,390)
  390 format(1x,34hcomplex stresses for hex8 elements,//,1x,5h  elm,
     . 2(3x,8hsigma-xx),2(3x,8hsigma-yy),2(3x,8hsigma-zz)/,6x,
     . 3(5x,4hreal,7x,4himag,2x),/,6x,
     . 2(4x,6htau-xy),2(4x,6htau-yz),2(4x,6htau-xz),/,5x,
     . 3(5x,4hreal,7x,4himag,2x),//)
      do 280 n=1,nume
      icode=0
      if (ind.eq.1) call echeck(lm(1,n),24,icode)
      if (icode.eq.1) go to 280
      m=mat(n)
      itype=iprop(1,m)
      nint=iprop(2,m)
      ilumps=iprop(3,m)
      idamps=iprop(4,m)
      istres=iprop(5,m)
      if(ind.eq.3.and.istres.eq.0)go to 280
      if(kpri.eq.3.and.idamps.eq.0)go to 280
      e=fmat(1,m)
      pr=fmat(2,m)
      fnu1=fmat(3,m)
      rho=fmat(4,m)
      do 210 i=1,21
      c(i)=fmat(4+i,m)
  210 ctil(i)=fmat(25+i,m)
      nstr=6
      if(itype.eq.3)go to 211
      do 12 i=1,21
   12 c(i)=0.0
      f=e/(1.0+pr)
      g=f*pr/(1.0-2.0*pr)
      h=f+g
      c(1)=h
      c(2)=g
      c(3)=g
      c(7)=h
      c(8)=g
      c(12)=h
      c(16)=f/2.0
      c(19)=f/2.0
      c(21)=c(16)
c
c         fill in full 4x4 stress strain matrix
c
  211 ic=0
      do 212 i=1,6
      do 212 j=i,6
      ic=ic+1
      d(i,j)=c(ic)
      d(j,i)=c(ic)
      dtil(i,j)=ctil(ic)
  212 dtil(j,i)=ctil(ic)
      kprip=kpri
      if(ind.eq.3)kprip=4
      call sghex1(n,nstr,nint,e,pr,xy(1,1,n),d,dtil,fnu1,
     . rho,ilumps,kprip,st,fk,fc,fm)
      if(ind.eq.3) go to 255
c
      m=0
      do 220 i=1,24
      do 220 j=i,24
      m=m+1
      stiff(m)=fk(i,j)
      damp(m)=fc(i,j)
  220 xmass(m)=fm(i,j)
c      print of element stiff damp and mass matrices and pointer array
      if(ibigw.eq.0)go to 950
      if(ipsdm.ne.1)go to 950
      write(6,951)n
  951 format(1h0,22hprint lm(i,n) array,n= ,i5//)
      write(6,952)(lm(ii,n),ii=1,24)
  952 format(1x,3(8i6,/,1x),//)
      if(kpri.ne.1)go to 702
      write(6,956)n
  956 format(1x//////,30h stiffness for hex8 element n= ,i5//)
      write(6,953)((fk(ii,jj),jj=1,24),ii=1,24)
  953 format(1x,3(8e13.5,/,1x))
  702 if(kpri.ne.3)go to 703
      write(6,957)n
  957 format(1x,//////,35h damping matrix for hex8 element n= ,i5//)
      write(6,953)((fc(ii,jj),jj=1,24),ii=1,24)
  703 if(kpri.ne.2)go to 950
      write(6,960)n
  960 format(1x,//////,32h mass matrix for hex8 element n= ,i5//)
      write(6,953)((fm(ii,jj),jj=1,24),ii=1,24)
  950 continue
c
      go to (230,240,250), kpri
c
c         assemble stiffness matrix
c
  230 call addban(a,mht,stiff,lm(1,n),24)
      go to 280
c
c         assemble mass matrix
c
  240 call addban(a,mht,xmass,lm(1,n),24)
      go to 280
c
c         assemble damping matrix
c
  250 call addban(a,mht,damp,lm(1,n),24)
      go to 280
c
c    ind =3 pass
c
  255 do 320 i=1,24
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
      do 340 j=1,24
      sigr(i)=sigr(i)+st(i,j)*ur(j)
  340 sigi(i)=sigi(i)+st(i,j)*ui(j)
c
c      print stresses
c
      if(kpstre.eq.0)go to 270
      write(6,360)n,(sigr(i),sigi(i),i=1,nstr)
  360 format(i5,2(1p6e11.3,/,6x))
c
  270 continue
      if(kpplot.eq.0)go to 280
      call esstr(sigr,nstr)
  280 continue
      return
c
c      ind=2 and ind=4 passes
c
  300 if(ind.ne.4)return
      do 315 n=1,nume
      call echeck(lm(1,n),24,icode)
      if(icode)315,310,315
  310 continue
      ngin=1
      return
  315 continue
      return
      end
*deck sghex1
      subroutine sghex1(nel,ist,nint,e,pr,xx,d,dtil,fnu1,
     1rho,ilumps,kpri,st,fk,fc,fm)
      implicit real*8 (a-h,o-z)
c
c..............................................................
c.
c.    p r o g r a m
c.    to calculate isoparametric 8-node solid element stiffness
c.    mass and damping matrices for isotropic and anisotropic
c.    materials
c.
c.- - input variables - -
c.      nel       = number of element
c.      itype     = element type
c.     eq.3 = input anisotropic material and
c.     damping matrices
c. c(21)   = input material property matrix
c.      ctil(21) = input damping matrix
c.      fnu1      = damping constant
c.      idamps    = switch for damping matrix: 0-no damping matrix
c.   1-damping matrix
c.      nint      = gauss numerical integration order
c.      e        = young"s modulus
c.      pr        = poisson"s ratio
c.      xx(3,8)   = element node coordinates
c.      fk(24,24)    = stiffness matrix on solution exit
c. istres = switch controling stress calculations: 0-no stress;
c.     1-stress calclulation
c. ilumps = switch controling lumped mass or consistent mass
c.   calculation: 0-consistent mass; 1-lumped mass
c.
c.- - output - -
c.      fk(24,24)    = stiffness matrix
c.      fm(24,24)   = mass matrix
c.      fc(24,24)    = damping matrix
c. st(6,24) = stress transformation matrix
c......................................................................
      dimension d(6,6),b(6,24),xx(3,8),fk(24,24),xg(4,4),wgt(4,4),db(6)
      dimension hh(3,24),fm(24,24)
      dimension dtil(6,6),st(6,24),fc(24,24),dtilb(6)
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
      go to (230,240,230,260)kpri
  240 continue
      do 33 i=1,24
      do 33 j=1,24
   33 fm(i,j)=0.0
      if(ilumps.eq.1)go to 86
  230 continue
      if (kpri.eq.2) go to 76
c
c.    c a l c u l a t e  e l e m e n t  s t i f f n e s s ,
c.    e l e m e n t  m a s s ,  a n d  e l e m e n t
c.    d a m p i n g
c.
  20  do 30 i=1,24
      do 30 j=1,24
      fc(i,j)=0.0
      fk(i,j)=0.0
   30 continue
   76 continue
      do 80 lx=1,nint
      ri=xg(lx,nint)
      do 80 ly=1,nint
      si=xg(ly,nint)
      do 80 lz=1,nint
      ti=xg(lz,nint)
c
c     evaluate deivative operator b and the jacobian determinant det
c     evaluate the displacement interpolation matrix h
c
      call hxstom(xx,b,det,hh,ri,si,ti,nel)
c
c     add contribution to element stiffness and mass
c
      wt=wgt(lx,nint)*wgt(ly,nint)*wgt(lz,nint)*det
      if(kpri.eq.2) go to 72
      do 70 j=1,24
      do 40 k=1,ist
      dtilb(k)=0.0
      db(k)=0.0
      do 40 l=1,ist
      dtilb(k)=dtilb(k) + dtil(k,l)*b(l,j)
  40  db(k)=db(k)+d(k,l)*b(l,j)
      do 60 i=j,24
      damp=0.0
      stiff=0.0
      do 50 l=1,ist
      damp=damp + b(l,i)*dtilb(l)
      stiff=stiff+b(l,i)*db(l)
   50 continue
      fc(i,j)=fc(i,j) + wt*(damp+stiff*fnu1)
      fk(i,j)=fk(i,j) +stiff*wt
   60 continue
  70  continue
      if(kpri.eq.1)go to 80
   72 continue
      do 27 j=1,24
      do 27 i=j,24
      fmass=0.0
      do 17 l=1,3
   17 fmass=fmass + hh(l,i)*hh(l,j)
   27 fm(i,j)=fm(i,j) + fmass*rho*wt
  80  continue
c
      do 90 j=1,24
      do 90 i=j,24
      fc(j,i)=fc(i,j)
      fm(j,i)=fm(i,j)
  90  fk(j,i)=fk(i,j)
      return
c.
c. c a l c u l a t i o n  o f  t h e  l u m p e d  m a s s  m a t r i x
c.
   86 continue
      vol=1./6.*(det4(1.0,xx(1,1),xx(2,1),xx(3,1),1.0,xx(1,2),xx(2,2),
     1xx(3,2),1.0,xx(1,3),xx(2,3),xx(3,3),1.0,xx(1,6),xx(2,6),xx(3,6))
     2 +         det4(1.0,xx(1,1),xx(2,1),xx(3,1),1.0,xx(1,3),xx(2,3),
     3xx(3,3),1.0,xx(1,4),xx(2,4),xx(3,4),1.0,xx(1,8),xx(2,8),xx(3,8))
     4 +         det4(1.0,xx(1,1),xx(2,1),xx(3,1),1.0,xx(1,3),xx(2,3),
     5xx(3,3),1.0,xx(1,8),xx(2,8),xx(3,8),1.0,xx(1,6),xx(2,6),xx(3,6))
     6 +         det4(1.0,xx(1,1),xx(2,1),xx(3,1),1.0,xx(1,6),xx(2,6),
     7xx(3,6),1.0,xx(1,8),xx(2,8),xx(3,8),1.0,xx(1,5),xx(2,5),xx(3,5))
     8 +         det4(1.0,xx(1,3),xx(2,3),xx(3,3),1.0,xx(1,7),xx(2,7),
     9xx(3,7),1.0,xx(1,8),xx(2,8),xx(3,8),1.0,xx(1,6),xx(2,6),xx(3,6)))
      w=vol*rho
      do 89 i=1,24
   89 fm(i,i)=w/8.0
c.
c. c a l c u l a t i o n  o f  t h e  s t r e s s  t r a n s f o r m -
c. a t i o n  m a t r i x
c.
  260 continue
      do 411 i=1,6
      do 411 j=1,24
  411 st(i,j)=0.0
      ri=0.0
      si=0.0
      ti=0.0
      call hxstom(xx,b,det,hh,ri,si,ti,nel)
      do 413 i=1,6
      do 413 j=1,24
      do 413 l=1,6
  413 st(i,j)=st(i,j) + d(i,l)*b(l,j)
      return
c
      end
*deck hxstom
      subroutine hxstom(xx,b,det,hh,r,s,t,nel)
      implicit real*8 (a-h,o-z)
c
c.................................................................
c.
c.    p r o g r a m
c.        to evaluate the strain-displacement transformation matrix
c.   b at point (r,s,t) for an eight-node solid element
c.
c..................................................................
      dimension xx(3,8),hh(3,24),b(6,24),h(8),p(3,8),xj(3,3),xji(3,3)
      data iwrit/0/
c
      rp= 1.0 + r
      sp= 1.0 + s
      rm= 1.0 - r
      sm= 1.0 - s
      tp= 1.0 + t
      tm= 1.0 - t
c
c     interpolation functions
c
      h(1)= .125*rp*sp*tm
      h(2)= .125*rm*sp*tm
      h(3)= .125*rm*sm*tm
      h(4)= .125*rp*sm*tm
      h(5)= .125*rp*sp*tp
      h(6)= .125*rm*sp*tp
      h(7)= .125*rm*sm*tp
      h(8)= .125*rp*sm*tp
c
c     displacement interpolation matrix for calculation of mass matrix
c
      do 45 i=1,3
      do 45 j=1,24
   45 hh(i,j)=0.0
      do 35 j=1,8
      hh(1,3*j-2)=h(j)
      hh(2,3*j-1)=h(j)
   35 hh(3,3*j)=h(j)
c
c     natural coordinate derivatives of the interpolation functions
c
c         1. with respect to r
c
      p(1,1)=.125*sp*tm
      p(1,2)= -p(1,1)
      p(1,3)= -.125*sm*tm
      p(1,4)= -p(1,3)
      p(1,5)=.125*sp*tp
      p(1,6)= -p(1,5)
      p(1,7)= -.125*sm*tp
      p(1,8)= -p(1,7)
c.
c.        2. with respect to s
c.
      p(2,1)=.125*rp*tm
      p(2,2)=.125*rm*tm
      p(2,3)= -p(2,2)
      p(2,4)= -p(2,1)
      p(2,5)=.125*rp*tp
      p(2,6)=.125*rm*tp
      p(2,7)= -p(2,6)
      p(2,8)= -p(2,5)
c.
c.        3. with respect to t
c.
      p(3,1)=-.125*rp*sp
      p(3,2)=-.125*rm*sp
      p(3,3)=-.125*rm*sm
      p(3,4)=-.125*rp*sm
      p(3,5)= -p(3,1)
      p(3,6)= -p(3,2)
      p(3,7)= -p(3,3)
      p(3,8)= -p(3,4)
c
c     evaluate the jacobian matrix at the point (r,s,t)
c
  10  do 30 i=1,3
      do 30 j=1,3
      dum=0.0
      do 20 k=1,8
  20  dum=dum+p(i,k)*xx(j,k)
  30  xj(i,j)=dum
c
c     compute the determinant of the jacobian matrix at point (r,s,t)
c
      if(iwrit.eq.1)
     .write(6,120)((xj(i,j),j=1,3),i=1,3)
  120 format(1x,"xj=",/,1x,3(3(1pe12.5,1x),/,1x))
c
c     compute inverse jacobian matrix
c
      xji(1,1)=xj(2,2)*xj(3,3)-xj(2,3)*xj(3,2)
      xji(2,1)=-xj(2,1)*xj(3,3)+xj(2,3)*xj(3,1)
      xji(3,1)=xj(2,1)*xj(3,2)-xj(2,2)*xj(3,1)
      xji(1,2)=-xj(1,2)*xj(3,3)+xj(1,3)*xj(3,2)
      xji(2,2)=xj(1,1)*xj(3,3)-xj(1,3)*xj(3,1)
      xji(3,2)=-xj(1,1)*xj(3,2)+xj(1,2)*xj(3,1)
      xji(1,3)=xj(1,2)*xj(2,3)-xj(1,3)*xj(2,2)
      xji(2,3)=-xj(1,1)*xj(2,3)+xj(2,1)*xj(1,3)
      xji(3,3)=xj(1,1)*xj(2,2)-xj(1,2)*xj(2,1)
      det=xj(1,1)*xji(1,1)+xj(1,2)*xji(2,1)+xj(1,3)*xji(3,1)
      if(det.gt.0.000000001)go to 40
      write(6,2000)nel,det
      write(6,120)((xj(i,j),j=1,3),i=1,3)
      stop
   40 do 13 i=1,3
      do 13 j=1,3
   13 xji(i,j)=xji(i,j)/det
c
c     evaluate global derivative operator b
c
      do 65 i=1,6
      do 65 j=1,24
   65 b(i,j)=0.0
      do 60 k=1,8
      do 50 i=1,3
      b(1,3*k-2)=b(1,3*k-2)+xji(1,i)*p(i,k)
      b(2,3*k-1)=b(2,3*k-1)+xji(2,i)*p(i,k)
   50 b(3,3*k)=b(3,3*k)+xji(3,i)*p(i,k)
      b(4,3*k-2)=b(2,3*k-1)
      b(4,3*k-1)=b(1,3*k-2)
      b(5,3*k-1)=b(3,3*k)
      b(5,3*k)=b(2,3*k-1)
      b(6,3*k-2)=b(3,3*k)
   60 b(6,3*k)=b(1,3*k-2)
      if(iwrit.eq.0)return
      write(6,109)((b(i,j),j=1,24),i=1,6)
  109 format(1x,"b(i,j)=",/,1x,18(8(1pe12.5,1x),/,1x))
      write(6,111)((hh(i,j),j=1,24),i=1,3)
  111 format(1x,"hh(i,j)=",/,1x,9(8(1pe12.5,1x),/,1x))
      return
c
 2000 format (1h1,"error:",1x,
     1"zero or negative jacobian det. for element ",i6,
     2 " det ",1pe16.8)
c
      end
*deck phex
      subroutine phex
      implicit real*8 (a-h,o-z)
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/dtwo/itwo,ndof,idim
      common/tapcom/nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlen
c
      common a(1)
      real a
c
      nfirst=n6e+4*itwo
      n101=nfirst
      n102=3*nummat*itwo+n101
      n103=24*nume*itwo+n102
      n104=nume+n103
      n105=8*nume+n104
      nlast=3*nummat+n105
      midest=nlast-nfirst+1
      min=nmax+nlast-n1l+1
      if(ind.eq.0)write(6,20)min
      if(min.le.nmax) go to 10
      stop 4
   10 call
     .phexp(a(n2),a(n2l),a(n1l),a(n2),a(n3),a(n4),a(n101),a(n102),
     .a(n103),a(n104),a(n105),nume,nummat,ndof,ind,kpri,nt11)
      if(ind.eq.0)call ewrite(a(n6e),ng)
   20 format (1h0,40hstorage required to input element data =,i10)
   30 format (1h0,21herror nmax less than ,i10)
      return
      end
*deck phexp
      subroutine phexp(a,id,mht,x,y,z,fmat,xy,mat,lm,iprop,nume,nummat,
     . ndof,ind,kpri,nt11)
      implicit real*8 (a-h,o-z)
      common/comtyp/itypee,ngroup,ngin,nlen
      common/fsicom/fsifac,wfsi,kfsi
c
      common /list/ data(64),ndum
      common/switch/ipsm,ipkbar,ipinpt,ipreac,ieldat,ipsdm,ipcnpf
     . ,ibigw
c
      dimension x(1),y(1),z(1),fmat(3,1),mat(1),lm(8,1),stiff(36),
     . xmass(36),damp(36),fc(8,8),fk(8,8),fm(8,8),
     . id(ndof,1),mht(1),iprop(3,1),d(6,6)
     .,xy(3,8,1),nonum(8)
      dimension ur(8),ui(8)
c
      if (ind.ne.0) go to 200
c
c         read and store material data
c
      do 115 m=1,nummat
      call fld
      do 110 n=1,3
  110 fmat(n,m)=data(n+3)
c *** print mat properties ****
      write(6,222)m
  222 format(1x//,1x,30hhex8 properties for mat number,i3//)
      write(6,223)(fmat(in,m),in=1,3)
  223 format(1x,3e12.5)
      do 115 n=1,3
  115 iprop(n,m)=data(n)
c
c         read and store element information
c
      if(ibigw.eq.0)go to 146
      if(ieldat.eq.0)go to 146
      write(6,140)
  146 continue
      do 150 n=1,nume
      call fld
      num=data(1)
      if (num.ne.n) stop 5
      mat(n)=data(2)
      do 135 i=1,8
      node=data(i+2)
      nonum(i)=node
      lm(i,n)=id(1,node)
      xy(1,i,n)=x(node)
      xy(2,i,n)=y(node)
  135 xy(3,i,n)=z(node)
c
      call colht(mht,8,lm(1,n))
      if(ibigw.eq.0)go to 150
      if(ieldat.eq.0)go to 150
c
      write(6,145) n,mat(n),(nonum(i),i=1,8)
  140 format(///10x,12helement data,/,2x,3helm,2x,3hmat,2x,5hnode1,
     . 2x,5hnode2,2x,5hnode3,2x,5hnode4,2x,5hnode5,2x,5hnode6
     .,2x,5hnode7,2x,5hnode8//)
  145 format(2i5,8i7)
c
  150 continue
c
      return
c
c         ind=1 pass to form element matrices
c
  200 if(ind.ne.1.and.ind.ne.3.and.ind.ne.6)go to 300
      if(ind.eq.1.and.kfsi.eq.2)return
      if(ind.eq.3)return
      do 280 n=1,nume
      icode=0
      if (ind.eq.1) call echeck(lm(1,n),8,icode)
      if (icode.eq.1) go to 280
      m=mat(n)
      nint=iprop(1,m)
      ilumps=iprop(2,m)
      idamps=iprop(3,m)
      if(kpri.eq.3.and.idamps.eq.0)go to 280
      rho=fmat(1,m)
      fnu1=fmat(2,m)
c         fkfl= fluid bulk modulus
      fkfl=fmat(3,m)
      nstr=6
c
c     o b t a i n  s t r e s s - s t r a i n  l a w
c
      do 11 i=4,6
      do 11 j=4,6
   11 d(i,j)=0.0
      do 29 i=1,6
   29 d(i,i)=fkfl
      d(1,2)=-d(1,1)
      d(1,3)=-d(1,1)
      d(2,1)=-d(1,1)
      d(2,3)=-d(1,1)
      d(3,1)=-d(1,1)
      d(3,2)=-d(1,1)
      kprip=kpri
      call prhex1(n,nstr,nint,xy(1,1,n),d,
     . rho,ilumps,fnu1,kprip,fk,fc,fm)
c
      m=0
      do 220 i=1,8
      do 220 j=i,8
      m=m+1
      stiff(m)=fk(i,j)
      damp(m)=fc(i,j)
  220 xmass(m)=fm(i,j)
c   print of element stiff and mass matrices and pointer array
      if(ibigw.eq.0)go to 950
      if(ipsdm.ne.1)go to 950
      write(6,951)n
  951 format(1h0,22hprint lm(i,n) array,n= ,i5//)
      write(6,952)(lm(ii,n),ii=1,8)
  952 format(1x,1(8i6,/,1x),//)
      if(kpri.ne.1)go to 702
      write(6,956)n
  956 format(1x//////,30h stiffness for phex element n= ,i5//)
      write(6,953)((fk(ii,jj),jj=1,8),ii=1,8)
  953 format(1x,1(8e13.5,/,1x))
  702 if(kpri.ne.3)go to 703
      write(6,954)n
      write(6,953)((fc(ii,jj),jj=1,8),ii=1,8)
  954 format(1x//////,28hdamping for phex element n= ,i5//)
  703 if(kpri.ne.2)go to 950
      write(6,960)n
  960 format(1x,//////,32h mass matrix for phex element n= ,i5//)
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
c assemble damping matrix
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
      ur(i)=0.0
      ui(i)=0.0
      if(idof.eq.0) go to 320
      ur(i)=x(idof)
      ui(i)=y(idof)
  320 continue
  280 continue
      return
c
c      ind=2 and ind=4 passes
c
  300 if(ind.ne.4)return
      do 315 n=1,nume
      call echeck(lm(1,n),8,icode)
      if(icode)315,310,315
  310 continue
      ngin=1
      return
  315 continue
      return
      end
*deck prhex1
      subroutine prhex1(nel,ist,nint,xx,d,rho,ilumps,
     1fnu1,kpri,fk,fc,fm)
      implicit real*8 (a-h,o-z)
c
c..............................................................
c.
c.    p r o g r a m
c.    to calculate the stiffness and mass matrices for an
c. 8-node solid element
c.
c.- - input variables - -
c.      nel       = number of element
c.      nint      = gauss numerical integration order
c. ilumps   = switch controlling the lumped mass matrix
c.   calculation: 0-consistant mass matrix;
c.         1-lumped mass matrix
c.      xx(3,8)   = element node coordinates
c.      fk(8,8)    = stiffness matrix on solution exit
c.
c.- - output - -
c.      fk(8,8)    = stiffness matrix
c.      fm(8,8)   = mass matrix
c......................................................................
      dimension d(6,6),b(6,8),xx(3,8),fk(8,8),xg(4,4),wgt(4,4),db(6)
      dimension h(8),fm(8,8),fc(8,8)
      data iwrits/0/
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
c
c.    c a l c u l a t e  e l e m e n t  s t i f f n e s s
c.    a n d  e l e m e n t  m a s s
c.
      go to (230,240,230)kpri
240   continue
      do 30 i=1,8
      do 30 j=1,8
   30 fm(i,j)=0.0
      if (ilumps.eq.1)go to 249
      go to 19
  230 do 31 i=1,8
      do 31 j=1,8
      fc(i,j)=0.0
   31 fk(i,j)=0.0
   19 continue
      do 80 lx=1,nint
      ri=xg(lx,nint)
      do 80 ly=1,nint
      si=xg(ly,nint)
      do 80 lz=1,nint
      ti=xg(lz,nint)
c
c     evaluate derivative operator b and the jacobian determinant det
c     evaluate the displacement interpolation matrix h
c
      call prxstom(xx,b,det,h,ri,si,ti,nel)
c
c     add contribution to element stiffness and mass
c
      wt=wgt(lx,nint)*wgt(ly,nint)*wgt(lz,nint)*det
      if(kpri.eq.2)go to 79
      do 70 j=1,8
      do 40 k=1,ist
      db(k)=0.0
      do 40 l=1,ist
  40  db(k)=db(k)+d(k,l)*b(l,j)
      if(iwrits.eq.0)go to 713
      write(6,59)((d(m,n),m=1,6),n=1,6)
   59 format(1x,"d(i,j)=",/,1x,6(6(e12.4,1x)/,1x))
      write(6,55)(db(k),k=1,6)
   55 format(1x,"db(k)=",/,1x,6(1pe12.5,/,1x))
  713 continue
      do 60 i=j,8
      stiff=0.0
      do 50 l=1,ist
      stiff=stiff+b(l,i)*db(l)
   50 continue
      fk(i,j)=fk(i,j) +stiff*wt
      fc(i,j)=fc(i,j) + fnu1*stiff*wt
   60 continue
  70  continue
      if(kpri.eq.1)go to 80
   79 continue
      do 27 j=1,8
      do 27 i=j,8
   27 fm(i,j)=fm(i,j) + h(i)*h(j)*wt*rho
   80 continue
c
      do 90 j=1,8
      do 90 i=j,8
      fm(j,i)=fm(i,j)
      fc(j,i)=fc(i,j)
  90  fk(j,i)=fk(i,j)
      return
  249 continue
c.
c. c a l c u l a t i o n  o f  t h e  l u m p e d  m a s s  m a t r i x
c.
      vol=1./6.*(det4(1.0,xx(1,1),xx(2,1),xx(3,1),1.0,xx(1,2),xx(2,2),
     1xx(3,2),1.0,xx(1,3),xx(2,3),xx(3,3),1.0,xx(1,6),xx(2,6),xx(3,6))
     2 +         det4(1.0,xx(1,1),xx(2,1),xx(3,1),1.0,xx(1,3),xx(2,3),
     3xx(3,3),1.0,xx(1,4),xx(2,4),xx(3,4),1.0,xx(1,8),xx(2,8),xx(3,8))
     4 +         det4(1.0,xx(1,1),xx(2,1),xx(3,1),1.0,xx(1,3),xx(2,3),
     5xx(3,3),1.0,xx(1,8),xx(2,8),xx(3,8),1.0,xx(1,6),xx(2,6),xx(3,6))
     6 +         det4(1.0,xx(1,1),xx(2,1),xx(3,1),1.0,xx(1,6),xx(2,6),
     7xx(3,6),1.0,xx(1,8),xx(2,8),xx(3,8),1.0,xx(1,5),xx(2,5),xx(3,5))
     8 +         det4(1.0,xx(1,3),xx(2,3),xx(3,3),1.0,xx(1,7),xx(2,7),
     9xx(3,7),1.0,xx(1,8),xx(2,8),xx(3,8),1.0,xx(1,6),xx(2,6),xx(3,6)))
      w=vol*rho
      do 89 i=1,8
   89 fm(i,i)=w/8.0
      return
c
      end
*deck prxstom
      subroutine prxstom(xx,b,det,h,r,s,t,nel)
      implicit real*8 (a-h,o-z)
c
c.................................................................
c.
c.    p r o g r a m
c.        to evaluate the strain-displacement transformation matrix
c.   b at point (r,s,t) for an eight-node solid element
c.
c..................................................................
      dimension xx(3,8),b(6,8),h(8),p(3,8),xj(3,3),xji(3,3)
      data iwrits/0/
c
      rp= 1.0 + r
      sp= 1.0 + s
      rm= 1.0 - r
      sm= 1.0 - s
      tp= 1.0 + t
      tm= 1.0 - t
c
c     interpolation functions
c
      h(1)= .125*rp*sp*tm
      h(2)= .125*rm*sp*tm
      h(3)= .125*rm*sm*tm
      h(4)= .125*rp*sm*tm
      h(5)= .125*rp*sp*tp
      h(6)= .125*rm*sp*tp
      h(7)= .125*rm*sm*tp
      h(8)= .125*rp*sm*tp
c
c     natural coordinate derivatives of the interpolation functions
c
c         1. with respect to r
c
      p(1,1)=.125*sp*tm
      p(1,2)= -p(1,1)
      p(1,3)= -.125*sm*tm
      p(1,4)= -p(1,3)
      p(1,5)=.125*sp*tp
      p(1,6)= -p(1,5)
      p(1,7)= -.125*sm*tp
      p(1,8)= -p(1,7)
c.
c.        2. with respect to s
c.
      p(2,1)=.125*rp*tm
      p(2,2)=.125*rm*tm
      p(2,3)= -p(2,2)
      p(2,4)= -p(2,1)
      p(2,5)=.125*rp*tp
      p(2,6)=.125*rm*tp
      p(2,7)= -p(2,6)
      p(2,8)= -p(2,5)
c.
c.        3. with respect to t
c.
      p(3,1)=-.125*rp*sp
      p(3,2)=-.125*rm*sp
      p(3,3)=-.125*rm*sm
      p(3,4)=-.125*rp*sm
      p(3,5)= -p(3,1)
      p(3,6)= -p(3,2)
      p(3,7)= -p(3,3)
      p(3,8)= -p(3,4)
c
c     evaluate the jacobian matrix at the point (r,s,t)
c
  10  do 30 i=1,3
      do 30 j=1,3
      dum=0.0
      do 20 k=1,8
  20  dum=dum+p(i,k)*xx(j,k)
  30  xj(i,j)=dum
c
c     compute the determinant of the jacobian matrix at point (r,s,t)
c
      if(iwrits.eq.0)go to 714
      write(6,120)((xj(i,j),j=1,3),i=1,3)
  120 format(1x,"xj=",/,1x,3(3(1pe12.5,1x),/,1x))
  714 continue
c
c     compute inverse jacobian matrix
c
      xji(1,1)=xj(2,2)*xj(3,3)-xj(2,3)*xj(3,2)
      xji(2,1)=-xj(2,1)*xj(3,3)+xj(2,3)*xj(3,1)
      xji(3,1)=xj(2,1)*xj(3,2)-xj(2,2)*xj(3,1)
      xji(1,2)=-xj(1,2)*xj(3,3)+xj(1,3)*xj(3,2)
      xji(2,2)=xj(1,1)*xj(3,3)-xj(1,3)*xj(3,1)
      xji(3,2)=-xj(1,1)*xj(3,2)+xj(1,2)*xj(3,1)
      xji(1,3)=xj(1,2)*xj(2,3)-xj(1,3)*xj(2,2)
      xji(2,3)=-xj(1,1)*xj(2,3)+xj(2,1)*xj(1,3)
      xji(3,3)=xj(1,1)*xj(2,2)-xj(1,2)*xj(2,1)
      det=xj(1,1)*xji(1,1)+xj(1,2)*xji(2,1)+xj(1,3)*xji(3,1)
      if(iwrits.eq.0)go to 715
      write(6,130)det
  130 format(1x,"det=",1x,1pe12.5)
  715 continue
      if(det.gt.0.000000001)go to 40
      write(6,2000)nel
      stop
   40 do 13 i=1,3
      do 13 j=1,3
   13 xji(i,j)=xji(i,j)/det
c
c     evaluate global derivative operator b
c
      do 65 i=1,6
      do 65 j=1,8
   65 b(i,j)=0.0
      do 60 k=1,8
      do 60 i=1,3
      b(1,k)=b(1,k)+xji(1,i)*p(i,k)
      b(4,k)=b(4,k)+xji(2,i)*p(i,k)
   60 b(6,k)=b(6,k)+xji(3,i)*p(i,k)
      if(iwrits.eq.0)go to 716
      write(6,109)((b(i,j),j=1,8),i=1,6)
  109 format(1x,"b(i,j)=",/,1x,18(8(1pe12.5,1x),/,1x))
  716 continue
      return
c
 2000 format (1h1,"error;",/,1x,
     1"zero or negative jacobian det. for element ",i6)
c
c
      end
c.
c.
c......................................................................
c.
c. subroutine to calculate the determinant of a 4 x 4 matrix
c.
c......................................................................
*deck det4
      function det4(a11,a12,a13,a14,a21,a22,a23,a24,a31,a32,a33,a34,
     1a41,a42,a43,a44)
      implicit real*8 (a-h,o-z)
      det4=a11*det3(a22,a23,a24,a32,a33,a34,a42,a43,a44) -
     1 a12*det3(a21,a23,a24,a31,a33,a34,a41,a43,a44) +
     2 a13*det3(a21,a22,a24,a31,a32,a34,a41,a42,a44) -
     3 a14*det3(a21,a22,a23,a31,a32,a33,a41,a42,a43)
      if(det4.ge.0.0)return
      write(6,100)
  100 format(1x,"negative determinat in the calculation of the"
     1,/,1x,"volume of hexahedron. check entering order of grid points")
      return
      end
c.
c......................................................................
c.
c. subroutine to calculate the determinant of a 3 x 3 matrix
c......................................................................
*deck det3
      function det3(a11,a12,a13,a21,a22,a23,a31,a32,a33)
      implicit real*8 (a-h,o-z)
      det3=a11*(a22*a33-a23*a32) - a12*(a21*a33-a23*a31) +
     1 a13*(a21*a32-a22*a31)
      return
      end
*deck pquad4
      subroutine pquad4
      implicit real*8 (a-h,o-z)
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/dtwo/itwo,ndof,idim
      common/tapcom/nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlen
c
      common a(1)
      real a
c
      nfirst=n6e+4*itwo
      n101=nfirst
      n102=3*nummat*itwo+n101
      n103=8*nume*itwo+n102
      n104=nume+n103
      n105=4*nume+n104
      nlast=3*nummat+n105
      midest=nlast-nfirst+1
      min=nmax+nlast-n1l+1
      if(ind.eq.0)write(6,20)min
      if(min.le.nmax) go to 10
      stop 4
   10 call
     .pquad4p(a(n2),a(n2l),a(n1l),a(n2),a(n3),a(n101),a(n102),a(n103),
     .a(n104),a(n105),nume,nummat,ndof,ind,kpri,nt11)
      if(ind.eq.0)call ewrite(a(n6e),ng)
   20 format (1h0,40hstorage required to input element data =,i10)
   30 format (1h0,21herror nmax less than ,i10)
      return
      end
*deck pquad4p
      subroutine pquad4p(a,id,mht,x,y,fmat,xy,mat,lm,iprop,nume,nummat,
     . ndof,ind,kpri,nt11)
      implicit real*8 (a-h,o-z)
      common/comtyp/itypee,ngroup,ngin,nlen
      common/fsicom/fsifac,wfsi,kfsi
c
      common /list/ data(64),ndum
      common/switch/ipsm,ipkbar,ipinpt,ipreac,ieldat,ipsdm,ipcnpf
     . ,ibigw
c
      dimension x(1),y(1),fmat(3,1),mat(1),lm(4,1),stiff(10),xmass(10),
     . fk(4,4),fm(4,4),xy(2,4,1),nonum(4),
     . id(ndof,1),mht(1),iprop(3,1)
      dimension ur(4),ui(4)
c
      if (ind.ne.0) go to 200
c
c         read and store material data
c
      do 115 m=1,nummat
      call fld
      do 110 n=1,3
  110 fmat(n,m)=data(n+3)
c *** print mat properties ****
      write(6,222)m
  222 format(1x//31hpqdm  properties for mat number,i3//)
      write(6,223)(fmat(in,m),in=1,3)
  223 format(1x,"rhofl=",e12.4,2x,"damp factor=",e12.4,2x,
     $ "fl bulk mod=",e12.4)
      do 115 n=1,3
  115 iprop(n,m)=data(n)
c
c         read and store element information
c
      if(ibigw.eq.0)go to 146
      if(ieldat.eq.0)go to 146
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
      lm(i,n)=id(1,node)
      xy(1,i,n)=x(node)
  135 xy(2,i,n)=y(node)
c
      call colht(mht,4,lm(1,n))
      if(ibigw.eq.0)go to 150
      if(ieldat.eq.0)go to 150
c
      write(6,145) n,mat(n),(nonum(i),i=1,4)
  140 format(///10x,12helement data,/,3x,3helm,3x,3hmat,2x,5hnode1,
     . 2x,5hnode2,2x,5hnode3,2x,5hnode4//)
  145 format(2i6,4i7)
c
  150 continue
c
      return
c
c         ind=1 pass to form element matrices
c
  200 if(ind.ne.1.and.ind.ne.3.and.ind.ne.6)go to 300
      if(ind.eq.1.and.kfsi.eq.2)return
      do 280 n=1,nume
      icode=0
      if (ind.eq.1) call echeck(lm(1,n),4,icode)
      if (icode.eq.1) go to 280
      m=mat(n)
      nint=iprop(1,m)
      ilumps=iprop(2,m)
      itype=iprop(3,m)
      nstr=3
      if(itype.eq.0)nstr=4
      if(ind.eq.3)go to 280
      if(kpri.eq.3)go to 280
      thic=fmat(1,m)
      rho=fmat(2,m)
c      fkfl = fluid bulk modulus
      fkfl = fmat(3,m)
      if (thic.eq.0.0) thic=1.0
  219 kprip=kpri
      if(ind.eq.3)kprip=4
      call prqm1(n,nint,nstr,thic,xy(1,1,n),rho,fkfl,ilumps,kprip,fk,fm)
      if(ind.eq.3) go to 255
c
      m=0
      do 220 i=1,4
      do 220 j=i,4
      m=m+1
      stiff(m)=fk(i,j)
  220 xmass(m)=fm(i,j)
c      print of element stiff damp and mass matrices and pointer array
      if(ibigw.eq.0)go to 950
      if(ipsdm.ne.1)go to 950
      write(6,951)n
  951 format(1h0,22hprint lm(i,n) array,n= ,i5//)
      write(6,952)(lm(ii,n),ii=1,4)
  952 format(1x,8i6//)
      if(kpri.ne.1)go to 702
      write(6,956)n
  956 format(1x//////,31h stiffness for pqdm element n= ,i5//)
      write(6,953)((fk(ii,jj),jj=1,4),ii=1,4)
  953 format(1x,4e13.5)
  702 if(kpri.ne.3)go to 703
  703 if(kpri.ne.2)go to 950
      write(6,960)n
  960 format(1x,//////,33h mass matrix for pqdm element n= ,i5//)
      write(6,953)((fm(ii,jj),jj=1,4),ii=1,4)
  950 continue
c
      go to (230,240,255), kpri
c
c         assemble stiffness matrix
c
  230 call addban(a,mht,stiff,lm(1,n),4)
      go to 280
c
c         assemble mass matrix
c
  240 call addban(a,mht,xmass,lm(1,n),4)
      go to 280
c
c    ind =3 pass
c
  255 do 320 i=1,4
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
  360 format(i5,1p4e11.3)
c
  280 continue
      return
c
c      ind=2 and ind=4 passes
c
  300 if(ind.ne.4)return
      do 315 n=1,nume
      call echeck(lm(1,n),4,icode)
      if(icode)315,310,315
  310 continue
      ngin=1
      return
  315 continue
      return
      end
c.................................................................
c.
c.  subroutine to calculate the stiffness and mass matrices for a
c.  four node pressure element.  the procedure is similar to the
c.  four node isoparametric element except for two changes; there
c.  is only one displacement component per node, and the material
c.  property matrix is constant and independent of material constants
c.  e and pr.
c.
c.  double precision is designated by implicit statement
c.
c.- - input variables- -
c. nel= number of element
c. nint= gauss numerical integration number
c. thic= thickness of element
c. xx(2,4)= element node coordinates
c.
c.- -output variables
c. fk(4,4)=stiffness matrix
c.      fm(4,4)=mass matrix
c...................................................................
*deck prqm1
      subroutine prqm1(nel,nint,nstr,thic,xx,rho,fkfl,ilumps,
     1 kprip,fk,fm)
      implicit real*8 (a-h,o-z)
      dimension xx(2,4),fk(4,4),fm(4,4),c(4,4),cb(4)
      dimension b(4,4),h(4),xg(4,4),wgt(4,4)
c
c     matrix xg stores gauss - legendre sampling points
c
      data xg/   0.00,   0.00,   0.00,   0.00,   -.577350269189600,
     1 .577350269189600,   0.00,   0.00,   -.774596669241500,   0.00,
     2 .774596669241500,   0.00,   -.861136311594100,
     3 -.339981043584900,   .339981043584900,   .861136311594100 /
c
c      matrix wgt stores gauss - legendre weighting factors
c
      data wgt /  2.00,   0.00,   0.00,   0.00,   1.00,   1.00,
     1 0.00,   0.00,   .555555555555600,   .888888888888900,
     2 .555555555555600,   0.00,   .347854845137500,
     3 .652145154862500,   .652145154862500,   .347854845137500 /
      data pi/3.14159265/
      itype=1
      if(nstr.eq.4)itype=0
      go to (230,240,250,250),kprip
  240 do 31 i=1,4
      do 31 j=1,4
   31 fm(i,j)=0.0
      if(ilumps.eq.1)go to 290
  230 continue
c.
c.d e f i n e  m a t e r i a l  m a t r i x  c
c.
      if(kprip.eq.2)go to 76
      do 10 i=1,4
   10 c(i,i)=fkfl
      c(1,2)=-fkfl
      c(1,3)=0.0
      c(1,4)=c(1,2)
      c(2,3)=0.0
      c(2,4)=c(1,2)
      c(3,4)=0.0
      do 15 i=1,4
      do 15 j=i,4
   15 c(j,i)=c(i,j)
c.
c.  calculate element stiffness and mass
c.
      do 30 i=1,4
      do 30 j=1,4
   30 fk(i,j)=0.0
   76 continue
      do 80 lx=1,nint
      ri=xg(lx,nint)
      do 80 ly=1,nint
      si=xg(ly,nint)
c.
c. evaluate derivative operator b and the jacobian determinant det
c.
      call prstom(xx,b,det,h,ri,si,xbar,nel,itype)
c.
c. add contribution to element stiffness
c.
      if(itype.gt.0)xbar=thic
      wt=wgt(lx,nint)*wgt(ly,nint)*xbar*det
      if(itype.eq.0)wt=2.0*pi*wt
      if(kprip.eq.2)go to 72
      do 70 j=1,4
      do 40 k=1,nstr
      cb(k)=0.0
      do 40 l=1,nstr
   40 cb(k)=cb(k) + c(k,l)*b(l,j)
      do 60 i=j,4
      stiff=0.0
      do 50 l=1,nstr
   50 stiff=stiff + b(l,i)*cb(l)
   60 fk(i,j)=fk(i,j) + stiff*wt
   70 continue
c.
c. add contribution to element mass
c.
      if(kprip.eq.1)go to 80
   72 continue
      do 75 i=1,4
      do 75 j=i,4
   75 fm(i,j)=fm(i,j) + h(i)*h(j)*wt*rho
   80 continue
c.
c. fill in other half of mass and stiffness matrices using symmetry
c.
      do 90 i=1,4
      do 90 j=i,4
      fm(j,i)=fm(i,j)
   90 fk(i,j)=fk(j,i)
  250 return
  290 continue
c.
c.    formation of the lumped mass matrix
c.
      area=((xx(1,2)-xx(1,3))*(xx(2,2)-xx(2,1)) + (xx(1,2)-xx(1,1))*
     1(xx(2,3)-xx(2,2)) + (xx(1,4)-xx(1,1))*(xx(2,4)-xx(2,3)) +
     2(xx(1,4)-xx(1,3))*(xx(2,1)-xx(2,4)))/2.0
      w=rho*thic*area
      ravg=(xx(1,1)+xx(1,2)+xx(1,3)+xx(1,4))/4.0
      if(itype.eq.0)w=2.0*pi*ravg*area*rho
      do 42 i=1,4
   42 fm(i,i)=w/4.0
      return
      end
c....................................................................
c.
c.  subroutine to evaluate the derivative operator b at the point(ri,si)
c....................................................................
*deck prstom
      subroutine prstom(xx,b,det,h,r,s,xbar,nel,itype)
      implicit real*8 (a-h,o-z)
      dimension xx(2,4),h(4),b(4,4),p(2,4),xj(2,2),xji(2,2)
c
      rp= 1.0 + r
      sp= 1.0 + s
      rm= 1.0 - r
      sm= 1.0 - s
c
c     interpolation functions
c
      h(1)= .25*rp*sp
      h(2)= .25*rm*sp
      h(3)= .25*rm*sm
      h(4)= .25*rp*sm
c
c
c     natural coordinate derivatives of the interpolation functions
c
c         1. with respect to r
c
      p(1,1)= .25*sp
      p(1,2)= -p(1,1)
      p(1,3)= -.25*sm
      p(1,4)= -p(1,3)
c
c         2. with respect to s
c
      p(2,1)= .25*rp
      p(2,2)= .25*rm
      p(2,3)= -p(2,2)
      p(2,4)= -p(2,1)
c
c     evaluate the jacobian matrix at the point (r,s)
c
  10  do 30 i=1,2
      do 30 j=1,2
      dum=0.0
      do 20 k=1,4
  20  dum=dum+p(i,k)*xx(j,k)
  30  xj(i,j)=dum
c
c     compute the determinant of the jacobian matrix at point (r,s)
c
      det=xj(1,1)*xj(2,2)-xj(2,1)*xj(1,2)
      if (det.gt.0.00000001)go to 40
      write (6,2000)nel
 2000 format(1h1,"error;",/,1x,
     1"zero or negative jacobian det for element",i6)
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
c     evaluate global derivative operator b
c
      do 60 k=1,4
      b(1,k)=0.0
      b(2,k)=0.0
      b(3,k)=0.0
      do 50 i=1,2
      b(2,k)=b(2,k) + xji(2,i)*p(i,k)
   50 b(3,k)=b(3,k) + xji(1,i)*p(i,k)
   60 continue
c
c     in case of plane strain or plane stress analysis do not include
c     the normal strain component
c
      if(itype.gt.0)return
c
c     compute the radius at point (r,s)
c
      xbar=0.0
      do 70 k=1,4
  70  xbar=xbar+h(k)*xx(1,k)
c
c     evaluate the hoop strain-displacement relation
c
      if(xbar.gt.0.00000001)go to 90
c
c     for the case of zero radius equate radial to hoop strain
c
      do 80 k=1,4
  80  b(4,k)=b(1,k)
      return
c
c     non zero radius
c
   90 do 100 k=1,4
 100  b(4,k)=0.0
      return
c
c
c
      end
*deck ptri
      subroutine ptri
      implicit real*8 (a-h,o-z)
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/dtwo/itwo,ndof,idim
      common/tapcom/nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlen
c
      common a(1)
      real a
c
      nfirst=n6e+4*itwo
      n101=nfirst
      n102=3*nummat*itwo+n101
      n103=6*nume*itwo+n102
      n104=nume+n103
      n105=3*nume+n104
      nlast=2*nummat+n105
      midest=nlast-nfirst+1
      min=nmax+nlast-n1l+1
      if(ind.eq.0)write(6,20)min
      if(min.le.nmax) go to 10
      stop 4
   10 call
     .ptrip(a(n2),a(n2l),a(n1l),a(n2),a(n3),a(n101),a(n102),a(n103),
     .a(n104),a(n105),nume,nummat,ndof,ind,kpri,nt11)
      if(ind.eq.0)call ewrite(a(n6e),ng)
   20 format (1h0,40hstorage required to input element data =,i10)
   30 format (1h0,21herror nmax less than ,i10)
      return
      end
*deck ptrip
      subroutine ptrip(a,id,mht,x,y,fmat,xy,mat,lm,iprop,nume,nummat,
     . ndof,ind,kpri,nt11)
      implicit real*8 (a-h,o-z)
      common/comtyp/itypee,ngroup,ngin,nlen
      common/fsicom/fsifac,wfsi,kfsi
c
      common /list/ data(64),ndum
      common/switch/ipsm,ipkbar,ipinpt,ipreac,ieldat,ipsdm,ipcnpf
     . ,ibigw
c
      dimension x(1),y(1),fmat(3,1),mat(1),lm(3,1),stiff(6),xmass(6),
     . fk(3,3),fm(3,3),xy(2,3,1),nonum(3),
     . id(ndof,1),mht(1),iprop(2,1)
      dimension ur(3),ui(3)
c
      if (ind.ne.0) go to 200
c
c         read and store material data
c
      do 115 m=1,nummat
      call fld
      do 110 n=1,3
  110 fmat(n,m)=data(n+2)
c *** print mat properties ****
      write(6,222)m
  222 format(1x//30hptri properties for mat number,i3//)
      write(6,223)(fmat(in,m),in=1,3)
  223 format(1x,3e12.2)
      do 115 n=1,2
  115 iprop(n,m)=data(n)
c
c         read and store element information
c
      if(ibigw.eq.0)go to 146
      if(ieldat.eq.0)go to 146
      write(6,140)
  146 continue
      do 150 n=1,nume
      call fld
      num=data(1)
      if (num.ne.n) stop 5
      mat(n)=data(2)
      do 135 i=1,3
      node=data(i+2)
      nonum(i)=node
      lm(i,n)=id(1,node)
      xy(1,i,n)=x(node)
  135 xy(2,i,n)=y(node)
c
      call colht(mht,3,lm(1,n))
      if(ibigw.eq.0)go to 150
      if(ieldat.eq.0)go to 150
c
      write(6,145) n,mat(n),(nonum(i),i=1,3)
  140 format(///10x,12helement data,/,2x,3helm,2x,3hmat,2x,5hnode1,
     . 2x,5hnode2,2x,5hnode3,//)
  145 format(2i5,3i7)
c
  150 continue
c
      return
c
c         ind=1 pass to form element matrices
c
  200 if(ind.ne.1.and.ind.ne.3.and.ind.ne.6)go to 300
      if(ind.eq.1.and.kfsi.eq.2)return
      do 280 n=1,nume
      icode=0
      if (ind.eq.1) call echeck(lm(1,n),3,icode)
      if (icode.eq.1) go to 280
      m=mat(n)
      ilumps=iprop(1,m)
      itype=iprop(2,m)
      nstr=3
      if(itype.eq.0)nstr=4
      if(ind.eq.3)go to 280
      if(kpri.eq.3)go to 280
      thic=fmat(1,m)
      rho=fmat(2,m)
c         fkfl=  fluid bulk modulus
      fkfl=fmat(3,m)
      if (thic.eq.0.0) thic=1.0
  219 kprip=kpri
      if(ind.eq.3)kprip=4
      call prtrm(n,nstr,thic,xy(1,1,n),rho,fkfl,ilumps,kprip,fk,fm)
      if(ind.eq.3) go to 255
c
      m=0
      do 220 i=1,3
      do 220 j=i,3
      m=m+1
      stiff(m)=fk(i,j)
  220 xmass(m)=fm(i,j)
c      print of element stiff damp and mass matrices and pointer array
      if(ibigw.eq.0)go to 950
      if(ipsdm.ne.1)go to 950
      write(6,951)n
  951 format(1h0,22hprint lm(i,n) array,n= ,i5//)
      write(6,952)(lm(ii,n),ii=1,3)
  952 format(1x,3i6//)
      if(kpri.ne.1)go to 702
      write(6,956)n
  956 format(1x//////,31h stiffness for ptri element n= ,i5//)
      write(6,953)((fk(ii,jj),jj=1,3),ii=1,3)
  953 format(1x,3e13.5)
  702 if(kpri.ne.3)go to 703
  703 if(kpri.ne.2)go to 950
      write(6,960)n
  960 format(1x,//////,33h mass matrix for ptri element n= ,i5//)
      write(6,953)((fm(ii,jj),jj=1,3),ii=1,3)
  950 continue
c
      go to (230,240,255), kpri
c
c         assemble stiffness matrix
c
  230 call addban(a,mht,stiff,lm(1,n),3)
      go to 280
c
c         assemble mass matrix
c
  240 call addban(a,mht,xmass,lm(1,n),3)
      go to 280
c
c    ind =3 pass
c
  255 do 320 i=1,3
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
  360 format(i5,1p4e11.3)
c
  280 continue
      return
c
c      ind=2 and ind=4 passes
c
  300 if(ind.ne.4)return
      do 315 n=1,nume
      call echeck(lm(1,n),3,icode)
      if(icode)315,310,315
  310 continue
      ngin=1
      return
  315 continue
      return
      end
c...............................................................
c.
c.  subroutine to calculate the stiffness and mass matricies for
c.  a triangular pressure element.  the procedure is similar to
c.  a regular element except for two changes; there is only one
c.  displacement component per node, and the material property
c.  matrix is constant and independent of the material properties
c.  e and pr.
c.
c.
c.- -input variables- -
c. xx(2,3)= coordinates of the corner points; i=1: x-value
c.   i=2: y-value; for the points j=1,3
c. thic   = thickness of element
c. rho    = material density
c.
c.- -output variables- -
c. fk(3,3) = stiffness matrix
c.      fm(3,3) = mass matrix
c.
c...................................................................
*deck prtrm
      subroutine prtrm(n,nstr,thic,xx,rho,fkfl,ilumps,kpri,fk,fm)
      implicit real*8 (a-h,o-z)
      dimension c(4,4),xx(2,3),cp(3),bp(3),b(4,3),cb(4),fk(3,3),fm(3,3)
      data pi/3.14159265/
      ravg=0.0
      do 79 i=1,3
   79 ravg=ravg + xx(1,i)/3.0
      y31=xx(2,3)-xx(2,1)
      y23=xx(2,2)-xx(2,3)
      y12=xx(2,1)-xx(2,2)
      x21=xx(1,2)-xx(1,1)
      x32=xx(1,3)-xx(1,2)
      x13=xx(1,1)-xx(1,3)
      ta=(x32*y12-x21*y23)
      go to (230,240,260,260)kpri
  230 continue
c.
c.d e f i n e  m a t e r i a l  m a t r i x  c
c.
      do 10 i=1,4
   10 c(i,i)=fkfl
      c(1,2)=-fkfl
      c(1,4)=c(1,2)
      c(1,3)=0.0
      c(2,4)=c(1,2)
      c(2,3)=0.0
      c(3,4)=0.0
      do 29 i=1,4
      do 29 j=i,4
   29 c(j,i)=c(i,j)
c.
c. o b t a i n  s t r a i n  d i s p l a c e m e n t  m a t r i x  b
c.
c.
c. define differences between x coordinates and y coordinates
c.   eg. x32=x(node 3) - x(node 2)
c.
      bp(1)=y23/ta
      bp(2)=y31/ta
      bp(3)=y12/ta
      cp(1)=x32/ta
      cp(2)=x13/ta
      cp(3)=x21/ta
c.
c. b  m a t r i x
c.
      do 25 j=1,3
      b(1,j)=0.0
      b(2,j)=cp(j)
      b(3,j)=bp(j)
      b(4,j)=0.0
   25 continue
c.
c. calculate element stiffness
c.
      wt=thic*ta/2.0
      if(nstr.eq.4)wt=pi*ravg*ta
      do 88 i=1,3
      do 88 j=1,3
   88 fk(i,j)=0.0
      do 70 j=1,3
      do 40 k=1,nstr
      cb(k)=0.0
      do 40 l=1,nstr
   40 cb(k)=cb(k)+c(k,l)*b(l,j)
      do 60 i=j,3
      stiff=0.0
      do 50 l=1,nstr
   50 stiff=stiff+b(l,i)*cb(l)
   60 fk(i,j)=fk(i,j)+stiff*wt
   70 continue
c.
c. obtain second half of stiffness matrix using symmetry
c.
      do 90 i=1,3
      do 90 j=i,3
   90 fk(i,j)=fk(j,i)
  260 return
  240 continue
c.
c. m a s s  m a t r i x
c.
      w=thic*ta*rho/2.0
      if(nstr.eq.4)w=pi*ravg*ta*rho
      write(6,109)ta,ravg,w,rho
  109 format(1x,"ta= ",e12.4," ravg= ",e12.4,
     x" w= ",e12.4," rho= ",e12.4)
      if(ilumps.eq.1)go to 99
      do 98 i=1,3
      do 98 j=1,3
      fm(i,j)=w/12.0
      if(i.eq.j)fm(i,j)=w/6.0
   98 continue
      return
   99 continue
c
c lumped mass matrix
c
      do 23 i=1,3
   23 fm(i,i)=w/3.0
      return
      end
