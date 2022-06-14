*deck gap
      program gap
      implicit real*8 (a-h,o-z)
      character*8 wsetup,wnodes,wfix,ww,wsput,wsget,wlast
      character*8 wmat,welem,wstop,wprint,wpress,wload
      character*8 wcon,wscale,wsover,word,wexit
      double precision data
      common /tapes/ nin,nout
      common/info/nmax,nodes,nume,nmat,ijd
      common/list/data(64),nword
      character*8 cdata
      common/clist/cdata(64)
      common/jkcom/jmax,kmax,knode
      common/pcom/iprint
      common/prcom/twopi,ipress
      common/ercom/ierror
      common/outcom/itout
      integer*4 lj
      character*80 illn
      character*8  hed
      dimension hed(20)
      dimension a(2500000)
      data wsetup,wnodes,wfix,ww/'setup','nodes','fix','w'/
      data wsput,wsget,wmat,welem/'sput','sget','mat','elements'/
      data wstop,wprint,wpress,wload/'stop','print','press','load'/
      data wcon,wscale,wsover/'con','scale','sover'/
      data wexit,wlast/'exit','last'/
      data nout/6/
c     open(unit=5,file="input",status='old')
c     open(unit=6,file="output",status='unknown')
c     open(unit=11,file='tape11',form='unformatted',status='unknown')
      rewind 11
      nmax=2500000
      iprint=0
      ipress=0
      ieplot=0
      ierror=0
      itout=0
      twopi=1.
      do 5 i=1,nmax
      a(i)=-1.23e-21
    5 continue
      n1=1
      read(5,'(a)')illn
      read(illn,3000)(hed(i),i=1,20)
c     read(5,3000)(hed(i),i=1,20)
      write(6,3010)n1,(hed(i),i=1,20)
      call ffldsb
   10 continue
      call ffin
      word=cdata(1)
      if(word.eq.wsetup)go to 20
      if(word.eq.wnodes)go to 40
      if(word.eq.wfix)go to 50
      if(word.eq.ww)go to 60
      if(word.eq.wsput)go to 90
      if(word.eq.wsget)go to 100
      if(word.eq.wmat)go to 120
      if(word.eq.welem)go to 140
      if(word.eq.wstop)go to 900
      if(word.eq.wprint)go to 150
      if(word.eq.wpress)go to 160
      if(word.eq.wload)go to 170
      if(word.eq.wcon)go to 180
      if(word.eq.wscale)go to 200
      if(word.eq.wsover)go to 210
      go to 10
   20 continue
      iprint=data(2)
      ni=data(3)
      nj=data(4)
      if(iprint.le.0)iprint=4
      n1=1
      n2=n1+ni*nj
      n3=n2+ni*nj
      n4=n3+ni*nj
      n5=n4+ni*nj
      write(6,1000)ni,nj
      if(n5.le.nmax)go to 30
      write(6,1010)n5
      stop
   30 continue
      call nmesh(a(n1),a(n2),a(n3),ni,nj,iprint)
      iprint=0
      go to 10
   40 continue
      call pnodes(a(n1),a(n2),a(n3),ni,nj)
      go to 10
   50 continue
      call fix(a(n4))
      go to 10
   60 continue
      call ffin
      if(cdata(1).eq.wexit)go to 10
      n=0
      do 70 i=1,64
      if(cdata(i).eq.wlast)go to 80
      n=i
   70 continue
   80 continue
      if(itout.eq.1)then
          lj=n
          write(11)lj,(data(i),i=1,n),(cdata(i),i=1,n)
      else
          write(11)n,(data(i),i=1,n),(cdata(i),i=1,n)
      endif
      go to 60
   90 continue
      call sput(a(n1),a(n2),a(n4),a(n3),a(n5),jmax,kmax)
      call look(a(n4),a(n3))
      go to 10
  100 continue
      rewind 41
      read(41)jmax,kmax
      write(6,1000)jmax,kmax
      ni=jmax
      nj=kmax
      jk=jmax*kmax
      knode=jk
      n1=1
      n2=n1+jk
      n3=n2+jk
      n4=n3+jk
      n5=n4+jk
      if(n5.le.nmax)go to 110
      write(6,1010)n5
      stop
  110 continue
      call sget(a(n1),a(n2),a(n4),jmax,kmax)
      go to 10
  120 continue
      nmat=data(2)
      n6=n5+64*nmat
      if(n6.le.nmax)go to 130
      write(6,1010)n6
      stop
  130 continue
      call gmat(a(n5))
      go to 10
  140 continue
      call pquad(a(n1),a(n2),a(n3),a(n5),ni,nj)
      call look(a(n4),a(n3))
      go to 10
  150 continue
      iprint=data(2)
      go to 10
  160 continue
      call press(a(n1),a(n2),a(n3),a(n4),jmax)
      go to 10
  170 continue
      call load(a(n3),a(n4))
      go to 10
  180 if(ieplot.eq.1)go to 190
      ieplot=1
c      rewind 43
c      call setup(43)
  190 continue
c      call con(a(n1),a(n2),a(n3),a(n4),jmax,kmax)
      go to 10
  200 continue
      if(data(2).eq.0.)twopi=8.*datan(1.d0)
      write(6,3020)twopi
      go to 10
  210 continue
      n6=n5+knode
      n7=n6+jmax
      if(n7.le.nmax)go to 220
      write(6,1010)n7
      stop
  220 continue
      call sover(a(n1),a(n2),a(n3),a(n4),a(n5),a(n6),jmax,kmax)
      go to 10
  900 continue
      rewind 11
      if(ierror.gt.0)write(6,1020)ierror
      stop
 1000 format(1h0,9x,7hjmax = ,i5,5x,7hkmax = ,i5)
 1010 format(' stop need at least ',i5,' words ')
 1020 format(' ----- number of range errors = ',i5,' -----')
 3000 format(20a4)
 3010 format(1h1,i4,5x,20a4)
 3020 format(' scale loads by ',1pe14.4)
      end
      subroutine sover(sx,sy,sxy,sz,p,buf,imax,jmax)
      implicit real*8 (a-h,o-z)
      character*8 wquad
      double precision data
      dimension sx(imax,1),sy(imax,1),sxy(imax,1),sz(imax,1),p(imax,1)
      dimension buf(1)
      common/info/nmax,nodes,nume,nmat,ijd
      character*8 cdata
      common/clist/cdata(64)
      common/list/data(64),nword
      dimension nl(4)
      data wquad/'quad'/
      data ione/1/
      do 20 j=1,jmax
      do 10 i=1,imax
      sx(i,j)=0.
      sy(i,j)=0.
      sxy(i,j)=0.
      sz(i,j)=0.
      p(i,j)=0.
   10 continue
   20 continue
      rewind 11
      rewind 49
      nel=0
   30 continue
      read(11,end=80)n,(data(i),i=1,n)
   40 continue
      if(cdata(1).ne.wquad)go to 30
      ne=data(2)
      nmat=data(3)
      do 45 i=1,nmat
      read(11)n
   45 continue
      do 70 neii=1,ne
      read(11)n,(data(i),i=1,n)
      do 50 j=1,4
      nl(j)=data(2+j)
   50 continue
      read(49)n,(data(i),i=1,9)
      data(10)=-(data(6)+data(7)+data(8))/3d0
      nel=nel+1
      i=0
      j=0
      do 60 m=1,4
      n=nl(m)
      call jkn(n,ii,jj)
      if(ii.lt.i.and.jj.lt.j)go to 60
      i=max(i,ii)
      j=max(j,jj)
   60 continue
      sx(i,j)=data(3)+data(10)
      sy(i,j)=data(4)+data(10)
      sxy(i,j)=data(5)
      sz(i,j)=data(6)+data(10)
      p(i,j)=data(10)
   70 continue
      go to 30
   80 continue
      if(nel.ne.nume)write(6,1000)nel,nume
      rewind 42
      rewind 41
      read(41)ix,jx,ione
      if(ix.ne.imax.or.jx.ne.jmax)write(6,1020)ix,jx,imax,jmax
      write(42)imax,jmax
      do 100 j=1,jmax
      read(41)(buf(i),i=1,imax)
      write(42)(buf(i),i=1,imax)
      read(41)(buf(i),i=1,imax)
      write(42)(buf(i),i=1,imax)
      read(41)(buf(i),i=1,imax)
      write(42)(buf(i),i=1,imax)
      write(42)(sx(i,j),sy(i,j),sxy(i,j),sz(i,j),p(i,j),i=1,imax)
  100 continue
      write(6,1010)nel
      return
 1000 format(' error nel nume ',2i5)
 1010 format(1h0,9x,'overlay   elements  = ',i6)
 1020 format(' error ix,jx,imax,jmax',4i5)
      end
