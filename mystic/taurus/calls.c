     if(kedump.eq.1)call esbgn()
  
        if(kedump.eq.1)call eselm(comp,ipt(i+k),ipt(i),kk)
         if(kedump.eq.1)call eselm(vari,ipt(i+k),ipt(i),k)

      if(kedump.eq.1)then
        call esnod(x,neq)
        call esdis(u,neq)
        call esend()
      endif




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

      subroutine saveit(x,u,scal,ip,ished,neq)
      dimension x(3,*),u(3,*),scal(*),ip(*)
      character*8 ished(8)
      common/cedump/kedump,kcdump
      common/data0/ np,nj,npt,iscal,nfile,icode,idis,rad,ic
      common/data1/ npl(5,523),da(3,523),dd(3,3,523),dif(523),
     1              icol(523),nfr(523),npls(523),rorg(3,523),
     2              xx(3,523)
      character*8 file1
      common/t51com/file1
      data npic/1/
c     save npic
      if(kcdump.eq.0)return
      call mfopen3(npic)
      npic=npic+1
      call bgnplt2(file1)
      call node3dc(ished,x,nj,1)
      call node3dd(ished,u,nj,2)
      call node3dv(ished,scal,nj,1)
      do 10 i=1,np
      ncount=1+(npl(2,i)-npl(1,i))/4
      itype=npl(4,i)
      imat=npl(3,i)
      call node3de(ished,ip(npl(1,i)),ncount,imat,itype)
   10 continue
      call endplt2()
      return
      end
