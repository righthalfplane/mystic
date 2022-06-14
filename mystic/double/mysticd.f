*deck mystic
      program mystic
      implicit real*8 (a-h,o-z)
      character*8 file1
      common/t51com/file1
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat,kpplot
      common/switch/ipsm,ipkbar,ipinpt,ipreac,ieldat,ipsdm,ipcnpf
     . ,ibigw
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/dyncm1/time,dt,dtold,delta,alpha
      common/dyncm2/idym,imass,idamp,ires,intial,ncycle,nstep,nbdym
      common/dyncm3/a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10
      common/stapes/nts1,ntb1,nts2,ntb2,nts3,ntb3
      common/scom/nload,iloads,mvlast
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/tcom/munit,lunit,nstif,nred,nunit
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/ncom/numest,nvect,istoh,maxbh
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common /addb/ neql,neqr,mla,nblock
      common/prcom/ibk,ipnt,idis,ivel,iacc,ipnode(3,20)
      common/dtwo/itwo,ndof,idim
      common/tapcom/nt11,nt31
      common/list/data(64),ndum
      common/comtyp/itype,ngroup,ngin,nlin
      common/fsicom/fsifac,wfsi,kfsi
      common/flgs1/ibig,idmp
      common/elcom/nwelem
      dimension etype(23),ptype(12)
      dimension ia(1),kploc(12)
      character*8 head
      common/hdcom/ head(8)
      character*8 cdata
      common/clist/cdata(64)
      dimension com(20)
      common a(41025000)
      real a
      integer ia
      equivalence (a(1),ia(1)),(kploc(1),kpnode)
      character*8 com,etype,ptype,end,strs,off,gno,word
      data com/'stop','node','form','load','solv','back','chec',
     .  'pon','poff','big','damp','elen','swit','fsi','mar','init',
     .  'pnt','rest','mode',' '/
      data etype/'reac','tape','con','user','quad','tri','beam',
     .  'axsh','pqdm','hex8','phex','ptri','bm3d','atri','aqua',
     .  'ashl','astf','pzel','bemd','feac','tloa',' ',' '/
      data ptype/'node','dof','elem','load','comp','equ','equl',
     .  'equr','stre','dis','mat','plot'/
      data end,strs,off,gno/'end','stre','off','no'/
      data ibig,idmp/0,1/
      data kploc/4*1,0,1,1,0,1,1,1,1/
      call second2(tstart)
      n1=1
      ip=1
      ibk=0
      iep=1
      itwo=1
      nt11=11
      nt31=31
      kfsi=1
      fsifac=0.
      iequit=1
      ibigw=1
      ieldat=1
      icheck=0
      nmax=40000000
      munit=2
      lunit=3
      nstif=4
      nred=5
      nunit=6
      ngroup=128
      time=0.
      dtold=0.
      delta=.5d0
      alpha=.25d0*(delta+.5d0)**2
      ncycle=0
      idym=0
      intial=0
      ires=0
      imass=0
      idamp=0
      nbdym=0
      call mopen(istoh,nblock,icheck)
      read(5,1000)head
      write(6,1010)ip,head
      call ffin
      rewind 11
   10 continue
c
c          read next command card
c
      call fld
c
c        command loop
c
      word=cdata(1)
      do 30 i=1,20
      ic=i
      if(word.eq.com(i))go to 50
   30 continue
c
c         element loop
c
      do 40 i=1,23
      ic=i
      if(word.eq.etype(i))go to 100
   40 continue
      write(6,2000)word
      stop
   50 continue
      go to (900,60,530,540,560,590,700,710,720,760,770,780,790,800,
     .       600,610,620,650,660),ic
   60 continue
c
c        generate nodes and d.o.f. table
c
	  call mfopen3(0)
	  call eshead(file1,8)
	  call estime(0.0d0)
      numest=0
      nlin=0
      ind=0
      ng=0
      call nodep
      n6e=n5
      n1l=n2l-neq
      do 65 i=1,neq
   65 ia(i+n1l-1)=0
      call esnodes(a(n2),a(n3),a(n4),numnp,idim)
      go to 10
  100 continue
c
c          elements types
c
      go to (110,120,130,140,150,160,170,180,190,200,210,220,230,
     .       240,250,260,270,280,290,300,310),ic
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
  280 continue
c
c         pzel
c
      itype=18
      goto 500
  290 continue
c
c         bemd
c
      itype=19
      goto 500
  300 continue
c
c         feac
c
      itype=20
      goto 500
  310 continue
c
c          tloa
c
      itype=21
      go to 500
  500 continue
      nume=data(2)
      nummat=data(3)
      iloads=data(4)
      ng=ng+1
      if(ng.le.ngroup)go to 510
      write(6,2150)ngroup
      stop
  510 continue
      call gstiff
      if(midest.gt.numest)numest=midest
      go to 10
  530 continue
c
c        form stiffness
c
      call second2(t1)
      call assem
      call second2(t2)
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
      n6=n4+numest+512
      n7=max(n5,n6)
      n6e=n4
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
      call second2(ta)
      istres=0
      if(cdata(2).eq.'stre')istres=1
      iequit=kpeq1+kpeq2+kpeq3
      iep=data(4)
      iback=0
      ncycle=ncycle+1
      call solve(iequit,iep,istres,iback)
      call second2(tb)
      t=tb-ta
      write(6,2120)t
      go to 10
  590 continue
c
c        back sub
c
      call second2(ta)
      ns=1
      istres=0
      if(cdata(2).eq.'stre')istres=1
      iequit=kpeq1+kpeq2+kpeq3
      iep=data(4)
      iback=1
      ncycle=ncycle+1
      call solve(iequit,iep,istres,iback)
      call second2(tb)
      t=tb-ta
      write(6,2140)t
      go to 10
  600 continue
      call second2(ta)
      nstep=data(2)
      dt=data(3)
      nstres=data(4)
      if(ires.ne.0)go to 610
      call coeff
      n3=n2+neq*itwo
      n4=n3+nvect*itwo
      n5=n4+numnp*ndof
      min=nmax+n5-n1l+1
      write(6,2180)min
      if(min.gt.nmax)call abort
      call inital(a(n2),a(n3),a(n4),ndof,nvect)
  610 continue
      write(6,2160)ncycle,time,nstep,dt
      if(nstep.le.0.or.dt.le.0.)call abort
      call march(nstres)
      call second2(tb)
      t=tb-ta
      write(6,2170)nstep,t
      go to 10
  620 continue
      ibk=0
      ipnt=data(2)
      idis=data(3)
      ivel=data(4)
      iacc=data(5)
  630 continue
      call fld
      if(cdata(1).eq.end)go to 10
      ibk=ibk+1
      if(ibk.le.20)go to 640
      write(6,2190)
      call abort
  640 continue
      ipnode(1,ibk)=data(1)
      ipnode(2,ibk)=data(2)
      ipnode(3,ibk)=data(3)
      go to 630
  650 continue
      call rstart(1)
      go to 10
  660 continue
c
c          get modes
c
      call second2(ta)
      call mode
      call second2(tb)
      t=tb-ta
      write(6,2145)t
      go to 10
  700 continue
c
c          set flag for data check
c
      icheck=1
      go to 10
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
      word=cdata(i)
      if(word.eq.'last')go to 10
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
      idmp=1
      if(cdata(2).eq.off)idmp=0
      go to 10
  780 continue
c
c          number of words required for element data
c
      nwelem=data(2)
      go to 10
  790 continue
      ipsm=data(2)
      ipkbar=data(3)
      ipinpt=data(4)
      ipreac=data(5)
      ieldat=data(6)
      ipsdm=data(7)
      ipcnpf=data(8)
      ibigw=data(9)
      write(6,2200)ipsm,ipkbar,ipinpt,ipreac,ieldat,ipsdm,ipcnpf,
     .            ibigw
      go to 10
  800 continue
      kfsi=2
      fsifac=data(2)
      write(6,2210)fsifac
      if(fsifac.eq.0.)stop
      go to 10
  900 continue
      if(cdata(2).ne.gno)call rstart(0)
      call esend()
      call second2(tend)
      t=tend-tstart
      write(6,2146)t
      stop
 1000 format(8a4)
 1010 format(1h1,i4,5x,8a4)
 2000 format(1h0,10x,27hstop unreconized command = ,a10)
 2100 format(1h0,10x,34htime required to form stiffness = ,f13.3,
     .  8h seconds)
 2120 format(1h0,10x,29htime required for solution = ,f13.3,8h seconds)
 2130 format(1h0,10x,45hnumber of words required to input load data = ,
     .       i10)
 2140 format(1h0,10x,29htime required for backsub  = ,f13.3,8h seconds)
 2145 format(1h0,10x,29htime required for modes  =   ,f13.3,8h seconds)
 2146 format(1h0,10x,37htotal time required for solution  =   ,
     .      f13.3,8h seconds)
 2150 format(1h0,10x,27hmaximum numger of groups = ,i5)
 2160 format(1h0,10x,8hcycle = ,i8,8h time = ,1pe12.4,8h nstep = ,i8,
     .   6h dt = ,1pe12.4)
 2170 format(1h0,10x,18htime required for ,i5,8h steps = ,f13.3,
     .  8h seconds)
 2180 format(1h0,10x,55hnumber of words required to  input initial condi
     .tions = ,i10)
 2190 format(1h0,10x,30hstop more than 20 print blocks)
 2200 format(1x,"ipsm= ",i2," ipkbar= ",i2," ipinpt= ",i2," ipreac=",
     . i2," ieldat= ",i2," ipsdm= ",i2," ipcnpf= ",i2," ibigw= ",i2)
 2210 format(1h0,10x,37hfluid structure interaction factor = ,1pe12.4)
      end
*deck rstart
      subroutine rstart(irflag)
      implicit real*8 (a-h,o-z)
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/dyncm1/time,dt,dtold,delta,alpha
      common/dyncm2/idym,imass,idamp,ires,intial,ncycle,nstep,nbdym
      common/stapes/nts1,ntb1,nts2,ntb2,nts3,ntb3
      common/scom/nload,iloads,mvlast
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/ncom/numest,nvect,istoh,maxbh
      common/adr/m1,m2,m3,m4,m5,m6,m7,m8
      common /addb/ neql,neqr,mla,nblock
      common/prcom/ibk,ipnt,idis,ivel,iacc,ipnode(3,20)
      common/dtwo/itwo,ndof,idim
      common/list/data(64),ndum
      common/comtyp/itype,ngroup,ngin,nlin
      common/ewcom/iia(2,128),isec,irl
      common/ewcom1/il(128)
      character*8 head
      common/hdcom/head(8)
      dimension c(5),i1(6),i2(8),i3(3),i4(6),i5(6),i6(4),i7(8),i8(4)
      dimension i9(65),i10(3),i11(4),i12(258),i13(128),i14(6)
      dimension ia(1)
      common a(1)
      integer ia
      equivalence (a(1),ia(1))
      equivalence (c(1),time)
      equivalence (i1(1),n1l),(i2(1),idym),(i3(1),nload)
      equivalence (i4(1),kpri),(i5(1),nmax),(i6(1),numest)
      equivalence (i7(1),m1),(i8(1),neql),(i9(1),ibk)
      equivalence (i10(1),itwo),(i11(1),itype),(i12(1),iia)
      equivalence (i13(1),il),(i14(1),nts1)
      if(irflag.eq.1)go to 100
      if(idym.ne.1.and.idym.ne.3)return
      rewind 8
      read(8)idum
      write(8)head,ncycle,time,dtold,nvect,nload
c
c        dis,vel,acc
c
      nts2=1
      nbl=6*nload
      do 60 nl=1,nbl
      call recs(7,a,nvect,nl)
      write(8)(a(i),i=1,nvect)
   60 continue
      return
  100 continue
c
c        restore restart data
c
      rewind 8
      read(8)idum
      read(8)head,ncycle,time,dtold,nvect,nload
      write(6,1000)head,ncycle,time,dtold
c     call mopen(istoh,nblock,icheck)
      ires=1
      intial=1
      dtold=0.
      nts2=1
      nbl=6*nload
      do 160 nl=1,nbl
      read(8)(a(i),i=1,nvect)
      call wecs(7,a,nvect,nl)
  160 continue
      return
 1000 format(1h0,10x,13hr e s t a r t ,//,14x,7hhead = ,8a4,/
     .       14x,9hncycle = ,i5,/,14x,7htime = ,1pe12.4,/,14x,5hdt = ,
     .       1pe12.4)
      end
*deck tload
      subroutine tload
      implicit real*8 (a-h,o-z)
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/dtwo/itwo,ndof,idim
      common a(1)
      real a
      nf=nume
      np=nummat
      n101=n6e+4*itwo
      n102=n101+nf*np*itwo
      n103=n102+nf*np*itwo
      n104=n103+nf*itwo
      n105=n104+nf
      if(n105.lt.n1l)go to 10
      min=nmax+n105-n1l+1
      write(6,1000)min
      call abort
   10 continue
      call tloadp(a(n101),a(n102),a(n103),a(n104),a(n2),np,nf)
      if(ind.ne.0)return
      midest=n105-(n101-1)
      call ewrite(a(n6e),ng)
      return
 1000 format(1h0,10x,25hincrease working space to,i10,6h words)
      end
*deck tloadp
      subroutine tloadp(tt,ft,fval,ntf,a,npp,nf)
      implicit real*8 (a-h,o-z)
      common/dyncm1/time,dt,dtold,delta,alpha
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/list/data(64),nword
      dimension tt(npp,1),ft(npp,1),fval(1),ntf(1),a(1)
      if(ind.gt.0)go to 80
      do 70 nn=1,nf
      call fld
      n=data(2)
      np=data(3)
      if(np.gt.npp)call abort
      ntf(n)=np
      n1=0
      n2=0
   10 continue
      call fld
      do 20 i=1,64
      n1=n1+1
      if(n1.gt.np)go to 30
      tt(n1,n)=data(i)
   20 continue
   30 continue
      call fld
      do 40 i=1,64
      n2=n2+1
      if(n2.gt.np)go to 50
      ft(n2,n)=data(i)
   40 continue
       go to 10
   50 continue
      write(6,1000)n
      nl=(np-1)/10+1
      is=1
      ie=10
      do 60 i=1,nl
      ie=min(np,ie)
      write(6,1010)(tt(j,n),j=is,ie)
      write(6,1010)(ft(j,n),j=is,ie)
      is=ie+1
      ie=ie+10
   60 continue
   70 continue
      return
   80 if(ind.ne.5)return
c
c        time dependent loads
c
      do 90 n=1,nf
      np=ntf(n)
      call timef(time,fval(n),tt(1,n),ft(1,n),np,n)
   90 continue
      do 110 n=1,neq
      ip=a(n)
      if(ip.eq.0)go to 110
      if(ip.le.nf)go to 100
      write(6,1020)ip
      call abort
  100 continue
      a(n)=fval(ip)
  110 continue
      return
 1000 format(1h0,10x,11h time/value,1x,11hfunction = ,i5)
 1010 format(1x,1p10e12.4)
 1020 format(1h0,10x,24hunknown load function = ,i5)
      end
*deck timef
      subroutine timef(t,f,tt,ft,np,n)
      implicit real*8 (a-h,o-z)
      dimension tt(1),ft(1)
      if(t.ge.tt(1).and.t.le.tt(np))go to 10
      write(6,1000)t,n
      call abort
   10 continue
      do 20 i=2,np
      ip=i
      if(tt(i).ge.t)go to 30
   20 continue
   30 continue
      f=ft(ip-1)+(ft(ip)-ft(ip-1))*(t-tt(ip-1))/(tt(ip)-tt(ip-1))
      return
 1000 format(1h0,10x,20htime out of range = ,1pe12.4,11h function =,i5)
      end