c      subroutine con(xx,yy,nn,zz,jmax,kmax)
c      double precision glab,ylab,com,data
c      dimension xx(jmax,1),yy(jmax,1),nn(jmax,1),zz(jmax,1)
c      dimension zc(30),com(8)
c      common/labcom/glab(10),ylab(10)
c      common/list/data(64),nw
c      common/pcom/iprint
c      data ylab/10*1h /
c      data com/8hsigma-xx,8hsigma-yy,6htau-xy,8hsigma-zz,
c     .     8hsigma-p+,8hsigma-p-,5hangle,1hp/
c      nc=data(2)
c      is=data(3)
c      if(is.le.1)is=1
c      if(nc.eq.0)nc=10
c      if(nc.gt.0)go to 20
c      k=iabs(nc)
c      do 10 i=1,k
c      zc(i)=data(4+i)
c   10 continue
c   20 continue
c      jn=1
c      jx=jmax
c      kn=1
c      kx=kmax
c      encode(36,1020,glab)com(is)
c      is=is+2
c      call gstress(nn,zz,jmax,kmax,is)
c      call conn(jn,jx,kn,kx,jmax,xx,yy,zz,zc,nc,glab,ylab,1)
c      nc=iabs(nc)
c      write(6,1010)(zc(i),i=1,nc)
c      if(iprint.lt.0)return
c      call outline(xx,yy,nn,jmax,kmax)
c      return
c 1000 format(1h0,9x,'number of elements  = ',i6)
c 1010 format(1h0,'contours',/,(1p10e12.4))
c 1020 format(a8,28x)
c      end
*deck outline
c      subroutine outline(xx,yy,nn,imax,jmax)
c      dimension xx(imax,1),yy(imax,1),nn(imax,1)
c      in=imax-1
c      jn=jmax-1
c      do 20 j=1,jmax
c      do 10 i=1,in
c      n1=nn(i,j)
c      n2=nn(i+1,j)
c      if(n1.eq.0.or.n2.eq.0)go to 10
c      if(n1.eq.4.or.n2.eq.4)go to 10
c      call isoplt(xx(i,j),yy(i,j),xx(i+1,j),yy(i+1,j))
c   10 continue
c   20 continue
c      do 40 i=1,imax
c      do 30 j=1,jn
c      n1=nn(i,j)
c      n2=nn(i,j+1)
c      if(n1.eq.0.or.n2.eq.0)go to 30
c      if(n1.eq.4.or.n2.eq.4)go to 30
c      call isoplt(xx(i,j),yy(i,j),xx(i,j+1),yy(i,j+1))
c   30 continue
c   40 continue
c      call isoend
c      return
c      end
c      subroutine gstress(nn,zz,imax,jmax,is)
c      double precision data,wquad
c      dimension nn(imax,1),zz(imax,1)
c      common/info/nmax,nodes,nume,nmat,ijd
c      common/list/data(64),nw
c      dimension nl(4)
c      data wquad/4hquad/
c      do 20 j=1,jmax
c      do 10 i=1,imax
c      zz(i,j)=0
c      nn(i,j)=0
c   10 continue
c   20 continue
c      rewind 11
c      rewind 49
c      nel=0
c   30 continue
c      read(11,end=80)n,(data(i),i=1,n)
c   40 continue
c      if(data(1).ne.wquad)go to 30
c      ne=data(2)
c      nmat=data(3)
c      do 45 i=1,nmat
c      read(11)n
c   45 continue
c      do 70 ii=1,ne
c      read(11)n,(data(i),i=1,n)
c      do 50 j=1,4
c      nl(j)=data(2+j)
c   50 continue
c      read(49)n,(data(i),i=1,9)
c      data(10)=-(data(6)+data(7)+data(8))/3
c      nel=nel+1
c      do 60 m=1,4
c      n=nl(m)
c      call jkn(n,i,j)
c      nn(i,j)=nn(i,j)+1
c      zz(i,j)=zz(i,j)+data(is)
c   60 continue
c   70 continue
c      go to 30
c   80 continue
c      if(nel.ne.nume)write(6,1000)nel,nume
c      do 100 j=1,jmax
c      do 90 i=1,imax
c      if(nn(i,j).eq.0)go to 90
c      zz(i,j)=zz(i,j)/float(nn(i,j))
c      zz(i,j)=onr(1,zz(i,j))
c   90 continue
c  100 continue
c      return
c 1000 format(' error nel nume ',2i5)
c      end
*deck load
      subroutine load(fx,fy)
      implicit real*8 (a-h,o-z)
      double precision data
      character*8 wend,wload,owend,swload
      character*8 bb
      dimension fx(1),fy(1)
      common/info/nmax,nodes,nume,nmat,ijd
      character*8 cdata
      common/clist/cdata(64)
      common/list/data(64),nword
      common/pcom/iprint
      common/prcom/twopi,ipress
      common/outcom/itout
      integer*4 lj
      data wload,wend/'load','end'/
      data owend/'end'/
      data bb/'        '/
      data swload/'load'/
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
      n=njk(i,j)
      fx(n)=px
      fy(n)=py
   30 continue
   40 continue
      go to 20
   50 continue
      nl=0
      z1=1
      n1=1
      z2=2
      n3=3
      if(itout.eq.1)then
          lj=n1
          write(11)lj,zero,wload
      else
          write(11)n1,zero,wload
      endif
      do 70 i=1,nodes
      zi=i
      if(fx(i).eq.0)go to 60
      fx(i)=twopi*fx(i)
      nl=nl+1
      if(itout.eq.1)then
          lj=n3
          write(11)lj,dble(zi),dble(fx(i)),dble(z1),(bb,j=1,n3)
      else
          write(11)n3,dble(zi),dble(fx(i)),dble(z1),(bb,j=1,n3)
      endif
      if(iprint.eq.1)write(6,1020)i,fx(i),z1
   60 if(fy(i).eq.0)go to 70
      fy(i)=twopi*fy(i)
      nl=nl+1
      if(itout.eq.1)then
          lj=n3
          write(11)lj,dble(zi),dble(fy(i)),dble(z2),(bb,j=1,n3)
      else
          write(11)n3,dble(zi),dble(fy(i)),dble(z2),(bb,j=1,n3)
      endif
      if(iprint.eq.1)write(6,1020)i,fy(1),z2
   70 continue
      write(6,1000)nl
      if(itout.eq.1)then
          lj=n1
          write(11)lj,zero,wend
      else
          write(11)n1,zero,wend
      endif
      return
 1000 format(1h0,9x,'number of loads     = ',i6)
 1020 format(' n = ',i5,' f = ',1pe12.4,' t = ',1pe12.4)
      end
