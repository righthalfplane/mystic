*deck mystic
      program griffin(input,output,tape5=input,tape6=output,
     .   tape8,tape9,tape11,tape49,tape31,tape32,tape33,
     .   tape34,tape35,tape36,tape37,tape43,tape50,tape81)
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat,kpdvec
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/scom/nload,iloads,mvlast
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/tcom/munit,lunit,nstif,nred,nunit
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/ncom/numest,nvect,istoh,maxbh
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8,n9
      common /addb/ neql,neqr,mla,nblock
      common/simcom/x0,y0,z0,insim,incon
      common/dtwo/itwo,ndof,idim
      common/flgs1/ibig,idamp
      common/tapcom/nt11,nt31
      common/grav/ax,ay,az
      common/prcom/ipress
      common/list/data(64),ndum
      common/elcom/nwelem
      common/comtyp/itype,ngroup,ngin,nlin
      common/dyncm2/idym,imass,ires,intial,ncycle,nstep,nbdym
      common/stapes/nts1,ntb1,nts2,ntb2,nts3,ntb3
      common/prcom2/ibk,ipnt,idva(3),ipnode(3,20)
      common/dyncm1/time,dt,dtold,delta,alpha
      dimension head(8)
      dimension com(18)
      dimension etype(20)
      dimension kploc(12),ptype(12)
      dimension ia(1)
      common a(3000000)
      real a
      integer ia
      equivalence (a(1),ia(1)),(kploc(1),kpnode)
      data com/4hstop,4hnode,4hform,4hload,4hsolv,4hback,4hchec,
     . 3hpon,4hpoff,3hbig,4hdamp,4helen,4hpres,4hgrav,4hiter,
     * 4hover,4hstar,3hpnt/
      data etype/4hreac,4htape,3hcon,4huser,4hquad,3htri,4hbeam,
     .  4haxsh,4hpqdm,4hhex8,4hphex,4hptri,4hbm3d,4hatri,4haqua,
     .  4hashl,4hastf,3*1h /
      data ptype/4hnode,3hdof,4helem,4hload,4hcomp,3hequ,4hequl,
     .  4hequr,4hstre,3hdis,3hmat,4hdvec/
      data end,strs,off/3hend,4hstre,3hoff/
      data ibig,idamp/0,1/
      data kploc/4*1,0,1,1,0,1,1,1,0/
      data ax,ay,az/0.,0.,0./
      n1=1
      ip=1
      itwo=1
      nt11=11
      nt31=31
      iequit=1
      icheck=0
      nmax=3000000
      munit=2
      lunit=3
      nstif=4
      nred=5
      nunit=6
      ipress=0
      ngroup=200
      insim=4
      incon=0
      open(unit=19,file='over',form='unformatted',status='unknown')
      read(5,1000)head
      write(6,1010)ip,head
      call ffin
      rewind 11
      rewind 49
   10 continue
c
c          read next command card
c
      call fld
c
c        command loop
c
      word=data(1)
      do 30 i=1,18
      ic=i
      if(word.eq.com(i))go to 50
   30 continue
c
c         element loop
c
      do 40 i=1,20
      ic=i
      if(word.eq.etype(i))go to 100
   40 continue
      write(6,2000)word
      stop
   50 continue
      go to (900,60,530,540,560,590,700,710,720,760,770,780,790,800,
     *  810,820,830,620)ic
   60 continue
c
c        generate nodes and d.o.f. table
c
      numest=0
      ndmax=0
      nconi=0
      nsave=0
      ncon=0
      nlin=0
      nc1=0
      nc2=0
      nc3=0
      nc4=0
      ind=0
      ng=0
      call nodep
      go to 10
  100 continue
c
c          elements types
c
      go to (110,120,130,140,150,160,170,180,190,200,210,220,230,
     .       240,250,260,270),ic
  110 continue
c
c          reactance
c
      itype=1
      go to 500
  120 continue
c
c          tape input
c
      itype=2
      go to 500
  130 continue
c
c        constraint data input
c
      if(nconi.le.0)nconi=ncon
      if(nlin.le.0)nlin=ng+1
      itype=3
      go to 500
  140 continue
c
c         user subroutine
c
      itype=4
      go to 500
  150 continue
c
c          quad4
c
      itype=5
      go to 500
  160 continue
c
c          tri
c
      itype=6
      go to 500
  170 continue
c
c          beam
c
      itype=7
      go to 500
  180 continue
c
c          axsh
c
      itype=8
      go to 500
  190 continue
c
c          pquad4
c
      itype=9
      go to 500
  200 continue
c
c          hex8
c
      itype=10
      go to 500
  210 continue
c
c          phex
c
      itype=11
      go to 500
  220 continue
c
c         ptri
c
      itype=12
      go to 500
  230 continue
c
c         beam3d
c
      itype=13
      go to 500
  240 continue
c
c         atri
c
      itype=14
      go to 500
  250 continue
c
c         aqua
c
      itype=15
      go to 500
  260 continue
c
c         ashl
c
      itype=16
      go to 500
  270 continue
c
c         astf
c
      itype=17
      go to 500
  500 continue
      nume=data(2)
      nummat=data(3)
      iloads=data(4)
      n6e=n5
      n1l=n2l-neq
c
c        clear array for column calculation
c
      if(ng.gt.0)go to 520
      call opene
      do 510 i=1,neq
  510 ia(i+n1l-1)=0
  520 continue
      ng=ng+1
      if(ng.le.ngroup)go to 525
      write(6,2150)ngroup
      stop
  525 continue
      call gstiff
      if(midest.gt.numest)numest=midest
      go to 10
  530 continue
c
c        form stiffness
c
      rewind 49
      call second(t1)
      call assem
      call second(t2)
      t=t2-t1
      write(6,2100)t
      go to 10
  540 continue
c
c        input load data
c
      n3=n2+neq*itwo
      n4=n3+nvect*itwo
      n5=n4+numnp*ndof
      n6=n4+numest
      n8=n6+neq*itwo
      n9=n8+neq*itwo
      call zero(a(n8),neq)
      call zero(a(n9),neq)
      n6e=n4
      n7=max0(n5,n9)
      ind=5
      min=nmax+n7-n1l+1
      write(6,2130)min
      if(min.le.nmax)go to 550
      stop
  550 continue
      call load(a(n2),a(n4),a(n6e),a(n3),ndof,nvect,0)
      go to 10
  560 continue
c
c        solution loop
c
      call second(ta)
      istres=0
      iequit=kpeq1+kpeq2+kpeq3
      if(data(2).eq.strs)istres=1
      iback=0
      call solve(iequit,istres,iback)
      call second(tb)
      t=tb-ta
      write(6,2120)t
      go to 10
  590 continue
c
c        back sub
c
      call second(ta)
      istres=0
      if(data(2).eq.strs)istres=1
      iequit=kpeq1+kpeq2+kpeq3
      iback=1
      call solve(iequit,istres,iback)
      call second(tb)
      t=tb-ta
      write(6,2140)t
      go to 10
  700 continue
c
c          set flag for data check
c
      icheck=1
      go to 10
  620 continue
      ibk=0
      ipnt=data(2)
      idis=data(3)
      ivel=data(4)
      iacc=data(5)
  630 continue
      call fld
      if(data(1).eq.end)go to 10
      ibk=ibk+1
      if(ibk.le.20)go to 640
      write(6,2190)
2190  format(1h0,10x,30hstop more than 20 print blocks)
      call abort
  640 continue
      ipnode(1,ibk)=data(1)
      ipnode(2,ibk)=data(2)
      ipnode(3,ibk)=data(3)
      go to 630
  650 continue
  710 continue
c
c          set flags on
c
      ipnt=1
      go to 730
  720 continue
c
c         set flags off
c
      ipnt=0
  730 continue
      do 750 i=2,64
      word=data(i)
      if(word.eq.0.)go to 10
      do 740 n=1,12
      if(word.eq.ptype(n))kploc(n)=ipnt
  740 continue
  750 continue
      go to 10
  760 continue
c
c         big option
c
      ibig=data(2)
      if(ibig.eq.0)go to 10
      iequit=0
      kpeq1=0
      kpeq2=0
      kpeq3=0
      go to 10
  770 continue
c
c          damping flag
c
      idamp=1
      if(data(2).eq.off)idamp=0
      go to 10
  780 continue
c
c          number of words required for element data
c
      nwelem=data(2)
      go to 10
  790 continue
      n3=n2+neq*itwo
      n4=n3+numnp*itwo
      n5=n4+numnp*itwo
      n6=n5+ndof*numnp
      min=nmax+n6-n1l+1
      write(6,2160)min
      if(min.gt.nmax)stop
      call press(a(n2),a(n3),a(n4),a(n5),ndof)
      go to 10
  800 continue
      ax=data(2)
      ay=data(3)
      az=data(4)
      write(6,2170)ax,ay,az
      go to 10
  810 continue
c
c        solution loop
c
      call second(ta)
      call iter
      call second(tb)
      t=tb-ta
      write(6,2120)t
      go to 10
  820 continue
      insim=1
      x0=data(2)
      y0=data(3)
      z0=data(4)
      write(6,1020)x0,y0,z0
      go to 10
  830 continue
      insim=2
      write(6,1030)
      go to 10
  900 continue
      call exit
 1000 format(8a4)
 1010 format(1h1,i4,5x,8a4)
 1020 format(1h0,10x,14hsimone overlay,/,
     . 1h0,15x,5hx0 = ,1pe12.4,/,
     . 1h0,15x,5hy0 = ,1pe12.4,/,
     . 1h0,15x,5hz0 = ,1pe12.4)
 1030 format(1h0,10x,12hstar overlay)
 2000 format(1h0,10x,27hstop unreconized command = ,a10)
 2100 format(1h0,10x,34htime required to form stiffness = ,f13.3,
     .  8h seconds)
 2120 format(1h0,10x,29htime required for solution = ,f13.3,8h seconds)
 2130 format(1h0,10x,45hnumber of words required to input load data = ,
     .       i10)
 2140 format(1h0,10x,29htime required for backsub  = ,f13.3,8h seconds)
 2150 format(1h0,10x,27hmaximum numger of groups = ,i5)
 2160 format(1h0,10x,45hnumber of words required for pressure loads =,
     . i10)
 2170 format(1h0,10x,26hgravity acceleration terms,///,15x,5hax = ,
     .   1pe12.4,/,15x,5hay = ,1pe12.4,/,15x,5haz = ,1pe12.4)
      end
*deck press
      subroutine press (f,x,y,id,ndof)
      dimension f(1),x(1),y(1),id(ndof,1),x2(3),y2(3),wgtl(3),
     .          xgl(3),vec(6),n(3),p(6)
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat,kpdvec
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/list/data(64),nw
      common/prcom/ipress
      data xgl/
     . -.7745966692415, .0000000000000, .7745966692415/
      data wgtl/
     .  .5555555555556, .8888888888889, .5555555555556/
      ipress=1
      if(kpload.gt.0)write(6,1000)
      rewind 8
      read(8)((id(i,j),i=1,ndof),j=1,numnp)
      read(8)ndump,(x(i),i=1,numnp),(y(i),i=1,numnp)
      do 10 i=1,neq
      f(i)=0.
   10 continue
 
   20 continue
      call fld
      if(data(1).eq.3hend)return
      p1=data(1)
      p2=data(2)
      n(1)=data(3)
      n(2)=data(4)
      n(3)=data(5)
      nrep=data(6)
      if(nrep.le.0)nrep=1
      inc=data(7)
      if(inc.eq.0)inc=1
      itype=data(8)
      thick=data(9)
      if(thick.le.0.)thick=1.
      node=2
      if(n(3).gt.0)node=3
      nv=2*node
      if(kpload.gt.0)write(6,1010)p1,p2,n,nrep,inc,itype,thick
      do 100 np=1,nrep
      do 30 i=1,node
      nn=n(i)
      x2(i)=x(nn)
      y2(i)=y(nn)
   30 continue
      do 40 i=1,nv
      p(i)=0.
   40 continue
      do 60 j=1,3
      call pre2d (xgl(j),vec,x2,y2,thick,p1,p2,itype,node)
      do 50 i=1,nv
      p(i)=p(i)+vec(i)*wgtl(j)
   50 continue
   60 continue
      do 80 i=1,node
      nn=n(i)
      ip=id(1,nn)
      if(ip.le.0)go to 70
      f(ip)=f(ip)+p(2*i-1)
   70 continue
      ip=id(2,nn)
      if(ip.le.0)go to 80
      f(ip)=f(ip)+p(2*i)
   80 continue
      do 90 i=1,node
      n(i)=n(i)+inc
   90 continue
  100 continue
      go to 20
 1000 format(1h0,10x,27hp r e s s u r e   l o a d s,//,
     .  5x,2hp1,10x,2hp2,8x,2hn1,3x,2hn2,3x,2hn3,1x,4hnrep,2x,3hinc,
     .  2x,4htype,3x,5hthick)
 1010 format(1p2e12.4,6i5,1pe12.4)
      end
*deck pre2d
      subroutine pre2d (r,vec,y,z,thick,pi,pj,itype,nodes)
      dimension vec(1),h(3),g(2),hr(3),y(3),z(3)
c
      g(1) = 0.5*(1.0+r)
      g(2) = 0.5*(1.0-r)
c
      if(nodes.gt.2) go to 10
c
      h(1) = g(1)
      h(2) = g(2)
c
      hr(1) = 0.5
      hr(2) =-0.5
c
      go to 20
c
   10 h(1) = 0.5*r*(1.0+r)
      h(2) =-0.5*r*(1.0-r)
      h(3) = 1.0-r*r
c
      hr(1)= 0.5+r
      hr(2) =-0.5+r
      hr(3)=-2.0*r
c
   20 x1 = thick
c
      if (itype.gt.0) go to 40
c
      x1 = 0.0
      do 30 k=1,nodes
   30 x1 = x1 + y(k)*h(k)
c
   40 yr = 0.0
      zr = 0.0
c
      do 50 k=1,nodes
      yr = yr + hr(k)*y(k)
   50 zr = zr + hr(k)*z(k)
c
      press = g(1)*pi + g(2)*pj
      x1    =  x1*press
c
      g(1) = x1*zr
      g(2) =-x1*yr
c
      do 60 k=1,nodes
      vec(2*k-1) = g(1)* h(k)
      vec(2*k  ) = g(2)* h(k)
   60 continue
c
      return
c
      end
*deck maxstr
      subroutine maxstr (stress,p1,p2,ag)
      dimension stress(3)
      cc = (stress(1)+stress(2)) * 0.5
      bb = (stress(1)-stress(2)) * 0.5
      cr =  sqrt(bb**2 + stress(3)**2)
      p1 =  cc+cr
      p2 =  cc-cr
      ag=45.0
      if(abs(bb).lt..00000001) return
      ag =  28.648* atan2(stress(3),bb)
      return
      end
*deck addma
      subroutine addma(a,s,lm,nd)
      dimension a(1),s(1),lm(1)
      do 100 i=1,nd
      ii=lm(i)
      if(ii)100,100,90
   90 continue
      a(ii)=a(ii)+s(i)
  100 continue
      return
      end
*deck colsol
      subroutine colsol(maxa,maxa2,ncolbv,icopl,va,vb,a,b,
     .                  da,db,nblock,neq,istorl,nvect,mvect,
     .                  maxbh,nstif,nred,lunit,munit,nunit,kkk)
c     . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c     .
c     .   p r o g r a m
c     .        to solve finite element static equilibrium equations out-
c     .        core, using compacted storage and column reduction scheme
c     .
c     . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c
      dimension a(1),b(1)
      dimension va(1),  vb(1)
      dimension da(1),  db(1)
      integer maxa(1),maxa2(1),icopl(1),ncolbv(1)
c
      khbb=0
      nwrite=0
      if (kkk-2) 20,280,280
   20 continue
c
c     - - factorize stiffness matrix  ( loop over all blocks ) - -
c
      mvectb=0
      mhold=0
      mlast=0
      mvectg=0
      do 270 nj=1,nblock
c
      call recs (nstif,a,istorl,nj)
      call recs (munit,maxa,maxbh,nj)
      ncolb=ncolbv(nj)
      mm=maxa(1)-1
      if (nj.eq.icopl(nj)) go to 120
c
      nwrite=0
      ik=icopl(nj)-1
      im=0
      if (ik) 120,50,30
   30 do 40 k=1,ik
   40 im=im+ncolbv(k)
   50 khb=khbb-im
      ik=ik+1
      nj1=nj-1
c
c     reduce block by the preceeding coupling blocks
c
      do 110 nk=ik,nj1
c
      call recs (nred,b,istorl,nk)
      call recs (munit,maxa2,maxbh,nk)
      khb=khb-ncolbv(nk)
      mc=maxa2(1)-1
c
      do 100 n=1,ncolb
      kn=maxa(n)-mm
      kl=kn+1
      ku=maxa(n+1)-1-mm
      kh=ku-kl-n+1
      kc=kh-khb
      if (kc.le.0) go to 100
      ic=0
      kcl=ncolbv(nk)-kc+1
      if (kcl.gt.0) go to 60
      ic=1-kcl
      kcl=1
   60 kcr=ncolbv(nk)
      klt=ku-ic
c
      do 90 k=kcl,kcr
      ic=ic+1
      klt=klt-1
      ki=maxa2(k)-mc
      nd=maxa2(k+1)-ki-mc-1
      if (nd) 90,90,70
   70 kk=min0(ic,nd)
      c=0.
      do 80 l=1,kk
      c=c+b(ki+l)*a(klt+l)
   80 continue
      a(klt)=a(klt)-c
   90 continue
  100 continue
c
      im=im+ncolbv(nk)
c
  110 continue
      nwrite=nj1
