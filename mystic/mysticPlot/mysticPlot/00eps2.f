 125 do 300 lx=1,nint                                                          
      e1=xg(lx,nint)                                                            
      do 300 ly=1,nint                                                          
      e2=xg(ly,nint)                                                            
      wt=wgt(lx,nint)*wgt(ly,nint)                                              
      ipt=(lx-1)*nint + ly                                                      
      if (indnl.eq.3) go to 200                                                 
c                                                                               
c                                                                               
c     t o t a l   l a g r a n g i a n   f o r m u l a t i o n                   
c                                                                               
c                                                                               
c     evaluate the derivative operator b                                        
c                                                                               
      call deriq (nel,yz,b,det,e1,e2,xbar,nod5)                                 
c                                                                               
c     calculate displacement derivatives                                        
c                                                                               
      do 130 i=1,5                                                              
  130 disd(i)=0.0                                                               
      do 140 j=2,nd,2                                                           
      i=j - 1                                                                   
      disd(1)=disd(1) + b(1,i)*edis(i)                                          
      disd(2)=disd(2) + b(2,j)*edis(j)                                          
      disd(3)=disd(3) + b(3,i)*edis(i)                                          
  140 disd(4)=disd(4) + b(3,j)*edis(j)                                          
      if (ist.eq.3) go to 160                                                   
      do 150 i=1,nd,2                                                           
  150 disd(5)=disd(5) + b(4,i)*edis(i)                                          
c  

      subroutine deriq (nel,xx,b,det,r,s,x1bar,nod5)                            
c                                                                               
c                                                                               
c     evaluation of the strain-displacement matrix at point (r,s) for           
c     a quadrilateral element, axisymmetric geometry                            
c                                                                               
c                                                                               
      common /el/ ind,icount,npar(20),numeg,negl,negnl,imass,idamp,istat        
     1           ,ndof,klin,ieig,imassn,idampn                                  
      common /todim/ bet,thic,de,iel,nnd5                                       
      dimension     xx(2,1),b(4,1),nod5(1)                                      
      dimension     h(8),         p(2,8),xj(2,2),xji(2,2)                       
c                                                                               
      equivalence (npar(5),ityp2d)                                              
c                                                                               
c                                                                               
c     find interpolation functions and jacobian                                 
c                                                                               
      call funct2 (r,s,h,p,nod5,xj,det,xx,nel)                                  
c                                                                               
c                                                                               
c     compute inverse of the jacobian matrix                                    
c                                                                               
      dum = 1.0/det                                                             
      xji(1,1) = xj(2,2)* dum                                                   
      xji(1,2) =-xj(1,2)* dum                                                   
      xji(2,1) =-xj(2,1)* dum                                                   
      xji(2,2) = xj(1,1)* dum                                                   
c                                                                               
c     evaluate global derivative operator ( b-matrix )                          
c                                                                               
      do 130 k=1,iel                                                            
      k2=k*2                                                                    
      b(1,k2-1) = 0.                                                            
      b(1,k2  ) = 0.                                                            
      b(2,k2-1) = 0.                                                            
      b(2,k2  ) = 0.                                                            
      do 120 i=1,2                                                              
      b(1,k2-1) = b(1,k2-1) + xji(1,i) * p(i,k)                                 
  120 b(2,k2  ) = b(2,k2  ) + xji(2,i) * p(i,k)                                 
      b(3,k2  ) = b(1,k2-1)                                                     
  130 b(3,k2-1) = b(2,k2  )                                                     
c                                                                               
c     in case of plane strain or plane stress analysis we do not include        
c     the normal strain component                                               
c                                                                               
      if (ityp2d.gt.0) return                                                   
c                                                                               
c     compute the radius at point (r,s)                                         
c                                                                               
      x1bar = 0.0                                                               
      do 50 k=1,iel                                                             
   50 x1bar = x1bar + h(k)* xx(1,k)                                             
c                                                                               
c     evaluate the hoop strain-displacement relation                            
c                                                                               
      if(x1bar.gt..00000001) go to 150                                          
c                                                                               
c     for the case of zero radius equate radial to hoop strain                  
c                                                                               
      nd=2*iel                                                                  
      do 140 k=1,nd                                                             
  140 b(4,k)=b(1,k)                                                             
      return                                                                    
c                                                                               
c     non-zero radius                                                           
c                                                                               
  150 dum = 1.0/x1bar                                                           
      do 160 k=1,iel                                                            
      k2=k*2                                                                    
      b(4,k2  ) = 0.                                                            
  160 b(4,k2-1) = h(k) * dum                                                    
c                                                                               
      return                                         
                                 
c                                                                               
c                                                                               
      end            
      
      *deck funct2                                                                    
c *uni* )for,is  n.funct2, r.funct2                                             
      subroutine funct2 (r,s,h,p,nod5,xj,det,xx,nel)                            