*deck press
      subroutine press (x,y,fx,fy,ni)
      implicit real*8 (a-h,o-z)
      double precision data
      character*8 wend
      dimension fx(1),fy(1),x(ni,1),y(ni,1),x2(3),y2(3),wgtl(3),
     .          xgl(3),vec(6),n(3),p(6)
      common/info/nmax,nodes,nume,nmat,ijd
      character*8 cdata
      common/clist/cdata(64)
      common/list/data(64),nw
      common/prcom/twopi,ipress
      common/pcom/iprint
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
      nn=njk(ii,jj)
      n(1)=nn
      x2(1)=x(ii,jj)
      y2(1)=y(ii,jj)
      nn=njk(ii+ip,jj+jp)
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
*deck pre2d
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
      subroutine pquad(xn,yn,ncon,smat,ni,nj)
      implicit real*8 (a-h,o-z)
      double precision data
      character*8 wend,wiload,word,wquad,star,swquad
      character*8 cdata
      character*8 bb
      common/clist/cdata(64)
      dimension xn(ni,1),yn(ni,1),ncon(1),smat(64,1)
      common/info/nmax,nodes,nume,nmat,ijd
      common/list/data(64),nword
      common/pcom/iprint
      common/outcom/itout
      integer*4 lj
      dimension ii(4),jj(4),nni(4),nnj(4),snn(4),io(4),jo(4),ipl(5)
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
      sload=data(2)
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
      znij=ix*jx
      snmat=nmat
      if(itout.eq.1)then
          lj=n4
          write(11)lj,zero,znij,snmat,sload,(wquad,i=1,n4)
      else
          write(11)n4,zero,znij,snmat,sload,(wquad,i=1,n4)
      endif
      do 70 i=1,nmat
      if(itout.eq.1)then
          lj=n64
          write(11)lj,(dble(smat(j,i)),j=1,64),(bb,j=1,64)
      else
          write(11)n64,(dble(smat(j,i)),j=1,64),(bb,j=1,64)
      endif
   70 continue
      call starw(istar)
      i1x=ki(ijd)*ix+kj(ijd)*jx
      i2x=kj(ijd)*ix+ki(ijd)*jx
      nel=0
      do 110 i2=1,i2x
      do 100 i1=1,i1x
      do 80 i=1,4
      io(i)=ii(i)+ki(ijd)*(i1-1)+kj(ijd)*(i2-1)
      jo(i)=jj(i)+kj(ijd)*(i1-1)+ki(ijd)*(i2-1)
      n=njk(io(i),jo(i))
      ncon(n)=1
      snn(i)=n
   80 continue
      nel=nel+1
      nume=nume+1
      snel=nel
      if(itout.eq.1)then
          lj=n6
          write(11)lj,snel,skmat,(snn(i),i=1,4),(bb,i=1,n6)
      else
          write(11)n6,snel,skmat,(snn(i),i=1,4),(bb,i=1,n6)
      endif
      do 90 iy=1,5
      ip=ipl(iy)
      i=io(ip)
      j=jo(ip)
      x(iy)=xn(i,j)
      y(iy)=yn(i,j)
   90 continue
      if(iprint.gt.0)write(6,1020)nume,(snn(i),i=1,4)
      if(iprint.gt.1)write(6,1030)(x(i),y(i),i=1,4)
      call plot(x,y,x,y,5,5,1,1,.false.)
  100 continue
  110 continue
      go to 10
  900 continue
c     call done
      write(6,1010)nume
      return
 1000 format(' bad material number = ',i5)
 1010 format(1h0,9x,'number of elements  = ',i6)
 1020 format(5i6)
 1030 format(1p8e12.4)
      end
      subroutine gmat(smat)
      implicit real*8 (a-h,o-z)
      dimension smat(64,1)
      common/info/nmax,nodes,nume,nmat,ijd
      do 10 i=1,nmat
      call ffld(63,smat(1,i))
   10 continue
      write(6,1000)nmat
      return
 1000 format(1h0,9x,'number of materials = ',i6)
      end
      subroutine look(nsave,ncon)
      implicit real*8 (a-h,o-z)
      dimension ncon(1),nsave(1)
      common/jkcom/jmax,kmax,knode
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
      subroutine fix(ncon)
      implicit real*8 (a-h,o-z)
      double precision data
      character*8 wend
      dimension ncon(1)
      common/jkcom/jmax,kmax,knode
      common/list/data(64),nw
      character*8 cdata
      common/clist/cdata(64)
      common/outcom/itout
      integer*4 lj
      data wend/'end'/
      n5=5
      go to 20
   10 continue
      call ffin
   20 continue
      if(itout.eq.1)then
          lj=n5
          write(11)n5,(data(i),i=1,n5),(cdata(i),i=1,n5)
      else
          write(11)n5,(data(i),i=1,n5),(cdata(i),i=1,n5)
      endif
      if(cdata(1).eq.wend)return
      nstart=data(3)
      nend=data(4)
      inc=data(5)
      if(nend.le.knode.and.nstart.ge.1.and.nstart.le.knode)go to 30
      write(6,2100)nstart,nend
      go to 10
   30 continue
      if(nend.lt.nstart)nend=nstart
      if(inc.le.0)inc=1
      do 40 i=nstart,nend,inc
      ncon(i)=-1
   40 continue
      go to 10
 2100 format(1h0,5x,'nstart,nend',2i6)
      end
      subroutine sget(xx,yy,nn,jmax,kmax)
      implicit real*8 (a-h,o-z)
      dimension xx(jmax,1),yy(jmax,1),nn(jmax,1)
      do 10 k=1,kmax
      read(41)(xx(j,k),j=1,jmax)
      read(41)(yy(j,k),j=1,jmax)
      read(41)(nn(j,k),j=1,jmax)
   10 continue
      n=jmax*kmax
      write(6,1000)n
      return
 1000 format(1h0,9x,'number of nodes     = ',i6)
      end
