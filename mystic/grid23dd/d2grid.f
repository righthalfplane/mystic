*deck gap
      program gap
c     program gap(input,output,tape5=input,tape6=output,tape43,tape11,
c    . tape42,tape14,tape49,tape41)
      common /tapes/ nout
      common/info/nmax,nodes,nume,nmat,ijd
      common/list/data(64),nword
      common/listo/odata(64)
      common/jkcom/jmax,kmax,knode
      common/pcom/iprint
      common/prcom/twopi,ipress
      common/ercom/ierror
      common/origin/x0,y0
      dimension hed(20)
      dimension a(25000)
      data wsetup,wnodes,wfix,ww/5hsetup,5hnodes,3hfix,1hw/
      data wsput,wsget,wmat,welem/4hsput,4hsget,3hmat,8helements/
      data wstop,wprint,wpress,wload/4hstop,5hprint,5hpress,4hload/
      data wcon,wscale,wsover/3hcon,5hscale,5hsover/
      data wexit,wlast,wlines/4hexit,4hlast,5hlines/
      data nout/6/
      rewind 11
      call setup(43)
      nmax=25000
      iprint=0
      ipress=0
      ieplot=0
      ierror=0
      twopi=1.
      x0=0.
      y0=0.
      do 5 i=1,nmax
      a(i)=-1.23e-21
    5 continue
      n1=1
      read(5,3000)(hed(i),i=1,20)
      write(6,3010)n1,(hed(i),i=1,20)
   10 continue
      call ffin
      word=data(1)
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
      if(word.eq.wlines)go to 230
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
      read(5,2000)(data(i),i=1,10)
      if(data(1).eq.wexit)go to 10
      write(11,2000)(data(i),i=1,10)
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
  190 continue
      call con(a(n1),a(n2),a(n3),a(n4),jmax,kmax)
      go to 10
  200 continue
      if(data(2).eq.0.)twopi=8.*atan(1.)
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
  230 continue
      call plines(a(n1),a(n2),a(n3),a(n4),jmax,kmax)
      go to 10
  900 continue
      rewind 11
      if(ierror.gt.0)write(6,1020)ierror
      call exit(0)
 1000 format(1h0,9x,7hjmax = ,i5,5x,7hkmax = ,i5)
 1010 format(' stop need at least ',i5,' words ')
 1020 format(' ----- number of range errors = ',i5,' -----')
 2000 format(10a8)
 3000 format(20a4)
 3010 format(1h1,i4,5x,20a4)
 3020 format(' scale loads by ',1pe14.4)
      end
      subroutine sover(sx,sy,sxy,sz,p,buf,imax,jmax)
      dimension sx(imax,1),sy(imax,1),sxy(imax,1),sz(imax,1),p(imax,1)
      dimension buf(1)
      common/info/nmax,nodes,nume,nmat,ijd
      common/list/data(64),nword
      dimension nl(4)
      data wquad/4hquad/
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
      if(data(1).ne.wquad)go to 30
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
      data(10)=-(data(6)+data(7)+data(8))/3
      nel=nel+1
      i=0
      j=0
      do 60 m=1,4
      n=nl(m)
      call jkn(n,ii,jj)
      if(ii.lt.i.and.jj.lt.j)go to 60
      i=max0(i,ii)
      j=max0(j,jj)
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
      subroutine plines(xx,yy,nn,zz,jmax,kmax)
      dimension xx(jmax,1),yy(jmax,1),nn(jmax,1),zz(jmax,1)
      dimension zc(30),com(11)
      common/uset/nl(5),l(10,5)
      common/list/data(64),nw
      common/origin/x0,y0
      common/pcom/iprint
      dimension x(100),y(100)
      data wend,wiline,wjline/3hend,5hiline,5hjline/
      data com/8hsigma-xx,8hsigma-yy,6htau-xy,8hsigma-zz,
     .     8hsigma-p+,8hsigma-p-,5hangle,1hp,8hsigma-rr,
     .     8hsigma-tt,8hsigma-rt/
      equivalence (tlab,data(3))
      nc=data(2)
      if(nc.eq.0)nc=10
      is=data(3)
      if(is.le.1)is=1
      idump=data(4)
      if(idump.lt.1)idump=1
      ipoint=data(5)
      if(ipoint.lt.1)ipoint=4
      if(nc.gt.0)go to 20
      k=iabs(nc)
      do 10 i=1,k
      zc(i)=data(4+i)
   10 continue
   20 continue
      nl(1)=1
      nl(2)=8
      nl(3)=13
      encode(8,1020,l(1,2))com(is)
      rewind 49
      do 90 idd=1,idump
      jn=1
      jx=jmax
      kn=1
      kx=kmax
      iss=is+2
      call gstress(nn,zz,jmax,kmax,iss,ipoint)
   30 continue
      call ffin
      if(data(1).eq.wend)go to 90
      nc=data(2)
      encode(13,1030,l(1,3))data(1),nc
      if(data(1).eq.wiline)go to 50
      j=nc
      n=0
      do 40 i=1,jmax
      if(nn(i,j).eq.0)go to 40
      n=n+1
      y(n)=zz(i,j)
      x(n)=xval(data(3),xx(i,j),yy(i,j))
   40 continue
      call plot(x,y,x,y,n,n,1,1,.true.)
      go to 30
   50 continue
      i=nc
      n=0
      do 60 j=1,kmax
      if(nn(i,j).eq.0)go to 60
      n=n+1
      y(n)=zz(i,j)
      x(n)=xval(data(3),xx(i,j),yy(i,j))
   60 continue
      call plot(x,y,x,y,n,n,1,1,.true.)
      go to 30
   90 continue
      nl(1)=0
      nl(2)=0
      nl(3)=0
      return
 1000 format(1h0,9x,'number of elements  = ',i6)
 1010 format(1h0,'contours',/,(1p10e12.4))
 1020 format(a8)
 1030 format(a5,' = ',i5)
      end
      function xval(t,x,y)
      common/uset/nl(5),l(10,5)
      common/origin/x0,y0
      data tx,ty,tr,tt/1hx,1hy,1hr,1ht/
      data lx,ly,lr,lt/1hx,1hy,1hr,1ht/
      data fac/57.296/
      if(t.ne.tx)go to 10
      xval=x
      l(1,1)=lx
      return
   10 if(t.ne.ty)go to 20
      xval=y
      l(1,1)=ly
      return
   20 if(t.ne.tt)go to 30
      xval=fac*atan2(y-y0,x-x0)
      l(1,1)=lt
      return
   30 continue
      xval=sqrt((x-x0)**2+(y-y0)**2)
      l(1,1)=lr
      return
      end
      subroutine con(xx,yy,nn,zz,jmax,kmax)
      dimension xx(jmax,1),yy(jmax,1),nn(jmax,1),zz(jmax,1)
      dimension zc(30),com(11)
      common/labcom/glab(10),ylab(10)
      common/list/data(64),nw
      common/pcom/iprint
      data ylab/10*1h /
      data com/8hsigma-xx,8hsigma-yy,6htau-xy,8hsigma-zz,
     .     8hsigma-p+,8hsigma-p-,5hangle,1hp,8hsigma-rr,
     .     8hsigma-tt,8hsigma-rt/
      nc=data(2)
      if(nc.eq.0)nc=10
      is=data(3)
      if(is.le.1)is=1
      idump=data(4)
      if(idump.lt.1)idump=1
      ipoint=data(5)
      if(ipoint.lt.1)ipoint=4
      if(nc.gt.0)go to 20
      k=iabs(nc)
      do 10 i=1,k
      zc(i)=data(4+i)
   10 continue
   20 continue
      rewind 49
      do 30 idd=1,idump
      jn=1
      jx=jmax
      kn=1
      kx=kmax
      encode(36,1020,glab)com(is)
      iss=is+2
      call gstress(nn,zz,jmax,kmax,iss,ipoint)
      call conn(jn,jx,kn,kx,jmax,xx,yy,zz,zc,nc,glab,ylab,1)
      nc=iabs(nc)
      write(6,1010)(zc(i),i=1,nc)
      if(iprint.lt.0)go to 30
      call outline(xx,yy,nn,jmax,kmax,ipoint)
      call done
   30 continue
      return
 1000 format(1h0,9x,'number of elements  = ',i6)
 1010 format(1h0,'contours',/,(1p10e12.4))
 1020 format(a8,28x)
      end
