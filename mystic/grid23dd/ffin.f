      subroutine ffin
      common/tapes/ntape5,ntape6,ntape1,ntape2,ntape3,ntape4,ntape52
      common/flist/fdata(256),nflag(256),nword
      common/clist/cdata(256)
      character*16 edum
      character*8 cdata,wlast
      character*1 sum(80),x(13),stemp(16),blank,comma,dollar,slash,save
      data x/'1','2','3','4','5','6','7','8','9','0','.','+','-'/
      data blank,comma,dollar,slash/' ', ',', '$', '/'/
      data iswap/0/
      data wlast/'last'/
      iline=1
   90 continue
      iline=iline+1
      do 100 j=1,64
      cdata(j)=blank
      nflag(j)=0
  100 fdata(j)=0.0
      nword=0
  120 continue
      read(ntape5,3000,end=350)sum
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
      write(edum,3020)stemp
      read(edum,3040)fdata(nword)
      go to 340
  320 continue
      write(cdata(nword),3050)(stemp(i),i=1,8)
  340 continue
      if(sum(ichar).eq.slash) go to 120
      if(sum(ichar).eq.dollar)go to 360
      go to 130
  350 continue
      nword=nword+1
  360 continue
      cdata(nword)=wlast
      return
 3000 format(80a1)
 3010 format(i5,5x,80a1)
 3020 format(16a1)
 3030 format(4a1)
 3040 format(e16.0)
 3050 format(8a1)
      end