*deck plot
      subroutine plot(xb,yb,xv,yv,np1,np2,nf,itype,iclose)
      implicit real*8 (a-h,o-z)
      return
      end
      subroutine jkn(n,j,k)
      implicit real*8 (a-h,o-z)
      common/jkcom/jmax,kmax,knode
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
      subroutine sput(xx,yy,nn,ncon,smat,jmax,kmax)
      implicit real*8 (a-h,o-z)
      double precision data
      character*8 wiload,wend,wquad
      character*8 bb
      character*8 cdata
      common/clist/cdata(64)
      dimension xx(jmax,1),yy(jmax,1),nn(jmax,1),x(5),y(5),node(4)
      dimension ncon(1),dnode(4),smat(64,1)
      common/info/nmax,nodes,nume,nmat,ijd
      common/list/data(64),nw
      common/outcom/itout
      integer*4 lj
      data wiload,wend/'end','iloads'/
      data wquad/'quad'/
      data bb/'        '/
      snmat=nmat
      nume=0
      n4=4
      n6=6
      n63=63
    5 continue
      sload=data(2)
    8 continue
      call ffin
      if(cdata(1).eq.wend)go to 900
      if(cdata(1).eq.wiload)go to 5
      mat=data(2)
      jn=data(3)
      jn=max(1,jn)
      kn=data(4)
      kn=max(1,kn)
      jx=data(5)
      jx=min(jx,jmax)
      kx=data(6)
      kx=min(kx,kmax)
      if(mat.le.0)mat=1
      dmat=mat
      nel=0
      do 15 k=kn,kx
      do 10 j=jn,jx
      if(nn(j,k).eq.0)go to 10
      nel=nel+1
   10 continue
   15 continue
      dnel=nel
      zero=0
      if(itout.eq.1)then
          lj=n4
          write(11)lj,zero,dnel,snmat,sloads,(wquad,i=1,4)
      else
          write(11)n4,zero,dnel,snmat,sloads,(wquad,i=1,4)
      endif
      do 20 i=1,nmat
      if(itout.eq.1)then
          lj=n63
          write(11)lj,(dble(smat(j,i)),j=1,63),(bb,j=1,63)
      else
          write(11)n63,(dble(smat(j,i)),j=1,63),(bb,j=1,63)
      endif
   20 continue
      nel=0
      do 40 k=kn,kx
      do 30 j=jn,jx
      if(nn(j,k).eq.0)go to 30
      x(1)=xx(j-1,k-1)
      x(5)=x(1)
      y(1)=yy(j-1,k-1)
      y(5)=y(1)
      x(2)=xx(j,k-1)
      y(2)=yy(j,k-1)
      x(3)=xx(j,k)
      y(3)=yy(j,k)
      x(4)=xx(j-1,k)
      y(4)=yy(j-1,k)
      call plot(x,y,x,y,5,5,1,1,.false.)
      if(jmax.gt.kmax)go to 30
      node(1)=njk(j-1,k-1)
      node(2)=njk(j,k-1)
      node(3)=njk(j,k)
      node(4)=njk(j-1,k)
      nel=nel+1
      do 25 i=1,4
      dnode(i)=node(i)
      kk=node(i)
      ncon(kk)=1
   25 continue
      dnel=nel
      if(itout.eq.1)then
          lj=n6
          write(11)lj,dnel,dmat,(dnode(i),i=1,4),(bb,i=1,n6)
      else
          write(11)n6,dnel,dmat,(dnode(i),i=1,4),(bb,i=1,n6)
      endif
   30 continue
   40 continue
      if(kmax.gt.jmax)go to 70
      do 60 j=jn,jx
      do 50 k=kn,kx
      if(nn(j,k).eq.0)go to 50
      node(1)=njk(j-1,k-1)
      node(2)=njk(j,k-1)
      node(3)=njk(j,k)
      node(4)=njk(j-1,k)
      nel=nel+1
      do 45 i=1,4
      dnode(i)=node(i)
      kk=node(i)
      ncon(kk)=1
   45 continue
      dnel=nel
      if(itout.eq.1)then
          lj=n6
          write(11)lj,dnel,dmat,(dnode(i),i=1,4),(bb,i=1,n6)
      else
          write(11)n6,dnel,dmat,(dnode(i),i=1,4),(bb,i=1,n6)
      endif
   50 continue
   60 continue
   70 continue
      nume=nume+nel
      go to 8
  900 continue
      write(6,1010)nume
c     call done
      return
 1010 format(1h0,9x,'number of elements  = ',i6)
      end
      subroutine starw(istar)
      implicit real*8 (a-h,o-z)
      double precision data
      character*8 wlast,wexit
      character*8 cdata
      common/clist/cdata(64)
      common/list/data(64),nw
      common/outcom/itout
      integer*4 lj
      data wexit,wlast/'exit','last'/
      if(istar.ne.1)return
      istar=istar+1
   60 continue
      call ffin
      if(cdata(1).eq.wexit)return
      n=0
      do 70 i=1,64
      if(cdata(i).eq.wlast)go to 80
      n=i
   70 continue
   80 continue
      if(itout.eq.1)then
         lj=n
         write(11)lj,(data(i),i=1,n),(cdata(i),i=1,n)
      else
          write(11)n,(data(i),i=1,n),(cdata(i),i=1,n)
      endif
      go to 60
      end
      function njk(j,k)
      common/jkcom/jmax,kmax,knode
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
*deck pnodes
      subroutine pnodes(x,y,ib,ni,nj)
      implicit real*8 (a-h,o-z)
      double precision data
      character*8 wend,wnode,wfix,owend,swnode,swfix
      character*8 bb
      dimension x(ni,1),y(ni,1),ib(ni,1)
      character*8 cdata
      common/clist/cdata(64)
      common/list/data(64),nw
      common/info/nmax,nodes,nume,nmat,ijd
      common/jkcom/jmax,kmax,knode
      common/outcom/itout
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
      z2=2
      n3=3
      n4=4
      if(ni.gt.nj)ijd=2
      knode=ni*nj
      zn=knode
      if(itout.eq.1)then
          lj=n4
          write(11)lj,zero,zn,z2,z2,(wnode,i=1,4)
      else
          write(11)n4,zero,zn,z2,z2,(wnode,i=1,4)
      endif
      nodes=knode
      do 10 n=1,nodes
      call jkn(n,i,j)
      ib(i,j)=0
      zn=n
      if(itout.eq.1)then
          lj=n3
          write(11)lj,dble(zn),dble(x(i,j)),dble(y(i,j)),(bb,i=1,n3)
      else
          write(11)n3,dble(zn),dble(x(i,j)),dble(y(i,j)),(bb,i=1,n3)
      endif
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
      call jkn(n,i,j)
      if(ib(i,j).eq.0)go to 70
      val=ib(i,j)
      vn=n
      if(itout.eq.1)then
          lj=n5
          write(11)lj,zero,val,vn,zo,zo,(wfix,i=1,n5)
      else
          write(11)n5,zero,val,vn,zo,zo,(wfix,i=1,n5)
      endif
      nbc=nbc+1
   70 continue
      write(6,1020)nbc
      if(itout.eq.1)then
          lj=n5
          write(11)lj,zero,z0,z0,z0,z0,(wend,i=1,n5)
      else
          write(11)n5,zero,z0,z0,z0,z0,(wend,i=1,n5)
      endif
      return
 1000 format(i5,1p2e12.4)
 1010 format(1h0,9x,'number of nodes     = ',i6)
 1020 format(1h0,9x,'number of bc s      = ',i6)
      end