*deck outline
      subroutine outline(xx,yy,nn,imax,jmax,ipoint)
      dimension xx(imax,1),yy(imax,1),nn(imax,1)
      np=4*ipoint
      in=imax-1
      jn=jmax-1
      do 20 j=1,jmax
      do 10 i=1,in
      n1=nn(i,j)
      n2=nn(i+1,j)
      if(n1.eq.0.or.n2.eq.0)go to 10
      if(n1.eq.np.or.n2.eq.np)go to 10
      call isoplt(xx(i,j),yy(i,j),xx(i+1,j),yy(i+1,j))
   10 continue
   20 continue
      do 40 i=1,imax
      do 30 j=1,jn
      n1=nn(i,j)
      n2=nn(i,j+1)
      if(n1.eq.0.or.n2.eq.0)go to 30
      if(n1.eq.np.or.n2.eq.np)go to 30
      call isoplt(xx(i,j),yy(i,j),xx(i,j+1),yy(i,j+1))
   30 continue
   40 continue
      call isoend
      return
      end
      subroutine gstress(nn,zz,imax,jmax,is,ipoint)
      dimension nn(imax,1),zz(imax,1)
      common/info/nmax,nodes,nume,nmat,ijd
      common/list/data(64),nw
      common/origin/x0,y0
      dimension nl(4)
