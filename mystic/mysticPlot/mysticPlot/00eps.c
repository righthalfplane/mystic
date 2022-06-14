

      double e1=0;                                                                        
      double e2=0;                                                                        
      double e3=0;
      double det;  
                                                                            
      deriq3 (n,xxx,b,&det,e1,e2,e3)                              
  
      for(int j=0;j<9;++j){
          disd(j)=0.0;
      }
      
      for(j=2;j<24;j += 3){                                                     
		  i=j-1;                                                                     
		  k=j-2;                                                                     
		  disd[0]=disd[0]+b(k)*edis(k);                                              
		  disd[1]=disd[1]+b(i)*edis(i);                                              
		  disd[2]=disd[2]+b(j)*edis(j);                                              
		  disd[3]=disd[3]+b(i)*edis(k);                                              
		  disd[4]=disd[4]+b(j)*edis(k);                                              
		  disd[5]=disd[5]+b(k)*edis(i);                                              
		  disd[6]=disd[6]+b(j)*edis(i);                                              
		  disd[7]=disd[7]+b(k)*edis(j);                                              
		  disd[8]=disd[8]+b(i)*edis(j);
      }                                              


int deriq3(int nel,double *xx,double *b,double *det,double r,double s,double t)
{                              
                                                
      double h[21],p[3*21],xj[3*3],xji[3*3];                                
                                                                            
      funct(r,s,t,h,p,xj,det,xx)                                     

      double dum=1.0/*det;                                                               
      xji[0  ]=dum*(   xj[1+1*3]*xj[2+2*3] - xj[1,3)*  xj[2+1*3]);                         
      xji[1  ]=dum*(  -xj[1  ]*  xj[2+2*3] + xj[1,3)*  xj[2  ]);                         
      xji[2  ]=dum*(   xj[1  ]*  xj[2+1*3] - xj[1+1*3]*xj[2  ]);                         
      xji[0+1*3]=dum*(-xj[0+1*3]*xj[2+2*3] + xj[0,3)*  xj[2+1*3]);                         
      xji[1+1*3]=dum*( xj[0  ]*  xj[2+2*3] - xj[0,3)*  xj[2  ]);                         
      xji[2+1*3]=dum*(-xj[0  ]*  xj[2+1*3] + xj[0+1*3]*xj[2  ]);                         
      xji[0+2*3]=dum*( xj[0+1*3]*xj[1+2*3] - xj[0,3)*  xj[1+1*3]);                         
      xji[1+2*3]=dum*(-xj[0  ]*  xj[1+2*3] + xj[0,3)*  xj[1  ]);                         
      xji[2+2*3]=dum*( xj[0  ]*  xj[1+1*3] - xj[0+1*3]*xj[1  ]);                         

      for(int k=0;k<8;++k){                                                           
          int k2=k*3;
          for(int i=0;i<3;++){                                                                                                                       
             b(k2-i)=0.0;
          }                                                             
          for(int i=0;i<3;++){                                                                                                                       
      		b[k2-2]=b[k2-2]   + xji[0+3*i]*p[i+3*k];                                         
      		b[k2-1]=b[k2-1]   + xji[1+3*i]*p[i+3*k];                                         
   			b[k2] = b[k2]     + xji[2+3*i]*p[i+3*k];
          }
      }                                             
   
  
      return 0;
  
}                                                                                                                                                
int funct(double r,double s,double t,double *h,double *p,double *xj,double *det,double *xx)
{                                                                            
      double rp=1.0 + r;                                                                
      double sp=1.0 + s;                                                                
      double tp=1.0 + t;                                                                
      double rm=1.0 - r;                                                               
      double sm=1.0 - s;                                                              
      double tm=1.0 - t;                                                             
      double rr=1.0 - r*r;                                                            
      double ss=1.0 - s*s;                                                            
      double tt=1.0 - t*t; 
                                                                 
      h[0]=0.125*rp*sp*tp;                                                       
      h[1]=0.125*rm*sp*tp;                                                       
      h[2]=0.125*rm*sm*tp;                                                       
      h[3]=0.125*rp*sm*tp;                                                       
      h[4]=0.125*rp*sp*tm;                                                       
      h[5]=0.125*rm*sp*tm;                                                       
      h[6]=0.125*rm*sm*tm;                                                       
      h[7]=0.125*rp*sm*tm;                                                       
                                                                               
      p[0]= 0.125*sp*tp;                                                       
      p[0+1*3]=-p[0];                                                            
      p[0+2*3]=-0.125*sm*tp;                                                       
      p[0+3*3]=-p[0+2*3];                                                            
      p[0+4*3]= 0.125*sp*tm;                                                       
      p[0+5*3]=-p[0+4*3];                                                            
      p[0+6*3]=-0.125*sm*tm;                                                       
      p[0+7*3]=-p[0+6*3];                                                            
                                                                               
      p[1]= 0.125*rp*tp;                                                       
      p[1+1*3]= 0.125*rm*tp;                                                       
      p[1+2*3]=-p[1+1*3];                                                            
      p[1+3*3]=-p[1];                                                            
      p[1+4*3]= 0.125*rp*tm;                                                       
      p[1+5*3]= 0.125*rm*tm;                                                       
      p[1+6*3]=-p[1+5*3];                                                            
      p[1+7*3]=-p[1+4*3];                                                            
                                                                               
      p[2]= 0.125*rp*sp;                                                       
      p[2+1*3]= 0.125*rm*sp;                                                       
      p[2+2*3]= 0.125*rm*sm;                                                       
      p[2+3*3]= 0.125*rp*sm;                                                       
      p[2+4*3]=-p[2];                                                            
      p[2+5*3]=-p[2+1*3];                                                            
      p[2+6*3]=-p[2+2*3];                                                            
      p[2+7*3]=-p[2+3*3];                                                            
                                                                               
      for(int i=0;i<3;++i){  
      	for(int j=0;j<3;++j){                                                         												   
		  double dum=0.0;
		  for(int k=0;k<8;++k){                                                                  
			  dum=dum + p[i+3*k]*xx[j+3*k];
		  }                                                  
          xj[i+3*j]=dum;  
		  }
      }                                                             
      *det =     xj[0]*xj[1+1*3]*xj[2+2*3]                                            
           + xj[0+1*3]*xj[1+2*3]*xj[2]                                             
           + xj[0+2*3]*xj[1]*xj[2+1*3]                                             
           - xj[0+2*3]*xj[1+1*3]*xj[2]                                             
           - xj[0+1*3]*xj[1])*xj[2+2*3]                                            
           - xj[0]*xj[1+2*3]*xj[2+1*3];
                                                      
      dum=det;                                                                   
      if (dum.lt.1.0e-08){                                           
         fprintf(stderr,"det = %g\n",det);
         return 1;                                                            
      }                                                               
      return 0;                                                                   
}