*deck ffin
      subroutine ffin
      implicit real*8 (a-h,o-z)
      double precision data,sdata
      common /iodata/sdata(64)
      common/list/data(64),nword
      call ffld(64,data)
      do 10 i=1,64
      data(i)=sdata(i)
   10 continue
      return
      end
*deck seg1
      subroutine seg1 (grad,x,n)
      implicit real*8 (a-h,o-z)
c
      dimension x(1)
c
      double precision nseg
c
c     ..subroutine to divide the interval [-1,+1] into  n - 1 segments
c     ..using the uniform gradient method    ( 5/31/74 )
c
      x(n)=1.0d0
      if (n.le.2) go to 50
      nseg=n-1
      ns=nseg
      p=1.0d0
      a=1.0d0/nseg
      if (.01d0-abs(grad-1.0d0)) 20,30,30
   20 p=exp((log(grad))/(nseg-1.0d0))
      a=(p-1.0d0)/(p**ns-1.0d0)
   30 sum=0.0d0
      do 40 i=1,ns
      j=n+1-i
      x(j)=1.0d0-2.0d0*sum
   40 sum=a+p*sum
   50 x(1)=-1.0d0
      return
      end
*deck error
      subroutine error (nover,i,j,n,nchar,word,istop)
      implicit real*8 (a-h,o-z)
      common /ploter/ nplots
      common /tapes/ nin,nout
c
      dimension word(1)
c
      data nchwd,jstop/4,0/
c
      nword=1+(nchar-1)/nchwd
      write (nout,30) nover,(word(i),i=1,nword)
      if (i+j+n.ne.0.and.i.lt.1000) write (nout,40) i,j,n
      if (i+j+n.ne.0.and.i.ge.1000) write (nout,50) i,j,n
c
      jstop=jstop+1
      if (istop.eq.0.and.jstop.lt.50) return
c
c
   10 continue
      jstop=5
      go to (10,20), jstop
   20 return
c
c
c
   30 format (10h0error in ,a6,4x,20a4)
   40 format (5h0i = ,i3,3x,4hj = ,i3,3x,4hn = ,i3)
   50 format (5h0i = ,i10,5x,4hj = ,i10,4hn = ,i10)
      end
*deck ffld
      subroutine ffld (nwords,x)
      implicit real*8 (a-h,o-z)
      double precision data
c
      common /iodata/ data(64)
      character*8 cdata
      common/clist/cdata(64)
c
      dimension x(nwords)
c
c
      do 10 i=1,nwords
      x(i)=0.0d0
   10 continue
c
      do 20 i=1,64
      data(i)=0.0d0
      cdata(i)=' '
   20 continue
      read(14)nword,(data(i),i=1,nword),(cdata(i),i=1,nword)
      if (nwords.eq.64) return
      do 30 i=1,nwords
   30 x(i)=data(i)
      return
      end
*deck ffldsb
      subroutine ffldsb
      implicit real*8 (a-h,o-z)
      common/tapes/ntape5,ntape6
      common/flist/fdata(64),nflag(64),nword
      common/clist/cdata(64)
      character*80 ill2
      character*16 edum
      character*8 cdata,wlast
      character*1 sum(80),x(13),stemp(16),blank,comma,dollar,slash,save
      data x/'1','2','3','4','5','6','7','8','9','0','.','+','-'/
      data blank,comma,dollar,slash/' ', ',', '$', '/'/
      data wlast/'last'/
      rewind 14
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
      read(ntape5,'(a)',end=400,err=400)ill2
      read(ill2,3000,end=400,err=400)sum
c     read(ntape5,3000,end=400,err=400)sum
  125 continue
      write(6,3010)iline,sum
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
      write(cdata(nword),3050,err=400)(stemp(i),i=1,8)
  340 continue
      if(sum(ichar).eq.slash) go to 120
      if(sum(ichar).eq.dollar)go to 360
      go to 130
  350 continue
      nword=nword+1
  360 continue
      cdata(nword)=wlast
      write(14)nword,(fdata(i),i=1,nword),(cdata(i),i=1,nword)
      goto 90
  400 continue
      rewind 14
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
*deck print
      subroutine print2(a,b,imin,jmin,imax,jmax,nd,md,p)
      implicit real*8 (a-h,o-z)
c
      common /tapes/ nin,nout
      character*1 p
      dimension a(nd,md), b(nd,md), p(2)
c
      ima=imin-1
   10 imn=ima+1
      ima=min(ima+10,imax)
      write (nout,30) (i,i=imn,ima)
      write (nout,40)
      do 20 j=jmin,jmax
      write (nout,50) j,p(1),(a(i,j),i=imn,ima)
   20 write (nout,60) p(2),(b(i,j),i=imn,ima)
c
      if (ima.lt.imax) go to 10
c
      return
c
c
   30 format (8h0      i,10(i6,5x))
   40 format (4h   j)
   50 format (i4,2x,a1,1x,1p10e11.3)
   60 format (6x,a1,1x,1p10e11.3)
      end
*deck nmesh
      subroutine nmesh(xn,yn,ibnd,nn,nm,iprint)
      implicit real*8 (a-h,o-z)
c
      character*8 end,polar,arc,arcs,point,pts,fillin
      character*8 fills,connect,arcrs,bkrs,erase,wpoints
      character*8 w,region,word
      character*1 p(2)
      character*8 cdata
      common/clist/cdata(64)
      double precision data
      common /iodata/ data(64)
      common /tapes/ nin,nout
      common /forier/ numel,iplane
      common /nedata/ dum,rmin,zmin,rmax,zmax,nw,irow,imin,jmin,ima
     1x,jmax
c
      dimension xn(nn,1), yn(nn,1), ibnd(nn,1)
      dimension cc(10000), hh(10000), cr(10000), hr(10000)
      dimension n(12), x(12), y(12), xi(9), yi(9), 
     1 a(5), dx(4), dy(4), ndat(12)
c
c     equivalence (n,ndat)
c
      real n
c
      data end,polar,arc,arcs,point,pts,fillin,fills,connect/'end',
     1 'polar','arc','a','point','p','fillin','fill','connect'/
      data arcrs,bkrs,erase/'arcrs','bkrs','erase'/
      data region,wpoints/'region','points'/
      data dum/-1.23e-21/
c
c         subroutine to form finite element mesh using
c           the isoparametric shape functions
c
      p(1)='x'
      p(2)='y'
      if (iplane.ne.0) go to 10
      p(1)='r'
      p(2)='z'
   10 np=0
      pi=datan(1.0d0)/45.d0
      do 20 i=1,nn
      do 20 j=1,nm
      yn(i,j)=dum
   20 xn(i,j)=dum
c
   30 np=np+1
      call ffld (64,data)
      w=cdata(1)
      if (w.eq.end) go to 430
      if (w.eq.arc.or.w.eq.arcs) go to 60
      if (w.eq.point.or.w.eq.pts) go to 50
      if (w.eq.fillin.or.w.eq.fills) go to 170
      if (w.eq.connect) go to 60
      if(w.eq.arcrs)go to 60
      if(w.eq.erase)go to 452
      w=region
      do 40 i=1,12
      x(i)=0.0
      y(i)=0.0
   40 continue
      if(cdata(5).ne.bkrs)go to 290
      w=bkrs
      data(5)=0.d0
      go to 290