c     onr(n,r)=(r.or.(2**(n-1)))
      data wquad/4hquad/
      do 20 j=1,jmax
      do 10 i=1,imax
      zz(i,j)=0
      nn(i,j)=0
   10 continue
   20 continue
      rewind 11
      nel=0
   30 continue
      read(11,end=80)n,(data(i),i=1,n)
   40 continue
      if(data(1).ne.wquad)go to 30
      ne=data(2)
      nmat=data(3)
      do 45 i=1,nmat
      read(11)n
   45 continue
      do 70 ii=1,ne
      read(11)n,(data(i),i=1,n)
      do 50 j=1,4
      nl(j)=data(2+j)
   50 continue
      do 65 ip4=1,ipoint
      read(49)n,(data(i),i=1,9)
      data(10)=-(data(6)+data(7)+data(8))/3
      theta=2.*atan2(data(2)-y0,data(1)-x0)
      sxy=data(5)
      s1=.5*(data(3)+data(4))
      s2=.5*(data(3)-data(4))
      data(11)=s1+s2*cos(theta)+sxy*sin(theta)
      data(12)=s1-s2*cos(theta)-sxy*sin(theta)
      data(13)=-s2*sin(theta)+sxy*cos(theta)
      nel=nel+1
      do 60 m=1,4
      n=nl(m)
      call jkn(n,i,j)
      nn(i,j)=nn(i,j)+1
      zz(i,j)=zz(i,j)+data(is)
   60 continue
   65 continue
   70 continue
      go to 30
   80 continue
c      if(nel.ne.nume)write(6,1000)nel,nume
      do 100 j=1,jmax
      do 90 i=1,imax
      if(nn(i,j).eq.0)go to 90
      zz(i,j)=zz(i,j)/float(nn(i,j))
c     zz(i,j)=onr(1,zz(i,j))
   90 continue
  100 continue
      return
 1000 format(' error nel nume ',2i5)
      end
*deck load
      subroutine load(fx,fy)
      dimension fx(1),fy(1)
      common/info/nmax,nodes,nume,nmat,ijd
      common/list/data(64),nword
      common/pcom/iprint
      common/prcom/twopi,ipress
      data wload,wend/4hload,3hend/
      data owend/3hend/
      if(ipress.eq.1)go to 20
      ipress=1
      do 10 n=1,nodes
      fx(n)=0.
      fy(n)=0.
   10 continue
   20 continue
      call ffin
      if(data(1).eq.wend)go to 50
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
      write(11)n1,wload
      do 70 i=1,nodes
      zi=i
      if(fx(i).eq.0)go to 60
      fx(i)=twopi*fx(i)
      nl=nl+1
      write(11)n3,(zi),(fx(i)),(z1)
      if(iprint.eq.1)write(6,1020)i,fx(i),z1
   60 if(fy(i).eq.0)go to 70
      fy(i)=twopi*fy(i)
      nl=nl+1
      write(11)n3,(zi),(fy(i)),(z2)
      if(iprint.eq.1)write(6,1020)i,fy(1),z2
   70 continue
      write(6,1000)nl
      write(11)n1,wend
      return
 1000 format(1h0,9x,'number of loads     = ',i6)
 1020 format(' n = ',i5,' f = ',1pe12.4,' t = ',1pe12.4)
      end