*deck dwrite
      subroutine dwrite(ur,fr,id,ndof,nvect)
      implicit real*8 (a-h,o-z)
      common/dyncm2/idym,imass,idamp,ires,intial,ncycle,nstep,nbdym
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat,kpplot
      common/stapes/nts1,ntb1,nts2,ntb2,nts3,ntb3
      common/prcom/ibk,ipnt,idva(3),ipnode(3,20)
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/dyncm1/time,dt,dtold,delta,alpha
      common/scom/nload,iloads,mvlast
      dimension ur(1),fr(1),id(ndof,1),dxr(6)
      dimension drc(10)
      if(ibk.gt.0)go to 10
      ibk=1
      ipnode(1,1)=1
      ipnode(2,1)=numnp
      ipnode(3,1)=1
      ipnt=1
      idva(1)=1
      idva(2)=1
      idva(3)=0
   10 if(mod(ncycle,ipnt).ne.0)return
      if(idym.gt.0)go to 30
      nts2=1
      ntb2=nvect
      idva(2)=0
      idva(3)=0
      do 20 nl=1,nload
      call recs(3,fr,nvect,nl)
      call wecs(7,fr,nvect,nl)
   20 continue
   30 continue
      do 100 nt=1,3
      it=idva(nt)
      if(it.le.0)go to 100
      if(nt.eq.1)write(6,1010)ncycle,time
      if(nt.eq.2)write(6,1020)ncycle,time
      if(nt.eq.3)write(6,1030)ncycle,time
      nts2=nt
      nloc=0
      mv=nvect
      kount=0
      kend=(10/ndof)*ndof
      do 50 nl=1,nload
      call recs(7,fr,nvect,nl)
      if(nl.eq.nload)mv=mvlast
      do 40 j=1,mv
      nloc=nloc+1
      ur(nloc)=fr(j)
   40 continue
   50 continue
      rewind 8
      read(8)((id(i,j),i=1,ndof),j=1,numnp)
      do 90 nb=1,ibk
      in1=ipnode(1,nb)
      in2=ipnode(2,nb)
      inc=ipnode(3,nb)
      do 80 n=in1,in2,inc
      do 70 i=1,ndof
      dxr(i)=0.
      ip=id(i,n)
      if(ip.le.0)go to 70
      dxr(i)=ur(ip)
   70 continue
      if(kpcomp.eq.1)go to 74
      write(6,1040)n,(dxr(i),i=1,ndof)
      go to 80
   74 continue
      do 78 i=1,ndof
      kount=kount+1
      drc(kount)=dxr(i)
   78 continue
      if(kount.lt.kend.and.(n+inc).le.in2)go to 80
      write(6,1050)n,(drc(i),i=1,kount)
      kount=0
   80 continue
   90 continue
  100 continue
      return
 1010 format(1h0,10x,25hd i s p l a c e m e n t s ,5x,
     .  8hncycle = ,i5,8h time = ,1pe12.4,/)
 1020 format(1h0,10x,19hv e l o c i t i e s ,11x,
     .  8hncycle = ,i5,8h time = ,1pe12.4,/)
 1030 format(1h0,10x,23ha c c l e r a t i o n s ,7x,
     .  8hncycle = ,i5,8h time = ,1pe12.4,/)
 1040 format(i8,1p6e12.4)
 1050 format(i8,1p10e11.3)
      end
      
*deck dplot
      subroutine dplot(ur,fr,id,ndof,nvect)
      implicit real*8 (a-h,o-z)
      common/dyncm2/idym,imass,idamp,ires,intial,ncycle,nstep,nbdym
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat,kpplot
      common/stapes/nts1,ntb1,nts2,ntb2,nts3,ntb3
      common/prcom/ibk,ipnt,idva(3),ipnode(3,20)
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/dyncm1/time,dt,dtold,delta,alpha
      common/scom/nload,iloads,mvlast
      dimension ur(1),fr(1),id(ndof,1),dxr(6)
      dimension drc(10)
      dimension dmin(6),dmax(6)
      ibk=1
      ipnode(1,1)=1
      ipnode(2,1)=numnp
      ipnode(3,1)=1
      ipnt=1
      idva(1)=1
      idva(2)=1
      idva(3)=0
      if(idym.gt.0)go to 30
      nts2=1
      ntb2=nvect
      idva(2)=0
      idva(3)=0
      do 20 nl=1,nload
      call recs(3,fr,nvect,nl)
      call wecs(7,fr,nvect,nl)
   20 continue
   30 continue
      do 100 nt=1,3
      it=idva(nt)
      if(it.le.0)go to 100
      nts2=nt
      nloc=0
      mv=nvect
      kount=0
      kend=(10/ndof)*ndof
      do 50 nl=1,nload
      call recs(7,fr,nvect,nl)
      if(nl.eq.nload)mv=mvlast
      do 40 j=1,mv
      nloc=nloc+1
      ur(nloc)=fr(j)
   40 continue
   50 continue
      do 60 i=1,6
      dmin(i)=  1e66
      dmax(i)= -1e66
   60 continue
      rewind 8
      read(8)((id(i,j),i=1,ndof),j=1,numnp)
      do 90 nb=1,ibk
      in1=ipnode(1,nb)
      in2=ipnode(2,nb)
      inc=ipnode(3,nb)
      do 80 n=in1,in2,inc
      do 70 i=1,ndof
      dxr(i)=0.
      ip=id(i,n)
      if(ip.le.0)go to 70
      dxr(i)=ur(ip)
   70 continue
      do 75 i=1,ndof
      v=dxr(i)
      if(v.gt.dmax(i))dmax(i)=v
      if(v.lt.dmin(i))dmin(i)=v
   75 continue
      if(nt.eq.1)call esdis(dxr,n)
      if(nt.eq.2)call esvel(dxr,n)
   80 continue
   90 continue
      write(6,1060)nt,(dmin(i),i=1,ndof)
      write(6,1070)nt,(dmax(i),i=1,ndof)
  100 continue
      return
 1010 format(1h0,10x,25hd i s p l a c e m e n t s ,5x,
     .  8hncycle = ,i5,8h time = ,1pe12.4,/)
 1020 format(1h0,10x,19hv e l o c i t i e s ,11x,
     .  8hncycle = ,i5,8h time = ,1pe12.4,/)
 1030 format(1h0,10x,23ha c c l e r a t i o n s ,7x,
     .  8hncycle = ,i5,8h time = ,1pe12.4,/)
 1040 format(i8,1p6e12.4)
 1050 format(i8,1p10e11.3)
 1060 format(1h0,5x,i5,5x,8h  min = ,1p6e12.4)
 1070 format(1h0,5x,i5,5x,8h  max = ,1p6e12.4,/)
      end
*deck abort
      
*deck abort
c      subroutine abort
c      implicit real*8 (a-h,o-z)
c      n=0
c      go to (10,10,20,20),n
c   10 continue
c      return
c   20 continue
c      return
c      end
*deck loadef
      subroutine loadef(wa,wb,aa,va,vb,maxa,ncolbv)
      implicit real*8 (a-h,o-z)
      dimension wa(1),wb(1),aa(1),va(1),vb(1),maxa(1),ncolbv(1)
      common/dyncm2/idym,imass,idamp,ires,intial,ncycle,nstep,nbdym
      common/dyncm3/a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10
      common/stapes/nts1,ntb1,nts2,ntb2,nts3,ntb3
      common/tcom/munit,lunit,nstif,nred,nunit
      common/ncom/numest,nvect,istoh,maxbh
      common/scom/nload,iloads,mvlast
      if(idym.le.0)return
      nts3=1
      ntb2=nload
      if(idym.ne.1)go to 280
c
c        form mass component of effective loads
c
      do 40 nl=1,nload
      nts2=4
      call recs(7,wa,nvect,nl)
      nts2=5
      call recs(7,va,nvect,nl)
      do 20 i=1,nvect
      wa(i)=wa(i)*a0+va(i)*a2
   20 continue
      nts2=6
      call recs(7,va,nvect,nl)
      do 30 i=1,nvect
      wa(i)=wa(i)+va(i)*a3
   30 continue
      call wecs(3,wa,nvect,nl)
   40 continue
      if(imass.ne.2)go to 50
      nts1=2
      nts2=7
      call mult(wa,wb,aa,va,vb,maxa,ncolbv,1,lunit,munit,7)
   50 if(imass.ne.1)go to 80
      nts2=7
      nts3=3
      do 70 nl=1,nload
      call recs(3,wa,nvect,nl)
      call recs(7,va,nvect,nl)
      do 60 i=1,nload
      va(i)=va(i)*wa(i)
   60 continue
      call wecs(7,va,nvect,nl)
   70 continue
   80 continue
      nts3=1
      do 140 nl=1,nload
      nts2=4
      call recs(7,wa,nvect,nl)
      nts2=5
      call recs(7,va,nvect,nl)
      do 120 i=1,nvect
      wa(i)=wa(i)*a1+va(i)*a4
  120 continue
      nts2=6
      call recs(7,va,nvect,nl)
      do 130 i=1,nvect
      wa(i)=wa(i)+va(i)*a5
  130 continue
      call wecs(lunit,wa,nvect,nl)
  140 continue
      if(idamp.ne.2)go to 150
      nts1=3
      nts2=8
      call mult(wa,wb,aa,va,vb,maxa,ncolbv,1,lunit,munit,7)
  150 if(idamp.ne.1)go to 180
      nts2=8
      do 170 nl=1,nload
      nts3=1
      call recs(3,wa,nvect,nl)
      nts3=3
      call recs(3,va,nvect,nl)
      do 160 i=1,nvect
      wa(i)=wa(i)*va(i)
  160 continue
      call wecs(7,wa,nvect,nl)
  170 continue
  180 continue
c
c        form effective load vector
c
      nts3=1
      do 270 nl=1,nload
      call recs(3,wa,nvect,nload+nl)
      if(imass.eq.0)go to 240
      nts2=7
      call recs(7,va,nvect,nl)
      do 220 i=1,nvect
      wa(i)=wa(i)+va(i)
  220 continue
  240 if(idamp.eq.0)go to 260
      nts2=8
      call recs(7,va,nvect,nl)
      do 250 i=1,nvect
      wa(i)=wa(i)+va(i)
  250 continue
  260 continue
      call wecs(3,wa,nvect,nl)
  270 continue
      return
  280 if(idym.ne.3)return
c
c        central diff mass
c
      do 340 nl=1,nload
      nts2=1
      call recs(7,wa,nvect,nl)
      nts2=4
      call recs(7,va,nvect,nl)
      do 320 i=1,nvect
      wa(i)=wa(i)*a2-va(i)*a0
  320 continue
      call wecs(3,wa,nvect,nl)
  340 continue
      if(imass.ne.2)go to 900
      nts1=2
      nts2=7
      call mult(wa,wb,aa,va,vb,maxa,ncolbv,1,lunit,munit,7)
c
c       damp
c
      nts3=1
      do 420 nl=1,nload
      nts2=4
      call recs(7,wa,nvect,nl)
      do 410 i=1,nvect
      wa(i)=wa(i)*a1
  410 continue
      call wecs(3,wa,nvect,nl)
  420 continue
      if(idamp.ne.2)go to 900
      nts1=3
      nts2=8
      call mult(wa,wb,aa,va,vb,maxa,ncolbv,1,lunit,munit,7)
c
c       merge m and c
c
      do 440 nl=1,nload
      nts2=7
      call recs(7,wa,nvect,nl)
      nts2=8
      call recs(7,va,nvect,nl)
      do 430 i=1,nvect
      wa(i)=wa(i)+va(i)
  430 continue
      nts2=7
      call wecs(7,wa,nvect,nl)
  440 continue
c
c       k*u
c
      nts2=1
      do 450 nl=1,nload
      call recs(7,wa,nvect,nl)
      call wecs(3,wa,nvect,nl)
  450 continue
      nts1=1
      nts2=8
      call mult(wa,wb,aa,va,vb,maxa,ncolbv,1,lunit,munit,7)
c
c        form effective load vector
c
      nts3=1
      do 570 nl=1,nload
      call recs(3,wa,nvect,nload+nl)
      if(imass.eq.0)go to 540
      nts2=7
      call recs(7,va,nvect,nl)
      do 520 i=1,nvect
      wa(i)=wa(i)+va(i)
  520 continue
  540 if(idamp.eq.0)go to 560
      nts2=8
      call recs(7,va,nvect,nl)
      do 550 i=1,nvect
      wa(i)=wa(i)-va(i)
  550 continue
  560 continue
      call wecs(3,wa,nvect,nl)
  570 continue
      return
  900 continue
      write(6,1000)imass,idamp
      stop
 1000 format(' stop imass or idamp bad ',2i5)
      end
*deck march
      subroutine march(nstres)
      implicit real*8 (a-h,o-z)
      common/dyncm1/time,dt,dtold,delta,alpha
      common/dyncm2/idym,imass,idamp,ires,intial,ncycle,nstep,nbdym
      if(intial.gt.0)go to 10
      write(6,1000)
      call abort
   10 if(idym.gt.0)go to 20
      write(6,1010)
      call abort
   20 continue
      iback=0
      if(dt.eq.dtold)iback=1
      call coeff
      dtold=dt
      iep=0
      iequit=0
      if(nstres.le.0)nstres=ncycle+nstep+1
      do 30 i=1,nstep
      time=time+dt
      call estime(time)
      ncycle=ncycle+1
      istres=0
      if(mod(ncycle,nstres).eq.0)istres=1
      call solve(iequit,iep,istres,iback)
      iback=1
   30 continue
      return
 1000 format(1h0,10x,27herror no initial conditions)
 1010 format(1h0,10x,23herror not a dynamic run)
      end
*deck coeff
      subroutine coeff
      implicit real*8 (a-h,o-z)
      common/dyncm1/time,dt,dtold,delta,alpha
      common/dyncm2/idym,imass,idamp,ires,intial,ncycle,nstep,nbdym
      common/dyncm3/a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10
      d1=.5d0*(delta+.5d0)
      d2=d1*d1
      deal=delta/alpha
      a0=1.d0/(alpha*dt*dt)
      a1=deal/dt
      a2=1.d0/(alpha*dt)
      a3=.5d0/alpha-1.d0
      a4=deal-1.d0
      a5=dt*(.5d0*deal-1.d0)
      a6=a0
      a7=-a2
      a8=-a3
      a9=dt*(1.d0-delta)
      a10=delta*dt
      if(idym.ne.3)return
      a0=1.d0/(dt*dt)
      a1=.5d0/dt
      a2=2.d0*a0
      a3=1.d0/a2
      return
      end