c
c        read data for a single point
c
   50 imin=data(2)
      imax=imin
      jmin=data(3)
      jmax=jmin
      word=cdata(6)
      xn(imin,jmin)=data(4)
      yn(imin,jmin)=data(5)
      x(1)=data(4)
      y(1)=data(5)
      xo=data(7)
      yo=data(8)
      go to 370
c
c          read data and set parameters to calculate an arc
c
   60 imin=data(2)
      jmin=data(3)
      imax=data(4)
      jmax=data(5)
      g1=data(6)
      word=cdata(7)
      xo=data(8)
      yo=data(9)
      n1=0
      if (imin.eq.imax) n1=jmax-jmin+1
      if (jmin.eq.jmax) n1=imax-imin+1
      if (n1.le.0) call error ('nmesh',imin,jmin,imax,26,
     1 'bad i,j arc connect limits',1)
      if (g1.le.0.) g1=1.
      call seg1 (g1,cc,n1)
      if (cdata(1).ne.connect) go to 100
      x(3)=0.d0
      y(3)=0.d0
      if (xn(imin,jmin).ne.dum) go to 70
      x(1)=data(10)
      x(2)=xn(imax,jmax)
      y(1)=data(11)
      y(2)=yn(imax,jmax)
      if (word.ne.polar) go to 110
      x1=x(1)
      y1=y(1)
      x(1)=xo+x1*dcos(pi*y1)
      y(1)=yo+x1*dsin(pi*y1)
      go to 90
   70 if (xn(imax,jmax).ne.dum) go to 80
      x(1)=xn(imin,jmin)
      x(2)=data(10)
      y(1)=yn(imin,jmin)
      y(2)=data(11)
      if (word.ne.polar) go to 110
      x1=x(2)
      y1=y(2)
      x(2)=xo+x1*dcos(pi*y1)
      y(2)=yo+x1*dsin(pi*y1)
      go to 90
   80 x(1)=xn(imin,jmin)
      x(2)=xn(imax,jmax)
      y(1)=yn(imin,jmin)
      y(2)=yn(imax,jmax)
   90 if (word.ne.polar) go to 110
      x1=x(1)
      y1=y(1)
      x2=x(2)
      y2=y(2)
      x(1)=dsqrt((x1-xo)**2+(y1-yo)**2)
      y(1)=datan2(y1-yo,x1-xo)/pi
      x(2)=dsqrt((x2-xo)**2+(y2-yo)**2)
      y(2)=datan2(y2-yo,x2-xo)/pi
      go to 110
  100 call ffld (4,x)
      call ffld (4,y)
      if(w.ne.arcrs)go to 110
      call ffld(n1,cc)
      do 105 i=1,n1
      cc(i)=2.d0*cc(i)-1.d0
  105 continue
  110 n1=1
      do 160 i=imin,imax
      do 160 j=jmin,jmax
      xn(i,j)=0.
      yn(i,j)=0.
      s=cc(n1)
      if (x(3).eq.0.) go to 130
      if (x(4).eq.0.) go to 120
      n(1)=1.d0/16.d0*(1.d0-s)*(9.d0*s*s-1.d0)
      n(2)=1.d0/16.d0*(1.d0+s)*(9.d0*s*s-1.d0)
      n(3)=9.d0/16.d0*(1.d0-s*s)*(1.d0-3.d0*s)
      n(4)=9.d0/16.d0*(1.d0-s*s)*(1.d0+3.d0*s)
      iflag=4
      go to 140
  120 continue
      n(1)=.5d0*s*(s-1.d0)
      n(2)=.5d0*s*(s+1.d0)
      n(3)=1.d0-s*s
      iflag=3
      go to 140
  130 n(1)=.5d0*(1.d0-s)
      n(2)=.5d0*(1.d0+s)
      iflag=2
  140 continue
      do 150 k=1,iflag
      xn(i,j)=xn(i,j)+n(k)*x(k)
      yn(i,j)=yn(i,j)+n(k)*y(k)
  150 continue
      n1=n1+1
  160 continue
      go to 370
c
c        fill in coordinates of undefined nodes
c
  170 continue
      do 180 i=1,nn
      do 180 j=1,nm
      ibnd(i,j)=0
  180 if (xn(i,j).ne.dum) ibnd(i,j)=1
      inc=data(6)
      if (inc.eq.0) inc=1
      alpha=data(7)
      omega=data(8)
      if (omega.eq.0.) omega=1.86d0
      iter=data(9)
      if (iter.eq.0) iter=100
      tol=data(10)
      if (tol.le.0.0) tol=1.d-4
      imin=data(2)+inc
      jmin=data(3)+inc
      imax=data(4)-inc
      jmax=data(5)-inc
      if (imin.le.0.or.imin.gt.imax.or.imax.gt.nn) call error ('nmesh',i
     1min,imax,nn,19,'hbad fillin i bounds',1)
      if (jmin.le.0.or.jmin.gt.jmax.or.jmax.gt.nm) call error ('nmesh',j
     1min,jmax,nm,19,'hbad fillin j bounds',1)
c
      xnorm=max(abs(xn(imax+inc,jmax+inc)-xn(imin-inc,jmin-inc)),abs(y
     1n(imax+inc,jmax+inc)-yn(imin-inc,jmin-inc)))*(imax-imin+1)*(jmax-j
     2min+1)/inc/inc
c
      if (iprint.gt.1) write (nout,460) np,imin,jmin,imax,jmax,inc,alpha
     1,omega,iter,tol,xnorm
      alfa=alpha
      do 240 it=1,iter
      resid=0.
      do 230 i=imin,imax,inc
      do 230 j=jmin,jmax,inc
      if (ibnd(i,j).eq.1) go to 230
      xi(1)=(xn(i,j-inc))
      xi(2)=(xn(i+inc,j-inc))
      xi(3)=(xn(i+inc,j))
      xi(4)=(xn(i+inc,j+inc))
      xi(5)=(xn(i,j+inc))
      xi(6)=(xn(i-inc,j+inc))
      xi(7)=(xn(i-inc,j))
      xi(8)=(xn(i-inc,j-inc))
      xi(9)=xi(1)
      yi(1)=yn(i,j-inc)
      yi(2)=yn(i+inc,j-inc)
      yi(3)=yn(i+inc,j)
      yi(4)=yn(i+inc,j+inc)
      yi(5)=yn(i,j+inc)
      yi(6)=yn(i-inc,j+inc)
      yi(7)=yn(i-inc,j)
      yi(8)=yn(i-inc,j-inc)
      yi(9)=yi(1)
      do 190 l=1,4
      a(l)=0.5d0*((xi(2*l)-xn(i,j))*(yi(2*l+1)-yi(2*l-1))-(yi(2*l)-
     1yn(i,j))*(xi(2*l+1)-xi(2*l-1)))
      dx(l)=xi(2*l+1)-xn(i,j)
  190 dy(l)=yi(2*l+1)-yn(i,j)
      a(5)=a(1)
      if (it.gt.1) go to 200
      a(1)=1.d0
      a(2)=1.d0
      a(3)=1.d0
      a(4)=1.d0
      a(5)=1.d0
  200 dxa=0.d0
      dya=0.
      do 210 l=1,5
      a(l)=abs(a(l))
  210 continue
      do 220 l=1,4
      dxa=alfa*(a(l+1)-a(l))/(a(l+1)+a(l))*(-dy(l))+(1.d0-alfa)*dx(l)+
     1 dxa
  220 dya=alfa*(a(l+1)-a(l))/(a(l+1)+a(l))*dx(l)+(1.d0-alfa)*dy(l)+dya
      resid=resid+abs(dxa)+abs(dya)
      xn(i,j)=xn(i,j)+omega*dxa/4.d0
      yn(i,j)=yn(i,j)+omega*dya/4.d0
  230 continue
      resid=resid*omega/xnorm
      if (resid.lt.tol) go to 250
  240 continue