c
c     reduce block by itself
c
  120 do 260 n=1,ncolb
      kn=maxa(n)-mm
      kl=kn+1
      ku=maxa(n+1)-1-mm
      kdif=ku-kl
      kh=min0(kdif,n-1)
      ks=n+khbb
  130 continue
      if (ks.le.mlast) go to 140
      mhold=mlast
      mlast=mlast+mvect
      if (mvectb.gt.0) call wecs (nunit,da,mvect,mvectb)
      mvectb=mvectb+1
      go to 130
  140 continue
      kss=ks-mhold
      if (kh) 250,190,150
  150 k=n-kh
      klt=kl+kh
      ic=0
      if ((n-1).lt.kdif) ic=kdif-n+1
c
      do 180 j=1,kh
      ic=ic+1
      klt=klt-1
      ki=maxa(k)-mm
      nd=maxa(k+1)-ki-mm-1
      if (nd) 180,180,160
  160 kk=min0(ic,nd)
      c=0.
      do 170 l=1,kk
      c=c+a(ki+l)*a(klt+l)
  170 continue
      a(klt)=a(klt)-c
  180 k=k+1
c
  190 continue
      if (kss.le.1) go to 210
      kup=min0(ku,kl+ks-mhold-2)
      k=kss
      e=0.
      do 200 kk=kl,kup
      k=k-1
      c=a(kk)/da(k)
      e=e+c*a(kk)
      a(kk)=c
  200 continue
      a(kn)=a(kn)-e
c
      if (kup.eq.ku) go to 250
      kl=kup+1
  210 continue
      if (mvectg.eq.mvectb-1) go to 230
      mvectg=mvectb
  220 continue
      mvectg=mvectg-1
      call recs (nunit,db,mvect,mvectg)
  230 continue
      k=mvect+1
      kup=min0(ku,kl+mvect-1)
      e=0.
      do 240 kk=kl,kup
      k=k-1
      c=a(kk)/db(k)
      e=e+c*a(kk)
      a(kk)=c
  240 continue
      a(kn)=a(kn)-e
      if (kup.ge.ku) go to 250
      kl=kup+1
      go to 220
  250 continue
      da(kss)=a(kn)
      if(da(kss))260,256,260
  256 continue
      write(6,1000)ks
      stop
  260 continue
      call wecs (nunit,da,mvect,mvectb)
c
      khbb=khbb+ncolb
      nwrite=nwrite+1
      call wecs (nred,a,istorl,nwrite)
  270 continue
      return
c
c     - - solution of equations ( loop over all blocks ) - -
c
c     reduce the load vector
c
  280 continue
      nvectb=0
      nvectg=0
      nhold=0
      nlast=0
      do 380 nj=1,nblock
      call recs (nred,a,istorl,nj)
      call recs (munit,maxa2,maxbh,nj)
      ncolb=ncolbv(nj)
      mm=maxa2(1)-1
      do 370 n=1,ncolb
      kl=maxa2(n)-mm+1
      ku=maxa2(n+1)-mm-1
      if (ku-kl) 370,290,290
  290 ks=n+khbb
      k=ks
  300 continue
      if (k.le.nlast) go to 310
      nhold=nlast
      nlast=nlast+nvect
      if (nvectb.gt.0) call wecs (lunit,va,nvect,nvectb)
      nvectb=nvectb+1
      call recs (lunit,va,nvect,nvectb)
      go to 300
  310 continue
      c=0.
      kss=ks-nhold
      if (kss.le.1) go to 330
      kup=min0(ku,kl+ks-nhold-2)
      k=kss
      c=0.
      do 320 kk=kl,kup
      k=k-1
      c=c+a(kk)*va(k)
  320 continue
      va(kss)=va(kss)-c
      if (kup.eq.ku) go to 370
      kl=kup+1
  330 continue
      if (nvectg.eq.nvectb-1) go to 350
      nvectg=nvectb
  340 continue
      nvectg=nvectg-1
      call recs (lunit,vb,nvect,nvectg)
  350 continue
      k=nvect+1
      kup=min0(ku,kl+nvect-1)
      c=0.
      do 360 kk=kl,kup
      k=k-1
      c=c+a(kk)*vb(k)
  360 continue
      va(kss)=va(kss)-c
      if (kup.eq.ku) go to 370
      kl=kup+1
      go to 340
  370 continue
      khbb=khbb+ncolb
  380 continue
      if (nvectb.gt.0) call wecs (lunit,va,nvect,nvectb)
c
c     backsubstitute
c
      nlast=0
      nload=(neq-1)/nvect+1
      do 420 nj=1,nload
      call recs(nunit,da,nvect,nj)
      call recs(lunit,va,nvect,nj)
      mv=min0(nvect,neq-nlast)
      nlast=nlast+nvect
      do 410 k=1,mv
      va(k)=va(k)/da(k)
  410 continue
      call wecs(lunit,va,nvect,nj)
  420 continue
      nvectb=nload
      nhold=nlast-nvect
      nbl=nblock
      nvectg=0
      do 530 nj=1,nblock
      if (nblock.eq.1) go to 430
      call recs(nred,a,istorl,nbl)
      call recs(munit,maxa2,maxbh,nbl)
      ncolb=ncolbv(nbl)
  430 khbb=khbb-ncolb
      mm=maxa2(1)-1
      n=ncolb
      do 520 l=1,ncolb
      kl=maxa2(n)-mm+1
      ku=maxa2(n+1)-mm-1
      if (ku-kl) 510,440,440
  440 ks=khbb+n
      k=ks
      if (k.gt.nhold) go to 450
      nlast=nhold
      nhold=nhold-nvect
      call wecs (lunit,va,nvect,nvectb)
      nvectb=nvectb-1
      call recs (lunit,va,nvect,nvectb)
  450 continue
      kss=ks-nhold
      if (kss.le.1) go to 470
      kup=min0(ku,kl+ks-nhold-2)
      k=kss
      do 460 kk=kl,kup
      k=k-1
      vks=va(kss)
      va(k)=va(k)-a(kk)*va(kss)
  460 continue
      if (ku.eq.kup) go to 510
      kl=kup+1
  470 continue
      if (nvectg.eq.nvectb-1) go to 490
      nvectg=nvectb
  480 continue
      nvectg=nvectg-1
      call recs (lunit,vb,nvect,nvectg)
  490 continue
      k=nvect+1
      kup=min0(ku,kl+nvect-1)
      do 500 kk=kl,kup
      k=k-1
      vks=va(kss)
      vb(k)=vb(k)-a(kk)*va(kss)
500   continue
      call wecs (lunit,vb,nvect,nvectg)
      if (kup.eq.ku) go to 510
      kl=kup+1
      go to 480
  510 n=n-1
  520 continue
      nbl=nbl-1
  530 continue
      call wecs (lunit,va,nvect,nvectb)
c
      return
 1000 format(10x,25hstop zero diagonal dof = ,i10)
      end
*deck equit
      subroutine equit(aa,ab,b,ca,cb,maxa,ncolbv,a,
     .                 nvect,munit,nstif,lunit,nunit)
c
c        calculate   f-k*x
c
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat,kpdvec
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/scom/nload,iloads,mvlast
      common/dtwo/itwo,ndof,idim
      dimension aa(1),ab(1),b(1)
      dimension ca(1),cb(1)
      dimension a(1)
      integer maxa(1),ncolbv(1)
c
c        save diagonal
c
      do 10 i=1,nload
      call recs(nunit,aa,nvect,i)
      call wecs(nunit,aa,nvect,i+nload)
   10 continue
      call mult(aa,ab,b,ca,cb,maxa,ncolbv,nstif,lunit,munit,nunit)
c
c        start print options
c
      mv=nvect
      dnorm=0.
      call load(aa,ab,a,a,ndof,nvect,1)
      do 30 i=1,nload
      call recs(lunit,ca,nvect,i)
      call recs(nunit,aa,nvect,i)
      if(i.eq.nload)mv=mvlast
      do 20 j=1,mv
      ca(j)=ca(j)-aa(j)
      dnorm=dnorm+ca(j)**2
   20 continue
      call wecs(lunit,ca,nvect,i)
   30 continue
      dnorm=sqrt(dnorm)
      write(6,1000)dnorm
      if(kpeq2.eq.1)call write(aa,a,a,ndof,nvect,nunit,1)
      if(kpeq3.eq.1)call write(aa,a,a,ndof,nvect,nunit,2)
  250 continue
c
c        restore diagonal
c
      do 260 i=1,nload
      call recs(nunit,aa,nvect,i+nload)
      call wecs(nunit,aa,nvect,i)
  260 continue
      return
 1000 format(1h0,10x,7hnorm = ,1pe12.4)
      end
*deck mult
      subroutine mult(aa,ab,b,ca,cb,maxa,ncolbv,nstif,lunit,munit,
     .                nunit)
c
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c .                                                                   .
c .   p r o g r a m                                                   .
c .      . to calculate   a = b*c   ,   where  b  is stored in        .
c .        compacted form  ,  a  and  c  are vectors                  .
c .                                                                   .
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c
      common/scom/nload,iloads
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/ncom/numest,nvect,istoh,maxbh
      common/addb/neql,neqr,mla,nblock
      dimension aa(1),ab(1),b(1)
      dimension ca(1),cb(1)
      integer maxa(1),ncolbv(1)
      if(neq.gt.1)go to 10
c
c        one dof case
c
      call recs(nstif,b,istoh,1)
      call recs(lunit,ca,nvect,1)
      aa(1)=b(1)*ca(1)
      call wecs(nunit,aa,nvect,1)
      return
   10 continue
c
c        zero a array
c
      do 20 i=1,nvect
      aa(i)=0.
   20 continue
      do 30 i=1,nload
      call wecs(nunit,aa,nvect,i)
   30 continue
c
c        start block loop
c
      khbb=0
      do 210 l=1,nblock
      call recs(nstif,b,istoh,l)
      call recs(munit,maxa,maxbh,l)
      ncolb=ncolbv(l)
      mm=maxa(1)-1
      nhold=0
      nlast=0
      nvectb=0
      mhold=0
      mlast=0
      mvectb=0
      mvectg=0
      ionce=0
      do 110 i=1,ncolb
      kl=maxa(i)-mm
      ku=maxa(i+1)-1-mm
      ii=i+khbb
      if(ii.le.nlast)go to 40
   35 continue
      nhold=nlast
      nlast=nlast+nvect
      nvectb=nvectb+1
      if(ii.gt.nlast)go to 35
      call recs(lunit,ca,nvect,nvectb)
   40 continue
      kss=ii-nhold
      cc=ca(kss)
      if(ii.le.mlast)go to 50
      if(ionce.gt.0)call wecs(nunit,aa,nvect,mvectb)
   45 continue
      mhold=mlast
      mlast=mlast+nvect
      mvectb=mvectb+1
      if(ii.gt.mlast)go to 45
      call recs(nunit,aa,nvect,mvectb)
      ionce=1
   50 continue
      kss=ii-mhold+1
      if(kss.le.1)go to 70
c
c        do current aa block
c
      kup=min0(ku,kl+ii-mhold-1)
      k=kss
      do 60 kk=kl,kup
      k=k-1
      aa(k)=aa(k)+b(kk)*cc
   60 continue
      if(ku.eq.kup)go to 110
      kl=kup+1
   70 continue
c
c        pick up old aa blocks
c
      if(mvectg.eq.mvectb-1)go to 90
      mvectg=mvectb
   80 continue
      mvectg=mvectg-1
      call recs(nunit,ab,nvect,mvectg)
   90 continue
      k=nvect+1
      kup=min0(ku,kl+nvect-1)
      do 100 kk=kl,kup
      k=k-1
      ab(k)=ab(k)+b(kk)*cc
  100 continue
      call wecs(nunit,ab,nvect,mvectg)
      if(kup.eq.ku)go to 110
      kl=kup+1
      go to 80
  110 continue
      call wecs(nunit,aa,nvect,mvectb)
      nhold=0
      nlast=0
      nvectb=0
      nvectg=0
      mhold=0
      mlast=0
      mvectb=0
      ionce=0
      do 200 i=1,ncolb
      kl=maxa(i)+1-mm
      ku=maxa(i+1)-1-mm
      if(ku-kl)200,120,120
  120 ii=i+khbb
      if(ii.le.mlast)go to 130
      if(ionce.gt.0)call wecs(nunit,aa,nvect,mvectb)
  125 continue
      mhold=mlast
      mlast=mlast+nvect
      mvectb=mvectb+1
      if(ii.gt.mlast)go to 125
      call recs(nunit,aa,nvect,mvectb)
      ionce=1
  130 continue
      if(ii.le.nlast)go to 140
  135 continue
      nhold=nlast
      nlast=nlast+nvect
      nvectb=nvectb+1
      if(ii.gt.nlast)go to 135
      call recs(lunit,ca,nvect,nvectb)
  140 continue
      kss=ii-mhold
      e=0.
      ks=ii-nhold
      if(ks.le.1)go to 160
c
c        do current ca block
c
      kup=min0(ku,kl+ii-nhold-2)
      k=ks
      do 150 kk=kl,kup
      k=k-1
      e=e+b(kk)*ca(k)
  150 continue
      if(kup.eq.ku)go to 195
      kl=kup+1
  160 continue
c
c        pick up old ca blocks
c
      if(nvectg.eq.nvectb-1)go to 180
      nvectg=nvectb
  170 continue
      nvectg=nvectg-1
      call recs(lunit,cb,nvect,nvectg)
  180 continue
      k=nvect+1
      kup=min0(ku,kl+nvect-1)
      do 190 kk=kl,kup
      k=k-1
      e=e+b(kk)*cb(k)
  190 continue
      if(kup.eq.ku)go to 195
      kl=kup+1
      go to 170
  195 continue
      aa(kss)=aa(kss)+e
  200 continue
      if(ionce.gt.0)call wecs(nunit,aa,nvect,mvectb)
      khbb=khbb+ncolb
  210 continue
  220 continue
      return
      end
*deck addban
      subroutine addban(a,maxa,s,lm,nd)
c
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c .                                                             .
c .                                                                   .
c .   assembles upper triangular element stiffness into               .
c .         compacted global stiffness                                .
c .                                                                   .
c .         a = global stiffness                                      .
c .         s = element stiffness                                     .
c .         nd = degrees of freedom in element stiffness              .
c .                                                                   .
c .                   s(1)        s(2)        s(3)        . . .       .
c .         s   =                 s(nd+1)     s(nd+2)     . . .       .
c .                                           s(2*nd)     . . .       .
c .                                                       . . .       .
c .                                                                   .
c .                                                                   .
c .                   a(1)        a(3)        a(6)        . . .       .
c .         a   =                 a(2)        a(5)        . . .       .
c .                                           a(4)        . . .       .
c .                                                       . . .       .
c .                                                                   .
c .                                                                   .
c .                                                                   .
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c
      dimension a(1),s(1)
      integer maxa(1),lm(1)
      common /addb/ neql,neqr,mla,nblock
c
c
      ndi=0
      do 200 i=1,nd
      ii=lm(i)
  100 if (ii.lt.neql .or. ii.gt.neqr) go to 200
      iim=ii-neql+1
      mi=maxa(iim)-mla
      ks=i
      do 220 j=1,nd
      jj=lm(j)
      if (jj) 220,220,110
 110  ij=ii - jj
      if (ij) 220,210,210
 210  kk=mi + ij
      kss=ks
      if (j.ge.i) kss=j + ndi
      a(kk)=a(kk) + s(kss)
  220 ks=ks + nd - j
  200 ndi=ndi + nd - i
c
      return
      end
*deck addres
      subroutine addres(maxa,mht)
c
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c .                                                                   .
c .   p r o g r a m                                                   .
c .      . to calculate addresses of diagonal elements in banded      .
c .        matrix whose column heights are known                      .
c .                                                                   .
c .        mht  = active column heights                               .
c.         maxa = addresses of diagonal elements                      .
c .                                                                   .
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c
      common/number/nmax,numnp,nume,neq,nwk,ma
      dimension maxa(2),mht(1)
c
      maxa(1)=1
      maxa(2)=2
      ma=0
      if (neq.eq.1) go to 20
      do 10 i=2,neq
      if (mht(i).gt.ma) ma=mht(i)
   10 maxa(i+1)=maxa(i) + mht(i) + 1
   20 ma=ma + 1
      nwk=maxa(neq+1) - maxa(1)
c
      return
      end
*deck assem
      subroutine assem
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/scom/nload,iloads,mvlast
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/tcom/munit,lunit,nstif,nred,nunit
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/ncom/numest,nvect,istoh,maxbh
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common /addb/ neql,neqr,mla,nblock
      common/dtwo/itwo,ndof,idim
      common/list/data(64),ndum
      common/tapcom/nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlin
      dimension ia(1)
      common a(1)
      real a
      integer ia
      equivalence (a(1),ia(1))
      ind=1
      nvect=data(3)
      mbh=100+neq/16
      if(nvect.le.0)nvect=neq
      lbs=2*nvect
      if(nvect.ne.neq)lbs=lbs*2
      lbs=5*neq
      call addres(ia(n1),ia(n1l))
      n1a=n1+neq+1
      n1b=n1a+neq
      iblock=4
      nblock=1
      if(numest.lt.neq)numest=neq
      mstore=1+lbs*itwo+numest+nsave*itwo+ng
c
c        calculate maximum block size for static solution
c
   30 mel=2*iblock
      istorl=(nmax-mstore-mel-2*mbh)/itwo
      call sblock(a(n1),a(n1a),a(n1b),istorl,nblock,neq,nwk,istoh)
      if(nblock.le.iblock)go to 40
      iblock=iblock*2
      if(iblock.le.256)go to 30
      write(6,2020)iblock
      stop
   40 continue
      nn=n1a+nblock-1
      maxbh=0
      do 50 i=n1a,nn
      maxbh=max0(maxbh,ia(i))
   50 continue
      maxbh=maxbh+1
      if(maxbh.le.mbh)go to 55
      mbh=2*mbh
      go to 30
   55 continue
      mam=nwk/neq+1
      write(6,2040)neq,nwk,ma,mam,istoh,nblock
      write(6,2050)(i,i=1,nblock)
      write(6,2060)(ia(i),i=n1a,nn)
      nn=n1b+nblock-1
      write(6,2070)(ia(i),i=n1b,nn)
      nn=n1a+nblock
      do 60 i=1,nblock
   60 ia(nn+i-1)=ia(n1b+i-1)
      n1b=nn
      n1c=n1b+nblock
      n2=n1c
      call open(istoh,nblock,icheck)
      khbb=0
      do 70 i=1,nblock
      call wecs(2,a(khbb+n1),maxbh,i)
      khbb=khbb+ia(n1a+i-1)
   70 continue
      npt=2*nblock
      n5l=nmax-ng+1
      n4l=n5l-nblock
      n3l=n4l-nblock
      n2l=n3l-maxbh
      n1l=n2l-maxbh
      if(nblock.eq.1)n1l=n2l
      do 80 i=1,npt
      ia(n3l+i-1)=ia(n1a+i-1)
   80 continue