*deck press
      subroutine press (x,y,fx,fy,ni)
      dimension fx(1),fy(1),x(ni,1),y(ni,1),x2(3),y2(3),wgtl(3),
     .          xgl(3),vec(6),n(3),p(6)
      common/info/nmax,nodes,nume,nmat,ijd
      common/list/data(64),nw
      common/prcom/twopi,ipress
      common/pcom/iprint
      data wend/3hend/
      data xgl/
     . -.7745966692415, .0000000000000, .7745966692415/
      data wgtl/
     .  .5555555555556, .8888888888889, .5555555555556/
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
      if(data(1).eq.wend)return
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
      subroutine pquad(xn,yn,ncon,smat,ni,nj)
      dimension xn(ni,1),yn(ni,1),ncon(1),smat(64,1)
      common/info/nmax,nodes,nume,nmat,ijd
      common/list/data(64),nword
      common/pcom/iprint
      dimension ii(4),jj(4),nni(4),nnj(4),snn(4),io(4),jo(4),ipl(5)
      dimension ki(2),kj(2),x(5),y(5)
      data wend,wiload,star/3hend,5hiload,4hstar/
      data nni/0,1,1,0/,nnj/0,0,1,1/
      data ki/1,0/,kj/0,1/
      data ipl/1,2,3,4,1/
      n4=4
      n6=6
      n64=64
      nume=0
      istar=0
      if(data(3).eq.star)istar=1
    5 continue
      sload=data(2)
   10 continue
      call ffin
      word=data(1)
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
      write(11)n4,wquad,(znij),(snmat),(sload)
      do 70 i=1,nmat
      write(11)n64,((smat(j,i)),j=1,64)
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
      write(11)n6,(snel),(skmat),((snn(i)),i=1,4)
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
      call done
      write(6,1010)nume
      return
 1000 format(' bad material number = ',i5)
 1010 format(1h0,9x,'number of elements  = ',i6)
 1020 format(5i6)
 1030 format(1p8e12.4)
      end
      subroutine gmat(smat)
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
      subroutine din
      common/list/data(64),nw
      common/listo/odata(64)
      do 10 i=1,64
      odata(i)=odata(i)
   10 continue
      return
      end
      subroutine fix(ncon)
      dimension ncon(1)
      common/jkcom/jmax,kmax,knode
      common/list/data(64),nw
      common/listo/odata(64)
      data wend/3hend/
      n5=5
      go to 20
   10 continue
      call ffin
   20 continue
      call din
      write(11)n5,(data(i),i=1,n5)
      if(data(1).eq.wend)return
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
      subroutine jkn(n,j,k)
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
      dimension xx(jmax,1),yy(jmax,1),nn(jmax,1),x(5),y(5),node(4)
      dimension ncon(1),dnode(4),smat(64,1)
      common/info/nmax,nodes,nume,nmat,ijd
      common/list/data(64),nw
      data wiload,wend/6hiloads,3hend/
      data wquad/4hquad/
      snmat=nmat
      nume=0
      n4=4
      n6=6
      n63=63
    5 continue
      sload=data(2)
    8 continue
      call ffin
      if(data(1).eq.wend)go to 900
      if(data(1).eq.wiloads)go to 5
      mat=data(2)
      jn=data(3)
      jn=max0(1,jn)
      kn=data(4)
      kn=max0(1,kn)
      jx=data(5)
      jx=min0(jx,jmax)
      kx=data(6)
      kx=min0(kx,kmax)
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
      write(11,2000)nel+nume,(node(i),i=1,4),nn(j,k)
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
      write(11,2000)nel+nume,(node(i),i=1,4),nn(j,k)
   50 continue
   60 continue
   70 continue
      nume=nume+nel
      go to 8
  900 continue
      write(6,1010)nume
      call done
      return
 1010 format(1h0,9x,'number of elements  = ',i6)
 2000 format(6i5,'    1')
      end
      subroutine starw(istar)
      common/list/data(64),nw
      data wexit,wlast/4hexit,4hlast/
      if(istar.ne.1)return
      istar=istar+1
   60 continue
      call ffin
      if(data(1).eq.wexit)return
      n=0
      do 70 i=1,64
      if(data(i).eq.wlast)go to 80
      n=i
   70 continue
   80 continue
      write(11)n,(data(i),i=1,n)
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
      subroutine pnodes(x,y,b,ni,nj)
      dimension x(ni,1),y(ni,1),b(ni,1)
      common/list/data(64),nw
      common/info/nmax,nodes,nume,nmat,ijd
      common/jkcom/jmax,kmax,knode
      data wfix,wend/3hfix,3hend/
      data wnode/4hnode/
      data owend/3hend/
      jmax=ni
      kmax=nj
      ijd=1
      z2=2
      n3=3
      n4=4
      if(ni.gt.nj)ijd=2
      knode=ni*nj
      zn=knode
      nodes=knode
      do 10 n=1,nodes
      call jkn(n,i,j)
      b(i,j)=0
   10 continue
      write(6,1010)nodes
      nbc=0
   30 continue
      call ffin
      if(data(1).eq.wend)go to 60
      val=data(1)
      i1n=data(2)
      j1n=data(3)
      i1x=data(4)
      j1x=data(5)
      do 50 j=j1n,j1x
      do 40 i=i1n,i1x
      b(i,j)=val
   40 continue
   50 continue
      go to 30
   60 continue
      n5=5
      zo=0.
      do 70 n=1,nodes
      call jkn(n,i,j)
      write(11,1000)n,b(i,j),x(i,j),y(i,j)
      if(b(i,j).eq.0)go to 70
      nbc=nbc+1
   70 continue
      write(6,1020)nbc
      return
 1000 format(i5,f5.0,f10.3,f10.3,'    1')
 1010 format(1h0,9x,'number of nodes     = ',i6)
 1020 format(1h0,9x,'number of bc s      = ',i6)
      end
