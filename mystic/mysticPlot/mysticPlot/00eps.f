  831 ipt=0                                                                     
      do 939 lx=1,nint                                                          
      e1=xg(lx,nint)                                                            
      do 939 ly=1,nint                                                          
      e2=xg(ly,nint)                                                            
      do 939 lz=1,nintz                                                         
      e3=xg(lz,nintz)                                                           
      ipt=ipt+1                                                                 
c                                                                               
      call deriq3 (n,xxx,b,det,e1,e2,e3,nod9(1,n))                              
c                                                                               
      do 910 j=1,9                                                              
  910 disd(j)=0.0                                                               
      do 915 j=3,nd,3                                                           
      i=j-1                                                                     
      k=j-2                                                                     
      disd(1)=disd(1)+b(k)*edis(k)                                              
      disd(2)=disd(2)+b(i)*edis(i)                                              
      disd(3)=disd(3)+b(j)*edis(j)                                              
      disd(4)=disd(4)+b(i)*edis(k)                                              
      disd(5)=disd(5)+b(j)*edis(k)                                              
      disd(6)=disd(6)+b(k)*edis(i)                                              
      disd(7)=disd(7)+b(j)*edis(i)                                              
      disd(8)=disd(8)+b(k)*edis(j)                                              
  915 disd(9)=disd(9)+b(i)*edis(j)                                              
c                                                                               
      call stst3n (disd)                                                        
c                                                                               
c                                                                               
c     transform piola-kirchhoff stresses to cauchy stresses                     
c                                                                               
c        cs = (1./det(f)) * ( f * pk * f(transposed) )                          
c                                                                               
      if (klin.eq.0) go to 930                                                  
      if (indnl.ne.2) go to 930                                                 
c                                                                               
      call cauch3                                                               
c                                                                               
  930 write(6,2040) ipt,stress                                                  
  939 continue                                                                  
  840 continue                                                                  


      subroutine deriq3  (nel,xx,b,det,r,s,t,nod9)                              
c                                                                               
c                                                                               
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .         
c .                                                                   .         
c .   p r o g r a m                                                   .         
c .                                                                   .         
c .      evaluates strain-displacement matrix b at point (r,s,t)      .         
c .                                                                   .         
c .      curvilinear hexahedron   8 to 21 nodes                       .         
c .                                                                   .         
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .         
c                                                                               
c                                                                               
c                                                                               
      common /el/ ind,icount,npar(20),numeg,negl,negnl,imass,idamp,istat        
     1            ,ndofdm,klin,ieig,imassn,idampn                               
      common /threed/ de,ield,ielx,iel,npt,idw,nnd9                             
      dimension xx(3,1),b(1),nod9(1)                                            
      dimension h(21),p(3,21),xj(3,3),xji(3,3)                                  
c                                                                               
c                                                                               
c     find interpolation functions and their derivatives                        
c     evaluate jacobian matrix at point (r,s,t)                                 
c     compute determinant of jacobian matrix at point (r,s,t)                   
c                                                                               
c                                                                               
      call funct (r,s,t,h,p,nod9,xj,det,xx)                                     
c                                                                               
c                                                                               
c     compute inverse of jacobian matrix                                        
c                                                                               
c                                                                               
      dum=1.0/det                                                               
      xji(1,1)=dum*( xj(2,2)*xj(3,3) - xj(2,3)*xj(3,2))                         
      xji(2,1)=dum*(-xj(2,1)*xj(3,3) + xj(2,3)*xj(3,1))                         
      xji(3,1)=dum*( xj(2,1)*xj(3,2) - xj(2,2)*xj(3,1))                         
      xji(1,2)=dum*(-xj(1,2)*xj(3,3) + xj(1,3)*xj(3,2))                         
      xji(2,2)=dum*( xj(1,1)*xj(3,3) - xj(1,3)*xj(3,1))                         
      xji(3,2)=dum*(-xj(1,1)*xj(3,2) + xj(1,2)*xj(3,1))                         
      xji(1,3)=dum*( xj(1,2)*xj(2,3) - xj(1,3)*xj(2,2))                         
      xji(2,3)=dum*(-xj(1,1)*xj(2,3) + xj(1,3)*xj(2,1))                         
      xji(3,3)=dum*( xj(1,1)*xj(2,2) - xj(1,2)*xj(2,1))                         
c                                                                               
c                                                                               
c     evaluate b matrix in global (x,y,z) coordinates                           
c                                                                               
c                                                                               
      do 130 k=1,iel                                                            
      k2=k*3                                                                    
      do 125 i=1,3                                                              
  125 b(k2+1-i)=0.0                                                             
      do 120 i=1,3                                                              
      b(k2-2)=b(k2-2) + xji(1,i)*p(i,k)                                         
      b(k2-1)=b(k2-1) + xji(2,i)*p(i,k)                                         
  120 b(k2)=b(k2) + xji(3,i)*p(i,k)                                             
  130 continue                                                                  