c
      write (nout,470) resid
      go to 260
c
  250 if (iprint.gt.1) write (nout,480) it
  260 continue
      if (inc.eq.1) go to 420
      mini=imin-inc
      maxi=imax+inc
      minj=jmin-inc
      maxj=jmax+inc
      do 280 i=mini,maxi,inc
      do 280 j=minj,maxj,inc
      iinc=i+inc
      jinc=j+inc
      delx=(xn(iinc,jinc)-xn(i,j))/dfloat(inc)
      dely=(yn(iinc,jinc)-yn(i,j))/dfloat(inc)
      incc=inc-1
      do 270 il=1,incc
      iinc1=i+il
      jinc1=j+il
      xn(iinc1,jinc1)=xn(i,j)+delx*dfloat(il)
      yn(iinc1,jinc1)=yn(i,j)+dely*dfloat(il)
  270 continue
  280 continue
      go to 420
c
  290 imin=data(1)
      jmin=data(2)
      imax=data(3)
      jmax=data(4)
      g1=data(5)
      g2=data(6)
      g3=data(7)
      g4=data(8)
      word=cdata(9)
      xo=data(10)
      yo=data(11)
      if (g1.le.0.) g1=1.d0
      if (g2.le.0.) g2=1.d0
      if (g3.le.0.) g3=1.d0
      if (g4.le.0.) g4=1.d0
      if (imin.le.0.or.imin.gt.imax.or.imax.gt.nn) call error ('nmesh',i
     1min,imax,nn,14,'h bad i bounds ',1)
      if (jmin.le.0.or.jmin.gt.jmax.or.jmax.gt.nm) call error ('nmesh',j
     1min,jmax,nm,14,'h bad j bounds ',1)
      call ffld (12,x)
      call ffld (12,y)
c
c       calculate the nodal point coordinates in the psi-eta plane
c       using straight line mesh in the interior
c
      ni=imax-imin+1
      nj=jmax-jmin+1
      call seg1 (g1,cc,ni)
      call seg1 (g3,cr,ni)
      call seg1 (g2,hr,nj)
      call seg1 (g4,hh,nj)
      if(w.ne.bkrs)go to 298
      call ffld(ni,cc)
      call ffld(nj,hr)
      call ffld(ni,cr)
      call ffld(nj,hh)
      do 292 i=1,ni
      cc(i)=2.d0*cc(i)-1.d0
      cr(i)=2.d0*cr(i)-1.d0
  292 continue
      do 294 i=1,nj
      hr(i)=2.d0*hr(i)-1.d0
      hh(i)=2.d0*hh(i)-1.d0
  294 continue
  298 continue
      do 360 i=imin,imax
      ki=i-imin+1
      do 360 j=jmin,jmax
      kj=j-jmin+1
      qx=(cr(ki)-cc(ki))*0.5d0
      px=(cr(ki)+cc(ki))*0.5d0
      qy=(hr(kj)-hh(kj))*0.5d0
      py=(hr(kj)+hh(kj))*0.5d0
      d=1.0d0-qx*qy
      psi=(px+qx*py)/d
      eta=(py+qy*px)/d
c
c     calculate xn and yn
c
      xn(i,j)=0.0d0
      yn(i,j)=0.0d0
      n(1)=1.d0/4.d0*(1.d0-psi)*(1.d0-eta)
      n(2)=1.d0/4.d0*(1.d0+psi)*(1.d0-eta)
      n(3)=1.d0/4.d0*(1.d0+psi)*(1.d0+eta)
      n(4)=1.d0/4.d0*(1.d0-psi)*(1.d0+eta)
      iflag=1
      do 300 kk=5,12
      ii=17-kk
      n(ii)=0.0d0
      if (x(ii).ne.0.0) then
         go to 310
      endif
  300 continue
      go to 320
  310 call fill (x,y,iflag)
  320 continue
      go to (350,330,340), iflag
  330 continue
      n(1)=n(1)*(-psi-eta-1.d0)
      n(2)=n(2)*(psi-eta-1.d0)
      n(3)=n(3)*(psi+eta-1.d0)
      n(4)=n(4)*(-psi+eta-1.d0)
      n(5)=1.d0/2.d0*(1.d0-psi**2)*(1.d0-eta)
      n(6)=1.d0/2.d0*(1.d0-eta**2)*(1.d0+psi)
      n(7)=1.d0/2.d0*(1.d0-psi**2)*(1.d0+eta)
      n(8)=1.d0/2.d0*(1.d0-eta**2)*(1.d0-psi)
      go to 350
  340 continue
      n(1)=n(1)*(1.d0/8.d0*(-10.d0+9.d0*(psi**2+eta**2)))
      n(2)=n(2)*(1.d0/8.d0*(-10.d0+9.d0*(psi**2+eta**2)))
      n(3)=n(3)*(1.d0/8.d0*(-10.d0+9.d0*(psi**2+eta**2)))
      n(4)=n(4)*(1.d0/8.d0*(-10.d0+9.d0*(psi**2+eta**2)))
      n(5)=9.d0/32.d0*(1.d0-psi**2)*(1.d0-eta)*(1.d0-3.d0*psi)
      n(6)=9.d0/32.d0*(1.d0-eta**2)*(1.d0+psi)*(1.d0-3.d0*eta)
      n(7)=9.d0/32.d0*(1.d0-psi**2)*(1.d0+eta)*(1.d0+3.d0*psi)
      n(8)=9.d0/32.d0*(1.d0-eta**2)*(1.d0-psi)*(1.d0+3.d0*eta)
      n(9)=9.d0/32.d0*(1.d0-psi**2)*(1.d0-eta)*(1.d0+3.d0*psi)
      n(10)=9.d0/32.d0*(1.d0-eta**2)*(1.d0+psi)*(1.d0+3.d0*eta)
      n(11)=9.d0/32.d0*(1.d0-psi**2)*(1.d0+eta)*(1.d0-3.d0*psi)
      n(12)=9.d0/32.d0*(1.d0-eta**2)*(1.d0-psi)*(1.d0-3.d0*eta)
  350 continue
      do 360 ii=1,12
      xn(i,j)=xn(i,j)+n(ii)*x(ii)
      yn(i,j)=yn(i,j)+n(ii)*y(ii)
  360 continue
  370 if (word.ne.polar) go to 390
      do 380 i=imin,imax
      do 380 j=jmin,jmax
      r=xn(i,j)
      ang=yn(i,j)*pi
      xn(i,j)=xo+r*dcos(ang)
  380 yn(i,j)=yo+r*dsin(ang)