*deck ffin
*deck seg1
      subroutine seg1 (grad,x,n)
c
      dimension x(1)
c
      real nseg
c
c     ..subroutine to divide the interval [-1,+1] into  n - 1 segments
c     ..using the uniform gradient method    ( 5/31/74 )
c
      x(n)=1.0
      if (n.le.2) go to 50
      nseg=n-1
      ns=nseg
      p=1.0
      a=1.0/nseg
      if (.01-abs(grad-1.0)) 20,30,30
   20 p=exp((alog(grad))/(nseg-1.0))
      a=(p-1.0)/(p**ns-1.0)
   30 sum=0.0
      do 40 i=1,ns
      j=n+1-i
      x(j)=1.0-2.0*sum
   40 sum=a+p*sum
   50 x(1)=-1.0
      return
      end
*deck error
      subroutine error (nover,i,j,n,nchar,word,istop)
      common /ploter/ nplots
      common /tapes/ nout
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
      subroutine ffin
      common /iodata/sdata(64)
      common/list/data(64),nword
      call ffld(64,data)
      do 10 i=1,64
      data(i)=sdata(i)
   10 continue
      return
      end
*deck ffld
      subroutine ffld (nwords,x)
c
      common /iodata/ data(64)
      common /list/ bdata(64),nword
c
      dimension x(nwords)
c
c
      call ffldsb
      do 5 i=1,64
      data(i)=bdata(i)
    5 continue
      if (nwords.eq.64) return
      do 10 i=1,nwords
   10 x(i)=data(i)
      return
      end
*deck ffldsb
      subroutine ffldsb
      common /list/ data(64),nword
      dimension sum(80),x(13),stemp(16),edum(2)
      data x/1h1,1h2,1h3,1h4,1h5,1h6,1h7,1h8,1h9,1h0,1h.,1h+,1h-/
      data blank,comma,dollar,slash/1h ,1h,,1h$,1h/ /
      data iswap/0/
      iline=1
   90 continue
      iline=iline+1
      do 100 j=1,64
  100 data(j)=0.0
      nword=0
  120 continue
      read(5,3000,end=370)sum
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
  370 continue
      return
 3000 format(80a1)
 3010 format(i5,5x,80a1)
 3020 format(16a1)
 3030 format(4a1)
 3040 format(e16.0)
 3050 format(8a1)
      end
*deck print
      subroutine print (a,b,imin,jmin,imax,jmax,nd,md,p)
c
      common /tapes/ nout
      dimension a(nd,md), b(nd,md), p(2)
c
      ima=imin-1
   10 imn=ima+1
      ima=min0(ima+10,imax)
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
c
      common /iodata/ data(64)
      common /tapes/ nout
      common /forier/ numel,iplane
      common /nedata/ p(2),dum,rmin,zmin,rmax,zmax,nw,irow,imin,jmin,ima
     1x,jmax
c
      dimension xn(nn,1), yn(nn,1), ibnd(nn,1), cc(100), hh(100), cr(100
     1), hr(100), n(12), x(12), y(12), xi(9), yi(9), a(5), dx(4), dy(4),
     .  ndat(12)
c
      equivalence (n,ndat)
c
      real n
c
      data end,polar,arc,arcs,point,pts,fillin,fills,connect/3hend,5hpol
     1ar,3harc,1ha,5hpoint,1hp,6hfillin,4hfill,7hconnect/
      data arcrs,bkrs,erase/5harcrs,4hbkrs,5herase/
      data region,wpoints/6hregion,6hpoints/
      data dum/-1.23e-21/
c
c         subroutine to form finite element mesh using
c           the isoparametric shape functions
c
      p(1)=1hx
      p(2)=1hy
      if (iplane.ne.0) go to 10
      p(1)=1hr
      p(2)=1hz
   10 np=0
      pi=atan(1.0)/45.
      do 20 i=1,nn
      do 20 j=1,nm
      yn(i,j)=dum
   20 xn(i,j)=dum
