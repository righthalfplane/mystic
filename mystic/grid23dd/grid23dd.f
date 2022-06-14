*deck post
      subroutine post()
      implicit real*8 (a-h,o-z)
c     program post(input,output,tape5=input,tape6=output,d3plot,
c     . d3thdt,tape19=d3thdt,tape33,tape43,tape59=tty,tape12=d3plot)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata,word,card
      dimension card(10),ia(2),ad(2)
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      common/dcom2/mini,minj,mink,maxi,maxj,maxk
      common/matcom/area(50),volume(50),weight(50),density(50),
     .              thickness(50),blength(50)
      character*8 head
      common/comh/head(10)
      common/size/nsize,n0,n4,n5
      common/pcom/iprint,itype
      common/cequal/nequal,kequal(2100)
      common/tapes/ntape5,ntape6,ntape1,ntape2,ntape3,ntape4,ntape52
      common/dumcom/dumval
      common a(10)
      real a
      equivalence (a(1),ia(1))
      equivalence (a(1),ad(1))
      itwo=2
      do 5 i=1,50
      area(i)=0.0
      volume(i)=0.0
      weight(i)=0.0
      density(i)=0.0
      thickness(i)=0.0
      blength(i)=0.0
    5 continue
      call bgnplt('tape43')
c
c        command loop
c
      nfam=1
      icheck=0
      ntape5=55
      ntape6=67
      dumval=-1.23e-21
      open(unit=55,file='input',status='old')
      open(unit=67,file='output',status='unknown')
      open(unit=33,file='tape33',status='unknown')
c      open(unit=43,file='tape43',status='unknown',form='unformatted')
 
   10 continue
      call ffin
      word=cdata(1)
      if(word.eq.'stop')go to 900
      if(word.eq.'setup')go to 20
      if(word.eq.'w')go to 80
      if(word.eq.'slide')go to 120
      if(word.eq.'surface')go to 125
      if(word.eq.'press')go to 130
      if(word.eq.'pressd2')go to 490
      if(word.eq.'fill')go to 140
      if(word.eq.'siminf')go to 150
      if(word.eq.'print')iprint=fdata(2)
      if(word.eq.'vel')itype=1
      if(word.eq.'elements')go to 190
      if(word.eq.'tie')go to 250
      if(word.eq.'tie2')go to 255
      if(word.eq.'tell')go to 280
      if(word.eq.'equal')go to 290
      if(word.eq.'equal2')go to 295
      if(word.eq.'press2')go to 300
      if(word.eq.'shell')go to 320
      if(word.eq.'velocity')go to 105
      if(word.eq.'beam')go to 330
      if(word.eq.'print')go to 340
      if(word.eq.'mass')go to 350
      if(word.eq.'merge2')go to 360
      if(word.eq.'merge')go to 370
      if(word.eq.'slave')go to 380
      if(word.eq.'noded2')go to 400
      if(word.eq.'elemd2')go to 410
      if(word.eq.'slided2')go to 420
      if(word.eq.'nodet3')go to 430
      if(word.eq.'elemt3')go to 440
      if(word.eq.'nodet2')go to 450
      if(word.eq.'elemt2')go to 460
      if(word.eq.'node3')go to 470
      if(word.eq.'brick')go to 480
      if(word.eq.'material')go to 500
      if(word.eq.'tied')go to 510
      if(word.eq.'tied2')go to 515
      if(word.eq.'velbc')go to 520
      if(word.eq.'mat')go to 530
      if(word.eq.'nodes')go to 550
      if(word.eq.'elemm')go to 560
      if(word.eq.'press')go to 570
      if(word.eq.'load')go to 580
      if(word.eq.'nodes3d')go to 590
      if(word.eq.'elemm3')go to 600
      if(word.eq.'load3')go to 620
      if(word.eq.'family')then
        nfam=fdata(2)
        goto 10
      endif
      go to 10
   20 continue
      iprint=fdata(2)
      if(iprint.le.0)iprint=4
      imax=fdata(3)
      if(imax.le.0)imax=20
      jmax=fdata(4)
      if(jmax.le.0)jmax=20
      kmax=fdata(5)
      if(kmax.le.0)kmax=20
      kskip=imax*jmax*kmax*itwo
      nmax=5*kskip+10000000
      write(6,1000)imax,jmax,kmax
      call memreq(nmax)
      n0=1
      n1=n0+kskip*itwo
      n2=n1+kskip*itwo
      n3=n2+kskip*itwo
      n4=n3+kskip*itwo
      n5=n4+kskip*itwo
      nsize=n5
      call mesh3d(a(n1),a(n2),a(n3),a(n4),imax,jmax,kmax,iprint)
      iprint=0
      do 40 i=1,kskip
      ia(n0+i-1)=i
   40 continue
      go to 10
   80 if((cdata(2).eq.'elements').or.(cdata(2).eq.'nodes'))go to 100
c
c         read and write data cards
c
   90 continue
      read(ntape5,1020)card
      if(card(1).eq.'exit')go to 10
      write(33,1020)card
      go to 90
  100 if(cdata(2).ne.'nodes')go to 110
      call wnodes(a(n1),a(n2),a(n3),a(n4),imax,jmax)
      go to 10
  105 continue
      call wvel(a(n1),a(n2),a(n3),a(n4),imax,jmax)
      go to 10
  110 if(cdata(2).ne.'elements')go to 10
      call welem(a(n1),a(n2),a(n3),a(n4),imax,jmax)
      go to 10
  120 continue
      call slide(a(n1),a(n2),a(n3),a(n4),imax,jmax)
      go to 10
  125 continue
      call surface(a(n1),a(n2),a(n3),a(n4),imax,jmax)
      go to 10
  130 continue
      call press(a(n1),a(n2),a(n3),a(n4),imax,jmax)
      go to 10
  140 continue
      call fill2(a(n1),a(n2),a(n3),a(n4),imax,jmax)
      go to 10
  150 continue
      call siminf(a(n1),a(n2),a(n3),a(n4),imax,jmax)
      go to 10
  190 continue
      do 200 i=1,nmax
      ia(i)=0
  200 continue
      call elem(a(n1),imax,jmax)
      go to 10
  250 continue
      call tie
      go to 10
  255 continue
      call tie2
      go to 10
  280 continue
      call tell(a(n1),imax,jmax)
      go to 10
  290 continue
      call equal(a(n1))
      go to 10
  295 continue
      call equal2(a(n1))
      go to 10
  300 continue
      call press2(a(n1),a(n2),a(n3))
      go to 10
  320 continue
      call shell(a(n1),a(n2),a(n3),a(n4),imax,jmax)
      go to 10
  330 continue
      call beam(a(n1),a(n2),a(n3),a(n4),imax,jmax)
      go to 10
  340 continue
      call print(a(n1),a(n2),a(n3),imax,jmax)
      go to 10
  350 continue
      call mass(a(n1),a(n2),a(n3),imax,jmax)
      go to 10
  360 continue
      call merge2(a(n1))
      go to 10
  370 continue
      call merge1(a(n1))
      go to 10
  380 continue
      call slave
      go to 10
  400 continue
      call noded2(a(n1),a(n2),a(n3),a(n4),imax,jmax)
      go to 10
  410 continue
      call elemd2(a(n1),a(n2),a(n3),a(n4),imax,jmax)
      go to 10
  420 continue
      call slid2
      go to 10
  430 continue
      call nodet3(a(n1),a(n2),a(n3),a(n4),imax,jmax)
      go to 10
  440 continue
      call elemt3(a(n1),a(n2),a(n3),a(n4),imax,jmax)
      go to 10
  450 continue
      call nodet2(a(n1),a(n2),a(n3),a(n4),imax,jmax)
      go to 10
  460 continue
      call elemt2(a(n1),a(n2),a(n3),a(n4),imax,jmax)
      go to 10
  470 continue
      call node3(a(n1),a(n2),a(n3),a(n4),imax,jmax)
      go to 10
  480 continue
      call brick(a(n1),a(n2),a(n3),a(n4),imax,jmax)
      go to 10
  490 continue
      call pressd2
      go to 10
  500 continue
      nmat=fdata(2)
      if(nmat.le.0.or.nmat.gt.50)then
        write(ntape6,*)' material ',nmat,' out of range'
        go to 10
      endif
      density(nmat)=fdata(3)
      thickness(nmat)=fdata(4)
      area(nmat)=fdata(5)
      write(ntape6,1030)nmat,density(nmat),thickness(nmat),area(nmat)
      go to 10
  510 continue
      call tied
      go to 10
  515 continue
      call tied2
      go to 10
  520 continue
      call velbc(a(n1),a(n2),a(n3),a(n4),imax,jmax)
      go to 10
  530 continue
      nmat=fdata(2)
      n6=n5+64*nmat
      if(n6.le.nmax)go to 540
      write(6,1040)n6,nmax
      stop
  540 continue
      call gmat(a(n5),nmat)
      go to 10
  550 continue
      call pnodes(a(n1),a(n2),a(n3),a(n4),imax,jmax)
      go to 10
  560 continue
      call pquad(a(n1),a(n2),a(n3),a(n5),nmat,imax,jmax)
      call look(a(n4),a(n3),imax*jmax)
      go to 10
  570 continue
      call pressa(a(n1),a(n2),a(n3),a(n4),imax,jmax)
      go to 10
  580 continue
      call load(a(n3),a(n4),imax,jmax)
      go to 10
  590 continue
      call wnodes3(a(n1),a(n2),a(n3),a(n4),imax,jmax)
      go to 10
  600 continue
      call elemm3(a(n1),a(n2),a(n3),a(n4),a(n5),nmat,imax,jmax)
      go to 10
  620 continue
      call load3(a(n2),a(n3),a(n4),imax,jmax,kmax)
      go to 10

  900 continue
