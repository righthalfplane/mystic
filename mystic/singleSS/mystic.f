*deck steady
c     program steady(input=200,output=200,tape5=input,tape6=output,
c    .               tape8=512,tape11=512,tape31=512,tape32=512,
c    . tape35=515,tape49=515,tape9=515)
      program main
      implicit real*8 (a-h,o-z)
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/adrnc/nc1,nc2,nc3,nc4
      common/scom/nload,ncon,nconi,nsave,ndmax,mcon
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/tcom/munit,lunit,nstif,nred,nunit
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/ncom/numest,nvect,istoh,maxbh
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common /addb/ neql,neqr,mla,nblock
      common/flgs1/ibig,idamp
      common/dtwo/itwo,ndof,idim
      common/tapcom/nt11,nt31
      common/list/data(64),ndum
      character*8 cdata
      common/clist/cdata(64)
      common/fsicom/fsifac,wfsi,kfsi
      common/elcom/nwelem
      common/comtyp/itype,ngroup,ngin,nlin
      common/wcom/w,isrlc
      common/switch/ipsm,ipkbar,ipinpt,ipreac,ieldat,ipsdm,ipcnpf
     . ,ibigw
      character*8 head
      dimension head(8)
      dimension com(15)
      dimension etype(23),ptype(11)
      dimension ia(1),kploc(11)
      common a(2005120)
      real a
      integer ia
      equivalence (a(1),ia(1)),(kploc(1),kpnode)
      character*8 com,etype,ptype,end,strs,off,gno,word
      data com/'stop','node','form','load','solv','back','chec',
     .  'pon','poff','big','damp','elen','swit','fsi','srlc'/
      data etype/'reac','tape','con','user','quad','tri','beam',
     .  'axsh','pqdm','hex8','phex','ptri','bm3d','atri','aqua',
     .  'ashl','astf','pzel','bemd','feac',3*' '/
      data ptype/'node','dof','elem','load','comp','equ','equl',
     .  'equr','stre','dis','mat'/
      data end,strs,off/'end','stre','off'/
      data ibig,idamp/0,1/
      data kploc/4*1,0,1,1,0,1,1,1/
      n1=1
      ip=1
      itwo=2
      nt11=11
      nt31=31
      kfsi=1
      fsifac=0.
      isrlc=0
      iequit=1
      ibigw=1
      ieldat=1
      icheck=0
      nmax=2000000
      munit=2
      lunit=3
      nstif=4
      nred=5
      nunit=6
      ngroup=200
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
      do 30 i=1,15
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
      go to (900,60,530,540,560,590,700,710,720,760,770,
     .       780,790,800,810),ic
  790 continue
      ipsm=data(2)
      ipkbar=data(3)
      ipinpt=data(4)
      ipreac=data(5)
      ieldat=data(6)
      ipsdm=data(7)
      ipcnpf=data(8)
      ibigw=data(9)
      write(6,1009)ipsm,ipkbar,ipinpt,ipreac,ieldat,ipsdm,ipcnpf
     1,ibigw
 1009 format(1x,"ipsm= ",i2," ipkbar= ",i2," ipinpt= ",i2," ipreac="
     1,i2," ieldat= ",i2," ipsdm= ",i2," ipcnpf= ",i2," ibigw= ",i2)
      go to 10
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
     .       240,250,260,270,280,290,300),ic
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
  500 continue
      nume=data(2)
      nummat=data(3)
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
      n4=n3+neq*itwo
      n5=n4+nvect*itwo
      n6=n5+nvect*itwo
      n7=n6+numnp*ndof
      min=nmax+n7-n1l+1
      write(6,2130)min
      if(min.le.nmax)go to 550
      stop
  550 continue
      call load(a(n2),a(n3),a(n6),a(n1),a(n4),a(n5),a(nc3),a(nc4),
     .          ndof,nvect,0)
      go to 10
  560 continue
c
c        solution loop
c
      call second2(ta)
      ns=1
      nword=1
      istres=0
      iequit=kpeq1+kpeq2+kpeq3
      if(cdata(2).ne.strs)go to 570
      istres=1
      nword=2
  570 continue
      nword=nword+1
      if(cdata(nword).eq.end)go to 580
      w=data(nword)
      iback=0
      call solve(w,iequit,ns,istres,iback)
      ns=2
      if(nword.ge.64)go to 580
      go to 570
  580 continue
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
      if(cdata(2).eq.strs)istres=1
      iequit=kpeq1+kpeq2+kpeq3
      iback=1
      call solve(w,iequit,ns,istres,iback)
      call second2(tb)
      t=tb-ta
      write(6,2140)t
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
      do 740 n=1,11
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
      if(cdata(2).eq.off)idamp=0
      go to 10
  780 continue
c
c          number of words required for element data
c
      nwelem=data(2)
      go to 10
  800 continue
      kfsi=2
      fsifac=data(2)
      write(6,2160)fsifac
      if(fsifac.eq.0.)stop
      go to 10
  810 continue
      isrlc=1
      if(cdata(2).eq.off)isrlc=0
      go to 10
  900 continue
      call exit
 1000 format(8a4)
 1010 format(1h1,i4,5x,8a4)
 2000 format(1h0,10x,27hstop unreconized command = ,a10)
 2100 format(1h0,10x,34htime required to form stiffness = ,f13.3,
     .  8h seconds)
 2120 format(1h0,10x,29htime required for solution = ,f13.3,8h seconds)
 2130 format(1h0,10x,45hnumber of words required to input load data = ,
     .       i10)
 2140 format(1h0,10x,29htime required for backsub  = ,f13.3,8h seconds)
 2150 format(1h0,10x,27hmaximum numger of groups = ,i5)
 2160 format(1h0,10x,37hfluid structure interaction factor = ,1pe12.4)
      end
*deck fwrite
*deck addban
      subroutine addban(a,maxa,s,lm,nd)
      implicit real*8 (a-h,o-z)
      save
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
      save
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
      save
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/adrnc/nc1,nc2,nc3,nc4
      common/scom/nload,ncon,nconi,nsave,ndmax,mcon
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/tcom/munit,lunit,nstif,nred,nunit
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/fsicom/fsifac,wfsi,kfsi
      common/ncom/numest,nvect,istoh,maxbh
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common /addb/ neql,neqr,mla,nblock
      common/dtwo/itwo,ndof,idim
      common/list/data(64),ndum
      character*8 cdata
      common/clist/cdata(64)
      common/tapcom/nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlin
      common/wcom/w,isrlc
      dimension ia(1)
      common a(1)
      real a
      integer ia
      equivalence (a(1),ia(1))
      ind=1
      nvect=data(3)
      mbh=100+neq/16
      if(nvect.le.0)nvect=neq
      lbs=4*nvect
      if(nvect.ne.neq)lbs=lbs*2
      call addres(ia(n1),ia(n1l))
      n1a=n1+neq+1
      n1b=n1a+neq
      iblock=4
      nblock=1
      kflag=0
      if(kfsi.eq.2)kflag=1
      mstore=1+lbs*itwo+numest+nsave*itwo+ng