*deck inital
      subroutine inital(f,fv,id,ndof,nvect)
      implicit real*8 (a-h,o-z)
      common/dyncm3/a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10
      common/dyncm1/time,dt,dtold,delta,alpha
      common/dyncm2/idym,imass,idamp,ires,intial,ncycle,nstep,nbdym
      common/stapes/nts1,ntb1,nts2,ntb2,nts3,ntb3
      common/number/nmax,numnp,nume,neq,nwk,ma
      character*8 cdata
      common/clist/cdata(64)
      common/list/data(64),nword
      common/scom/nload,iloads,mvlast
      dimension f(1),fv(1),id(ndof,1)
      character*8 end
      data end/'end'/
      intial=1
      ierr=0
      ip=data(2)
      rewind 8
      read(8)((id(i,j),i=1,ndof),j=1,numnp)
      ns=0
      nload=(neq-1)/nvect+1
      if(ires.gt.0)go to 30
      do 10 i=1,nvect
      fv(i)=0.
   10 continue
      nts2=1
      ntb2=nload
      nbl=6*nload
      do 20 nl=1,nbl
      call wecs(7,fv,nvect,nl)
   20 continue
      nts2=3
   30 continue
      ns=ns+1
      nts2=nts2+1
      if(ip.eq.1)go to 35
      if(ns.eq.1)write(6,1000)
      if(ns.eq.2)write(6,1010)
      if(ns.eq.3)write(6,1020)
   35 continue
      do 40 i=1,neq
      f(i)=0.
   40 continue
   50 continue
      call fld
      if(cdata(1).eq.end)go to 80
      nod=data(1)
      val=data(2)
      idof=data(3)
      if(idof.le.0)idof=1
      is=nod
      ie=data(4)
      inc=data(5)
      if(ie.lt.is)ie=is
      if(inc.le.0)inc=1
      do 70 i=is,ie,inc
      nod=i
      if(ip.ne.1)write(6,1030)nod,val,idof
      if(nod.ge.1.and.nod.le.numnp)go to 60
      write(6,1040)nod
      ierr=1
      go to 50
   60 continue
      inode=id(idof,nod)
      if(inode.le.0)go to 70
      f(inode)=f(inode)+val
   70 continue
      go to 50
   80 if(ires.gt.0)go to 130
      nloc=0
      do 120 nl=1,nload
      do 110 j=1,nvect
      fv(j)=0.
      nloc=nloc+1
      if(nloc.gt.neq)go to 110
      fv(j)=f(nloc)
  110 continue
      call wecs(7,fv,nvect,nl)
  120 continue
  130 if(ns.ge.3)go to 140
      go to 30
  140 if(ierr.gt.0)call abort
      if(idym.ne.3)return
      do 170 nl=1,nload
      nts2=4
      call recs(7,f,nvect,nl)
      nts2=1
      call wecs(7,f,nvect,nl)
      nts2=5
      call recs(7,fv,nvect,nl)
      nts2=2
      call wecs(7,fv,nvect,nl)
      do 150 i=1,nvect
      f(i)=f(i)-dt*fv(i)
  150 continue
      nts2=6
      call recs(7,fv,nvect,nl)
      nts2=3
      call wecs(7,fv,nvect,nl)
      do 160 i=1,nvect
      f(i)=f(i)+a3*fv(i)
  160 continue
      nts2=4
      call wecs(7,f,nvect,nl)
  170 continue
 1000 format(1h0,10x,40hi n i t i a l  d i s p l a c e m e n t s, //
     .  7x,4hnode,5x,5hvalue,4x,3hdof)
 1010 format(1h0,10x,34hi n i t i a l  v e l o c i t i e s ,//
     .  7x,4hnode,5x,5hvalue,4x,3hdof)
 1020 format(1h0,10x,40hi n i t i a l  a c c e l e r a t i o n s,//
     .  7x,4hnode,5x,5hvalue,4x,3hdof)
 1030 format(i10,1pe12.4,i5)
 1040 format(1h0,10x,17hundefined node = ,i5)
      end
*deck newdav
      subroutine newdav(dis,disp)
      implicit real*8 (a-h,o-z)
      common/dyncm2/idym,imass,idamp,ires,intial,ncycle,nstep,nbdym
      common/dyncm3/a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10
      common/stapes/nts1,ntb1,nts2,ntb2,nts3,ntb3
      common/ncom/numest,nvect,istoh,maxbh
      common/scom/nload,iloads,mvlast
      dimension dis(1),disp(1)
c
c        incremental displacements
c
      if(idym.le.0)return
      nts3=1
      if(idym.ne.1)go to 100
      do 20 nl=1,nload
      call recs(3,dis,nvect,nl)
      nts2=4
      call recs(7,disp,nvect,nl)
      nts2=1
      call wecs(7,dis,nvect,nl)
      do 10 i=1,nvect
      dis(i)=dis(i)-disp(i)
   10 continue
      nts2=7
      call wecs(7,dis,nvect,nl)
   20 continue
c
c        new accelerations
c
      do 50 nl=1,nload
      nts2=7
      call recs(7,dis,nvect,nl)
      nts2=5
      call recs(7,disp,nvect,nl)
      do 30 i=1,nvect
      dis(i)=dis(i)*a6+disp(i)*a7
   30 continue
      nts2=6
      call recs(7,disp,nvect,nl)
      do 40 i=1,nvect
      dis(i)=dis(i)+disp(i)*a8
   40 continue
      nts2=3
      call wecs(7,dis,nvect,nl)
   50 continue
c
c        new velocities
c
      do 80 nl=1,nload
      nts2=5
      call recs(7,dis,nvect,nl)
      nts2=6
      call recs(7,disp,nvect,nl)
      do 60 i=1,nvect
      dis(i)=dis(i)+disp(i)*a9
   60 continue
      nts2=3
      call recs(7,disp,nvect,nl)
      do 70 i=1,nvect
      dis(i)=dis(i)+disp(i)*a10
   70 continue
      nts2=2
      call wecs(7,dis,nvect,nl)
   80 continue
c
c        update old dis,vel,acc
c
      do 90 nl=1,nload
      nts2=1
      call recs(7,dis,nvect,nl)
      nts2=4
      call wecs(7,dis,nvect,nl)
      nts2=2
      call recs(7,dis,nvect,nl)
      nts2=5
      call wecs(7,dis,nvect,nl)
      nts2=3
      call recs(7,dis,nvect,nl)
      nts2=6
      call wecs(7,dis,nvect,nl)
   90 continue
      return
  100 if(idym.ne.3)return
c
c        new accelerations
c
      nts3=1
      do 150 nl=1,nload
      nts2=3
      call recs(7,dis,nvect,nl)
      nts2=6
      call wecs(7,dis,nvect,nl)
c
      nts2=4
      call recs(7,dis,nvect,nl)
      nts2=1
      call recs(7,disp,nvect,nl)
      do 130 i=1,nvect
      dis(i)=dis(i)-2.0d0*disp(i)
  130 continue
c
      call recs(3,disp,nvect,nl)
      do 140 i=1,nvect
      dis(i)=a0*(dis(i)+disp(i))
  140 continue
      nts2=3
      call wecs(7,dis,nvect,nl)
  150 continue
c
c        new velocities
c
      do 180 nl=1,nload
      nts2=2
      call recs(7,dis,nvect,nl)
      nts2=5
      call wecs(7,dis,nvect,nl)
c
      call recs(3,dis,nvect,nl)
      nts2=4
      call recs(7,disp,nvect,nl)
      do 160 i=1,nvect
      dis(i)=a1*(dis(i)-disp(i))
  160 continue
      nts2=2
      call wecs(7,dis,nvect,nl)
  180 continue
c
c        update dis
c
      nts3=1
      do 200 nl=1,nload
      call recs(3,dis,nvect,nl)
      nts2=1
      call recs(7,disp,nvect,nl)
      nts2=4
      call wecs(7,disp,nvect,nl)
      nts2=1
      call wecs(7,dis,nvect,nl)
  200 continue
      end
*deck addma
      subroutine addma(a,s,lm,nd)
      implicit real*8 (a-h,o-z)
      dimension a(1),s(1),lm(1)
      do 100 i=1,nd
      ii=lm(i)
      if(ii)100,100,90
   90 continue
      a(ii)=a(ii)+s(i)
  100 continue
      return
      end
*deck preaem
      subroutine preaem(ntable,ke,ncolbv,ee,nblock)
      implicit real*8 (a-h,o-z)
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
*deck addban
      subroutine addban(a,maxa,s,lm,nd)
      implicit real*8 (a-h,o-z)
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
      implicit real*8 (a-h,o-z)
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
      implicit real*8 (a-h,o-z)
      common/dyncm2/idym,imass,idamp,ires,intial,ncycle,nstep,nbdym
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/scom/nload,iloads,mvlast
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/tcom/munit,lunit,nstif,nred,nunit
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/ncom/numest,nvect,istoh,maxbh
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/freqif/istohf,n1a,n1b,n1c
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
      idym=data(2)
      nvect=data(3)
      imass=data(4)
      idamp=data(5)
      mbh=100+neq/16
      if(nvect.le.0)nvect=neq
      lbs=2*nvect
      if(nvect.ne.neq)lbs=lbs*2
      if(idym.eq.2)lbs=lbs+8*neq
      call addres(ia(n1),ia(n1l))
      n1a=n1+neq+1
      n1b=n1a+neq
      iblock=4
      nblock=1
      dnom=1.
      npass=0
      nbdym=0
      if(imass.eq.2.or.idamp.eq.2)nbdym=1
      mstore=1+lbs*itwo+numest+512+ng
c
c        calculate maximum block size for static solution
c
   30 mel=2*iblock
      istorl=(nmax-mstore-mel-2*mbh)/(itwo*dnom)
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
      maxbh=max(maxbh,ia(i))
   50 continue
      maxbh=maxbh+1
      if(maxbh.le.mbh)go to 60
      mbh=maxbh+10
      go to 30
   60 continue
      if(nbdym.eq.0.or.nblock.gt.1)go to 65
      dnom=2.
      if(npass.gt.0)go to 65
      npass=1
      go to 30
   65 continue
      mam=nwk/neq+1
      write(6,2040)neq,nwk,ma,mam,istoh,nblock,maxbh,nvect
      write(6,2050)(i,i=1,nblock)
      write(6,2060)(ia(i),i=n1a,nn)
      nn=n1b+nblock-1
      write(6,2070)(ia(i),i=n1b,nn)
      nn=n1a+nblock
      do 70 i=1,nblock
   70 ia(nn+i-1)=ia(n1b+i-1)
      n1b=nn
      n1c=n1b+nblock
      n2=n1c
      if(idym.ne.2)go to 80
      rewind 35
      write(35)(ia(i),i=n1,n1c)
   80 continue
c     call mopen(istoh,nblock,icheck)
      khbb=0
      do 100 i=1,nblock
      call wecs(2,ia(khbb+n1),maxbh,i)
      khbb=khbb+ia(n1a+i-1)
  100 continue
      npt=2*nblock
      n5l=nmax-ng+1
      n4l=n5l-nblock
      n3l=n4l-nblock
      n2l=n3l-maxbh
      n1l=n2l-maxbh
      if(nblock.eq.1)n1l=n2l
      do 110 i=1,npt
      ia(n3l+i-1)=ia(n1a+i-1)
  110 continue