c                                                                               
c                                                                               
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .         
c .                                                                   .         
c .   p r o g r a m                                                   .         
c .                                                                   .         
c .      to find interpolation functions ( h )                        .         
c .      and derivatives ( p ) corresponding to the nodal points      .         
c .      of a 4- to 8-node isoparametric quadrilateral                .         
c .                                                                   .         
c .      to find jacobian ( xj ) and its determinant ( det )          .         
c                                                                               
c .                                                                   .         
c .                    node numbering convention                      .         
c                                                                               
c .                                                                   .         
c .               2                 5                 1               .         
c .                                                                   .         
c .                 o . . . . . . . o . . . . . . . o                 .         
c .                 .                               .                 .         
c .                 .                               .                 .         
c .                 .               s               .                 .         
c .                 .               .               .                 .         
c .                 .               .               .                 .         
c .               6 o               . . . r         o 8               .         
c .                 .                               .                 .         
c .                 .                               .                 .         
c .                 .                               .                 .         
c .                 .                               .                 .         
c .                 .                               .                 .         
c .                 o . . . . . . . o . . . . . . . o                 .         
c .                                                                   .         
c .               3                 7                 4               .         
c .                                                                   .         
c .                                                                   .         
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .         
c                                                                               
c                                                                               
      common /todim/ bet,thic,de,iel,nnd5                                       
      dimension h(*),p(2,*),nod5(*),iperm(4),xj(2,2),xx(2,*)                    
      data iperm/2,3,4,1/                                                       
c                                                                               
      rp = 1.0 + r                                                              
      sp = 1.0 + s                                                              
      rm = 1.0 - r                                                              
      sm = 1.0 - s                                                              
      r2 = 1.0 - r*r                                                            
      s2 = 1.0 - s*s                                                            
c                                                                               
c                                                                               
c     interpolation functions and their derivatives                             
c                                                                               
c     4-node element                                                            
c                                                                               
      h(1) = 0.25* rp* sp                                                       
      h(2) = 0.25* rm* sp                                                       
      h(3) = 0.25* rm* sm                                                       
      h(4) = 0.25* rp* sm                                                       
      p(1,1)=0.25*sp                                                            
      p(1,2)=-p(1,1)                                                            
      p(1,3)=-0.25*sm                                                           
      p(1,4)=-p(1,3)                                                            
      p(2,1)=0.25*rp                                                            
      p(2,2)=0.25*rm                                                            
      p(2,3)=-p(2,2)                                                            
      p(2,4)=-p(2,1)                                                            
c                                                                               
      if (iel.eq.4) go to 50                                                    
c                                                                               
c     add degrees of freedom in excess of 4                                     
c                                                                               
      i=0                                                                       
    2 i=i + 1                                                                   
      if (i.gt.nnd5) go to 40                                                   
      nn=nod5(i) - 4                                                            
      go to (5,6,7,8), nn                                                       
c                                                                               
    5 h(5) = 0.50* r2* sp                                                       
      p(1,5)=-r*sp                                                              
      p(2,5)=0.50*r2                                                            
      go to 2                                                                   
    6 h(6) = 0.50* rm* s2                                                       
      p(1,6)=-0.50*s2                                                           
      p(2,6)=-rm*s                                                              
      go to 2                                                                   
    7 h(7) = 0.50* r2* sm                                                       
      p(1,7)=-r*sm                                                              
      p(2,7)=-0.50*r2                                                           
      go to 2                                                                   
    8 h(8) = 0.50* rp* s2                                                       
      p(1,8)=0.50*s2                                                            
      p(2,8)=-rp*s                                                              
      go to 2                                                                   
c                                                                               
c     correct functions and derivatives if 5 or more nodes are                  
c     used to describe the element                                              
c                                                                               
   40 ih=0                                                                      
   41 ih=ih + 1                                                                 
      if (ih.gt.nnd5) go to 50                                                  
      in=nod5(ih)                                                               
      i1=in - 4                                                                 
      i2=iperm(i1)                                                              
      h(i1)=h(i1) - 0.5*h(in)                                                   
      h(i2)=h(i2) - 0.5*h(in)                                                   
      h(ih + 4)=h(in)                                                           
      do 45 j=1,2                                                               
      p(j,i1)=p(j,i1) - 0.5*p(j,in)                                             
      p(j,i2)=p(j,i2) - 0.5*p(j,in)                                             
   45 p(j,ih + 4)=p(j,in)                                                       
      go to 41                                                                  
c                                                                               
c     evaluate the jacobian matrix at point (r,s)                               
c                                                                               
   50 do 100 i=1,2                                                              
      do 100 j=1,2                                                              
      dum = 0.0                                                                 
      do 90 k=1,iel                                                             
   90 dum = dum + p(i,k)* xx(j,k)                                               
  100 xj(i,j) = dum                                                             
c                                                                               
c     compute the determinant of the jacobian matrix at point (r,s)             
c                                                                               
      det = xj(1,1)* xj(2,2) - xj(2,1)* xj(1,2)                                 
      dum=det                                                                   
      if(dum.gt..00000001) go to 110                                            
      write(6,2000) nel                                                         
      stop                                                                      
  110 continue                                                                  
c                                                                               
      return                                                                    
c                                                                               
c                                                                               
 2000 format(10h0*** error,                                                     
     +      40h zero jacobian determinant for element (,i4,1h)  )               
c                                                                               
      end                                                                       
                          