c
   30 np=np+1
      call ffld (64,data)
      w=data(1)
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
      if(data(5).ne.bkrs)go to 290
      w=bkrs
      data(5)=0.
      go to 290
c
c        read data for a single point
c
   50 imin=data(2)
      imax=imin
      jmin=data(3)
      jmax=jmin
      word=data(6)
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
      word=data(7)
      xo=data(8)
      yo=data(9)
      n1=0
      if (imin.eq.imax) n1=jmax-jmin+1
      if (jmin.eq.jmax) n1=imax-imin+1
      if (n1.le.0) call error (5hnmesh,imin,jmin,imax,26,26hbad i,j arc/
     1connect limits,1)
      if (g1.le.0.) g1=1.
      call seg1 (g1,cc,n1)
      if (data(1).ne.connect) go to 100
      x(3)=0.
      y(3)=0.
      if (xn(imin,jmin).ne.dum) go to 70
      x(1)=data(10)
      x(2)=xn(imax,jmax)
      y(1)=data(11)
      y(2)=yn(imax,jmax)
      if (word.ne.polar) go to 110
      x1=x(1)
      y1=y(1)
      x(1)=xo+x1*cos(pi*y1)
      y(1)=yo+x1*sin(pi*y1)
      go to 90
   70 if (xn(imax,jmax).ne.dum) go to 80
      x(1)=xn(imin,jmin)
      x(2)=data(10)
      y(1)=yn(imin,jmin)
      y(2)=data(11)
      if (word.ne.polar) go to 110
      x1=x(2)
      y1=y(2)
      x(2)=xo+x1*cos(pi*y1)
      y(2)=yo+x1*sin(pi*y1)
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
      x(1)=sqrt((x1-xo)**2+(y1-yo)**2)
      y(1)=atan2(y1-yo,x1-xo)/pi
      x(2)=sqrt((x2-xo)**2+(y2-yo)**2)
      y(2)=atan2(y2-yo,x2-xo)/pi
      go to 110
  100 call ffld (4,x)
      call ffld (4,y)
      if(w.ne.arcrs)go to 110
      call ffld(n1,cc)
      do 105 i=1,n1
      cc(i)=2.*cc(i)-1.
  105 continue
  110 n1=1
      do 160 i=imin,imax
      do 160 j=jmin,jmax
      xn(i,j)=0.
      yn(i,j)=0.
      s=cc(n1)
      if (x(3).eq.0.) go to 130
      if (x(4).eq.0.) go to 120
      n(1)=1./16.*(1.-s)*(9.*s*s-1.)
      n(2)=1./16.*(1.+s)*(9.*s*s-1.)
      n(3)=9./16.*(1.-s*s)*(1.-3.*s)
      n(4)=9./16.*(1.-s*s)*(1.+3.*s)
      iflag=4
      go to 140
  120 continue
      n(1)=.5*s*(s-1.)
      n(2)=.5*s*(s+1.)
      n(3)=1.-s*s
      iflag=3
      go to 140
  130 n(1)=.5*(1.-s)
      n(2)=.5*(1.+s)
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
      if (omega.eq.0.) omega=1.86
      iter=data(9)
      if (iter.eq.0) iter=100
      tol=data(10)
      if (tol.le.0.0) tol=1.e-4
      imin=data(2)+inc
      jmin=data(3)+inc
      imax=data(4)-inc
      jmax=data(5)-inc
      if (imin.le.0.or.imin.gt.imax.or.imax.gt.nn) call error (5hnmesh,i
     1min,imax,nn,19,19hbad fillin i bounds,1)
      if (jmin.le.0.or.jmin.gt.jmax.or.jmax.gt.nm) call error (5hnmesh,j
     1min,jmax,nm,19,19hbad fillin j bounds,1)