c
c        preassem pass
c
      n6e=n1+nblock*ng*itwo
      n7=n6e+numest
      min=nmax+n7-n3l+1
      write(6,2090)min
      if(min.gt.nmax)stop
      ind=4
      call preaem(a(n1),a(n5l),a(n3l),a(n6e),nblock)
      ind=1
c
c        setup assembly storage
c
      n2=n1
      n3=n2+neq*itwo
      n4=n3+istoh*itwo
      n5=n4+istoh+itwo
      if(nblock.le.1)n5=n4
      n6e=n5
      n7=n6e+numest
      min=nmax+n7-n1l+1
      write(6,2080)min
      if(min.le.nmax)go to 100
      stop
  100 continue
      call zero(a(n2),neq)
      call assem1(a(n3),a(n4),a(n5),a(n6e),a(n1l),a(n3l),a(n5l),
     .   istoh,maxbh,munit,0)
      return
 2020 format(1h0,10x,25herror number of blocks = ,i10)
 2040 format(1h1,
     .17htotal system data ,///5x,
     .55hnumber of equations                                =   ,i9//5x,
     .55hnumber of matrix elements                          =   ,i9//5x,
     .55hmaximum half bandwidth                             =   ,i9//5x,
     .55hmean half bandwidth                                =   ,i9//5x,
     .55hmaximum block length                               =   ,i9//5x,
     .55hnumber of blocks                                   =   ,i9//)
 2050 format(10x,16h number of block ,12x,(14i6,/))
 2060 format(10x,28h number of columns per block ,(14i6/))
 2070 format(10x,21h first coupling block ,7x,(14i6/))
 2080 format(1h0,10x,41hstorage required to assemble stiffness = ,i10)
 2090 format(1h0,10x,32hstorage required for preassem = ,i10)
      end
*deck zero
      subroutine zero(a,neq)
      dimension a(1)
      do 10 i=1,neq
      a(i)=0.
   10 continue
      return
      end
*deck assem1
      subroutine assem1(a,aa,cc,ee,maxa,ncolbv,ke,istoh,maxbh,munit,ifl)
      common/scom/nload,iloads,mvlast
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/addb/neql,neqr,mla,nblock
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/flgs1/ibig,idamp
      common/tapcom/nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlin
      dimension a(1),aa(1),cc(1),maxa(1),ncolbv(1),ee(1)
      dimension ke(1)
      if(icheck.eq.1)return
      rewind 9
      if(ifl.gt.0)go to 60
c
c        form stiffness matrix
c
      if(ng.eq.1)call eread(ee,1)
      ipt=0
      neql=1
      neqr=0
      mla=0
      n2s=n2
      do 40 l=1,nblock
      read(9)(ke(i),i=1,ng)
      call recs(munit,maxa,maxbh,l)
      mla=maxa(1)-1
      neqr=neqr+ncolbv(l)
c
c        one solution block
c
      do 5 i=1,istoh
      a(i)=0.
    5 continue
      do 10 i=1,ng
      if(ke(i).eq.0)go to 10
      if(ng.ne.1)call eread(ee,i)
      n2=n2s
      kpri=1
      call gstiff
   10 continue
      n2=n2s
      ipt=ipt+1
      call wecs(4,a,istoh,ipt)
      neql=neql+ncolbv(l)
   40 continue
      return
   60 continue
c
c        final assembly
c
      if(ng.eq.1)call eread(ee,1)
      neqr=0
      neql=1
      do 90 l=1,nblock
      read(9)(ke(i),i=1,ng)
      call recs(munit,maxa,maxbh,l)
      mla=maxa(1)-1
      neqr=neqr+ncolbv(l)
      do 70 i=1,istoh
      a(i)=0.
   70 continue
      do 80 i=1,ng
      if(ke(i).eq.0)go to 80
      if(ng.ne.1)call eread(ee,i)
      kpri=1
      call gstiff
   80 continue
   85 continue
      call wecs(4,a,istoh,l)
      neql=neql+ncolbv(l)
   90 continue
      return
      end
*deck colht
      subroutine colht (mht,nd,lm)
c
      dimension lm(1),mht(1)
c
      ls=500000
      do 100 i=1,nd
      if (lm(i)) 110,100,110
  110 if (lm(i)-ls) 120,100,100
  120 ls=lm(i)
  100 continue
c
      do 200 i=1,nd
      ii=lm(i)
      if (ii.eq.0) go to 200
      me=ii - ls
      if (me.gt.mht(ii)) mht(ii)=me
 200  continue
c
      return
      end
*deck echeck
      subroutine echeck (lm,nd,icode)
      integer lm(1)
      common /addb/ neql,neqr,mla,nblock
c
      icode=1
      do 100 i=1,nd
      if(lm(i).lt.neql)go to 100
      if(lm(i).gt.neqr)go to 100
      icode=0
      go to 200
  100 continue
  200 return
      end
*deck ffin
      subroutine ffin
c        free format reader......dale ranta
c        slash...............data continues on next card
c        comma...............separates data
      common /list/ data(64)
      dimension sum(80),x(13),stemp(16),edum(2)
      data x/1h1,1h2,1h3,1h4,1h5,1h6,1h7,1h8,1h9,1h0,1h.,1h+,1h-/
      data blank,comma,dollar,slash/1h ,1h,,1h$,1h/ /
      data iswap/0/
      data wtap,wnop,iprint/3htap,3hnop,1/
      iline=1
   90 continue
      iline=iline+1
      nword=0
  120 continue
c      following read sum and if(eof) cards are for cdc version only
c      and are inactive due to comment card symbol c in front
      read(5,3000,end=370)sum
  125 continue
      if(iprint.eq.1)write(6,3010)iline,sum
      ichar=0
  130 nword=nword+1
      if (nword.gt.64) go to 360
  140 ichar=ichar+1
      if (ichar.gt.80) go to 360
      if (sum(ichar).eq.dollar) go to 360
      if(sum(ichar).eq.slash) go to 120
      if (sum(ichar).eq.comma) go to 130
      if (sum(ichar).eq.blank) go to 140
      iflag=0
      do 160 i=1,13
  160 if (sum(ichar).eq.x(i)) iflag=1
      ifirst=ichar
  180 continue
      if (ichar.gt.79) go to 200
      if (sum(ichar).eq.comma) go to 200
      if (sum(ichar).eq.dollar) go to 200
      if (sum(ichar).eq.slash) go to 200
      if (sum(ichar).eq.blank) go to 200
      ichar=ichar+1
      go to 180
  200 continue
      nlast=ichar-1
      do 220 i=1,16
  220 stemp(i)=blank
      n=0
      do 240 i=ifirst,nlast
      if (sum(i).eq.blank) go to 240
      n=n+1
      stemp(n)=sum(i)
      if(n.gt.15)go to 260
  240 continue
  260 continue
      if (iflag.ne.1) go to 320
      do 280 i=1,16
      m=17-i
      if (n.eq.0) go to 280
      save=stemp(n)
      stemp(n)=blank
      stemp(m)=save
      n=n-1
  280 continue
      encode(16,3020,edum)stemp
      decode(16,3040,edum)data(nword)
      go to 340
  320 continue
      encode(8,3050,data(nword))(stemp(i),i=1,8)
  340 continue
      if(sum(ichar).eq.slash) go to 120
      if(sum(ichar).eq.dollar)go to 360
      go to 130
  360 continue
      if(data(1).eq.wtap)go to 370
      if(data(1).eq.wnop)go to 380
      nword=nword-1
      write(11)nword,(data(i),i=1,nword)
      go to 90
  370 continue
      rewind 11
      return
  380 continue
      iprint=0
      go to 90
 3000 format(80a1)
 3010 format(i5,5x,80a1)
 3020 format(16a1)
 3030 format(4a1)
 3040 format(e16.0)
 3050 format(8a1)
      end
*deck gstiff
      subroutine gstiff
      common/comtyp/itype
      go to (10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,
     .  170),itype
   10 continue
c      call react
      return
   20 continue
c      call tapein
      return
   30 continue
c      call con
      return
   40 continue
c      call user
      return
   50 continue
      call star4
      return
   60 continue
c      call tri
      return
   70 continue
c      call beam
      return
   80 continue
c      call axsh
      return
   90 continue
c      call pquad4
      return
  100 continue
c      call hex8
      return
  110 continue
c      call phex
      return
  120 continue
c      call ptri
      return
  130 continue
c      call beam3d
      return
  140 continue
c      call atri
       return
  150 continue
c c    call aqua
       return
  160 continue
c     call ashl
      return
  170 continue
c      call astf
      return
      end
*deck load
      subroutine load(fr,id,ee,fc,ndof,nvect,ifg)
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat,kpdvec
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/scom/nload,iloads,mvlast
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/simcom/x0,y0,z0,insim,incon
      common/tapcom/nt11,nt31
      common/list/data(64),ndum
      common/prcom/ipress
      common/ewcom1/il(128)
      dimension fr(1),ee(1),id(ndof,1),fc(1)
      data end/3hend/
      if(ifg.gt.0)go to 110
      rewind 8
      read(8)((id(i,j),i=1,ndof),j=1,numnp)
      nstep=1
      ierr=0
      ns=0
    5 continue
c
c        read in loads
c
      ns=ns+1
      if(ipress.gt.0)go to 15
      do 10 i=1,neq
      fr(i)=0.
   10 continue
   15 continue
      ipress=0
      if(kpload.gt.0)write(6,2000)ns
   20 continue
      call fld
      if(data(1).eq.end)go to 50
      nod=data(1)
      val=data(2)
      idof=data(3)
      if(idof.le.0)idof=1
      if(kpload.gt.0)write(6,2010)nod,val,idof
      if(nod.ge.1.and.nod.le.numnp)go to 30
      write(6,2020)
      ierr=1
      go to 20
   30 continue
      inode=id(idof,nod)
      if(inode.eq.0)go to 20
      fr(inode)=fr(inode)+val
      go to 20
   50 if(incon.eq.1)go to 65
      do 60 n=1,ng
      if(il(n).eq.0)go to 60
      iloads=il(n)
      call eread(ee,n)
      call gstiff
   60 continue
      incon=1
   65 continue
      nload=(neq-1)/nvect+1
      nloc=0
      do 80 i=1,nload
      do 70 j=1,nvect
      fc(j)=0.
      nloc=nloc+1
      if(nloc.gt.neq)go to 70
      fc(j)=fr(nloc)
   70 continue
      call wecs(3,fc,nvect,i)
   80 continue
      do 90 i=1,nload
      call recs(3,fc,nvect,i)
      call wecs(3,fc,nvect,i+nload)
   90 continue
      mvlast=neq-(nload-1)*nvect
      if(ierr.ne.0)stop
      return
  110 if(ifg.gt.1)go to 160
c
c        write load vector to random file
c
      do 150 nl=1,nload
      call recs(3,fr,nvect,nl+nload)
      call wecs(3,fr,nvect,nl)
  150 continue
      return
  160 continue
      nloc=0
      mv=nvect
      do 180 nl=1,nload
      call recs(3,fr,nvect,nl+nload)
      if(nl.eq.nload)mv=mvlast
      do 170 j=1,mv
      nloc=nloc+1
      fc(nloc)=fr(j)
  170 continue
  180 continue
      nloc=0
      do 210 nl=1,nload
      do 200 j=1,nvect
      fr(j)=0.
      nloc=nloc+1
      if(nloc.gt.neq)go to 200
      fr(j)=fc(nloc)
  200 continue
      call wecs(3,fr,nvect,nl)
      call wecs(3,fr,nvect,nl+nload)
  210 continue
      return
 2000 format(1h0,10x,63hn o d a l   p o i n t   f o r c e s   f o r   l
     .o a d   c a s e ,i3,//
     .  7x,4hnode,5x,5hvalue,4x,3hdof)
 2010 format(i10,1pe12.4,2i5)
 2020 format(5x,19hstop undefined node)
      end
*deck nodep
      subroutine nodep
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/dtwo/itwo,ndof,idim
      common/list/data(64)
      common a(1)
      real a
      numnp=data(2)
      ndof=data(3)
      idim=data(4)
      if(numnp.gt.0)go to 10
      write(6,100)numnp
      stop
   10 continue
      n2=n1
      n3=n2+numnp*itwo
      n4=n3+numnp*itwo
      n5=n4+numnp*itwo
      if(idim.lt.1)n3=n2
      if(idim.lt.2)n4=n3
      if(idim.lt.3)n5=n4
      n2l=nmax-ndof*numnp+1
      min=nmax+n5-n2l+1
      write(6,90)min
c
c        check storage for nodal point input
c
      if(min.le.nmax)go to 20
      write(6,110)n5,numnp
      stop
   20 continue
      call nodgen(a(n2l),a(n2),a(n3),a(n4),numnp,neq,ndof,idim)
      return
   90 format(1h0,10x,39hstorage required to input nodal data = ,i10)
  100 format(1h0,5x,24herror number of nodes = ,i10)
  110 format(1h0,5x,24herror space requested = ,i10,3x,
     .  18hspace available = ,i10)
      end
*deck nodgen
      subroutine nodgen(id,x,y,z,numnp,neq,ndof,idim)
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat,kpdvec
      common/list/data(64)
c
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c .                                                                   .
c .   p r o g r a m                                                   .
c .     1. to read and print nodal point input data
c .     2. to generate and print all nodal data
c .     3. to calculate equation numbers and store them in id arrray  .
c .                                                                   .
c .           n=element number                                        .
c .           id=boundary condition codes (0=free,1=fixed)            .
c .           x,y,z= coordinates                                      .
c .           kn= generation code, i.e. increment on nodal point no   .
c .                                                                   .
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c
c
      common/tapcom/nt11
      dimension x(1),y(1),z(1),id(ndof,1)
      dimension iprc(4)
c
      data iprc /1h ,1ha,1hb,1hc /
      data end,free/3hend,4hfree/
c
c
c
      nold=0
      dum=0.
      rad=datan(1.d0)/45.0
      knold=0
      feet=1.0
      it=iprc(1)
   10 continue
      call fld
      if (data(1).ne.4hfeet) go to 13
      feet=12.0
      if (data(2).ne.0.0) feet=data(2)
      go to 10
   13 continue
      if(data(1).ne.4hcart)go to 11
      it=iprc(1)
      go to 10
   11 if(data(1).ne.4hpola) go to 12
      it=iprc(2)
      go to 10
   12 continue
      n=data(1)
      x(n)=data(2)
      y(n)=data(3)
      if(idim.eq.3)z(n)=data(4)
      kn=data(5)
      if(kn.le.0)kn=1
      if(it.eq.iprc(1))go to 20
      dum=z(n)*rad
      z(n)=y(n)*sin(dum)
      y(n)=y(n)*cos(dum)
   20 continue
      if (knold.eq.0) go to 50
      num=(n-nold) / knold
      numn=num-1
      if(numn.lt.1) go to 50
      xnum=num
      dx=(x(n)-x(nold))/xnum
      if (it.eq.iprc(1)) go to 21
      rold=y(nold)/cos(dumold)
      rnew=y(n)/cos(dum)
      dr=(rnew-rold)/xnum
      dt=(dum-dumold)/xnum
      go to 22
   21 dy=(y(n)-y(nold))/xnum
      dz=(z(n)-z(nold))/xnum
   22 k=nold
      do 30 j=1,numn
      kk=k
      k=k + knold
      x(k)=x(kk)+dx
      if (it.eq.iprc(1)) go to 26
      rold=rold+dr
      dumold=dumold+dt
      y(k)=rold*cos(dumold)
      z(k)=rold*sin(dumold)
      go to 28
   26 y(k)=y(kk)+dy
      z(k)=z(kk)+dz
   28 continue
   30 continue
   50 nold=n
      knold=kn
      dumold=dum
      if (n.lt.numnp) go to 10
      do 62 n=1,numnp
      x(n)=feet*x(n)
      y(n)=feet*y(n)
   62 z(n)=feet*z(n)
      do 70 n=1,numnp
      do 70 j=1,ndof
      id(j,i)=0
   70 continue
   75 continue
      call fld
      if(data(1).eq.end)go to 90
      nstart=data(3)
      nend=data(4)
      inc=data(5)
      if(nend.le.numnp.and.nstart.ge.1.and.nstart.le.numnp)go to 80
      write(6,2100)nstart,nend
      go to 75
   80 continue
      if(nend.lt.nstart)nend=nstart
      ipoint=data(2)
      if(inc.le.0)inc=1
      do 85 i=nstart,nend,inc
      do 85 j=1,ndof
c      ip= .... card is to peel off the ip boundary cond 1 or 0
c      the shift( ,) is for cdc   the ishft(,) is for the vax
c      shift or ishft is a left circular shift routine
      ip=shiftr(ipoint,j-1).and.1
      if(ip.eq.0)go to 85
      id(j,i)=1
      if(data(1).eq.free)id(j,i)=0
   85 continue
      go to 75
   90 continue
c
      if(kpnode.eq.0)go to 52
      write(6,2000)
      write(6,2008)
      write(6,2005) (n,x(n),y(n),z(n),n=1,numnp)
   52 continue
c
c     number unknowns
c
      neq=0
      do 115 n=1,numnp
      do 100 i=1,ndof
      if (id(i,n)) 110,120,110
  120 neq=neq + 1
      id(i,n)=neq
      go to 100
  110 id(i,n)=0
  100 continue
  115 continue
c
      if(kpdof.eq.0)go to 200
      write(6,2004)
      do 250 n=1,numnp
      write(6,2006)n,(id(i,n),i=1,ndof)
  250 continue
  200 continue
c
c     store id array on tape 8
c
      rewind 8
      write (8) ((id(i,j),i=1,ndof),j=1,numnp)
      write(8)numnp,(x(i),i=1,numnp),(y(i),i=1,numnp),(z(i),i=1,numnp)
c
      return
c
 2000 format(1h1,33h n o d a l   p o i n t   d a t a   //)
 2004 format(17h0equation numbers//
     . 7x,1hn,7x,1hx,7x,1hy,7x,1hz,6x,2hxx,6x,2hyy,6x,2hzz,//)
 2005 format(i8,1p3e12.4)
 2006 format(7i8)
 2008 format(3x,5h node,4x,1hx,11x,1hy,11x,1hz)
 2100 format(1h0,10x,24hb.c. error first node = ,i5,5x,12hlast node = ,
     .    i5)
c
      end
*deck open
      subroutine open(istoh,nblock,icheck)
      common/com76/itap(8),itapu(8),nsec
      common/scom/nload,iloads,mvlast
      common/ncom/numest,nvect,istohc,maxbh
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/dtwo/itwo,ndof,idim
      common/elcom/nwelem
      data nr/10/
      if(nr.eq.1)return
      nr=1
      write(6,2000)
      nt=32
      nb=3*nblock
      lb=istoh*itwo
      nblkl=(lb-1)/nsec+1
      nblk=nblkl*nb
      nwords=nblk*nsec
      write(6,2010)nt,nb,lb,nwords,nblkl,nblk
      ier=0
c     open(unit=32,file='tape32',status='unknown',
c    .  form='unformatted',access='direct',
c    .  recl=nsec*8)
      if(ier.ne.0)go to 90
      nt=33
      nb=nblock
      lb=maxbh
      nblkl=(lb-1)/nsec+1
      nblk=nblkl*nb
      nwords=nblk*nsec
      write(6,2010)nt,nb,lb,nwords,nblkl,nblk
c     open(unit=33,file='tape33',status='unknown',
c    .  form='unformatted',access='direct',
c    .  recl=nsec*8)
      if(ier.ne.0)go to 90
      nt=34
      nb=(neq-1)/nvect+1
      nb=2*nb
      lb=2*nvect*itwo
      nblkl=(lb-1)/nsec+1
      nblk=nblkl*nb
      nwords=nblk*nsec
      write(6,2010)nt,nb,lb,nwords,nblkl,nblk
c     open(unit=34,file='tape34',status='unknown',
c    .  form='unformatted',access='direct',
c    .  recl=nsec*8)
      if(ier.ne.0)go to 90
      nt=35
      nb=nblock
      lb=2*istoh*itwo
      nblkl=(lb-1)/nsec+1
      nblk=nblkl*nb
      nwords=nblk*nsec
      write(6,2010)nt,nb,lb,nwords,nblkl,nblk
c     open(unit=35,file='tape35',status='unknown',
c    .  form='unformatted',access='direct',
c    .  recl=nsec*8)
      if(ier.ne.0)go to 90
      nt=36
      nb=nblock
      lb=2*istoh*itwo
      nblkl=(lb-1)/nsec+1
      nblk=nblkl*nb
      nwords=nblk*nsec
      write(6,2010)nt,nb,lb,nwords,nblkl,nblk
c     open(unit=36,file='tape36',status='unknown',
c    .  form='unformatted',access='direct',
c    .  recl=nsec*8)
      if(ier.ne.0)go to 90
      nt=37
      nb=(neq-1)/nvect+1
      nb=2*nb
      lb=2*nvect*itwo
      nblkl=(lb-1)/nsec+1
      nblk=nblkl*nb
      nwords=nblk*nsec
      write(6,2010)nt,nb,lb,nwords,nblkl,nblk
c     open(unit=37,file='tape37',status='unknown',
c    .  form='unformatted',access='direct',
c    .  recl=nsec*8)
      if(ier.ne.0)go to 90
      return
   90 continue
      write(6,1000)nt,ier
      stop
      entry opene
c
c          open element file
c
      if(nr.eq.1)return
      write(6,2000)
      nt=31
      nb=1
      lb=27*numnp*itwo+100000
      if(nwelem.gt.0)lb=nwelem
      nblkl=(lb-1)/nsec+1
      nblk=nb*nblkl
      nwords=nblk*nsec
      write(6,2010)nt,nb,lb,nwords,nblkl,nblk
      ier=0
c     open(unit=31,file='tape31',status='unknown',
c    .  form='unformatted',access='direct',
c    .  recl=nsec*8)
      return
 1000 format(5x,20hassign error file = ,i5,2x,8herror = ,i5)
 2000 format(1h0,5x,4hfile,8x,2hnb,4x,6hlength,4x,6hnwords,5x,5hnblkl,
     .       6x,4hnblk)
 2010 format(6i10)
      end
*deck sblock
      subroutine sblock (maxa,ncolbv,icopl,istorl,nblock,neq,nwk,istoh)
c
      integer maxa(1),ncolbv(1),icopl(1)
c
c     check for one block case
c
      if (nblock.gt.1) go to 20
      if (nwk.gt.istorl) go to 20
      nblock=1
      ncolbv(1)=neq
      icopl(1)=1
      istoh=nwk
      return
c
c     check whether istorl/2 is at least as large as any one column
c
   20 istoh=istorl/2
      istorl=2*istoh
      do 30 i=1,neq
      icl=maxa(i+1)-maxa(i)
      if (istoh.ge.icl) go to 30
      write (6,120) i
      stop
   30 continue
c
c     establish the number of columns per block
c
      nblock=0
      nn=0
      ib=0
c
      do 60 i=2,neq
   40 ii=istoh-maxa(i+1)+1+nn
      if (ii) 50,60,60
   50 nn=maxa(i)-1
      nblock=nblock+1
      ncolbv(nblock)=i-1-ib
      ib=i-1
      go to 40
   60 continue
      nblock=nblock+1
      ncolbv(nblock)=neq-ib
c
c     establish coupling of blocks
c
      do 70 i=1,nblock
   70 icopl(i)=i
      if (nblock.eq.1) return
      nn=ncolbv(1)
      do 110 n=2,nblock
      iclm=0
      ncolb=ncolbv(n)
      do 80 i=1,ncolb
      icl=maxa(nn+i+1)-maxa(nn+i)-i-1
      if (icl.gt.iclm) iclm=icl
   80 continue
      j=n-1
   90 if (iclm.le.0) go to 100
      icopl(n)=j
      iclm=iclm-ncolbv(j)
      j=j-1
      go to 90
  100 nn=nn+ncolbv(n)
  110 continue
c
      return
c
  120 format (1h1,49h***error high speed storage is too small to fit  ,6
     1hcolumn,2h (,i5,2h) ,10hinto core )
      end
*deck solve
      subroutine solve(iequit,istres,iback)
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/scom/nload,iloads,mvlast
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/tcom/munit,lunit,nstif,nred,nunit
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/ncom/numest,nvect,istoh,maxbh
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common /addb/ neql,neqr,mla,nblock
      common/dtwo/itwo,ndof,idim
      common/list/data(64),ndum
      dimension ia(1)
      common a(1)
      real a
      integer ia
      equivalence (a(1),ia(1))
      data end/3hend/
c
c        setup solution storage
c
      n3=n2+nvect*itwo
      n4=n3+nvect*itwo
      if(nvect.eq.neq)n4=n3
      n5=n4+istoh*itwo
      n6=n5+istoh*itwo
      if(nblock.eq.1)n6=n5
      n7=n6+nvect*itwo
      n8=n7+nvect*itwo
      if(nvect.eq.neq)n8=n7
      n6e=n8
      n15=n6e+numest
      n3a=n2+neq*itwo
      n41=n2+nvect*itwo
      n42=n2+numest
      n43=n2+ndof*numnp
      n15=max0(n15,n41,n42,n43)
      n5l=n4l+nblock
      min=nmax+n15-n1l+1
      if(iback.eq.0)write(6,2110)min
      if(min.le.nmax)go to 40
      stop
   40 continue
   50 continue
c
c        frequency sweep loop
c
      ind=2
      if(icheck.eq.1)return
      if(iback.eq.1)go to 60
      call assem1(a(n4),a(n5),a(n6),a(n6e),a(n1l),a(n3l),a(n5l),
     .   istoh,maxbh,
     .           munit,1)
      call colsol(a(n1l),a(n2l),a(n3l),a(n4l),a(n2),a(n3),a(n4),a(n5),
     .            a(n6),a(n7),nblock,neq,istoh,nvect,nvect,maxbh,
     .            nstif,nred,lunit,munit,nunit,1)
   60 continue
      n6e=n3a
      call load(a(n3a),a(n3a),a(n3a),a(n2),ndof,nvect,2)
      call colsol(a(n1l),a(n2l),a(n3l),a(n4l),a(n2),a(n3),a(n4),a(n5),
     .            a(n6),a(n7),nblock,neq,istoh,nvect,nvect,maxbh,
     .            nstif,nred,lunit,munit,nunit,2)
      call write(a(n2),a(n3a),a(n3a),ndof,nvect,lunit,0)
      if(istres.eq.1)call stress(a(n2),a(n6e),nvect,lunit)
      if(iequit.ne.0)call equit(a(n2),a(n3),a(n4),a(n6),a(n7),a(n1l),
     .                         a(n3l),a(n3a),nvect,
     .                         munit,nstif,lunit,nunit)
      return
 2110 format(1h0,10x,39hstorage requested for solution phase =  ,i10)
 2290 format(1h0,10x,42hstorage to store entire solution vector =    ,
     .       i10)
      end
*deck stress
      subroutine stress(ur,fr,nvect,nunit)
c
      common /flgs/ kpri,ind,nummat,midest,ng,icheck
      common/scom/nload,iloads,mvlast
c
      dimension ur(1),fr(1)
c
      if(icheck.eq.1)return
c
      ind=3
      kpri=1
      do 85 i=1,ng
      call eread(fr,i)
      call gstiff
   85 continue
      call edone
      ind=2
      return
      end
*deck wecs
      subroutine wecs(nt,a,in,nb)
      common/flgs1/ibig,idamp
      common/dtwo/itwo,ndof,idim
      dimension a(1)
      real a
      logical ifg
      data nt32,nt33,nt34,nt35,nt36/32,33,34,35,36/
      data nt37/37/
      data nt38/38/
      iw=in*itwo
      ifg=.true.
      go to (10,30,50,70,90,110,130),nt
   10 continue
      if(ifg)call writms(nt32,a,iw,nb,-1,0)
      if(.not.ifg)call readms(nt32,a,iw,nb)
      return
   30 continue
      if(ifg)call writms(nt33,a,in,nb,-1,0)
      if(.not.ifg)call readms(nt33,a,in,nb)
      return
   50 continue
      if(ifg)call writms(nt34,a,iw,nb,-1,0)
      if(.not.ifg)call readms(nt34,a,iw,nb)
      return
   70 continue
   75 if(ibig.eq.2)go to 10
      if(ifg)call writms(nt35,a,iw,nb,-1,0)
      if(.not.ifg)call readms(nt35,a,iw,nb)
      return
   90 continue
      if(ibig.ne.0)go to 75
      if(ifg)call writms(nt36,a,iw,nb,-1,0)
      if(.not.ifg)call readms(nt36,a,iw,nb)
      return
  110 continue
      if(ifg)call writms(nt37,a,iw,nb,-1,0)
      if(.not.ifg)call readms(nt37,a,iw,nb)
      return
  130 continue
      if(ifg)call writms(nt38,a,iw,nb,-1,0)
      if(.not.ifg)call readms(nt38,a,iw,nb)
      return
c
      entry recs(nt,a,in,nb)
c
      iw=in*itwo
      ifg=.false.
      go to (10,30,50,70,90,110,130),nt
      end
*deck write
      subroutine write(ur,fr,id,ndof,nvect,nunit,ifg)
      common/dyncm2/idym,imass,ires,intial,ncycle,nstep,nbdym
      common/stapes/nts1,ntb1,nts2,ntb2,nts3,ntb3
      common/prcom2/ibk,ipnt,idva(3),ipnode(3,20)
      common/dyncm1/time,dt,dtold,delta,alpha
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8,n9
      common a(1)
      dimension xat(10000),yat(10000)
      common/scom/nload,iloads,mvlast
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat,kpdvec
      dimension ur(1),fr(1),id(ndof,1)
      dimension dxr(6),drc(10)
c
      if(kpdis.eq.0.and.ifg.eq.0)return
      mv=nvect
      nloc=0
      do 20 i=1,nload
      call recs(nunit,fr,nvect,i)
      if(i.eq.nload)mv=mvlast
      do 10 j=1,mv
      nloc=nloc+1
      ur(nloc)=fr(j)
   10 continue
   20 continue
c
c        print solution
c
      rewind 8
      read(8)((id(i,j),i=1,ndof),j=1,numnp)
      read(8)numnp,(xat(iii),iii=1,numnp),(yat(jjj),jjj=1,numnp)
      if (ibk.gt.0)goto 05
      ibk=1
      ipnode(1,1)=1
      ipnode(2,1)=numnp
      ipnode(3,1)=1
      ipnt=1
      idva(1)=1
      idva(2)=1
      idva(3)=0
 05   continue
      if(ifg.le.0)write(6,1000)
      if (kpcomp.ne.1)write(6,1009)
 1009 format(3x,'NODE',9x,'X',11x,'Y',11x,'DX',10x,'DY',10x,'DR')
      if(ifg.eq.1)write(6,1030)
      if(ifg.eq.2)write(6,1040)
      kount=0
      kend=(10/ndof)*ndof
      do 90 nb=1,ibk
      in1=ipnode(1,nb)
      in2=ipnode(2,nb)
      inc=ipnode(3,nb)
      do 60 n=in1,in2,inc
      do 30 i=1,ndof
      ip=id(i,n)
      dxr(i)=0.
      if(ip.le.0)go to 30
      dxr(i)=ur(ip)
   30 continue
      if(kpcomp.eq.1)go to 40
      dr = sqrt(dxr(1)**2 + dxr(2)**2)
      write(6,1010)n,xat(n),yat(n),(dxr(i),i=1,ndof),dr
      go to 60
   40 continue
      do 50 i=1,ndof
      kount=kount+1
      drc(kount)=dxr(i)
   50 continue
      if(kount.lt.kend.and.(n+inc).le.in2) goto 60
      write(6,1050)n,(drc(i),i=1,kount)
      kount=0
   60 continue
 90    continue
      return
 1000 format(1h0,10x,49hn o d a l   p o i n t   d i s p l a c e m e n t
     .s ,///)
 1010  format(i5,5x,1p6e12.4)
 1020 format(10x,1p6e12.4)
 1030 format(1h0,10x,63hc a l c u l a t e d  n o d a l   p o i n t  f o
     .r c e s         ,//)
 1040 format(1h0,10x,59hr e s i d u a l  n o d a l  p o i n t  f o r c e
     . s         ,//)
 1050 format(i8,1p10e11.3)
 1060 format(8x,1p10e11.3)
      end
*deck writel
      subroutine writel(ur,fr,id,xat,yat,ndof,nvect,nunit,ifg)
      common/dyncm2/idym,imass,ires,intial,ncycle,nstep,nbdym
      common/stapes/nts1,ntb1,nts2,ntb2,nts3,ntb3
      common/prcom2/ibk,ipnt,idva(3),ipnode(3,20)
      common/dyncm1/time,dt,dtold,delta,alpha
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8,n9
      common a(1)
      common/scom/nload,iloads,mvlast
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat,kpdvec
      dimension ur(1),fr(1),id(ndof,1),xat(1),yat(1)
      dimension dxr(6),drc(10)
c
      rewind 8
      read(8)((id(i,j),i=1,ndof),j=1,numnp)
      read(8)numnp,(xat(iii),iii=1,numnp),(yat(jjj),jjj=1,numnp)
      if (ibk.gt.0)goto 05
      ibk=1
      ipnode(1,1)=1
      ipnode(2,1)=numnp
      ipnode(3,1)=1
      ipnt=1
      idva(1)=1
      idva(2)=1
      idva(3)=0
 05   continue
      if(ifg.le.0)write(6,1000)
      if (kpcomp.ne.1)write(6,1009)
 1009 format(3x,'NODE',9x,'X',11x,'Y',11x,'DX',10x,'DY',10x,'DR')
      if(ifg.eq.1)write(6,1030)
      if(ifg.eq.2)write(6,1040)
      kount=0
      kend=(10/ndof)*ndof
      do 90 nb=1,ibk
      in1=ipnode(1,nb)
      in2=ipnode(2,nb)
      inc=ipnode(3,nb)
      do 60 n=in1,in2,inc
      do 30 i=1,ndof
      ip=id(i,n)
      dxr(i)=0.
      if(ip.le.0)go to 30
      dxr(i)=ur(ip)
   30 continue
      if(kpcomp.eq.1)go to 40
      dr = sqrt(dxr(1)**2 + dxr(2)**2)
      write(6,1010)n,xat(n),yat(n),(dxr(i),i=1,ndof),dr
      go to 60
   40 continue
      do 50 i=1,ndof
      kount=kount+1
      drc(kount)=dxr(i)
   50 continue
      if(kount.lt.kend.and.(n+inc).le.in2) goto 60
      write(6,1050)n,(drc(i),i=1,kount)
      kount=0
   60 continue
 90    continue
      return
 1000 format(1h0,10x,49hn o d a l   p o i n t   d i s p l a c e m e n t
     .s ,///)
 1010  format(i5,5x,1p6e12.4)
 1020 format(10x,1p6e12.4)
 1030 format(1h0,10x,63hc a l c u l a t e d  n o d a l   p o i n t  f o
     .r c e s         ,//)
 1040 format(1h0,10x,59hr e s i d u a l  n o d a l  p o i n t  f o r c e
     . s         ,//)
 1050 format(i8,1p10e11.3)
 1060 format(8x,1p10e11.3)
      end
*deck write50
      subroutine write50(ur,fr,id,xat,yat,ndof,nvect,nunit,ifg)
      common/dyncm2/idym,imass,ires,intial,ncycle,nstep,nbdym
      common/stapes/nts1,ntb1,nts2,ntb2,nts3,ntb3
      common/prcom2/ibk,ipnt,idva(3),ipnode(3,20)
      common/dyncm1/time,dt,dtold,delta,alpha
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8,n9
      common a(1)
      common/scom/nload,iloads,mvlast
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat,kpdvec
      dimension ur(1),fr(1),id(ndof,1),xat(1),yat(1)
      dimension dxr(6),drc(10)
c
      rewind 8
      read(8)((id(i,j),i=1,ndof),j=1,numnp)
      read(8)numnp,(xat(iii),iii=1,numnp),(yat(jjj),jjj=1,numnp)
      do 60 n=1,numnp
      do 30 i=1,ndof
      ip=id(i,n)
      dxr(i)=0.
      if(ip.le.0)go to 30
      dxr(i)=ur(ip)
   30 continue
      xat(n)=xat(n)+dxr(1)
      yat(n)=yat(n)+dxr(2)
   60 continue
      write(50)numnp,(xat(i),i=1,numnp),(yat(i),i=1,numnp)
      return
      end
*deck writms
      subroutine writms(iunit,a,iword,irec,idum1,idum2)
      dimension a(1)
      common/com76/itap(8),itapu(8),nsec
      real a
      data itap/31,32,33,34,35,36,37,38/
c      data itap/6ltape31,6ltape33,6ltape34,6ltape35,
c     .          6ltape36,6ltape37,6ltape38,6ltape39/
      data nsec,itapu/512,8*0/
      npru=(iword-1)/nsec+1
      nword=npru*nsec
      id=npru*(irec-1)+1
      iu=iunit-30
      if(id.le.itapu(iu))go to 10
      itapu(iu)=id
c      id=0
   10 continue
      call fwrite(itap(iu),a,nword,id)
      return
      end
*deck readms
      subroutine readms(iunit,a,iword,irec)
      dimension a(1)
      common/com76/itap(8),itapu(8),nsec
      common/bufcom/abuf(512)
      real a,abuf
      npruw=(iword-1)/nsec+1
      nword=npruw*nsec
      nprur=iword/nsec
      iwords=nprur*nsec
      do 10 i=1,nsec
      abuf(i)=a(iwords+i)
   10 continue
      iu=iunit-30
      id=npruw*(irec-1)+1
      call fread(itap(iu),a,nword,id)
      if(npruw.eq.nprur)return
      nlm=iword-iwords+1
      do 30 i=nlm,nsec
      a(iwords+i)=abuf(i)
   30 continue
      return
      end
*deck fread
      subroutine fread(iu,a,nword,nrec)
      common/com76/itap(8),itapu(8),nsec
      dimension a(1)
      real a
      kend=(nword-1)/nsec+1
      is=1
      iend=nsec
      krec=nrec
c     do 10 k=1,kend
c     read(iu,rec=krec)(a(i),i=is,iend)
c     is=is+nsec
c     krec=krec+1
c     iend=iend+nsec
c  10 continue
      call rfile(iu,a,nword,(nrec-1)*nsec)
      return
      end
*deck fwrite
      subroutine fwrite(iu,a,nword,nrec)
      common/com76/itap(8),itapu(8),nsec
      dimension a(1)
      real a
      kend=(nword-1)/nsec+1
      is=1
      iend=nsec
      krec=nrec
c     do 10 k=1,kend
c     write(iu,rec=krec)(a(i),i=is,iend)
c     is=is+nsec
c     krec=krec+1
c     iend=iend+nsec
c  10 continue
      call wfile(iu,a,kend*nsec,(nrec-1)*nsec)
      return
      end
*deck rfile
      subroutine rfile(ntape,a,nwords,npos)
      dimension a(1)
      call rdabsf(ntape,a,nwords,npos)
      if(unit(ntape))10,900,900
10    continue
      return
900   continue
      write(6,1000)ntape,nwords,npos
      return
1000  format(5x,*error-ntape,nwords,npos*,5i6)
      end
*deck wfile
      subroutine wfile(ntape,a,nwords,npos)
      dimension a(1)
      call wrabsf(ntape,a,nwords,npos)
      if(unit(ntape))10,900,900
10    continue
      return
900   continue
      write(6,1000)ntape,nwords,npos
      return
1000  format(5x,*error-ntape,nwords,npos*,5i6)
      end
*deck fld
      subroutine fld
      common/list/data(64),nword
      read(11,end=900)nw,(data(i),i=1,nw)
    5 continue
      nwp=nw+1
      do 10 i=nwp,64
      data(i)=0.
   10 continue
      return
  900 continue
      write(6,1000)
      stop
 1000 format(5x,27hstop - end of file on input)
      end
*deck preaem
      subroutine preaem(ntable,ke,ncolbv,ee,nblock)
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/addb/neql,neqr,mla,nblo
      common/comtyp/itype,ngroup,ngin,nlin
      dimension ee(1),ntable(nblock,1),ke(1),ncolbv(1)
      if(ng.gt.1)write(6,1000)
      do 20 n=1,ng
      call eread(ee,n)
      neql=1
      neqr=0
      do 10 l=1,nblock
      neqr=neqr+ncolbv(l)
      ngin=0
      call gstiff
      ntable(l,n)=ngin
      neql=neql+ncolbv(l)
   10 continue
   20 continue
      rewind 9
      do 50 l=1,nblock
      do 40 j=1,ng
      ke(j)=ntable(l,j)
   40 continue
      write(9)(ke(i),i=1,ng)
      if(ng.gt.1)write(6,1010)(ke(i),i=1,ng)
   50 continue
      write(6,1020)
      return
 1000 format(1h0,5x,19helement group table ,/)
 1010 format(4x,128i1)
 1020 format(1h0)
      end
*deck ewrite
      subroutine ewrite(e,n)
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,new,nwk,ma
      common/comtyp/itype,ngroup,ngin,nlin
      common/dtwo/itwo,ndof,idim
      common/ewcom/ia(2,200),isec,irl
      common/scom/nload,iloads,mvlast
      common/ewcom1/il(128)
      dimension e(4)
      common/com76/itap(8),itapu(8),nsec
      real e
      nw=midest+4*itwo
      midest=nw
      il(n)=iloads
      if(n.ne.1)go to 10
      isec=0
      go to 10
      entry ewrit2(e,n)
      n=irl
      nw=ia(1,n)
      isec=ia(2,n)
   10 continue
      e(1)=nume
      e(2)=nummat
      e(3)=itype
      e(4)=nw
      call hsiow(31,e,nw,isec)
      ia(1,n)=nw
      ia(2,n)=isec
      isec=isec+(nw-1)/nsec+1
      return
      entry eread(e,n)
      nw=ia(1,n)
      isec=ia(2,n)
      call hsior(31,e,nw,isec)
      nume=e(1)
      nummat=e(2)
      itype=e(3)
      nwt=e(4)
      irl=n
      if(nwt.eq.nw)return
      write(6,1000)nw,nwt
      stop
 1000 format(1h0,10x,15herror in ewrite ,2i10)
      end
*deck hsiow
      subroutine hsiow(iunit,a,iword,npos)
      dimension a(1)
      common/com76/itap(8),itapu(8),nsec
      real a
      npru=(iword-1)/nsec+1
      nword=npru*nsec
      id=npos+1
      iu=iunit-30
      if(id.le.itapu(iu))go to 10
      itapu(iu)=id
c      id=0
   10 continue
      call fwrite(itap(iu),a,nword,id)
      return
      end
*deck hsior
      subroutine hsior(iunit,a,iword,npos)
      common/bufcom/abuf(512)
      common/com76/itap(8),itapu(8),nsec
      dimension a(1)
      real a,abuf
      npruw=(iword-1)/nsec+1
      nword=npruw*nsec
      nprur=iword/nsec
      iwords=nprur*nsec
      do 10 i=1,nsec
      abuf(i)=a(iwords+i)
   10 continue
      id=npos+1
      iu=iunit-30
      call fread(itap(iu),a,nword,id)
      if(npruw.le.nprur)return
      nlm=iword-iwords+1
      do 30 i=nlm,nsec
      a(iwords+i)=abuf(i)
   30 continue
      return
      end
 
*deck istress
      subroutine istress(nel,mat,itype,nint,xx,sig0,vmin0,rn0,
     .                  en0,bn0)
      real xplt(300),yplt(300),rs(300),rr
c
      common/simcom/x0,y0,z0,insim,incon
      common/eqss/estcon(30,28)
      common/eqss2/estcn2(30,28)
      common/alpcom/coef(124,28)
 
      dimension rsi(300),dc(300),dm(300),e(300),s1d(300),s2d(300)
      dimension bjf1(300),bjf2(300)
      dimension b(4,8),xx(2,4),xg(4,4),wgt(4,4),hh(2,8)
      dimension sig0(4,4),vmin0(4),rn0(4),en0(4),bn0(4,4)
      dimension sigi(3),an(3),sigxyz(6)
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
      if(nel.ne.1)go to 10
      rewind 19
      read(19)nc,tim,nr
      read(19)(rsi(j),d,xplt(j),yplt(j),e(j),bjf1(j),bjf2(j),s1d(j),
     .  s2d(j),d,d,j=1,nr)
      do 5 i=1,nr
      dc(i)=xplt(i)
      dm(i)=yplt(i)
      rs(i)=rsi(i)
    5 continue
c
   10 continue
      kint=0
      do 180 lx=1,nint
      ri=xg(lx,nint)
      do 180 ly=1,nint
      si=xg(ly,nint)
      kint=kint+1
      call stom(xx,b,det,hh,ri,si,xbar,nel,itype)
      xp=0.
      yp=0.
      zp=0.
      do 20 l=1,4
      xp=xp+hh(2,2*l)*xx(1,l)
      yp=yp+hh(2,2*l)*xx(2,l)
   20 continue
      vx=xp-x0
      vy=yp-y0
      vz=0.
      rr=sqrt(vx**2+vy**2+vz**2)
      call index1(nr,rs,rr,n)
      if(n.gt.0)go to 40
      n=1
      rr=rs(n)
   40 if(n.lt.nr)go to 50
      n=nr-1
      rr=rs(nr)
   50 continue
      rf=(rr-rs(n))/(rs(n+1)-rs(n))
      es=e(n)+(e(n+1)-e(n))*rf
      dms=dm(n)+(dm(n+1)-dm(n))*rf
      dcs=dc(n)+(dc(n+1)-dc(n))*rf
      s1s=s1d(n)+(s1d(n+1)-s1d(n))*rf
      s2s=s2d(n)+(s2d(n+1)-s2d(n))*rf
      bj1=bjf1(n)+(bjf1(n+1)-bjf1(n))*rf
      bj2=bjf2(n)+(bjf2(n+1)-bjf2(n))*rf
   60 continue
      es=es
      vmin=0.
      if(dms.ne.0.)vmin=1./dms
      vf=0.
      if(dcs.ne.0.)vf=1./dcs
      sigi(3)=s1s
      sigi(2)=s2s
      sigi(1)=sigi(2)
      an(1)=vx
      an(2)=vy
      an(3)=vz
      call ovrot(sigi,an,sigxyz)
      ps=-(sigxyz(1)+sigxyz(2)+sigxyz(3))/3.
      call hug(mat,-1.0,vmin,vf,es,ps,vnew,enew)
      rn0(kint)=0.
      if(vnew.ne.0.)go to 170
      write(6,1000)nel,kint
      stop
  170 continue
      rn0(kint)=1./vnew
      en0(kint)=enew
      vmin0(kint)=vmin
      sig0(1,kint)=sigxyz(1)
      sig0(2,kint)=sigxyz(2)
      sig0(3,kint)=sigxyz(4)
      sig0(4,kint)=sigxyz(3)
      sigi(3)=bj1
      sigi(2)=bj2
      sigi(1)=sigi(2)
      call ovrot(sigi,an,sigxyz)
      bn0(1,kint)=sigxyz(1)
      bn0(2,kint)=sigxyz(2)
      bn0(3,kint)=sigxyz(4)
      bn0(4,kint)=sigxyz(3)
  180 continue
      return
 1000 format('vnew = 0. nel = ',i5,' kint = ',i5)
      end
*deck sstress
      subroutine sstress(nel,mat,itype,nint,xx,sig0,vmin0,rn0,
     .                  en0,bn0)
      common/eqss/estcon(30,28)
      common/eqss2/estcn2(30,28)
      common/alpcom/coef(124,28)
      common/var/povb,iplot,iprnt,matl
      common/eoscom/nn,vol,e,p,sndsqr,alpha,dalpha,
     *              alph,vmin,rmu,yield,dpdv,dpde
 
      dimension sig0(4,4),vmin0(4),rn0(4),en0(4),bn0(4,4)
      dimension sigxyz(4)
      matl=mat
      nn=mat
      read(81)sigxyz,vmin,es
      vf=estcon(1,mat)
      ps=-(sigxyz(1)+sigxyz(2)+sigxyz(3))/3.
      if(ps.gt.0.)go to 160
      vnew=vf
      enew=es
      go to 170
  160 continue
      call hug(nn,-1.0,vmin,vf,es,ps,vnew,enew)
      if(vnew.ne.0.)go to 170
      write(6,1000)nel,kint
      stop
  170 continue
      last=nint*nint
      do 180 kint=1,last
      rn0(kint)=1./vnew
      en0(kint)=enew
      vmin0(kint)=vmin
      sig0(1,kint)=sigxyz(1)
      sig0(2,kint)=sigxyz(2)
      sig0(3,kint)=sigxyz(4)
      sig0(4,kint)=sigxyz(3)
      bn0(1,kint)=sigxyz(1)
      bn0(2,kint)=sigxyz(2)
      bn0(3,kint)=sigxyz(4)
      bn0(4,kint)=sigxyz(3)
  180 continue
      return
 1000 format('vnew = 0. nel = ',i5,' kint = ',i5)
      end
*deck ovrot
      subroutine ovrot(sig,ac,sigxy)
c
c        rotate stress and find forces
c
      dimension sig(3),ac(3),sigxy(6)
      xc=ac(1)
      yc=ac(2)
      zc=ac(3)
      sigf=sig(1)
      sigt=sig(2)
      sigr=sig(3)
      rr=sqrt(xc**2+yc**2+zc**2)
      cf=zc/rr
      cf2=cf*cf
      sf=sqrt(1.-cf2)
      sf2=sf*sf
      rx=sqrt(xc**2+yc**2)
      rm=0.
      if(rx.ne.0.)rm=1./rx
      ct=xc*rm
      ct2=ct*ct
      st=yc*rm
      st2=st*st
      sigxy(1)=(ct2*(sigf*cf2+sigr*sf2)+sigt*st2)
      sigxy(4)=    ct*st*(sigf*cf2+sigr*sf2-sigt)
      sigxy(6)=    -ct*cf*sf*(sigf-sigr)
      sigxy(2)=    (st2*(sigf*cf2+sigr*sf2)+sigt*ct2)
      sigxy(5)=     -st*cf*sf*(sigf-sigr)
      sigxy(3)=     (sf2*sigf+cf2*sigr)
      return
      end
*deck index1
      subroutine index1(na,xa,x,n)
      dimension xa(1)
      if(x-xa(1))10,20,30
   10 n=0
      return
   20 n=1
      return
   30 if(x-xa(na))50,40,40
   40 n=na
      return
   50 if(n-1)140,130,60
   60 if(n-na+1)70,80,90
   70 if(x-xa(n))100,170,130
   80 if(x-xa(n))100,170,170
   90 n=na
  100 n=n-1
      if(x-xa(n))110,170,170
  110 nl=1
  120 nh=n
      go to 200
  130 n=n+1
      if(x-xa(n))160,170,150
  140 n=1
  150 n=n+1
      if(x-xa(n))160,170,180
  160 n=n-1
  170 return
  180 nh=na
  190 nl=n
  200 n=(nl+nh)/2
      if(n-nl)170,170,210
  210 if(x-xa(n))120,170,190
      end
*deck iter
      subroutine iter
      common/bfgs1/method,iatken,itemax,ntloc,ntst,maxup,numupd,ieqref
      common/bfgs2/dtol,stol,rtol,etol,rnorm
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/scom/nload,iloads,mvlast
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/tcom/munit,lunit,nstif,nred,nunit
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/ncom/numest,nvect,istoh,maxbh
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8,n9
      common/ratecom/dt,de(4),bb(4),dbb(4)
      common/adrs/n3s,n4s
      common /addb/ neql,neqr,mla,nblock
      common/dtwo/itwo,ndof,idim
      common/list/data(64),ndum
      dimension ia(1)
      common a(1)
      real a
      integer ia
      equivalence (a(1),ia(1))
      data end/3hend/
c
c        setup solution storage
c
      nstep=data(2)
      istres=data(3)
      if(istres.le.0)istres=nstep+1
      idisp=data(4)
      if(idisp.le.0)idisp=nstep+1
      iback=data(5)
      dt=data(6)
      itmax=data(7)
      rtol=data(8)
      if(neq.eq.nvect)go to 10
      write(6,1010)
      stop
   10 continue
      if(rtol.le.0.)rtol=.002
      itemax=itmax
      if(itemax.le.0)itemax=15
      method=2
      iatken=0
      if(method.ne.1)iatken=0
      write(6,1050)nstep,dt,method,iatken,neq
      n3a=n2+neq*itwo
      n3b=n2+3*neq*itwo
      n3=n2+nvect*itwo
      n4=n3+nvect*itwo
      if(nvect.eq.neq)n4=n3
      n5=n4+nvect*itwo
      n6=n5+nvect*itwo
      if(nvect.eq.neq)n6=n5
      if(n6.lt.n3a)n6=n3a
      n7=n6+istoh*itwo
      n8=n7+istoh*itwo
      if(nblock.eq.1)n8=n7
      if(n8.lt.n3b)n8=n3b
      n9=n8+neq*itwo
      n10=n9+neq*itwo
      n11=n10+neq*itwo
      n6e=n11
      n15=n6e+numest
      n41=n2+nvect*itwo
      n42=n2+numest
      n43=n2+ndof*numnp
      n15=max0(n15,n41,n42,n43)
      n5l=n4l+nblock
      min=nmax+n15-n1l+1
      write(6,2110)min
      if(min.gt.nmax)stop
c
c          get load vector
c
      write(6,1000)nstep
      if(nstep.le.0.or.nstep.gt.50)stop
      if(icheck.eq.1)return
      call load(a(n3a),a(n3a),a(n3a),a(n2),ndof,nvect,2)
      call start(a(n2),a(n4),nstep,iback)
      kstep=0
   20 continue
      kstep=kstep+1
      call colsol(a(n1l),a(n2l),a(n3l),a(n4l),a(n2),a(n3),a(n6),a(n7),
     .            a(n4),a(n5),nblock,neq,istoh,nvect,nvect,maxbh,
     .            nstif,nred,lunit,munit,nunit,1)
      kter=0
      call load(a(n3a),a(n3a),a(n3a),a(n2),ndof,nvect,1)
   30 continue
      call colsol(a(n1l),a(n2l),a(n3l),a(n4l),a(n2),a(n3),a(n6),a(n7),
     .            a(n4),a(n5),nblock,neq,istoh,nvect,nvect,maxbh,
     .            nstif,nred,lunit,munit,nunit,2)
      call equitb(a(n2),a(n4),a(n8),a(n9),a(n10))
  100 continue
      n3s=n3
      n4s=n4
      n3=n2+neq*itwo
      n4=n3+neq*itwo
      call save(a(n2),a(n3),a(n4),a(n6e),nvect,lunit)
      n3=n3s
      n4=n4s
      if(kstep.eq.nstep)go to 900
      ind=2
      n3s=n3
      n3=n6
      call assem1(a(n6),a(n6),a(n6),a(n6e),a(n1l),a(n3l),a(n5l),
     .   istoh,maxbh,munit,1)
      n3=n3s
      call getvec(a(n2),a(n3a),lunit,2)
      if(mod(kstep,idisp).eq.0)
     .   call writel(a(n2),a(n3a),a(n3a),a(n8),a(n9),ndof,nvect,lunit,0)
      if(mod(kstep,istres).eq.0)call stress(a(n2),a(n6e),nvect,lunit)
      go to 20
  900 continue
      call getvec(a(n2),a(n3a),lunit,2)
      call write50(a(n2),a(n3a),a(n3a),a(n8),a(n9),ndof,nvect,lunit,0)
      if(mod(kstep,idisp).eq.0)
     .   call writel(a(n2),a(n3a),a(n3a),a(n8),a(n9),ndof,nvect,lunit,0)
      if(mod(kstep,istres).eq.0)call stress(a(n2),a(n6e),nvect,lunit)
      return
 1000 format(1h0,10x,18hnumber of steps = ,i5)
 1010 format(19h neq .ne. nvect     )
 1050 format(' nstep = ',i5,' dt = ',1pe12.4,' method = ',i5,
     .       ' iatken =',i5,' neq = ',i5)
 2110 format(1h0,10x,39hstorage requested for solution phase =  ,i10)
 2290 format(1h0,10x,42hstorage to store entire solution vector =    ,
     .       i10)
      end
*deck start
      subroutine start(f,ur,nstep,iback)
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/ncom/numest,nvect,istoh,maxbh
      common/scom/nload,iloads,mvlast
      dimension f(1),ur(1)
      step=1./float(nstep)
      call getvec(ur,f,3,0)
      do 10 i=1,neq
      ur(i)=ur(i)*step
   10 continue
      call putvec(ur,f,3,0)
      call putvec(ur,f,3,1)
      if(iback.ne.0)return
      call putvec(ur,f,3,2)
      call putvec(ur,f,3,3)
      do 20 i=1,neq
      ur(i)=0.
   20 continue
      call putvec(ur,f,3,2)
      return
      end
*deck getvec
      subroutine getvec(ur,f,nunit,kfile)
      common/ncom/numest,nvect,istoh,maxbh
      common/scom/nload,iloads,mvlast
      dimension ur(1),f(1)
      kloc=0
      mv=nvect
      kblock=nload*kfile
      do 20 nl=1,nload
      if(nl.eq.nload)mv=mvlast
      call recs(nunit,f,nvect,nl+kblock)
      do 10 i=1,mv
      kloc=kloc+1
      ur(kloc)=f(i)
   10 continue
   20 continue
      return
      end
*deck putvec
      subroutine putvec(ur,f,nunit,kfile)
      common/ncom/numest,nvect,istoh,maxbh
      common/scom/nload,iloads,mvlast
      dimension ur(1),f(1)
      kloc=0
      mv=nvect
      kblock=nload*kfile
      do 20 nl=1,nload
      if(nl.eq.nload)mv=mvlast
      do 10 i=1,mv
      kloc=kloc+1
      f(i)=ur(kloc)
   10 continue
      call wecs(nunit,f,nvect,nl+kblock)
   20 continue
      return
      end
*deck save
      subroutine save(ur,f1,f2,ee,nvect,nunit)
c
      common /flgs/ kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/scom/nload,iloads,mvlast
      common/dtwo/itwo,ndof,idim
c
      dimension ur(1),f1(1),f2(1),ee(1)
c
      if(icheck.eq.1)return
      call getvec(ur,f2,nunit,2)
      call getvec(f1,f2,nunit,0)
      do 10 i=1,neq
      f2(i)=0.
   10 continue
 
      ind=6
      kpri=1
      do 60 i=1,ng
c
c        update stresses and strains
c
      call eread(ee,i)
      call gstiff
   60 continue
c
c        update dispacements
c
  100 continue
 
      call getvec(ur,f2,nunit,2)
      call getvec(f1,f2,nunit,0)
      do 130 j=1,neq
      ur(j)=ur(j)+f1(j)
  130 continue
      call putvec(ur,f2,nunit,2)
      return
      end
*deck equitb
      subroutine equitb (dispi,dincor,re,disp,wv)
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c .                                                                   .
c .   p r o g r a m                                                   .
c .      . to iterate for dynamic equilibrium                         .
c .                                                                   .
c .   method = 1  modified newton iteration                           .
c .      iatken = 0  no acceleration                                  .
c .      iatken = 1  aitken acceleration                              .
c .                                                                   .
c .   method = 2  bfgs matrix updating                                .
c .                                                                   .
c .        re   = out of balance loads                                .
c .        disp = displacement at previous time step                  .
c .        dispi= displacement increment at current time step         .
c .        dincor displacement increment correction                   .
c .        wv   = working vector                                      .
c .               effective stiffness matrix                          .
c .                                                                   .
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c
      common/bfgs1/method,iatken,itemax,ntloc,ntst,maxup,numupd,ieqref
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/bfgs2/dtol,stol,rtol,etol,rnorm
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
c
      common a(1)
      integer ia(1)
      dimension dispi(1),re(1),disp(1),wv(1)
     1         ,dincor(1)
      real a
      equivalence (a(1),ia(1))
      data rnorm/.00001/
c
      dtol=1.e-2
      stol=.5
      etol=10*rtol*dtol
      call getvec(disp,a(n6e),3,2)
      call getvec(re,a(n6e),3,1)
      call getvec(dispi,a(n6e),3,0)
c       write(6,7777)(dispi(i),i=1,neq)
c 7777  format(1p10e12.4)
      rm=prdinn(re,re,neq)
      if(rm.gt.rnorm)rnorm=rm
      ntst=4
      ntloc=ntst
      maxup=20
      numupd=0
c
      icount=3
      iconvg=0
      ite=0
      iacc=1
c
c     calculate initial potential energy for divergence check and
c     convergence criteria
c
      peinit=prdinn(re,dispi,neq)
      peold=peinit
c
c     if aitken acceleration is to be used (iatken # 0)
c     disp is used as a working array and is stored on tape natkn as :
c        1) displacement at previous time step
c        2) displacement increment for aitken acceleration
c
      if (iatken.eq.0) go to 499
      ntloc=ntst
      call putvec(disp,a(n6e),3,ntloc)
      ntloc=ntloc+1
      call putvec(dispi,a(n6e),3,ntloc)
      ntloc=ntloc+1
      dnorm=prdinn(dispi,dispi,neq)
      dnorm=sqrt(dnorm)
      dnmtol=0.01*dnorm
c
  499 if (method.lt.2) go to 500
c
   20 do 25 i=1,neq
      dincor(i)=dispi(i)
   25 dispi(i)=0.0
   30 continue
      ntloc=ntst
      call putvec(re,a(n6e),3,ntloc)
c
  500 ite=ite + 1
      step=1.0
      if (method.lt.2) step=0.0
      call unbld (step,dincor,dispi,re,disp,wv)
c
      if (ite.eq.1 .and. peinit.lt.1.0e-10*rnorm) peinit=1.0e-10*rnorm
c
      if (method.lt.2) go to 200
      pe=prdinn(dincor,re,neq)
  110 do 120 i=1,neq
  120 dispi(i)=dispi(i) + step*dincor(i)
c
  200 if (peold.gt.10000.0*peinit) go to 210
      if (ite  .lt. (itemax/2+1)) go to 230
c
c     check that potential energy of system is decreasing
c
      if (peold.le.peinit) go to 230
  210 ieqref=1
      icount=2
      write(6,1010)peold,peinit
      stop
 1010 format(' error no convergence peold peinit',1p2e12.4)
      return
c
  230 do 235 i=1,neq
  235 wv(i)=re(i)
c
c     calculate norm of incremental load
c
      renorm=prdinn(re,re,neq)
c
      if (method.lt.2) go to 250
      dinorm=prdinn(dincor,dincor,neq)
      dinorm=sqrt(dinorm)*step
      go to 250
c
c
  250 call newdir (pe,peold,step,dincor,dispi,re,dinorm,disp,wv)
c
c
c     calculate new potential energy
c
      if (method.eq.1) peold=prdinn(re,wv,neq)
      if (method.eq.2) peold=prdinn(dincor,re,neq)
c
c
c     c h e c k   f o r   c o n v e r g e n c e
c
c
        write(6,1000)ite,renorm,rtol*rnorm,peold,etol*peinit
 1000   format(' ite,renorm,rtol,peold,etol',i5,1p4e12.4)
 
      if (renorm.gt.rtol*rnorm) go to 256
  290 if (peold.gt.etol*peinit) go to 256
      iconvg=1
  256 if (iatken.eq.0 .or. iconvg.eq.1) go to 298
c
c     check which acceleration scheme should be used
c
      if (iatken.gt.1) go to 281
c
c     use aitken acceleration on each degree of freedom
c
      iacc=iacc + 1
      if (iacc.eq.2) go to 265
      call putvec(re,a(n6e),3,ntloc)
      ntloc=ntloc+1
      go to 298
c
c     apply acceleration factor
c
  265 continue
      call getvec(disp,a(n6e),3,ntloc)
      ntloc=ntloc+1
      iacc=0
      do 280 i=1,neq
      denom=disp(i) - re(i)
      if (abs(denom).lt.dnmtol) go to 275
      acfac=re(i)/denom
      go to 276
  275 acfac=0.0
  276 re(i)=re(i)*(1.0 + acfac)
  280 continue
      go to 298
c
c     overrelaxation
c
  281 continue
c
c     add increment to total displacement increment
c
  298 continue
  299 if (method.eq.2) go to 310
      do 300 i=1,neq
  300 dispi(i)=dispi(i) + re(i)
c
  310 if (iconvg.eq.1) go to 400
c
  370 if (ite.lt.itemax) go to 500
      write(6,2020)
      write(6,2010)ite
      stop
c
  400 icount=2
      if (method.eq.2)go to 405
      call putvec(dispi,a(n6e),3,0)
      return
  405 continue
      do 410 i=1,neq
  410 dispi(i)=dispi(i) + dincor(i)
      call putvec(dispi,a(n6e),3,0)
c       write(6,7777)(dispi(i),i=1,neq)
      return
c
 2010 format (37h number of iterations               = ,i5  /)
 2020 format (////45h iteration limit reached with no convergence  /5x,
     1            24h s t o p   of solution   )
      end
*deck unbld
      subroutine unbld (step,dincor,dispi,re,disp,wv)
c
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c .                                                                   .
c .                                                                   .
c .      p r o g r a m                                                .
c .                                                                   .
c .         to compute the unbalanced load in the configuration       .
c .         disp + dispi + step*dincor                                .
c .         (step = 0.0 for method = 1)                               .
c .                                                                   .
c .                                                                   .
c .                                                                   .
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c
c
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/list/data(64),ndum
      common/bfgs1/method,iatken,itemax,ntloc,ntst,maxup,numupd,ieqref
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/bfgs2/dtol,stol,rtol,etol,rnorm
      common a(1)
c
      integer ia(1)
      dimension dispi(1),disp(1),re(1),dincor(1),
     1          wv(1)
      real a
      equivalence (a(1),ia(1))
c
c
c     c a l c u l a t e   c u r r e n t    r h s   l o a d s
c
c
c
c     for bfgs method, temporarily add step*dincor to dispi
c
      if (method.eq.1) go to 15
      do 10 i=1,neq
   10 dispi(i)=dispi(i) + step*dincor(i)
   15 continue
   25 continue
      call getvec(re,a(n6e),3,1)
c
c     read displacements off tape natkn
c
      if (iatken.eq.0) go to 26
      ntloc=ntst
      call getvec(disp,a(n6e),3,ntloc)
      ntloc=ntloc+1
c
c     in divergence procedure, subtract off residual load (dincor)
c
   26 continue
  100 continue
  110 do 120 i=1,neq
  120 wv(i)=disp(i) + dispi(i)
c
c
c     a d d   n o n l i n e a r   c o n t r i b u t i o n s
c
c
      ind=7
      kpri=1
      do 200 i=1,ng
      call eread(a(n6e),i)
      call gstiff
  200 continue
c
c     for bfgs method, take out step*dincor from dispi
c
  240 if (method.eq.1) go to 500
      do 250 i=1,neq
  250 dispi(i)=dispi(i) - step*dincor(i)
c
  500 return
      end
*deck newdir
      subroutine newdir (y,yold,step,dincor,dispi,re,dinorm,disp,wv)
c
c
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/scom/nload,iloads,mvlast
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/tcom/munit,lunit,nstif,nred,nunit
      common/ncom/numest,nvect,istoh,maxbh
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/adrs/n3s,n4s
      common /addb/ neql,neqr,mla,nblock
      common/dtwo/itwo,ndof,idim
      common/bfgs1/method,iatken,itemax,ntloc,ntst,maxup,numupd,ieqref
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/bfgs2/dtol,stol,rtol,etol,rnorm
c
      common a(1)
      integer ia(1)
      dimension dispi(1),disp(1),re(1),dincor(1),
     1          wv(1)
      real a
      equivalence (a(1),ia(1))
c
      condmx=1.0e05
      if (method.ge.2) go to 5
      call putvec(wv,a(n6e),3,3)
      call putvec(re,a(n6e),3,0)
      call colsol(a(n1l),a(n2l),a(n3l),re,re,a(n3s),a(n6),a(n7),
     .            wv,wv,nblock,neq,istoh,nvect,nvect,maxbh,
     .            nstif,nred,lunit,munit,nunit,2)
      call getvec(re,a(n6e),3,0)
      call getvec(wv,a(n6e),3,3)
      return
    5 numupd = mod (numupd,maxup)
c
c     we already have
c           y = )dincor,resid# and yold = )dincor,oldresid#
c
c     we compute
c
c           delgam = )delta,gamma# = step*)dincor,oldresid-resid#
c     and
c           delh1d = )delta,h(-1)delta# = step**2 * )dincor,oldresid#
c
      delgam = step*(yold-y)
      delh1d = step*step*yold
c
c     check conditions for updating
c
      iup=0
      if (delgam.gt.0.0 .and. delh1d.gt.0.0) iup=1
      if (iup.eq.1) go to 13
      do 12 i=1,neq
      dincor(i)=re(i)
   12 continue
      if (numupd.eq.0)ntloc=ntst
      call putvec(re,a(n6e),3,ntloc)
      go to 15
c
c     read the old residual
c
   13 continue
      call getvec(wv,a(n6e),3,ntloc)
      if (numupd.eq.0)ntloc=ntst
      fact1=-sqrt(delgam/delh1d)*step - 1.0
      fact2 = step/delgam
c
c     wv = v = (sqrt(delgam/delh1d)*step-1)*oldresid + resid
c
c     resid = u = step/delgam * dincor
c
c
   21 do 20 i=1,neq
      aux=re(i)
      wv(i)=aux + fact1*wv(i)
      re(i)=fact2*dincor(i)
      dincor(i)=aux
   20 continue
c
c     check estimate on increase of condition number
c     of updated matrix   (estcon)
c
c     we have : x1 = )v,v#
c               x2 = )u,u#
c               x3 = 4*()u,v# + 1) = 4*()resid,wv# + 1)
c
   26 x1=prdinn(wv,wv,neq)
      x2 = dinorm*dinorm/(delgam*delgam)
      x3=4.*fact2*(fact1*yold + y) + 4.0
      if (x3.eq.0.0) iup=0
      if (iup.eq.0) go to 16
      x4=abs(x1*x2 + x3)
      estcon=((sqrt(x1)*sqrt(x2) + sqrt(x4))**2.0)/abs(x3)
      if (estcon.ge.condmx) iup=0
      if (iup.eq.0) go to 16
c
c     save updating vectors
c
      call putvec(wv,a(n6e),3,ntloc)
      ntloc=ntloc+1
      call putvec(re,a(n6e),3,ntloc)
      ntloc=ntloc+1
c
c     save new residual load (in dincor) for next iteration
c
   16 call putvec(dincor,a(n6e),3,ntloc)
      if (iup.eq.0) go to 15
      ntloc=max0(ntloc-2,ntst)
c
c     right half of updating
c
      factor = fact2 * y
      do 25 i=1,neq
      dincor(i)=dincor(i) + factor*wv(i)
   25 continue
   15 continue
      if (numupd.eq.0) go to 37
      do 30 j=1,numupd
      ntloc=ntloc-1
      call getvec(re,a(n6e),3,ntloc)
      factor=prdinn(dincor,re,neq)
      ntloc=ntloc-1
      call getvec(wv,a(n6e),3,ntloc)
      do 35 i=1,neq
      dincor(i)=dincor(i) + factor*wv(i)
   35 continue
   30 continue
c
c     backsubstitution
c
   37 continue
      call putvec(dincor,a(n6e),3,0)
      call colsol(a(n1l),a(n2l),a(n3l),a(n4l),re,re,a(n6),a(n7),
     .            wv,wv,nblock,neq,istoh,nvect,nvect,maxbh,
     .            nstif,nred,lunit,munit,nunit,2)
      call getvec(dincor,a(n6e),3,0)
c
c     left half of updating
c
      if (iup.eq.1) numupd=numupd + 1
      ntloc=ntst
      if (numupd.eq.0) go to 50
      do 40 j=1,numupd
      call getvec(wv,a(n6e),3,ntloc)
      ntloc=ntloc+1
      factor=prdinn(dincor,wv,neq)
      call getvec(re,a(n6e),3,ntloc)
      ntloc=ntloc+1
      do 45 i=1,neq
   45 dincor(i)=dincor(i) + factor*re(i)
   40 continue
c
c     read resid for computation of yold in equit
c
   50 continue
      call getvec(re,a(n6e),3,ntloc)
      return
      end
*deck prdinn
      function prdinn (aa,bb,n)
c
      common a(1)
      integer ia(1)
      dimension aa(n),bb(n)
      real a
      equivalence (a(1),ia(1))
c
      prdinn=0.0
   50 do 100 i=1,n
  100 prdinn=prdinn + aa(i)*bb(i)
      return
      end
*deck star4
      subroutine star4
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8,n9
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
      n103a=n102+8*nume*itwo
      n103b=n103a+16*nume*itwo
      n103c=n103b+16*nume*itwo
      n103d=n103c+4*nume*itwo
      n103e=n103d+4*nume*itwo
      n103f=n103e+4*nume*itwo
      n103=n103f+16*nume*itwo
      n104=nume+n103
      n105=8*nume+n104
      nlast=5*nummat+n105
      midest=nlast-nfirst+1
      n21=n2
      n31=n3
      n41=n3
      min=nmax+nlast-n1l+1
      if(ind.eq.0)write(6,2000)min
      if(min.le.nmax) go to 10
      stop 4
   10 if(ind.ne.7)go to 20
      n31=n2
      n41=n8
      n21=n9
   20 if(ind.ne.5)go to 30
      n41=n2
      n21=n8
      n31=n9
   30 continue
      call
     .star4p(a(n41),a(n2l),a(n1l),a(n21),a(n31),a(n101),a(n102),a(n103),
     .  a(n104),a(n105),nume,nummat,ndof,ind,kpri,a(n103a),a(n103b),
     .  a(n103c),a(n103d),a(n103e),a(n103f),ng)
      if(ind.eq.0)call ewrite(a(n6e),ng)
      if(ind.eq.6)call ewrit2(a(n6e),njunk)
 2000 format(1h0,10x,40hstorage required to input element data =,i10)
      return
      end
*deck star4p
      subroutine star4p(a,id,mht,x,y,fmat,xy,mat,lm,iprop,nume,nummat,
     .  ndof,ind,kpri,sig0,eps0,vmn0,rn0,en0,bn0,ng)
c
      common/comtyp/itypee,ngroup,ngin,nlin
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat,kpdvec
      common /list/ data(64),ndum
      common/scom/nload,iloads,mvlast
      common/simcom/x0,y0,z0,insim,incon
      common/grav/ax,ay,az
      common/var/povb,iplot,iprnt,matl
      common/stcom/stress(4),strain(4),ur(8),uri(8),ymod,poi,vmn,yld
     . r0,e0
      dimension x(1),y(1),fmat(25,1),mat(1),lm(8,1),stiff(36),xmass(36),
     . damp(36),st(4,8),fk(8,8),fc(8,8),fm(8,8),xy(2,4,1),nonum(4),
     . id(ndof,1),mht(1),iprop(5,1),c(10),ctil(10),d(4,4),dtil(4,4)
      dimension f0(8),sigr(8),sig0(16,1),eps0(16,1),vmn0(4,1)
      dimension rn0(4,1),en0(4,1),bn0(16,1)
c
      if (ind.ne.0) go to 200
c
c         read and store material data
c
      write(6,990)nume
      do 115 m=1,nummat
      call fld
      do 110 n=1,25
  110 fmat(n,m)=data(n+5)
      do 112 n=1,5
  112 iprop(n,m)=data(n)
      if(kpmat.eq.1)write(6,1000)m,(iprop(n,m),n=1,5)
      if(kpmat.eq.1)write(6,1010)(fmat(n,m),n=1,25)
  115 continue
      if(ng.eq.1)call mcelp
c
c         read and store element information
c
      if(kpelem.gt.0)write(6,1020)
      do 150 n=1,nume
      call fld
      num=data(1)
      m=data(2)
      mat(n)=m+100*num
      itype=iprop(1,m)
      nint=iprop(2,m)
      do 120 i=1,16
      sig0(i,n)=0.
      eps0(i,n)=0.
      bn0(i,n)=0.
  120 continue
      do 135 i=1,4
      vmn0(i,n)=0.
      rn0(i,n)=0.
      en0(i,n)=0.
      node=data(i+2)
      nonum(i)=node
      lm(2*i-1,n)=id(1,node)
      lm(2*i,n)=0
      if(ndof.gt.1)lm(2*i,n)=id(2,node)
      xy(1,i,n)=x(node)
  135 xy(2,i,n)=y(node)
c
      call colht(mht,8,lm(1,n))
c
      if(insim.eq.1)call istress(n,m,itype,nint,xy(1,1,n),sig0(1,n),
     .                           vmn0(1,n),rn0(1,n),en0(1,n),bn0(1,n))
      if(insim.eq.2)call sstress(n,m,itype,nint,xy(1,1,n),sig0(1,n),
     .                           vmn0(1,n),rn0(1,n),en0(1,n),bn0(1,n))
      if(insim.eq.4)call sstress4(n,m,itype,nint,xy(1,1,n),sig0(1,n),
     .                           vmn0(1,n),rn0(1,n),en0(1,n),bn0(1,n))
      if(kpelem.eq.0)go to 150
      write(6,1030) n,mat(n),(nonum(i),i=1,4)
c
  150 continue
c
      return
c
c         ind=1 pass to form element matrices
c
  200 if(ind.eq.4)go to 300
c       write(6,7712)ind,kpri
7712    format(' ind kpri',2i5)
      matl=-13
      if (ind.eq.3) write(6,1040)
      do 280 n=1,nume
      icode=0
c       write(6,1456)n,(lm(i,n),i=1,8)
c1456   format(10i5)
      if (ind.eq.1.or.ind.eq.2) call echeck(lm(1,n),8,icode)
      if (icode.eq.1) go to 280
      m=mat(n)
      istres=m/100
      m=m-100*istres
      itype=iprop(1,m)
      nint=iprop(2,m)
      ilumps=iprop(3,m)
      idamps=iprop(4,m)
      if (kpri.eq.3.and.idamps.eq.0) go to 280
      do 5 i=1,8
c
c       expand solution displ vector (fill in constraint zeros)
c
      idof=lm(i,n)
      ur(i)=0.0
      uri(i)=0.0
      if(idof.eq.0) go to 5
      ur(i)=x(idof)
      uri(i)=y(idof)
    5 continue
      if(m.eq.matl)go to 219
      matl=m
      thic=fmat(1,m)
      e=fmat(2,m)
      pr=fmat(3,m)
      fnu1=fmat(4,m)
      rho=fmat(5,m)
      if (thic.eq.0.0) thic=1.0
      nstr = 4
      if (itype.eq.2)nstr=3
  219 kprip=kpri
      if(ind.eq.5)go to 350
      if(ind.eq.7)go to 355
      if(ind.eq.3) go to 348
      if(ind.eq.6)kprip=4
      call stars1(n,itype,nstr,nint,thic,e,pr,xy(1,1,n),d,dtil,fnu1,
     .     rho,ilumps,kprip,st,fk,fc,fm,f0,sigr,sig0(1,n),eps0(1,n),
     .     vmn0(1,n),rn0(1,n),en0(1,n),bn0(1,n))
c       if(n.eq.1)write(6,7777)n,ymod,poi,yld
7777    format('  n = ',i5,'  ymod,poi,yld',1p3e12.4)
      if(ind.eq.6) go to 255
c
      m=0
      do 220 i=1,8
      do 220 j=i,8
      m=m+1
      stiff(m)=fk(i,j)
      damp(m)=fc(i,j)
  220 xmass(m)=fm(i,j)
c      print of element stiff damp and mass matrices and pointer array
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
  255 continue
 
      if(ind.eq.6)go to 280
c
c      print stresses
c
  348 continue
c       if(n.eq.1)write(6,7777)n,ymod,poi,yld
      kprip=7
      call stars1(n,itype,nstr,nint,thic,e,pr,xy(1,1,n),d,dtil,fnu1,
     .     rho,ilumps,kprip,st,fk,fc,fm,f0,sigr,sig0(1,n),eps0(1,n),
     .     vmn0(1,n),rn0(1,n),en0(1,n),bn0(1,n))
      kint=1
      iiend=nint*nint
      do 349 ii=1,iiend
      k=(ii-1)*4
      call maxstr(sig0(1+k,n),p1,p2,ag)
      if(kpstre.eq.1.and.istres.ne.0)
     .write(6,1050)n,sigr(kint),sigr(kint+1),(sig0(i,n),i=1+k,4+k),p1,p2
      if(kpstre.eq.1.and.istres.ne.0)
     .write(6,2050)(eps0(i,n),i=1+k,4+k)
      write(49)n,sigr(kint),sigr(kint+1),(sig0(i,n),i=1+k,4+k),p1,p2,ag,
     .         vmn0(ii,n),en0(ii,n)
      kint=kint+2
  349 continue
      call eplot(xy(1,1,n),ur)
c
      go to 280
  350 if((iloads.and.2).ne.2)go to 360
      kprip=8
      call stars1(n,itype,nstr,nint,thic,e,pr,xy(1,1,n),d,dtil,fnu1,
     .     rho,ilumps,kprip,st,fk,fc,fm,f0,sig0,sig0(1,n),eps0(1,n),
     .     vmn0(1,n),rn0(1,n),en0(1,n),bn0(1,n))
      call addma(a,f0,lm(1,n),8)
      go to 360
  355 continue
      kprip=5
      call stars1(n,itype,nstr,nint,thic,e,pr,xy(1,1,n),d,dtil,fnu1,
     .     rho,ilumps,kprip,st,fk,fc,fm,f0,sig0,sig0(1,n),eps0(1,n),
     .     vmn0(1,n),rn0(1,n),en0(1,n),bn0(1,n))
      call addma(a,f0,lm(1,n),8)
      go to 280
  360 if((iloads.and.1).ne.1)go to 280
      do 370 j=1,4
      sigr(2*j-1)=ax
      sigr(2*j)=ay
  370 continue
      kprip=6
      call stars1(n,itype,nstr,nint,thic,e,pr,xy(1,1,n),d,dtil,fnu1,
     .     rho,ilumps,kprip,st,fk,fc,fm,f0,sigr,sig0(1,n),eps0(1,n),
     .     vmn0(1,n),rn0(1,n),en0(1,n),bn0(1,n))
      call addma(a,f0,lm(1,n),8)
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
  990 format(1h0,10x,26hnumber of quad elements = ,i5)
 1000 format(1h0,10x,31hquad4 properties for mat number,i3,//,
     .2x,5hitype,3x,4hnint,1x,6hilumps,1x,6hidamps,1x,6histres,/,5i7,/)
 1010 format(1x,1p5e12.4)
 1020 format(///10x,12helement data,/,2x,3helm,2x,3hmat,2x,5hnode1,
     . 2x,5hnode2,2x,5hnode3,2x,5hnode4//)
 1030 format(2i5,4i7)
 1040 format(1h0,10x,33h        stress for quad4 elements,//,1x,5h  elm,
     .  5x,1hx,10x,1hy,4x,
     . (3x,8hsigma-xx),(3x,8hsigma-yy),(4x,6htau-xy,1x),
     .(3x,8hsigma-zz),(3x,8hsigma-p+),(3x,8hsigma-p-),(3x,5hangle,3x))
 1050 format(i5,1p9e11.3)
 2050 format(5x,22x,1p4e11.3)
      end
*deck stars1
      subroutine stars1(nel,itype,nstr,nint,thic,e,pr,xx,d,dtil,fnu1,
     .rho,ilumps,kpri,st,fk,fc,fm,f0,sig0,sig1,eps1,vmn1,rn0,en0,bn0)
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
c.              eq.0 = axisymetric
c.                     eq.1 = plane strain
c.                     eq.2 = plane stress
c.                   eq.3 = input matrix
c.c(6)  = input material property matrix
c.      ctil(6) = input damping matrix
c.      fnu1      = damping constant
c.      ilumps    = switch for mass matrix  0-consistant mass matrix
c.       1-lumped mass matrix
c.      nint      = gauss numerical integration order
c.      thic      = thickness of element
c.      e        = young@s modulus
c.      pr        = poisson@s ratio
c.      xx(2,4)   = element node coordinates
c.      fk(8,8)    = stiffness matrix on solution exit
c.
c.- - output - -
c.      fk(8,8)    = stiffness matrix
c.      fm(8,8)   = mass matrix
c.      fc(8,8)    = damping matrix
c.st(3,8)= stress transformation matrix
c......................................................................
      common/stcom/stress(4),strain(4),ur(8),uri(8),ymod,poi,vmn,yld
     . ,r0,e0
      common/ratecom/dt,de(4),bb(4),dbb(4)
      dimension d(4,4),b(4,8),xx(2,4),fk(8,8),xg(4,4),wgt(4,4),db(4)
      dimension hh(2,8),fm(8,8),sigr(4)
      dimension dtil(4,4),st(4,8),fc(8,8),dtilb(4)
      dimension f0(8),sig0(8),sig1(4,4),eps1(4,4),vmn1(4)
      dimension rn0(4),en0(4),bn0(4,4)
c
c................................................................
c.      this program is used in single precision arithmetic on cdc
c.      equipment and double pecision arithmetic on ibm or univac
c.      machines. for single or double precision arithmetic activ-
c.      ate, deactivate or adjust above and data xg/wgt cards.
c................................................................
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
      data ndof/8/
      go to (230,240,230,260,500,600,700,800),kpri
  230 continue
c
c.    c a l c u l a t e  e l e m e n t  s t i f f n e s s ,
c.    e l e m e n t  m a s s ,  a n d  e l e m e n t
c.    d a m p i n g
c.
  20  do 30 i=1,ndof
      do 30 j=1,ndof
      fc(i,j)=0.0
      fk(i,j)=0.0
   30 continue
      kint=0
      do 80 lx=1,nint
      ri=xg(lx,nint)
      do 80 ly=1,nint
      si=xg(ly,nint)
      kint=kint+1
c
c     evaluate deivative operator b and the jacobian determinant det
c     evaluate the displacement interpolation matrix h
c
      call stom(xx,b,det,hh,ri,si,xbar,nel,itype)
      call setstr(eps1(1,kint),sig1(1,kint),bn0(1,kint),b,ndof)
      vmn=vmn1(kint)
      r0=rn0(kint)
      e0=en0(kint)
      call youngp(d,itype,nel)
c
c     add contribution to element stiffness and mass
c
      if (itype.gt.0)xbar=thic
      wt=wgt(lx,nint)*wgt(ly,nint)*xbar*det
      do 70 j=1,ndof
      do 40 k=1,nstr
      dtilb(k)=0.0
      db(k)=0.0
      do 40 l=1,nstr
      dtilb(k)=dtilb(k) + dtil(k,l)*b(l,j)
  40  db(k)=db(k)+d(k,l)*b(l,j)
      do 60 i=j,ndof
      damp=0.0
      stiff=0.0
      do 50 l=1,nstr
      damp=damp + b(l,i)*dtilb(l)
  50  stiff=stiff+b(l,i)*db(l)
      fc(i,j)=fc(i,j) + wt*(damp + fnu1*stiff)
  60  fk(i,j)=fk(i,j)+stiff*wt
  70  continue
  80  continue
c
      do 90 j=1,ndof
      do 90 i=j,ndof
      fc(j,i)=fc(i,j)
  90  fk(j,i)=fk(i,j)
      return
  240 continue
      do 31 i=1,ndof
      do 31 j=1,ndof
   31 fm(i,j)=0.0
      if(ilumps.eq.0) go to 81
c.
c. c a l c u l a t i o n  o f  t h e   l u m p e d  m a s s  m a t r i x
c.
      area=((xx(1,2)-xx(1,3))*(xx(2,2)-xx(2,1)) + (xx(1,2)-xx(1,1))*
     1(xx(2,3)-xx(2,2)) + (xx(1,4)-xx(1,1))*(xx(2,4)-xx(2,3)) +
     2(xx(1,4)-xx(1,3))*(xx(2,1)-xx(2,4)))/2.0
      w=rho*thic*area
      do 315 i=1,8
  315 fm(i,i)=w/4.0
      return
   81 continue
      do 27 j=1,ndof
      do 27 i=j,ndof
      fmass=0.0
      do 17 l=1,2
   17 fmass=fmass + hh(l,i)*hh(l,j)
   27 fm(i,j)=fm(i,j) + fmass*rho*wt
      do 91 j=1,ndof
      do 91 i=j,ndof
   91 fm(j,i)=fm(i,j)
      return
  260 continue
c.
c. c a l c u l a t i o n  o f  t h e  s t r e s s  t r a n s f o r m -
c. a t i o n  m a t r i x
c.
      kint=0
      do 480 lx=1,nint
      ri=xg(lx,nint)
      do 480 ly=1,nint
      si=xg(ly,nint)
      kint=kint+1
      do 411 i=1,nstr
      do 411 j=1,ndof
  411 st(i,j)=0.0
      call stom(xx,b,det,hh,ri,si,xbar,nel,itype)
      call setstr(eps1(1,kint),sig1(1,kint),bn0(1,kint),b,ndof)
      vmn=vmn1(kint)
      r0=rn0(kint)
      e0=en0(kint)
      do 412 i=1,4
      strain(i)=strain(i)+de(i)
  412 continue
      call youngp(d,itype,nel)
      do 413 i=1,nstr
      do 413 j=1,ndof
      do 413 l=1,nstr
  413 st(i,j)=st(i,j) + d(i,l)*b(l,j)
      f1=0.
      f2=0.
      do 414 l=1,4
      f1=f1+hh(2,2*l)*xx(1,l)
      f2=f2+hh(2,2*l)*xx(2,l)
  414 continue
c
c compute stresses
c     multiply stress transformation matrix times displacement vector
c
      sigr(4)=0.
      do 340 i=1,nstr
      sigr(i)=0.0
      do 340 j=1,8
      sigr(i)=sigr(i)+st(i,j)*uri(j)
  340 continue
c
c        update stress ans strains
c
      call cstr(b,ndof)
      call incstr(sigr)
      do 345 i=1,4
      sig1(i,kint)=stress(i)+sigr(i)
      eps1(i,kint)=strain(i)+eps1(i,kint)
      bn0(i,kint)=bb(i)
  345 continue
      vmn1(kint)=vmn
  480 continue
c       if(nel.eq.1)write(6,888)nel,(bb(i),i=1,4)
  888   format(' nel ',i5,1p4e12.4)
      return
c
c          element forces
c
  500 continue
      do 530 j=1,ndof
      f0(j)=0.
  530 continue
      kint=0
      do 580 lx=1,nint
      ri=xg(lx,nint)
      do 580 ly=1,nint
      si=xg(ly,nint)
      kint=kint+1
      do 511 i=1,nstr
      do 511 j=1,ndof
  511 st(i,j)=0.0
      call stom(xx,b,det,hh,ri,si,xbar,nel,itype)
      call setstr(eps1(1,kint),sig1(1,kint),bn0(1,kint),b,ndof)
      do 512 i=1,4
      strain(i)=strain(i)+de(i)
  512 continue
      vmn=vmn1(kint)
      r0=rn0(kint)
      e0=en0(kint)
      call youngp(d,itype,nel)
      do 513 i=1,nstr
      do 513 j=1,ndof
      do 513 l=1,nstr
  513 st(i,j)=st(i,j) + d(i,l)*b(l,j)
c
c compute stresses
c     multiply stress transformation matrix times displacement vector
c
      sigr(4)=0.
      do 514 i=1,nstr
      sigr(i)=0.0
      do 514 j=1,8
      sigr(i)=sigr(i)+st(i,j)*uri(j)
  514 continue
      call incstr(sigr)
c
c    add initial stress vector
c
      if (itype.gt.0)xbar=thic
      wt=wgt(lx,nint)*wgt(ly,nint)*xbar*det
      do 570 j=1,ndof
      for=0.
      do 560 l=1,nstr
      for=for+sigr(l)*b(l,j)
  560 continue
      f0(j)=f0(j)-for*wt
  570 continue
  580 continue
      return
  600 continue
      do 630 j=1,ndof
      f0(j)=0.
  630 continue
      kint=0
      do 680 lx=1,nint
      ri=xg(lx,nint)
      do 680 ly=1,nint
      si=xg(ly,nint)
      kint=kint+1
      call stom(xx,b,det,hh,ri,si,xbar,nel,itype)
      call setstr(eps1(1,kint),sig1(1,kint),bn0(1,kint),b,ndof)
      vmn=vmn1(kint)
      r0=rn0(kint)
      e0=en0(kint)
      call youngp(d,itype,nel)
      if (itype.gt.0)xbar=thic
      wt=wgt(lx,nint)*wgt(ly,nint)*xbar*det*r0
      do 660 l=1,ndof
      f0(l)=f0(l)+(hh(1,l)+hh(2,l))*wt*sig0(l)
  660 continue
  680 continue
      return
  700 continue
      kint=0
      do 780 lx=1,nint
      ri=xg(lx,nint)
      do 780 ly=1,nint
      si=xg(ly,nint)
      call stom(xx,b,det,hh,ri,si,xbar,nel,itype)
      f1=0.
      f2=0.
      do 710 l=1,4
      f1=f1+hh(2,2*l)*xx(1,l)
      f2=f2+hh(2,2*l)*xx(2,l)
  710 continue
      kint=kint+1
      sig0(kint)=f1
      kint=kint+1
      sig0(kint)=f2
  780 continue
      return
  800 continue
      do 810 i=1,8
      ur(i)=0.
  810 continue
      do 830 j=1,ndof
      f0(j)=0.
  830 continue
      kint=0
      do 880 lx=1,nint
      ri=xg(lx,nint)
      do 880 ly=1,nint
      si=xg(ly,nint)
      kint=kint+1
      call stom(xx,b,det,hh,ri,si,xbar,nel,itype)
      call setstr(eps1(1,kint),sig1(1,kint),bn0(1,kint),b,ndof)
c
c    add initial stress vector
c
      if (itype.gt.0)xbar=thic
      wt=wgt(lx,nint)*wgt(ly,nint)*xbar*det
      do 870 j=1,ndof
      for=0.
      do 860 l=1,nstr
      for=for+stress(l)*b(l,j)
  860 continue
      f0(j)=f0(j)-for*wt
  870 continue
  880 continue
      end
*deck stom
      subroutine stom(xx,b,det,hh,r,s,xbar,nel,itype)
      common/stcom/stress(4),strain(4),ur(2,4),uri(8),ymod,poi,vmn,yld
     . ,r0,e0
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
c     displacement interpolation matrix for calculation of mass matrix
c
      do 45 i=1,2
      do 45 j=1,8
   45 hh(i,j)=0.0
      do 35 j=1,4
      hh(1,2*j-1)=h(j)
   35 hh(2,2*j)=h(j)
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
  20  dum=dum+p(i,k)*(xx(j,k)+ur(j,k))
  30  xj(i,j)=dum
c
c     compute the determinant of the jacobian matrix at point (r,s)
c
      det=xj(1,1)*xj(2,2)-xj(2,1)*xj(1,2)
      if (det.gt.0.00000001)go to 40
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
      b(1,k2-1)=0.
      b(1,k2)=0.
      b(2,k2-1)=0.
      b(2,k2)=0.
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
      xbar=0.0
      do 70 k=1,4
  70  xbar=xbar+h(k)*(xx(1,k)+ur(1,k))
c
c     evaluate the hoop strain-displacement relation
c
      if(xbar.gt.0.00000001)go to 90
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
      b(4,k2)=0.
 100  b(4,k2-1)=h(k)*dum
      return
c
 2000 format (1h1,6herror>,/,1x,
     143hzero or negative jacobian det. for element ,i6)
c
c
      end
*deck youngp
      subroutine youngp(d,itype,nel)
      common/stcom/stress(4),strain(4),ur(8),uri(8),e,pr,vmn,yld
     . ,r0,e0
      dimension c(10),d(4,4)
      call getyp
      if(e.eq.0.)call gerror(nel)
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
  212 continue
C
C   modify for plane strain
C
      if (itype.ne.1) goto 214
      do 213 i=1,4
         d(i,4) = 0.0
 213  continue
 214  continue
c
c         modify for plane stress
c
      if (itype.ne.2) go to 217
      if (d(4,4).eq.0.0) go to 217
      do 215 i=1,3
      con=d(i,4)/d(4,4)
      do 215 j=1,3
  215 d(i,j)=d(i,j)-d(4,j)*con
c
  217 continue
      return
      end
*deck gerror
      subroutine gerror(nel)
      write(6,1000)nel
      stop
 1000 format(' Error element = ',i5,' has failed')
      end
*deck cstr
      subroutine cstr(b,ndof)
      common/stcom/stress(4),strain(4),ur(8),uri(8),ymod,poi,vmn,yld
     . ,r0,e0
      dimension b(4,8)
      do 420 i=1,4
      strain(i)=0.
      do 420 j=1,ndof
      strain(i)=strain(i)+b(i,j)*uri(j)
  420 continue
      return
      end
*deck incstr
      subroutine incstr(sigr)
      common/stcom/stress(4),strain(4),ur(8),uri(8),ymod,poi,vmn,yld
     . ,r0,e0
      common/ratecom/dt,de(4),bb(4),dbb(4)
      common/var/povb,iplot,iprnt,mat
      common/eqss2/estcn2(30,28)
      dimension sigr(4),snew(4)
      dg=estcn2(22,mat)
      do 10 i=1,4
      snew(i)=stress(i)+sigr(i)+dg*dbb(i)
      bb(i)=bb(i)+dbb(i)
   10 continue
      sm=(snew(1)+snew(2)+snew(4))/3.
      s1d=snew(1)-sm
      s2d=snew(2)-sm
      s3d=snew(3)
      s4d=snew(4)-sm
      sj2p=sqrt(3.*(s1d**2+s2d**2+s3d**2+s1d*s2d))
      if(sj2p.lt.yld)go to 30
      fact=yld/sj2p
      snew(1)=sm+fact*s1d
      snew(2)=sm+fact*s2d
      snew(3)=fact*s3d
      snew(4)=sm+fact*s4d
      do 20 i=1,4
      bb(i)=fact*bb(i)
   20 continue
   30 continue
      do 40 i=1,4
      sigr(i)=snew(i)-stress(i)
   40 continue
      return
      end
*deck setstr
      subroutine setstr(eps1,str1,bn1,b,ndof)
      common/stcom/stress(4),strain(4),ur(8),uri(8),ymod,poi,vmn,yld
     . ,r0,e0
      common/ratecom/dt,de(4),bb(4),dbb(4)
      dimension eps1(4),str1(4),bn1(4)
      dimension b(4,8)
      do 10 i=1,4
      de(i)=0.
      do 10 j=1,ndof
      de(i)=de(i)+b(i,j)*uri(j)
   10 continue
      do 20 i=1,4
      strain(i)=eps1(i)
      stress(i)=str1(i)
      bb(i)=bn1(i)
   20 continue
      return
      end
*deck mcelp
      subroutine mcelp
      common/stcom/stress(4),strain(4),ur(8),uri(8),ymod,poi,vmn,yld
     . ,r0,e0
      common/var/povb,iplot,iprnt,mat
      common/eqss/estcon(30,28)
      common/eqss2/estcn2(30,28)
      common/alpcom/coef(124,28)
      common/eoscm2/fake(20)
      common/eoscom/nn,vol,e,p,sndsqr,alpha,dalpha,
     *              alph,vmin,rmu,yield,dpdv,dpde
      common/ratecom/dt,de(4),bb(4),dbb(4)
      nn=0
      e=0.0
      sndsqr=0.0
      alpha=0.0
      dalpha=0.0
      alph=0.0
      vmin=0.0
      rmu=0.0
      yield=0.0
      dpdv=0.0
      dpde=0.0
      vol=0.0
      mat=1
      call setmat
      return
      entry getyp
      e=e0
      vmin=vmn
      if(vmin.eq.0.)vmin=1.e10/estcon(21,mat)
      if(r0.eq.0.)r0=1./estcon(1,mat)
      v0=1./r0
      nn=mat
      vol=v0*exp(strain(1)+strain(2)+strain(4))
      dev=(de(1)+de(2)+de(4))/3.
      de(1)=de(1)-dev
      de(2)=de(2)-dev
      de(4)=de(4)-dev
      call eos
      ak=-vol*dpdv
      if(rmu.gt.0.)go to 10
      a1=estcon(7,mat)
      a2=1.e7
      a3=a1-a2
      eta=(vol/v0-1.)
      rmu=a2+a3*exp(-1.e3*eta)
   10 continue
      if(sndsqr.le.1.1)yield=1e15
      denom=3.0*ak+rmu
      ymod=9.0*ak*rmu/denom
      poi=(3.0*ak-2.0*rmu)/(2.0*denom)
      vmn=vmin
      yld=yield
      return
      end
*deck setmat
      subroutine setmat
      common/arrays/xa(150),xb(150),xc(150),xd(150),
     *   xe(150),xf(150),xg(150),xh(150),xi(150),xj(150),xk(150)
      common/var/povb,iplot,iprnt,mat
      common/eqss/estcon(30,28)
      common/eqss2/estcon2(30,28)
      common/alpcom/coef(124,28)
      dimension a(60)
      namelist/eq/ieqst,a
      namelist/name0/povb
      namelist/pa/ialpha,a
      data nmt/28/
      read(5,name0)
      do 125 mat=1,nmt
      do 100 i=1,30
      estcon2(i,mat)=0.0
  100 estcon(i,mat)=0.0
      do 120 i=1,124
  120 coef(i,mat)=0.0
  125 continue
  126 continue
      do 110 i=1,60
 110  a(i)=0.0
      read(5,eq)
      if(ieqst.eq.0)return
      mat=ieqst
 130  do 140 i=1,60
      if(i.le.30)estcon(i,mat)=a(i)
      if(i.gt.30)estcon2(i-30,mat)=a(i)
      a(i)=0.0
  140 continue
      read(5,pa)
      if(ialpha.eq.0)go to 160
      do 150 i=1,52
      coef(i,mat)=a(i)
  150 continue
  160 dummy=alphaf(0.d0,mat,povb)
      go to 126
      return
      end
*deck eplot
      subroutine eplot(xy,dxy)
      dimension xy(2,4),dxy(2,4)
      dimension x(500),y(500),dx(500),dy(500)
      common/pntcom/kpnode,kpdof,dpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat,kpdvec
      data n/0/
      if(n.lt.500)go to 100
      iplt = 3
      if (kpdvec.eq.1) iplt = 2
      call plot(x,y,dx,dy,n,5,1,iplt,.false.)
      n=0
  100 continue
      do 110 i=1,4
      n=n+1
      x(n)=xy(1,i)
      y(n)=xy(2,i)
      dx(n)=dxy(1,i)
      dy(n)=dxy(2,i)
  110 continue
      n=n+1
      x(n)=xy(1,1)
      y(n)=xy(2,1)
      dx(n)=dxy(1,1)
      dy(n)=dxy(2,1)
      return
      entry edone
      if(n.ne.0)call plot(x,y,dx,dy,n,5,1,iplt,.false.)
      call done
      n=0
      return
      end
*deck plot
      subroutine plot(xb,yb,xv,yv,np1,np2,nf,itype,iclose)
      common/uset/nl(5),l(10,5)
      dimension xb(1),yb(1),xv(1),yv(1)
      logical flag,iclose
      data kk,b,keof,nn,nout/1,0.,6heofeof,0,43/
      data flag/.false. /
      if(flag)go to 5
      write(nout)kk,(nl(i),i=1,5),b,b,b,b
      do 4 i=1,5
      if(nl(i).gt.0)write(nout)(l(j,i),j=1,10)
    4 continue
      flag=.true.
    5 continue
      go to (10,20,20,40,40,60,70,60,70,100,60),itype
   10 continue
      nw=np1
      go to 800
   20 continue
      nw=np1
      nw2=2*np1
      go to 810
   40 continue
      nw=3*np1
      go to 800
   60 continue
      nw=(np2-1)/10+1
      nw2=2*nw
      go to 810
   70 continue
      nw=np1
      nw2=2*np1
      go to 810
  100 continue
      nw=np1
      go to 800
  800 continue
      write(nout)itype,nw,np2,nf,(xb(i),i=1,nw),(yb(i),i=1,nw)
      if(iclose)go to 900
      return
  810 continue
      write(nout)itype,nw2,np2,nf,(xb(i),i=1,nw),(yb(i),i=1,nw),
     .                            (xv(i),i=1,nw),(yv(i),i=1,nw)
      if(iclose)go to 900
      return
  900 continue
       entry done
      write(nout)(nn,i=1,10)
      write(nout)(keof,i=1,10)
      backspace nout
      flag=.false.
      return
      end
*deck sstress4
      subroutine sstress4(nel,mat,itype,nint,xx,sig0,vmin0,rn0,
     .                  en0,bn0)
      common/eqss/estcon(30,28)
      common/eqss2/estcn2(30,28)
      common/alpcom/coef(124,28)
      common/var/povb,iplot,iprnt,matl
      common/eoscom/nn,vol,e,p,sndsqr,alpha,dalpha,
     *              alph,vmin,rmu,yield,dpdv,dpde
 
      dimension sig0(4,4),vmin0(4),rn0(4),en0(4),bn0(4,4)
      dimension sigxyz(4)
      matl=mat
      nn=mat
      vf=estcon(1,mat)
      vnew=vf
      enew=estcon(26,nn)
      last=nint*nint
      do 180 kint=1,last
      rn0(kint)=1./vnew
      en0(kint)=enew
      vmin0(kint)=estcon(1,mat)
      sig0(1,kint)=-povb
      sig0(2,kint)=-povb
      sig0(3,kint)=0.00
      sig0(4,kint)=-povb
      bn0(1,kint)=0.00
      bn0(2,kint)=0.00
      bn0(3,kint)=0.00
      bn0(4,kint)=0.00
  180 continue
      return
 1000 format('vnew = 0. nel = ',i5,' kint = ',i5)
      end
