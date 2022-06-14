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

int deriq (int nel,double *xx,double *b,double *det,double *r,double *s,double *x1bar);

int funct2 (double r,double s,double *h,double *p,double *xj,double *det,double *xx,int nel);
                            
int deriq (int nel,double *xx,double *b,double *det,double *r,double *s,double *x1bar)                            
{      
      funct2 (r,s,h,p,xj,det,xx,nel);                                  

      double dum = 1.0/(*det);
                                                                   
#define xji(i,j) xji[i-1+(j-1)*2]
#define xj(i,j) xj[i-1+(j-1)*2]

      double dum = 1.0/det 
                                                                  
      xji(1,1) = xj(2,2)* dum;                                                   
      xji(1,2) =-xj(1,2)* dum;                                                   
      xji(2,1) =-xj(2,1)* dum;                                                   
      xji(2,2) = xj(1,1)* dum;   
      
#define b(i,j) b[i-1+(j-1)*2]
#define p(i,j) p[i-1+(j-1)*2]
                                                      
	for(int k=1;k<=4;++k){                                                            
		  int k2=k*2;                                                                    
		  b(1,k2-1) = 0.;                                                            
		  b(1,k2  ) = 0.;                                                            
		  b(2,k2-1) = 0.;                                                            
		  b(2,k2  ) = 0.;                                                            
		  for(int i=1;i<=2;++i){                                                             
			 b(1,k2-1) = b(1,k2-1) + xji(1,i) * p(i,k);                                 
			 b(2,k2  ) = b(2,k2  ) + xji(2,i) * p(i,k);
		  }                                 
		  b(3,k2  ) = b(1,k2-1)                                                     
		  b(3,k2-1) = b(2,k2  )  
	}                                                   


      if (ityp2d > 0) return 1;                                                  
 
#define xx(i,j) xx[i-1+(j-1)*2]
 
      double x1bar = 0.0;                                                              
 	 for(int k=1;k<=4;++k){                                                                                                                         
         x1bar = x1bar + h[k-1]* xx(1,k)
     }                                             

      if(x1bar < 0.00000001){                                         
		  int nd=2*4                                                                  
		  for(int k=1;k<=nd;++k){                                                             
			  b(4,k)=b(1,k);      
		  }                                                       
		  return 0
      }                                                                   

      dum = 1.0/x1bar                                                           
      for(int k=1;k<=4;++k){                                                            
		  k2=k*2;                                                                    
		  b(4,k2  ) = 0.;                                                            
		  b(4,k2-1) = h[k-1] * dum; 
      }    
      
#undef xx                                            
#undef b                                            
#undef p                                            
#undef xji                                            
#undef xj                                            
    return 0;                                         
}
int funct2 (double r,double s,double *h,double *p,double *xj,double *det,double *xx,int nel) 
{                                                                                          
      double *rp = 1.0 + r;                                                              
      double *sp = 1.0 + s;                                                              
      double *rm = 1.0 - r;                                                              
      double *sm = 1.0 - s;                                                              
      h[0] = 0.25* rp* sp;                                                      
      h[1] = 0.25* rm* sp;                                                       
      h[2] = 0.25* rm* sm;                                                       
      h[3] = 0.25* rp* sm;                                                      
      p[0  ]=0.25*s;                                                            
      p[0+1*2]=-p[0  ];                                                            
      p[0+2*2]=-0.25*sm;                                                          
      p[0+3*2]=-p[0+2*2];                                                           
      p[1  ]=0.25*rp;                                                            
      p[1+1*2]=0.25*rm;                                                           
      p[1+2*2]=-p[1+1*2];                                                            
      p[1+3*2]=-p[1  ];                                                           
                                                                               
      for(int i=0;i<2;++i){ 
		  for(int j=0;j<2;++j){                                                                
			  double dum = 0.0;
			  for(int k=0;k<4;++k){                                                                                                                         
					   dum = dum + p[i+k*2]* xx[j+k*2]; 
			  }                                             
			  xj[i+j*2] = dum;  
		  }     
      }                                                      
      *det = xj[0]* xj[3] - xj[2]* xj[1];                                 
      double dum=(*det);                                                                   
      if(dum < 0.00000001){                                            
          write(6,2000) nel                                                         
      }
	return 0;                                                          
}