c
c        calculate maximum block size for static solution
c
   30 mel=2*iblock
      istorl=(nmax-mstore-mel-2*mbh)/(2*itwo)
      call sblock(a(n1),a(n1a),a(n1b),istorl,nblock,neq,nwk,istoh)
      if(nblock.le.iblock)go to 40
      iblock=iblock*2
      go to 30
c***      if(iblock.le.4096)go to 30
c***      write(6,2020)iblock
c***      stop
   40 continue
      if(kflag.eq.0)go to 45
      kflag=0
      if(nblock.gt.1)go to 45
      mstore=mstore+istorl
      go to 30
   45 continue
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
      mcon=nconi
      call open(istoh,nblock,icheck)
      if(mcon.le.0)go to 68
c
c        prewrite constraint tape
c
      do 64 i=1,mcon
      call wecs(7,a,nsave,i)
   64 continue
   68 continue
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
      n6=n1+nblock*ng*itwo
      n6e=n6
      n7=n6+numest
      min=nmax+n7-n3l+1
      write(6,2090)min
      if(min.gt.nmax)stop
      ind=4
      call preaem(a(n1),a(n5l),a(n3l),a(n6),nblock)
      ind=1
c
c        setup assembly storage
c
      nc1=n1+3*ndmax*itwo
      nc2=nc1+ndmax*itwo
      nc3=nc2+ndmax*itwo
      nc4=nc3+ndmax*itwo
      n2=nc4+itwo
      n3=n2+istoh*itwo
      n4=n3+istoh*itwo
      n5=n4+istoh*itwo
      if(nblock.le.1)n5=n4
      n6=n5
      n6e=n6
      n7=n6+numest
      min=nmax+n7-n1l+1
      write(6,2080)min
      if(min.le.nmax)go to 90
      stop
   90 continue
      call assem1(a(n3),a(n2),a(n4),a(n6),a(n1l),a(n3l),a(n5l),
     .   istoh,maxbh,
     .           munit,0)
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
*deck assem1
      subroutine assem1(a,aa,cc,ee,maxa,ncolbv,ke,istoh,maxbh,munit,ifl)
      implicit real*8 (a-h,o-z)
      save
      common/scom/nload,ncon,nconi,nsave,ndmax,mcon
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/addb/neql,neqr,mla,nblock
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/fsicom/fsifac,wfsi,kfsi
      common/flgs1/ibig,idamp
      common/tapcom/nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlin
      common/wcom/w,isrlc
       common/switch/ipsm,ipkbar,ipinpt,ipreac,ieldat,ipsdm,ipcnpf
     . ,ibigw
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
      inds=ind
      do 40 l=1,nblock
      read(9)(ke(i),i=1,ng)
      call recs(munit,maxa,maxbh,l)
      mla=maxa(1)-1
      neqr=neqr+ncolbv(l)
      ind=inds
      do 35 k=1,kfsi
      if(k.eq.2)ind=6
      if(nblock.gt.1)go to 22
c
c        one solution block
c
      do 5 i=1,istoh
      a(i)=0.
      aa(i)=0.
    5 continue
      do 10 i=1,ng
      if(ke(i).eq.0)go to 10
      if(ng.ne.1)call eread(ee,i)
      n2=n2s
      kpri=1
      call gstiff
      n2=n3
      kpri=2
      call gstiff
   10 continue
      n2=n2s
      ipt=ipt+1
      call wecs(1,aa,istoh,ipt)
      ipt=ipt+1
      call wecs(1,a,istoh,ipt)
      if(idamp.eq.0)go to 35
      do 15 i=1,istoh
      aa(i)=0.
   15 continue
      kpri=3
      do 20 i=1,ng
      if(ke(i).eq.0)go to 20
      if(ng.ne.1)call eread(ee,i)
      call gstiff
   20 continue
      ipt=ipt+1
      call wecs(1,aa,istoh,ipt)
      go to 35
   22 continue
c
c        two or more blocks
c
      do 25 i=1,istoh
      a(i)=0.
      aa(i)=0.
      cc(i)=0.
   25 continue
      do 30 i=1,ng
      if(ke(i).eq.0)go to 30
      if(ng.ne.1)call eread(ee,i)
      kpri=1
      n2=n2s
      call gstiff
      n2=n3
      kpri=2
      call gstiff
      n2=n4
      if(idamp.eq.0)go to 30
      kpri=3
      call gstiff
   30 continue
      n2=n2s
      ipt=ipt+1
      call wecs(1,aa,istoh,ipt)
      ipt=ipt+1
      call wecs(1,a,istoh,ipt)
      if(idamp.eq.0)go to 35
      ipt=ipt+1
      call wecs(1,cc,istoh,ipt)
   35 continue
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
      w2=w*w
      ipt=0
      n6s=n6
      dnom=1.
      if(kfsi.eq.2)dnom=1./(fsifac*wfsi*wfsi)
      do 90 l=1,nblock
      read(9)(ke(i),i=1,ng)
      call recs(munit,maxa,maxbh,l)
      mla=maxa(1)-1
      neqr=neqr+ncolbv(l)
      ipt=ipt+1
      call recs(1,aa,istoh,ipt)
      ipt=ipt+1
      call recs(1,cc,istoh,ipt)
      if(ibigw.eq.0)goto 905
      if(ipsm.ne.1)goto 905