c
c     print input data
c
  390 if (iprint.le.1) go to 30
      write (nout,490) np,w,imin,jmin,imax,jmax,g1,g2,g3,g4
      ndp=0
      do 410 i=1,12
      if (i.le.4) go to 400
      if (x(i).eq.0.0.and.y(i).eq.0.0) go to 410
  400 ndp=ndp+1
      ndat(ndp)=i
      cr(ndp)=x(i)
      hr(ndp)=y(i)
  410 continue
      write (nout,500) (ndat(i),i=1,ndp)
      write (nout,510) p(1),(cr(i),i=1,ndp)
      write (nout,510) p(2),(hr(i),i=1,ndp)
  420 if (iprint.lt.4) go to 30
      call print2(xn,yn,imin,jmin,imax,jmax,nn,nm,p)
      go to 30
  430 np=np-1
      if (np.le.0) call error ('nmesh',0,0,0,12,'hno mesh data',1)
      imin=nn
      jmin=nm
      imax=0
      jmax=0
      rmax=1.0d0/dum
      rmin=-rmax
      zmin=rmin
      zmax=rmax
      do 440 i=1,nn
      do 440 j=1,nm
      if (xn(i,j).eq.dum) go to 440
      imin=min(imin,i)
      jmin=min(jmin,j)
      imax=max(imax,i)
      jmax=max(jmax,j)
      rmin=min(rmin,xn(i,j))
      zmin=min(zmin,yn(i,j))
      rmax=max(rmax,xn(i,j))
      zmax=max(zmax,yn(i,j))
  440 continue
      nw=nn*(jmax-jmin+1)
      irow=nn
      write (nout,530) imin,jmin,imax,jmax,p(1),rmin,p(2),zmin,p(1),rmax
     1,p(2),zmax
      if (cdata(2).ne.wpoints) go to 450
  450 continue
      if (iprint.lt.3) return
      write (nout,520) np
      call print2(xn,yn,imin,jmin,imax,jmax,nn,nm,p)
      return
  452 continue
      imin=data(2)
      jmin=data(3)
      imax=data(4)
      jmax=data(5)
      if(iprint.gt.1)write(nout,490)np,w,imin,jmin,imax,jmax,0.,0.,0.,0.
      do 456 j=jmin,jmax
      do 454 i=imin,imax
      xn(i,j)=dum
      yn(i,j)=dum
  454 continue
  456 continue
      go to 30
c
c
c
  460 format
     .(9h0part no.,i3,7h  imin=,i3,7h  jmin=,i3,7h  imax=,i3,7h  jmax=
     .   ,i3,//,25h  fillin parameters  inc=,i2,8h  alpha=,1pe11.3,
     .8h  omega=,1pe11.3,7h  iter=,i4,6h  tol=,1pe11.3,
     .8h  xnorm=,1pe11.3)
  470 format (38h0fillin did not converge.  residual = ,1pe11.3)
  480 format (20h0fillin converged in,i4,11h iterations)
  490 format (9h0part no.,i3,2x,a10,7h  imin=,i3,7h  jmin=,i3,7h  imax=,
     1i3,7h  jmax=,i3,5h  g1=,f6.3,5h  g2=,f6.3,5h  g3=,f6.3,5h  g4=,f6.
     23)
  500 format (12h data points/4x,12(4x,i2,5x))
  510 format (2x,a1,1x,1p12e11.3)
  520 format (35h1all coordinates generated for all ,i3,6h parts)
  530 format (69h0range of i and j values and coordinates found during m
     1esh generation//9h  imin = ,i3,9x,9h  jmin = ,i3,9x,9h  imax = ,i3
     2,9x,9h  jmax = ,i3//2(2x,a1,6hmin = ,1pe12.4),2(2x,a1,6hmax = ,1pe
     312.4))
      end
*deck fill
      subroutine fill (x,y,iflag)
      implicit real*8 (a-h,o-z)
c
      dimension x(12), y(12)
c
c     fills in any omitted data points by averaging
c
      if (x(9).ne.0.0) go to 20
      if (x(10).ne.0.0) go to 20
      if (x(11).ne.0.0) go to 20
      if (x(12).ne.0.0) go to 20
      if (x(5).ne.0.0) go to 10
      if (x(6).ne.0.0) go to 10
      if (x(7).ne.0.0) go to 10
      if (x(8).ne.0.0) go to 10
      return
   10 continue
      if (x(8).eq.0.0) x(8)=(x(1)+x(4))/2.d0
      if (y(8).eq.0.0) y(8)=(y(1)+y(4))/2.d0
      if (x(7).eq.0.0) x(7)=(x(4)+x(3))/2.d0
      if (y(7).eq.0.0) y(7)=(y(4)+y(3))/2.d0
      if (x(6).eq.0.0) x(6)=(x(3)+x(2))/2.d0
      if (y(6).eq.0.0) y(6)=(y(3)+y(2))/2.d0
      if (x(5).eq.0.0) x(5)=(x(2)+x(1))/2.d0
      if (y(5).eq.0.0) y(5)=(y(2)+y(1))/2.d0
      iflag=2
      return
   20 continue
      if (x(5).eq.0.0) x(5)=x(1)+(x(2)-x(1))/3.d0
      if (y(5).eq.0.0) y(5)=y(1)+(y(2)-y(1))/3.d0
      if (x(6).eq.0.0) x(6)=x(2)+(x(3)-x(2))/3.d0
      if (y(6).eq.0.0) y(6)=y(2)+(y(3)-y(2))/3.d0
      if (x(7).eq.0.0) x(7)=x(3)+(x(4)-x(3))/3.d0
      if (y(7).eq.0.0) y(7)=y(3)+(y(4)-y(3))/3.d0
      if (x(8).eq.0.0) x(8)=x(4)+(x(1)-x(4))/3.d0
      if (y(8).eq.0.0) y(8)=y(4)+(y(1)-y(4))/3.d0
      if (y(9).eq.0.0) y(9)=y(1)+2.d0*(y(2)-y(1))/3.d0
      if (x(9).eq.0.0) x(9)=x(1)+2.d0*(x(2)-x(1))/3.d0
      if (x(10).eq.0.0) x(10)=x(2)+2.d0*(x(3)-x(2))/3.d0
      if (y(10).eq.0.0) y(10)=y(2)+2.d0*(y(3)-y(2))/3.d0
      if (x(11).eq.0.0) x(11)=x(3)+2.d0*(x(4)-x(3))/3.d0
      if (y(11).eq.0.0) y(11)=y(3)+2.d0*(y(4)-y(3))/3.d0
      if (x(12).eq.0.0) x(12)=x(4)+2.d0*(x(1)-x(4))/3.d0
      if (y(12).eq.0.0) y(12)=y(4)+2.d0*(y(1)-y(4))/3.d0
      iflag=3
      return
      end