c      if(icheck.eq.0)call check(a(n1),a(n4))
      call carea()
      call endplt()
      call doneplt()
      call exit(0)
 1000 format(1h0,9x,' imax = ',i5,' jmax = ',i5,
     .       ' kmax = ',i5)
 1010 format(8a10)
 1020 format(10a8)
 1030 format(' material = ',i5,' density = ',1pe12.4,' thickness = ',
     .       1pe12.4,' area = ',1pe12.4)
 1040 format(' stop need at least ',i8,' words nmax ',i8)
      end 
      subroutine load3(fx,fy,fz,imax,jmax,kmax)
      implicit real*8 (a-h,o-z)
      double precision data
      character*8 wend,wload,owend,swload
      character*8 bb
      dimension fx(1),fy(1),fz(1)
      common/flist/data(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
      common/pcom/iprint,itype
      common/prcom/twopi,ipress
      common/outcom/itout
      integer*4 lj
      data wload,wend/'load','end'/
      data owend/'end'/
      data bb/'        '/
      data swload/'load'/
      nodes=imax*jmax*kmax
      iprint=0
      twopi=1.0
      if(ipress.eq.1)go to 20
      ipress=1
      do 10 n=1,nodes
      fx(n)=0.
      fy(n)=0.
      fz(n)=0.
   10 continue
   20 continue
      call ffin
      if(cdata(1).eq.wend)go to 50
      px=data(1)
      py=data(2)
      pz=data(3)
      i1=data(4)
      j1=data(5)
      k1=data(6)
      i2=data(7)
      j2=data(8)
      k2=data(9)
      do 45 k=k1,k2
      do 40 j=j1,j2
      do 30 i=i1,i2
      n=nijk(i,j,k)
      fx(n)=px
      fy(n)=py
      fz(n)=pz
c      write(6,*)n,px,py,pz
   30 continue
   40 continue
   45 continue
      go to 20
   50 continue
      nl=0
      z1=1
      n1=1
      z2=2
      n2=2
      n3=3
      z3=3
      write(33,2000)wload
 2000 format(a9)
      do 80 i=1,nodes
      zi=i
      if(fx(i).eq.0)go to 60
      fx(i)=twopi*fx(i)
      nl=nl+1
      write(33,2010)i,fx(i),n1
 2010 format(i8,1pe16.8,i3)
      if(iprint.eq.1)write(6,1020)i,fx(i),z1
   60 if(fy(i).eq.0)go to 70
      fy(i)=twopi*fy(i)
      nl=nl+1
      write(33,2010)i,fy(i),n2
      if(iprint.eq.1)write(6,1020)i,fy(i),z2
   70 if(fz(i).eq.0)go to 80
      fz(i)=twopi*fz(i)
      nl=nl+1
      write(33,2010)i,fz(i),n3
      if(iprint.eq.1)write(6,1020)i,fz(i),z3    
   80 continue
      write(6,1000)nl
      write(33,2000)wend
      return
 1000 format(1h0,9x,'number of loads     = ',i6)
 1020 format(' n = ',i5,' f = ',1pe12.4,' t = ',1pe12.4)
      end            
      subroutine elemm3(xd,yd,zd,ibc,smat,nmat,in,jn)
      implicit real*8 (a-h,o-z)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata,hex8
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      dimension ipl(5,4),xd(1),yd(1),zd(1),ibc(1)
      dimension xb(20),yb(20),zb(20),num(8),smat(64,1)
      dimension ii(8),jj(8),kk(8),ni(8),nj(8),nk(8)
      data ipl/1,2,3,4,1, 5,6,7,8,5, 1,2,6,5,1, 4,3,7,8,4/
      data ni/0,1,1,0, 0,1,1,0/
      data nj/0,0,1,1, 0,0,1,1/
      data nk/0,0,0,0, 1,1,1,1/
      data hex8/'hex8'/

      nel=0
      load=fdata(2)
   10 continue
      call ffin
      if(cdata(1).eq.'end')go to 900
      if(cdata(1).eq.'family')then
        nfam=fdata(2)
        goto 10
      endif
      mat=fdata(2)
      if(mat.ge.1.and.mat.le.nmat)go to 15
      write(6,1000)mat
      mat=1
   15 continue
      if(fdata(6).le.0.)go to 30
      k=3
      do 20 i=1,8
      ii(i)=fdata(k)
      jj(i)=fdata(k+1)
      kk(i)=fdata(k+2)
      k=k+3
   20 continue
      go to 50
   30 continue
      im=fdata(3)
      jm=fdata(4)
      km=fdata(5)
      do 40 i=1,8
      ii(i)=im+ni(i)
      jj(i)=jm+nj(i)
      kk(i)=km+nk(i)
   40 continue
   50 continue
      call ffin
      is=fdata(1)
      if(is.le.0)is=1
      js=fdata(2)
      if(js.le.0)js=1
      ks=fdata(3)
      if(ks.le.0)ks=1
      ie=fdata(4)
      if(ie.le.is)ie=is
      je=fdata(5)
      if(je.lt.js)je=js
      ke=fdata(6)
      if(ke.lt.ks)ke=ks
      inc=fdata(7)
      if(inc.le.0)inc=1
      jnc=fdata(8)
      if(jnc.le.0)jnc=1
      knc=fdata(9)
      if(knc.le.0)knc=1
      incc=1
      nnel=0
      do 70 kl=ks,ke,knc
      do 70 jl=js,je,jnc
      do 70 il=is,ie,inc
      nnel=nnel+1
   70 continue      
      nel=0
      write(33,2000)hex8,nnel,nmat,load
 2000 format(a9,i8,i3,i3)  
      do 75 i=1,nmat
      write(33,2010)(dble(smat(j,i)),j=1,62)
 2010 format(4(1pe16.8),'/')  
c      write(33,2015)(dble(smat(j,i)),j=62,63)
 2015 format(1pe16.8,1pe16.8)  
   75 continue
      do 130 kl=ks,ke,knc
      do 120 jl=js,je,jnc
      do 110 il=is,ie,inc
      do 80 i=1,8
      num(i)=nijk(ii(i)+il-is,jj(i)+jl-js,kk(i)+kl-ks)
c      write(6,*)i,num(i),ii(i)+il-is,jj(i)+jl-js,kk(i)+kl-ks
      ibc(num(i))=1
   80 continue
      nel=nel+1
      write(33,1000)nel,mat,
     .  (num(m),m=1,8)
      n=0
      do 100 k3=1,4
      do 90 k2=1,5
      n=n+1
      ip=ipl(k2,k3)
      xb(n)=xd(num(ip))
      yb(n)=yd(num(ip))
      zb(n)=zd(num(ip))
   90 continue
  100 continue
      call line3d(xb,yb,zb,20,5,nfam)
  110 continue
  120 continue
  130 continue
      go to 10
  900 continue
      numel=nel
      write(6,1010)numel
      return
 1000 format(4(i8),"/")
 1010 format(5x,'mystic elements written = ' ,i8)
      end      
      
      subroutine wnodes3(xd,yd,zd,ibc,in,jn)
      implicit real*8 (a-h,o-z)
      dimension xd(in,jn,*),yd(in,jn,*),zd(in,jn,*),ibc(in,jn,*)
      common/dumcom/dumval
      dimension word(6)
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      common/size/nsize,n0,n4,n5
      common/flist/data(256),nflag(256),nword
      common/clist/cdata(256)
      common ia(10)
      character*8 cdata,word,fix,wnode,wend
      data fix/'fix'/
      data wnode/'nodes'/
      data wend/'end'/
      knode=imax*jmax*kmax
      write(33,2000)wnode,knode,3,3
 2000 format(a9,i8,i3,i3)
      do 30 k=1,kmax
      do 20 j=1,jmax
      do 10 i=1,imax
      ibc(i,j,k)=0
   10 continue
   20 continue
   30 continue
c
c          loop until end card
c
   40 continue
      call ffin
      if(cdata(1).eq.'end')go to 80
      n=data(1)
      i1=data(2)
      j1=data(3)
      k1=data(4)
      i2=data(5)
      j2=data(6)
      k2=data(7)
      if(i2.lt.i1)i2=i1
      if(j2.lt.j1)j2=j1
      if(k2.lt.k1)k2=k1
      do 70 k=k1,k2
      do 60 j=j1,j2
      do 50 i=i1,i2
      ibc(i,j,k)=n
   50 continue
   60 continue
   70 continue
      go to 40
   80 continue
      nlast=0
      do 110 k=1,kmax
      do 100 j=1,jmax
      do 90 i=1,imax
      nlast=nlast+1
      write(33,1000)nlast,xd(i,j,k),yd(i,j,k),zd(i,j,k)
   90 continue
  100 continue
  110 continue
      nauto=0
      nlast=0
      do 130 k=1,kmax
      do 120 j=1,jmax
      do 115 i=1,imax
      nlast=nlast+1
      ib1=ibc(i,j,k)
      if(xd(i,j,k).ne.dumval)go to 112
      nauto=nauto+1
      ib1=7
  112 continue
      if(ib1.gt.0)then
        write(33,1020)fix,ib1,nlast
      endif
      ibc(i,j,k)=0
  115 continue
  120 continue
  130 continue
      write(6,1010)nlast,nauto
      write(33,1020)wend
      return
 1000 format(i8,1pe16.8,1pe16.8,1pe16.8)
 1010 format(5x,'number of nodes written = ',i6,' auto b.c. = ',i8)
 1020 format(a9,i3,i8)
      end
     
      subroutine load(fx,fy,imax,jmax)
      implicit real*8 (a-h,o-z)
      double precision data
      character*8 wend,wload,owend,swload
      character*8 bb
      dimension fx(1),fy(1)
      common/flist/data(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
      common/pcom/iprint,itype
      common/prcom/twopi,ipress
      common/outcom/itout
      integer*4 lj
      data wload,wend/'load','end'/
      data owend/'end'/
      data bb/'        '/
      data swload/'load'/
      nodes=imax*jmax
      iprint=0
      twopi=1.0
      if(ipress.eq.1)go to 20
      ipress=1
      do 10 n=1,nodes
      fx(n)=0.
      fy(n)=0.
   10 continue
   20 continue
      call ffin
      if(cdata(1).eq.wend)go to 50
      px=data(1)
      py=data(2)
      i1=data(3)
      j1=data(4)
      i2=data(5)
      j2=data(6)
      do 40 j=j1,j2
      do 30 i=i1,i2
      n=njk(i,j,imax,jmax)
      fx(n)=px
      fy(n)=py
c      write(6,*)n,px,py
   30 continue
   40 continue
      go to 20
   50 continue
      nl=0
      z1=1
      n1=1
      z2=2
      n2=2
      n3=3
      write(33,2000)wload
 2000 format(a9)
      do 70 i=1,nodes
      zi=i
      if(fx(i).eq.0)go to 60
      fx(i)=twopi*fx(i)
      nl=nl+1
      write(33,2010)i,fx(i),n1
 2010 format(i8,1pe16.8,i3)
      if(iprint.eq.1)write(6,1020)i,fx(i),z1
   60 if(fy(i).eq.0)go to 70
      fy(i)=twopi*fy(i)
      nl=nl+1
      write(33,2020)i,fy(i),n2
 2020 format(i8,1pe16.8,i3)
      if(iprint.eq.1)write(6,1020)i,fy(1),z2
   70 continue
      write(6,1000)nl
      write(33,2000)wend
      return
 1000 format(1h0,9x,'number of loads     = ',i6)
 1020 format(' n = ',i5,' f = ',1pe12.4,' t = ',1pe12.4)
      end      
      subroutine pressa(x,y,fx,fy,ni,nj)
      implicit real*8 (a-h,o-z)
      double precision data
      character*8 wend
      dimension fx(1),fy(1),x(ni,1),y(ni,1),x2(3),y2(3),wgtl(3),
     .          xgl(3),vec(6),n(3),p(6)
      common/info/nmax,nodes,nume,nmat,ijd
      common/flist/data(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
      common/prcom/twopi,ipress
      common/pcom/iprint,itype
      data wend/'end'/
      data xgl/
     . -.7745966692415d0, .0000000000000d0, .7745966692415d0/
      data wgtl/
     .  .5555555555556d0, .8888888888889d0, .5555555555556d0/
      ipress=1
      if(iprint.gt.0)write(6,1000)
      do 10 nn=1,nodes
      fx(nn)=0.
      fy(nn)=0.
   10 continue
      itype=data(2)
      thick=data(3)
      if(thick.le.0.)thick=1.
      node=2
   20 continue
      call ffin
      if(cdata(1).eq.wend)return
      p1=data(1)
      p2=p1
      i1=data(2)
      j1=data(3)
      i2=data(4)
      j2=data(5)
      nv=2*node
      ip=1
      jp=0
      if(i1.ne.i2)go to 25
      ip=0
      jp=1
   25 continue
      do 100 jj=j1,j2
      do 90 ii=i1,i2
      nn=njk(ii,jj,ni,nj)
      n(1)=nn
      x2(1)=x(ii,jj)
      y2(1)=y(ii,jj)
      nn=njk(ii+ip,jj+jp,ni,nj)
      n(2)=nn
      x2(2)=x(ii+ip,jj+jp)
      y2(2)=y(ii+ip,jj+jp)
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
      fx(nn)=fx(nn)+p(2*i-1)
      fy(nn)=fy(nn)+p(2*i)
   80 continue
   90 continue
  100 continue
      go to 20
 1000 format(1h0,10x,27hp r e s s u r e   l o a d s,//,
     .  5x,2hp1,10x,2hp2,8x,2hn1,3x,2hn2,3x,2hn3,1x,4hnrep,2x,3hinc,
     .  2x,4htype,3x,5hthick)
 1010 format(1p2e12.4,6i5,1pe12.4)
      end 
      subroutine pre2d (r,vec,y,z,thick,pi,pj,itype,nodes)
      implicit real*8 (a-h,o-z)
      dimension vec(1),h(3),g(2),hr(3),y(3),z(3)
c
      g(1) = 0.5d0*(1.0d0+r)
      g(2) = 0.5d0*(1.0d0-r)
c
      if(nodes.gt.2) go to 10
c
      h(1) = g(1)
      h(2) = g(2)
c
      hr(1) = 0.5d0
      hr(2) =-0.5d0
c
      go to 20
c
   10 h(1) = 0.5d0*r*(1.0d0+r)
      h(2) =-0.5d0*r*(1.0d0-r)
      h(3) = 1.0d0-r*r
c
      hr(1)= 0.5d0+r
      hr(2) =-0.5d0+r
      hr(3)=-2.0d0*r
c
   20 x1 = thick
c
      if (itype.gt.0) go to 40
c
      x1 = 0.0d0
      do 30 k=1,nodes
   30 x1 = x1 + y(k)*h(k)
c
   40 yr = 0.0d0
      zr = 0.0d0
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
      subroutine look(nsave,ncon,knode)
      implicit real*8 (a-h,o-z)
      dimension ncon(1),nsave(1)
      n=0
      do 10 i=1,knode
      if(ncon(i).ne.0)go to 10
      n=n+1
      nsave(n)=i
   10 continue
      if(n.le.0)return
      write(6,1000)(nsave(i),i=1,n)
      return
 1000 format(1h0,9x,'nodes not connected',/,(20i5))
      end
      function njk(j,k,jmax,kmax)
      common/ercom/ierror
      if(j.le.jmax.and.k.le.kmax)go to 5
      ierror=ierror+1
      j=1
      k=1
    5 if(jmax.gt.kmax)go to 10
      njk=j+(k-1)*jmax
      return
   10 continue
      njk=k+(j-1)*kmax
      return
      end
      subroutine pquad(xn,yn,ncon,smat,nmat,ni,nj)
      implicit real*8 (a-h,o-z)
      double precision data
      character*8 wend,wiload,word,wquad,star,swquad
      character*8 bb
      common/flist/data(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
      dimension xn(ni,1),yn(ni,1),ncon(1),smat(64,1)
      integer*4 lj
      dimension ii(4),jj(4),nni(4),nnj(4),nn(4),io(4),jo(4),ipl(5)
      dimension ki(2),kj(2),x(5),y(5)
      data wend,wiload,star/'end','iload','star'/
      data swquad/'quad'/
      data nni/0,1,1,0/,nnj/0,0,1,1/
      data ki/1,0/,kj/0,1/
      data ipl/1,2,3,4,1/
      data bb/'        '/
      n4=4
      n6=6
      n64=64
      nume=0
      istar=0
      if(cdata(3).eq.star)istar=1
    5 continue
      load=data(2)
   10 continue
      call ffin
      word=cdata(1)
      if(word.eq.wiload)go to 5
      if(word.eq.wend)go to 900
      wquad=word
      skmat=data(2)
      kmat=skmat
      if(kmat.ge.1.and.kmat.le.nmat)go to 20
      write(6,1000)kmat
      kmat=1
   20 continue
      if(data(5).le.0)go to 40
      k=3
      do 30 i=1,4
      ii(i)=data(k)
      jj(i)=data(k+1)
      k=k+2
   30 continue
      go to 60
   40 continue
      im=data(3)
      jm=data(4)
      do 50 i=1,4
      ii(i)=im+nni(i)
      jj(i)=jm+nnj(i)
   50 continue
   60 continue
      call ffin
      ix=data(1)
      jx=data(2)
      nij=ix*jx
      snmat=nmat
      write(33,2000)wquad,nij,nmat,load
 2000 format(a9,i8,i3,i3)  
      do 70 i=1,nmat
      write(33,2010)(dble(smat(j,i)),j=1,62)
 2010 format(4(1pe16.8),'/')  
c      write(33,2015)(dble(smat(j,i)),j=62,63)
 2015 format(1pe16.8,1pe16.8)  
   70 continue
c      if(istar.eq.1)call starw(istar)
      i1x=ki(ijd)*ix+kj(ijd)*jx
      i2x=kj(ijd)*ix+ki(ijd)*jx
      nel=0
      do 110 i2=1,i2x
      do 100 i1=1,i1x
      do 80 i=1,4
      io(i)=ii(i)+ki(ijd)*(i1-1)+kj(ijd)*(i2-1)
      jo(i)=jj(i)+kj(ijd)*(i1-1)+ki(ijd)*(i2-1)
      n=njk(io(i),jo(i),ni,nj)
      ncon(n)=1
      nn(i)=n
   80 continue
      nel=nel+1
      nume=nume+1
      snel=nel
      write(33,2020)nel,kmat,(nn(i),i=1,4)
 2020 format(i8,i3,i8,i8,i8,i8)  
      do 90 iy=1,5
      ip=ipl(iy)
      i=io(ip)
      j=jo(ip)
      x(iy)=xn(i,j)
      y(iy)=yn(i,j)
   90 continue
      if(iprint.gt.0)write(6,1020)nume,(nn(i),i=1,4)
      if(iprint.gt.1)write(6,1030)(x(i),y(i),i=1,4)
c      call plot(x,y,x,y,5,5,1,1,.false.)
  100 continue
  110 continue
      go to 10
  900 continue
c     call done
      write(6,1010)nume
      return
 1000 format(' bad material number = ',i5)
 1010 format(1h0,9x,'pquad number of elements  = ',i6)
 1020 format(5i6)
 1030 format(1p8e12.4)
      end      
      subroutine jkn(n,j,k,jmax,kmax,knode)
      implicit real*8 (a-h,o-z)
      common/ercom/ierror
      if(n.le.knode)go to 5
      ierror=ierror+1
      n=1
    5 if(jmax.gt.kmax)go to 10
      k=1+(n-1)/jmax
      j=n-(k-1)*jmax
      return
   10 continue
      j=1+(n-1)/kmax
      k=n-(j-1)*kmax
      return
      end
      subroutine pnodes(x,y,z,ib,ni,nj)
      implicit real*8 (a-h,o-z)
      double precision data
      character*8 wend,wnode,wfix,owend,swnode,swfix
      character*8 bb
      dimension x(ni,1),y(ni,1),z(ni,1),ib(ni,1)
      common/flist/data(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata,word
c      common/info/nmax,nodes,nume,nmat,ijd
c      common/jkcom/jmax,kmax,knode
c      common/outcom/itout
      integer*4 lj
      data wfix,wend/'fix','end'/
      data swfix/'fix'/
      data wnode/'node'/
      data swnode/'node'/
      data owend/'end'/
      data bb/'        '/
      jmax=ni
      kmax=nj
      ijd=1
      n2=2
      n3=3
      n4=4
      if(ni.gt.nj)ijd=2
      knode=ni*nj
      zn=kmax
      write(33,2000)wnode,knode,n2,n2
 2000 format(a9,i8,i3,i3)
      nodes=knode
      do 10 n=1,nodes
      call jkn(n,i,j,ni,nj,ni*nj)
      ib(i,j)=0
      zn=n
      write(33,2010)n,x(i,j),y(i,j)
 2010 format(i8,1pe16.8,1pe16.8)
   10 continue
      write(6,1010)nodes
      nbc=0
   30 continue
      call ffin
      if(cdata(1).eq.wend)go to 60
      val=data(1)
      i1n=data(2)
      j1n=data(3)
      i1x=data(4)
      j1x=data(5)
      do 50 j=j1n,j1x
      do 40 i=i1n,i1x
      ib(i,j)=val
   40 continue
   50 continue
      go to 30
   60 continue
      n5=5
      zo=0.
      do 70 n=1,nodes
      call jkn(n,i,j,ni,nj,ni*nj)
      if(ib(i,j).eq.0)go to 70
      val=ib(i,j)
      vn=n
      write(33,2020)wfix,ib(i,j),n
 2020 format(a9,i4,i8)
      nbc=nbc+1
   70 continue
      write(6,1020)nbc
      write(33,2020)owend
 2030 format(a9)
      return
 1000 format(i5,1p2e12.4)
 1010 format(1h0,9x,'number of nodes     = ',i6)
 1020 format(1h0,9x,'number of bc s      = ',i6)
      end
      subroutine gmat(smat,nmat)
      implicit real*8 (a-h,o-z)
      common/flist/fdata(256),nflag(256),nword
      dimension smat(64,1)
      do 20 n=1,nmat
      call ffin
      do 10 i=1,63
      smat(i,n)=fdata(i)
   10 continue
   20 continue
      write(6,1000)nmat
      return
 1000 format(1h0,9x,'number of materials = ',i6)
      end
      block data sam
      implicit real*8 (a-h,o-z)
      character*8 head
      common/comh/head(10)
      common/pcom/iprint,itype
      common/cequal/nequal,kequal(2100)
      data head/10*1h /
      data iprint,itype/0,0/
      data nequal/0/
      common /tapes/ nin,nout,ntape1,ntape2,ntape3,ntape4,ntape5
      data nin,nout/5,6/
      common/transx/xyz(3,15),r(3,3,15),alpha(15),ng(15)
      data ng/1,2,3,4,11*0/
      end
      subroutine carea()
      implicit real*8 (a-h,o-z)
      common/tapes/ntape5,ntape6,ntape1,ntape2,ntape3,ntape4,ntape52
      common/matcom/area(50),volume(50),weight(50),density(50),
     .              thickness(50),blength(50)
      total=0.0
      write(ntape6,1000)
      do 10 i=1,50
      if(area(i).gt.0.0.or.volume(i).gt.0.0.or.blength(i).gt.0.0)then
        write(ntape6,1010)i,weight(i),volume(i),area(i),blength(i)
        total=total+weight(i)
      endif
   10 continue
      write(ntape6,*)' total weight = ',total
      return
 1000 format(' material ','   weight   ','   volume    ',
     .        '     area    ','   length   ')
 1010 format(2x,i5,3x,1pe12.4,1pe12.4,1pe12.4,1pe12.4)
      end
      subroutine brick(xd,yd,zd,ibc,in,jn)
      implicit real*8 (a-h,o-z)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      dimension ipl(5,4),xd(1),yd(1),zd(1),ibc(1)
      dimension xb(20),yb(20),zb(20),num(8)
      dimension ii(8),jj(8),kk(8),ni(8),nj(8),nk(8)
c     dimension io(8),jo(8),ko(8)
      data ipl/1,2,3,4,1, 5,6,7,8,5, 1,2,6,5,1, 4,3,7,8,4/
      data ni/0,1,1,0, 0,1,1,0/
      data nj/0,0,1,1, 0,0,1,1/
      data nk/0,0,0,0, 1,1,1,1/
      nel=0
      nee=0
      go to 15
   10 continue
      call ffin
   15 if(cdata(1).eq.'end')go to 900
      if(cdata(1).eq.'family')then
        nfam=fdata(2)
        goto 10
      endif
      mat=fdata(2)
      if(fdata(6).le.0.)go to 30
      k=3
      do 20 i=1,8
      ii(i)=fdata(k)
      jj(i)=fdata(k+1)
      kk(i)=fdata(k+2)
      k=k+3
   20 continue
      go to 50
   30 continue
      im=fdata(3)
      jm=fdata(4)
      km=fdata(5)
      do 40 i=1,8
      ii(i)=im+ni(i)
      jj(i)=jm+nj(i)
      kk(i)=km+nk(i)
   40 continue
   50 continue
      call ffin
      is=fdata(1)
      if(is.le.0)is=1
      js=fdata(2)
      if(js.le.0)js=1
      ks=fdata(3)
      if(ks.le.0)ks=1
      ie=fdata(4)
      if(ie.le.is)ie=is
      je=fdata(5)
      if(je.lt.js)je=js
      ke=fdata(6)
      if(ke.lt.ks)ke=ks
      inc=fdata(7)
      if(inc.le.0)inc=1
      jnc=fdata(8)
      if(jnc.le.0)jnc=1
      knc=fdata(9)
      if(knc.le.0)knc=1
      incc=1
      do 130 kl=ks,ke,knc
      do 120 jl=js,je,jnc
      do 110 il=is,ie,inc
      do 80 i=1,8
      num(i)=nijk(ii(i)+il-is,jj(i)+jl-js,kk(i)+kl-ks)
      ibc(num(i))=1
   80 continue
      nel=nel+1
      write(33,1000)nel,mat,incc,
     .  (num(m),m=1,8)
      n=0
      do 100 k3=1,4
      do 90 k2=1,5
      n=n+1
      ip=ipl(k2,k3)
      xb(n)=xd(num(ip))
      yb(n)=yd(num(ip))
      zb(n)=zd(num(ip))
   90 continue
  100 continue
      xc=.25*(xd(num(1))+xd(num(2))+xd(num(3))+xd(num(4))+
     .        xd(num(5))+xd(num(6))+xd(num(7))+xd(num(8)))
      yc=.25*(yd(num(1))+yd(num(2))+yd(num(3))+yd(num(4))+
     .        yd(num(5))+yd(num(6))+yd(num(7))+yd(num(8)))
      zc=.25*(zd(num(1))+zd(num(2))+zd(num(3))+zd(num(4))+
     .        zd(num(5))+zd(num(6))+zd(num(7))+zd(num(8)))
      rc=sqrt(xc**2+yc**2+zc**2)
c     write(*,*)' nel ',nel,' xc yc zc rc ',xc,yc,zc,rc
      call line3d(xb,yb,zb,20,5,nfam)
      call mbrick(nel,mat,num,xd,yd,zd)
  110 continue
  120 continue
  130 continue
      go to 10
  900 continue
      call cbrick(xd,yd,zd,ibc,nel,nee)
      numel=nel
      write(6,1010)numel
      return
 1000 format(11i5)
 1010 format(5x,'brick number of elements written = ' ,i5)
      end
      subroutine b8write(xd,yd,zd,ibc,nel,nee,nfam,mat,
     .          i1,j1,k1,i2,j2,k2,i3,j3,k3,i4,j4,k4,
     .          i5,j5,k5,i6,j6,k6,i7,j7,k7,i8,j8,k8)
      implicit real*8 (a-h,o-z)
      dimension xd(*),yd(*),zd(*),ibc(*)
      common/dumcom/dumval
      dimension xb(20),yb(20),zb(20),num(8)
      dimension ipl(5,4)
      data ipl/1,2,3,4,1, 5,6,7,8,5, 1,2,6,5,1, 4,3,7,8,4/
c
      num(1)=nijk(i1,j1,k1)
      ibc(num(1))=1
      num(2)=nijk(i2,j2,k2)
      ibc(num(2))=1
      num(3)=nijk(i3,j3,k3)
      ibc(num(3))=1
      num(4)=nijk(i4,j4,k4)
      ibc(num(4))=1
      num(5)=nijk(i5,j5,k5)
      ibc(num(5))=1
      num(6)=nijk(i6,j6,k6)
      ibc(num(6))=1
      num(7)=nijk(i7,j7,k7)
      ibc(num(7))=1
      num(8)=nijk(i8,j8,k8)
      ibc(num(8))=1
      nel=nel+1
      incc=1
      write(33,1000)nel,mat,incc,
     .  (num(m),m=1,8)
      n=0
      do 100 k3=1,4
      do 90 k2=1,5
      n=n+1
      ip=ipl(k2,k3)
      xb(n)=xd(num(ip))
      yb(n)=yd(num(ip))
      zb(n)=zd(num(ip))
   90 continue
  100 continue
      call line3d(xb,yb,zb,20,5,nfam)
      call mbrick(nel,mat,num,xd,yd,zd)
      return
 1000 format(11i5)
      end
      subroutine mbrick(nel,mat,num,xd,yd,zd)
      implicit real*8 (a-h,o-z)
      dimension num(8),xd(*),yd(*),zd(*)
      common/tapes/ntape5,ntape6,ntape1,ntape2,ntape3,ntape4,ntape52
      common/matcom/area(50),volume(50),weight(50),density(50),
     .              thickness(50),blength(50)
      dimension h(8),hr(8),hs(8),ht(8)
      dimension xg(4,4),wgt(4,4)
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
 
      if(mat.le.0.or.mat.gt.50)return
      v=0.0
      nint=2
      do 40 ir=1,nint
      r=xg(ir,nint)
      do 30 is=1,nint
      s=xg(is,nint)
      do 20 it=1,nint
      t=xg(it,nint)
      call gethrst(h,hr,hs,ht,r,s,t)
      xr=0
      xs=0
      xt=0
      yr=0
      ys=0
      yt=0
      zr=0
      zs=0
      zt=0
      do 10 i=1,8
      xr=xr+hr(i)*xd(num(i))
      yr=yr+hr(i)*yd(num(i))
      zr=zr+hr(i)*zd(num(i))
      xs=xs+hs(i)*xd(num(i))
      ys=ys+hs(i)*yd(num(i))
      zs=zs+hs(i)*zd(num(i))
      xt=xt+ht(i)*xd(num(i))
      yt=yt+ht(i)*yd(num(i))
      zt=zt+ht(i)*zd(num(i))
   10 continue
      det=xr*(ys*zt-zs*yt)-xs*(yr*zt-zr*yt)+xt*(yr*zs-zr*ys)
      if(det.le.0.0)then
          write(ntape6,1000)nel
          return
      endif
      wt=wgt(ir,nint)*wgt(is,nint)*wgt(it,nint)*det
      v = v + wt
   20 continue
   30 continue
   40 continue
      volume(mat) = volume(mat)+v
      weight(mat) = weight(mat)+v*density(mat)
 1000 format(' brick = ',i8,' bad jacobian')
      return
      end
      subroutine gethrst(h,hr,hs,ht,r,s,t)
      implicit real*8 (a-h,o-z)
      dimension h(8),hr(8),hs(8),ht(8)
      rr=(1.-r)
      rb=(1.+r)
      ss=(1.-s)
      sb=(1.+s)
      tt=(1.-t)
      tb=(1.+t)
      h(1)=.125*rr*ss*tt
      h(2)=.125*rb*ss*tt
      h(3)=.125*rb*sb*tt
      h(4)=.125*rr*sb*tt
      h(5)=.125*rr*ss*tb
      h(6)=.125*rb*ss*tb
      h(7)=.125*rb*sb*tb
      h(8)=.125*rr*sb*tb
      hr(1) = -.125*ss*tt
      hs(1) = -.125*rr*tt
      ht(1) = -.125*rr*ss
      hr(2) =  .125*ss*tt
      hs(2) = -.125*rb*tt
      ht(2) = -.125*rb*ss
      hr(3) =  .125*sb*tt
      hs(3) =  .125*rb*tt
      ht(3) = -.125*rb*sb
      hr(4) = -.125*sb*tt
      hs(4) =  .125*rr*tt
      ht(4) = -.125*rr*sb
      hr(5) = -.125*ss*tb
      hs(5) = -.125*rr*tb
      ht(5) =  .125*rr*ss
      hr(6) =  .125*ss*tb
      hs(6) = -.125*rb*tb
      ht(6) =  .125*rb*ss
      hr(7) =  .125*sb*tb
      hs(7) =  .125*rb*tb
      ht(7) =  .125*rb*sb
      hr(8) = -.125*sb*tb
      hs(8) =  .125*rr*tb
      ht(8) =  .125*rr*sb
      return
      end
      subroutine mbeam(nel,mat,n1,n2,xd,yd,zd)
      implicit real*8 (a-h,o-z)
      dimension xd(*),yd(*),zd(*)
      common/tapes/ntape5,ntape6,ntape1,ntape2,ntape3,ntape4,ntape52
      common/matcom/area(50),volume(50),weight(50),density(50),
     .              thickness(50),blength(50)
      if(mat.le.0.or.mat.gt.50)return
      xl=xd(n1)-xd(n2)
      yl=yd(n1)-yd(n2)
      zl=zd(n1)-zd(n2)
      xx=xl*xl+yl*yl+zl*zl
      if(xx.le.0.0)then
         write(ntape6,1000)nel
         return
      endif
      xx=sqrt(xx)
      blength(mat) = blength(mat)+xx
      volume(mat) = volume(mat)+xx*area(mat)
      weight(mat) = weight(mat)+xx*area(mat)*density(mat)
      return
 1000 format(' beam = ',i8,' zero length')
      end
      subroutine mshell(ns,mat,num,xd,yd,zd)
      implicit real*8 (a-h,o-z)
      dimension num(4),xd(*),yd(*),zd(*)
      common/tapes/ntape5,ntape6,ntape1,ntape2,ntape3,ntape4,ntape52
      common/matcom/area(50),volume(50),weight(50),density(50),
     .              thickness(50),blength(50)
      dimension v1(3),v2(3),v3(3)
      if(mat.le.0.or.mat.gt.50)return
      if(num(3).eq.num(4))then
        v1(1)=xd(num(2))-xd(num(1))
        v1(2)=yd(num(2))-yd(num(1))
        v1(3)=zd(num(2))-zd(num(1))
        v2(1)=xd(num(3))-xd(num(1))
        v2(2)=yd(num(3))-yd(num(1))
        v2(3)=zd(num(3))-zd(num(1))
        v3(1)=v1(2)*v2(3)-v2(2)*v1(3)
        v3(2)= -(v1(1)*v2(3)-v2(1)*v1(3))
        v3(3)=v1(1)*v2(2)-v2(1)*v1(2)
        a=v3(1)*v3(1)+v3(2)*v3(2)+v3(3)*v3(3)
        if(a.le.0.0)then
           write(ntape6,1000)ns,mat,num
        else
           a = .5*sqrt(a)
           area(mat) = area(mat)+a
           volume(mat) = volume(mat)+a*thickness(mat)
           weight(mat) = weight(mat)+a*thickness(mat)*density(mat)
        endif
      else
        v1(1)=xd(num(4))-xd(num(1))
        v1(2)=yd(num(4))-yd(num(1))
        v1(3)=zd(num(4))-zd(num(1))
        v2(1)=xd(num(2))-xd(num(1))
        v2(2)=yd(num(2))-yd(num(1))
        v2(3)=zd(num(2))-zd(num(1))
        v3(1)=v1(2)*v2(3)-v2(2)*v1(3)
        v3(2)= -(v1(1)*v2(3)-v2(1)*v1(3))
        v3(3)=v1(1)*v2(2)-v2(1)*v1(2)
        a=v3(1)*v3(1)+v3(2)*v3(2)+v3(3)*v3(3)
        if(a.le.0.0)then
           write(ntape6,1000)ns,mat,num
        else
           a = .5*sqrt(a)
           area(mat) = area(mat)+a
           volume(mat) = volume(mat)+a*thickness(mat)
           weight(mat) = weight(mat)+a*thickness(mat)*density(mat)
        endif
        v1(1)=xd(num(4))-xd(num(3))
        v1(2)=yd(num(4))-yd(num(3))
        v1(3)=zd(num(4))-zd(num(3))
        v2(1)=xd(num(2))-xd(num(3))
        v2(2)=yd(num(2))-yd(num(3))
        v2(3)=zd(num(2))-zd(num(3))
        v3(1)=v1(2)*v2(3)-v2(2)*v1(3)
        v3(2)= -(v1(1)*v2(3)-v2(1)*v1(3))
        v3(3)=v1(1)*v2(2)-v2(1)*v1(2)
        a=v3(1)*v3(1)+v3(2)*v3(2)+v3(3)*v3(3)
        if(a.le.0.0)then
           write(ntape6,1000)ns,mat,num
        else
           a = .5*sqrt(a)
           area(mat) = area(mat)+a
           volume(mat) = volume(mat)+a*thickness(mat)
           weight(mat) = weight(mat)+a*thickness(mat)*density(mat)
        endif
      endif
      return
 1000 format(' shell = ',i5,' area = ',1pe12.4,' mat = ',i5,' n = ',4i8)
      end
      subroutine node3(xd,yd,zd,ibc,in,jn)
      implicit real*8 (a-h,o-z)
      dimension xd(in,jn,1),yd(in,jn,1),zd(in,jn,1),ibc(in,jn,1)
      common/dumcom/dumval
      dimension word(6)
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      common/size/nsize,n0,n4,n5
      common/flist/data(256),nflag(256),nword
      common/clist/cdata(256)
      common ia(10)
      character*8 cdata,word
      do 30 k=1,kmax
      do 20 j=1,jmax
      do 10 i=1,imax
      ibc(i,j,k)=0
   10 continue
   20 continue
   30 continue
c
c          loop until end card
c
   40 continue
      call ffin
      if(cdata(1).eq.'end')go to 80
      n=data(1)
      i1=data(2)
      j1=data(3)
      k1=data(4)
      i2=data(5)
      j2=data(6)
      k2=data(7)
      if(i2.lt.i1)i2=i1
      if(j2.lt.j1)j2=j1
      if(k2.lt.k1)k2=k1
      do 70 k=k1,k2
      do 60 j=j1,j2
      do 50 i=i1,i2
      ibc(i,j,k)=n
   50 continue
   60 continue
   70 continue
      go to 40
   80 continue
      nauto=0
      nlast=0
      do 110 k=1,kmax
      do 100 j=1,jmax
      do 90 i=1,imax
      nlast=nlast+1
      call cramin(xd(i,j,k),word(1))
      call cramin(yd(i,j,k),word(3))
      call cramin(zd(i,j,k),word(5))
      ib2=ibc(i,j,k)/1000
      ib1=ibc(i,j,k)-1000*ib2
      if(xd(i,j,k).ne.dumval)go to 85
      nauto=nauto+1
      ib1=7
      ib2=7
   85 continue
      write(33,1000)nlast,ib1,word,ib2
   90 continue
  100 continue
  110 continue
      write(6,1010)nlast,nauto
      do 130 k=1,kmax
      do 120 j=1,jmax
      do 115 i=1,imax
      ibc(i,j,k)=0
  115 continue
  120 continue
  130 continue
      return
 1000 format(2i5,3(10x,a8,a2),4x,1h1,i5)
 1010 format(5x,'Nike3d or Dyna3d nodes = ',i8,' auto b.c. = ',i8)
      end
      subroutine elemt2(xd,yd,zd,ibc,in,jn)
      implicit real*8 (a-h,o-z)
      dimension xd(1),yd(1),zd(1),ibc(1)
      common/dumcom/dumval
      dimension xb(5),yb(5),zb(5)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
      ns=0
      nee=0
    5 continue
      icurve=fdata(2)
      flip=fdata(3)
      if(icurve.le.0)icurve=1
   10 continue
      call ffin
      if(cdata(1).eq.'end')go to 50
      if(cdata(1).eq.'elemt2')go to 5
      if(cdata(1).eq.'family')then
        nfam=fdata(2)
        goto 10
      endif
      i1=fdata(1)
      j1=fdata(2)
      i2=fdata(3)
      j2=fdata(4)
      i3=fdata(5)
      j3=fdata(6)
      i4=fdata(7)
      j4=fdata(8)
      inc=fdata(9)
      jnc=fdata(10)
      k1=1
      k2=1
      k3=1
      k4=1
      knc=1
      k=0
      do 40 kk=1,knc
      j=0
      do 30 jj=1,jnc
      i=0
      do 20 ii=1,inc
      ns=ns+1
      if(flip.ne.0)go to 15
      write(33,1000)ns,nijk(i1+i,j1+j,k1+k),nijk(i2+i,j2+j,k2+k),
     .              nijk(i3+i,j3+j,k3+k),nijk(i4+i,j4+j,k4+k),icurve
      go to 18
   15 continue
      write(33,1000)ns,nijk(i1+i,j1+j,k1+k),nijk(i4+i,j4+j,k4+k),
     .              nijk(i3+i,j3+j,k3+k),nijk(i2+i,j2+j,k2+k),icurve
   18 continue
      xb(1)=xd(nijk(i1+i,j1+j,k1+k))
      yb(2)=yd(nijk(i1+i,j1+j,k1+k))
      zb(3)=zd(nijk(i1+i,j1+j,k1+k))
      xb(2)=xd(nijk(i2+i,j2+j,k2+k))
      yb(2)=yd(nijk(i2+i,j2+j,k2+k))
      zb(2)=zd(nijk(i2+i,j2+j,k2+k))
      xb(3)=xd(nijk(i3+i,j3+j,k3+k))
      yb(3)=yd(nijk(i3+i,j3+j,k3+k))
      zb(3)=zd(nijk(i3+i,j3+j,k3+k))
      xb(4)=xd(nijk(i4+i,j4+j,k4+k))
      yb(4)=yd(nijk(i4+i,j4+j,k4+k))
      zb(4)=zd(nijk(i4+i,j4+j,k4+k))
      xb(5)=xd(nijk(i1+i,j1+j,k1+k))
      yb(5)=yd(nijk(i1+i,j1+j,k1+k))
      zb(5)=zd(nijk(i1+i,j1+j,k1+k))
      ibc(nijk(i1+i,j1+j,k1+k))=1
      ibc(nijk(i2+i,j2+j,k2+k))=1
      ibc(nijk(i3+i,j3+j,k3+k))=1
      ibc(nijk(i4+i,j4+j,k4+k))=1
      if(xb(1).eq.dumval)nee=nee+1
      if(xb(2).eq.dumval)nee=nee+1
      if(xb(3).eq.dumval)nee=nee+1
      if(xb(4).eq.dumval)nee=nee+1
      call line3d(xb,yb,zb,5,5,nfam)
      i=i+1
   20 continue
      j=j+1
   30 continue
      k=k+1
   40 continue
      go to 10
   50 continue
      write(6,1010)ns,nee
      return
 1000 format(6i5,4x,1h0,4x,1h1)
 1010 format(5x,'topaz2d elements  written =',i5,' nodes errors =',i5)
      end
      subroutine nodet2(xd,yd,zd,bc,in,jn)
      implicit real*8 (a-h,o-z)
      dimension xd(in,jn,1),yd(in,jn,1),zd(in,jn,1),bc(in,jn,1)
      dimension word(6)
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata,word
      do 30 k=1,kmax
      do 20 j=1,jmax
      do 10 i=1,imax
      bc(i,j,k)=0
   10 continue
   20 continue
   30 continue
c
c          loop until end card
c
   40 continue
      call ffin
      if(cdata(1).eq.'end')go to 80
      dn=fdata(1)
      i1=fdata(2)
      j1=fdata(3)
      i2=fdata(4)
      j2=fdata(5)
      k1=1
      k2=1
      if(i2.lt.i1)i2=i1
      if(j2.lt.j1)j2=j1
      if(k2.lt.k1)k2=k1
      do 70 k=k1,k2
      do 60 j=j1,j2
      do 50 i=i1,i2
      bc(i,j,k)=dn
   50 continue
   60 continue
   70 continue
      go to 40
   80 continue
      nauto=0
      last=0
      do 110 k=1,kmax
      do 100 j=1,jmax
      do 90 i=1,imax
      call cramin(xd(i,j,k),word(1))
      call cramin(yd(i,j,k),word(3))
      call cramin(bc(i,j,k),word(5))
c     ib2=ibc(i,j,k)/1000
c     ib1=ibc(i,j,k)-1000*ib2
      ione=1
      if(xd(i,j,k).ne.-1.23e-21)go to 85
      nauto=nauto+1
      ib1=7
      ib2=7
   85 continue
      last=last+1
      write(33,1000)last,word
   90 continue
  100 continue
  110 continue
      write(6,1010)last,nauto
      do 130 k=1,kmax
      do 120 j=1,jmax
      do 115 i=1,imax
      bc(i,j,k)=0
  115 continue
  120 continue
  130 continue
      return
 1000 format(i5,5x,3(a8,a2))
 1010 format(5x,'topaz2d nodes written =',i6,' auto b.c. = ',i8)
      end
      subroutine elemt3(xd,yd,zd,ibc,in,jn)
      implicit real*8 (a-h,o-z)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      dimension ipl(5,4),xd(1),yd(1),zd(1),ibc(1)
      dimension xb(20),yb(20),zb(20),num(8)
      dimension ii(8),jj(8),kk(8),ni(8),nj(8),nk(8)
      data ipl/1,2,3,4,1, 5,6,7,8,5, 1,2,6,5,1, 4,3,7,8,4/
      data ni/0,1,1,0, 0,1,1,0/
      data nj/0,0,1,1, 0,0,1,1/
      data nk/0,0,0,0, 1,1,1,1/
      nel=0
   10 continue
      call ffin
      if(cdata(1).eq.'end')go to 900
      if(cdata(1).eq.'family')then
        nfam=fdata(2)
        goto 10
      endif
      mat=fdata(2)
      if(fdata(6).le.0.)go to 30
      k=3
      do 20 i=1,8
      ii(i)=fdata(k)
      jj(i)=fdata(k+1)
      kk(i)=fdata(k+2)
      k=k+3
   20 continue
      go to 50
   30 continue
      im=fdata(3)
      jm=fdata(4)
      km=fdata(5)
      do 40 i=1,8
      ii(i)=im+ni(i)
      jj(i)=jm+nj(i)
      kk(i)=km+nk(i)
   40 continue
   50 continue
      call ffin
      is=fdata(1)
      if(is.le.0)is=1
      js=fdata(2)
      if(js.le.0)js=1
      ks=fdata(3)
      if(ks.le.0)ks=1
      ie=fdata(4)
      if(ie.le.is)ie=is
      je=fdata(5)
      if(je.lt.js)je=js
      ke=fdata(6)
      if(ke.lt.ks)ke=ks
      inc=fdata(7)
      if(inc.le.0)inc=1
      jnc=fdata(8)
      if(jnc.le.0)jnc=1
      knc=fdata(9)
      if(knc.le.0)knc=1
      incc=1
      do 130 kl=ks,ke,knc
      do 120 jl=js,je,jnc
      do 110 il=is,ie,inc
      do 80 i=1,8
      num(i)=nijk(ii(i)+il-is,jj(i)+jl-js,kk(i)+kl-ks)
      ibc(num(i))=1
   80 continue
      nel=nel+1
      write(33,1000)nel,mat,incc,
     .  (num(m),m=1,8)
      n=0
      do 100 k3=1,4
      do 90 k2=1,5
      n=n+1
      ip=ipl(k2,k3)
      xb(n)=xd(num(ip))
      yb(n)=yd(num(ip))
      zb(n)=zd(num(ip))
   90 continue
  100 continue
      call line3d(xb,yb,zb,20,5,nfam)
  110 continue
  120 continue
  130 continue
      go to 10
  900 continue
      numel=nel
      write(6,1010)numel
      return
 1000 format(11i5)
 1010 format(5x,'topaz3d elements written = ' ,i5)
      end
      subroutine nodet3(xd,yd,zd,bc,in,jn)
      implicit real*8 (a-h,o-z)
      dimension xd(in,jn,1),yd(in,jn,1),zd(in,jn,1),bc(in,jn,1)
      dimension word(8)
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata,word
      do 30 k=1,kmax
      do 20 j=1,jmax
      do 10 i=1,imax
      bc(i,j,k)=0
   10 continue
   20 continue
   30 continue
c
c          loop until end card
c
   40 continue
      call ffin
      if(cdata(1).eq.'end')go to 80
      dn=fdata(1)
      i1=fdata(2)
      j1=fdata(3)
      k1=fdata(4)
      i2=fdata(5)
      j2=fdata(6)
      k2=fdata(7)
      if(i2.lt.i1)i2=i1
      if(j2.lt.j1)j2=j1
      if(k2.lt.k1)k2=k1
      do 70 k=k1,k2
      do 60 j=j1,j2
      do 50 i=i1,i2
      bc(i,j,k)=dn
   50 continue
   60 continue
   70 continue
      go to 40
   80 continue
      nauto=0
      do 110 k=1,kmax
      do 100 j=1,jmax
      do 90 i=1,imax
      call cramin(xd(i,j,k),word(1))
      call cramin(yd(i,j,k),word(3))
      call cramin(zd(i,j,k),word(5))
      call cramin(bc(i,j,k),word(7))
c     ib2=ibc(i,j,k)/1000
c     ib1=ibc(i,j,k)-1000*ib2
      ione=1
      if(xd(i,j,k).ne.-1.23e-21)go to 85
      nauto=nauto+1
      ib1=7
      ib2=7
   85 continue
      write(33,1000)nijk(i,j,k),ione,word
c     write(33,1000)nijk(i,j,k),ib1,word,ib2
   90 continue
  100 continue
  110 continue
      ijkmax=imax*jmax*kmax
      write(6,1010)ijkmax,nauto
      do 130 k=1,kmax
      do 120 j=1,jmax
      do 115 i=1,imax
      bc(i,j,k)=0
  115 continue
  120 continue
  130 continue
      return
c1000 format(2i5,3(a8,a2),4x,1h1,i5)
 1000 format(2i5,3(10x,a8,a2),a8,a2)
 1010 format(5x,'topaz3d nodes written =',i6,' auto b.c. = ',i8)
      end
      subroutine slave
      implicit real*8 (a-h,o-z)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
      ns=0
   10 continue
      call ffin
      if(cdata(1).eq.'end')go to 50
      i1=fdata(1)
      j1=fdata(2)
      k1=fdata(3)
      i2=fdata(4)
      j2=fdata(5)
      k2=fdata(6)
      do 40 k=k1,k2
      do 30 j=j1,j2
      do 20 i=i1,i2
      ns=ns+1
      write(33,1000)ns,nijk(i,j,k)
   20 continue
   30 continue
   40 continue
      go to 10
   50 continue
      write(6,1010)ns
      return
 1000 format(6i5)
 1010 format(5x,'number of slave nodes written =',i5)
      end
      subroutine check(x,ibc)
      implicit real*8 (a-h,o-z)
      dimension x(*),ibc(*),ie(20)
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      common/dumcom/dumval
      nmax=imax*jmax*kmax
      iee=0
      nee=0
      do 20 i=1,nmax
      if(x(i).eq.dumval)go to 20
      if(ibc(i).ne.0)go to 20
      iee=iee+1
      nee=nee+1
      ie(iee)=i
      if(iee.lt.20)go to 20
      call nbad(ie,iee)
      iee=0
   20 continue
      if(iee.ne.0)call nbad(ie,iee)
      iee=0
      nee=0
      do 30 i=1,nmax
      if(x(i).eq.dumval)go to 30
      if(ibc(i).ne.0)go to 30
      iee=iee+1
      nee=nee+1
      ie(iee)=i
      if(iee.lt.10)go to 30
      write(6,1000)(ie(n),n=1,iee)
      iee=0
   30 continue
      if(iee.ne.0)write(6,1000)(ie(n),n=1,iee)
      write(6,1010)nee
      return
 1000 format(10i6)
 1010 format(' number of free nodes = ',i5)
      end
      subroutine nbad(ie,nn)
      implicit real*8 (a-h,o-z)
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      dimension ie(*),ii(20),jj(20),kk(20)
      do 10 iii=1,nn
      n=ie(iii)
      k=1+(n-1)/(imax*jmax)
      j=1+(n-1-(k-1)*imax*jmax)/imax
      i=n-(j-1)*imax-(k-1)*imax*jmax
      ii(iii)=i
      jj(iii)=j
      kk(iii)=k
   10 continue
      write(6,1000)(ii(i),jj(i),kk(i),i=1,nn)
      return
 1000 format(4(' ','(',3i5,')'))
      end
      subroutine mass(xd,yd,zd,in,jn)
      implicit real*8 (a-h,o-z)
      dimension xd(1),yd(1),zd(1)
      dimension xyz(2)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
      ns=0
    5 continue
      xmass=fdata(2)
      call cramin(xmass,xyz)
   10 continue
      call ffin
      if(cdata(1).eq.'end')go to 50
      if(cdata(1).eq.'mass')go to 5
      i1=fdata(1)
      j1=fdata(2)
      k1=fdata(3)
      i2=fdata(4)
      j2=fdata(5)
      k2=fdata(6)
      do 40 k=k1,k2
      do 30 j=j1,j2
      do 20 i=i1,i2
      ns=ns+1
      write(33,1000)nijk(i,j,k),xyz
   20 continue
   30 continue
   40 continue
      go to 10
   50 continue
      write(6,1010)ns
      return
 1000 format(i5,a8,a2)
 1010 format(' number of masses = ',i8)
      end
      subroutine print(xd,yd,zd,in,jn)
      implicit real*8 (a-h,o-z)
      dimension xd(*),yd(*),zd(*)
      dimension xyz(3)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
   10 continue
      call ffin
      if(cdata(1).eq.'end')go to 50
      i1=fdata(1)
      j1=fdata(2)
      k1=fdata(3)
      i2=fdata(4)
      j2=fdata(5)
      k2=fdata(6)
      do 40 k=k1,k2
      do 30 j=j1,j2
      do 20 i=i1,i2
      xyz(1)=xd(nijk(i,j,k))
      xyz(2)=yd(nijk(i,j,k))
      xyz(3)=zd(nijk(i,j,k))
      write(6,1000)i,j,k,nijk(i,j,k),xyz
   20 continue
   30 continue
   40 continue
      go to 10
   50 continue
      return
 1000 format(4i5,1p3e12.4)
      end
      subroutine merge1(xn)
      implicit real*8 (a-h,o-z)
      common/flist/fdata(256),nflag(256),nword
      common/dumcom/dumval
      common/clist/cdata(256)
      character*8 cdata
      common/size/nsize,n0,n4,n5
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      dimension io(21),xn(*)
      data io/1,0,0,  0,1,0,  0,0,1,  -1,0,0,  0,-1,0,  0,0,-1,
     .        0,0,0/
      common ia(10)
      last=imax*jmax*kmax
      i1=fdata(2)
      j1=fdata(3)
      k1=fdata(4)
      i2=fdata(5)
      j2=fdata(6)
      k2=fdata(7)
      i3=fdata(8)
      j3=fdata(9)
      k3=fdata(10)
      i4=fdata(11)
      j4=fdata(12)
      k4=fdata(13)
      ix=fdata(14)
      jx=fdata(15)
      kx=fdata(16)
      if(i1.ne.i2)then
        if(i1.gt.i2)then
	  m1=10
	  n1x=i1-i2+1
        else
          m1=1
          n1x=i2-i1+1
        endif
      else if(j1.ne.j2)then
        if(j1.gt.j2)then
	  m1=13
	  n1x=j1-j2+1
        else
          m1=4
          n1x=j2-j1+1
        endif
      else if(k1.ne.k2)then
        if(k1.gt.k2)then
	  m1=16
	  n1x=k1-k2+1
        else
          m1=7
          n1x=k2-k1+1
        endif
      else
          m1=19
          n1x=1
      endif
      if(i3.ne.i4)then
        if(i3.gt.i4)then
	  m2=10
	  n2x=i3-i4+1
        else
          m2=1
          n2x=i4-i3+1
        endif
      else if(j3.ne.j4)then
        if(j3.gt.j4)then
	  m2=13
	  n2x=j3-j4+1
        else
          m2=4
          n2x=j4-j3+1
        endif
      else if(k3.ne.k4)then
        if(k3.gt.k4)then
	  m2=16
	  n2x=k3-k4+1
        else
          m2=7
          n2x=k4-k3+1
        endif
      else
          m2=19
          n2x=1
      endif
      if(n1x.ne.n2x)then
          write(6,*)' Merge Error n1x = ',n1x,' n2x = ',n2x
          return
      endif
      do 370 k=1,kx
      km1=k1+(k-1)
      km2=k3+(k-1)
      do 360 j=1,jx
      jm1=j1+(j-1)
      jm2=j3+(j-1)
      do 350 i=1,ix
      im1=i1+(i-1)
      im2=i3+(i-1)
      do 340 nn=1,n1x
      nm=nn-1
      n1=nijk(im1+io(m1)*nm,jm1+io(m1+1)*nm,km1+io(m1+2)*nm)
      n2=nijk(im2+io(m2)*nm,jm2+io(m2+1)*nm,km2+io(m2+2)*nm)
      if(n1.le.0.or.n1.gt.last.or.n2.le.0.or.n2.gt.last)then
          write(6,*)' Merge error n1 = ',n1,' n2 = ',n2,' last = ',last
          go to 340
      endif
      kn1=ia(n0+n1-1)
      kn2=ia(n0+n2-1)
      if(kn1.gt.kn2)then
          ia(n0+n1-1)=kn2
          xn(n1)=dumval
      elseif(kn1.ne.kn2)then
          ia(n0+n2-1)=kn1
          xn(n2)=dumval
      endif
  340 continue
  350 continue
  360 continue
  370 continue
  900 continue
      do 1000 i=1,last
      if(i.ne.ia(n0+i-1))then
         nn=ia(n0+i-1)
         ia(n0+i-1)=ia(n0+nn-1)
      endif
 1000 continue
c     write(6,1020)(ia(i),i=1,last)
 1020 format(10i8)
      return
      end
      subroutine merge2(xn)
      implicit real*8 (a-h,o-z)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      common/dumcom/dumval
      character*8 cdata
      common/size/nsize,n0,n4,n5
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      dimension io(21),xn(*)
      data io/1,0,0,  0,1,0,  0,0,1,  -1,0,0,  0,-1,0,  0,0,-1,
     .        0,0,0/
      common ia(10)
      last=imax*jmax*kmax
      i1=fdata(2)
      j1=fdata(3)
      k1=fdata(4)
      i2=fdata(5)
      j2=fdata(6)
      k2=fdata(7)
      i3=fdata(8)
      j3=fdata(9)
      k3=fdata(10)
      ix=fdata(11)
      jx=fdata(12)
      kx=fdata(13)
      if(i1.ne.i2)then
        if(i1.gt.i2)then
	      m1=10
	      n1x=i1-i2+1
        else
          m1=1
          n1x=i2-i1+1
        endif
      else if(j1.ne.j2)then
        if(j1.gt.j2)then
	       m1=13
	       n1x=j1-j2+1
        else
          m1=4
          n1x=j2-j1+1
        endif
      else if(k1.ne.k2)then
        if(k1.gt.k2)then
	     m1=16
	     n1x=k1-k2+1
        else
          m1=7
          n1x=k2-k1+1
        endif
      else
	    m1=19
	    n1x=1
      endif
      if(i1.ne.i3)then
        if(i1.gt.i3)then
	      m2=10
	      n2x=i1-i3+1
        else
          m2=1
          n2x=i3-i1+1
        endif
      else if(j1.ne.j3)then
        if(j1.gt.j3)then
	      m2=13
	      n2x=j1-j3+1
        else
          m2=4
          n2x=j3-j1+1
        endif
      else if(k1.ne.k3)then
        if(k1.gt.k3)then
	      m2=16
	      n2x=k1-k3+1
        else
          m2=7
          n2x=k3-k1+1
        endif
      else
	     m2=19
	     n2x=1
      endif
      if(n1x.ne.n2x)then
          write(6,*)' Merge Error n1x = ',n1x,' n2x = ',n2x
          return
      endif
      do 370 k=1,kx
      km1=k1+(k-1)
      do 360 j=1,jx
      jm1=j1+(j-1)
      do 350 i=1,ix
      im1=i1+(i-1)
      do 340 nn=1,n1x
      nm=nn-1
      n1=nijk(im1+io(m1)*nm,jm1+io(m1+1)*nm,km1+io(m1+2)*nm)
      n2=nijk(im1+io(m2)*nm,jm1+io(m2+1)*nm,km1+io(m2+2)*nm)
      if(n1.le.0.or.n1.gt.last.or.n2.le.0.or.n2.gt.last)then
          write(6,*)' Merge error n1 = ',n1,' n2 = ',n2,' last = ',last
          go to 340
      endif
      kn1=ia(n0+n1-1)
      kn2=ia(n0+n2-1)
      if(kn1.gt.kn2)then
          ia(n0+n1-1)=kn2
          xn(n1)=dumval
      elseif(kn1.ne.kn2)then
          ia(n0+n2-1)=kn1
          xn(n2)=dumval
      endif
  340 continue
  350 continue
  360 continue
  370 continue
  900 continue
      do 1000 i=1,last
      if(i.ne.ia(n0+i-1))then
         nn=ia(n0+i-1)
         ia(n0+i-1)=ia(n0+nn-1)
      endif
 1000 continue
c     write(6,1020)(ia(i),i=1,last)
 1020 format(10i8)
      return
      end
      subroutine beam(xd,yd,zd,ibc,in,jn)
      implicit real*8 (a-h,o-z)
      dimension xd(*),yd(*),zd(*),ibc(*)
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      common/dumcom/dumval
      dimension xb(2),yb(2),zb(2)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
      ns=0
      nee=0
    5 continue
      icurve=fdata(2)
      if(icurve.le.0)icurve=1
   10 continue
      call ffin
      if(cdata(1).eq.'cbeam')then
        call cbeam(xd,yd,zd,ibc,ns,nee)
        goto 10
      endif
      if(cdata(1).eq.'end')go to 50
      if(cdata(1).eq.'beam')go to 5
      if(cdata(1).eq.'family')then
        nfam=fdata(2)
        goto 10
      endif
      i1=fdata(1)
      j1=fdata(2)
      k1=fdata(3)
      i2=fdata(4)
      j2=fdata(5)
      k2=fdata(6)
      i3=fdata(7)
      j3=fdata(8)
      k3=fdata(9)
      inc=fdata(10)
      jnc=fdata(11)
      knc=fdata(12)
      k=0
      do 40 kk=1,knc
      j=0
      do 30 jj=1,jnc
      i=0
      do 20 ii=1,inc
      ns=ns+1
      write(33,1000)ns,icurve,nijk(i1+i,j1+j,k1+k),nijk(i2+i,j2+j,k2+k),
     . nijk(i3,j3,k3)
      xb(1)=xd(nijk(i1+i,j1+j,k1+k))
      yb(1)=yd(nijk(i1+i,j1+j,k1+k))
      zb(1)=zd(nijk(i1+i,j1+j,k1+k))
      xb(2)=xd(nijk(i2+i,j2+j,k2+k))
      yb(2)=yd(nijk(i2+i,j2+j,k2+k))
      zb(2)=zd(nijk(i2+i,j2+j,k2+k))
      ibc(nijk(i1+i,j1+j,k1+k))=1
      ibc(nijk(i2+i,j2+j,k2+k))=1
      if(xb(1).eq.dumval)nee=nee+1
      if(xb(2).eq.dumval)nee=nee+1
      call line3d(xb,yb,zb,2,2,nfam)
      n1=nijk(i1+i,j1+j,k1+k)
      n2=nijk(i2+i,j2+j,k2+k)
      call mbeam(ns,icurve,n1,n2,xd,yd,zd)
      i=i+1
   20 continue
      j=j+1
   30 continue
      k=k+1
   40 continue
      go to 10
   50 continue
      write(6,1010)ns,nee
      return
 1000 format(2i5,4x,1h1,4i5)
 1010 format(5x,'number of beams written =',i5,' node errors = ',i5)
      end
      subroutine bwrite(xd,yd,zd,ibc,ns,nee,nfam,imat,i1,j1,k1,
     .                  i2,j2,k2,i3,j3,k3)
      implicit real*8 (a-h,o-z)
      dimension xd(*),yd(*),zd(*),ibc(*)
      common/dumcom/dumval
      dimension xb(2),yb(2),zb(2)
      ns=ns+1
      write(33,1000)ns,imat,nijk(i1,j1,k1),nijk(i2,j2,k2),
     . nijk(i3,j3,k3)
      xb(1)=xd(nijk(i1,j1,k1))
      yb(1)=yd(nijk(i1,j1,k1))
      zb(1)=zd(nijk(i1,j1,k1))
      xb(2)=xd(nijk(i2,j2,k2))
      yb(2)=yd(nijk(i2,j2,k2))
      zb(2)=zd(nijk(i2,j2,k2))
      ibc(nijk(i1,j1,k1))=1
      ibc(nijk(i2,j2,k2))=1
      if(xb(1).eq.dumval)nee=nee+1
      if(xb(2).eq.dumval)nee=nee+1
      call line3d(xb,yb,zb,2,2,nfam)
      n1=nijk(i1,j1,k1)
      n2=nijk(i2,j2,k2)
      call mbeam(ns,imat,n1,n2,xd,yd,zd)
      return
 1000 format(2i5,4x,1h1,4i5)
      end
      subroutine shell(xd,yd,zd,ibc,in,jn)
      implicit real*8 (a-h,o-z)
      dimension xd(*),yd(*),zd(*),ibc(*),num(4)
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      common/dumcom/dumval
      dimension xb(5),yb(5),zb(5)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
      ns=0
      nee=0
    5 continue
      icurve=fdata(2)
      flip=fdata(3)
      if(icurve.le.0)icurve=1
   10 continue
      call ffin
      if(cdata(1).eq.'end')go to 50
      if(cdata(1).eq.'shell')go to 5
      if(cdata(1).eq.'family')then
        nfam=fdata(2)
        goto 10
      endif
      i1=fdata(1)
      j1=fdata(2)
      k1=fdata(3)
      i2=fdata(4)
      j2=fdata(5)
      k2=fdata(6)
      i3=fdata(7)
      j3=fdata(8)
      k3=fdata(9)
      i4=fdata(10)
      j4=fdata(11)
      k4=fdata(12)
      inc=fdata(13)
      jnc=fdata(14)
      knc=fdata(15)
      k=0
      do 40 kk=1,knc
      j=0
      do 30 jj=1,jnc
      i=0
      do 20 ii=1,inc
      ns=ns+1
      if(flip.ne.0)go to 15
      write(33,1000)ns,icurve,nijk(i1+i,j1+j,k1+k),nijk(i2+i,j2+j,k2+k),
     .              nijk(i3+i,j3+j,k3+k),nijk(i4+i,j4+j,k4+k)
      go to 18
   15 continue
      write(33,1000)ns,icurve,nijk(i1+i,j1+j,k1+k),nijk(i4+i,j4+j,k4+k),
     .              nijk(i3+i,j3+j,k3+k),nijk(i2+i,j2+j,k2+k)
   18 continue
      num(1)=nijk(i1+i,j1+j,k1+k)
      num(2)=nijk(i2+i,j2+j,k2+k)
      num(3)=nijk(i3+i,j3+j,k3+k)
      num(4)=nijk(i4+i,j4+j,k4+k)
      xb(1)=xd(nijk(i1+i,j1+j,k1+k))
      yb(1)=yd(nijk(i1+i,j1+j,k1+k))
      zb(1)=zd(nijk(i1+i,j1+j,k1+k))
      xb(2)=xd(nijk(i2+i,j2+j,k2+k))
      yb(2)=yd(nijk(i2+i,j2+j,k2+k))
      zb(2)=zd(nijk(i2+i,j2+j,k2+k))
      xb(3)=xd(nijk(i3+i,j3+j,k3+k))
      yb(3)=yd(nijk(i3+i,j3+j,k3+k))
      zb(3)=zd(nijk(i3+i,j3+j,k3+k))
      xb(4)=xd(nijk(i4+i,j4+j,k4+k))
      yb(4)=yd(nijk(i4+i,j4+j,k4+k))
      zb(4)=zd(nijk(i4+i,j4+j,k4+k))
      xb(5)=xd(nijk(i1+i,j1+j,k1+k))
      yb(5)=yd(nijk(i1+i,j1+j,k1+k))
      zb(5)=zd(nijk(i1+i,j1+j,k1+k))
      ibc(nijk(i1+i,j1+j,k1+k))=1
      ibc(nijk(i2+i,j2+j,k2+k))=1
      ibc(nijk(i3+i,j3+j,k3+k))=1
      ibc(nijk(i4+i,j4+j,k4+k))=1
      if(xb(1).eq.dumval)nee=nee+1
      if(xb(2).eq.dumval)nee=nee+1
      if(xb(3).eq.dumval)nee=nee+1
      if(xb(4).eq.dumval)nee=nee+1
      call line3d(xb,yb,zb,5,5,nfam)
      call mshell(ns,icurve,num,xd,yd,zd)
      i=i+1
   20 continue
      j=j+1
   30 continue
      k=k+1
   40 continue
      go to 10
   50 continue
      call cshell(xd,yd,zd,ibc,ns,nee)
      write(6,1010)ns,nee
      return
 1000 format(2i5,4x,1h1,4i5)
 1010 format(5x,'number of shells written =',i5,' nodes errors =',i5)
      end
      subroutine s4write(xd,yd,zd,ibc,nel,nee,nfam,mat,
     .          i1,j1,k1,i2,j2,k2,i3,j3,k3,i4,j4,k4)
      implicit real*8 (a-h,o-z)
      dimension xd(*),yd(*),zd(*),ibc(*)
      common/dumcom/dumval
      dimension xb(20),yb(20),zb(20),num(8)
c
      num(1)=nijk(i1,j1,k1)
      ibc(num(1))=1
      num(2)=nijk(i2,j2,k2)
      ibc(num(2))=1
      num(3)=nijk(i3,j3,k3)
      ibc(num(3))=1
      num(4)=nijk(i4,j4,k4)
      ibc(num(4))=1
      num(5)=num(1)
      nel=nel+1
      write(33,1000)nel,mat,(num(m),m=1,4)
      n=0
      do 90 k=1,5
      n=n+1
      xb(n)=xd(num(k))
      yb(n)=yd(num(k))
      zb(n)=zd(num(k))
   90 continue
      call line3d(xb,yb,zb,5,5,nfam)
      call mshell(nel,mat,num,xd,yd,zd)
      return
 1000 format(2i5,4x,1h1,4i5)
      end
*deck press2
      subroutine press2(xd,yd,zd)
      implicit real*8 (a-h,o-z)
      dimension xd(*),yd(*),zd(*)
      dimension nn(4),xx(3,4),f(3,4),p1(4),w(2)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
      ns=0
    5 p=fdata(2)
      icurve=fdata(3)
      if(icurve.le.0)icurve=1
   10 continue
      call ffin
      if(cdata(1).eq.'end')go to 90
      if(cdata(1).eq.'press2')go to 5
      i1=fdata(1)
      j1=fdata(2)
      k1=fdata(3)
      i2=fdata(4)
      j2=fdata(5)
      k2=fdata(6)
      i3=fdata(7)
      j3=fdata(8)
      k3=fdata(9)
      i4=fdata(10)
      j4=fdata(11)
      k4=fdata(12)
      inc=fdata(13)
      jnc=fdata(14)
      knc=fdata(15)
      k=0
      do 80 kk=1,knc
      j=0
      do 70 jj=1,jnc
      i=0
      do 60 ii=1,inc
      nn(1)=nijk(i1+i,j1+j,k1+k)
      nn(2)=nijk(i2+i,j2+j,k2+k)
      nn(3)=nijk(i3+i,j3+j,k3+k)
      nn(4)=nijk(i4+i,j4+j,k4+k)
      do 20 l=1,4
      xx(1,l)=xd(nn(l))
      xx(2,l)=yd(nn(l))
      xx(3,l)=zd(nn(l))
      f(1,l)=0.
      f(2,l)=0.
      f(3,l)=0.
      p1(l)=p
   20 continue
      call plvec3(xx,f,p1)
      do 40 l=1,4
      do 30 m=1,3
      ns=ns+1
      call cramin(f(m,l),w)
      write(33,1000)nn(l),m,icurve,w
   30 continue
   40 continue
      i=i+1
   60 continue
      j=j+1
   70 continue
      k=k+1
   80 continue
      go to 10
   90 continue
      write(6,1010)ns
      return
 1000 format(3i5,a8,a2)
 1010 format(5x,'number of pressures written =',i5)
      end
*deck plvec3
      subroutine plvec3 (xx,pl,print)
      implicit real*8 (a-h,o-z)
c
c
      dimension        xg(3),wgt(3),xx(3,1),pl(3,1)
      dimension        h(12),q(2,12),xj(2,3),a(3),print(1)
      data node3/4/
      data xg/
     1 -.7745966692415d0, .0000000000000d0, .7745966692415d0/
      data wgt/
     1  .5555555555556d0, .8888888888889d0, .5555555555556d0/
c
c
c     surface integration loop
c
      do 300 lr=1,3
      r      =xg(lr)
c
      do 300 ls=1,3
      s       =xg(ls)
c
      wt      = wgt(lr)*wgt(ls)
c
c     evaluate the interpolation functions and derivatives
c
      rm     = 1.0 - r
      sm     = 1.0 - s
      rp     = 1.0 + r
      sp     = 1.0 + s
c
c          1. corner nodes
c
      h(1)    = 0.25*rp*sp
      h(2)    = 0.25*rm*sp
      h(3)    = 0.25*rm*sm
      h(4)    = 0.25*rp*sm
c
      q(1,1) = 0.25*    sp
      q(1,2) =-0.25*    sp
      q(1,3) =-0.25*    sm
      q(1,4) = 0.25*    sm
c
      q(2,1) = 0.25* rp
      q(2,2) = 0.25* rm
      q(2,3) =-0.25* rm
      q(2,4) =-0.25* rp
c
c     linear interpolation of pressure at station *(r,s)*
c
      press=0.
      do 5 i=1,4
    5 press=press + print(i)*h(i)
c
c
c
c     compute (r,s) derivatives with respect to (x,y,z) coordinates
c
   40 do 50 i=1,2
      do 50 j=1,3
      x      = 0.0
c
      do 55 k=1,node3
   55 x      = x + q(i,k)* xx(j,k)
      xj(i,j)= x
   50 continue
c
c     compute the direction cosines of the surface normal vector
c     at point (r,s) in the element face
c
      a(1)    = xj(1,2) * xj(2,3) - xj(1,3) * xj(2,2)
      a(2)    = xj(1,3) * xj(2,1) - xj(1,1) * xj(2,3)
      a(3)    = xj(1,1) * xj(2,2) - xj(1,2) * xj(2,1)
      x       = 0.0
      do 60 k=1,3
   60 x       = x + a(k)*a(k)
      x       = sqrt(x)
c
      if(x.gt.1.0e-6) go to 70
c
      write (6,2000) r,s
      stop
c
   70 x      = 1.0/x
      do 80 k=1,3
   80 a(k)   = a(k)* x
c
c     compute the area differential
c
      a1     = 0.0
      a2     = 0.0
      a3     = 0.0
      do 90 k=1,3
      a1     = a1 + xj(1,k)* xj(1,k)
      a2     = a2 + xj(1,k)* xj(2,k)
      a3     = a3 + xj(2,k)* xj(2,k)
   90 continue
      x      = sqrt(a1*a3 - a2*a2)
c
      factor = wt* x* press
c
c     assemble the node force contribution
c
      do 100 k=1,node3
c
c
      x      = factor* h(k)
   93 do 95 i=1,3
   95 pl(i,k)=pl(i,k) - a(i)* x
c
  100 continue
  300 continue
c
      return
 2000 format (48h0***error   undefined element face normal vector, /
     1        12x,3hr =, f10.4 /
     2        12x,3hs =, f10.4 )
      end
      subroutine equal2(xn)
      implicit real*8 (a-h,o-z)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      common/dumcom/dumval
      dimension xn(*)
      character*8 cdata
      common/size/nsize,n0,n4,n5
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      common ia(10)
      nn=0
      ns=0
      nequal=0
      last=imax*jmax*kmax
   10 continue
      call ffin
      if(cdata(1).eq.'end')go to 950
      i1=fdata(1)
      j1=fdata(2)
      k1=fdata(3)
      i2=fdata(4)
      j2=fdata(5)
      k2=fdata(6)
      inc=fdata(7)
      jnc=fdata(8)
      knc=fdata(9)
      k=0
      do 140 kk=1,knc
      j=0
      do 130 jj=1,jnc
      i=0
      do 120 ii=1,inc
      ns=ns+1
      kmin=1000000
      lequal=nijk(i+i1,j+j1,k+k1)
      lequal=ia(n0+lequal-1)
      if(lequal.le.kmin)kmin=lequal
      lequal=nijk(i+i2,j+j2,k+k2)
      lequal=ia(n0+lequal-1)
      if(lequal.le.kmin)kmin=lequal
c
      lequal=nijk(i+i1,j+j1,k+k1)
      lequal=ia(n0+lequal-1)
      if(lequal.gt.kmin)then
        ia(n0+lequal-1)=kmin
        xn(lequal)=dumval
      else
        ia(n0+lequal-1)=kmin
      endif
      lequal=nijk(i+i2,j+j2,k+k2)
      lequal=ia(n0+lequal-1)
      if(lequal.gt.kmin)then
        ia(n0+lequal-1)=kmin
        xn(lequal)=dumval
      else
        ia(n0+lequal-1)=kmin
      endif
      i=i+1
  120 continue
      j=j+1
  130 continue
      k=k+1
  140 continue
      go to 10
  950 continue
      nequal=ns
      write(6,1020)nequal
      do 1000 i=1,last
      if(i.ne.ia(n0+i-1))then
         nn=ia(n0+i-1)
         ia(n0+i-1)=ia(n0+nn-1)
      endif
 1000 continue
c     write(6,1020)(ia(i),i=1,last)
      return
 1020 format(5x,'number of equal groups written =',i5)
      end
      subroutine equal(xn)
      implicit real*8 (a-h,o-z)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      common/dumcom/dumval
      dimension xn(*)
      character*8 cdata
      common/size/nsize,n0,n4,n5
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      common ia(10)
      nn=0
      ns=0
      nequal=0
      last=imax*jmax*kmax
   10 continue
      call ffin
      if(cdata(1).eq.'end')go to 950
      i1=fdata(1)
      j1=fdata(2)
      k1=fdata(3)
      i2=fdata(4)
      j2=fdata(5)
      k2=fdata(6)
      inc=fdata(7)
      jnc=fdata(8)
      knc=fdata(9)
      k=0
      kmin=1000000
      do 40 kk=1,knc
      j=0
      do 30 jj=1,jnc
      i=0
      do 20 ii=1,inc
      ns=ns+1
      do 6 il=i1,i2
      do 4 jl=j1,j2
      do 2 kl=k1,k2
      lequal=nijk(i+il,j+jl,k+kl)
      lequal=ia(n0+lequal-1)
      if(lequal.le.kmin)kmin=lequal
    2 continue
    4 continue
    6 continue
      i=i+1
   20 continue
      j=j+1
   30 continue
      k=k+1
   40 continue
      k=0
      do 140 kk=1,knc
      j=0
      do 130 jj=1,jnc
      i=0
      do 120 ii=1,inc
      ns=ns+1
      do 66 il=i1,i2
      do 64 jl=j1,j2
      do 62 kl=k1,k2
      lequal=nijk(i+il,j+jl,k+kl)
      lequal=ia(n0+lequal-1)
      if(lequal.gt.kmin)then
        ia(n0+lequal-1)=kmin
        xn(lequal)=dumval
      else
        ia(n0+lequal-1)=kmin
      endif
   62 continue
   64 continue
   66 continue
      i=i+1
  120 continue
      j=j+1
  130 continue
      k=k+1
  140 continue
      go to 10
  950 continue
      nequal=ns
      write(6,1020)nequal
      do 1000 i=1,last
      if(i.ne.ia(n0+i-1))then
         nn=ia(n0+i-1)
         ia(n0+i-1)=ia(n0+nn-1)
      endif
 1000 continue
c     write(6,1020)(ia(i),i=1,last)
      return
 1020 format(5x,'number of equal groups written =',i5)
      end
      subroutine tell(neijk,in,jn)
      implicit real*8 (a-h,o-z)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      dimension neijk(in,jn,1)
   10 continue
      call ffin
      if(cdata(1).eq.'end')go to 900
      im=fdata(2)
      jm=fdata(3)
      km=fdata(4)
      if(im.ge.1.and.im.le.imax.and.jm.ge.1.and.jm.le.jmax.and.
     .   km.ge.1.and.km.le.kmax)go to 20
      write(6,990)im,jm,km
      go to 10
   20 continue
      if(cdata(1).eq.'n')go to 40
      if(cdata(1).eq.'e')go to 30
      write(6,1000)
      go to 10
   30 continue
      write(6,1010)im,jm,km,neijk(im,jm,km)
      go to 10
   40 continue
      write(6,1020)im,jm,km,nijk(im,jm,km)
      go to 10
  900 continue
      return
  990 format(5x,'request range error im,jm,km = ',3i5)
 1000 format(5x,'can request only e or n')
 1010 format(5x,'element  i,j,k,n =' ,4i8)
 1020 format(5x,'node i,j,k,n = ',4i8)
      end
      subroutine tied2
      implicit real*8 (a-h,o-z)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata,p(2)
      dimension iii(12),jjj(12),kkk(12)
      ns=0
      itwo=2
      ntie=fdata(2)
      strain=fdata(3)
      call cramin(strain,p(1))
   10 continue
      call ffin
      if(cdata(1).eq.'end')go to 90
      do 20 i=1,ntie
      iii(i)=fdata(1+3*(i-1))
      jjj(i)=fdata(2+3*(i-1))
      kkk(i)=fdata(3+3*(i-1))
   20 continue
      imax=fdata(1+3*(ntie))
      jmax=fdata(2+3*(ntie))
      kmax=fdata(3+3*(ntie))
      inc=fdata(1+4*(ntie))
      jnc=fdata(2+4*(ntie))
      knc=fdata(3+4*(ntie))
      k=0
      do 50 kk=1,kmax,knc
      j=0
      do 40 jj=1,jmax,jnc
      i=0
      do 30 ii=1,imax,inc
      ns=ns+1
      write(33,1000)p,(nijk(iii(m)+i,jjj(m)+j,kkk(m)+k),m=1,ntie)
      i=i+inc
   30 continue
      j=j+jnc
   40 continue
      k=k+knc
   50 continue
      go to 10
   90 continue
      write(6,1010)ns
      return
 1000 format(a8,a2,12i5)
 1010 format(5x,'number of tie nodes written =',i5)
      end
      subroutine tied
      implicit real*8 (a-h,o-z)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata,p(2)
      dimension iii(12),jjj(12),kkk(12)
      ns=0
      itwo=2
      ntie=fdata(2)
      strain=fdata(3)
      call cramin(strain,p(1))
   10 continue
      call ffin
      if(cdata(1).eq.'end')go to 90
      do 20 i=1,ntie
      iii(i)=fdata(1+3*(i-1))
      jjj(i)=fdata(2+3*(i-1))
      kkk(i)=fdata(3+3*(i-1))
   20 continue
      inc=fdata(1+3*(ntie))
      jnc=fdata(2+3*(ntie))
      knc=fdata(3+3*(ntie))
      k=0
      do 50 kk=1,knc
      j=0
      do 40 jj=1,jnc
      i=0
      do 30 ii=1,inc
      ns=ns+1
      write(33,1000)p,(nijk(iii(m)+i,jjj(m)+j,kkk(m)+k),m=1,ntie)
      i=i+1
   30 continue
      j=j+1
   40 continue
      k=k+1
   50 continue
      go to 10
   90 continue
      write(6,1010)ns
      return
 1000 format(a8,a2,12i5)
 1010 format(5x,'number of tie nodes written =',i5)
      end
      subroutine tie
      implicit real*8 (a-h,o-z)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
      ns=0
      itwo=2
      ntie=fdata(2)
   10 continue
      call ffin
      if(cdata(1).eq.'end')go to 50
      i1=fdata(1)
      j1=fdata(2)
      k1=fdata(3)
      i2=fdata(4)
      j2=fdata(5)
      k2=fdata(6)
      inc=fdata(7)
      jnc=fdata(8)
      knc=fdata(9)
      k=0
      do 40 kk=1,knc
      j=0
      do 30 jj=1,jnc
      i=0
      do 20 ii=1,inc
      ns=ns+1
      write(33,1000)itwo,ntie
      write(33,1000)nijk(i1+i,j1+j,k1+k),nijk(i2+i,j2+j,k2+k)
      i=i+1
   20 continue
      j=j+1
   30 continue
      k=k+1
   40 continue
      go to 10
   50 continue
      write(6,1010)ns
      return
 1000 format(6i5)
 1010 format(5x,'number of tie nodes written =',i5)
      end
      subroutine tie2
      implicit real*8 (a-h,o-z)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
      ns=0
      ntie=fdata(2)
   10 continue
      call ffin
      if(cdata(1).eq.'end')go to 50
      i1=fdata(1)
      j1=fdata(2)
      k1=fdata(3)
      i2=fdata(4)
      j2=fdata(5)
      k2=fdata(6)
      inc=fdata(7)
      jnc=fdata(8)
      knc=fdata(9)
      itwo=i2-i1+j2-j1+k2-k1+1
      k=0
      do 40 kk=1,knc
      j=0
      do 30 jj=1,jnc
      i=0
      do 20 ii=1,inc
      ns=ns+1
      write(33,1000)itwo,ntie
      write(33,1000)
     . (((nijk(i+il,j+jl,k+kl),il=i1,i2),jl=j1,j2),kl=k1,k2)
      i=i+1
   20 continue
      j=j+1
   30 continue
      k=k+1
   40 continue
      go to 10
   50 continue
      write(6,1010)ns
      return
 1000 format(15i5)
 1010 format(5x,'number of tie nodes written =',i5)
      end
      subroutine tie3
      implicit real*8 (a-h,o-z)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
      ns=0
      itwo=2
      ntie=fdata(2)
   10 continue
      call ffin
      if(cdata(1).eq.'end')go to 50
      i1=fdata(1)
      j1=fdata(2)
      k1=fdata(3)
      i2=fdata(4)
      j2=fdata(5)
      k2=fdata(6)
      inc=fdata(7)
      jnc=fdata(8)
      knc=fdata(9)
      k=0
      do 40 kk=1,knc
      j=0
      do 30 jj=1,jnc
      i=0
      do 20 ii=1,inc
      ns=ns+1
      write(33,1000)nijk(i1+i,j1+j,k1+k),nijk(i2+i,j2+j,k2+k),ntie,ntie
      i=i+1
   20 continue
      j=j+1
   30 continue
      k=k+1
   40 continue
      go to 10
   50 continue
      write(6,1010)ns
      return
 1000 format(6i5)
 1010 format(5x,'number of tie nodes written =',i5)
      end
      subroutine elem(neijk,in,jn)
      implicit real*8 (a-h,o-z)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      dimension neijk(in,jn,1)
      nel=0
   10 continue
      call ffin
      if(cdata(1).eq.'end')go to 900
      im=fdata(3)
      jm=fdata(4)
      km=fdata(5)
      call ffin
      is=fdata(1)
      if(is.le.0)is=1
      js=fdata(2)
      if(js.le.0)js=1
      ks=fdata(3)
      if(ks.le.0)ks=1
      ie=fdata(4)
      if(ie.le.is)ie=is
      je=fdata(5)
      if(je.lt.js)je=js
      ke=fdata(6)
      if(ke.lt.ks)ke=ks
      inc=fdata(7)
      if(inc.le.0)inc=1
      jnc=fdata(8)
      if(jnc.le.0)jnc=1
      knc=fdata(9)
      if(knc.le.0)knc=1
      incc=1
      do 130 kl=ks,ke,knc
      do 120 jl=js,je,jnc
      do 110 il=is,ie,inc
      i=im+il-is
      j=jm+jl-js
      k=km+kl-ks
      nel=nel+1
      neijk(i,j,k)=nel
  110 continue
  120 continue
  130 continue
      go to 10
  900 continue
      numel=nel
      write(6,1010)numel
      return
 1000 format(11i5)
 1010 format(5x,'elem number of elements written = ' ,i5)
      end
*deck add
      subroutine add(a,b,c,sm)
      implicit real*8 (a-h,o-z)
c
c        vector addition
c
      dimension a(3),b(3),c(3)
      do 10 i=1,3
      c(i)=a(i)+b(i)*sm
   10 continue
      return
      end
*deck cross
      subroutine cross(a,b,v,a1,n)
      implicit real*8 (a-h,o-z)
c
c        subroutine to find area and normal vector
c
      dimension a(3),b(3),v(3)
      v(1)=a(2)*b(3)-b(2)*a(3)
      v(2)=b(1)*a(3)-a(1)*b(3)
      v(3)=a(1)*b(2)-b(1)*a(2)
      a1=sqrt(v(1)**2+v(2)**2+v(3)**2)
      if(a1.ne.0.)go to 10
      write(6,1000)n
   10 continue
      do 20 j=1,3
      v(j)=v(j)/a1
   20 continue
      return
 1000 format(1h0,10x,'warning zero area element = ',i10)
      end
      subroutine cramin(x,word)
      implicit real*8 (a-h,o-z)
      character*10 word
      if(x.gt.-1.e7)go to 10
      write(word,1000)x
      return
   10 if(abs(x).gt.1.e-4.and.abs(x).lt.1.e8)go to 20
      write(word,1000)x
      return
   20 if(abs(x).ge.0.9999)go to 30
      write(word,2000)x
      return
   30 if(abs(x).ge.99.9999)go to 40
      write(word,3000)x
      return
   40 if(abs(x).ge.9999.)go to 50
      write(word,4000)x
      return
   50 if(abs(x).ge.1.e6)go to 60
      write(word,5000)x
      return
   60 continue
      write(word,6000)x
      return
 1000 format(1pe10.3)
 2000 format(f10.8)
 3000 format(f10.6)
 4000 format(f10.4)
 5000 format(f10.2)
 6000 format(f10.0)
      end
      subroutine siminf(xd,yd,zd,ibc,in,jn)
      implicit real*8 (a-h,o-z)
      dimension xd(in,jn,1),yd(in,jn,1),zd(in,jn,1),ibc(in,jn,1)
      common/dcom1/numel,imx,jmx,kmx,nmax,ntwo,nfam
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
      if(tol.le.0.)tol=1.e-4
      call loops(ibc,in,jn)
      n=0
      do 110 k=1,kmx
      do 100 j=1,jmx
      do 90 i=1,imx
      if(ibc(i,j,k).eq.0)go to 90
      n=n+1
      write(33,1000)n,nijk(i,j,k)
   90 continue
  100 continue
  110 continue
      write(6,1010)n
      return
 1000 format(2i5)
 1010 format(5x,'number of siminf nodes =',i5)
      end
      subroutine loops(ibc,in,jn)
      implicit real*8 (a-h,o-z)
      dimension ibc(in,jn,1)
      common/dcom1/numel,imx,jmx,kmx,nmax,ntwo,nfam
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
      do 30 k=1,kmx
      do 20 j=1,jmx
      do 10 i=1,imx
      ibc(i,j,k)=0
   10 continue
   20 continue
   30 continue
c
c          loop until end card
c
   40 continue
      call ffin
      if(cdata(1).eq.'end')go to 80
      i1=fdata(1)
      j1=fdata(2)
      k1=fdata(3)
      i2=fdata(4)
      j2=fdata(5)
      k2=fdata(6)
      if(i2.lt.i1)i2=i1
      if(j2.lt.j1)j2=j1
      if(k2.lt.k1)k2=k1
      do 70 k=k1,k2
      do 60 j=j1,j2
      do 50 i=i1,i2
      ibc(i,j,k)=1
   50 continue
   60 continue
   70 continue
      go to 40
   80 continue
      return
      end
      subroutine fill2(xd,yd,zd,ibc,in,jn)
      implicit real*8 (a-h,o-z)
      dimension xd(in,jn,*),yd(in,jn,*),zd(in,jn,*),ibc(in,jn,*)
      common/dcom1/numel,imx,jmx,kmx,nmax,ntwo,nfam
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
      imin=fdata(2)+1
      jmin=fdata(3)+1
      kmin=fdata(4)
      imax=fdata(5)-1
      jmax=fdata(6)-1
      kmax=fdata(7)
      omega=fdata(8)
      if(omega.eq.0.)omega=1.8
      iter=fdata(9)
      if(iter.eq.0)iter=100
      tol=fdata(10)
      if(tol.le.0.)tol=1.e-4
      call loops(ibc,in,jn)
      xnorm=max(
     .abs(xd(imin-1,jmin-1,kmin)-xd(imax+1,jmax+1,kmin)),
     .abs(yd(imax+1,jmax+1,kmin)-yd(imin-1,jmin-1,kmin))
     . )*(imax-imin)*(jmax-jmin)
      onine=1./8.
      omega1=1-omega
      do 240 it=1,iter
      resid=0.
      do 230 k=kmin,kmax
      do 220 j=jmin,jmax
      do 210 i=imin,imax
      if(ibc(i,j,k).ne.0)go to 210
      ipinc=i+1
      ininc=i-1
      jpinc=j+1
      jninc=j-1
      xnew=onine*(xd(i,jninc,k)+xd(ipinc,jninc,k)+
     .xd(ipinc,j,k)+xd(ipinc,jpinc,k)+xd(i,jpinc,k)+
     .xd(ininc,jpinc,k)+xd(ininc,j,k)+xd(ininc,jninc,k))
      ynew=onine*(yd(i,jninc,k)+yd(ipinc,jninc,k)+
     . yd(ipinc,j,k)+yd(ipinc,jpinc,k)+yd(i,jpinc,k)+
     . yd(ininc,jpinc,k)+yd(ininc,j,k)+yd(ininc,jninc,k))
      xold=xd(i,j,k)
      yold=yd(i,j,k)
      resid=resid+abs(xnew-xold)+abs(ynew-yold)
      xd(i,j,k)=omega*xnew+omega1*xold
      yd(i,j,k)=omega*ynew+omega1*yold
      zd(i,j,k)=zd(imin-1,jmin-1,k)
  210 continue
  220 continue
  230 continue
      resid=resid*omega/xnorm
      if(resid.lt.tol)go to 250
  240 continue
      write(6,1000)resid
  250 continue
      write(6,1010)it
      return
 1000 format(38h0fillin did not converge.  residual = ,1pe11.3)
 1010 format(5x,'number of iterations =',i5)
      end
*deck surface
      subroutine surface(xd,yd,zd,ibc,in,jn)
      implicit real*8 (a-h,o-z)
      dimension xd(*),yd(*),zd(*),ibc(*),ap(3)
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      dimension xb(5),yb(5),zb(5),nn(4)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
      character*8 p(6)
      one3=1.0/3.0
      one4=1.0/4.0
      ns=0
      nic=1
      inorm=0
      iswap=0
      ipoint=0
      ion=0
      iflip=fdata(2)
   10 continue
      call ffin
      if(cdata(1).eq.'end')go to 50
      if(cdata(1).eq.'family')then
        nfam=fdata(2)
        goto 10
      endif
      if(cdata(1).eq.'normal')then
        inorm=0
        if(cdata(2).eq.'on')inorm=1
        goto 10
      endif
      if(cdata(1).eq.'point')then
        ipoint=0
        if(cdata(2).eq.'on+'.or.cdata(2).eq.'on-')then
            ipoint=1
	    ion=0
	    if(cdata(2).eq.'on-')ion=1
            ap(1)=fdata(3)
            ap(2)=fdata(4)
            ap(3)=fdata(5)
        endif
        goto 10
      endif
      i1=fdata(1)
      j1=fdata(2)
      k1=fdata(3)
      i2=fdata(4)
      j2=fdata(5)
      k2=fdata(6)
      i3=fdata(7)
      j3=fdata(8)
      k3=fdata(9)
      i4=fdata(10)
      j4=fdata(11)
      k4=fdata(12)
      inc=fdata(13)
      jnc=fdata(14)
      knc=fdata(15)
      k=0
      do 40 kk=1,knc
      j=0
      do 30 jj=1,jnc
      i=0
      do 20 ii=1,inc
      ns=ns+1
      if(iflip.ne.0)go to 15
      nn(1)=nijk(i1+i,j1+j,k1+k)
      nn(2)=nijk(i2+i,j2+j,k2+k)
      nn(3)=nijk(i3+i,j3+j,k3+k)
      nn(4)=nijk(i4+i,j4+j,k4+k)
      if(nn(1).eq.nn(2))then
         nn(1)=nn(3)
         nn(3)=nn(2)
         nn(2)=nn(4)
         nn(4)=nn(3)
         iswap= iswap+1
      elseif(nn(1).eq.nn(4))then
         nn(1)=nn(2)
         nn(2)=nn(3)
         nn(3)=nn(4)
         iswap= iswap+1
      elseif(nn(2).eq.nn(3))then
         nsave=nn(1)
         nn(1)=nn(4)
         nn(2)=nsave
         nn(4)=nn(3)
         iswap= iswap+1
      endif
      if(ipoint.eq.1)call spoint(xd,yd,zd,nn,ap,ns,ion)
c     write(33,1000)ns,nic,nn(1),nn(2),
c    .              nn(3),nn(4)
      xb(1)=xd(nn(1))
      yb(1)=yd(nn(1))
      zb(1)=zd(nn(1))
      xb(2)=xd(nn(2))
      yb(2)=yd(nn(2))
      zb(2)=zd(nn(2))
      xb(3)=xd(nn(3))
      yb(3)=yd(nn(3))
      zb(3)=zd(nn(3))
      xb(4)=xd(nn(4))
      yb(4)=yd(nn(4))
      zb(4)=zd(nn(4))
      xb(5)=xb(1)
      yb(5)=yb(1)
      zb(5)=zb(1)
      go to 18
   15 continue
      nn(1)=nijk(i1+i,j1+j,k1+k)
      nn(2)=nijk(i4+i,j4+j,k4+k)
      nn(3)=nijk(i3+i,j3+j,k3+k)
      nn(4)=nijk(i2+i,j2+j,k2+k)
      if(nn(1).eq.nn(2))then
         nn(1)=nn(3)
         nn(3)=nn(2)
         nn(2)=nn(4)
         nn(4)=nn(3)
         iswap= iswap+1
      elseif(nn(1).eq.nn(4))then
         nn(1)=nn(2)
         nn(2)=nn(3)
         nn(3)=nn(4)
         iswap= iswap+1
      elseif(nn(2).eq.nn(3))then
         nsave=nn(1)
         nn(1)=nn(4)
         nn(2)=nsave
         nn(4)=nn(3)
         iswap= iswap+1
      endif
      if(ipoint.eq.1)call spoint(xd,yd,zd,nn,ap,ns,ion)
c     write(33,1000)ns,nic,nn(1),nn(2),
c    .              nn(3),nn(4)
      xb(1)=xd(nn(1))
      yb(1)=yd(nn(1))
      zb(1)=zd(nn(1))
      xb(2)=xd(nn(2))
      yb(2)=yd(nn(2))
      zb(2)=zd(nn(2))
      xb(3)=xd(nn(3))
      yb(3)=yd(nn(3))
      zb(3)=zd(nn(3))
      xb(4)=xd(nn(4))
      yb(4)=yd(nn(4))
      zb(4)=zd(nn(4))
      xb(5)=xb(1)
      yb(5)=yb(1)
      zb(5)=zb(1)
   18 continue
      if(nn(3).eq.nn(4))then
        xc=one3*(xb(1)+xb(2)+xb(3))
        yc=one3*(yb(1)+yb(2)+yb(3))
        zc=one3*(zb(1)+zb(2)+zb(3))
      else
        xc=one4*(xb(1)+xb(2)+xb(3)+xb(4))
        yc=one4*(yb(1)+yb(2)+yb(3)+yb(4))
        zc=one4*(zb(1)+zb(2)+zb(3)+zb(4))
      endif
      call cramin(xc,p(1))
      call cramin(yc,p(3))
      call cramin(zc,p(5))
      write(33,1020)ns,p(1),p(2),p(3),p(4),p(5),p(6)
      call line3d(xb,yb,zb,5,5,nfam)
      if(inorm.eq.1)call snorm(xb,yb,zb,nfam,ns)
      i=i+1
   20 continue
      j=j+1
   30 continue
      k=k+1
   40 continue
      go to 10
   50 continue
      write(6,1010)ns,iswap
      ns = -1
      write(33,1020)ns,p(1),p(2),p(3),p(4),p(5),p(6)
      return
 1000 format(6i5)
 1010 format(5x,'number of surfaces written =',i8,' iswap =',i8)
 1020 format(i5,1x,a8,a2,1x,a8,a2,1x,a8,a2)
 1030 format(i5,1pe12.4,1pe12.4,1pe12.4,1pe12.4)
      end
*deck slide
      subroutine slide(xd,yd,zd,ibc,in,jn)
      implicit real*8 (a-h,o-z)
      dimension xd(*),yd(*),zd(*),ibc(*),ap(3)
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      dimension xb(5),yb(5),zb(5),nn(4)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
      ns=0
      nic=1
      inorm=0
      iswap=0
      ipoint=0
      ion=0
      iflip=fdata(2)
   10 continue
      call ffin
      if(cdata(1).eq.'end')go to 50
      if(cdata(1).eq.'family')then
        nfam=fdata(2)
        goto 10
      endif
      if(cdata(1).eq.'normal')then
        inorm=0
        if(cdata(2).eq.'on')inorm=1
        goto 10
      endif
      if(cdata(1).eq.'point')then
        ipoint=0
        if(cdata(2).eq.'on+'.or.cdata(2).eq.'on-')then
            ipoint=1
	    ion=0
	    if(cdata(2).eq.'on-')ion=1
            ap(1)=fdata(3)
            ap(2)=fdata(4)
            ap(3)=fdata(5)
        endif
        goto 10
      endif
      i1=fdata(1)
      j1=fdata(2)
      k1=fdata(3)
      i2=fdata(4)
      j2=fdata(5)
      k2=fdata(6)
      i3=fdata(7)
      j3=fdata(8)
      k3=fdata(9)
      i4=fdata(10)
      j4=fdata(11)
      k4=fdata(12)
      inc=fdata(13)
      jnc=fdata(14)
      knc=fdata(15)
      k=0
      do 40 kk=1,knc
      j=0
      do 30 jj=1,jnc
      i=0
      do 20 ii=1,inc
      ns=ns+1
      if(iflip.ne.0)go to 15
      nn(1)=nijk(i1+i,j1+j,k1+k)
      nn(2)=nijk(i2+i,j2+j,k2+k)
      nn(3)=nijk(i3+i,j3+j,k3+k)
      nn(4)=nijk(i4+i,j4+j,k4+k)
      if(nn(1).eq.nn(2))then
         nn(1)=nn(3)
         nn(3)=nn(2)
         nn(2)=nn(4)
         nn(4)=nn(3)
         iswap= iswap+1
      elseif(nn(1).eq.nn(4))then
         nn(1)=nn(2)
         nn(2)=nn(3)
         nn(3)=nn(4)
         iswap= iswap+1
      elseif(nn(2).eq.nn(3))then
         nsave=nn(1)
         nn(1)=nn(4)
         nn(2)=nsave
         nn(4)=nn(3)
         iswap= iswap+1
      endif
      if(ipoint.eq.1)call spoint(xd,yd,zd,nn,ap,ns,ion)
      write(33,1000)ns,nic,nn(1),nn(2),
     .              nn(3),nn(4)
      xb(1)=xd(nn(1))
      yb(1)=yd(nn(1))
      zb(1)=zd(nn(1))
      xb(2)=xd(nn(2))
      yb(2)=yd(nn(2))
      zb(2)=zd(nn(2))
      xb(3)=xd(nn(3))
      yb(3)=yd(nn(3))
      zb(3)=zd(nn(3))
      xb(4)=xd(nn(4))
      yb(4)=yd(nn(4))
      zb(4)=zd(nn(4))
      xb(5)=xb(1)
      yb(5)=yb(1)
      zb(5)=zb(1)
      go to 18
   15 continue
      nn(1)=nijk(i1+i,j1+j,k1+k)
      nn(2)=nijk(i4+i,j4+j,k4+k)
      nn(3)=nijk(i3+i,j3+j,k3+k)
      nn(4)=nijk(i2+i,j2+j,k2+k)
      if(nn(1).eq.nn(2))then
         nn(1)=nn(3)
         nn(3)=nn(2)
         nn(2)=nn(4)
         nn(4)=nn(3)
         iswap= iswap+1
      elseif(nn(1).eq.nn(4))then
         nn(1)=nn(2)
         nn(2)=nn(3)
         nn(3)=nn(4)
         iswap= iswap+1
      elseif(nn(2).eq.nn(3))then
         nsave=nn(1)
         nn(1)=nn(4)
         nn(2)=nsave
         nn(4)=nn(3)
         iswap= iswap+1
      endif
      if(ipoint.eq.1)call spoint(xd,yd,zd,nn,ap,ns,ion)
      write(33,1000)ns,nic,nn(1),nn(2),
     .              nn(3),nn(4)
      xb(1)=xd(nn(1))
      yb(1)=yd(nn(1))
      zb(1)=zd(nn(1))
      xb(2)=xd(nn(2))
      yb(2)=yd(nn(2))
      zb(2)=zd(nn(2))
      xb(3)=xd(nn(3))
      yb(3)=yd(nn(3))
      zb(3)=zd(nn(3))
      xb(4)=xd(nn(4))
      yb(4)=yd(nn(4))
      zb(4)=zd(nn(4))
      xb(5)=xb(1)
      yb(5)=yb(1)
      zb(5)=zb(1)
   18 continue
      call line3d(xb,yb,zb,5,5,nfam)
      if(inorm.eq.1)call snorm(xb,yb,zb,nfam,ns)
      i=i+1
   20 continue
      j=j+1
   30 continue
      k=k+1
   40 continue
      go to 10
   50 continue
      write(6,1010)ns,iswap
      return
 1000 format(6i5)
 1010 format(5x,'number of slides written =',i8,' iswap =',i8)
      end
      subroutine spoint(xd,yd,zd,nn,ap,ne,ion)
      implicit real*8 (a-h,o-z)
      dimension xd(*),yd(*),zd(*),nn(4),ap(3)
      dimension a(3),b(3),an(3),z(3)
      dimension xb(3),yb(3),zb(3)
      do 10 i=1,3
      xb(i)=xd(nn(i))
      yb(i)=yd(nn(i))
      zb(i)=zd(nn(i))
   10 continue
      a(1)=xb(3)-xb(2)
      a(2)=yb(3)-yb(2)
      a(3)=zb(3)-zb(2)
      b(1)=xb(1)-xb(2)
      b(2)=yb(1)-yb(2)
      b(3)=zb(1)-zb(2)
      call cross(a,b,an,area,ne)
      if(area.le.0)return
      xx=(xb(1)+xb(2)+xb(3))/3.0
      yy=(yb(1)+yb(2)+yb(3))/3.0
      zz=(zb(1)+zb(2)+zb(3))/3.0
      z(1)=xx-ap(1)
      z(2)=yy-ap(2)
      z(3)=zz-ap(3)
      if((an(1)*z(1)+an(2)*z(2)+an(3)*z(3)).ge.0.0)then
c
c      Normals in the same direction
c
         if(ion.eq.1)then
             if(nn(3).eq.nn(4))then
                 ns=nn(2)
                 nn(2)=nn(3)
                 nn(3)=ns
                 nn(4)=ns
             else
                 ns=nn(2)
                 nn(2)=nn(4)
                 nn(4)=ns
             endif
         endif
      else
c
c      Normals in the oposite direction
c
         if(ion.eq.0)then
             if(nn(3).eq.nn(4))then
                 ns=nn(2)
                 nn(2)=nn(3)
                 nn(3)=ns
                 nn(4)=ns
             else
                 ns=nn(2)
                 nn(2)=nn(4)
                 nn(4)=ns
             endif
         endif
      endif
      return
      end
      subroutine snorm(xb,yb,zb,nfam,ne)
      implicit real*8 (a-h,o-z)
      dimension xb(4),yb(4),zb(4)
      dimension xx(2),yy(2),zz(2)
      dimension a(3),b(3),an(3)
      xx(1)=.25*(xb(1)+xb(2)+xb(3)+xb(4))
      yy(1)=.25*(yb(1)+yb(2)+yb(3)+yb(4))
      zz(1)=.25*(zb(1)+zb(2)+zb(3)+zb(4))
      a(1)=xb(3)-xb(2)
      a(2)=yb(3)-yb(2)
      a(3)=zb(3)-zb(2)
      b(1)=xb(1)-xb(2)
      b(2)=yb(1)-yb(2)
      b(3)=zb(1)-zb(2)
      call cross(a,b,an,area,ne)
      if(area.le.0)return
      area=sqrt(area)
      xx(2)=xx(1)+area*an(1)
      yy(2)=yy(1)+area*an(2)
      zz(2)=zz(1)+area*an(3)
      call line3d(xx,yy,zz,2,2,nfam)
      return
      end
*deck press
      subroutine press(xd,yd,zd,ibc,in,jn)
      implicit real*8 (a-h,o-z)
      dimension xd(*),yd(*),zd(*),ibc(*)
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      dimension xb(5),yb(5),zb(5)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata,p(2)
      nel=0
      ns=0
      inorm=0
    5 pi=fdata(2)
      call cramin(pi,p(1))
      icurve=fdata(3)
      if(icurve.le.0)icurve=1
      icinc=fdata(4)
   10 continue
      call ffin
      if(cdata(1).eq.'end')go to 50
      if(cdata(1).eq.'press')go to 5
      if(cdata(1).eq.'family')then
        nfam=fdata(2)
        goto 10
      endif
      if(cdata(1).eq.'normal')then
        inorm=0
        if(cdata(2).eq.'on')inorm=1
        goto 10
      endif
      i1=fdata(1)
      j1=fdata(2)
      k1=fdata(3)
      i2=fdata(4)
      j2=fdata(5)
      k2=fdata(6)
      i3=fdata(7)
      j3=fdata(8)
      k3=fdata(9)
      i4=fdata(10)
      j4=fdata(11)
      k4=fdata(12)
      inc=fdata(13)
      jnc=fdata(14)
      knc=fdata(15)
      k=0
      do 40 kk=1,knc
      j=0
      do 30 jj=1,jnc
      i=0
      do 20 ii=1,inc
      ns=ns+1
      write(33,1000)ns,icurve,nijk(i1+i,j1+j,k1+k),nijk(i2+i,j2+j,k2+k),
     .              nijk(i3+i,j3+j,k3+k),nijk(i4+i,j4+j,k4+k),p,p,p,p
      xb(1)=xd(nijk(i1+i,j1+j,k1+k))
      yb(1)=yd(nijk(i1+i,j1+j,k1+k))
      zb(1)=zd(nijk(i1+i,j1+j,k1+k))
      xb(2)=xd(nijk(i2+i,j2+j,k2+k))
      yb(2)=yd(nijk(i2+i,j2+j,k2+k))
      zb(2)=zd(nijk(i2+i,j2+j,k2+k))
      xb(3)=xd(nijk(i3+i,j3+j,k3+k))
      yb(3)=yd(nijk(i3+i,j3+j,k3+k))
      zb(3)=zd(nijk(i3+i,j3+j,k3+k))
      xb(4)=xd(nijk(i4+i,j4+j,k4+k))
      yb(4)=yd(nijk(i4+i,j4+j,k4+k))
      zb(4)=zd(nijk(i4+i,j4+j,k4+k))
      xb(5)=xd(nijk(i1+i,j1+j,k1+k))
      yb(5)=yd(nijk(i1+i,j1+j,k1+k))
      zb(5)=zd(nijk(i1+i,j1+j,k1+k))
      call line3d(xb,yb,zb,5,5,nfam)
      if(inorm.eq.1)call snorm(xb,yb,zb,nfam,ns)
      i=i+1
      icurve=icurve+icinc
   20 continue
      j=j+1
   30 continue
      k=k+1
   40 continue
      go to 10
   50 continue
      nel=ns
      call cpress(xd,yd,zd,ibc,nel,nee)
      write(6,1010)nel
      return
 1000 format(6i5,4(a8,a2),8x,2h0.)
 1010 format(5x,'number of pressures written =',i5)
      end
      subroutine press4(xd,yd,zd,ibc,nel,nee,nfam,icurve,pi,
     .          i1,j1,k1,i2,j2,k2,i3,j3,k3,i4,j4,k4)
      implicit real*8 (a-h,o-z)
      dimension xd(*),yd(*),zd(*),ibc(*)
      common/dumcom/dumval
      dimension xb(5),yb(5),zb(5),num(5)
      character*8 p(2)
c
      call cramin(pi,p(1))
c
      num(1)=nijk(i1,j1,k1)
      num(5)=num(1)
      ibc(num(1))=1
      num(2)=nijk(i2,j2,k2)
      ibc(num(2))=1
      num(3)=nijk(i3,j3,k3)
      ibc(num(3))=1
      num(4)=nijk(i4,j4,k4)
      ibc(num(4))=1
      nel=nel+1
      write(33,1000)nel,icurve,num(1),num(2),
     .              num(3),num(4),p,p,p,p
      n=0
      do 90 k2=1,5
      n=n+1
      ip=num(k2)
      xb(n)=xd(ip)
      yb(n)=yd(ip)
      zb(n)=zd(ip)
   90 continue
      call line3d(xb,yb,zb,5,5,nfam)
      return
 1000 format(6i5,4(a8,a2),8x,2h0.)
      end
*deck pressd2
      subroutine pressd2
      implicit real*8 (a-h,o-z)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata,p(2)
      ns=0
    5 pi=fdata(2)
      call cramin(pi,p(1))
      icurve=fdata(3)
      if(icurve.le.0)icurve=1
      icinc=fdata(4)
   10 continue
      call ffin
      if(cdata(1).eq.'end')go to 50
      if(cdata(1).eq.'pressd2')go to 5
      i1=fdata(1)
      j1=fdata(2)
      k1=fdata(3)
      i2=fdata(4)
      j2=fdata(5)
      k2=fdata(6)
      inc=fdata(7)
      jnc=fdata(8)
      knc=fdata(9)
      k=0
      do 40 kk=1,knc
      j=0
      do 30 jj=1,jnc
      i=0
      do 20 ii=1,inc
      ns=ns+1
      write(33,1000)ns,icurve,nijk(i1+i,j1+j,k1+k),nijk(i2+i,j2+j,k2+k),
     .             p,p
      i=i+1
      icurve=icurve+icinc
   20 continue
      j=j+1
   30 continue
      k=k+1
   40 continue
      go to 10
   50 continue
      write(6,1010)ns
      return
 1000 format(4i5,2(a8,a2),8x,2h0.,4x,1h1,4x,1h0)
 1010 format(5x,'number of Dyna2d pressures written =',i5)
      end
*deck wnodes
      subroutine noded2(xd,yd,zd,ibc,in,jn)
      implicit real*8 (a-h,o-z)
      dimension xd(in,jn,*),yd(in,jn,*),zd(in,jn,*),ibc(in,jn,*)
      common/dumcom/dumval
      dimension word(4)
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      common/size/nsize,n0,n4,n5
      common/flist/data(256),nflag(256),nword
      common/clist/cdata(256)
      common ia(10)
      character*8 cdata,word
      do 30 k=1,kmax
      do 20 j=1,jmax
      do 10 i=1,imax
      ibc(i,j,k)=0
   10 continue
   20 continue
   30 continue
c
c          loop until end card
c
   40 continue
      call ffin
      if(cdata(1).eq.'end')go to 80
      n=data(1)
      i1=data(2)
      j1=data(3)
      i2=data(4)
      j2=data(5)
      k1=1
      k2=1
      if(i2.lt.i1)i2=i1
      if(j2.lt.j1)j2=j1
      if(k2.lt.k1)k2=k1
      do 70 k=k1,k2
      do 60 j=j1,j2
      do 50 i=i1,i2
      ibc(i,j,k)=n
   50 continue
   60 continue
   70 continue
      go to 40
   80 continue
      nauto=0
      nlast=0
      do 110 k=1,kmax
      do 100 j=1,jmax
      do 90 i=1,imax
      nlast=nlast+1
      call cramin(xd(i,j,k),word(1))
      call cramin(yd(i,j,k),word(3))
c      call cramin(zd(i,j,k),word(5))
      ib2=ibc(i,j,k)/1000
      ib1=ibc(i,j,k)-1000*ib2
      if(xd(i,j,k).ne.dumval)go to 85
      nauto=nauto+1
      ib1=7
      ib2=7
   85 continue
      write(33,1000)nlast,ib1,word
   90 continue
  100 continue
  110 continue
      write(6,1010)nlast,nauto
      do 130 k=1,kmax
      do 120 j=1,jmax
      do 115 i=1,imax
      ibc(i,j,k)=0
  115 continue
  120 continue
  130 continue
      return
 1000 format(2i5,2(a8,a2),4x,1h1)
 1010 format(5x,'dyna2d nodes written = ',i6,' auto b.c. = ',i8)
      end
      subroutine elemd2(xd,yd,zd,ibc,in,jn)
      implicit real*8 (a-h,o-z)
      dimension xd(*),yd(*),zd(*),ibc(*)
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      common/dumcom/dumval
      dimension xb(5),yb(5),zb(5)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
      ns=0
      nee=0
    5 continue
      icurve=fdata(2)
      flip=fdata(3)
      if(icurve.le.0)icurve=1
   10 continue
      call ffin
      if(cdata(1).eq.'end')go to 50
      if(cdata(1).eq.'elemd2')go to 5
      if(cdata(1).eq.'family')then
        nfam=fdata(2)
        goto 10
      endif
      i1=fdata(1)
      j1=fdata(2)
      i2=fdata(3)
      j2=fdata(4)
      i3=fdata(5)
      j3=fdata(6)
      i4=fdata(7)
      j4=fdata(8)
      inc=fdata(9)
      jnc=fdata(10)
      k1=1
      k2=1
      k3=1
      k4=1
      knc=1
      k=0
      do 40 kk=1,knc
      j=0
      do 30 jj=1,jnc
      i=0
      do 20 ii=1,inc
      ns=ns+1
      if(flip.ne.0)go to 15
      write(33,1000)ns,nijk(i1+i,j1+j,k1+k),nijk(i2+i,j2+j,k2+k),
     .              nijk(i3+i,j3+j,k3+k),nijk(i4+i,j4+j,k4+k),icurve
      go to 18
   15 continue
      write(33,1000)ns,nijk(i1+i,j1+j,k1+k),nijk(i4+i,j4+j,k4+k),
     .              nijk(i3+i,j3+j,k3+k),nijk(i2+i,j2+j,k2+k),icurve
   18 continue
      xb(1)=xd(nijk(i1+i,j1+j,k1+k))
      yb(1)=yd(nijk(i1+i,j1+j,k1+k))
      zb(1)=zd(nijk(i1+i,j1+j,k1+k))
      xb(2)=xd(nijk(i2+i,j2+j,k2+k))
      yb(2)=yd(nijk(i2+i,j2+j,k2+k))
      zb(2)=zd(nijk(i2+i,j2+j,k2+k))
      xb(3)=xd(nijk(i3+i,j3+j,k3+k))
      yb(3)=yd(nijk(i3+i,j3+j,k3+k))
      zb(3)=zd(nijk(i3+i,j3+j,k3+k))
      xb(4)=xd(nijk(i4+i,j4+j,k4+k))
      yb(4)=yd(nijk(i4+i,j4+j,k4+k))
      zb(4)=zd(nijk(i4+i,j4+j,k4+k))
      xb(5)=xd(nijk(i1+i,j1+j,k1+k))
      yb(5)=yd(nijk(i1+i,j1+j,k1+k))
      zb(5)=zd(nijk(i1+i,j1+j,k1+k))
      ibc(nijk(i1+i,j1+j,k1+k))=1
      ibc(nijk(i2+i,j2+j,k2+k))=1
      ibc(nijk(i3+i,j3+j,k3+k))=1
      ibc(nijk(i4+i,j4+j,k4+k))=1
      if(xb(1).eq.dumval)nee=nee+1
      if(xb(2).eq.dumval)nee=nee+1
      if(xb(3).eq.dumval)nee=nee+1
      if(xb(4).eq.dumval)nee=nee+1
      call line3d(xb,yb,zb,5,5,nfam)
      i=i+1
   20 continue
      j=j+1
   30 continue
      k=k+1
   40 continue
      go to 10
   50 continue
      write(6,1010)ns,nee
      return
 1000 format(6i5,4x,1h1)
 1010 format(5x,'dyna2d elements  written =',i5,' nodes errors =',i5)
      end
      subroutine wnodes(xd,yd,zd,ibc,in,jn)
      implicit real*8 (a-h,o-z)
      dimension xd(in,jn,*),yd(in,jn,*),zd(in,jn,*),ibc(in,jn,*)
      common/dumcom/dumval
      dimension word(6)
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      common/size/nsize,n0,n4,n5
      common/flist/data(256),nflag(256),nword
      common/clist/cdata(256)
      common ia(10)
      character*8 cdata,word
      do 30 k=1,kmax
      do 20 j=1,jmax
      do 10 i=1,imax
      ibc(i,j,k)=0
   10 continue
   20 continue
   30 continue
c
c          loop until end card
c
   40 continue
      call ffin
      if(cdata(1).eq.'end')go to 80
      n=data(1)
      i1=data(2)
      j1=data(3)
      k1=data(4)
      i2=data(5)
      j2=data(6)
      k2=data(7)
      if(i2.lt.i1)i2=i1
      if(j2.lt.j1)j2=j1
      if(k2.lt.k1)k2=k1
      do 70 k=k1,k2
      do 60 j=j1,j2
      do 50 i=i1,i2
      ibc(i,j,k)=n
   50 continue
   60 continue
   70 continue
      go to 40
   80 continue
      nauto=0
      nlast=0
      do 110 k=1,kmax
      do 100 j=1,jmax
      do 90 i=1,imax
      nlast=nlast+1
      call cramin(xd(i,j,k),word(1))
      call cramin(yd(i,j,k),word(3))
      call cramin(zd(i,j,k),word(5))
      ib2=ibc(i,j,k)/1000
      ib1=ibc(i,j,k)-1000*ib2
      if(xd(i,j,k).ne.dumval)go to 85
      nauto=nauto+1
      ib1=7
      ib2=7
   85 continue
      write(33,1000)nlast,ib1,word,ib2
   90 continue
  100 continue
  110 continue
      write(6,1010)nlast,nauto
      do 130 k=1,kmax
      do 120 j=1,jmax
      do 115 i=1,imax
      ibc(i,j,k)=0
  115 continue
  120 continue
  130 continue
      return
 1000 format(2i5,3(a8,a2),4x,1h1,i5)
 1010 format(5x,'number of nodes written = ',i6,' auto b.c. = ',i8)
      end
      subroutine slid2
      implicit real*8 (a-h,o-z)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
      ns=0
      nic=1
      iflip=fdata(2)
   10 continue
      call ffin
      if(cdata(1).eq.'end')go to 50
      i1=fdata(1)
      j1=fdata(2)
      ir=fdata(3)
      jr=fdata(4)
c
      if(ir.lt.0)then
        ir  = -ir
        inc = -1
      else
        inc = 1
      endif
c
      if(jr.lt.0)then
        jr  = -jr
        jnc = -1
      else
        jnc = 1
      endif
c
      j=0
      do 30 jj=1,jr
      i=0
      do 20 ii=1,ir
      ns=ns+1
      write(33,1000)ns,nijk(i1+i,j1+j,1)
   18 continue
      i=i+inc
   20 continue
      j=j+jnc
   30 continue
      go to 10
   50 continue
      write(6,1010)ns
      return
 1000 format(6i5)
 1010 format(5x,'dyna2d slides written =',i5)
      end
      subroutine velbc(xd,yd,zd,ibc,in,jn)
      implicit real*8 (a-h,o-z)
      dimension xd(in,jn,*),yd(in,jn,*),zd(in,jn,*),ibc(in,jn,*)
      dimension word(8)
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata,word
      icurve=fdata(2)
      scale=fdata(3)
      vx=fdata(4)
      vy=fdata(5)
      vz=fdata(6)
      iaccel=fdata(7)
      nvel=0
c
c          loop until end card
c
   40 continue
      call ffin
      if(cdata(1).eq.'end')go to 80
      n=fdata(1)
      i1=fdata(2)
      j1=fdata(3)
      k1=fdata(4)
      i2=fdata(5)
      j2=fdata(6)
      k2=fdata(7)
      if(i2.lt.i1)i2=i1
      if(j2.lt.j1)j2=j1
      if(k2.lt.k1)k2=k1
      do 70 k=k1,k2
      do 60 j=j1,j2
      do 50 i=i1,i2
      nvel=nvel+1
      call cramin(scale,word(1))
      call cramin(vx,word(3))
      call cramin(vy,word(5))
      call cramin(vz,word(7))
      write(33,1000)nijk(i,j,k),icurve,n,word,iaccel
   50 continue
   60 continue
   70 continue
      go to 40
   80 continue
      ijkmax=nvel
      write(6,1010)ijkmax
      return
 1000 format(3i5,4(a8,a2),i5)
 1010 format(5x,'number of velbc written =',i6)
      end
      subroutine wvel(xd,yd,zd,ibc,in,jn)
      implicit real*8 (a-h,o-z)
      dimension xd(in,jn,*),yd(in,jn,*),zd(in,jn,*),ibc(in,jn,*)
      dimension word(6)
      dimension word2(4)
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata,word,word2
      idim=3
      if(fdata(2).ne.0.0)idim=2
      last=imax*jmax*kmax
      call vel(xd,yd,zd,last)
      nvel=0
      do 30 k=1,kmax
      do 20 j=1,jmax
      do 10 i=1,imax
      ibc(i,j,k)=0
   10 continue
   20 continue
   30 continue
c
c          loop until end card
c
   40 continue
      call ffin
      if(cdata(1).eq.'end')go to 80
      n=fdata(1)
      i1=fdata(2)
      j1=fdata(3)
      k1=fdata(4)
      i2=fdata(5)
      j2=fdata(6)
      k2=fdata(7)
      if(i2.lt.i1)i2=i1
      if(j2.lt.j1)j2=j1
      if(k2.lt.k1)k2=k1
      do 70 k=k1,k2
      do 60 j=j1,j2
      do 50 i=i1,i2
      ibc(i,j,k)=n
   50 continue
   60 continue
   70 continue
      go to 40
   80 continue
      do 110 k=1,kmax
      do 100 j=1,jmax
      do 90 i=1,imax
      nvel=nvel+1
      if(ibc(i,j,k).ne.0.and.nijk(i,j,k).eq.nvel)go to 85
      xd(i,j,k)=0.
      yd(i,j,k)=0.
      zd(i,j,k)=0.
   85 continue
      if(idim.eq.3)then
         call cramin(xd(i,j,k),word(1))
         call cramin(yd(i,j,k),word(3))
         call cramin(zd(i,j,k),word(5))
         write(33,1000)nvel,word
      else
         call cramin(xd(i,j,k),word2(1))
         call cramin(yd(i,j,k),word2(3))
         write(33,1020)nvel,word2
      endif      
   90 continue
  100 continue
  110 continue
      ijkmax=nvel
      if(idim.eq.3)then
         write(6,1010)ijkmax
      else
         write(6,1015)ijkmax
      endif
      return
 1000 format(i5,3(a8,a2),4x,1h1)
 1010 format(5x,'number of 3D velocities written =',i6)
 1015 format(5x,'number of 2D velocities written =',i6)
 1020 format(i5,2(a8,a2),4x,1h1)
      end
      subroutine wvelbc(xd,yd,zd,ibc,in,jn)
      implicit real*8 (a-h,o-z)
      dimension xd(in,jn,*),yd(in,jn,*),zd(in,jn,*),ibc(in,jn,*)
      dimension word(6)
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata,word
      icurve=fdata(2)
      nvel=0
      do 30 k=1,kmax
      do 20 j=1,jmax
      do 10 i=1,imax
      ibc(i,j,k)=0
   10 continue
   20 continue
   30 continue
c
c          loop until end card
c
   40 continue
      call ffin
      if(cdata(1).eq.'end')go to 80
      n=fdata(1)
      i1=fdata(2)
      j1=fdata(3)
      k1=fdata(4)
      i2=fdata(5)
      j2=fdata(6)
      k2=fdata(7)
      if(i2.lt.i1)i2=i1
      if(j2.lt.j1)j2=j1
      if(k2.lt.k1)k2=k1
      do 70 k=k1,k2
      do 60 j=j1,j2
      do 50 i=i1,i2
      ibc(i,j,k)=n
   50 continue
   60 continue
   70 continue
      go to 40
   80 continue
      do 110 k=1,kmax
      do 100 j=1,jmax
      do 90 i=1,imax
      if(ibc(i,j,k).eq.0)go to 90
      nvel=nvel+1
      call cramin(xd(i,j,k),word(1))
      call cramin(yd(i,j,k),word(3))
      call cramin(zd(i,j,k),word(5))
      write(33,1000)nijk(i,j,k),icurve,ibc(i,j,k),word
   90 continue
  100 continue
  110 continue
      ijkmax=nvel
      write(6,1010)ijkmax
      return
 1000 format(3i5,8x,2h1.,3(a8,a2))
 1010 format(5x,'number of velocities written =',i6)
      end
*deck nijk
      function nijk(i,j,k)
      implicit real*8 (a-h,o-z)
      common/size/nsize,n0,n4,n5
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      common ia(10)
      nijk=i+(j-1)*imax+(k-1)*imax*jmax
      nijk=ia(n0+nijk-1)
      return
      end
      subroutine welem(xd,yd,zd,ibc,in,jn)
      implicit real*8 (a-h,o-z)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
      common/dcom1/numel,imax,jmax,kmax,nmax,ntwo,nfam
      dimension ipl(5,4),xd(*),yd(*),zd(*),ibc(*)
      dimension xb(20),yb(20),zb(20),num(8)
      dimension ii(8),jj(8),kk(8),ni(8),nj(8),nk(8)
c     dimension io(8),jo(8),ko(8)
      data ipl/1,2,3,4,1, 5,6,7,8,5, 1,2,6,5,1, 4,3,7,8,4/
      data ni/0,1,1,0, 0,1,1,0/
      data nj/0,0,1,1, 0,0,1,1/
      data nk/0,0,0,0, 1,1,1,1/
      nel=0
      nee=0
   10 continue
      call ffin
      if(cdata(1).eq.'end')go to 900
      if(cdata(1).eq.'family')then
        nfam=fdata(2)
        goto 10
      endif
      mat=fdata(2)
      if(fdata(6).le.0.)go to 30
      k=3
      do 20 i=1,8
      ii(i)=fdata(k)
      jj(i)=fdata(k+1)
      kk(i)=fdata(k+2)
      k=k+3
   20 continue
      go to 50
   30 continue
      im=fdata(3)
      jm=fdata(4)
      km=fdata(5)
      do 40 i=1,8
      ii(i)=im+ni(i)
      jj(i)=jm+nj(i)
      kk(i)=km+nk(i)
   40 continue
   50 continue
      call ffin
      is=fdata(1)
      if(is.le.0)is=1
      js=fdata(2)
      if(js.le.0)js=1
      ks=fdata(3)
      if(ks.le.0)ks=1
      ie=fdata(4)
      if(ie.le.is)ie=is
      je=fdata(5)
      if(je.lt.js)je=js
      ke=fdata(6)
      if(ke.lt.ks)ke=ks
      inc=fdata(7)
      if(inc.le.0)inc=1
      jnc=fdata(8)
      if(jnc.le.0)jnc=1
      knc=fdata(9)
      if(knc.le.0)knc=1
      incc=1
      do 130 kl=ks,ke,knc
      do 120 jl=js,je,jnc
      do 110 il=is,ie,inc
      do 80 i=1,8
      num(i)=nijk(ii(i)+il-is,jj(i)+jl-js,kk(i)+kl-ks)
      ibc(num(i))=1
   80 continue
      nel=nel+1
      write(33,1000)nel,mat,incc,
     .  (num(m),m=1,8)
      n=0
      do 100 k3=1,4
      do 90 k2=1,5
      n=n+1
      ip=ipl(k2,k3)
      xb(n)=xd(num(ip))
      yb(n)=yd(num(ip))
      zb(n)=zd(num(ip))
   90 continue
  100 continue
      xc=.25*(xd(num(1))+xd(num(2))+xd(num(3))+xd(num(4))+
     .        xd(num(5))+xd(num(6))+xd(num(7))+xd(num(8)))
      yc=.25*(yd(num(1))+yd(num(2))+yd(num(3))+yd(num(4))+
     .        yd(num(5))+yd(num(6))+yd(num(7))+yd(num(8)))
      zc=.25*(zd(num(1))+zd(num(2))+zd(num(3))+zd(num(4))+
     .        zd(num(5))+zd(num(6))+zd(num(7))+zd(num(8)))
      rc=sqrt(xc**2+yc**2+zc**2)
c     write(*,*)' nel ',nel,' xc yc zc rc ',xc,yc,zc,rc
      call line3d(xb,yb,zb,20,5,nfam)
      call mbrick(nel,mat,num,xd,yd,zd)
  110 continue
  120 continue
  130 continue
      go to 10
  900 continue
      call cbrick(xd,yd,zd,ibc,nel,nee)
      numel=nel
      write(6,1010)numel
      return
 1000 format(11i5)
 1010 format(5x,'wlem number of elements written = ' ,i5)
      end
*deck memreq
      subroutine memreq(n)
      implicit real*8 (a-h,o-z)
      call memadj(n,ierror)
      if(ierror.eq.0)return
      write(6,1000)ierror
      stop
 1000 format(5x,'stop ierror = ',i5)
      end
      subroutine ffld(nin,nd,dat,cdat)
      implicit real*8 (a-h,o-z)
      dimension dat(*),cdat(*)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata,cdat
      call ffin
      do 10 i=1,128
      dat(i)=fdata(i)
      cdat(i)=cdata(i)
      if(i.le.nd)go to 10
      dat(i)=0
      fdata(i)=0
      cdat(i)=' '
      cdata(i)=' '
   10 continue
       return
       end
*deck ffin
      subroutine ffin
      implicit real*8 (a-h,o-z)
      common/tapes/ntape5,ntape6,ntape1,ntape2,ntape3,ntape4,ntape52
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*16 edum
      character*8 cdata,wlast
      character*1 sum(128),x(13),stemp(16),blank,comma,dollar,slash,save
      data x/'1','2','3','4','5','6','7','8','9','0','.','+','-'/
      data blank,comma,dollar,slash/' ', ',', '$', '/'/
      data iswap/0/
      data wlast/'last'/
      iline=1
   90 continue
      iline=iline+1
      do 100 j=1,256
      cdata(j)=blank
      nflag(j)=0
  100 fdata(j)=0.0
      nword=0
  120 continue
      read(ntape5,3000,end=350)sum
  125 continue
      ichar=0
  130 nword=nword+1
      if (nword.gt.256) go to 360
  140 ichar=ichar+1
      if (ichar.gt.128) go to 360
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
      if (ichar.gt.127) go to 200
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
      write(edum,3020)stemp
      read(edum,3040,end=380,err=380)fdata(nword)
      go to 340
  320 continue
      write(cdata(nword),3050)(stemp(i),i=1,8)
  340 continue
      if(sum(ichar).eq.slash) go to 120
      if(sum(ichar).eq.dollar)go to 360
      go to 130
  350 continue
c      nword=nword+1
  360 if(nword.gt.256)then
        nword=256
      endif
c      cdata(nword)=wlast
      return
  380 continue
      write(ntape6,3060)sum
      return
 3000 format(128a1)
 3010 format(i5,5x,128a1)
 3020 format(16a1)
 3030 format(4a1)
 3040 format(e16.0)
 3050 format(8a1)
 3060 format(' Error Decoding Line ',/,1x,128a1)
      end
*deck abort
      subroutine abort2
      implicit real*8 (a-h,o-z)
      n=0
      read(n)i
      go to (10,10,20,20),n
   10 continue
      return
   20 continue
      return
      end
*deck error1
      subroutine error1 (nchar,bcd,i,j,k,n,word,istop)
      implicit real*8 (a-h,o-z)
c
c     .....    error printing routine - istop=0 for return
c
      common /tapes/ nin,nout,ntape1,ntape2,ntape3,ntape4,ntape52
c
      dimension bcd(10)
      data kstop/0/
c
      nword=(nchar+7)/8
      nword=min0(8,iabs(nword))
      write(nout,10) (bcd(l),l=1,nword)
      if (i.ne.0.or.n.ne.0) write(nout,20) i,j,k,n,word
c
      kstop=kstop+1
      if (istop.eq.0.and.kstop.lt.50) return
c
      stop 1
c
   10 format(10h0error in ,10a8)
   20 format(/5h i = ,i5,5h j = ,i5,5h k = ,i5,5h n = ,i10,12h key word
     .= ,a10)
c
      end
*deck inv3
      subroutine inv3 (a,det)
      implicit real*8 (a-h,o-z)
c
c             inverts a 3x3 general matrix dimensioned 3x3 in the
c                               calling routine
c
      dimension a(9)
c
      a11=a(1)
      a21=a(2)
      a31=a(3)
      a12=a(4)
      a22=a(5)
      a32=a(6)
      a13=a(7)
      a23=a(8)
      a33=a(9)
      f11=a22*a33-a23*a32
      f22=a11*a33-a13*a31
      f33=a11*a22-a12*a21
      f12=a21*a33-a31*a23
      f21=a12*a33-a32*a13
      f13=a21*a32-a31*a22
      f31=a23*a12-a13*a22
      f23=a11*a32-a31*a12
      f32=a11*a23-a21*a13
      det=a11*f11-a12*f12+a13*f13
      if (det.eq.0.0)  return
      a(1)=f11/det
      a(2)=-f12/det
      a(3)=f13/det
      a(4)=-f21/det
      a(5)=f22/det
      a(6)=-f23/det
      a(7)=f31/det
      a(8)=-f32/det
      a(9)=f33/det
c
      return
c
      end
*deck seg3
      subroutine seg3 (x1,y1,z1,x2,y2,z2,grad,xs,ys,zs,n)
      implicit real*8 (a-h,o-z)
c
      common /tapes/ nin,nout,ntape1,ntape2,ntape3,ntape4,ntape52
c
      dimension xs(9999), ys(9999), zs(9999), s(9999)
c
c          divides a line with ends (x1,y1,z1) and (x2,y2,z2)
c          into n-1 segments with gradient grad......
c
c          rewritten 3/17/76 to call seg1
c
      if(grad.eq.0) grad=1.0
      call seg1 (grad,s,n)
      xa=(x1+x2)*.5
      ya=(y1+y2)*.5
      za=(z1+z2)*.5
      xd=(x2-x1)*.5
      yd=(y2-y1)*.5
      zd=(z2-z1)*.5
c
      xl=2.*sqrt(xd*xd+yd*yd+zd*zd)
      if(xl.ne.0.)go to 15
       write(nout,10)x1,y1,z1,x2,y2,z2,n
   10 format(13h0warningxl=0  ,6e11.3,i4)
c
      call abort2
   15 continue
      do 20 i=1,n
         xs(i)=xa+xd*s(i)
         ys(i)=ya+yd*s(i)
         zs(i)=za+zd*s(i)
   20 continue
c
      return
c
      end
*deck mesh3d
      subroutine mesh3d (xn,yn,zn,kbc,ni,nj,nk,iprint)
      implicit real*8 (a-h,o-z)
c
      common /ijck/ imin,jmin,kmin,imax,jmax,kmax,ic(3)
      common /tapes/ nin,nout,ntape1,ntape2,ntape3,ntape4,ntape5
      common /iodata/ data(128)
      common /iodatac/ cdata(128)
      character*8 cdata,word,cmmnd
      character*4 name
      character*8 qp
      common/transx/xyz(3,15),r(3,3,15),alpha(15),ng(15)
      common /el/ an(32),x(32),y(32),z(32),c11(9999),c12(9999),
     1 c13(9999),c21(9999),c22(9999),c23(9999),c31(9999),c32(9999),
     2 c33(9999),dt1(9999),dt2(9999),dt3(9999),db1(9999),db2(9999),
     3 db3(9999),psi(9999),eta(9999),zta(9999),c41(9999),c42(9999),
     4 c43(9999),g(12)
c
      dimension xn(ni,nj,nk),yn(ni,nj,nk),zn(ni,nj,nk),ip(3,3),word(28)
     .,name(3,5),ifg(4,3),ifg2(4,3),iface(12,3),qp(8),t(8)
      dimension iarc(4,3),a(3),b(3),grad(3,3),f1(3,3),f2(3,3),x1(3),
     .  x2(3),f(3),f3(3),kbc(ni,nj,*)
      common/dumcom/dumval
      data word/'csystm','point','pt','arc','arc-l','arc-q','arc-c',
     .          'face','face-l','face-q','face-c','block','block-l',
     .          'block-q','block-c','extdef','connect','fill',
     .          'copy','kill','normal','intrsct','end','blockrs',
     .          'facers','movex','movey','movez'/
      data imask0/-1/,imask1/1/
      data name/'cart','esia','n','cyli','ndri','cal','sphe','rica','l',
     1 'toro','idal',' ','exte','rnal',' def'/
      data iface/1,4,8,5,24,16,20,25,12,28,32,13,1,5,6,2,13,17,26,21,25,
     129,14,9,1,2,3,4,9,10,11,12,21,22,23,24/
      data ifg/2,11,6,10,12,7,11,3,5,6,7,8/,ip/0,0,1,0,1,0,1,0,0/
      data ifg2/4,12,8,9, 9,5,10,1, 1,2,3,4/
      data iarc/1,2,9,21,1,4,24,12,1,5,13,25/,istop/0/
      data qp/1hi,1hj,1hk,1hi,1hj,7h linear,8hquadrati,6h cubic/
      data nskip/1/
      data one/1.0d0/
c
      phi1(xx)=1.-3.*xx**2+2.*xx**3
      phi2(xx)=(xx-2.*xx**2+xx**3)*1.
      phi3(xx)=3.*xx**2-2.*xx**3
      phi4(xx)=(-xx**2+xx**3)*1.
c
      do 3 k=1,nk
      do 2 j=1,nj
      do 1 i=1,ni
      xn(i,j,k)=dumval
      yn(i,j,k)=dumval
      zn(i,j,k)=dumval
    1 continue
    2 continue
    3 continue
c
      maxlin=59
      line=maxlin
      if (nskip.eq.-1) go to 40
      nskip=-1
      ng(1)=1
      ng(2)=2
      ng(3)=3
      do 5 i=4,15
         ng(i)=0
    5 continue
      do 20 i=1,3
      do 20 j=1,3
         xyz(j,i)=0.
         do 10 k=1,3
   10    r(j,k,i)=0.
         r(j,j,i)=1.
   20 continue
      ic(1)=ni
      ic(2)=nj
      ic(3)=nk
   40 call ffld (nin,128,data,cdata)
      if (cdata(1).eq.word(23)) go to 860
      do 55 i=1,32
         x(i)=dumval
         y(i)=dumval
         z(i)=dumval
   55 continue
      imin=data(3)
      jmin=data(4)
      kmin=data(5)
      imax=data(6)
      jmax=data(7)
      kmax=data(8)
      if (cdata(1).eq.word(21)) go to 580
      nc=data(2)
      if (nc.eq.0) nc=1
      if (nc.ge.1.and.nc.le.15) go to 56
      write(nout,82) nc
      istop=istop+1
      go to 40
   56 cmmnd=cdata(1)
      do 60 n=1,28
         nn=n
         if (cmmnd.eq.word(n)) go to 70
   60 continue
      call error1 (27,27hmesh3d-unreconized command ,0,0,0,0,cmmnd,0)
      istop=istop+1
      go to 40
c
   70 go to (80,140,140,150,150,160,170,220,220,230,240,310,310,320,330,
     1 425,430,460,530,560,580,750,860,900,910,920,930,940), nn
c
c         define local coordinate system nc
c
   80 ng(nc)=data(3)
      if (ng(nc).le.5.and.ng(nc).ge.1) go to 85
      write (nout,82) nc,ng(nc)
   82 format(38h0illegal coordinate system type. nc =  ,i2,
     .  7h  ng =   ,i2)
      istop=istop+1
      ng(nc)=0
      go to 40
   85 xyz(1,nc)=data(4)
      xyz(2,nc)=data(5)
      xyz(3,nc)=data(6)
      alpha(nc)=data(13)
      dx=data(7)**2+data(8)**2+data(9)**2
      if (dx.le..0001) go to 90
      dot=data(7)*data(10)+data(8)*data(11)+data(9)*data(12)
      if(abs(dot).le.1.e-4) go to 87
      write (nout,86) nc
   86 format (32h0local axes of coordinate system,i3,
     . 14hnot orthogonal)
      istop=istop+1
      ng(nc)=0
      go to 40
   87 dx=sqrt(dx)
      r(1,1,nc)=data(7)/dx
      r(2,1,nc)=data(8)/dx
      r(3,1,nc)=data(9)/dx
      dz=data(10)**2+data(11)**2+data(12)**2
      if (dz.le..0001) go to 90
      dz=sqrt(dz)
      r(1,3,nc)=data(10)/dz
      r(2,3,nc)=data(11)/dz
      r(3,3,nc)=data(12)/dz
      r(1,2,nc)=r(2,3,nc)*r(3,1,nc)-r(3,3,nc)*r(2,1,nc)
      r(2,2,nc)=r(3,3,nc)*r(1,1,nc)-r(1,3,nc)*r(3,1,nc)
      r(3,2,nc)=r(1,3,nc)*r(2,1,nc)-r(2,3,nc)*r(1,1,nc)
      go to 120
   90 do 110 i=1,3
         do 100 j=1,3
  100    r(i,j,nc)=0.
  110 r(i,i,nc)=1.
  120 loc=ng(nc)
      if (iprint.le.2) go to 40
      line=line+7
      if (loc.eq.4) line=line+1
      if (line.lt.maxlin) go to 125
      line=8
      write (nout,124)
  124 format (1h1)
  125 write (nout,130) nc,(name(i,loc),i=1,3),(xyz(i,nc),i=1,3),
     1 ((r(i,j,nc),j=1,3),i=1,3)
  130 format (24h0local coordinate system,i3,5h  is ,3a4,/25h the local
     1origin is at  ,e10.3,2x,e10.3,2x,e10.3,/5x,19h direction cosines ,
     221x,5hlocal,/34x,1hx,11x,1hy,11x,1hz,/25x,1hx,4x,e10.3,2x,e10.3,2x
     3,e10.3,/10x,6hglobal,9x,1hy,4x,e10.3,2x,e10.3,2x,e10.3,/25x,1hz,4x
     4,e10.3,2x,e10.3,2x,e10.3)
      if (loc.eq.4) write (nout,135) alpha(nc)
  135 format (5x,24htoroidal radius alpha = ,e10.3)
      go to 40
c
c         define a single point
c
  140 imax=imin
      jmax=jmin
      kmax=kmin
      i=imin
      j=jmin
      k=kmin
      jstop=istop
      call icheck (istop,word(nn))
      if (istop.ne.jstop) go to 40
      xn(i,j,k)=data(6)
      yn(i,j,k)=data(7)
      zn(i,j,k)=data(8)
      call mshprt (xn,yn,zn,ni,nj,nk,cmmnd,iprint,nc,nc,ng,name,maxlin,l
     .ine)
      if (nc.eq.1) go to 40
      jstop=istop
      call trans (xn,yn,zn,ni,nj,nc,istop)
      if (istop.ne.jstop) go to 40
      if (iprint.gt.3) call mshprt (xn,yn,zn,ni,nj,nk,cmmnd,iprint,
     1  nc,1,ng,name,maxlin,line)
      go to 40
c
c         define a one dimensonal array (arc) of points
c
  150 int=1
      nf=8
      go to 180
  160 int=2
      nf=20
      go to 180
  170 int=3
      nf=32
  180 jstop=istop
      call icheck (istop,word(nn))
      if (jstop.ne.istop) go to 40
      l=1
      if (jmin.ne.jmax) l=2
      if (kmin.ne.kmax) l=3
      do 190 n=1,4
         if (n.gt.int+1) go to 200
         m=iarc(n,l)
      if (int.eq.2.and.m.gt.20) m=m-12
         mm=7+3*n
         x(m)=data(mm)
         y(m)=data(mm+1)
         z(m)=data(mm+2)
  190 continue
  200 continue
      if (data(9).eq.0.) data(9)=1.
      if (iprint.lt.4) go to 205
      line=line+2
      if (line.ge.maxlin) line=3
      if (line.eq.3) write (nout,124)
      write (nout,204) data(9)
  204 format (36h0gradient for next arc generation = ,e10.3)
  205 continue
      do 210 n=1,12
  210 g(n)=data(9)
      call steve(int,xn,yn,zn,ni,nj,nf,istop)
      call mshprt (xn,yn,zn,ni,nj,nk,cmmnd,iprint,nc,nc,ng,name,maxlin,l
     .ine)
      if (nc.eq.1) go to 40
      jstop=istop
      call trans (xn,yn,zn,ni,nj,nc,istop)
      if (istop.ne.jstop) go to 40
      if (iprint.gt.3) call mshprt (xn,yn,zn,ni,nj,nk,cmmnd,iprint,
     1  nc,1,ng,name,maxlin,line)
      go to 40
c
c         define a two dimensonal array (face) of points
c
  220 int=1
      nf=8
      go to 250
  230 int=2
      nf=20
      go to 250
  240 int=3
      nf=32
  250 jstop=istop
      call icheck (istop,word(nn))
      if (istop.ne.jstop) go to 40
      l=1
      if (jmin.eq.jmax) l=2
      if (kmin.eq.kmax) l=3
      do 260 n=1,12
  260 g(n)=1.
      do 270 n=1,4
         m=ifg(n,l)
         if (data(n+8).eq.0.) data(n+8)=1.
         g(m)=data(n+8)
         mm=ifg2(n,l)
         g(mm)=g(m)
  270 continue
      if (iprint.lt.4) go to 275
      line=line+5
      if (line.ge.maxlin) line=6
      if (line.eq.6) write (nout,124)
      write (nout,274) (i,data(i+8),i=1,4)
  274 format (35h0gradients for next face generation,//(10x,2hg(,i1,
     .  4h) = ,e10.3,10x,2hg(,i1,4h) = ,e10.3))
  275 continue
      call ffld (nin,128,data,cdata)
      do 280 n=1,4
         m=iface(n,l)
         mm=3*n-2
         x(m)=data(mm)
         y(m)=data(mm+1)
  280 z(m)=data(mm+2)
      if (int.eq.1) go to 300
      call ffld (nin,128,data,cdata)
      do 290 n=1,32,4
         nm=data(n)
         if (nm.eq.0) go to 300
         if (nm.ge.5.and.nm.le.12) go to 285
         istop=istop+1
      call error1(38,38hmesh3d-bad face interpolation point no ,imin,
     . jmin,kmin,nm,1h ,0)
         go to 40
  285    m=iface(nm,l)
      if (int.eq.2.and.m.gt.20) m=m-12
      x(m)=data(n+1)
      y(m)=data(n+2)
  290 z(m)=data(n+3)
  300 call steve(int,xn,yn,zn,ni,nj,nf,istop)
      call mshprt (xn,yn,zn,ni,nj,nk,cmmnd,iprint,nc,nc,ng,name,maxlin,l
     .ine)
      if (nc.eq.1) go to 40
      jstop=istop
      call trans (xn,yn,zn,ni,nj,nc,istop)
      if (istop.ne.jstop) go to 40
      if (iprint.gt.3) call mshprt (xn,yn,zn,ni,nj,nk,cmmnd,iprint,
     1  nc,1,ng,name,maxlin,line)
      go to 40
c
c         define a three dimensional array (block) of points
c
  310 int=1
      nf=8
      go to 340
  320 int=2
      nf=20
      go to 340
  330 int=3
      nf=32
  340 jstop=istop
      call icheck (istop,word(nn))
      if (istop.ne.jstop) go to 40
      do 350 n=1,12
  350 g(n)=1.
      do 360 n=9,32,2
         m=data(n)
         if (m.eq.0) go to 370
      if (m.gt.0.and.m.le.12) go to 360
      istop=istop+1
      call error1(24,24h mesh3d-bad gradient no ,imin,jmin,kmin,m,1h ,0)
  360 g(m)=data(n+1)
  370 if (iprint.lt.4) go to 375
      line=line+9
      if (line.ge.maxlin) line=10
      if (line.eq.10) write (nout,124)
      write (nout,371) (i,g(i),i=1,12)
  371 format (36h0gradients for next block generation,//(10x,2hg(,i2,
     .  4h) = ,e10.3,10x,2hg(,i2,4h) = ,e10.3))
  375 call ffld(nin,128,data,cdata)
      do 380 m=1,8
         n=3*m-2
         x(m)=data(n)
         y(m)=data(n+1)
  380 z(m)=data(n+2)
      if (int.eq.1) go to 400
      call ffld(nin,128,data,cdata)
      do 390 n=1,64,4
         m=data(n)
         if (m.eq.0) go to 400
      if (m.ge.9.and.m.le.nf) go to 385
         istop=istop+1
      call error1 (30,30hmesh3d-interpolation point no ,imin,jmin,kmin,m
     .  ,1h ,0)
         go to 40
  385    x(m)=data(n+1)
         y(m)=data(n+2)
  390 z(m)=data(n+3)
  400 call steve(int,xn,yn,zn,ni,nj,nf,istop)
      call mshprt (xn,yn,zn,ni,nj,nk,cmmnd,iprint,nc,nc,ng,name,maxlin,l
     .ine)
      if (nc.eq.1) go to 40
      jstop=istop
      call trans (xn,yn,zn,ni,nj,nc,istop)
      if (istop.ne.jstop) go to 40
      if (iprint.gt.3) call mshprt (xn,yn,zn,ni,nj,nk,cmmnd,iprint,
     1  nc,1,ng,name,maxlin,line)
      go to 40
c
c         external definition of nodes
c
  425 adat=data(4)
      bdat=data(5)
      cdat=data(6)
      call extdef(xn,yn,zn,ni,nj,adat,bdat,cdat,istop)
      go to 40
c
c         define a one dimensional array of points along
c              an arc connecting two points
c
  430 n1=data(10)
      if(n1.le.0)n1=1
      n2=data(11)
      if(n2.le.0)n2=1
      n3=data(12)
      if(n3.le.0)n3=1
      m=max0(imax-imin,jmax-jmin,kmax-kmin)+1
      do 450 in=1,n1
      do 450 jn=1,n2
      do 450 kn=1,n3
         imin=data(3)-.9+in
         jmin=data(4)-.9+jn
         kmin=data(5)-.9+kn
         imax=data(6)-.9+in
         jmax=data(7)-.9+jn
         kmax=data(8)-.9+kn
         jstop=istop
         call icheck (istop,word(nn))
         if (istop.ne.jstop) go to 40
         call untran (xn(imin,jmin,kmin),yn(imin,jmin,kmin),zn(imin,jmin
     . ,kmin),a1,b1,c1,nc,istop)
         call untran (xn(imax,jmax,kmax),yn(imax,jmax,kmax),zn(imax,jmax
     . ,kmax),a2,b2,c2,nc,istop)
         call seg3 (a1,b1,c1,a2,b2,c2,data(9),c11,c12,c13,m)
         l=0
         do 440 i=imin,imax
         do 440 j=jmin,jmax
         do 440 k=kmin,kmax
            l=l+1
            xn(i,j,k)=c11(l)
            yn(i,j,k)=c12(l)
            zn(i,j,k)=c13(l)
  440    continue
      call mshprt (xn,yn,zn,ni,nj,nk,cmmnd,iprint,nc,nc,ng,name,maxlin,l
     .ine)
      if (nc.eq.1) go to 450
      jstop=istop
      call trans (xn,yn,zn,ni,nj,nc,istop)
      if (istop.ne.jstop) go to 40
      if (iprint.gt.3) call mshprt (xn,yn,zn,ni,nj,nk,cmmnd,iprint,
     1  nc,1,ng,name,maxlin,line)
  450 continue
      go to 40
c
c          fill a three dimensional array using laplace averaging
c
  460 jstop=istop
      call icheck (istop,word(nn))
      if (istop.ne.jstop) go to 40
      i=imin
  461 do 462 j=jmin,jmax
      do 462 k=kmin,kmax
         if (zn(i,j,k).ne.dumval) go to 462
      call error1 (37,37hmesh3d-point on ext of fill undefined ,i,j,k,
     . 0,1h ,0)
         istop=istop+1
         go to 40
  462 continue
      if (i.eq.imax) go to 463
      i=imax
      go to 461
  463 j=jmin
  464 do 465 i=imin,imax
      do 465 k=kmin,kmax
         if (zn(i,j,k).ne.dumval) go to 465
      call error1 (37,37hmesh3d-point on ext of fill undefined ,i,j,k,
     . 0,1h ,0)
         istop=istop+1
         go to 40
  465 continue
      if (j.eq.jmax) go to 466
      j=jmax
      go to 464
  466 k=kmin
  467 do 468 i=imin,imax
      do 468 j=jmin,jmax
         if (zn(i,j,k).ne.dumval) go to 468
      call error1 (37,37hmesh3d-point on ext of fill undefined ,i,j,k,
     . 0,1h ,0)
         istop=istop+1
         go to 40
  468 continue
      if (k.eq.kmax) go to 469
      k=kmax
      go to 467
  469 i1=imin+1
      i2=imax-1
      j1=jmin+1
      j2=jmax-1
      k1=kmin+1
      k2=kmax-1
      if ((i2-i1)*(j2-j1)*(k2-k1).le.0) go to 510
      do 470 i=i1,i2
      do 470 j=j1,j2
      do 470 k=k1,k2
         kbc(i,j,k)=0
         if (zn(i,j,k).ne.dumval) kbc(i,j,k)=1
  470 continue
      w=1.9
      do 490 it=1,100
         delta=0.
         do 480 i=i1,i2
         do 480 j=j1,j2
         do 480 k=k1,k2
            if (kbc(i,j,k).eq.1)  go to 480
            dx=(xn(i+1,j,k)+xn(i-1,j,k)+xn(i,j+1,k)+xn(i,j-1,k)+xn(i,j,k
     1      +1)+xn(i,j,k-1))/6.-xn(i,j,k)
            dy=(yn(i+1,j,k)+yn(i-1,j,k)+yn(i,j+1,k)+yn(i,j-1,k)+yn(i,j,k
     1      +1)+yn(i,j,k-1))/6.-yn(i,j,k)
            dz=(zn(i+1,j,k)+zn(i-1,j,k)+zn(i,j+1,k)+zn(i,j-1,k)+zn(i,j,k
     1      +1)+zn(i,j,k-1))/6.-zn(i,j,k)
            delta=delta+dx**2+dy**2+dz**2
            xn(i,j,k)=xn(i,j,k)+w*dx
            yn(i,j,k)=yn(i,j,k)+w*dy
            zn(i,j,k)=zn(i,j,k)+w*dz
  480    continue
         if (delta.gt..01*xn(imax,jmax,kmax)) go to 490
  485    call mshprt (xn,yn,zn,ni,nj,nk,cmmnd,iprint,1,1,ng,name,maxlin,
     .   line)
         go to 40
  490 continue
      write (nout,500) delta
  500 format (61h laplace iteration in fill command did not converge; de
     1lta = ,e10.3)
      istop=istop+1
      go to 40
  510 istop=istop+1
      write (nout,520) imin,jmin,kmin,imax,jmax,kmax
  520 format (41h attempt to fill zero or negative volume ,6i3)
      go to 40
c
c         copy an array of nodes to new location
c
  530 in=data(9)
      jn=data(10)
      kn=data(11)
      id=in-imin
      jd=jn-jmin
      kd=kn-kmin
      imax=in+imax-imin
      jmax=jn+jmax-jmin
      kmax=kn+kmax-kmin
      imin=in
      jmin=jn
      kmin=kn
      jstop=istop
      call icheck (istop,word(nn))
      if (istop.ne.jstop) go to 40
      do 540 i=imin,imax
      do 540 j=jmin,jmax
      do 540 k=kmin,kmax
      xn(i,j,k)=xn(i-id,j-jd,k-kd)
      yn(i,j,k)=yn(i-id,j-jd,k-kd)
      zn(i,j,k)=zn(i-id,j-jd,k-kd)
  540 continue
      if (ng(nc).ne.1) write (nout,550) imin,jmin,kmin
  550 format(71h0warning - copy attempted with non-cartesian transformat
     .ion / i1,i2,i3 ,/3i3)
      call mshprt (xn,yn,zn,ni,nj,nk,cmmnd,iprint,nc,nc,ng,name,maxlin,l
     .ine)
      if (nc.eq.1) go to 40
      jstop=istop
      call trans (xn,yn,zn,ni,nj,nc,istop)
      if (istop.ne.jstop) go to 40
      if (iprint.gt.3) call mshprt (xn,yn,zn,ni,nj,nk,cmmnd,iprint,
     1  nc,1,ng,name,maxlin,line)
      go to 40
c
c         delete array of points
c
  560 jstop=istop
      call icheck (istop,word(nn))
      if (istop.ne.jstop) go to 40
      do 570 i=imin,imax
      do 570 j=jmin,jmax
      do 570 k=kmin,kmax
         xn(i,j,k)=dumval
         yn(i,j,k)=dumval
         zn(i,j,k)=dumval
  570 continue
      if (iprint.ge.3) write (nout,575) imin,jmin,kmin,imax,jmax,kmax
  575 format (19h0points from i j k ,3i3,10h to i j k ,3i3,8h deleted)
      go to 40
c
c     generate layers of points
c
  580 continue
      nlyr=data(2)
      id=1
      if (nlyr.lt.0) id=-1
      imn=imin
      imx=imax
      jmn=jmin
      jmx=jmax
      kmn=kmin
      kmx=kmax
      if (imn.ne.imx) go to 581
      itype=1
      imin=jmn
      imax=jmx
      jmin=kmn
      jmax=kmx
      kmin=imn
      kmax=imx+iabs(nlyr)-1
      go to 583
  581 continue
      if (jmx.ne.jmn) go to 582
      itype=2
      imin=kmn
      imax=kmx
      jmin=imn
      jmax=imx
      kmin=jmn
      kmax=jmx+iabs(nlyr)-1
      go to 583
  582 continue
      if (kmx.eq.kmn) go to 586
      call error1(29,29hmesh3d-error in normal bounds,imin,jmin,kmin,0,
     1 1h ,0)
      istop=istop+1
      go to 40
  586 itype=3
      kmin=kmn
      kmax=kmx+iabs(nlyr)-1
  583 continue
      nn=23
      jstop=istop
      call icheck (istop,word(nn))
      if (istop.ne.jstop) go to 40
      m2=(imax-imin+1)*(jmax-jmin+1)*(kmax-kmin+1)
      do 585 i=1,8
  585 t(i)=data(i+8)
      int=1
      do 588 i=2,4
      if (t(i).ne.0.0) go to 589
  588 continue
      do 584 i=2,4
  584 t(i)=t(1)
      go to 5891
  589 continue
      do 587 i=5,8
      if (t(i).ne.0.0) int=2
      j=i-3
      if (i.eq.8) j=1
      if (t(i).eq.0.0) t(i)=0.5*(t(i-4)+t(j))
  587 continue
 5891 continue
      si=imin
      bi=imax
      ai=(bi-si)/2.0
      ac=(bi+si)/2.0
      sj=jmin
      bj=jmax
      aj=(bj-sj)/2.0
      ad=(bj+sj)/2.0
      it=4+4*(int-1)
      write(nout,590) qp(itype),kmin,qp(itype+1),imin,qp(itype+2),jmin,
     1 qp(itype+1),imax,qp(itype+2),jmax,nlyr,(t(i),i=1,it)
  590 format(/////3x,a1,5hplane,3h = ,i2,1x,55hgeneration of grid points
     1 normal to surface bounded by ,/6x,a1,5hmin =,i2,1x,a1,5hmin =,i2,
     21x,a1,5hmax =,i2,1x,a1,5hmax =,i2,7h nlyr =,i3,19h of nodes genera
     3ted,/6x,11hthickness =,8e12.4)
      write (nout,600)
  600 format (/, 16h     i    j    k,9x,1hx,14x,1hy,14x,1hz)
      nd=0
      do 740 i=imin,imax
      do 740 j=jmin,jmax
         ps=(si-ac)/ai
         et=(sj-ad)/aj
         an(1)=(1.-ps)*(1.-et)/4.0
         an(2)=(1.+ps)*(1.-et)/4.0
         an(3)=(1.+ps)*(1.+et)/4.0
         an(4)=(1.-ps)*(1.+et)/4.0
         if (t(5).eq.0.) go to 610
         an(1)=an(1)*(-ps-et-1.0)
         an(2)=an(2)*(ps-et-1.0)
         an(3)=an(3)*(ps+et-1.0)
         an(4)=an(4)*(-ps+et-1.0)
         an(5)=(1.-ps)*(1.+ps)*(1.-et)/2.0
         an(6)=(1.+ps)*(1.-et)*(1.+et)/2.0
         an(7)=(1.-ps)*(1.+ps)*(1.+et)/2.0
         an(8)=(1.-ps)*(1.-et)*(1.+et)/2.0
  610    th=0.
         do 620 k=1,8
  620    th=th+an(k)*t(k)
         th=th/nlyr
      k=kmin
         i1=i+1
         j1=j+1
         if (i.eq.imax) i1=i-1
         if (j.eq.jmax) j1=j-1
         go to (640,630,650), itype
  630    xa=xn(j,k,i1)-xn(j,k,i)
         yb=yn(j,k,i1)-yn(j,k,i)
         zc=zn(j,k,i1)-zn(j,k,i)
         xd=xn(j1,k,i)-xn(j,k,i)
         ye=yn(j1,k,i)-yn(j,k,i)
         zf=zn(j1,k,i)-zn(j,k,i)
         go to 660
  640    xa=xn(k,i1,j)-xn(k,i,j)
         yb=yn(k,i1,j)-yn(k,i,j)
         zc=zn(k,i1,j)-zn(k,i,j)
         xd=xn(k,i,j1)-xn(k,i,j)
         ye=yn(k,i,j1)-yn(k,i,j)
         zf=zn(k,i,j1)-zn(k,i,j)
         go to 660
  650    xa=xn(i1,j,k)-xn(i,j,k)
         yb=yn(i1,j,k)-yn(i,j,k)
         zc=zn(i1,j,k)-zn(i,j,k)
         xd=xn(i,j1,k)-xn(i,j,k)
         ye=yn(i,j1,k)-yn(i,j,k)
         zf=zn(i,j1,k)-zn(i,j,k)
  660    io=i1-i
         jo=j1-j
         xa=io*xa
         yb=io*yb
         zc=io*zc
         xd=jo*xd
         ye=jo*ye
         zf=jo*zf
         xq=yb*zf-zc*ye
         yq=zc*xd-xa*zf
         zq=xa*ye-yb*xd
         tl=(xq*xq+yq*yq+zq*zq)**(0.5)
         xq=xq*th/tl
         yq=yq*th/tl
         zq=zq*th/tl
         n=0
         do 730 k=kmin,kmax
            n=n+1
            ii=ip(itype,1)*i+ip(itype,2)*j+ip(itype,3)*(k+(n-1)*(id-1))
            jj=ip(itype,3)*i+ip(itype,1)*j+ip(itype,2)*(k+(n-1)*(id-1))
            kk=ip(itype,2)*i+ip(itype,3)*j+ip(itype,1)*(k+(n-1)*(id-1))
            iii=ip(itype,1)*i+ip(itype,2)*j+ip(itype,3)*(k+n*(id-1)+1)
            jjj=ip(itype,3)*i+ip(itype,1)*j+ip(itype,2)*(k+n*(id-1)+1)
            kkk=ip(itype,2)*i+ip(itype,3)*j+ip(itype,1)*(k+n*(id-1)+1)
            xn(iii,jjj,kkk)=xn(ii,jj,kk)+xq
            yn(iii,jjj,kkk)=yn(ii,jj,kk)+yq
            zn(iii,jjj,kkk)=zn(ii,jj,kk)+zq
      if(iprint.lt.4)go to 730
            nd=nd+1
  670       if (nd-ia) 710,690,680
  680       ia=ia+58
            go to 670
  690       write (nout,700)
  700 format (1h1)
            write (nout,600)
  710       write (nout,720) iii,jjj,kkk,xn(iii,jjj,kkk),yn(iii,jjj,kkk)
     1       ,zn(iii,jjj,kkk)
  720 format (1x,3i5,3e15.4)
  730    continue
         si=si+1.0
         sj=sj+1.0
  740 continue
      ia=ia-m2-10
      go to 40
c
c     calculate points along the intersection of two surfaces
c
  750 jstop=istop
      call icheck (istop,data)
      if (istop.ne.jstop) go to 40
      eps=data(2)
      if (eps.eq.0.0) eps=0.01
      call ffld (nin,128,data,cdata)
      nc1=data(1)
      n1=data(2)
      a(1)=data(3)
      a(2)=data(4)
      a(3)=data(5)
      m=data(6)
      d8=data(7)
      d9=data(8)
      g1=data(9)
      if (g1.eq.0.) g1=1.
      nn=max0(imax-imin,jmax-jmin,kmax-kmin)+1
      call seg3 (d8,one,one,d9,one,one,g1,c11,c12,c12,nn)
      call ffld (nin,128,data,cdata)
      nc2=data(1)
      n2=data(2)
      b(1)=data(3)
      b(2)=data(4)
      b(3)=data(5)
      eps=eps*eps
      ij=1
      do 850 i=imin,imax
      do 850 j=jmin,jmax
      do 850 k=kmin,kmax
         a(m)=c11(ij)
         ij=ij+1
         do 820 iter=1,100
            call surf (a,x1,f1,nc1)
            call surf (b,x2,f2,nc2)
      if (mod(iter,2).ne.0) go to 759
c
c         average new estimates every 2 cycles
c
      do 751 n=1,3
  751 grad(n,1)=(x1(n)+x2(n))/2.0
      call untran(grad(1,1),grad(2,1),grad(3,1),grad(1,2),grad(2,2),
     . grad(3,2),nc1,istop)
      do 752 n=1,3
      if (n.ne.m.and.n.ne.n1) a(n)=grad(n,2)
  752 continue
      call untran(grad(1,1),grad(2,1),grad(3,1),grad(1,2),grad(2,2),
     . grad(3,2),nc2,istop)
      do 753 n=1,3
      if (n.ne.n2) b(n)=grad(n,2)
  753 continue
  759 continue
            del=0.
      do 760 n=1,3
      f3(n)=x2(n)-x1(n)
  760 del=del+f3(n)**2
            if (del.lt.eps) go to 830
            do 780 n=1,3
               if (n.eq.n1.or.n.eq.m) go to 780
               do 770 l=1,3
  770          grad(l,1)=f1(l,n)
  780       continue
            nn=1
            do 800 n=1,3
               if (n.eq.n2) go to 800
               nn=nn+1
               do 790 l=1,3
  790          grad(l,nn)=-f2(l,n)
  800       continue
      call inv3 (grad,det)
  805 do 807 n=1,3
      f(n)=0.0
      do 807 nn=1,3
  807 f(n)=f(n)+grad(n,nn)*f3(nn)
            nn=1
            do 810 n=1,3
               if (n.ne.n1.and.n.ne.m) a(n)=f(1)+a(n)
               if (n.eq.n2) go to 810
               nn=nn+1
               b(n)=f(nn)+b(n)
  810       continue
      if (iprint.gt.4) write(nout,840) iter
     1      ,del,x1,x2
  820    continue
  830    xn(i,j,k)=(x1(1)+x2(1))/2.
         yn(i,j,k)=(x1(2)+x2(2))/2.
         zn(i,j,k)=(x1(3)+x2(3))/2.
  840 format (37h trace  intrsection/iter,delta,x1,x2 /i5,e10.3,6f12.5)
  850 continue
      call mshprt (xn,yn,zn,ni,nj,nk,cmmnd,iprint,1,1,ng,name,maxlin,
     . line )
      go to 40
  860 call mshprt(xn,yn,zn,ni,nj,nk,3hend,iprint,1,1,ng,name,maxlin,
     . line )
      call mshplt (xn,yn,zn,ni,nj,nk,data,ntape5)
      if (istop.ne.0) call error1 (25,25hmesh3d-error termination  ,0,0
     . ,0,0,1h ,1)
      last=ni*nj*nk
      call grid(xn,yn,zn,last)
      return
c
  900 jstop=istop
      call icheck (istop,word(nn))
      if (istop.ne.jstop) go to 40
      call bkrs(xn,yn,zn,ni,nj,istop)
      call mshprt (xn,yn,zn,ni,nj,nk,cmmnd,iprint,nc,nc,ng,name,maxlin,l
     .ine)
      if (nc.eq.1) go to 40
      jstop=istop
      call trans (xn,yn,zn,ni,nj,nc,istop)
      if (istop.ne.jstop) go to 40
      if (iprint.gt.3) call mshprt (xn,yn,zn,ni,nj,nk,cmmnd,iprint,
     1  nc,1,ng,name,maxlin,line)
      go to 40
  910 jstop=istop
      call icheck (istop,word(nn))
      if (istop.ne.jstop) go to 40
      call facers(xn,yn,zn,ni,nj,istop)
      call mshprt (xn,yn,zn,ni,nj,nk,cmmnd,iprint,nc,nc,ng,name,maxlin,l
     .ine)
      if (nc.eq.1) go to 40
      jstop=istop
      call trans (xn,yn,zn,ni,nj,nc,istop)
      if (istop.ne.jstop) go to 40
      if (iprint.gt.3) call mshprt (xn,yn,zn,ni,nj,nk,cmmnd,iprint,
     1  nc,1,ng,name,maxlin,line)
      go to 40
  920 jstop=istop
      call icheck (istop,word(nn))
      if (istop.ne.jstop) go to 40
      call movex(xn,yn,zn,ni,nj,istop)
      call mshprt (xn,yn,zn,ni,nj,nk,cmmnd,iprint,nc,nc,ng,name,maxlin,l
     .ine)
      if (nc.eq.1) go to 40
      jstop=istop
      call trans (xn,yn,zn,ni,nj,nc,istop)
      if (istop.ne.jstop) go to 40
      if (iprint.gt.3) call mshprt (xn,yn,zn,ni,nj,nk,cmmnd,iprint,
     1  nc,1,ng,name,maxlin,line)
      go to 40
  930 jstop=istop
      call icheck (istop,word(nn))
      if (istop.ne.jstop) go to 40
      call movey(xn,yn,zn,ni,nj,istop)
      call mshprt (xn,yn,zn,ni,nj,nk,cmmnd,iprint,nc,nc,ng,name,maxlin,l
     .ine)
      if (nc.eq.1) go to 40
      jstop=istop
      call trans (xn,yn,zn,ni,nj,nc,istop)
      if (istop.ne.jstop) go to 40
      if (iprint.gt.3) call mshprt (xn,yn,zn,ni,nj,nk,cmmnd,iprint,
     1  nc,1,ng,name,maxlin,line)
      go to 40
  940 jstop=istop
      call icheck (istop,word(nn))
      if (istop.ne.jstop) go to 40
      call movez(xn,yn,zn,ni,nj,istop)
      call mshprt (xn,yn,zn,ni,nj,nk,cmmnd,iprint,nc,nc,ng,name,maxlin,l
     .ine)
      if (nc.eq.1) go to 40
      jstop=istop
      call trans (xn,yn,zn,ni,nj,nc,istop)
      if (istop.ne.jstop) go to 40
      if (iprint.gt.3) call mshprt (xn,yn,zn,ni,nj,nk,cmmnd,iprint,
     1  nc,1,ng,name,maxlin,line)
      go to 40
      end
*deck icheck
      subroutine icheck (istop,word)
      implicit real*8 (a-h,o-z)
      common /ijck/ ii(6),ic(3)
      logical check
      common /tapes/ nin,nout,ntape1,ntape2,ntape3,ntape4,ntape5
c
      check=.true.
      do 10 i=1,3
         if (ii(i).lt.1.or.ii(i).gt.ic(i).or.ii(i+3).lt.1.or.ii(i+3).gt.
     1   ic(i)) check=.false.
   10 continue
      if (check) return
      istop=istop+1
      write (nout,20) word,ii
   20 format (9h command ,a7,35h aborted - i,j, or k out of bounds ,6i3)
      return
c
      end
*deck trans
      subroutine trans (xn,yn,zn,ni,nj,nc,istop)
      implicit real*8 (a-h,o-z)
c
      common /ijck/ imin,jmin,kmin,imax,jmax,kmax,ic(3)
      common/transx/xyz(3,15),r(3,3,15),alpha(15),ng(15)
      common /tapes/ nin,nout,ntape1,ntape2,ntape3,ntape4,ntape5
c
      dimension xn(ni,nj,*), yn(ni,nj,*), zn(ni,nj,*)
      common/dumcom/dumval
c
      conv=atan(1.)/45.
      l=ng(nc)
      if (l.eq.0) go to 70
      do 60 i=imin,imax
      do 60 j=jmin,jmax
      do 60 k=kmin,kmax
         x=xn(i,j,k)
         y=yn(i,j,k)
         z=zn(i,j,k)
         go to (50,10,20,30,40), l
c
c         polar cylindrical
c
   10    o=y*conv
         y=x*sin(o)
         x=x*cos(o)
         go to 50
c
c         spherical
c
   20    o=y*conv
         p=z*conv
         z=x*cos(p)
         y=x*sin(o)*sin(p)
         x=x*cos(o)*sin(p)
         go to 50
c
c         toroidal
c
   30    o=y*conv
         p=z*conv
         a=alpha(nc)+cos(p)*x
         z=x*sin(p)
         y=a*sin(o)
         x=a*cos(o)
         go to 50
   40    call extcst (x,y,z,istop)
c
c         cartesian transformation
c
   50    xn(i,j,k)=xyz(1,nc)+r(1,1,nc)*x+r(1,2,nc)*y+r(1,3,nc)*z
         yn(i,j,k)=xyz(2,nc)+r(2,1,nc)*x+r(2,2,nc)*y+r(2,3,nc)*z
         zn(i,j,k)=xyz(3,nc)+r(3,1,nc)*x+r(3,2,nc)*y+r(3,3,nc)*z
   60 continue
      return
   70 istop=istop+1
      write (nout,80) nc
   80 format (53h0attempt to use non-existing coordinate system; nc = ,i
     13)
      do 90 i=imin,imax
      do 90 j=jmin,jmax
      do 90 k=kmin,kmax
         xn(i,j,k)=dumval
         yn(i,j,k)=dumval
         zn(i,j,k)=dumval
   90 continue
      return
c
      end
*deck untran
      subroutine untran(x,y,z,a,b,c,nc,istop)
      implicit real*8 (a-h,o-z)
      common /tapes/ nin,nout,ntape1,ntape2,ntape3,ntape4,ntape5
      common/transx/xyz(3,15),r(3,3,15),alpha(15),ng(15)
      conv=45./atan(1.)
      l=ng(nc)
      if (l.lt.1.or.l.gt.5) go to 60
c
c         cartesian transformation
c
      a=r(1,1,nc)*(x-xyz(1,nc))+r(2,1,nc)*(y-xyz(2,nc))
     1  +r(3,1,nc)*(z-xyz(3,nc))
      b=r(1,2,nc)*(x-xyz(1,nc))+r(2,2,nc)*(y-xyz(2,nc))
     1  +r(3,2,nc)*(z-xyz(3,nc))
      c=r(1,3,nc)*(x-xyz(1,nc))+r(2,3,nc)*(y-xyz(2,nc))
     1  +r(3,3,nc)*(z-xyz(3,nc))
      go to (10,20,30,40,50), l
   10 return
c
c         polar cylindrical
c
   20 o=atan2(b,a)
      a=(a**2+b**2)**.5
      b=o*conv
c
c     c=c
c
      return
c
c         spherical
c
   30 d=(a**2+b**2)**.5
      b=atan2(b,a)*conv
      a=(d**2+c**2)**.5
      c=atan2(d,c)*conv
      return
c
c         toroidal
c
   40 d=(a**2+b**2)**.5-alpha(nc)
      b=atan2(b,a)*conv
      a=(d**2+c**2)**0.5
      c=atan2(c,d)*conv
      return
c
   50 call extcst (a,b,c,istop)
      return
c
   60 istop=istop+1
      write (nout,70) nc
   70 format(54h attempt to use non-existing global coordinate system   
     .       ,6h nc =  ,i3)
      return
c
      end
*deck surf
      subroutine surf (a,xx,f,nc)
      implicit real*8 (a-h,o-z)
      common /tapes/ nin,nout,ntape1,ntape2,ntape3,ntape4,ntape52
      common/transx/xyz(3,15),r(3,3,15),alpha(15),ng(15)
      dimension a(3), x(3), f(3,3), g(3,3), xx(3)
      conv=atan(1.)/45.
      l=ng(nc)
      go to (10,40,50,60,70), l
c
c     cartesian
c
   10 do 30 i=1,3
         x(i)=a(i)
         do 20 j=1,3
   20    g(i,j)=0.
   30 g(i,i)=1.
      go to 90
c
c     polar coordinates
c
   40 continue
      c=cos(a(2)*conv)
      s=sin(a(2)*conv)
      x(1)=a(1)*c
      x(2)=a(1)*s
      x(3)=a(3)
      g(1,1)=c
      g(1,2)=-x(2)
      g(1,3)=0.
      g(2,1)=s
      g(2,2)=x(1)
      g(2,3)=0.
      g(3,1)=0.
      g(3,2)=0.
      g(3,3)=1.
      go to 90
c
c     spherical
c
   50 continue
      ct=cos(a(2)*conv)
      st=sin(a(2)*conv)
      cp=cos(a(3)*conv)
      sp=sin(a(3)*conv)
      x(1)=a(1)*ct*sp
      x(2)=a(1)*st*sp
      x(3)=a(1)*cp
      g(1,1)=ct*sp
      g(1,2)=-x(2)
      g(1,3)=a(1)*ct*cp
      g(2,1)=st*sp
      g(2,2)=x(1)
      g(2,3)=a(1)*st*cp
      g(3,1)=cp
      g(3,2)=0.
      g(3,3)=-a(1)*sp
      go to 90
c
c     toroidal coordinates
c
   60 continue
      ct=cos(a(2)*conv)
      st=sin(a(2)*conv)
      cp=cos(a(3)*conv)
      sp=sin(a(3)*conv)
      rm=alpha(nc)
      x(1)=(rm+a(1)*cp)*ct
      x(2)=(rm+a(1)*cp)*st
      x(3)=a(1)*sp
      g(1,1)=ct*cp
      g(1,2)=-x(2)
      g(1,3)=-a(1)*ct*sp
      g(2,1)=st*cp
      g(2,2)=x(1)
      g(2,3)=-a(1)*st*sp
      g(3,1)=sp
      g(3,2)=0.
      g(3,3)=a(1)*cp
      go to 90
c
c     externally defined coordinate system
c     user should insert coding to calculate gradient
c     of coordinate transformation and coordinates
c     x(i) = local cartesian coordinates
c     g(i,j) = d(x(i))/da(j)
c
   70 write (nout,80)
   80 format (54h attempt to use non existant coordinate transformation)
      write(nout,3000) nc,l,ng
 3000 format(20i5)
      call error1(4,4hsurf,0,0,0,0,1h ,1)
      return
c
c     transform to global coordinates
c
   90 do 110 i=1,3
         xx(i)=xyz(i,nc)
      do 110 j=1,3
         xx(i)=xx(i)+r(i,j,nc)*x(j)
         f(i,j)=0.
         do 100 k=1,3
  100    f(i,j)=f(i,j)+r(i,k,nc)*g(k,j)
  110 continue
      return
c
      end
*deck steve
      subroutine steve(int,xn,yn,zn,ni,nj,nf,istop)
      implicit real*8 (a-h,o-z)
      common /ijck/ imin,jmin,kmin,imax,jmax,kmax,ic(3)
c
      common /el/an(32),x(32),y(32),z(32),c11(9999),c12(9999),
     1 c13(9999),c21(9999),c22(9999),c23(9999),c31(9999),c32(9999),
     2 c33(9999),dt1(9999),dt2(9999),dt3(9999),db1(9999),db2(9999),
     3 db3(9999),psi(9999),eta(9999),zta(9999),c41(9999),c42(9999),
     4 c43(9999),g(12)
c
      dimension xn(ni,nj,1), yn(ni,nj,1), zn(ni,nj,1)
      data one/1.0d0/

      if (int.ge.2) call fill(x,y,z,int-1,istop)
c
      npi=imax-imin+1
      npj=jmax-jmin+1
      npk=kmax-kmin+1
c
c      generate coordinates along sides parallel to zta
c
      call seg3 (-one,-one,-one,-one,-one,one,g(9),c11,c12,c13,npk)
      call seg3 (one,-one,-one,one,-one,one,g(10),c21,c22,c23,npk)
      call seg3 (one,one,-one,one,one,one,g(11),c31,c32,c33,npk)
      call seg3 (-one,one,-one,-one,one,one,g(12),c41,c42,c43,npk)
      k=0
      do 80 kk=kmin,kmax
         k=k+1
         fac=1.
         if (npk.gt.1) fac=(float(k-1))/(float(npk-1))
c
c      coordinates along sides parallel to psi
c
         gb=g(1)-(g(1)-g(5))*fac
         gt=g(3)-(g(3)-g(7))*fac
         gr=g(2)-(g(2)-g(6))*fac
         gl=g(4)-(g(4)-g(8))*fac
         call seg3 (c11(k),c12(k),c13(k),c21(k),c22(k),c23(k),gb,db1,db2
     1   ,db3,npi)
         call seg3 (c41(k),c42(k),c43(k),c31(k),c32(k),c33(k),gt,dt1,dt2
     1   ,dt3,npi)
         i=0
         do 70 ii=imin,imax
            i=i+1
            faci=1.
            if (npi.gt.1) faci=(float(i-1))/(float(npi-1))
            glr=gl+(gr-gl)*faci
            call seg3 (db1(i),db2(i),db3(i),dt1(i),dt2(i),dt3(i),glr,psi
     1      ,eta,zta,npj)
            j=0
            do 60 jj=jmin,jmax
               j=j+1
               xn(ii,jj,kk)=0.
               yn(ii,jj,kk)=0.
               zn(ii,jj,kk)=0.
               pm=1.-psi(j)
               pp=1.+psi(j)
               em=1.-eta(j)
               ep=1.+eta(j)
               zm=1.-zta(j)
               zp=1.+zta(j)
c
c      linear variation
c
               an(1)=0.125*pm*em*zm
               an(2)=0.125*pp*em*zm
               an(3)=0.125*pp*ep*zm
               an(4)=0.125*pm*ep*zm
               an(5)=0.125*pm*em*zp
               an(6)=0.125*pp*em*zp
               an(7)=0.125*pp*ep*zp
               an(8)=0.125*pm*ep*zp
               go to (40,10,20), int
c
c      quadratric variation
c
   10          p=psi(j)
               e=eta(j)
               zt=zta(j)
               an(1)=an(1)*(-p-e-zt-2.)
               an(2)=an(2)*(p-e-zt-2.)
               an(3)=an(3)*(p+e-zt-2.)
               an(4)=an(4)*(-p+e-zt-2.)
               an(5)=an(5)*(-p-e+zt-2.)
               an(6)=an(6)*(p-e+zt-2.)
               an(7)=an(7)*(p+e+zt-2.)
               an(8)=an(8)*(-p+e+zt-2.)
               fac=0.25*(1.-p*p)
               an(9)=fac*em*zm
               an(17)=fac*em*zp
               an(19)=fac*ep*zp
               an(11)=fac*ep*zm
               fac=0.25*(1.-e*e)
               an(12)=fac*pm*zm
               an(10)=fac*pp*zm
               an(18)=fac*pp*zp
               an(20)=fac*pm*zp
               fac=0.25*(1.-zt*zt)
               an(16)=fac*pm*ep
               an(15)=fac*pp*ep
               an(14)=fac*pp*em
               an(13)=fac*pm*em
               go to 40
c
c      cubic variation
c
   20          p=psi(j)
               e=eta(j)
               zt=zta(j)
               fac=0.125*(9.*(p*p+e*e+zt*zt)-19.)
               do 30 l=1,8
   30          an(l)=an(l)*fac
c
c      psi=plus or minus 1/3
c
               fac=9./64.*(1.-p*p)
               fac3=fac*(1.+3.*p)
               an(11)=fac3*ep*zm
               an(21)=fac3*em*zm
               an(29)=fac3*em*zp
               an(19)=fac3*ep*zp
               fac3=fac*(1.-3.*p)
               an(23)=fac3*ep*zm
               an(9)=fac3*em*zm
               an(17)=fac3*em*zp
               an(31)=fac3*ep*zp
c
c      eta=plus of minus 1/3
c
               fac=9./64.*(1.-e*e)
               fac3=fac*(1.+3.*e)
               an(12)=fac3*pm*zm
               an(22)=fac3*pp*zm
               an(30)=fac3*pp*zp
               an(20)=fac3*pm*zp
               fac3=fac*(1.-3.*e)
               an(24)=fac3*pm*zm
               an(10)=fac3*pp*zm
               an(18)=fac3*pp*zp
               an(32)=fac3*pm*zp
c
c      zta=plus or minus 1/3
c
               fac=9./64.*(1.-zt*zt)
               fac3=fac*(1.+3.*zt)
               an(28)=fac3*pm*ep
               an(27)=fac3*pp*ep
               an(26)=fac3*pp*em
               an(25)=fac3*pm*em
               fac3=fac*(1.-3.*zt)
               an(16)=fac3*pm*ep
               an(15)=fac3*pp*ep
               an(14)=fac3*pp*em
               an(13)=fac3*pm*em
   40          continue
               do 50 l=1,nf
                  xn(ii,jj,kk)=xn(ii,jj,kk)+x(l)*an(l)
                  yn(ii,jj,kk)=yn(ii,jj,kk)+y(l)*an(l)
   50             zn(ii,jj,kk)=zn(ii,jj,kk)+z(l)*an(l)
   60       continue
   70    continue
   80 continue
      return
c
      end
*deck mshprt
      subroutine mshprt (xn,yn,zn,ni,nj,nk,cmmd,iprint,nc1,nc2,ng,name,m
     .axlin,line)
      implicit real*8 (a-h,o-z)
      character*8 cmmd
      character*4 name0(3)
      character*8 coord(3),type
      common /tapes/ nin,nout,ntape1,ntape2,ntape3,ntape4,ntape5
      common /ijck/ imin,jmin,kmin,imax,jmax,kmax,ic(3)
      dimension xn(ni,nj,nk), yn(ni,nj,nk), zn(ni,nj,nk), name(3,5), 
     1 ng(1)
      common/dumcom/dumval
      data name0(1) /'unkn'/
      data name0(2) /'own '/
      data name0(3) /'    '/
c
      loc1=ng(nc1)
      loc2=ng(nc2)
      if (cmmd.eq.'end') go to 210
      if (iprint.lt.3) return
      if (iprint.ge.4) go to 20
c
      line=line+3
      if (line.lt.maxlin) go to 15
      line=4
      write (nout,14)
   14 format (1h1)
   15 if (loc1.ne.0) go to 16
      write (nout,10) cmmd,(name0(i),i=1,3),nc1,imin,jmin,kmin,imax,
     1 jmax,kmax
      return
   16 write (nout,10) cmmd,(name(i,loc1),i=1,3),nc1,imin,jmin,kmin,imax,
     1jmax,kmax
   10 format ( 17h0mesh generation ,a8,  4h in ,3a4, 19h coordinate syst
     1em ,i2,/, 12h from i j k ,3i3, 10h to i j k ,3i3)
      return
c
   20 if (loc2.eq.0) go to 65
      go to (30,40,50,50,60), loc2
c
c                                  cartesian
c
   30 coord(1)='x'
      coord(2)='y'
      coord(3)='z'
      go to 70
c
c                                 cylindrical
c
   40 coord(1)='r'
      coord(2)='theta'
      coord(3)='z'
      go to 70
c
c                           spherical and toroidal
c
   50 coord(1)='r'
      coord(2)='theta'
      coord(3)='phi'
      go to 70
c
c                             external definition
c
   60 return
c
c                  undefined coordinate system;
c       print local coordinates before abort of command...
c
   65 coord(1)='c1'
      coord(2)='c2'
      coord(3)='c3'
c
   70 type='local'
      if (nc2.lt.5) type='global'
      if (nc2.eq.4.and.ng(nc2).ne.4) type='local'
c
      i=imin
      j=jmin
      k=kmin
c
   80 line=line+7
      if ((line+1).lt.maxlin) go to 85
   82 line=7
      write (nout,14)
   85 if (loc1.ne.0) go to 86
      write(nout,90) cmmd,(name0(l),l=1,3),nc1,imin,jmin,kmin,imax,
     1jmax,kmax
      write(nout,100) type,(name0(l),l=1,3),coord,coord
      go to 110
   86 write(nout,90) cmmd,(name(l,loc1),l=1,3),nc1,imin,jmin,kmin,imax,
     1jmax,kmax
   90 format ( 17h0mesh generation ,a8,  4h in ,3a4, 19h coordinate syst
     1em ,i2,/, 12h from i j k ,3i3, 10h to i j k ,3i3)
      write(nout,100)type,(name(l,loc2),l=1,3),coord,coord
  100 format (  1h0,a6,1x,3a4, 32h coordinates of points generated,//4x,
     1  11hi    j    k,5x,a5,7x,a5,7x,a5,15x,  11hi    j    k,5x,a5,7x,a
     25,7x,a5)
  110 if (xn(i,j,k).eq.dumval) go to 180
c
      line=line+1
      if (line.ge.maxlin) go to 82
c
      write (nout,120) i,j,k,xn(i,j,k),yn(i,j,k),zn(i,j,k)
  120 format (3i5,3x,e11.4,1x,e11.4,1x,e11.4)
  130 if (k.eq.kmax) go to 140
      k=k+1
      go to 160
c
  140 if (j.eq.jmax) go to 150
      j=j+1
      k=kmin
      go to 160
c
  150 if (i.eq.imax) return
      i=i+1
      j=jmin
      k=kmin
c
  160 if (xn(i,j,k).eq.dumval) go to 130
      write (nout,170) i,j,k,xn(i,j,k),yn(i,j,k),zn(i,j,k)
  170 format (1h+,59x,3i5,3x,e11.4,1x,e11.4,1x,e11.4)
c
  180 if (k.eq.kmax) go to 190
      k=k+1
      go to 110
c
  190 if (j.eq.jmax) go to 200
      j=j+1
      k=kmin
      go to 110
c
  200 if (i.eq.imax) return
      i=i+1
      j=jmin
      k=kmin
      go to 110
c
  210 if (iprint.lt.2) return
      imin=1
      jmin=1
      kmin=1
      imax=ni
      jmax=nj
      kmax=nk
      nc1=1
      nc2=1
      loc1=1
      loc2=1
      go to 20
c
      end
*deck mshplt
      subroutine mshplt (x,y,z,ni,nj,nk,data,ntape)
      implicit real*8 (a-h,o-z)
      common/clist/cdata(256)
      character*8 cdata
      common/dumcom/dumval
c
c     write tape for plotting nodal points
c
      dimension x(ni,nj,nk), y(ni,nj,nk), z(ni,nj,nk)
c
      dimension data(128)
c     data n7,n6,n8/9999999997,9999999996,9999999998/
      data n7,n6,n8/9999997,9999996,9999998/
      nfun(i,j,k)=k+100*j+10000*i
c
      if (cdata(3).ne.'plot') return
c
      rewind ntape
      write (ntape,10) n7
   10 format (4hgeom/i10)
      do 30 i=1,ni
      do 30 j=1,nj
      do 30 k=1,nk
         n=nfun(i,j,k)
         if (x(i,j,k).eq.dumval) go to 30
         write (ntape,20) n,x(i,j,k),y(i,j,k),z(i,j,k)
   20 format (i10,3f10.5)
   30 continue
      write (ntape,40) n6
   40 format (i10)
      ni1=ni-1
      nj1=nj-1
      nk1=nk-1
      do 80 i=1,ni1
      do 80 j=1,nj1
      do 80 k=1,nk1
         if (x(i,j,k).eq.dumval) go to 80
         m1=nfun(i,j,k)
         if (x(i+1,j,k).eq.dumval) go to 60
         m2=nfun(i+1,j,k)
         write (ntape,50) m1,m2
   50 format (4i10)
   60    if (x(i,j+1,k).eq.dumval) go to 70
         m2=nfun(i,j+1,k)
         write (ntape,50) m1,m2
   70    if (x(i,j,k+1).eq.dumval) go to 80
         m2=nfun(i,j,k+1)
         write (ntape,50) m1,m2
   80 continue
      write (ntape,40) n8
      rewind ntape
      return
c
      end
*deck extdef
      subroutine extdef (xn,yn,zn,ni,nj,a,b,c,istop)
      implicit real*8 (a-h,o-z)
c
c        user supplied routine for mesh generation
c
      common/tapes/nin,nout,ntape1,natpe2,ntape3,ntape4,ntape52
      dimension xn(ni,nj,*),yn(ni,nj,*),zn(ni,nj,*)
      istop=istop+1
      write (nout,10)
   10 format (53h0attempt to use non-existant user-supplied subroutine)
      return
      end
*deck seg1
      subroutine seg1 (grad,x,n)
      implicit real*8 (a-h,o-z)
      dimension x(*)
      real*8 nseg
c
c     subroutine to divide the interval [-1,+1] into  n - 1 segments
c           using the uniform gradient method    ( 5/31/74 )
c
      x(n)=1.0
      if (n.le.2) go to 40
      nseg=n-1
      ns=nseg
      p=1.0
      a=1.0/nseg
      if (.01-abs(grad-1.0)) 10,20,20
   10 p=exp((log(grad))/(nseg-1.0))
      a=(p-1.0)/(p**ns-1.0)
   20 sum=0.0
      do 30 i=1,ns
         j=n+1-i
         x(j)=1.0-2.0*sum
   30 sum=a+p*sum
   40 x(1)=-1.0
      return
c
      end
*deck extcst
      subroutine extcst (x,y,z,istop)
      implicit real*8 (a-h,o-z)
      common /tapes/ nin,nout,ntape1,ntape2,ntape3,ntape4,ntape5
c
c
c         user supplied routine to transform xyz from arbitrary
c         coordinate system to local cartesian system
c     c
c
      istop=istop+1
      write (nout,10)
   10 format (42h attempt to use non-existant user-supplied,15h transfor
     1mation)
      return
c
      end
      subroutine movex(xn,yn,zn,ni,nj,istop)
      implicit real*8 (a-h,o-z)
      common /ijck/ imin,jmin,kmin,imax,jmax,kmax,ic(3)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
c
      common /el/ h(32),x(32),y(32),z(32),cx(9999),cy(9999),cz(9999),
     1 c21(9999),c22(9999),c23(9999),c31(9999),c32(9999),c33(9999),
     2 dt1(9999),dt2(9999),dt3(9999),db1(9999),db2(9999),db3(9999),
     4 psi(9999),eta(9999),zta(9999),c41(9999),c42(9999),c43(9999),g(12)

c
      dimension xn(ni,nj,*), yn(ni,nj,*), zn(ni,nj,*)
      dimension if1(4,3),if2(4,3)
      call ffin
      xmove=fdata(1)
      do 90 k=kmin,kmax
      do 80 j=jmin,jmax
      do 70 i=imin,imax
      xn(i,j,k)=xmove
   70 continue
   80 continue
   90 continue
      return
c
      end
      subroutine movey(xn,yn,zn,ni,nj,istop)
      implicit real*8 (a-h,o-z)
      common /ijck/ imin,jmin,kmin,imax,jmax,kmax,ic(3)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
c
      common /el/ h(32),x(32),y(32),z(32),cx(9999),cy(9999),cz(9999),
     1 c21(9999),c22(9999),c23(9999),c31(9999),c32(9999),c33(9999),
     2 dt1(9999),dt2(9999),dt3(9999),db1(9999),db2(9999),db3(9999),
     4 psi(9999),eta(9999),zta(9999),c41(9999),c42(9999),c43(9999),g(12)
c
      dimension xn(ni,nj,*), yn(ni,nj,*), zn(ni,nj,*)
      dimension if1(4,3),if2(4,3)
      call ffin
      ymove=fdata(1)
      do 90 k=kmin,kmax
      do 80 j=jmin,jmax
      do 70 i=imin,imax
      yn(i,j,k)=ymove
   70 continue
   80 continue
   90 continue
      return
c
      end
      subroutine movez(xn,yn,zn,ni,nj,istop)
      implicit real*8 (a-h,o-z)
      common /ijck/ imin,jmin,kmin,imax,jmax,kmax,ic(3)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
c
      common /el/ h(32),x(32),y(32),z(32),cx(9999),cy(9999),cz(9999),
     1 c21(9999),c22(9999),c23(9999),c31(9999),c32(9999),c33(9999),
     2 dt1(9999),dt2(9999),dt3(9999),db1(9999),db2(9999),db3(9999),
     4 psi(9999),eta(9999),zta(9999),c41(9999),c42(9999),c43(9999),g(12)
c
      dimension xn(ni,nj,*), yn(ni,nj,*), zn(ni,nj,*)
      dimension if1(4,3),if2(4,3)
      call ffin
      zmove=fdata(1)
      do 90 k=kmin,kmax
      do 80 j=jmin,jmax
      do 70 i=imin,imax
      zn(i,j,k)=zmove
   70 continue
   80 continue
   90 continue
      return
c
      end
      subroutine facers(xn,yn,zn,ni,nj,istop)
      implicit real*8 (a-h,o-z)
      common /ijck/ imin,jmin,kmin,imax,jmax,kmax,ic(3)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
c
      common /el/ h(32),x(32),y(32),z(32),cx(9999),cy(9999),cz(9999),
     1 c21(9999),c22(9999),c23(9999),c31(9999),c32(9999),c33(9999),
     2 dt1(9999),dt2(9999),dt3(9999),db1(9999),db2(9999),db3(9999),
     4 psi(9999),eta(9999),zta(9999),c41(9999),c42(9999),c43(9999),g(12)
c
      dimension xn(ni,nj,*), yn(ni,nj,*), zn(ni,nj,*)
      dimension if1(4,3),if2(4,3)
      data if1/1,4,8,5,  1,2,6,5,  1,2,3,4/
      data if2/2,3,7,6,  4,3,7,8,  5,6,7,8/
      m=1
      if(jmin.eq.jmax)m=2
      if(kmin.eq.kmax)m=3
      call ffin
      do 10 i=1,4
      m1=if1(i,m)
      m2=if2(i,m)
      x(m1)=fdata(1+3*(i-1))
      y(m1)=fdata(2+3*(i-1))
      z(m1)=fdata(3+3*(i-1))
      x(m2)=x(m1)
      y(m2)=y(m1)
      z(m2)=z(m1)
   10 continue
      call ffin
      do 20 i=1,128
      cx(i)=fdata(i)
   20 continue
      call ffin
      do 30 i=1,128
      cy(i)=fdata(i)
   30 continue
      if(m.eq.1)then
      do 35 i=1,128
      cz(i)=cy(i)
      cy(i)=cx(i)
   35 continue
      cx(1) = -1.
      else if(m.eq.2)then
      do 40 i=1,128
      cz(i)=cy(i)
   40 continue
      cy(1) = -1.
      else
      cz(1) = -1.
      endif
      do 90 k=kmin,kmax
      t=cz(k-kmin+1)
      do 80 j=jmin,jmax
      s=cy(j-jmin+1)
      do 70 i=imin,imax
      r=cx(i-imin+1)
      h(1)=(1.-r)*(1.-s)*(1.-t)
      h(2)=r*(1.-s)*(1.-t)
      h(3)=r*s*(1.-t)
      h(4)=(1.-r)*s*(1.-t)
      h(5)=(1.-r)*(1.-s)*t
      h(6)=r*(1.-s)*t
      h(7)=r*s*t
      h(8)=(1.-r)*s*t
      xx=0.
      yy=0.
      zz=0.
      do 60 ii=1,8
      xx=xx+h(ii)*x(ii)
      yy=yy+h(ii)*y(ii)
      zz=zz+h(ii)*z(ii)
   60 continue
      xn(i,j,k)=xx
      yn(i,j,k)=yy
      zn(i,j,k)=zz
   70 continue
   80 continue
   90 continue
      return
c
      end
      subroutine bkrs(xn,yn,zn,ni,nj,istop)
      implicit real*8 (a-h,o-z)
      common /ijck/ imin,jmin,kmin,imax,jmax,kmax,ic(3)
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*8 cdata
c
      common /el/ h(32),x(32),y(32),z(32),cx(9999),cy(9999),cz(9999),
     1 c21(9999),c22(9999),c23(9999),c31(9999),c32(9999),c33(9999),
     2 dt1(9999),dt2(9999),dt3(9999),db1(9999),db2(9999),db3(9999),
     4 psi(9999),eta(9999),zta(9999),c41(9999),c42(9999),c43(9999),g(12)
c
      dimension xn(ni,nj,*), yn(ni,nj,*), zn(ni,nj,*)
      call ffin
      do 10 i=1,8
      x(i)=fdata(1+3*(i-1))
      y(i)=fdata(2+3*(i-1))
      z(i)=fdata(3+3*(i-1))
   10 continue
      call ffin
      do 20 i=1,128
      cx(i)=fdata(i)
   20 continue
      if(cx(1).eq.-99.0.and.(imin.ne.imax))then
        dcx=1.0/float(imax-imin)
        do 25 i=imin,imax
          cx(i-imin+1)=float(i-imin)*dcx
   25   continue
      endif
      call ffin
      do 30 i=1,128
      cy(i)=fdata(i)
   30 continue
      if(cy(1).eq.-99.0.and.(jmin.ne.jmax))then
        dcy=1.0/float(jmax-jmin)
        do 35 j=jmin,jmax
          cy(j-jmin+1)=float(j-jmin)*dcy
   35   continue
      endif
      call ffin
      do 40 i=1,128
      cz(i)=fdata(i)
   40 continue
      if(cz(1).eq.-99.0.and.(kmin.ne.kmax))then
        dcz=1.0/float(kmax-kmin)
        do 45 k=kmin,kmax
          cz(k-kmin+1)=float(k-kmin)*dcz
   45   continue
      endif
      do 90 k=kmin,kmax
      t=cz(k-kmin+1)
      do 80 j=jmin,jmax
      s=cy(j-jmin+1)
      do 70 i=imin,imax
      r=cx(i-imin+1)
      h(1)=(1.-r)*(1.-s)*(1.-t)
      h(2)=r*(1.-s)*(1.-t)
      h(3)=r*s*(1.-t)
      h(4)=(1.-r)*s*(1.-t)
      h(5)=(1.-r)*(1.-s)*t
      h(6)=r*(1.-s)*t
      h(7)=r*s*t
      h(8)=(1.-r)*s*t
      xx=0.
      yy=0.
      zz=0.
      do 60 ii=1,8
      xx=xx+h(ii)*x(ii)
      yy=yy+h(ii)*y(ii)
      zz=zz+h(ii)*z(ii)
   60 continue
      xn(i,j,k)=xx
      yn(i,j,k)=yy
      zn(i,j,k)=zz
   70 continue
   80 continue
   90 continue
      return
c
      end
*deck memadj
      subroutine memadj(nreq,ierror)
      implicit real*8 (a-h,o-z)
      parameter(MAXCOM=20000000)
      data nsiz/0/
      common //a(MAXCOM)
      nsiz=nsiz+nreq
      if(nsiz.gt.MAXCOM)THEN
      write(6,*)nsiz,MAXCOM
      write(6,*)' Error out of Memory'
        ierror=1
      stop
      else
        ierror=0
      endif
      return
      end
*deck fill
      subroutine fill(x,y,z,nn,istop)
      implicit real*8 (a-h,o-z)
      dimension x(32), y(32), z(32)
      data dum/-1.23e-21/
      go to (10,50), nn
   10 do 30 k=1,2
         ks=4*(k-1)+1
         ke=4*k
         do 20 i=ks,ke
            j=i-ks+1+8*k
            l=1
            if (i.eq.ke) l=-3
            if (x(j).eq.dum) x(j)=(x(i)+x(i+l))/2.
            if (y(j).eq.dum) y(j)=(y(i)+y(i+l))/2.
            if (z(j).eq.dum) z(j)=(z(i)+z(i+l))/2.
   20    continue
   30 continue
      do 40 i=1,4
         j=i+12
      if (x(j).ne.dum) go to 40
      x(j)=(x(i)+x(i+4))/2.0
      y(j)=(y(i)+y(i+4))/2.0
      z(j)=(z(i)+z(i+4))/2.0
   40 continue
      return
   50 do 90 m=1,2
         ks=4*(m-1)+1
         ke=4*m
         do 80 i=ks,ke
            ii=i-ks+1
            j=ii+8*m
            k=ii+20+8*(m-1)
            l=1
            if (i.eq.ke) l=-3
      if (x(j).ne.dum) go to 60
      x(j)=(2.0*x(i)+x(i+l))/3.0
      y(j)=(2.0*y(i)+y(i+l))/3.0
      z(j)=(2.0*z(i)+z(i+l))/3.0
   60 if (x(k).ne.dum) go to 80
      x(k)=(x(i)+2.0*x(i+l))/3.0
      y(k)=(y(i)+2.0*y(i+l))/3.0
      z(k)=(z(i)+2.0*z(i+l))/3.0
   80    continue
   90 continue
      l=4
      do 120 i=1,4
         j=i+12
         k=i+24
      if (x(j).ne.dum) go to 100
      x(j)=(2.0*x(i)+x(i+l))/3.0
      y(j)=(2.0*y(i)+y(i+l))/3.0
      z(j)=(2.0*z(i)+z(i+l))/3.0
  100 if (x(k).ne.dum) go to 120
      x(k)=(x(i)+2.0*x(i+l))/3.0
      y(k)=(y(i)+2.0*y(i+l))/3.0
      z(k)=(z(i)+2.0*z(i+l))/3.0
  120 continue
      return
      end
      subroutine bgnplt(filenm)
      implicit real*8 (a-h,o-z)
      character*(*) filenm
      common/uset1/nl(5),iflag
      common/uset2/l(5)
      save /uset1/,/uset2/
      character*80 l
      open(unit=43,err=90,file=filenm,
     .             status='unknown',form='unformatted')
      do 10 i=1,5
      nl(i)=0
      l(i)=' '
   10 continue
      return
   90 continue
      write(*,*)' Error opening file ',filenm
      stop
      end
      subroutine xlab(lab,n)
      implicit real*8 (a-h,o-z)
      common/uset1/nl(5),iflag
      common/uset2/l(5)
      save /uset1/,/uset2/
      character*80 l
      character*(*) lab
      l(1)=lab
      nl(1)=n
      return
      end
      subroutine ylab(lab,n)
      implicit real*8 (a-h,o-z)
      common/uset1/nl(5),iflag
      common/uset2/l(5)
      save /uset1/,/uset2/
      character*80 l
      character*(*) lab
      l(2)=lab
      nl(2)=n
      return
      end
      subroutine head(lab,n)
      implicit real*8 (a-h,o-z)
      common/uset1/nl(5),iflag
      common/uset2/l(5)
      save /uset1/,/uset2/
      character*80 l
      character*(*) lab
      l(3)=lab
      nl(3)=n
      return
      end
      subroutine xlabu(lab,n)
      implicit real*8 (a-h,o-z)
      common/uset1/nl(5),iflag
      common/uset2/l(5)
      save /uset1/,/uset2/
      character*80 l
      character*(*) lab
      l(4)=lab
      nl(4)=n
      return
      end
      subroutine headu(lab,n)
      implicit real*8 (a-h,o-z)
      common/uset1/nl(5),iflag
      common/uset2/l(5)
      save /uset1/,/uset2/
      character*80 l
      character*(*) lab
      l(5)=lab
      nl(5)=n
      return
      end
      subroutine labels(n)
      implicit real*8 (a-h,o-z)
      save flag
      common/uset1/nl(5),iflag
      common/uset2/l(5)
      save /uset1/,/uset2/
      character*80 l
      logical flag
      data kk,ib,keof,nn,nout/1,0,-1,0,43/
      data flag/.false./
      if(n.gt.0)go to 40
      if(flag)return
      write(nout)kk,(nl(i),i=1,5)
      do 30 i=1,5
      if(nl(i).gt.0)write(nout)kk,l(i)
   30 continue
      flag=.true.
      return
   40 continue
      flag=.false.
      return
      end
      subroutine line2d(xb,yb,np1,np2,nf)
      implicit real*8 (a-h,o-z)
      dimension xb(*),yb(*)
      data nout/43/
      data np4,np5/0,0/
      data nn/0/
      call labels(nn)
      itype=1
      write(nout)itype,np1,np2,nf,np4,np5
      write(nout)itype,(xb(i),i=1,np1),(yb(i),i=1,np1)
      return
      end
      subroutine con2ij(xb,yb,zb,mm,np1,np2,nf)
      implicit real*8 (a-h,o-z)
      dimension xb(*),yb(*),zb(*),mm(*)
      data nout/43/
      data np4,np5/0,0/
      data nn/0/
      call labels(nn)
      nnn=np1*np2
      itype=11
      write(nout)itype,nnn,np2,nf,np1,np5
      write(nout)itype,(xb(i),i=1,nnn),(yb(i),i=1,nnn),
     .                 (zb(i),i=1,nnn),(mm(i),i=1,nnn)
      return
      end
      subroutine con2d(xb,yb,zb,np1,np2,nf)
      implicit real*8 (a-h,o-z)
      dimension xb(*),yb(*),zb(*)
      data nout/43/
      data np4,np5/0,0/
      data nn/0/
      call labels(nn)
      nnn=np1*np2
      itype=10
      write(nout)itype,nnn,np2,nf,np4,np5
      write(nout)itype,(xb(i),i=1,nnn),(yb(i),i=1,nnn),(zb(i),i=1,nnn)
      return
      end
      subroutine line3d(xb,yb,zb,np1,np2,nf)
      implicit real*8 (a-h,o-z)
      dimension xb(*),yb(*),zb(*)
      data nout/43/
      data np4,np5/0,0/
      data nn/0/
      call labels(nn)
      itype=4
      write(nout)itype,np1,np2,nf,np4,np5
      write(nout)itype,(xb(i),i=1,np1),(yb(i),i=1,np1),(zb(i),i=1,np1)
      return
      end
      subroutine vector(xb,yb,xv,yv,np1,nf)
      implicit real*8 (a-h,o-z)
      dimension xb(*),yb(*),xv(*),yv(*)
      data nout/43/
      data np2,np4,np5/0,0,0/
      data nn/0/
      call labels(nn)
      itype=2
      np2=np1
      write(nout)itype,np1,np2,nf,np4,np5
      write(nout)itype,(xb(i),i=1,np1),(yb(i),i=1,np1),
     .                 (xv(i),i=1,np1),(yv(i),i=1,np1)
      return
      end
      subroutine disp2d(xb,yb,xv,yv,np1,np2,nf)
      implicit real*8 (a-h,o-z)
      dimension xb(*),yb(*),xv(*),yv(*)
      data nout/43/
      data np4,np5/0,0/
      data nn/0/
      call labels(nn)
      itype=3
      write(nout)itype,np1,np2,nf,np4,np5
      write(nout)itype,(xb(i),i=1,np1),(yb(i),i=1,np1),
     .                 (xv(i),i=1,np1),(yv(i),i=1,np1)
      return
      end
      subroutine disp3d(xb,yb,zb,xv,yv,zv,np1,np2,nf)
      implicit real*8 (a-h,o-z)
      dimension xb(*),yb(*),zb(*),xv(*),yv(*),zv(*)
      data nout/43/
      data np4,np5/0,0/
      data nn/0/
      call labels(nn)
      itype=5
      write(nout)itype,np1,np2,nf,np4,np5
      write(nout)itype,(xb(i),i=1,np1),(yb(i),i=1,np1),(zb(i),i=1,np1),
     .                 (xv(i),i=1,np1),(yv(i),i=1,np1),(zv(i),i=1,np1)
      return
      end
      subroutine labxy(x,y,lab,n,nf)
      implicit real*8 (a-h,o-z)
      character*(*) lab
      character*80 out
      common/uset1/nl(5),iflag
      common/uset2/l(5)
      save /uset1/,/uset2/
      character*80 l
      data nout/43/
      data np1,np2,np3,np4,np5/0,0,0,0,0/
      data nn/0/
      call labels(nn)
      itype=6
      out=lab
      np1=n
      write(nout)itype,np1,np2,nf,np4,np5
      write(nout)itype,x,y,out
      return
      end
      subroutine labrxy(x,y,lab,n,nf)
      implicit real*8 (a-h,o-z)
      character*(*) lab
      character*80 out
      data nout/43/
      data np1,np2,np3,np4,np5/0,0,0,0,0/
      data nn/0/
      call labels(nn)
      itype=8
      out=lab
      np1=n
      write(nout)itype,np1,np2,nf,np4,np5
      write(nout)itype,x,y,out
      return
      end
      subroutine symbols(x,y,ch,np1,nf)
      implicit real*8 (a-h,o-z)
      dimension x(*),y(*),ch(*)
      character*1 ch
      data nout/43/
      data np2,np3,np4,np5/0,0,0,0/
      data nn/0/
      call labels(nn)
      itype=7
      write(nout)itype,np1,np2,nf,np4,np5
      write(nout)itype,(x(i),i=1,np1),(y(i),i=1,np1),(ch(i),i=1,np1)
      return
      end
      subroutine family(lab,np1,nf)
      implicit real*8 (a-h,o-z)
      character*(*) lab
      character*80 out
      data nout/43/
      data np2,np3,np4,np5/0,0,0,0/
      data nn/0/
      call labels(nn)
      itype=9
      out=lab
      write(nout)itype,np1,np2,nf,np4,np5
      write(nout)itype,out
      return
      end
      subroutine endplt()
      implicit real*8 (a-h,o-z)
      common/uset1/nl(5),iflag
      common/uset2/l(5)
      save /uset1/,/uset2/
      character*80 l
      data kk,keof,nn,nout/1,-1,0,43/
      call labels(nn)
      write(nout)(nn,i=1,6)
      call labels(kk)
      do 10 i=1,5
      nl(i)=0
      l(i)=' '
   10 continue
      return
      end
      subroutine doneplt()
      implicit real*8 (a-h,o-z)
      data keof,nout/-1,43/
      write(nout)(keof,i=1,6)
      close(unit=nout)
      return
      end