c***   print assembled mass and stiffness matrices   ***
      write(6,907)w
  907 format(1h0,"omega(rad/sec)=",e12.5//)
      write(6,908)
  908 format(/,"stiffness matrix print")
      write(6,910)(aa(ix),ix=1,istoh)
  910 format(1x,11e12.4)
      write(6,911)
  911 format(/,"mass matrix print")
      write(6,910)(cc(ix),ix=1,istoh)
  905 continue
c***   end of print code   ***
      if(isrlc.eq.0)then
      do 70 i=1,istoh
      aa(i)=aa(i)-w2*cc(i)
   70 continue
      if(idamp.eq.0)go to 76
      ipt=ipt+1
      call recs(1,cc,istoh,ipt)
      do 75 i=1,istoh
      cc(i)=cc(i)*w
   75 continue
      go to 79
   76 continue
      do 77 i=1,istoh
      cc(i)=0.
   77 continue
   79 continue
      else
      do 170 i=1,istoh
      aa(i)=w*cc(i)-aa(i)/w
  170 continue
      if(idamp.eq.0)go to 176
      ipt=ipt+1
      call recs(1,cc,istoh,ipt)
      if(ibigw.ne.0.and.ipsm.eq.1)then
      write(6,912)
  912 format(/,"damping matrix print")
      write(6,910)(cc(ix),ix=1,istoh)
      endif
      go to 179
  176 continue
      do 177 i=1,istoh
      cc(i)=0.
  177 continue
  179 continue
      do 180 i=1,istoh
      value=cc(i)
      cc(i)=aa(i)
      aa(i)=value
  180 continue
      endif
      if(nlin.le.0)go to 85
      do 80 i=nlin,ng
      if(ke(i).eq.0)go to 80
      if(ng.ne.1)call eread(ee,i)
      kpri=1
      n6=n6s
      call gstiff
      kpri=2
      n6=n7
      call gstiff
   80 continue
      n6=n6s
   85 continue
      if(kfsi.eq.1)go to 89
      ipt=ipt+1
      call recs(1,a,istoh,ipt)
      do 86 i=1,istoh
      aa(i)=aa(i)*dnom+a(i)
   86 continue
      ipt=ipt+1
      call recs(1,a,istoh,ipt)
      do 87 i=1,istoh
      aa(i)=aa(i)-w2*a(i)
   87 continue
      if(idamp.eq.0)go to 89
      ipt=ipt+1
      call recs(1,a,istoh,ipt)
      do 88 i=1,istoh
      cc(i)=dnom*cc(i)+a(i)*w
   88 continue
   89 continue
      call wecs(4,aa,istoh,l)
      if(ibigw.ne.0.and.ipsm.eq.1)then
      write(6,918)
  918 format(/,"matrix real")
      write(6,910)(aa(ix),ix=1,istoh)
      write(6,919)
  919 format(/,"matrix imaginary")
      write(6,910)(cc(ix),ix=1,istoh)
      endif
      neql=neql+ncolbv(l)
   90 continue
      return
      end
*deck colht
      subroutine colht (mht,nd,lm)
      implicit real*8 (a-h,o-z)
      save
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
*deck colsol
      subroutine colsol(maxa,maxa2,ncolbv,icopl,va,vai,vb,vbi,a,ai,b,bi,
     .                  da,dai,db,dbi,nblock,neq,istorl,nvect,mvect,
     .                  maxbh,nstif,nred,lunit,munit,nunit,kkk)
      implicit real*8 (a-h,o-z)
      save
c     . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c     .
c     .   p r o g r a m
c     .        to solve finite element static equilibrium equations out-
c     .        core, using compacted storage and column reduction scheme
c     .
c     . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c
      dimension a(1),ai(1),b(1),bi(1)
      dimension va(1), vai(1), vb(1), vbi(1)
      dimension da(1), dai(1), db(1), dbi(1)
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
      ci=0.
      do 80 l=1,kk
      c=c+b(ki+l)*a(klt+l)-bi(ki+l)*ai(klt+l)
      ci=ci+bi(ki+l)*a(klt+l)+b(ki+l)*ai(klt+l)
   80 continue
      ai(klt)=ai(klt)-ci
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
      ci=0.
      do 170 l=1,kk
      c=c+a(ki+l)*a(klt+l)-ai(ki+l)*ai(klt+l)
      ci=ci+ai(ki+l)*a(klt+l)+a(ki+l)*ai(klt+l)
  170 continue
      ai(klt)=ai(klt)-ci
      a(klt)=a(klt)-c
  180 k=k+1
c
  190 continue
      if (kss.le.1) go to 210
      kup=min0(ku,kl+ks-mhold-2)
      k=kss
      e=0.
      ei=0.
      do 200 kk=kl,kup
      k=k-1
      c=(a(kk)*da(k)+ai(kk)*dai(k))/(da(k)**2+dai(k)**2)
      ci=(ai(kk)*da(k)-a(kk)*dai(k))/(da(k)**2+dai(k)**2)
      e=e+c*a(kk)-ci*ai(kk)
      ei=ei+ci*a(kk)+c*ai(kk)
      ai(kk)=ci
      a(kk)=c
  200 continue
      a(kn)=a(kn)-e
      ai(kn)=ai(kn)-ei
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
      ei=0.
      do 240 kk=kl,kup
      k=k-1
      c=(a(kk)*db(k)+ai(kk)*dbi(k))/(db(k)**2+dbi(k)**2)
      ci=(ai(kk)*db(k)-a(kk)*dbi(k))/(db(k)**2+dbi(k)**2)
      e=e+c*a(kk)-ci*ai(kk)
      ei=ei+ci*a(kk)+c*ai(kk)
      ai(kk)=ci
      a(kk)=c
  240 continue
      a(kn)=a(kn)-e
      ai(kn)=ai(kn)-ei
      if (kup.ge.ku) go to 250
      kl=kup+1
      go to 220
  250 continue
      da(kss)=a(kn)
      dai(kss)=ai(kn)
      if(da(kss))260,254,260
  254 if(dai(kss))260,256,260
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
      ci=0.
      kss=ks-nhold
      if (kss.le.1) go to 330
      kup=min0(ku,kl+ks-nhold-2)
      k=kss
      c=0.
      ci=0.
      do 320 kk=kl,kup
      k=k-1
      c=c+a(kk)*va(k)-ai(kk)*vai(k)
      ci=ci+ai(kk)*va(k)+a(kk)*vai(k)
  320 continue
      va(kss)=va(kss)-c
      vai(kss)=vai(kss)-ci
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
      ci=0.
      do 360 kk=kl,kup
      k=k-1
      c=c+a(kk)*vb(k)-ai(kk)*vbi(k)
      ci=ci+ai(kk)*vb(k)+a(kk)*vbi(k)
  360 continue
      va(kss)=va(kss)-c
      vai(kss)=vai(kss)-ci
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
      nvectb=0
      nhold=0
      nlast=0
      mvectb=0
      mlast=0
      mhold=0
      do 420 nj=1,neq
      if (nj.le.mlast) go to 420
      mvectb=mvectb+1
      call recs (nunit,da,mvect,mvectb)
      mhold=mlast
      mlast=mlast+mvect
      kl=1
      ku=min0(mvect,neq-mhold)
      neqf=nj
  390 continue
      if (neqf.le.nlast) go to 400
      nhold=nlast
      nlast=nlast+nvect
      if (nvectb.gt.0) call wecs (lunit,va,nvect,nvectb)
      nvectb=nvectb+1
      call recs (lunit,va,nvect,nvectb)
      mv=0
      go to 390
  400 continue
      kup=min0(ku,kl+nlast-neqf)
      do 410 k=kl,kup
      mv=mv+1
      vns=va(mv)
      va(mv)=(va(mv)*da(k)+vai(mv)*dai(k))/(da(k)**2+dai(k)**2)
      vai(mv)=(vai(mv)*da(k)-vns*dai(k))/(da(k)**2+dai(k)**2)
  410 continue
      if (kup.ge.ku) go to 420
      kl=kup+1
      neqf=nlast+1
      go to 390
  420 continue
      call wecs (lunit,va,nvect,nvectb)
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
      va(k)=va(k)-a(kk)*va(kss)+ai(kk)*vai(kss)
      vai(k)=vai(k)-ai(kk)*vks-a(kk)*vai(kss)
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
      vb(k)=vb(k)-a(kk)*va(kss)+ai(kk)*vai(kss)
      vbi(k)=vbi(k)-ai(kk)*vks-a(kk)*vai(kss)
  500 continue
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
      save
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/adrnc/nc1,nc2,nc3,nc4
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/scom/nload,ncon,nconi,nsave,ndmax,mcon
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/dtwo/itwo,ndof,idim
      common/tapcom/nt11,nt31
      common/comtyp/itype,ngroup,ngin,nlin
      common a(1)
      real a
      nd=nummat
      nd1=nd+1
      nk=2*(nd+1)
      n101=n6e+4*itwo
      n102=n101+nk*nume*itwo
      n103=n102+nd*nd*itwo
      n104=n103+nd*nd*itwo
      n105=n104+nd*nd*itwo
      n106=n105+nd1*nume
      if(n106.lt.n1l)go to 20
      min=nmax+n106-n1l+1
      write(6,1000)min
      stop
   20 if(ind.ne.0)go to 30
      ndmax=max0(ndmax,nd)
      nsave=max0(nsave,6*nd+1)
      call conp(a(n101),a(n105),a(n1l),a(n2l),nd,ndof,nd1)
      midest=n106-(n101-1)
      call ewrite(a(n6e),ng)
      return
   30 if(ind.ne.1)go to 40
      call rcon(a(n1),a(nc1),a(nc2),a(nc3),a(nc4),a(n2),a(n101),a(n102),
     .           a(n103),a(n104),a(n105),a(n1l),nd,nd1,0)
      return
   40 continue
      nadd=n6
      if(kpri.eq.2)nadd=n7
      call rcon(a(n1),a(nc1),a(nc2),a(nc3),a(nc4),a(nadd),a(n101),
     .           a(n102),a(n103),a(n104),a(n105),a(n1l),nd,nd1,1)
      return
 1000 format(1h0,10x,48hstop working space must be increased to at least
     .                  ,i10,6h words)
      end
*deck conp
      subroutine conp(cc,lm,mht,id,nd,ndof,nk)
      implicit real*8 (a-h,o-z)
      save
      dimension cc(2,nk,1),lm(nk,1),mht(1),id(ndof,1)
      common/scom/nload,ncon,nconi,nsave,ndmax,mcon
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat
      common/tapcom/nt11,nt31
      common/list/data(64),ndum
      character*8 cdata
      common/clist/cdata(64)
      character*8 dof
      data dof/'dof'/
      do 80 ne=1,nume
      nconi=nconi+1
      lm(nk,ne)=nconi
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
      nst=2*nd
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
      nst=nd
   60 continue
c
c        read the constraints
c
      do 70 i=1,nk
      cc(1,i,ne)=data(2*i+nst)
      cc(2,i,ne)=data(2*i+1+nst)
   70 continue
      call colht(mht,nd,lm(1,ne))
      if(kpelem.eq.0)go to 80
      write(6,1015)
      write(6,1020)(lm(i,ne),i=1,nd)
      write(6,1030)(cc(1,i,ne),i=1,nk)
      write(6,1030)(cc(2,i,ne),i=1,nk)
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
      save
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
*deck equit
      subroutine equit(aa,aai,ab,abi,b,bi,ca,cai,cb,cbi,maxa,ncolbv,a,
     .                 neq,istoh,nblock,nvect,maxbh,munit,nstif,lunit,
     .                 nunit)
      implicit real*8 (a-h,o-z)
      save
c
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c .                                                                   .
c .   p r o g r a m                                                   .
c .      . to calculate   a = b*c   ,   where  b  is stored in        .
c .        compacted form  ,  a  and  c  are vectors                  .
c .                                                                   .
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
c
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat
      common/flgs1/ibig,idamp
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/adrnc/nc1,nc2,nc3,nc4
      common/scom/nload,ncon,nconi,nsave,ndmax,mcon
      common/dtwo/itwo,ndof,idim
      dimension aa(1),aai(1),ab(1),abi(1),b(1),bi(1)
      dimension ca(1),cai(1),cb(1),cbi(1)
      dimension a(1)
      integer maxa(1),ncolbv(1)
      if(ibig.eq.0)go to 5
      write(6,1010)
      return
    5 continue
      if(neq.gt.1)go to 10
c
c        one dof case
c
      call recs(nstif,b,istoh,1)
      call recs(lunit,ca,nvect,1)
c
c        save diagonal
c
      call recs(nunit,aa,nvect,1)
      call wecs(nunit,aa,nvect,2)
      call wecs(nunit,ca,nvect,3)
      aa(1)=b(1)*ca(1)-bi(1)*cai(1)
      aai(1)=b(1)*cai(1)+bi(1)*ca(1)
      call wecs(nunit,aa,nvect,1)
      go to 220
   10 continue
c
c        zero a array
c
      ipt=nload
      do 30 n=1,nload
      call recs(nunit,aa,nvect,n)
      ipt=ipt+1
      call wecs(nunit,aa,nvect,ipt)
      call recs(lunit,aa,nvect,n)
      ipt=ipt+1
      call wecs(nunit,aa,nvect,ipt)
      do 20 i=1,nvect
      aa(i)=0.
      aai(i)=0.
   20 continue
      call wecs(nunit,aa,nvect,n)
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
      cci=cai(kss)
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
      aa(k)=aa(k)+b(kk)*cc-bi(kk)*cci
      aai(k)=aai(k)+b(kk)*cci+bi(kk)*cc
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
      ab(k)=ab(k)+b(kk)*cc-bi(kk)*cci
      abi(k)=abi(k)+b(kk)*cci+bi(kk)*cc
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
      ei=0.
      ks=ii-nhold
      if(ks.le.1)go to 160
c
c        do current ca block
c
      kup=min0(ku,kl+ii-nhold-2)
      k=ks
      do 150 kk=kl,kup
      k=k-1
      e=e+b(kk)*ca(k)-bi(kk)*cai(k)
      ei=ei+b(kk)*cai(k)+bi(kk)*ca(k)
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
      e=e+b(kk)*cb(k)-bi(kk)*cbi(k)
      ei=ei+bi(kk)*cb(k)+b(kk)*cbi(k)
  190 continue
      if(kup.eq.ku)go to 195
      kl=kup+1
      go to 170
  195 continue
      aa(kss)=aa(kss)+e
      aai(kss)=aai(kss)+ei
  200 continue
      call wecs(nunit,aa,nvect,mvectb)
      khbb=khbb+ncolb
  210 continue
  220 continue
c
c        start print options
c
      nlast=0
      nhold=0
      nvectb=0
      dnorm=0.
      call load(aa,aai,ab,a,a(nc1),a(nc2),a(nc3),a(nc4),ndof,nvect,1)
      do 240 i=1,neq
      if(i.le.nlast)go to 240
      nhold=nlast
      nlast=nlast+nvect
      if(nvectb.gt.0)call wecs(lunit,ca,nvect,nvectb)
      nvectb=nvectb+1
      call recs(lunit,ca,nvect,nvectb)
      call recs(nunit,aa,nvect,nvectb)
      kl=1
      ku=min0(nvect,neq-nhold)
      do 230 kk=kl,ku
      ca(kk)=ca(kk)-aa(kk)
      cai(kk)=cai(kk)-aai(kk)
      dnorm=dnorm+ca(kk)**2+cai(kk)**2
  230 continue
  240 continue
      call wecs(lunit,ca,nvect,nvectb)
      dnorm=sqrt(dnorm)/float(neq)
      write(6,1000)dnorm
      if(kpeq2.eq.1)call write(aa,aai,b,ndof,nvect,nunit,1)
      if(kpeq3.eq.1)call write(aa,aai,b,ndof,nvect,lunit,2)
  250 continue
c
c        restore diagonal
c
      ipt=nload
      do 260 n=1,nload
      ipt=ipt+1
      call recs(nunit,aa,nvect,ipt)
      call wecs(nunit,aa,nvect,n)
      ipt=ipt+1
      call recs(nunit,aa,nvect,ipt)
      call wecs(lunit,aa,nvect,n)
  260 continue
      return
 1000 format(1h0,10x,7hnorm = ,1pe12.4)
 1010 format(1h0,10x,35hbig option on - k*u not calculated  )
      end
*deck ffin
*deck fsi
      subroutine fsi(fr,fi,a,ai,b,maxa,ncolbv,nunit,munit,nvect,maxbh,
     .               istoh)
      implicit real*8 (a-h,o-z)
      save
      dimension fr(1),fi(1),a(1),ai(1),b(1),maxa(1),ncolbv(1)
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/addb/neql,neqr,mla,nblock
      common/fsicom/fsifac,wfsi,kfsi
      common/flgs1/ibig,idamp
      if(kfsi.le.1)return
      factd=1./(fsifac*wfsi*wfsi)
      neql=1
      neqr=0
      nc=0
      nload=(neq-1)/nvect+1
      nb=0
      call recs(nunit,fr,nvect,1)
      ipt=0
      do 50 l=1,nblock
      call recs(munit,maxa,maxbh,l)
      ipt=ipt+1
      call recs(1,a,istoh,ipt)
      ipt=ipt+1
      call recs(1,ai,istoh,ipt)
      do 10 i=1,istoh
      b(i)=0.
   10 continue
      if(idamp.eq.0)go to 20
      ipt=ipt+1
      call recs(1,b,istoh,ipt)
      ipt=ipt+1
   20 continue
      ipt=ipt+2
      mla=maxa(1)-1
      neqr=neqr+ncolbv(l)
      do 40 ii=neql,neqr
      iim=ii-neql+1
      mi=maxa(iim)-mla
      nc=nc+1
      if(nc.le.nvect)go to 30
      nc=1
      nb=nb+1
      call wecs(nunit,fr,nvect,nb)
      if(nb.eq.nload)go to 50
      call recs(nunit,fr,nvect,nb+1)
   30 continue
      if(a(mi).eq.0.0.and.ai(mi).eq.0.0.and.b(mi).eq.0.)go to 40
      fr(nc)=fr(nc)*factd
      fi(nc)=fi(nc)*factd
   40 continue
      neql=neql+ncolbv(l)
   50 continue
      call wecs(nunit,fr,nvect,nload)
      return
      end
      subroutine ffin
      implicit real*8 (a-h,o-z)
      common/tapes/ntape5,ntape6
      common/flist/fdata(64),nflag(64),nword
      common/clist/cdata(64)
      character*16 edum
      character*8 cdata,wlast
      character*1 sum(80),x(13),stemp(16),blank,comma,dollar,slash,save
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
 3020 format(16a1)
 3030 format(4a1)
 3040 format(e16.0)
 3050 format(8a1)
      end
*deck gstiff
      subroutine gstiff
      implicit real*8 (a-h,o-z)
      save
      common/comtyp/itype,ngroup,ngin,nlin
      go to (10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,
     .  170,180,190,200),itype
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
      end
*deck load
      subroutine load(fr,fi,id,diag,fc,fci,lm,nd,ndof,nvect,ifg)
      implicit real*8 (a-h,o-z)
      save
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat
      common/scom/nload,ncon,nconi,nsave,ndmax,mcon
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/tapcom/nt11,nt31
      common/list/data(64),ndum
      character*8 cdata
      common/clist/cdata(64)
      dimension fr(1),fi(1),id(ndof,1)
      dimension diag(3,1),fc(1),fci(1),lm(1)
      character*8 end
      data end/'end'/
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
      fi(i)=0.
   10 continue
      if(kpload.gt.0)write(6,2000)ns
   20 continue
      call fld
      if(cdata(1).eq.end)go to 60
      nod=data(1)
      val=data(2)
      idof=data(3)
      ityp=data(4)
      if(idof.le.0)idof=1
      if(kpload.gt.0)write(6,2010)nod,val,idof,ityp
      if(nod.ge.1.and.nod.le.numnp)go to 30
      write(6,2020)
      ierr=1
      go to 20
   30 continue
      inode=id(idof,nod)
c***  ignore force card if this dof is constrained  *** goto 20 ***
      if(inode.eq.0)goto 20
      if(ityp)40,40,50
   40 continue
      fr(inode)=fr(inode)+val
      go to 20
   50 continue
      fi(inode)=fi(inode)+val
      go to 20
   60 continue
      nload=(neq-1)/nvect+1
      nloc=0
      do 80 i=1,nload
      do 70 j=1,nvect
      fc(j)=0.
      fci(j)=0.
      nloc=nloc+1
      if(nloc.gt.neq)go to 70
      fc(j)=fr(nloc)
      fci(j)=fi(nloc)
   70 continue
      call wecs(3,fc,nvect,i)
   80 continue
      do 90 i=1,nload
      call recs(3,fc,nvect,i)
      call wecs(3,fc,nvect,i+nload)
   90 continue
      if(ierr.ne.0)stop
      return
  110 continue
c
c        write load vector to random file
c
      neqr=0
      neql=1
      do 150 nl=1,nload
      call recs(3,fr,nvect,nl+nload)
      if(mcon.le.0)go to 140
      neqr=neqr+nvect
      do 130 nc=1,mcon
      call recs(7,diag,nsave,nc)
      do 120 kd=1,nd
      lmp=lm(kd)
      if(lmp.lt.neql.or.lmp.gt.neqr)go to 120
      lml=lmp-neql+1
      fr(lml)=fr(lml)+fc(kd)
      fi(lml)=fi(lml)+fci(kd)
  120 continue
  130 continue
      neql=neql+nvect
  140 continue
      call wecs(3,fr,nvect,nl)
  150 continue
      return
 2000 format(1h0,10x,63hn o d a l   p o i n t   f o r c e s   f o r   l
     .o a d   c a s e ,i3,//
     .  7x,4hnode,5x,5hvalue,4x,3hdof,1x,4htype)
 2010 format(i10,1pe12.4,2i5)
 2020 format(5x,19hstop undefined node)
      end
*deck nodep
      subroutine nodep
      implicit real*8 (a-h,o-z)
      save
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common/dtwo/itwo,ndof,idim
      common/list/data(64),ndum
      character*8 cdata
      common/clist/cdata(64)
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
      implicit real*8 (a-h,o-z)
      save
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat
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
      dimension x(1),y(1),z(1),id(ndof,1)
      dimension iprc(4)
c
      character*8 iprc,end,free,it
      data iprc /' ','a','b','c' /
      data end,free/'end','free'/
c
c
c
      nold=0
      dum=0.
      rad=atan(1.)/45.0
      knold=0
      feet=1.0
      it=iprc(1)
   10 continue
      call fld
      if (cdata(1).ne.'feet') go to 13
      feet=12.0
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
*deck open
      subroutine open(istoh,nblock,icheck)
      implicit real*8 (a-h,o-z)
      save
      integer ccreate
      external ccreate
      common/scom/nload,ncon,nconi,nsave,ndmax,mcon
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
      nsecl=(lb-1)/512+1
      nsec=nsecl*nb
      nwords=nsec*512
      write(6,2010)nt,nb,lb,nwords,nsecl,nsec
      ier=0
c     call assign(ier,'dn'l,'ft32'l,'rdm'l,'u'l)
      ier=ccreate(nt,"tape.32;")
      if(ier.ne.0)go to 90
      nt=33
      nb=nblock
      lb=maxbh
      nsecl=(lb-1)/512+1
      nsec=nsecl*nb
      nwords=nsec*512
      write(6,2010)nt,nb,lb,nwords,nsecl,nsec
c     call assign(ier,'dn'l,'ft33'l,'rdm'l,'u'l)
      ier=ccreate(nt,"tape.33;")
      if(ier.ne.0)go to 90
      nt=34
      nb=(neq-1)/nvect+1
      nb=2*nb
      lb=2*nvect*itwo
      nsecl=(lb-1)/512+1
      nsec=nsecl*nb
      nwords=nsec*512
      write(6,2010)nt,nb,lb,nwords,nsecl,nsec
c     call assign(ier,'dn'l,'ft34'l,'rdm'l,'u'l)
      ier=ccreate(nt,"tape.34;")
      if(ier.ne.0)go to 90
      nt=35
      nb=nblock
      lb=2*istoh*itwo
      nsecl=(lb-1)/512+1
      nsec=nsecl*nb
      nwords=nsec*512
      write(6,2010)nt,nb,lb,nwords,nsecl,nsec
c     call assign(ier,'dn'l,'ft35'l,'rdm'l,'u'l)
      ier=ccreate(nt,"tape.35;")
      if(ier.ne.0)go to 90
      nt=36
      nb=nblock
      lb=2*istoh*itwo
      nsecl=(lb-1)/512+1
      nsec=nsecl*nb
      nwords=nsec*512
      write(6,2010)nt,nb,lb,nwords,nsecl,nsec
c     call assign(ier,'dn'l,'ft36'l,'rdm'l,'u'l)
      ier=ccreate(nt,"tape.36;")
      if(ier.ne.0)go to 90
      nt=37
      nb=(neq-1)/nvect+1
      nb=2*nb
      lb=2*nvect*itwo
      nsecl=(lb-1)/512+1
      nsec=nsecl*nb
      nwords=nsec*512
      write(6,2010)nt,nb,lb,nwords,nsecl,nsec
c     call assign(ier,'dn'l,'ft37'l,'rdm'l,'u'l)
      ier=ccreate(nt,"tape.37;")
      if(ier.ne.0)go to 90
      if(nsave.le.0)return
      nt=38
      nb=mcon
      lb=nsave*itwo
      nsecl=(lb-1)/512+1
      nsec=nsecl*nb
      nwords=nsec*512
      write(6,2010)nt,nb,lb,nwords,nsecl,nsec
c     call assign(ier,'dn'l,'ft38'l,'rdm'l,'u'l)
      ier=ccreate(nt,"tape.38;")
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
      nsecl=(lb-1)/512+1
      nsec=nb*nsecl
      nwords=nsec*512
      write(6,2010)nt,nb,lb,nwords,nsecl,nsec
      ier=0
c     call assign(ier,'dn'l,'ft31'l,'rdm'l,'u'l)
      ier=ccreate(nt,"tape.31;")
      if(ier.ne.0)go to 90
      return
 1000 format(5x,20hassign error file = ,i5,2x,8herror = ,i5)
 2000 format(1h0,5x,4hfile,8x,2hnb,4x,6hlength,4x,6hnwords,5x,5hnsecl,
     .       6x,4hnsec)
 2010 format(6i10)
      return
      end
*deck rcon
      subroutine rcon(diag,fc,fci,lms,nds,a,cc,ss,ssi,s,lm,maxa,nd,
     .                 nk,ifg)
      implicit real*8 (a-h,o-z)
      save
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common /addb/ neql,neqr,mla,nblock
      common/scom/nload,ncon,nconi,nsave,ndmax,mcon
      common/wcom/w,isrlc
      dimension diag(3,1),fc(2),fci(1),lms(1),a(1)
      dimension cc(2,nk,1),s(1),ss(nd,nd),ssi(nd,nd)
      dimension lm(nk,1),maxa(1)
      common/comtyp/itype,ngroup,ngin,nlin
      do 100 ne=1,nume
      call echeck(lm(1,ne),nd,icode)
      if(icode.eq.1)go to 100
      ngin=1
      if(ind.eq.4)return
      kcon=lm(nk,ne)
      call recs(7,diag,nsave,kcon)
      if(ifg.gt.0)go to 20
c
c        store diagonals
c
      nds=nd
      do 10 i=1,nd
      ii=lm(i,ne)
      lms(i)=ii
      if(ii.lt.neql.or.ii.gt.neqr)go to 10
      iim=ii-neql+1
      mi=maxa(iim)-mla
      diag(kpri,i)=a(mi)
   10 continue
      call wecs(7,diag,nsave,kcon)
      go to 100
   20 continue
c
c        find largest diagonal
c
      ni=1
      w2=w**2
      d1=diag(1,1)-diag(2,1)*w2
      d2=w*diag(3,1)
      smax=d1**2+d2**2
      if(nd.le.1)go to 35
      do 30 i=2,nd
      d1=diag(1,i)-w2*diag(2,i)
      d2=w*diag(3,i)
      snew=d1**2+d2**2
      if(snew.le.smax)go to 30
      smax=snew
      ni=i
   30 continue
   35 continue
c
c        calculate constraints
c
      do 50 i=1,nd
      ccs=cc(1,i,ne)
      ccsi=cc(2,i,ne)
      do 40 j=1,nd
      ss(i,j)=ccs*cc(1,j,ne)-ccsi*cc(2,j,ne)
      ssi(i,j)=ccs*cc(2,j,ne)+ccsi*cc(1,j,ne)
   40 continue
   50 continue
      den=ss(ni,ni)**2+ssi(ni,ni)**2
      if(den.ne.0.)go to 60
      write(6,1000)ni
      stop
   60 continue
c
c        add constraints in stiffness matrix
c
      fac=10e5*sqrt(smax)/den
      k=0
      do 80 i=1,nd
      do 70 j=i,nd
      k=k+1
      if(kpri.eq.1)s(k)=ss(i,j)*fac
      if(kpri.eq.2)s(k)=ssi(i,j)*fac
   70 continue
   80 continue
      call addban(a,maxa,s,lm(1,ne),nd)
      nl=nd+1
c
c        calculate constraint load vector
c
      ccs=cc(1,nl,ne)
      ccsi=cc(2,nl,ne)
      do 90 i=1,nd
      fc(i)=ccs*cc(1,i,ne)-ccsi*cc(2,i,ne)
      fci(i)=ccs*cc(2,i,ne)+ccs*cc(1,i,ne)
      fc(i)=fc(i)*fac
      fci(i)=fci(i)*fac
   90 continue
      call wecs(7,diag,nsave,kcon)
  100 continue
      return
 1000 format(1h0,10x,36hstop zero diagonal in constraint no.,i10)
      end
*deck sblock
      subroutine sblock (maxa,ncolbv,icopl,istorl,nblock,neq,nwk,istoh)
      implicit real*8 (a-h,o-z)
      save
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
      subroutine solve(wa,iequit,ns,istres,iback)
      implicit real*8 (a-h,o-z)
      save
      common/fsicom/fsifac,wfsi,kfsi
      common/adrl/n1l,n2l,n3l,n4l,n5l,n6e
      common/adrnc/nc1,nc2,nc3,nc4
      common/scom/nload,ncon,nconi,nsave,ndmax,mcon
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/tcom/munit,lunit,nstif,nred,nunit
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/ncom/numest,nvect,istoh,maxbh
      common/adr/n1,n2,n3,n4,n5,n6,n7,n8
      common /addb/ neql,neqr,mla,nblock
      common/dtwo/itwo,ndof,idim
      common/list/data(64),ndum
      character*8 cdata
      common/clist/cdata(64)
      common/wcom/w,isrlc
      dimension ia(1)
      common a(1)
      real a
      integer ia
      equivalence (a(1),ia(1))
      character*8 end
      data end/'end'/
c
c        setup solution storage
c
      if(ns.gt.1)go to 50
      n3=n2+nvect*itwo
      n4=n3+nvect*itwo
      n5=n4+nvect*itwo
      n6=n5+nvect*itwo
      if(nvect.ne.neq)go to 10
      n6=n4
      n4=n2
      n5=n3
   10 continue
      n7=n6+istoh*itwo
      n8=n7+istoh*itwo
      n9=n8+istoh*itwo
      n10=n9+istoh*itwo
      if(nblock.ne.1.or.kfsi.eq.2)go to 20
      n10=n8
      n8=n6
      n9=n7
   20 continue
      n11=n10+nvect*itwo
      n12=n11+nvect*itwo
      n13=n12+nvect*itwo
      n14=n13+nvect*itwo
      if(nvect.ne.neq)go to 30
      n14=n12
      n12=n10
      n13=n11
   30 continue
      n15=n14+numest
      n5l=n4l+nblock
      idl=ndof*numnp
      if(n4+idl.gt.n15)n15=n4+idl
      min=nmax+n15-n1l+1
      write(6,2110)min
      if(min.le.nmax)go to 40
      stop
   40 continue
   50 continue
c
c        frequency sweep loop
c
      w=wa
      ind=2
      wfsi=w
      if(wfsi.eq.0.)wfsi=1.e-10
      n6e=n14
      if(icheck.eq.1)go to 70
      if(iback.eq.1)go to 60
      call assem1(a(n8),a(n6),a(n7),a(n14),a(n1l),a(n3l),a(n5l),
     .   istoh,maxbh,
     .           munit,1)
      call colsol(a(n1l),a(n2l),a(n3l),a(n4l),a(n2),a(n3),a(n4),a(n5),
     .            a(n6),a(n7),a(n8),a(n9),a(n10),a(n11),a(n12),a(n13),
     .            nblock,neq,istoh,nvect,nvect,maxbh,nstif,nred,
     .            lunit,munit,nunit,1)
   60 continue
      call load(a(n2),a(n3),a(n4),a(n1),a(nc1),a(nc2),a(nc3),a(nc4),
     .          ndof,nvect,1)
      call colsol(a(n1l),a(n2l),a(n3l),a(n4l),a(n2),a(n3),a(n4),a(n5),
     .            a(n6),a(n7),a(n8),a(n9),a(n10),a(n11),a(n12),a(n13),
     .            nblock,neq,istoh,nvect,nvect,maxbh,nstif,nred,
     .            lunit,munit,nunit,2)
      if(iequit.ne.0)call equit(a(n2),a(n3),a(n4),a(n5),a(n6),a(n7),
     .                          a(n10),a(n11),a(n12),a(n13),a(n1l),
     .                          a(n3l),a(n1),neq,istoh,nblock,nvect,
     .                          maxbh,munit,nstif,lunit,nunit)
      call fsi(a(n2),a(n3),a(n6),a(n7),a(n8),a(n1l),a(n3l),lunit,munit,
     .         nvect,maxbh,istoh)
      call write(a(n2),a(n3),a(n6),ndof,nvect,lunit,0)
   70 continue
      if (istres.eq.0) go to 125
      n3s=n3
      n6s=n6
      n3=n2+itwo*neq
      n6=n3+itwo*neq
      n4s=n6+itwo*nvect
      n5s=n4s+itwo*nvect
      nlast=n5s+ndof*numnp
      min=nmax+nlast-n3l+1
      write(6,2290) min
      if (min.le.nmax) go to 110
      stop
  110 nlast=n6+numest
      n6e=n6
      min=nmax+nlast-n3l+1
      write(6,2295) min
      if (min.le.nmax) go to 120
      stop
c
  120 call stress(a(n2),a(n3),a(n6),a(n4s),a(n5s),ndof,nvect,lunit)
      n3=n3s
      n6=n6s
  125 continue
      return
 2110 format(1h0,10x,39hstorage requested for solution phase =  ,i10)
 2290 format(1h0,10x,42hstorage to store entire solution vector =    ,
     .       i10)
 2295 format(1h0,10x,29hstorage to compute stresses = ,i10)
      end
*deck stress
      subroutine stress(ur,ui,fr,fi,id,ndof,nvect,nunit)
      implicit real*8 (a-h,o-z)
      save
c
      common /flgs/ kpri,ind,nummat,midest,ng,icheck
      common /number/ nmax,numnp,nume,neq,nwk,ma
      common /tapcom/ nt11,nt31
      common /comtyp/ itype,ngroup,ngin,nlin
c
      dimension ur(1),ui(1),fr(1),fi(1),id(ndof,1)
c
      if(icheck.eq.1)return
      rewind 8
      read(8) ((id(i,j),i=1,ndof),j=1,numnp)
c
      m=0
      nhold=0
      nlast=0
      nvectb=0
      do 30 n=1,numnp
      do 20 i=1,ndof
      ip=id(i,n)
      if (ip.le.0) go to 20
      m=m+1
      if (m.le.nlast) go to 15
      nhold=nlast
      nlast=nlast+nvect
      nvectb=nvectb+1
      call recs(nunit,fr,nvect,nvectb)
   15 continue
      ur(ip)=fr(ip-nhold)
      ui(ip)=fi(ip-nhold)
   20 continue
   30 continue
c
      ind=3
      kpri=1
      do 85 i=1,ng
      call eread(fr,i)
      call gstiff
   85 continue
      ind=2
      return
      end
*deck wecs
      subroutine wecs(nt,a,in,nb)
      implicit real*8 (a-h,o-z)
      save
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
c vax if(ifg)write(nt33'nb)(a(i),i=1,in)
c vax if(.not.ifg)read(nt33'nb)(a(i),i=1,in)
      return
   50 continue
      iw=2*iw
      if(ifg)call writms(nt34,a,iw,nb,-1,0)
      if(.not.ifg)call readms(nt34,a,iw,nb)
c vax if(ifg)write(nt34'nb)(a(i),i=1,iw)
c vax if(.not.ifg)read(nt34'nb)(a(i),i=1,iw)
      return
   70 continue
      iw=iw*2
   75 if(ibig.eq.2)go to 10
      if(ifg)call writms(nt35,a,iw,nb,-1,0)
      if(.not.ifg)call readms(nt35,a,iw,nb)
      return
   90 continue
      iw=iw*2
      if(ibig.ne.0)go to 75
      if(ifg)call writms(nt36,a,iw,nb,-1,0)
      if(.not.ifg)call readms(nt36,a,iw,nb)
c vax if(ifg)write(nt36'nb)(a(i),i=1,iw)
c vax if(.not.ifg)read(nt36'nb)(a(i),i=1,iw)
      return
  110 continue
      iw=iw*2
      if(ifg)call writms(nt37,a,iw,nb,-1,0)
      if(.not.ifg)call readms(nt37,a,iw,nb)
c vax if(ifg)write(nt37'nb)(a(i),i=1,iw)
c vax if(.not.ifg)read(nt37'nb)(a(i),i=1,iw)
      return
  130 continue
      if(ifg)call writms(nt38,a,iw,nb,-1,0)
      if(.not.ifg)call readms(nt38,a,iw,nb)
c vax if(ifg)write(nt38'nb)(a(i),i=1,iw)
c vax if(.not.ifg)read(nt38'nb)(a(i),i=1,iw)
      return
c
      entry recs(nt,a,in,nb)
c
      iw=in*itwo
      ifg=.false.
      go to (10,30,50,70,90,110,130),nt
      end
*deck write
      subroutine write(fr,fi,id,ndof,nvect,nunit,ifg)
      implicit real*8 (a-h,o-z)
      save
      common/number/nmax,numnp,nume,neq,nwk,ma
      common/pntcom/kpnode,kpdof,kpelem,kpload,kpcomp,kpeq1,
     .              kpeq2,kpeq3,kpstre,kpdis,kpmat
      common/wcom/w,isrlc
      dimension fr(1),fi(1),id(ndof,1)
      dimension dxr(6),dxi(6)
      dimension drc(10),dic(10)
c
c        print solution
c
      rewind 8
      read(8)((id(i,j),i=1,ndof),j=1,numnp)
      if(ifg.le.0)write(6,1000)w
      if(ifg.eq.1)write(6,1030)w
      if(ifg.eq.2)write(6,1040)w
      kount=0
      kend=(10/ndof)*ndof
      m=0
      nhold=0
      nlast=0
      nvectb=0
      do 30 n=1,numnp
      do 20 i=1,ndof
      ip=id(i,n)
      dxr(i)=0.
      dxi(i)=0.
      if(ip.le.0)go to 20
      m=m+1
      if(m.le.nlast)go to 15
      nhold=nlast
      nlast=nlast+nvect
      nvectb=nvectb+1
      call recs(nunit,fr,nvect,nvectb)
   15 continue
      dxr(i)=fr(ip-nhold)
      dxi(i)=fi(ip-nhold)
   20 continue
      if(kpcomp.eq.1)go to 25
      write(6,1010)n,(dxr(i),i=1,ndof)
      write(6,1020)(dxi(i),i=1,ndof)
      go to 30
   25 continue
      do 26 i=1,ndof
      kount=kount+1
      drc(kount)=dxr(i)
      dic(kount)=dxi(i)
   26 continue
      if(kount.lt.kend.and.n.lt.numnp)go to 30
      write(6,1050)n,(drc(i),i=1,kount)
      write(6,1060)(dic(i),i=1,kount)
      kount=0
   30 continue
      return
 1000 format(1h0,10x,49hn o d a l   p o i n t   d i s p l a c e m e n t
     .s ,5x,6hw   = ,f14.4,///)
 1010 format(i5,5x,1p6e12.4)
 1020 format(10x,1p6e12.4)
 1030 format(1h0,10x,63hc a l c u l a t e d  n o d a l   p o i n t  f o
     .r c e s         ,4hw = ,f14.4,//)
 1040 format(1h0,10x,59hr e s i d u a l  n o d a l  p o i n t  f o r c e
     . s         ,4hw = ,f14.4,//)
 1050 format(i8,1p10e11.3)
 1060 format(8x,1p10e11.3)
      end
*deck writms
      subroutine writms(iunit,a,iword,irec,idum1,idum2)
      implicit real*8 (a-h,o-z)
      save
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
      save
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
*deck fld
      subroutine fld
      implicit real*8 (a-h,o-z)
      save
      common/list/data(64),nword
      character*8 cdata
      common/clist/cdata(64)
      data nt11/11/
      read(11)nw,(data(i),i=1,nw),(cdata(i),i=1,nw)
      nwp=nw+1
      do 10 i=nwp,64
      data(i)=0.
      cdata(i)=' '
   10 continue
      return
      end
*deck preaem
      subroutine preaem(ntable,ke,ncolbv,ee,nblock)
      implicit real*8 (a-h,o-z)
      save
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
      implicit real*8 (a-h,o-z)
      save
      common/flgs/kpri,ind,nummat,midest,ng,icheck
      common/number/nmax,numnp,nume,new,nwk,ma
      common/comtyp/itype,ngroup,ngin,nlin
      common/dtwo/itwo,ndof,idim
      common/ewcom/ia(2,200)
      dimension e(4)
      data nsec/512/
      nw=midest+4*itwo
      midest=nw
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
      entry eread(e,n)
      nw=ia(1,n)
      isec=ia(2,n)
      call hsior(31,e,nw,isec*nsec)
      nume=e(1)
      nummat=e(2)
      itype=e(3)
      nwt=e(4)
      if(nwt.eq.nw)return
      write(6,1000)nw,nwt
      stop
 1000 format(1h0,10x,15herror in ewrite ,2i10)
      end
*deck hsiow
      subroutine hsiow(iunit,a,iword,npos)
      implicit real*8 (a-h,o-z)
      save
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
      save
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