c
c        preassem pass
c
      n6e=n1+nblock*ng
      n7=n6e+numest+512
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
      n3=n2+istoh*itwo
      n4=n3+istoh*itwo
      n5=n4
      if(nblock.le.1)n5=n3
      n6e=n5
      n7=n6e+numest+512
      min=nmax+n7-n1l+1
      write(6,2080)min
      if(min.le.nmax)go to 120
      stop
  120 continue
      call assem1(a(n2),a(n3),a(n4),a(n6e),a(n1l),a(n3l),a(n5l),
     .   istoh,maxbh,munit,0)
      return
 2020 format(1h0,10x,25herror number of blocks = ,i10)
 2040 format(1h1,
     .17htotal system data ,///5x,
     .55hnumber of equations                                =   ,i10//5x,
     .55hnumber of matrix elements                          =   ,i10//5x,
     .55hmaximum half bandwidth                             =   ,i10//5x,
     .55hmean half bandwidth                                =   ,i10//5x,
     .55hmaximum block length                               =   ,i10//5x,
     .55hnumber of blocks                                   =   ,i10//5x,
     .55hmaximum column block size                          =   ,i10//5x,
     .55hvector block size                                  =   ,i10//)
 2050 format(10x,16h number of block ,12x,(14i6,/))
 2060 format(10x,28h number of columns per block ,(14i6/))
 2070 format(10x,21h first coupling block ,7x,(14i6/))
 2080 format(1h0,10x,41hstorage required to assemble stiffness = ,i11)
 2090 format(1h0,10x,32hstorage required for preassem = ,i11)
      end
*deck assem1
      subroutine assem1(a,aa,cc,ee,maxa,ncolbv,ke,istoh,maxbh,munit,ifl)
      implicit real*8 (a-h,o-z)
      common/dyncm2/idym,imass,idamp,ires,intial,ncycle,nstep,nbdym
      common/dyncm3/a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10
      common/stapes/nts1,ntb1,nts2,ntb2,nts3,ntb3
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/addb/neql,neqr,mla,nblock
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/tapcom/nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlin
      dimension a(1),aa(1),cc(1),maxa(1),ncolbv(1),ee(1)
      dimension ke(1)
      if(icheck.eq.1)return
      rewind 9
      if(ifl.gt.0)go to 110
c
c        form stiffness matrix
c
      nbl=3*nblock
      nts1=1
      ntb1=nblock
      do 5 l=1,nbl
      call wecs(1,a,istoh,l)
    5 continue
      if(ng.eq.1)call eread(ee,1)
      ipt=0
      neql=1
      neqr=0
      mla=0
      n2s=n2
      do 100 l=1,nblock
      read(9)(ke(i),i=1,ng)
      call recs(munit,maxa,maxbh,l)
      mla=maxa(1)-1
      neqr=neqr+ncolbv(l)
      if(nblock.gt.1)go to 40
c
c        one solution block
c
      do 30 irac=1,3
      kpri=irac
      nts1=irac
      do 10 i=1,istoh
      a(i)=0.
   10 continue
      do 20 i=1,ng
      if(ke(i).eq.0)go to 20
      if(ng.ne.1)call eread(ee,i)
      call gstiff
   20 continue
      call wecs(1,a,istoh,l)
   30 continue
      go to 90
   40 continue
c
c        two or more blocks
c
      do 50 i=1,istoh
      a(i)=0.
      aa(i)=0.
   50 continue
      do 60 i=1,ng
      if(ke(i).eq.0)go to 60
      if(ng.ne.1)call eread(ee,i)
      kpri=1
      n2=n2s
      call gstiff
      n2=n3
      kpri=2
      call gstiff
   60 continue
      n2=n2s
      nts1=1
      call wecs(1,a,istoh,l)
      nts1=2
      call wecs(1,aa,istoh,l)
      do 70 i=1,istoh
      a(i)=0.
   70 continue
      kpri=3
      do 80 i=1,ng
      if(ke(i).eq.0)go to 80
      if(ng.ne.1)call eread(ee,i)
      call gstiff
   80 continue
      nts1=3
      call wecs(1,a,istoh,l)
   90 continue
      neql=neql+ncolbv(l)
  100 continue
      return
  110 continue
c
c        final assembly
c
      if(idym.ne.0)go to 150
      kpri=1
      if(ng.eq.1)call eread(ee,1)
      neqr=0
      neql=1
      nts1=1
      do 140 l=1,nblock
      call recs(1,a,istoh,l)
      if(nlin.le.0)go to 130
      call recs(munit,maxa,maxbh,l)
      mla=maxa(1)-1
      neqr=neqr+ncolbv(l)
      read(9)(ke(i),i=1,ng)
      do 120 i=nlin,ng
      if(ke(i).eq.0)go to 120
      if(ng.ne.1)call eread(ee,i)
      call gstiff
  120 continue
      neql=neql+ncolbv(l)
  130 continue
      call wecs(4,a,istoh,l)
  140 continue
      return
c
c        dynamic final assembly
c
  150 continue
      do 300 l=1,nblock
      if(idym.eq.3)go to 160
      nts1=1
      call recs(1,a,istoh,l)
      go to 170
  160 continue
      do 165 i=1,istoh
      a(i)=0.d0
  165 continue
  170 continue
      if(imass.ne.2)go to 190
      nts1=2
      call recs(1,aa,istoh,l)
      do 180 i=1,istoh
      a(i)=a(i)+aa(i)*a0
  180 continue
      go to 220
  190 continue
      if(imass.ne.1)go to 220
      nts3=3
  220 if(idamp.ne.2)go to 240
      nts1=3
      call recs(1,aa,istoh,l)
      do 230 i=1,istoh
      a(i)=a(i)+aa(i)*a1
  230 continue
      go to 270
  240 if(idamp.ne.1)go to 270
      nts3=4
  270 continue
      call wecs(4,a,istoh,l)
  300 continue
      return
      end
*deck colht
      subroutine colht (mht,nd,lm)
      implicit real*8 (a-h,o-z)
c
      dimension lm(1),mht(1)
c
      ls=100000000
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
*deck colsol
      subroutine colsol(maxa,maxa2,ncolbv,icopl,va,vb,a,b,
     .                  da,db,nblock,neq,istorl,nvect,mvect,
     .                  maxbh,nstif,nred,lunit,munit,nunit,kkk)
      implicit real*8 (a-h,o-z)
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
   70 kk=min(ic,nd)
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
      kh=min(kdif,n-1)
      ks=n+khbb
  130 continue
      if (ks.le.mlast) go to 140
      mhold=mlast
      mlast=mlast+mvect
      if (mvectb.gt.0) then
      call wecs (nunit,da,mvect,mvectb)
      endif
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
  160 kk=min(ic,nd)
      c=0.
      do 170 l=1,kk
      c=c+a(ki+l)*a(klt+l)
  170 continue
      a(klt)=a(klt)-c
  180 k=k+1
c
  190 continue
      if (kss.le.1) go to 210
      kup=min(ku,kl+ks-mhold-2)
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
      kup=min(ku,kl+mvect-1)
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
      kup=min(ku,kl+ks-nhold-2)
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
      kup=min(ku,kl+nvect-1)
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
      mv=min(nvect,neq-nlast)
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
      kup=min(ku,kl+ks-nhold-2)
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
      kup=min(ku,kl+nvect-1)
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
*deck con
      subroutine con
      implicit real*8 (a-h,o-z)
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/scom/nload,iloads,mvlast
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/dtwo/itwo,ndof,idim
      common/tapcom/nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlin
      common a(1)
      real a
      nd=nummat
      nk=nd+1
      n101=n6e+4*itwo
      n102=n101+nk*nume*itwo
      n103=n102+nd*nd*itwo
      n104a=n103+nd*3*nume*itwo
      n104=n104a+nd*itwo
      n105=n104+nd*nd*itwo
      n106=n105+nd*nume
      if(n106.lt.n1l)go to 20
      min=nmax+n106-n1l+1
      write(6,1000)min
      stop
   20 if(ind.ne.0)go to 30
      call conp(a(n101),a(n105),a(n1l),a(n2l),nd,nk,ndof)
      midest=n106-(n101-1)
      call ewrite(a(n6e),ng)
      return
   30 if(ind.ne.1)go to 40
      call rcon(a(n103),a(n104a),a(n2),a(n101),a(n102),a(n104),a(n105),
     .          a(n1l),nd,nk,0)
      call ewrit2(a(n6e),njunk)
      return
   40 if(ind.ne.2.and.ind.ne.4.and.ind.ne.5)return
      nadd=n4
      if(ind.eq.5)nadd=n2
      call rcon(a(n103),a(n104a),a(nadd),a(n101),a(n102),a(n104),
     .          a(n105),a(n1l),nd,nk,1)
      return
 1000 format(1h0,10x,48hstop working space must be increased to at least
     .                  ,i10,6h words)
      end
*deck conp
      subroutine conp(cc,lm,mht,id,nd,nk,ndof)
      implicit real*8 (a-h,o-z)
      dimension cc(nk,1),lm(nd,1),mht(1),id(ndof,1)
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat,kpplot
      common/tapcom/nt11,nt31
      character*8 cdata
      common/clist/cdata(64)
      common/list/data(64),ndum
      character*8 dof
      data dof/'dof'/
      do 80 ne=1,nume
      call fld
      if(cdata(1).eq.dof)go to 30
c
c        constraints referenced by nodes
c
      do 20 i=1,nd
      node=data(2*i)
      idof=data(2*i+1)
      if(node.ge.1.and.node.le.numnp)go to 10
      write(6,1000)node
      stop
   10 continue
      lm(i,ne)=id(idof,node)
   20 continue
      nst=2*nd+1
      go to 60
   30 continue
c
c        constraint referenced by dof
c
      do 50 i=1,nd
      ii=data(i+1)
      if(ii.ge.1.and.ii.le.neq)go to 40
      write(6,1010)ii
      stop
   40 continue
      lm(i,ne)=ii
   50 continue
      nst=nd+1
   60 continue
c
c        read the constraints
c
      do 70 i=1,nk
      cc(i,ne)=data(i+nst)
   70 continue
      call colht(mht,nd,lm(1,ne))
      if(kpelem.eq.0)go to 80
      write(6,1015)
      write(6,1020)(lm(i,ne),i=1,nd)
      write(6,1030)(cc(i,ne),i=1,nk)
   80 continue
      return
 1000 format(1h0,10x,22hstop undefined node = ,i10)
 1010 format(1h0,10x,21hstop undefined dof = ,i10)
 1015 format(1h0,10x,11hconstraints)
 1020 format(10i12)
 1030 format(1p10e12.4)
      end
*deck echeck
      subroutine echeck (lm,nd,icode)
      implicit real*8 (a-h,o-z)
      integer lm(1)
      common /addb/ neql,neqr,mla,nblock
c
      icode=1
      if (nblock.gt.1) go to 10
      icode=0
      return
   10 do 100 i=1,nd
      ii=lm(i)
      if (ii.lt.neql) go to 100
      if (ii.gt.neqr) go to 100
      icode=0
      go to 200
  100 continue
  200 return
      end
*deck equit
      subroutine equit(aa,ab,b,ca,cb,maxa,ncolbv,a,neq,istoh,
     .                 nvect,munit,nstif,lunit,nunit,iep)
      implicit real*8 (a-h,o-z)
c
c        calculate   f-k*x
c
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat,kpplot
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
      dnorm=dsqrt(dnorm)
      write(6,1000)dnorm
      if(kpeq2.eq.1)call writed(aa,a,a,ndof,nvect,nunit,1)
      if(kpeq3.eq.1)call writed(aa,a,a,ndof,nvect,lunit,2)
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
*deck ewrite
      subroutine ewrite(e,n)
      implicit real*8 (a-h,o-z)
      common/scom/nload,iloads,mvlast
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,new,nwk,ma
      common/comtyp/itype,ngroup,ngin,nlin
      common/dtwo/itwo,ndof,idim
      common/ewcom/ia(2,128),isec,irl
      common/ewcom1/il(128)
      dimension e(*)
      data nsec/512/
      nw=midest+4*itwo
      il(n)=iloads
      if(n.ne.1)go to 10
      isec=0
   10 continue
      e(1)=nume
      e(2)=nummat
      e(3)=itype
      e(4)=nw
      call hsiow(31,e,nw,isec*nsec)
      ia(1,n)=nw
      ia(2,n)=isec
      isec=isec+(nw-1)/nsec+1
      return
      end
      subroutine ewrit2(e,n)
      implicit real*8 (a-h,o-z)
      common/scom/nload,iloads,mvlast
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,new,nwk,ma
      common/comtyp/itype,ngroup,ngin,nlin
      common/dtwo/itwo,ndof,idim
      common/ewcom/ia(2,128),isec,irl
      common/ewcom1/il(128)
      dimension e(*)
      data nsec/512/
      n=irl
      nw=ia(1,n)
      isec=ia(2,n)
      e(1)=nume
      e(2)=nummat
      e(3)=itype
      e(4)=nw
      call hsiow(31,e,nw,isec*nsec)
      ia(1,n)=nw
      ia(2,n)=isec
      isec=isec+(nw-1)/nsec+1
      return
      end
      subroutine eread(e,n)
      implicit real*8 (a-h,o-z)
      common/scom/nload,iloads,mvlast
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,new,nwk,ma
      common/comtyp/itype,ngroup,ngin,nlin
      common/dtwo/itwo,ndof,idim
      common/ewcom/ia(2,128),isec,irl
      common/ewcom1/il(128)
      dimension e(*)
      data nsec/512/
      nw=ia(1,n)
      isec=ia(2,n)
      call hsior(31,e,nw,isec*nsec)
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
      subroutine ffin
      implicit real*8 (a-h,o-z)
      common/tapes/ntape5,ntape6
      common/flist/fdata(64),nflag(64),nword
      common/clist/cdata(64)
      character*32 edum
      character*8 cdata,wlast
      character*1 sum(80),x(13),stemp(32),blank,comma,dollar,slash,save
      data x/'1','2','3','4','5','6','7','8','9','0','.','+','-'/
      data blank,comma,dollar,slash/' ', ',', '$', '/'/
      data wlast/'last'/
      ntape5=5
      ntape6=6
      iline=1
   90 continue
      iline=iline+1
      do 100 j=1,64
      cdata(j)=blank
      nflag(j)=0
  100 fdata(j)=0.0
      nword=0
  120 continue
c     if(ntape5.eq.5)write(6,1000)
      read(ntape5,3000,end=400,err=400)sum
  125 continue
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
      nflag(nword)=iflag
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
      do 220 i=1,32
  220 stemp(i)=blank
      n=0
      do 240 i=ifirst,nlast
      if (sum(i).eq.blank) go to 240
      n=n+1
      stemp(n)=sum(i)
      if(n.gt.32)go to 260
  240 continue
  260 continue
      if (iflag.ne.1) go to 320
      do 280 i=1,32
      m=33-i
      if (n.eq.0) go to 280
      save=stemp(n)
      stemp(n)=blank
      stemp(m)=save
      n=n-1
  280 continue
      write(edum,3020,err=400)stemp
      read(edum,3040,err=400)fdata(nword)
      go to 340
  320 continue
      stemp(5)=' '
      stemp(6)=' '
      stemp(7)=' '
      stemp(8)=' '
      write(cdata(nword),3050,err=400)(stemp(i),i=1,8)
  340 continue
      if(sum(ichar).eq.slash) go to 120
      if(sum(ichar).eq.dollar)go to 360
      go to 130
  350 continue
      nword=nword+1
  360 continue
      if(cdata(1).eq.'tap')goto 400
      cdata(nword)=wlast
      write(11)nword,(fdata(i),i=1,nword),(cdata(i),i=1,nword)
      goto 90
  400 continue
      rewind 11
      return
c
 1000 format('? ',$)
 2000 format('error reading input ')
 3000 format(80a1)
 3010 format(i5,5x,80a1)
 3020 format(32a1)
 3030 format(4a1)
 3040 format(e32.0)
 3050 format(8a1)
      end
*deck fld
      subroutine fld
      implicit real*8 (a-h,o-z)
      common/list/data(64),nword
      character*8 cdata
      common/clist/cdata(64)
      data nt11/11/
      read(11)nw,(data(i),i=1,nw),(cdata(i),i=1,nw)
      nwp=nw+1
      do 10 i=nwp,64
      data(i)=0.d0
      cdata(i)=' '
   10 continue
      return
      end
*deck gstiff
      subroutine gstiff
      implicit real*8 (a-h,o-z)
      common/comtyp/itype,ngroup,ngin,nlin
      go to (10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,
     .  170,180,190,200,210),itype
   10 continue
      call react
      return
   20 continue
      call tapein
      return
   30 continue
      call con
      return
   40 continue
      call user
      return
   50 continue
      call quad4
      return
   60 continue
      call tri
      return
   70 continue
      call beam
      return
   80 continue
      call axsh
      return
   90 continue
      call pquad4
      return
  100 continue
      call hex8
      return
  110 continue
      call phex
      return
  120 continue
      call ptri
      return
  130 continue
      call beam3d
      return
  140 continue
      call atri
       return
  150 continue
c     call aqua
       return
  160 continue
      call ashl
      return
  170 continue
      call astf
      return
  180 continue
      call pzel
      return
  190 continue
      call bemd
      return
  200 continue
      call freac
      return
  210 continue
      call tload
      return
      end
*deck load
      subroutine load(fr,id,ee,fc,ndof,nvect,ifg)
      implicit real*8 (a-h,o-z)
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat,kpplot
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/stapes/nts1,ntb1,nts2,ntb2,nts3,ntb3
      common/scom/nload,iloads,mvlast
      common/ewcom1/il(128)
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/tapcom/nt11,nt31
      character*8 cdata
      common/clist/cdata(64)
      common/list/data(64),ndum
      dimension fr(1),id(ndof,1)
      dimension fc(1),ee(1)
      character*8 end
      data end/'end'/
      nts3=1
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
      do 10 i=1,neq
      fr(i)=0.
   10 continue
      if(kpload.gt.0)write(6,2000)ns
   20 continue
      call fld
      if(cdata(1).eq.end)go to 60
      nod=data(1)
      val=data(2)
      idof=data(3)
      if(idof.le.0)idof=1
      is=nod
      ie=data(4)
      inc=data(5)
      if(ie.lt.is)ie=is
      if(inc.le.0)inc=1
      do 40 i=is,ie,inc
      nod=i
      if(kpload.gt.0)write(6,2010)nod,val,idof
      if(nod.ge.1.and.nod.le.numnp)go to 30
      write(6,2020)
      ierr=1
      go to 20
   30 continue
      inode=id(idof,nod)
      if(inode.le.0)go to 40
      fr(inode)=fr(inode)+val
   40 continue
      go to 20
   60 continue
      nload=(neq-1)/nvect+1
      ntb3=nload
      nloc=0
      nlast=0
      do 80 i=1,nload
      mv=min(nvect,neq-nlast)
      nlast=nlast+nvect
      do 70 j=1,nvect
      fc(j)=0.
      nloc=nloc+1
      if(nloc.gt.neq)go to 70
      fc(j)=fr(nloc)
   70 continue
      call wecs(3,fc,nvect,i)
   80 continue
      mvlast=mv
      do 90 i=1,nload
      call recs(3,fc,nvect,i)
      call wecs(3,fc,nvect,i+nload)
   90 continue
      do 100 i=1,nload
      nts3=1
      call recs(3,fc,nvect,i)
      nts3=3
      call wecs(3,fc,nvect,i)
  100 continue
      nts3=1
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
      nts3=3
      call recs(3,fr,nvect,nl)
      if(nl.eq.nload)mv=mvlast
      do 170 j=1,mv
      nloc=nloc+1
      fc(nloc)=fr(j)
  170 continue
  180 continue
      inds=ind
      ind=5
      do 190 n=1,ng
      if(il(n).eq.0)go to 190
      call eread(ee,n)
      call gstiff
  190 continue
      ind=inds
      nloc=0
      do 210 nl=1,nload
      do 200 j=1,nvect
      fr(j)=0.
      nloc=nloc+1
      if(nloc.gt.neq)go to 200
      fr(j)=fc(nloc)
  200 continue
      nts3=1
      call wecs(3,fr,nvect,nl)
      nts3=2
      call wecs(3,fr,nvect,nl)
  210 continue
      nts3=1
      return
 2000 format(1h0,10x,63hn o d a l   p o i n t   f o r c e s   f o r   l
     .o a d   c a s e ,i3,//
     .  7x,4hnode,5x,5hvalue,4x,3hdof)
 2010 format(i10,1pe12.4,2i5)
 2020 format(5x,19hstop undefined node)
      end
*deck mult
      subroutine mult(aa,ab,b,ca,cb,maxa,ncolbv,nstif,lunit,munit,
     .                nunit)
      implicit real*8 (a-h,o-z)
c
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c .                                                                   .
c .   p r o g r a m                                                   .
c .      . to calculate   a = b*c   ,   where  b  is stored in        .
c .        compacted form  ,  a  and  c  are vectors                  .
c .                                                                   .
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c
      common/scom/nload,iloads,mvlast
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
      aa(i)=0.0d0
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
      kup=min(ku,kl+ii-mhold-1)
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
      kup=min(ku,kl+nvect-1)
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
      e=0.0d0
      ks=ii-nhold
      if(ks.le.1)go to 160
c
c        do current ca block
c
      kup=min(ku,kl+ii-nhold-2)
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
      kup=min(ku,kl+nvect-1)
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
*deck nodep
      subroutine nodep
      implicit real*8 (a-h,o-z)
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/adr/n1,n2,n3,n4,n5,n6a,n7a,n8a
      common/dtwo/itwo,ndof,idim
      common/list/data(64),ndum
      common a(1)
      real a
      numnp=data(2)
      ndof=data(3)
      idim=data(4)
      if(numnp.gt.0)go to 10
      write(6,100)numnp
      stop
   10 continue
      n1=1
      n2=n1
      n3=n2+numnp*itwo
      n4=n3+numnp*itwo
      n5=n4+numnp*itwo
      n6=n5+numnp+1
      n7=n6+ndof*numnp+1
      n2l=nmax-ndof*numnp+1
      min=nmax+n7-n2l+1
      write(6,90)min
c
c        check storage for nodal point input
c
      if(min.le.nmax)go to 20
      write(6,110)n5,numnp
      stop
   20 continue
      call nodgen(a(n2l),a(n2),a(n3),a(n4),a(n5),a(n6),
     .  numnp,neq,ndof,idim)
      if(idim.lt.1)n3=n2
      if(idim.lt.2)n4=n3
      if(idim.lt.3)n5=n4
      return
   90 format(1h0,10x,39hstorage required to input nodal data = ,i10)
  100 format(1h0,5x,24herror number of nodes = ,i10)
  110 format(1h0,5x,24herror space requested = ,i10,3x,
     .  18hspace available = ,i10)
      end
*deck nodgen
      subroutine nodgen(id,x,y,z,nodes,id2,numnp,neq,ndof,idim)
      implicit real*8 (a-h,o-z)
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat,kpplot
      common/list/data(64),ndum
      character*8 cdata
      common/clist/cdata(64)
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
      common/tapcom/nt11,nt31
      dimension x(1),y(1),z(1),id(ndof,1),nodes(2),id2(ndof,1)
      character*1 iprc,it
      character*8 end,free
      dimension iprc(4)
c
      data iprc /' ','a','b','c' /
      data end,free/'end','free'/
c
c
c
      nold=0
      dum=0.
      rad=datan(1.d0)/45.0d0
      knold=0
      feet=1.0
      it=iprc(1)
   10 continue
      call fld
      if (cdata(1).ne.'feet') go to 13
      feet=12.0d0
      if (data(2).ne.0.0) feet=data(2)
      go to 10
   13 continue
      if(cdata(1).ne.'cart')go to 11
      it=iprc(1)
      go to 10
   11 if(cdata(1).ne.'pola') go to 12
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
      z(n)=y(n)*dsin(dum)
      y(n)=y(n)*dcos(dum)
   20 continue
      if (knold.eq.0) go to 50
      num=(n-nold) / knold
      numn=num-1
      if(numn.lt.1) go to 50
      xnum=num
      dx=(x(n)-x(nold))/xnum
      if (it.eq.iprc(1)) go to 21
      rold=y(nold)/dcos(dumold)
      rnew=y(n)/dcos(dum)
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
      y(k)=rold*dcos(dumold)
      z(k)=rold*dsin(dumold)
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
      id(j,n)=0
   70 continue
   75 continue
      call fld
      if(cdata(1).eq.end)go to 90
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
      ip=iand(ishft(ipoint,1-j),1)
      if(ip.eq.0)go to 85
      id(j,i)=1
      if(cdata(1).eq.free)id(j,i)=0
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
      nr=40
      ier=copen(nr,"Minimize.dat;")
      if(ier.ne.0)go to 190
      ier=cread(nr,nodes,1,0)
      if(ier.ne.0)go to 190
      if(nodes(1).ne.numnp)goto 190
      ier=cread(nr,nodes(2),numnp,1)
      if(ier.ne.0)goto 190
      do 125 n=1,numnp
      do 122 i=1,ndof
      id2(i,n)=id(i,n)
  122 continue
  125 continue
      nq=1
      do 135 n=1,numnp
      nn=nodes(1+n)
      do 130 i=1,ndof
      if(id2(i,nn).ne.0)then
         id(i,nn)=nq;
         nq=nq+1
      else
         id(i,nn)=0;
      endif
  130 continue
  135 continue
  190 continue;
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
*deck mopen
      subroutine mopen(istoh,nblock,icheck)
      implicit real*8 (a-h,o-z)
      integer ccreate
      external ccreate
      data nr/10/
      if(nr.eq.1)return
      ier=0
      nr=31
c      call assign(ier,'dn'l,'ft31'l,'rdm'l,'u'l)
      ier=ccreate(nr,"tape.31;")
      if(ier.ne.0)go to 10
      nr=32
c      call assign(ier,'dn'l,'ft32'l,'rdm'l,'u'l)
      ier=ccreate(nr,"tape.32;")
      if(ier.ne.0)go to 10
      nr=33
c      call assign(ier,'dn'l,'ft33'l,'rdm'l,'u'l)
      ier=ccreate(nr,"tape.33;")
      if(ier.ne.0)go to 10
      nr=34
c      call assign(ier,'dn'l,'ft34'l,'rdm'l,'u'l)
      ier=ccreate(nr,"tape.34;")
      if(ier.ne.0)go to 10
      nr=36
c      call assign(ier,'dn'l,'ft36'l,'rdm'l,'u'l)
      ier=ccreate(nr,"tape.36;")
      if(ier.ne.0)go to 10
      nr=37
c      call assign(ier,'dn'l,'ft37'l,'rdm'l,'u'l)
      ier=ccreate(nr,"tape.37;")
      if(ier.ne.0)go to 10
      nr=38
c      call assign(ier,'dn'l,'ft38'l,'rdm'l,'u'l)
      ier=ccreate(nr,"tape.38;")
      if(ier.ne.0)go to 10
      nr=39
      ier=ccreate(nr,"tape.39;")
      if(ier.ne.0)go to 10
      nr=1
      return
   10 continue
      write(6,1000)nr
      stop
 1000 format(' open error file = ',i5)
      end
*deck rcon
      subroutine rcon(diag,fc,a,cc,ss,s,lm,maxa,nd,nk,ifg)
      implicit real*8 (a-h,o-z)
      common/comtyp/itype,ngroup,ngin,nlin
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common /addb/ neql,neqr,mla,nblock
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/ewcom1/il(128)
      dimension diag(3,nd,1),fc(1),a(1)
      dimension cc(nk,1),s(1),ss(nd,nd)
      dimension lm(nd,1),maxa(1)
      do 100 ne=1,nume
      if(ind.eq.5)go to 20
      call echeck(lm(1,ne),nd,icode)
      if(icode.eq.1)go to 100
      ngin=1
      if(ind.eq.4)return
      if(ifg.gt.0)go to 20
c
c        store diagonals
c
      nds=nd
      do 10 i=1,nd
      ii=lm(i,ne)
      if(ii.lt.neql.or.ii.gt.neqr)go to 10
      iim=ii-neql+1
      mi=maxa(iim)-mla
      diag(kpri,i,ne)=a(mi)
   10 continue
      go to 100
   20 continue
c
c        find largest diagonal
c
      ni=1
      d1=diag(1,1,ne)
      smax=d1**2
      if(nd.le.1)go to 35
      do 30 i=2,nd
      d1=diag(1,i,ne)
      snew=d1**2
      if(snew.le.smax)go to 30
      smax=snew
      ni=i
   30 continue
   35 continue
c
c        calculate constraints
c
      do 50 i=1,nd
      ccs=cc(i,ne)
      do 40 j=1,nd
      ss(i,j)=ccs*cc(j,ne)
   40 continue
   50 continue
      den=ss(ni,ni)**2
      if(den.ne.0.)go to 60
      write(6,1000)ni
      stop
   60 continue
c
c        add constraints in stiffness matrix
c
      fac=10.d5*dsqrt(smax)/den
      k=0
      do 80 i=1,nd
      do 70 j=i,nd
      k=k+1
      s(k)=ss(i,j)*fac
   70 continue
   80 continue
      if(ind.eq.5)go to 85
      call addban(a,maxa,s,lm(1,ne),nd)
      go to 100
   85 continue
c
c        calculate constraint load vector
c
      ccs=cc(nk,ne)*fac
      do 90 i=1,nd
      fc(i)=ccs*cc(i,ne)
   90 continue
      call addma(a,fc,lm(1,ne),nd)
  100 continue
      return
 1000 format(1h0,10x,36hstop zero diagonal in constraint no.,i10)
      end
*deck sblock
      subroutine sblock (maxa,ncolbv,icopl,istorl,nblock,neq,nwk,istoh)
      implicit real*8 (a-h,o-z)
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
      subroutine solve(iequit,iep,istres,iback)
      implicit real*8 (a-h,o-z)
      common/dyncm1/time,dt,dtold,delta,alpha
      common/dyncm2/idym,imass,idamp,ires,intial,ncycle,nstep,nbdym
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat,kpplot
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
      character*8 file1
      common/t51com/file1
      dimension ia(1)
      common a(1)
      real a
      integer ia
      equivalence (a(1),ia(1))
c
c        setup solution storage
c
      n3=n2+nvect*itwo
      n4=n3+nvect*itwo
      if(neq.eq.nvect)n4=n3
      n5=n4+istoh*itwo
      n6=n5+istoh*itwo
      if(nblock.eq.1.and.nbdym.eq.0)n6=n5
      n7=n6+nvect*itwo
      n8=n7+nvect*itwo
      if(neq.eq.nvect)n8=n7
      n6e=n8
      n15=n6e+numest+512
      n2i=n2+neq*itwo
      n3a=n2i+neq*itwo
      n41=n3a+nvect*itwo
      n42=n3a+numest+512
      n43=n3a+ndof*numnp
      n15=max(n15,n41,n42,n43)
      min=nmax+n15-n1l+1
      if(iback.eq.0)write(6,2110)min
      if(min.le.nmax)go to 40
      stop
   40 continue
      ind=2
c
c
      if(icheck.eq.1)go to 70
      if(iback.eq.1)go to 60
      call assem1(a(n4),a(n5),a(n6),a(n6e),a(n1l),a(n3l),a(n5l),
     .   istoh,maxbh,munit,1)
      call colsol(a(n1l),a(n2l),a(n3l),a(n4l),a(n2),a(n3),a(n4),a(n5),
     .            a(n6),a(n7),nblock,neq,istoh,nvect,nvect,maxbh,nstif,
     .            nred,lunit,munit,nunit,1)
   60 continue
      n6e=n3a
      call load(a(n3a),a(n3a),a(n3a),a(n2),ndof,nvect,2)
      call loadef(a(n2),a(n3),a(n4),a(n6),a(n7),a(n1l),a(n3l))
      call colsol(a(n1l),a(n2l),a(n3l),a(n4l),a(n2),a(n3),a(n4),a(n5),
     .            a(n6),a(n7),nblock,neq,istoh,nvect,nvect,maxbh,nstif,
     .            nred,lunit,munit,nunit,2)
      call newdav(a(n2),a(n6))
      call dwrite(a(n2),a(n3a),a(n3a),ndof,nvect)
   70 continue
      if(icheck.eq.1)return
      n6e=n3a
      n3s=n3
      n3=n2i
      if(istres.eq.0)go to 80
      call esend()
      call mfopen3(ncycle)
      if(kpplot.ne.0)then
      call eshead(file1,8);
      call estime(time)
      call dplot(a(n2),a(n3a),a(n3a),ndof,nvect)
      endif
      call stress(a(n2),a(n3),a(n6e),nvect,lunit)
      if(kpplot.ne.0)call esend()
   80 continue
      n3=n3s
      if(iequit.ne.0)call equit(a(n2),a(n3),a(n4),a(n6),a(n7),a(n1l),
     .                         a(n3l),a(n3a),neq,istoh,nvect,
     .                         munit,nstif,lunit,nunit,iep)
      return
 2110 format(1h0,10x,40h storage requested for solution phase =  ,i10)
      end
*deck mfopen3
      subroutine mfopen3(npic)
      character*8 file1
      common/t51com/file1
      if(npic.gt.9)go to 10
      write(file1,1000)npic
      go to 100
   10 if(npic.gt.99)go to 20
      write(file1,1010)npic
      go to 100
   20 if(npic.gt.999)go to 30
      write(file1,1020)npic
      go to 100
   30 if(npic.gt.9999)go to 40
      write(file1,1030)npic
      go to 100
   40 continue
      write(file1,1050)npic
      go to 100
  100 continue
      return
 1000 format('t0000',i1)
 1010 format('t000',i2)
 1020 format('t00',i3)
 1030 format('t0',i4)
 1050 format('t',i5)
      end
*deck stress
      subroutine stress(ur,ui,fr,nvect,nunit)
      implicit real*8 (a-h,o-z)
c
      common /flgs/ kpri,ind,nummat,midest,ng,icheck
      common /number/ nmax,numnp,nume,neq,nwk,ma
      common/scom/nload,iloads,mvlast
      common /tapcom/ nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlin
c
      dimension ur(1),ui(1),fr(1)
c
      if(icheck.eq.1)return
      nloc=0
      do 20 i=1,nload
      call recs(nunit,fr,nvect,i)
      if(i.eq.nload)mv=mvlast
      do 10 j=1,mv
      nloc=nloc+1
      ur(nloc)=fr(j)
      ui(nloc)=0.d0
10    continue
20    continue

      ind=3
      do 30 i=1,ng
      call eread(fr,i)
      call gstiff
   30 continue
      return
      end
*deck wecs
      subroutine wecs(nt,a,in,nb)
      implicit real*8 (a-h,o-z)
      common/stapes/nts1,ntb1,nts2,ntb2,nts3,ntb3
      common/dtwo/itwo,ndof,idim
      dimension a(1)
      real a
      logical ifg
      data nt32,nt33,nt34,nt35,nt36/32,33,34,35,36/
      data nt37,nt38,nt39/37,38,39/
      iw=in*itwo
      ifg=.true.
      go to (10,30,50,70,90,110,130,140),nt
   10 continue
      nbb=nb+(nts1-1)*ntb1
      if(ifg)call writms(nt32,a,iw,nbb,-1,0)
      if(.not.ifg)call readms(nt32,a,iw,nbb)
      return
   30 continue
      if(ifg)call writms(nt33,a,in,nb,-1,0)
      if(.not.ifg)call readms(nt33,a,in,nb)
      return
   50 continue
      nbb=nb+(nts3-1)*ntb3
      if(ifg)call writms(nt34,a,iw,nbb,-1,0)
      if(.not.ifg)call readms(nt34,a,iw,nbb)
      return
   70 continue
      if(nb.eq.1)rewind nt35
      if(ifg)write(nt35)iw,(a(i),i=1,iw)
      if(.not.ifg)read(nt35)iw,(a(i),i=1,iw)
      return
   90 continue
      if(ifg)call writms(nt36,a,iw,nb,-1,0)
      if(.not.ifg)call readms(nt36,a,iw,nb)
      return
  110 continue
      if(ifg)call writms(nt37,a,iw,nb,-1,0)
      if(.not.ifg)call readms(nt37,a,iw,nb)
      return
  130 continue
      nbb=nb+(nts2-1)*ntb2
      if(ifg)call writms(nt38,a,iw,nbb,-1,0)
      if(.not.ifg)call readms(nt38,a,iw,nbb)
      return
  140 continue
      if(ifg)call writms(nt39,a,iw,nb,-1,0)
      if(.not.ifg)call readms(nt39,a,iw,nb)
      return
 
      entry recs(nt,a,in,nb)
 
      iw=in*itwo
      ifg=.false.
      go to (10,30,50,70,90,110,130,140),nt
      end
*deck writed
      subroutine writed(ur,fr,id,ndof,nvect,nunit,ifg)
      implicit real*8 (a-h,o-z)
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat,kpplot
      common/scom/nload,iloads,mvlast
      dimension fr(1),id(ndof,1)
      dimension dxr(6)
      dimension ur(1)
      dimension drc(10)
c
c        print solution
c
      nloc=0
      nlast=0
      kount=0
      kend=(10/ndof)*ndof
      do 20 i=1,nload
      call recs(nunit,fr,nvect,i)
      if(i.eq.nload)mv=mvlast
      do 10 j=1,mv
      nloc=nloc+1
      ur(nloc)=fr(j)
   10 continue
   20 continue
      rewind 8
      read(8)((id(i,j),i=1,ndof),j=1,numnp)
      if(ifg.le.0)write(6,1000)
      if(ifg.eq.1)write(6,1030)
      if(ifg.eq.2)write(6,1040)
      do 40 n=1,numnp
      do 30 i=1,ndof
      ip=id(i,n)
      dxr(i)=0.
      if(ip.le.0)go to 30
      dxr(i)=ur(ip)
   30 continue
      if(kpcomp.eq.1)go to 34
      write(6,1010)n,(dxr(i),i=1,ndof)
      go to 40
   34 continue
      do 38 i=1,ndof
      kount=kount+1
      drc(kount)=dxr(i)
   38 continue
      if(kount.lt.kend.and.n.lt.numnp)go to 40
      write(6,1050)n,(drc(i),i=1,kount)
      kount=0
   40 continue
      return
 1000 format(1h0,10x,49hn o d a l   p o i n t   d i s p l a c e m e n t
     .s ///)
 1010 format(i8,5x,1p6e12.4)
 1030 format(1h0,10x,63hc a l c u l a t e d  n o d a l   p o i n t  f o
     .r c e s         ,f14.4,///)
 1040 format(1h0,10x,59hr e s i d u a l  n o d a l  p o i n t  f o r c e
     . s         ,f14.4,///)
 1050 format(i8,1p10e11.3)
      end
*deck hsiow
      subroutine hsiow(iunit,a,iword,npos)
      implicit real*8 (a-h,o-z)
      integer cwrite
      external cwrite
      dimension a(1)
      npru=(iword-1)/512+1
      nword=npru*512
      ierr=cwrite(iunit,a,nword,npos)
      if(ierr.gt.0)go to 20
      return
   20 continue
      write(6,100)iunit
      write(6,200)npru,iword,nword,npos
      stop
  100 format(10x,13hstop ntape = ,i5)
  200 format(10x,7hnpru = ,i10,2x,8hiword = ,i10,2x,8hnword = ,i10,2x,
     .           7hnpos = ,i10)
      end
*deck hsior
      subroutine hsior(iunit,a,iword,npos)
      implicit real*8 (a-h,o-z)
      integer cread
      external cread
      real abuf(512)
      real a(1)
      npruw=(iword-1)/512+1
      nword=npruw*512
      nprur=iword/512
      iwords=nprur*512
      do 10 i=1,512
      abuf(i)=a(iwords+i)
   10 continue
      ierr=cread(iunit,a,nword,npos)
      if(ierr.gt.0)go to 40
   20 continue
      if(npruw.le.nprur)return
      nlm=iword-iwords+1
c      do 30 i=nlm,512
c      a(iwords+i)=abuf(i)
c   30 continue
      return
   40 continue
      write(6,100)iunit
      write(6,200)npruw,nword,npos,nprur,iwords,iword
      stop
  100 format(10x,13hstop ntape = ,i5)
  200 format(10x,8hnpruw = ,i10,2x,8hnword = ,i10,2x,7hnpos = ,i10,2x,
     .           8hnprur = ,i10,2x,9hiwords = ,i10,2x,8hiword = ,i10)
      end
*deck writms
      subroutine writms(iunit,a,iword,irec,idum1,idum2)
      implicit real*8 (a-h,o-z)
      integer cwrite
      external cwrite
      real a(1)
      npru=(iword-1)/512+1
      nword=npru*512
      npos=nword*(irec-1)
      ierr=cwrite(iunit,a,nword,npos)
      if(ierr.gt.0)go to 20
   10 continue
      return
   20 continue
      write(6,100)iunit,irec
      write(6,200)npru,iword,nword,npos
      stop
  100 format(10x,13hstop ntape = ,i5,5x,7hirec = ,i5)
  200 format(10x,7hnpru = ,i10,2x,8hiword = ,i10,2x,8hnword = ,i10,2x,
     .           7hnpos = ,i10)
      end
*deck readms
      subroutine readms(iunit,a,iword,irec)
      implicit real*8 (a-h,o-z)
      integer cread
      external cread
      real a(1)
      real abuf(512)
      npruw=(iword-1)/512+1
      nword=npruw*512
      npos=nword*(irec-1)
      nprur=iword/512
      iwords=nprur*512
      do 10 i=1,512
      abuf(i)=a(iwords+i)
   10 continue
      ierr=cread(iunit,a,nword,npos)
      if(ierr.gt.0)go to 40
   20 continue
      if(npruw.le.nprur)return
      nlm=iword-iwords+1
      do 30 i=nlm,512
      a(iwords+i)=abuf(i)
   30 continue
      return
   40 continue
      write(6,100)iunit
      write(6,200)npruw,nword,npos,nprur,iwords,iword
      stop
  100 format(10x,13hstop ntape = ,i5)
  200 format(10x,8hnpruw = ,i10,2x,8hnword = ,i10,2x,7hnpos = ,i10,2x,
     .           8hnprur = ,i10,2x,9hiwords = ,i10,2x,8hiword = ,i10)
      end
*deck mode
      subroutine mode
      implicit real*8 (a-h,o-z)
      common/dyncm2/idym,imass,idamp,ires,intial,ncycle,nstep,nbdym
      common/stapes/nts1,ntb1,nts2,ntb2,nts3,ntb3
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/scom/nload,iloads,mvlast
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/tcom/munit,lunit,nstif,nred,nunit
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/ncom/numest,nvect,istoh,maxbh
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common /addb/ neql,neqr,mla,nblock
      common/freqif/istohf,n1a,n1b,n1c
      common/dtwo/itwo,ndof,idim
      common/list/data(64),ndum
      common/tapcom/nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlin
      dimension ia(1)
      common a(1)
      real a
      integer ia
      equivalence (a(1),ia(1))
      id1=data(2)
      id2=data(3)
      id3=data(4)
      id4=data(5)
      data(2)=2.
      data(3)=neq
      data(4)=2
      data(5)=0
      call second2(t1)
      call assem
      call second2(t2)
      t=t2-t1
      write(6,1000)t
      data(2)=id1
      data(3)=id2
      data(4)=id3
      data(5)=id4
      idim=2
      istohf=istoh
      rewind 35
      read(35)(ia(i),i=n1,n1c)
      n2=n1c
      nts1=2
      do 10 i=1,nblock
      call recs(1,a(n2),istoh,i)
      call wecs(8,a(n2),istoh,i)
   10 continue
      nts1=1
      call freqs
      return
 1000 format(1h0,10x,'time required to form stiffness =',f13.3,
     .       ' seconds')
      end
*deck freqs
      subroutine freqs
      implicit real*8 (a-h,o-z)
c
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c .                                                                   .
c .   p r o g r a m                                                   .
c .      to find the lowest frequencies and associated                .
c .      mode shapes of linearized structure                          .
c .                                                                   .
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c
      common /number/ nmax, numnp, nume, neq, nwk, ma
      common /adr/ n1, n2, n3, n4, n5, n6, n7, n8
      common /dtwo/ itwo, ndof, imass
      common /list/ data(64),ndum
      common /flgs/ kpri, ind, nummat, midest, ng, icheck
      common /tapeig/ scale, nstif, nt, nmass, nred, nshift
      common /freqif/ istoh, n1a, n1b, n1c
      common /addb/ neql, neqr, mla, nblock
      common a(1)
      real a
c
c     input frequency control data
c
      nfreq=data(2)
      cofq=data(3)
      ifpr=data(4)
      nmode=data(5)
      irbm=data(6)
      rbmsh=data(7)
      if (nfreq.eq.0) nfreq=1
      if (cofq.eq.0) cofq=1.d+08
      cofq2=cofq*cofq
      write (6,40) nfreq,cofq,ifpr,nmode
c
      nstif=1
      nshift=3
      nred=5
      nmass=8
      nt=9
      nc=nfreq+3
c
c     calculate new array addresses
c
      m3=n2+istoh*itwo
      m4=m3+istoh*itwo
      if (imass.eq.1.and.nblock.eq.1) m4=m3
      if (icheck.ne.0) go to 10
c
c     find pseudonorm of stiffness matrix
c
      call pnorm(a(n1),a(n1a),a(n2),a(m3),a(m4),irbm,rbmsh,neq,istoh,
     .           nblock,nstif,nmass,imass,anorm)
   10 m5=m4+neq*itwo
      if (imass.eq.2) m5=m4
      m6=m5+neq*itwo
      m7=m6+neq*itwo
      m8=m7+neq*itwo
      m9=m8+6*neq*itwo
      m10=m9+nc*itwo
      m11=m10+nc*itwo
      m12=m11+nc*itwo
      m13=m12+nc*itwo
      m14=m13+nc+1
      write (6,50) m14
      if (m14.le.nmax) go to 20
      write (6,30)
   20 continue
      if (icheck.ne.0) return
c
      call secant (a(n1),a(n1a),a(n1b),a(n2),a(m3),a(m4),a(m5),a(m6),a
     1 (m7),a(m8),a(m9),a(m10),a(m11),a(m12),a(m13),neq,istoh,nfreq,nc
     2 ,nblock,imass,ifpr,anorm,cofq2)
c
c     print the frequencies and mode shapes
c
      call wrfreq (a(n2),nfreq,rbmsh)
c
      m3=n2+nfreq*itwo
      m4=m3+neq*itwo
      m5=m4+ndof*numnp
      nn=m5-1
      rewind 8
      read (8) (a(i),i=m4,nn)
c
      call wrmod (a(n2),a(m3),a(m4),numnp,ndof,neq,nfreq,nmode)
c
      return
c
c
   30 format (50h blank common too small for eigenvalue,s needs    )
   40 format (1h1,39h f r e q u e n c i e s  s o l u t i o n,//3x,23h c 00
     1o n t r o l  d a t a,//6x,54h number of frequencies to be determin
     2ed      (nfreq) =,i5/6x,54h cutoff frequency                      00
     3      (cofq) =  ,e10.4/6x,54h print option                        00
     4      (ifpr) =    ,i5/6x,40h      eq.0, no printing               00                 
     5   ,/,6x,40h      eq.1, print                       /6x,54h number 
     6 of mode shapes to be printed         (nmode) =,i5///)
   50 format (1h0,10x,43hstorage required for eigenvalue solution = ,i10
     1 )
      end
*deck secant
      subroutine secant (maxa,ncolbv,icopl,a,b,xm,d,v,w,ww,root,tim
     1 ,errvr,errvl,nite,n,istoh,nroot,nc,nblock,imass,ifpr,anorm,cofq)
      implicit real*8 (a-h,o-z)
c
c     program to calculate smallest eigenvalues and corresponding
c     eigenvectors of the problem
c                           a* v = lambda* b* v
c
      dimension a(istoh), b(istoh), xm(1), v(1), d(1), w(1), ww(n,1),
     1 root(1), tim(1), errvl(1), errvr(1)
      integer nite(1), maxa(1), ncolbv(1), icopl(1)
      common /tapeig/ scale, nstif, nt, nmass, nred, nshift
      common /shift/ rr, ra, rb, rc, fdetr, fdeta, fdetb, fdetc, ffr,
     1 ffa, ffb, ffc, idetr, ideta, idetb, idetc, ifr, ifa, ifb, ifc
c
c     following tolerances are set for 13 (or more) digit arithmetic
      actol=0.0001d0
      rcbtol=0.00001d0
      ritol=0.000001d0
      rqtol=0.000000000001d0
c
c     the following are iteration number tolerances
c
      ntf=5
      iitem=10
      nitem=39
      nitemm=59
      nvm=6
      eta=2.0d0
      nov=0
      jr=1
      nsk=0
      nstore=nred
      rewind nt
c
c     find locations for negative elements in starting
c     iteration vectors
c
      nc1=nc+1
      if (imass.eq.2) go to 10
      call recs (nmass,xm,n,1)
   10 neql=1
      neqr=0
      mla=0
      do 50 nj=1,nblock
      ncolb=ncolbv(nj)
      neqr=neqr+ncolb
      call recs (nstif,a,istoh,nj)
      if (imass.eq.2) call recs (nmass,b,istoh,nj)
      do 40 i=neql,neqr
      ii=maxa(i)-mla
      aa=a(ii)
      if (aa.gt.0.) go to 20
      write (6,860) i,aa
      stop
   20 if (imass.eq.2) go to 30
      v(i)=xm(i)/aa
      go to 40
   30 v(i)=b(ii)/aa
   40 continue
      if (nj.eq.nblock) go to 50
      neql=neql+ncolb
      mla=maxa(neql)-1
   50 continue
      do 70 j=3,nc1
      imax=0
      rmax=0.
      do 60 i=1,n
      if (v(i).lt.rmax) go to 60
      rmax=v(i)
      imax=i
   60 continue
      nite(j)=imax
   70 v(imax)=0.
c
c     check for single degree-of-freedom system
c
      if (n.gt.1) go to 90
      if (imass.eq.1) b(1)=xm(1)
      if (b(1).gt.0) go to 80
      write (6,1100)
      stop
   80 root(1)=a(1)/b(1)
      nsch=1
      a(1)=1./dsqrt(b(1))
      write (nt) a(1)
      errvl(1)=0.
      go to 840
c
   90 call second2 (tim1)
      ra=0.0d0
      rr=0.0d0
      call bandet (a,b,xm,v,d,maxa,ncolbv,icopl,n,istoh,nblock,ra,nsch
     1 ,imass,fdeta,ideta,1)
      ffa=fdeta
      ifa=ideta
      ffr=ffa
      ifr=ifa
      fdetr=fdeta
      idetr=ideta
c
c   check for zero eigenvalue(s)
c
      n1=maxa(n)-mla
      if (a(n1).gt.anorm) go to 100
      write (6,900)
      stop
c
c   find lower bound on smallest eigenvalue
c
  100 if (ifpr.eq.1) write (6,910)
      if (imass-1) 130,130,110
  110 do 120 i=1,n
  120 v(i)=1.
      call mltply (w,b,v,maxa,n,ncolbv,istoh,nblock,nmass)
      go to 150
  130 do 140 i=1,n
  140 w(i)=xm(i)
  150 rt=0.0d0
      iite=0
  160 iite=iite+1
      do 170 i=1,n
  170 v(i)=w(i)
      call bandet (a,b,xm,v,d,maxa,ncolbv,icopl,n,istoh,nblock,ra,nsch
     1 ,imass,fdeta,ideta,2)
      rqt=0.0
      do 180 i=1,n
  180 rqt=rqt+w(i)*v(i)
      if (imass-1) 200,200,190
  190 call mltply (w,b,v,maxa,n,ncolbv,istoh,nblock,nmass)
      go to 220
  200 do 210 i=1,n
  210 w(i)=xm(i)*v(i)
  220 rqb=0.0
      do 230 i=1,n
  230 rqb=rqb+w(i)*v(i)
      rq=rqt/rqb
      if (ifpr.eq.1) write (6,870) rq
      bs=dsqrt(rqb)
      tol=abs(rq-rt)/rq
      if (tol.lt.rcbtol) go to 250
      do 240 i=1,n
  240 w(i)=w(i)/bs
      rt=rq
      if (iite.lt.iitem) go to 160
c
  250 temp=100.d0*tol
      if (temp.gt.0.1) temp=0.1
      rb=rq*(1.0d0-temp)
      is=0
  260 call bandet (a,b,xm,v,d,maxa,ncolbv,icopl,n,istoh,nblock,rb,nsch
     1 ,imass,fdetb,idetb,1)
      if (ifpr.eq.1) write (6,940) rb,nsch
      ffb=fdetb
      ifb=idetb
      if (nsch.eq.0) go to 280
      is=is+1
      if (is.le.ntf) go to 270
      write (6,950) ntf
      stop
  270 rb=rb/(nsch+1)
      go to 260
c
c     i t e r a t i o n  f o r  i n d i v i d u a l  e i g e n p a i r s
c
  280 if (ifpr.eq.1) write (6,960)
      nite(jr)=-1
      if (ifpr.eq.1) write (6,970) jr,nite(jr),ra,fdeta,ffa,eta,ideta
     1 ,ifa
      nite(jr)=0
      if (ifpr.eq.1) write (6,970) jr,nite(jr),rb,fdetb,ffb,eta,idetb
     1 ,ifb
c
c   we stop when we have the required number of roots smaller than rc an
c   nov=0
c
  290 if (nsch.ge.nroot) go to 730
      if (rb.gt.cofq) go to 730
c
      i=ifa-ifb
      ffa=ffa*2.0d0**i
      ifa=ifb
      dif=ffb-ffa
      if (dif.ne.0.0) go to 300
      write (6,980)
      go to 730
  300 del=ffb*(rb-ra)/dif
      rc=rb-eta*del
      if (rc.gt.0.) go to 310
      write (6,990) rc
      stop
  310 tol=rcbtol*rc
      if (abs(rc-rb).gt.tol) go to 320
      if (ifpr.eq.1) write (6,1000)
      root(jr)=rb
      go to 380
c
  320 call bandet (a,b,xm,v,d,maxa,ncolbv,icopl,n,istoh,nblock,rc,nsch
     1 ,imass,fdetc,idetc,1)
      ffc=fdetc
      ifc=idetc
      nite(jr)=nite(jr)+1
      if (jr.eq.1) go to 340
      jj=jr-1
      do 330 k=1,jj
      ffc=ffc/(rc-root(k))
  330 call rscale (ffc,ifc)
  340 if (ifpr.eq.1) write (6,970) jr,nite(jr),rc,fdetc,ffc,eta,idetc
     1 ,ifc
c
c   if we have more signchanges than eigenvalues smaller than rc we
c   start inverse iteration
c
      nes=0
      if (jr.eq.1) go to 360
      do 350 i=1,jj
  350 if (root(i).lt.rc) nes=nes+1
  360 nov=nsch-nes
      if (nov.eq.0) go to 370
      if (ifpr.eq.1) write (6,1010) nov
      root(jr)=rc
      if (nov.gt.1) nsk=1
c
      go to 380
  370 rr=ra
      ffr=ffa
      ifr=ifa
      fdetr=fdeta
      idetr=ideta
      ra=rb
      ffa=ffb
      ifa=ifb
      fdeta=fdetb
      ideta=idetb
      rb=rc
      ffb=ffc
      ifb=ifc
      fdetb=fdetc
      idetb=idetc
c
c     we reset eta if we can accelerate the secant iteration still more
c
      tol=rb*actol
      if (abs(ra-rb).lt.tol) eta=eta*2
      if (nite(jr).le.nitem) go to 290
      write (6,930) jr,nite(jr)
      go to 730
c
c   check for storage
c
  380 if (jr.le.nc) go to 390
      write (6,1020)
      go to 730
c
c     initialize starting inverse iteration vector
c
  390 nor=jr-1
      if (nor.gt.nvm) nor=nvm
      call second2 (tim3)
      if (ifpr.eq.1) write (6,1030) nor
      if (jr.eq.1) go to 410
      do 400 i=1,n
  400 v(i)=1.0
      i=nite(jr+1)
      v(i)=-1.0
  410 if (imass-1) 430,430,420
  420 call mltply (w,b,v,maxa,n,ncolbv,istoh,nblock,nmass)
      go to 450
  430 do 440 i=1,n
  440 w(i)=xm(i)*v(i)
  450 is=0
      go to 540
c
c     inverse iteration
c
  460 nite(jr)=nite(jr)+1
      do 470 i=1,n
  470 v(i)=w(i)
      call bandet (a,b,xm,v,d,maxa,ncolbv,icopl,n,istoh,nblock,rc,nsch
     1 ,imass,fdetc,idetc,2)
      if (is.eq.1) go to 620
      rqt=0.0
      do 480 i=1,n
  480 rqt=rqt+w(i)*v(i)
      if (imass-1) 500,500,490
  490 call mltply (w,b,v,maxa,n,ncolbv,istoh,nblock,nmass)
      go to 520
  500 do 510 i=1,n
  510 w(i)=xm(i)*v(i)
  520 rqb=0.0
      do 530 i=1,n
  530 rqb=rqb+w(i)*v(i)
      rq=rqt/rqb
      rt=root(jr)+rq
      if (ifpr.eq.1) write (6,1040) jr,nite(jr),rt,rq
      tol=rt*rqtol
      eigdif=abs(rt-rta)
      if (eigdif.gt.tol) go to 540
      is=1
      go to 460
c
  540 rta=rt
      bs=dsqrt(rqb)
      if (nor.eq.0) go to 580
      do 570 k=1,nor
      al=0.0
      do 550 i=1,n
  550 al=al+ww(i,k)*v(i)
      do 560 i=1,n
  560 w(i)=w(i)-al*ww(i,k)
  570 continue
  580 do 590 i=1,n
  590 w(i)=w(i)/bs
c
c     perform rayleigh quotient shift if convergence is slow
c
      if (nite(jr).le.nitem) go to 460
      if (nite(jr).gt.(nitem+1)) go to 610
      tol=rt*ritol
      if (eigdif.lt.tol) go to 600
      write (6,930) jr,nitem
      go to 730
  600 if (ifpr.eq.1) write (6,920) jr
      nred=nshift
      call bandet (a,b,xm,v,d,maxa,ncolbv,icopl,n,istoh,nblock,rt,nscht
     1 ,imass,fdett,idett,1)
      root(jr)=rt
  610 if (nite(jr).le.nitemm) go to 460
      write (6,930) jr,nite(jr)
      go to 730
c
  620 rqt=0.0
      errt=rqb
      do 630 i=1,n
  630 rqt=rqt+v(i)*w(i)
      if (imass-1) 650,650,640
  640 call mltply (w,b,v,maxa,n,ncolbv,istoh,nblock,nmass)
      go to 670
  650 do 660 i=1,n
  660 w(i)=xm(i)*v(i)
  670 rqb=0.0
      do 680 i=1,n
  680 rqb=rqb+v(i)*w(i)
c
c     obtain error bounds -
c     the bounds depend on the distance from the root and can be large
c
      rq=rqt/rqb
      root(jr)=root(jr)+rq
      err=dsqrt(errt/rqb)
      errvl(jr)=root(jr)-err
      errvr(jr)=root(jr)+err
c
      bs=dsqrt(rqb)
      do 690 i=1,n
      w(i)=w(i)/bs
  690 v(i)=v(i)/bs
      write (nt) (v(i),i=1,n)
      jj=jr
      if (jj.le.nvm) go to 710
      do 700 k=1,n
      do 700 l=2,nvm
  700 ww(k,l-1)=ww(k,l)
      jj=nvm
  710 do 720 k=1,n
  720 ww(k,jj)=w(k)
c
      call second2 (tim2)
      tim3=tim2-tim3
      if (ifpr.eq.1) write (6,1050) tim3
      tim(jr)=tim2-tim1
      tim1=tim2
      nred=nstore
c
c     decide strategy for iteration towards next root
c
      call strat (a,b,xm,v,d,maxa,ncolbv,icopl,root,nite,n,istoh,nblock
     1 ,jr,nov,nsk,nsch,eta,imass)
c
      if (nov.gt.0) go to 380
      go to 280
c
  730 nroot=jr-1
      if (nroot.gt.0) go to 740
      write (6,1100)
      stop
  740 if (ifpr.eq.0) go to 750
      write (6,1060)
      write (6,880) (nite(j),j=1,nroot)
      write (6,1070)
      write (6,890) (tim(j),j=1,nroot)
      write (6,1080)
      write (6,870) (errvl(j),j=1,nroot)
      write (6,870) (errvr(j),j=1,nroot)
c
c     calculate physical error norms
c
  750 rewind nt
      do 830 l=1,nroot
      rt=root(l)
      read (nt) (v(i),i=1,n)
      call mltply (w,a,v,maxa,n,ncolbv,istoh,nblock,nstif)
      vnorm=0.0
      do 760 i=1,n
  760 vnorm=vnorm+w(i)*w(i)
      if (imass-1) 790,790,770
  770 call mltply (ww,b,v,maxa,n,ncolbv,istoh,nblock,nmass)
      do 780 i=1,n
  780 w(i)=w(i)-rt*ww(i,1)
      go to 810
  790 do 800 i=1,n
  800 w(i)=w(i)-rt*xm(i)*v(i)
  810 wnorm=0.0
      do 820 i=1,n
  820 wnorm=wnorm+w(i)*w(i)
      vnorm=dsqrt(vnorm)
      wnorm=dsqrt(wnorm)
      errvl(l)=wnorm/vnorm
  830 continue
c
  840 write (6,1090)
      nroot=nsch
      write (6,870) (root(j),j=1,nroot)
      write (6,1110)
      write (6,870) (errvl(j),j=1,nroot)
c
      write (nt) (root(i),i=1,nroot)
c
      return
c
  860 format (43h ***error   neg or zero diagonal element a(,i4,4h) =
     1 ,e11.4,21hbefore decomposition )
  870 format (1h0,6e20.12)
  880 format (1h0,6i20)
  890 format (1h0,6f20.2)
  900 format (44h0***error   solution terminated in *secant* ,/12x,25hri
     1gid body mode(s) found.,/1x)
  910 format (51h1inverse iteration gives following approximation to,18h
     1 lowest eigenvalue,1x)
  920 format (///48h rayleigh quotient shift is carried out for root,i3)
  930 format (42h0***error   pre-mature exit from *secant* ,/12x,37hiter
     1ation abandoned for root number =,i4/12x,37hnumber of iterations p
     2erformed      =,i4/1x)
  940 format (5h0rb =,e20.12,7h nsch =,i4)
  950 format (38h0***error   solution stop in *secant* ,/12x,1h(,i3,48h)
     1 factorizations performed in an attempt to find,32h lower bound on
     2 first eigenvalue,/12x,16hcheck the model.,/1x)
  960 format (1h1,4x,4hroot,4x,4hnite,18x,2hrc,15x,12hdet (a-rc*b),15x,2
     1 hff,13x,3heta,3x,4hidet,4x,2hif)
  970 format (1h0,4x,i4,4x,i4,8x,3e22.14,f7.2,2i6)
  980 format (42h0the deflated polynomial has no more roots)
  990 format (36h0***error  solution stop in *secant*,/10x,40hcalculated
     1 shift is non-positive. shift=,e20.11)
 1000 format (29h0(rc-rb) is smaller than tol )
 1010 format (16h0we jumped over ,i4,16h unknown root(s))
 1020 format (42h0***error   pre-mature exit from *secant* ,34h caused b
     1y either of the following,/12x,22h(1) bad model data, or,/12x,52h(
     22) root cluster (i.e., near equal or repeated eigen,36hvalues) enc
     3ountered at current shift,/16x,25hcausing storage over-flow,1x)
 1030 format (1h0,34x,4hroot,18x,2hrq,18x,4hnor=,i2)
 1040 format (1h0,4x,i4,4x,i4,8x,2e22.14)
 1050 format (20h0time for inv itern ,f5.2)
 1060 format (42h0no of iterations for each eigenvalue are /)
 1070 format (30h0time used for each eigenvalue/)
 1080 format (46h0following are error bounds on the eigenvalues/51h (the
     1 bounds depend on the shifting in the solution,28h and can therefo
     2re be large))
 1090 format (1h1,22h e i g e n v a l u e s)
 1100 format (38h0***error   solution stop in 'secant' ,/12x,23hno eigen
     1values computed,/1x)
 1110 format (///40h the following are physical error bounds,20h on the 00
     1eigenvalues )
      end
*deck strat
      subroutine strat (a,b,xm,v,d,maxa,ncolbv,icopl,root,nite,n,istoh
     1 ,nblock,jr,nov,nsk,nsch,eta,imass)
      implicit real*8 (a-h,o-z)
      common /shift/ rr, ra, rb, rc, fdetr, fdeta, fdetb, fdetc, ffr,
     1 ffa, ffb, ffc, idetr, ideta, idetb, idetc, ifr, ifa, ifb, ifc
      dimension a(1), b(1), v(1), d(1), xm(1), root(1)
      integer maxa(1), ncolbv(1), icopl(1), nite(1)
c
c   case1 no root jumping has occured. the calculated root has been
c   approached from below
c
      rtol=0.0000000001d0
      tol=rtol*root(jr)
      if (nov.gt.0) go to 20
      if (abs(root(jr)-rb).gt.tol) go to 60
      if (ra.gt.0.0) go to 10
      ra=rb/2.d0
      call bandet (a,b,xm,v,d,maxa,ncolbv,icopl,n,istoh,nblock,ra,nsch
     1 ,imass,fdeta,ideta,1)
      ffa=fdeta
      ifa=ideta
   10 rb=ra
      ffb=ffa
      ifb=ifa
      fdetb=fdeta
      idetb=ideta
      ra=rr
      ffa=ffr
      ifa=ifr
      fdeta=fdetr
      ideta=idetr
      go to 60
c
c   case2 root jumping has occured. the calculated root is smaller than
c   the current shift and is a simple root
c
   20 if (root(jr).gt.rc) nsk=1
      if (nsk.eq.1) go to 70
      if (abs(rc-root(jr)).lt.tol) go to 40
      if (abs(root(jr)-rb).lt.tol) go to 30
      ra=rb
      ffa=ffb
      ifa=ifb
      fdeta=fdetb
      ideta=idetb
   30 rb=rc
      ffb=ffc
      ifb=ifc
      fdetb=fdetc
      idetb=idetc
      go to 60
   40 if (abs(root(jr)-rb).gt.tol) go to 60
      if (ra.gt.0.0) go to 50
      ra=rb/2.d0
      call bandet (a,b,xm,v,d,maxa,ncolbv,icopl,n,istoh,nblock,ra,nsch
     1 ,imass,fdeta,ideta,1)
      ffa=fdeta
      ifa=ideta
   50 rb=ra
      ffb=ffa
      ifb=ifa
      fdetb=fdeta
      idetb=ideta
      ra=rr
      ffa=ffr
      ifa=ifr
      fdeta=fdetr
      ideta=idetr
   60 ffa=ffa/(ra-root(jr))
      call rscale (ffa,ifa)
      ffb=ffb/(rb-root(jr))
      call rscale (ffb,ifb)
      jr=jr+1
      nov=0
      eta=2.0d0
      return
c
c   case3 root jumping has occured. the calculated root is a multiple
c   root and/or is larger than the current shift
c
   70 if (ra.gt.0.0) go to 80
      ra=rb/2.d0
      call bandet (a,b,xm,v,d,maxa,ncolbv,icopl,n,istoh,nblock,ra,nsch
     1 ,imass,fdeta,ideta,1)
      ffa=fdeta
      ifa=ideta
   80 if (abs(root(jr)-rb).gt.tol) go to 90
      rb=ra
      ffb=ffa
      ifb=ifa
      fdetb=fdeta
      idetb=ideta
      ra=rr
      ffa=ffr
      ifa=ifr
      fdeta=fdetr
      ideta=idetr
   90 ffa=ffa/(ra-root(jr))
      call rscale (ffa,ifa)
      ffb=ffb/(rb-root(jr))
      call rscale (ffb,ifb)
      ffr=ffr/(rr-root(jr))
      call rscale (ffr,ifr)
      if (root(jr).le.rc) nov=nov-1
      jr=jr+1
      nite(jr)=0
      root(jr)=rc
      if (nov.gt.0) return
      nsk=0
      eta=2.0d0
      return
      end
*deck bandet
      subroutine bandet (a,b,xm,v,d,maxa,ncolbv,icopl,neq,istoh,nblock
     1 ,ra,nsch,imass,fdet,idet,kkk)
      implicit real*8 (a-h,o-z)
c
      common /tapeig/ scale, nstif, nt, nmass, nred, nshift
      dimension a(istoh), b(istoh), v(1), d(1), xm(1)
      integer maxa(1), ncolbv(1), icopl(1)
c
      nr=neq-1
      khbb=0
      nwrite=0
      if (kkk-2) 10,320,380
c
   10 tol=10000000.d0
      rtol=0.0000000001d0
      ntf=3
      is=1
      bscale=2.d0**(-80)
      uscale=2.d0**(80)
      nsch=0
      fdet=1.d0
      idet=0
   20 neql=1
      neqr=0
      mla=0
c
c - - factorize  matrix  ( a - ra*b )  ( loop over all blocks ) - -
c
      do 280 nj=1,nblock
c
      call recs (nstif,a,istoh,nj)
      ncolb=ncolbv(nj)
c
      if (imass.eq.1) go to 40
      call recs (nmass,b,istoh,nj)
      do 30 i=1,istoh
   30 a(i)=a(i)-ra*b(i)
      go to 60
   40 neqr=neqr+ncolb
      do 50 i=neql,neqr
      ii=maxa(i)-mla
   50 a(ii)=a(ii)-ra*xm(i)
c
   60 mm=maxa(khbb+1)-1
      if (nj.eq.icopl(nj)) go to 160
c
      nwrite=0
      ik=icopl(nj)-1
      im=0
      if (ik) 160,90,70
   70 do 80 k=1,ik
   80 im=im+ncolbv(k)
   90 khb=khbb-im
      ik=ik+1
      nj1=nj-1
c
c     reduce block by the preceeding coupling blocks
c
      do 150 nk=ik,nj1
c
      call recs (nred,b,istoh,nk)
      khb=khb-ncolbv(nk)
      mc=maxa(im+1)-1
c
      do 140 n=1,ncolb
      kn=maxa(khbb+n)-mm
      kl=kn+1
      ku=maxa(khbb+n+1)-1-mm
      kh=ku-kl-n+1
      kc=kh-khb
      if (kc.le.0) go to 140
      ic=0
      kcl=ncolbv(nk)-kc+1
      if (kcl.gt.0) go to 100
      ic=1-kcl
      kcl=1
  100 kcr=ncolbv(nk)
      klt=ku-ic
c
      do 130 k=kcl,kcr
c
      ic=ic+1
      klt=klt-1
      ki=maxa(k+im)-mc
      nd=maxa(k+im+1)-ki-mc-1
      if (nd) 130,130,110
  110 kk=min(ic,nd)
      c=0.d0
      do 120 l=1,kk
  120 c=c+b(ki+l)*a(klt+l)
      a(klt)=a(klt)-c
  130 continue
  140 continue
c
      im=im+ncolbv(nk)
c
  150 continue
      nwrite=nj1
c
c     reduce block by itself
c
  160 do 270 n=1,ncolb
      kn=maxa(khbb+n)-mm
      kl=kn+1
      ku=maxa(khbb+n+1)-1-mm
      kdif=ku-kl
      kh=min(kdif,n-1)
      ks=n+khbb
      if (kh) 240,210,170
  170 k=n-kh
      klt=kl+kh
      ic=0
      if ((n-1).lt.kdif) ic=kdif-n+1
c
      do 200 j=1,kh
      ic=ic+1
      klt=klt-1
      ki=maxa(khbb+k)-mm
      nd=maxa(khbb+k+1)-ki-mm-1
      if (nd) 200,200,180
  180 kk=min(ic,nd)
      c=0.
      do 190 l=1,kk
  190 c=c+a(ki+l)*a(klt+l)
      a(klt)=a(klt)-c
  200 k=k+1
c
  210 k=ks
      e=0.d0
      do 230 kk=kl,ku
      k=k-1
      c=a(kk)/d(k)
      if (abs(c).lt.tol) go to 220
      go to 250
  220 e=e+c*a(kk)
  230 a(kk)=c
      a(kn)=a(kn)-e
c
  240 d(ks)=a(kn)
      if (d(ks)) 270,250,270
  250 is=is+1
      if (is.le.ntf) go to 260
      write (6,460) ntf,ra
      stop
  260 ra=ra*(1.d0-rtol)
      khbb=0
      nwrite=0
      go to 20
c
  270 continue
c
      nwrite=nwrite+1
      call wecs (nred,a,istoh,nwrite)
      khbb=khbb+ncolb
      neql=neql+ncolb
      mla=maxa(neql)-1
  280 continue
c
      if (d(neq).ne.0.) go to 300
      aa=abs(d(1))
      do 290 i=2,neq
  290 aa=aa+abs(d(i))
      d(neq)=-(aa/nr)*0.00000000000001d0
c
  300 do 310 i=1,neq
      if (d(i).lt.0.) nsch=nsch+1
      fdet=fdet*d(i)
      if (fdet.lt.uscale.and.fdet.ge.bscale) go to 310
      call rscale (fdet,idet)
  310 continue
c
      return
c
c - - find eigenvectors ( loop over all blocks ) - -
c
  320 do 370 nj=1,nblock
      if (nblock.eq.1) go to 330
      call recs (nred,a,istoh,nj)
  330 ncolb=ncolbv(nj)
      mm=maxa(khbb+1)-1
      do 360 n=1,ncolb
      kl=maxa(n+khbb)-mm+1
      ku=maxa(n+khbb+1)-mm-1
      if (ku-kl) 360,340,340
  340 ks=n+khbb
      k=ks
      c=0.
      do 350 kk=kl,ku
      k=k-1
  350 c=c+a(kk)*v(k)
      v(ks)=v(ks)-c
  360 continue
      khbb=khbb+ncolb
  370 continue
c
c     backsubstitute
c
  380 do 390 n=1,neq
  390 v(n)=v(n)/d(n)
      nbl=nblock
      do 450 nj=1,nblock
      if (nblock.eq.1) go to 400
      nwrite=nblock+1-nj
      call recs (nred,a,istoh,nwrite)
      ncolb=ncolbv(nbl)
  400 khbb=khbb-ncolb
      mm=maxa(khbb+1)-1
      n=ncolb
      do 440 l=1,ncolb
      kl=maxa(n+khbb)-mm+1
      ku=maxa(n+khbb+1)-mm-1
      if (ku-kl) 430,410,410
  410 ks=khbb+n
      k=ks
      do 420 kk=kl,ku
      k=k-1
  420 v(k)=v(k)-a(kk)*v(ks)
  430 n=n-1
  440 continue
      nbl=nbl-1
  450 continue
c
      return
c
  460 format (37h0***error   solution stop in *bandet*,/12x,1h(,i3,37h)
     1triangular factorizations attempted,/12x,16hcurrent shift = ,
     2 e20.14  ,/1x)
      end
*deck mltply
      subroutine mltply (a,b,c,maxa,neq,ncolbv,istoh,nblock,ntape)
      implicit real*8 (a-h,o-z)
c
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c .                                                                   .
c .   p r o g r a m                                                   .
c .      . to calculate   a = b*c   ,   where  b  is stored in        .
c .        compacted form  ,  a  and  c  are vectors                  .
c .                                                                   .
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c
      dimension a(1), b(istoh), c(1)
      integer maxa(1), ncolbv(1)
c
      if (neq.gt.1) go to 10
      call recs (ntape,b,istoh,1)
      a(1)=b(1)*c(1)
      return
c
   10 do 20 i=1,neq
   20 a(i)=0.
      neql=1
      neqr=0
      mla=0
      do 70 l=1,nblock
      call recs (ntape,b,istoh,l)
      neqr=neqr+ncolbv(l)
      do 30 i=neql,neqr
      kl=maxa(i)-mla
      ku=maxa(i+1)-1-mla
      ii=i+1
      cc=c(i)
      do 30 kk=kl,ku
      ii=ii-1
   30 a(ii)=a(ii)+b(kk)*cc
      do 60 i=neql,neqr
      kl=maxa(i)+1-mla
      ku=maxa(i+1)-1-mla
      if (ku-kl) 60,40,40
   40 ii=i
      aa=0.
      do 50 kk=kl,ku
      ii=ii-1
   50 aa=aa+b(kk)*c(ii)
      a(i)=a(i)+aa
   60 continue
c
      neql=neql+ncolbv(l)
      mla=maxa(neql)-1
   70 continue
c
      return
      end
*deck rscale
      subroutine rscale (fnum,iexp)
      implicit real*8 (a-h,o-z)
      bscale=2.d0**(-80)
      uscale=2.d0**80
      ipower=80
   10 if (abs(fnum).lt.uscale) go to 20
      fnum=fnum*bscale
      iexp=iexp+ipower
      go to 10
   20 if (abs(fnum).ge.bscale) go to 30
      fnum=fnum*uscale
      iexp=iexp-ipower
      go to 10
   30 continue
      return
      end
*deck wrmod
      subroutine wrmod (frq,phi,id,numnp,ndof,neq,nfreq,nmode)
      implicit real*8 (a-h,o-z)
c
      common /tapeig/ scale, nstif, nt, nmass, nred, nshift
      character*8 file1
      common/t51com/file1
      dimension frq(1), phi(1), id(ndof,1)
      dimension d(6)
c
c     print eigenvector for each mode
c
	  call esend()
      rewind nt
      do 40 im=1,nfreq
      if (im.gt.nmode) return
      call mfopen3(im)
      call eshead(file1,8);
      write (6,50) im,frq(im)
      read (nt) (phi(i),i=1,neq)
      do 30 ii=1,numnp
      do 10 i=1,6
   10 d(i)=0.
      do 20 i=1,ndof
      kk=id(i,ii)
      il=i
c      if (ndof.eq.2) il=i+1
   20 if (kk.ne.0) d(il)=phi(kk)
      write (6,60) ii,d
      nn=ii
      call esdis(d,nn)
   30 continue
      call esend()
   40 continue
      return
c
c
   50 format (///28h   m o d e  s h a p e  n o .,i3,55x,13h( frequency =
     1 ,e11.4,2h )//9h    node ,12x,14hx-displacement,4x,
     2 14hy-displacement,4x,14hz-displacement,8x,10hx-rotation,
     4 8x,10hy-rotation,8x,10hz-rotation/)
   60 format (2x,i5,8x,6e18.6)
      end
*deck pnorm
      subroutine pnorm (maxa,ncolbv,s,b,xm,irbm,rbmsh,neq,istoh,
     1                  nblock,nstif,nmass,imass,anorm)
      implicit real*8 (a-h,o-z)
c
c     finds a norm of the matrix s
c
      dimension s(istoh),b(istoh),xm(1)
      integer maxa(1),ncolbv(1)
c
      neql=1
      neqr=0
      mla=0
      sum=0.
      shift=0.d0
      nt=9
      rewind nt
      if(imass.eq.1)call recs(nmass,xm,neq,1)
c
      do 20 nj=1,nblock
      neqr=neqr + ncolbv(nj)
      call recs(nstif,s,istoh,nj)
      if (imass.eq.2 .and. irbm.gt.0) call recs(nmass,b,istoh,nj)
      if (irbm .gt. 0) write (nt) s
c
      do 10 i=neql,neqr
      ii=maxa(i) - mla
      aa=s(ii)
      if (aa.gt.0.) go to 10
      write (6,1000) i,aa
      stop
   10 sum=sum + aa
c
      if (rbmsh.lt.0. .or. irbm.eq.0) go to 19
c
      if (imass.eq.2) go to 15
      do 12 i=neql,neqr
      ii=maxa(i) - mla
      dum=xm(i)/s(ii)
      if (dum.gt.shift) shift=dum
   12 continue
      go to 19
c
   15 do 17 i=neql,neqr
      ii=maxa(i) - mla
      dum=b(ii)/s(ii)
      if (dum.gt.shift) shift=dum
   17 continue
c
   19 neql=neql + ncolbv(nj)
   20 mla=maxa(neql) - 1
c
      anorm=(sum/neq)*.000000001d0
      if (irbm.eq.0) return
c
c     apply shift if rigid body modes are present
c
      if (rbmsh) 30,25,22
   22 write (6,3000)
      stop
   25 rbmsh=-0.001d0/shift
      write (6,2000) rbmsh
   30 neql=1
      neqr=0
      mla=0
      rewind nt
c
      do 120 nj=1,nblock
      neqr=neqr + ncolbv(nj)
      read (nt) s
      if (imass.eq.2) go to 104
c
      do 106 i=neql,neqr
      ii=maxa(i) - mla
  106 s(ii)=s(ii) - rbmsh*xm(i)
      go to 102
c
  104 call recs(nmass,b,istoh,nj)
      do 108 i=1,istoh
  108 s(i)=s(i) - rbmsh*b(i)
c
  102 call wecs(nstif,s,istoh,nj)
      neql=neql + ncolbv(nj)
  120 mla=maxa(neql) - 1
c
      return
c
 1000 format (43h ***error   neg or zero diagonal element a(,i4,4h) =  ,
     1        e11.4,21hbefore decomposition   )
 2000 format (//32h rigid body mode shift applied =,e15.5)
 3000 format (//46h *** error in frequency control card input ***,/,
     1       54h the value for rigid body shift input must be negative )
c
      end
*deck wrfreq
      subroutine wrfreq (aa,nfreq,rbmsh)
      implicit real*8 (a-h,o-z)
c
c     subroutine to read and write frequencies
c
      dimension aa(1)
      common /tapeig/ scale,nstif,nt,nmass,nred,nshift
c
      backspace nt
      read(nt) (aa(i),i=1,nfreq)
      if (rbmsh.eq.0.) go to 100
      do 200 i=1,nfreq
      aa(i)=aa(i) + rbmsh
  200 if (aa(i).le.0.) aa(i)=1.d-10
  100 do 300 i=1,nfreq
  300 aa(i)=dsqrt(aa(i))
c
      write(6,2010)
      pi=4.d0*datan (1.d0)
      do 500 i=1,nfreq
      acirc=aa(i)/(2.d0*pi)
      aperd=1.d0/acirc
  500 write(6,2000) i,aa(i),acirc,aperd
c
      return
 2000 format (9x,i5,24x,e11.4,15x,e11.4,15x,e11.4)
 2010 format (1h1,// 24h   f r e q u e n c i e s   //  2x,
     121h    frequency number   ,10x,20h frequency (rad/sec) ,4x,
     224h  frequency (cycles/sec),8x,16hperiod (seconds)  / )
      end