c                                                                               
c                                                                               
      return                                                                    
c                                                                               
      end                                                                       
                                                                                
*deck funct                                                                     
c *uni* )for,is  n.funct,  r.funct                                              
      subroutine funct (r,s,t,h,p,nod9,xj,det,xx)                               
c                                                                               
c                                                                               
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .         
c .                                                                   .         
c .   p r o g r a m                                                   .         
c .                                                                   .         
c .      to find interpolation functions ( h )                        .         
c .      and derivatives ( p ) corresponding to the nodal             .         
c .      points of a curvilinear isoparametric hexahedron             .         
c .      or subparametric hexahedron (8 to 21 nodes)                  .         
c .                                                                   .         
c .      to find jacobian ( xj ) and its determinant ( det )          .         
c .                                                                   .         
c . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .         
c                                                                               
c                                                                               
      common /threed/ de,ield,ielx,iel,npt,idw,nnd9                             
      dimension h(*),p(3,*),nod9(*),iperm(8),xj(3,3),xx(3,*)                    
c                                                                               
      data iperm / 2,3,4,1,6,7,8,5 /                                            
c                                                                               
      rp=1.0 + r                                                                
      sp=1.0 + s                                                                
      tp=1.0 + t                                                                
      rm=1.0 - r                                                                
      sm=1.0 - s                                                                
      tm=1.0 - t                                                                
      rr=1.0 - r*r                                                              
      ss=1.0 - s*s                                                              
      tt=1.0 - t*t                                                              
c                                                                               
c                                                                               
c     interpolation functions and their derivatives                             
c                                                                               
c                                                                               
c     8-node brick                                                              
c                                                                               
      h(1)=0.125*rp*sp*tp                                                       
      h(2)=0.125*rm*sp*tp                                                       
      h(3)=0.125*rm*sm*tp                                                       
      h(4)=0.125*rp*sm*tp                                                       
      h(5)=0.125*rp*sp*tm                                                       
      h(6)=0.125*rm*sp*tm                                                       
      h(7)=0.125*rm*sm*tm                                                       
      h(8)=0.125*rp*sm*tm                                                       
c                                                                               
      p(1,1)= 0.125*sp*tp                                                       
      p(1,2)=-p(1,1)                                                            
      p(1,3)=-0.125*sm*tp                                                       
      p(1,4)=-p(1,3)                                                            
      p(1,5)= 0.125*sp*tm                                                       
      p(1,6)=-p(1,5)                                                            
      p(1,7)=-0.125*sm*tm                                                       
      p(1,8)=-p(1,7)                                                            
c                                                                               
      p(2,1)= 0.125*rp*tp                                                       
      p(2,2)= 0.125*rm*tp                                                       
      p(2,3)=-p(2,2)                                                            
      p(2,4)=-p(2,1)                                                            
      p(2,5)= 0.125*rp*tm                                                       
      p(2,6)= 0.125*rm*tm                                                       
      p(2,7)=-p(2,6)                                                            
      p(2,8)=-p(2,5)                                                            
c                                                                               
      p(3,1)= 0.125*rp*sp                                                       
      p(3,2)= 0.125*rm*sp                                                       
      p(3,3)= 0.125*rm*sm                                                       
      p(3,4)= 0.125*rp*sm                                                       
      p(3,5)=-p(3,1)                                                            
      p(3,6)=-p(3,2)                                                            
      p(3,7)=-p(3,3)                                                            
      p(3,8)=-p(3,4)                                                            
c                                                                               
                                                                   
   50 if (ielx.lt.ield) return                                                  
   51 do 100 i=1,3                                                              
      do 100 j=1,3                                                              
      dum=0.0                                                                   
      do 90 k=1,ielx                                                            
   90 dum=dum + p(i,k)*xx(j,k)                                                  
  100 xj(i,j)=dum                                                               
c                                                                               
c                                                                               
c     compute determinant of jacobian matrix at point (r,s,t)                   
c                                                                               
c                                                                               
      det = xj(1,1)*xj(2,2)*xj(3,3)                                             
     1    + xj(1,2)*xj(2,3)*xj(3,1)                                             
     2    + xj(1,3)*xj(2,1)*xj(3,2)                                             
     3    - xj(1,3)*xj(2,2)*xj(3,1)                                             
     4    - xj(1,2)*xj(2,1)*xj(3,3)                                             
     5    - xj(1,1)*xj(2,3)*xj(3,2)                                             
      dum=det                                                                   
      if (dum.gt.1.0e-08) go to 110                                             
      write(6,2000)                                                             
      stop                                                                      
  110 if (ielx.lt.ield) go to 42                                                
c                                                                               
c                                                                               
      return                                                                    
c                                                                               
c                                                                               
c                                                                               
 2000 format (40h stop  -  negative or zero determinant       )                 
c                                                                               
c                                                                               
      end          