c
      xnorm=amax1(abs(xn(imax+inc,jmax+inc)-xn(imin-inc,jmin-inc)),abs(y
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
      a(l)=0.5*((xi(2*l)-xn(i,j))*(yi(2*l+1)-yi(2*l-1))-(yi(2*l)-yn(i,j)
     1)*(xi(2*l+1)-xi(2*l-1)))
      dx(l)=xi(2*l+1)-xn(i,j)
  190 dy(l)=yi(2*l+1)-yn(i,j)
      a(5)=a(1)
      if (it.gt.1) go to 200
      a(1)=1.
      a(2)=1.
      a(3)=1.
      a(4)=1.
      a(5)=1.
  200 dxa=0.
      dya=0.
      do 210 l=1,5
      a(l)=abs(a(l))
  210 continue
      do 220 l=1,4
      dxa=alfa*(a(l+1)-a(l))/(a(l+1)+a(l))*(-dy(l))+(1.-alfa)*dx(l)+dxa
  220 dya=alfa*(a(l+1)-a(l))/(a(l+1)+a(l))*dx(l)+(1.-alfa)*dy(l)+dya
      resid=resid+abs(dxa)+abs(dya)
      xn(i,j)=xn(i,j)+omega*dxa/4.
      yn(i,j)=yn(i,j)+omega*dya/4.
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
      delx=(xn(iinc,jinc)-xn(i,j))/inc
      dely=(yn(iinc,jinc)-yn(i,j))/inc
      incc=inc-1
      do 270 il=1,incc
      iinc1=i+il
      jinc1=j+il
      xn(iinc1,jinc1)=xn(i,j)+delx*il
      yn(iinc1,jinc1)=yn(i,j)+dely*il
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
      word=data(9)
      xo=data(10)
      yo=data(11)
      if (g1.le.0.) g1=1.
      if (g2.le.0.) g2=1.
      if (g3.le.0.) g3=1.
      if (g4.le.0.) g4=1.
      if (imin.le.0.or.imin.gt.imax.or.imax.gt.nn) call error (5hnmesh,i
     1min,imax,nn,14,14h bad i bounds ,1)
      if (jmin.le.0.or.jmin.gt.jmax.or.jmax.gt.nm) call error (5hnmesh,j
     1min,jmax,nm,14,14h bad j bounds ,1)
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
      cc(i)=2.*cc(i)-1.
      cr(i)=2.*cr(i)-1.
  292 continue
      do 294 i=1,nj
      hr(i)=2.*hr(i)-1.
      hh(i)=2.*hh(i)-1.
  294 continue
  298 continue
      do 360 i=imin,imax
      ki=i-imin+1
      do 360 j=jmin,jmax
      kj=j-jmin+1
      qx=(cr(ki)-cc(ki))*0.5
      px=(cr(ki)+cc(ki))*0.5
      qy=(hr(kj)-hh(kj))*0.5
      py=(hr(kj)+hh(kj))*0.5
      d=1.0-qx*qy
      psi=(px+qx*py)/d
      eta=(py+qy*px)/d
c
c     calculate xn and yn
c
      xn(i,j)=0.0
      yn(i,j)=0.0
      n(1)=1./4.*(1.-psi)*(1.-eta)
      n(2)=1./4.*(1.+psi)*(1.-eta)
      n(3)=1./4.*(1.+psi)*(1.+eta)
      n(4)=1./4.*(1.-psi)*(1.+eta)
      iflag=1
      do 300 kk=5,12
      ii=17-kk
      n(ii)=0.0
      if (x(ii).ne.0.0) go to 310
  300 continue
      go to 320
  310 call fill (x,y,iflag)
  320 continue
      go to (350,330,340), iflag
  330 continue
      n(1)=n(1)*(-psi-eta-1.)
      n(2)=n(2)*(psi-eta-1.)
      n(3)=n(3)*(psi+eta-1.)
      n(4)=n(4)*(-psi+eta-1.)
      n(5)=1./2.*(1.-psi**2)*(1.-eta)
      n(6)=1./2.*(1.-eta**2)*(1.+psi)
      n(7)=1./2.*(1.-psi**2)*(1.+eta)
      n(8)=1./2.*(1.-eta**2)*(1.-psi)
      go to 350
  340 continue
      n(1)=n(1)*(1./8.*(-10.+9.*(psi**2+eta**2)))
      n(2)=n(2)*(1./8.*(-10.+9.*(psi**2+eta**2)))
      n(3)=n(3)*(1./8.*(-10.+9.*(psi**2+eta**2)))
      n(4)=n(4)*(1./8.*(-10.+9.*(psi**2+eta**2)))
      n(5)=9./32.*(1.-psi**2)*(1.-eta)*(1.-3.*psi)
      n(6)=9./32.*(1.-eta**2)*(1.+psi)*(1.-3.*eta)
      n(7)=9./32.*(1.-psi**2)*(1.+eta)*(1.+3.*psi)
      n(8)=9./32.*(1.-eta**2)*(1.-psi)*(1.+3.*eta)
      n(9)=9./32.*(1.-psi**2)*(1.-eta)*(1.+3.*psi)
      n(10)=9./32.*(1.-eta**2)*(1.+psi)*(1.+3.*eta)
      n(11)=9./32.*(1.-psi**2)*(1.+eta)*(1.-3.*psi)
      n(12)=9./32.*(1.-eta**2)*(1.-psi)*(1.-3.*eta)
  350 continue
      do 360 ii=1,12
      xn(i,j)=xn(i,j)+n(ii)*x(ii)
  360 yn(i,j)=yn(i,j)+n(ii)*y(ii)
  370 if (word.ne.polar) go to 390
      do 380 i=imin,imax
      do 380 j=jmin,jmax
      r=xn(i,j)
      ang=yn(i,j)*pi
      xn(i,j)=xo+r*cos(ang)
  380 yn(i,j)=yo+r*sin(ang)
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
      call print (xn,yn,imin,jmin,imax,jmax,nn,nm,p)
      go to 30
  430 np=np-1
      if (np.le.0) call error (5hnmesh,0,0,0,12,12hno mesh data,1)
      imin=nn
      jmin=nm
      imax=0
      jmax=0
      rmax=1.0/dum
      rmin=-rmax
      zmin=rmin
      zmax=rmax
      do 440 i=1,nn
      do 440 j=1,nm
      if (xn(i,j).eq.dum) go to 440
      imin=min0(imin,i)
      jmin=min0(jmin,j)
      imax=max0(imax,i)
      jmax=max0(jmax,j)
      rmin=amin1(rmin,xn(i,j))
      zmin=amin1(zmin,yn(i,j))
      rmax=amax1(rmax,xn(i,j))
      zmax=amax1(zmax,yn(i,j))
  440 continue
      nw=nn*(jmax-jmin+1)
      irow=nn
      write (nout,530) imin,jmin,imax,jmax,p(1),rmin,p(2),zmin,p(1),rmax
     1,p(2),zmax
      if (data(2).ne.wpoints) go to 450
  450 continue
      if (iprint.lt.3) return
      write (nout,520) np
      call print (xn,yn,imin,jmin,imax,jmax,nn,nm,p)
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
     .   ,i3//25h  fillin parameters  inc=,i2,9h  alpha=,1pe11.3,
     .  8h  omega=,
     .1pe11.3,7h  iter=,i4,6h  tol=,1pe11.3,8h  xnorm=,1pe11.3)
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
      if (x(8).eq.0.0) x(8)=(x(1)+x(4))/2.
      if (y(8).eq.0.0) y(8)=(y(1)+y(4))/2.
      if (x(7).eq.0.0) x(7)=(x(4)+x(3))/2.
      if (y(7).eq.0.0) y(7)=(y(4)+y(3))/2.
      if (x(6).eq.0.0) x(6)=(x(3)+x(2))/2.
      if (y(6).eq.0.0) y(6)=(y(3)+y(2))/2.
      if (x(5).eq.0.0) x(5)=(x(2)+x(1))/2.
      if (y(5).eq.0.0) y(5)=(y(2)+y(1))/2.
      iflag=2
      return
   20 continue
      if (x(5).eq.0.0) x(5)=x(1)+(x(2)-x(1))/3.
      if (y(5).eq.0.0) y(5)=y(1)+(y(2)-y(1))/3.
      if (x(6).eq.0.0) x(6)=x(2)+(x(3)-x(2))/3.
      if (y(6).eq.0.0) y(6)=y(2)+(y(3)-y(2))/3.
      if (x(7).eq.0.0) x(7)=x(3)+(x(4)-x(3))/3.
      if (y(7).eq.0.0) y(7)=y(3)+(y(4)-y(3))/3.
      if (x(8).eq.0.0) x(8)=x(4)+(x(1)-x(4))/3.
      if (y(8).eq.0.0) y(8)=y(4)+(y(1)-y(4))/3.
      if (y(9).eq.0.0) y(9)=y(1)+2.*(y(2)-y(1))/3.
      if (x(9).eq.0.0) x(9)=x(1)+2.*(x(2)-x(1))/3.
      if (x(10).eq.0.0) x(10)=x(2)+2.*(x(3)-x(2))/3.
      if (y(10).eq.0.0) y(10)=y(2)+2.*(y(3)-y(2))/3.
      if (x(11).eq.0.0) x(11)=x(3)+2.*(x(4)-x(3))/3.
      if (y(11).eq.0.0) y(11)=y(3)+2.*(y(4)-y(3))/3.
      if (x(12).eq.0.0) x(12)=x(4)+2.*(x(1)-x(4))/3.
      if (y(12).eq.0.0) y(12)=y(4)+2.*(y(1)-y(4))/3.
      iflag=3
      return
      end
 
