! gfortran -fno-second-underscore -ffloat-store -o fWriteSdsForShow fWriteSdsForShow.f90 -ldf -ljpeg -lz
! f90 -YEXT_NAMES=LCS -YEXT_SFX='_'  -o fWriteSdsForShow fWriteSdsForShow.f90 -L/usr/local/hdf/lib -ldf -ljpeg -lz
! xlf95 -qsuffix=f=f90 -qextname -o fWriteSdsForShow fWriteSdsForShow.f90 -L/usr/local/hdf/lib -ldf -ljpeg -lz -L/usr/lib/gcc/powerpc-apple-darwin8/4.0.1

      module plot_hdf
      implicit none
      save
      integer,     parameter :: DFNT_FLOAT32  = 5
      integer,     parameter :: DFNT_FLOAT64  = 6
      integer,     parameter :: DFTAG_SDG     = 700
      
      integer, parameter :: REAL4 = SELECTED_REAL_KIND(6)
      integer, parameter :: REAL8 = SELECTED_REAL_KIND(12)
      
      integer, parameter :: DATA_TYPE_DOUBLE     = 0
      integer, parameter :: DATA_TYPE_FLOAT      = 1
      integer, parameter :: DATA_TYPE_BYTE       = 2
      integer, parameter :: DATA_TYPE_DOUBLE_3D  = 3
      integer, parameter :: DATA_TYPE_FLOAT_3D   = 4
      
      integer, external :: dspdata,dsclear,dsfirst,dssnt,dsadata,dapdesc,daplab      
      integer, external :: DFSDlastref,DFR8lastref
      integer, external :: d8pimg,d8aimg
      
        type, public ::   SDS2Dout
           character(256) path
           character(256) name
           character(16) pioName
           integer ixmax
           integer iymax
           integer izmax
           real(REAL8),    dimension(:),   pointer ::  data
           real(REAL8) xmin
           real(REAL8) xmax
           real(REAL8) ymin
           real(REAL8) ymax
           real(REAL8) zmin
           real(REAL8) zmax
           real(REAL8) vmin
           real(REAL8) vmax
           real(REAL8) time
           integer type
           integer n
	    end type SDS2Dout
	    
	  private :: writesds2dDouble,writesds2dFloat,writesds2dByte,writesds3dDouble,writesds3dFloat
	    
      contains
        integer function writesds(sdsout)
          implicit none
          type(SDS2Dout), intent(inout)  :: sdsout
          
          
		if(sdsout % xmin .gt. sdsout % xmax)then
			write(*,*)'Warning            : Time ',sdsout % time
			write(*,*)'Warning Bad Bounds : xmin ',sdsout % xmin,' xmax ',sdsout % xmax
			sdsout % xmax = sdsout % xmin+abs(sdsout % xmin)+1.0;
			write(*,*)'Reset To           : xmin ',sdsout % xmin,' xmax ',sdsout % xmax
			write(*,*)' '
		endif
		
		if(sdsout % ymin .gt. sdsout % ymax)then
			write(*,*)'Warning            : Time ',sdsout % time
			write(*,*)'Warning Bad Bounds : ymin ',sdsout % ymin,' ymax ',sdsout % ymax
			sdsout % ymax = sdsout % ymin+abs(sdsout % ymin)+1.0;
			write(*,*)'Reset To           : ymin ',sdsout % ymin,' ymax ',sdsout % ymax
			write(*,*)' '
		endif
          
         
          select case(sdsout % type)
            case (DATA_TYPE_DOUBLE)
             writesds = writesds2dDouble(sdsout)
            case (DATA_TYPE_FLOAT)
             writesds = writesds2dFloat(sdsout)
            case (DATA_TYPE_BYTE)
             writesds = writesds2dByte(sdsout)
            case (DATA_TYPE_DOUBLE_3D)
             writesds = writesds3dDouble(sdsout)
            case (DATA_TYPE_FLOAT_3D)
             writesds = writesds3dFloat(sdsout)
            case default
             write(*,*)' writesds Unknown type = ',sdsout % type
             writesds = 1
          end select
        end function writesds
        
         integer function writesds3dFloat(sdsout)
          implicit none
          type(SDS2Dout), intent(in)  :: sdsout
          real(REAL4),    dimension(:),   allocatable ::  data
          integer :: length,n
	      real(REAL8) :: vmin,vmax,v
 	      integer rank,size(3)
 	      integer ret
          character(256) :: outdata
          integer :: allostat
          integer last,type
        
          type=sdsout % type
          
          writesds3dFloat=1
          if(type.ne.DATA_TYPE_FLOAT_3D)then
              write(*,*)'writesds3dFloat type error = ',sdsout % type
              return
          endif
 
          if(.not. associated(sdsout % data))then
              write(*,*)'writesds3dFloat : Error data array not associated '
              return
          endif

 		  length=sdsout % ixmax*sdsout % iymax*sdsout % izmax
	
	      if(length .le. 0)then
	    	 write(*,*)'writesds3dFloat Found data length less than one'
	    	 write(*,*)sdsout % ixmax,sdsout % iymax,sdsout % izmax
	    	 return
	      endif
	      
	      vmin =  1e33
	      vmax = -1e33
	      do n=1,length
	         v=sdsout % data(n)
	         if(v < vmin)vmin = v
	         if(v > vmax)vmax = v
	      enddo
	      
          allocate(data(length),            stat = allostat)
          if(allostat.ne.0)then
            write(*,*)' writesds3dFloat out of memory'
            return
	       endif
	       do n=1,length
	         data(n)=sdsout % data(n)
	       enddo
	      
	      rank=3
	      size(1)=sdsout % ixmax
	      size(2)=sdsout % iymax
	      size(3)=sdsout % izmax
	      
	      if(sdsout % n .eq. 0)then
	          ret=dsclear()
	          ret=dsfirst()
              ret=dssnt(DFNT_FLOAT32)
	          ret=dspdata(sdsout % path,rank,size,data)
	       else
	          ret=dsadata(sdsout % path,rank,size,data)
	       endif

	      
	      write(outdata,710)'xmin',sdsout % xmin,'xmax',sdsout % xmax, &
                                                   'ymin',sdsout % ymin,'ymax',sdsout % ymax, &
                                                   'zmin',sdsout % zmin,'zmax',sdsout % zmax, &
                                                   'time',sdsout % time,'pioName',sdsout % pioName
      
710       format(7(a,1pe12.4,1x),a,1x,'"',a,'"')

          last=DFSDlastref()

	      ret=dapdesc(sdsout % path,DFTAG_SDG,last,outdata,len_trim(outdata))
	     
  	      ret=daplab(sdsout % path,DFTAG_SDG,last,sdsout % name)
  	      
         writesds3dFloat=0
         
         if (allocated(data)) deallocate(data, stat = allostat)
         
       end function writesds3dFloat
        
         integer function writesds3dDouble(sdsout)
          implicit none
          type(SDS2Dout), intent(in)  :: sdsout
          integer :: length,n
	      real(REAL8) :: vmin,vmax,v
 	      integer rank,size(3)
 	      integer ret
          character(256) :: outdata
          integer last
        
          writesds3dDouble=1
          if(sdsout % type.ne.DATA_TYPE_DOUBLE_3D)then
              write(*,*)'writesds3dDouble type error = ',sdsout % type
              return
          endif
 
          if(.not. associated(sdsout % data))then
              write(*,*)'writesds3dDouble : Error data array not associated '
              return
          endif

 		  length=sdsout % ixmax*sdsout % iymax*sdsout % izmax
	
	      if(length .le. 0)then
	    	 write(*,*)'writesds3dDouble Found data length less than one'
	    	 write(*,*)sdsout % ixmax,sdsout % iymax,sdsout % izmax
	    	 return
	      endif
	      
	      vmin =  1e33
	      vmax = -1e33
	      do n=1,length
	         v=sdsout % data(n)
	         if(v < vmin)vmin = v
	         if(v > vmax)vmax = v
	      enddo
	      
	      
	      rank=3
	      size(1)=sdsout % ixmax
	      size(2)=sdsout % iymax
	      size(3)=sdsout % izmax
	      
	      if(sdsout % n .eq. 0)then
	          ret=dsclear()
	          ret=dsfirst()
              ret=dssnt(DFNT_FLOAT64)
	          ret=dspdata(sdsout % path,rank,size,sdsout % data)
	       else
	          ret=dsadata(sdsout % path,rank,size,sdsout % data)
	       endif

	      
	      write(outdata,710)'xmin',sdsout % xmin,'xmax',sdsout % xmax, &
                                                   'ymin',sdsout % ymin,'ymax',sdsout % ymax, &
                                                   'zmin',sdsout % zmin,'zmax',sdsout % zmax, &
                                                   'time',sdsout % time,'pioName',sdsout % pioName
      
710       format(7(a,1pe12.4,1x),a,1x,'"',a,'"')

          last=DFSDlastref()

	      ret=dapdesc(sdsout % path,DFTAG_SDG,last,outdata,len_trim(outdata))
	     
  	      ret=daplab(sdsout % path,DFTAG_SDG,last,sdsout % name)
  	      
         writesds3dDouble=0
                  
       end function writesds3dDouble
          
         integer function writesds2dByte(sdsout)
          implicit none
          type(SDS2Dout), intent(in)  :: sdsout
          character,    dimension(:),   allocatable ::  data
          integer :: length,n
	      real(REAL8) :: vmin,vmax,v
 	      integer rank,size(2)
 	      integer ret
          character(256) :: outdata
          integer :: allostat
          integer last,type
        
          type=sdsout % type
          
          writesds2dByte=1
          if(type.ne.DATA_TYPE_BYTE)then
              write(*,*)'writesds2dByte type error = ',sdsout % type
              return
          endif
 
          if(.not. associated(sdsout % data))then
              write(*,*)'writesds2dByte : Error data array not associated '
              return
          endif

 		  length=sdsout % ixmax*sdsout %iymax
	
	      if(length .le. 0)then
	    	 write(*,*)'writesds2dByte Found data length less than one'
	    	 return
	      endif
	      
	      vmin =  1e33
	      vmax = -1e33
	      do n=1,length
	         v=sdsout % data(n)
	         if(v < vmin)vmin = v
	         if(v > vmax)vmax = v
	      enddo
	      
          allocate(data(length),            stat = allostat)
          if(allostat.ne.0)then
            write(*,*)' writesds2dByte out of memory'
            return
	       endif
	       
		if(vmax .le. vmin)then
			vmax=vmin+1+2*abs(vmin)
			do n=1,length
				data(n)=char(2)
			enddo
		else
			do n=1,length
				v=sdsout % data(n)
				data(n)=char(int(2+252.*(v-vmin)/(vmax-vmin)))
			enddo
		endif
	       
	      rank=2
	      size(1)=sdsout % ixmax
	      size(2)=sdsout % iymax
	      
	      if(sdsout % n .eq. 0)then
              ret=d8pimg(sdsout % path,data,sdsout % ixmax,sdsout % iymax,11)
	       else
              ret=d8aimg(sdsout % path,data,sdsout % ixmax,sdsout % iymax,11)
	       endif

	      write(outdata,710)'xmin',sdsout % xmin,'xmax',sdsout % xmax, &
                            'ymin',sdsout % ymin,'ymax',sdsout % ymax, &
                            'zmin',sdsout % zmin,'zmax',sdsout % zmax, &
                            'vmin',vmin,'vmax',vmax, &
                            'time',sdsout % time,'pioName',sdsout % pioName
      
710       format(9(a,1pe12.4,1x),a,1x,'"',a,'"')
    
          last=DFR8lastref()

	      ret=dapdesc(sdsout % path,DFTAG_SDG,last,outdata,len_trim(outdata))
	     
  	      ret=daplab(sdsout % path,DFTAG_SDG,last,sdsout % name)
  	      
         writesds2dByte=0
         
         if (allocated(data)) deallocate(data, stat = allostat)
         
       end function writesds2dByte
          
       
         integer function writesds2dFloat(sdsout)
          implicit none
          type(SDS2Dout), intent(in)  :: sdsout
          real(REAL4),    dimension(:),   allocatable ::  data
          integer :: length,n
	      real(REAL8) :: vmin,vmax,v
 	      integer rank,size(2)
 	      integer ret
          character(256) :: outdata
          integer :: allostat
          integer last,type
        
          type=sdsout % type
          
          writesds2dFloat=1
          if(type.ne.DATA_TYPE_FLOAT)then
              write(*,*)'writesds2dFloat type error = ',sdsout % type
              return
          endif
 
          if(.not. associated(sdsout % data))then
              write(*,*)'writesds2dFloat : Error data array not associated '
              return
          endif

 		  length=sdsout % ixmax*sdsout %iymax
	
	      if(length .le. 0)then
	    	 write(*,*)'writesds2dFloat Found data length less than one'
	    	 return
	      endif
	      
	      vmin =  1e33
	      vmax = -1e33
	      do n=1,length
	         v=sdsout % data(n)
	         if(v < vmin)vmin = v
	         if(v > vmax)vmax = v
	      enddo
	      
          allocate(data(length),            stat = allostat)
          if(allostat.ne.0)then
            write(*,*)' writesds2dFloat out of memory'
            return
	       endif
	       do n=1,length
	         data(n)=sdsout % data(n)
	       enddo
	      
	      rank=2
	      size(1)=sdsout % ixmax
	      size(2)=sdsout % iymax
	      
	      if(sdsout % n .eq. 0)then
	          ret=dsclear()
	          ret=dsfirst()
              ret=dssnt(DFNT_FLOAT32)
	          ret=dspdata(sdsout % path,rank,size,data)
	       else
	          ret=dsadata(sdsout % path,rank,size,data)
	       endif

	      
	      write(outdata,710)'xmin',sdsout % xmin,'xmax',sdsout % xmax, &
                                                   'ymin',sdsout % ymin,'ymax',sdsout % ymax, &
                                                   'zmin',sdsout % zmin,'zmax',sdsout % zmax, &
                                                   'time',sdsout % time,'pioName',sdsout % pioName
      
710       format(7(a,1pe12.4,1x),a,1x,'"',a,'"')

          last=DFSDlastref()

	      ret=dapdesc(sdsout % path,DFTAG_SDG,last,outdata,len_trim(outdata))
	     
  	      ret=daplab(sdsout % path,DFTAG_SDG,last,sdsout % name)
  	      
  
         writesds2dFloat=0
         
         if (allocated(data)) deallocate(data, stat = allostat)
         
       end function writesds2dFloat
        
        integer function writesds2dDouble(sdsout)
          implicit none
          type(SDS2Dout), intent(in)  :: sdsout
          integer :: length,n
	      real(REAL8) :: vmin,vmax,v
 	      integer rank,size(2)
 	      integer ret
          character(256) :: outdata
          integer last
        
          writesds2dDouble=1
          if(sdsout % type.ne.DATA_TYPE_DOUBLE)then
              write(*,*)'writesds2dDouble type error = ',sdsout % type
              return
          endif
 
          if(.not. associated(sdsout % data))then
              write(*,*)'writesds2dDouble : Error data array not associated '
              return
          endif
 
 		  length=sdsout % ixmax*sdsout %iymax
	
	      if(length .le. 0)then
	    	 write(*,*)'writesds2dDouble Found data length less than one'
	    	 return
	      endif
	      
	      vmin =  1e33
	      vmax = -1e33
	      do n=1,length
	         v=sdsout % data(n)
	         if(v < vmin)vmin = v
	         if(v > vmax)vmax = v
	      enddo
	      
	      rank=2
	      size(1)=sdsout % ixmax
	      size(2)=sdsout % iymax
	      
	      if(sdsout % n .eq. 0)then
	          ret=dsclear()
	          ret=dsfirst()
              ret=dssnt(DFNT_FLOAT64)
	          ret=dspdata(sdsout % path,rank,size,sdsout % data)
	       else
	          ret=dsadata(sdsout % path,rank,size,sdsout % data)
	       endif

	      
	      write(outdata,710)'xmin',sdsout % xmin,'xmax',sdsout % xmax, &
                                                   'ymin',sdsout % ymin,'ymax',sdsout % ymax, &
                                                   'zmin',sdsout % zmin,'zmax',sdsout % zmax, &
                                                   'time',sdsout % time,'pioName',sdsout % pioName
      
710       format(7(a,1pe12.4,1x),a,1x,'"',a,'"')
   
          last=DFSDlastref()

	      ret=dapdesc(sdsout % path,DFTAG_SDG,last,outdata,len_trim(outdata))
	     
  	      ret=daplab(sdsout % path,DFTAG_SDG,last,sdsout % name)
  	      
         writesds2dDouble=0
                  
       end function writesds2dDouble
          
      
      end module plot_hdf
      
      
      program main
      use plot_hdf
      implicit none
      integer, external :: writeData,writeData3d
      integer :: ret
      
      ret= writeData('waveDouble',DATA_TYPE_DOUBLE)
      ret= writeData('waveFloat',DATA_TYPE_FLOAT)
      ret= writeData('waveBytes',DATA_TYPE_BYTE)
      ret= writeData3d('waveDouble3d',DATA_TYPE_DOUBLE_3D)
      ret= writeData3d('waveFloat3d',DATA_TYPE_FLOAT_3D)
      
      stop
      end
      
      integer function writeData3d(nameFile,type)
      use plot_hdf
      implicit none
      character(*), intent(in)  :: nameFile
      integer,      intent(in)  :: type
      
      type(SDS2Dout) :: sdsout
      real(REAL8) :: xmin,xmax,ymin,ymax,zmin,zmax
	  character(256) buff
	  character(256) name
	  real(REAL8) :: dt,time
	  real(REAL8) :: value
	  real(REAL8) :: xc,yc,zc,r,x,y,z
	  real(REAL8) :: dxdi,dydj,dzdk
      integer :: ixmax
      integer :: iymax
      integer :: izmax
      integer :: n,i,j,k
      integer :: allostat
      
 	
       writeData3d=1
         
       if(type.ne.DATA_TYPE_DOUBLE_3D.and.type.ne.DATA_TYPE_FLOAT_3D)then
         write(*,*)'writeData3d Found Wrong type = ',type
         return
       endif
       
  	   r=20
 	
 	   ixmax=150
 	   iymax=200
 	   izmax=250
 	
 	   xmin=0
 	   xmax=400
 	
 	   dxdi=(xmax-xmin)/real(ixmax, REAL8)
 	
 	   ymin=300
 	   ymax=600
 	
 	   dydj=(ymax-ymin)/real(iymax, REAL8)
 
 	   zmin=400
 	   zmax=800
 	
 	   dzdk=(zmax-zmin)/real(izmax, REAL8)
 
 	   dt=1e-6
 	
 	   time=0
		
      allocate(sdsout % data(ixmax*iymax*izmax),            stat = allostat)
      if(allostat.ne.0)then
         write(*,*)' writeData3d out of memory'
         return
	  endif
	  
	yc=ymin
	xc=xmin
	zc=zmin
	
	do n=0,49
	  do k=0,izmax-1
	    z=zmin+0.5*dzdk+real(k, REAL8)*dzdk             ! /* locate z center of pixel */
	    do j=0,iymax-1
	        y=ymin+0.5*dydj+real(j, REAL8)*dydj         ! /* locate y center of pixel */
	        do i=0,ixmax-1
	            x=xmin+0.5*dxdi+real(i, REAL8)*dxdi     ! /* locate x center of pixel */
	            value=sqrt((x-xc)*(x-xc)+(y-yc)*(y-yc)+(z-zc)*(z-zc))
	            if(value < r)value = 1000
	            
	            sdsout % data(1+i+ixmax*j+k*ixmax*iymax)=value
	            
	        enddo
	    enddo
	  enddo
	    
		xc = xc + (xmax-xmin)/50.
	    yc = yc + (ymax-ymin)/50.
		zc = zc + (zmax-zmin)/50.
		
		
	    write(name,'(a,i5,a)')nameFile,n+10000,'.s3d'
	    write(buff,'(a,i5,a)')nameFile,n+10000,'.s3d'
	   
		sdsout % path=buff
	    
		sdsout % name=name
		sdsout % ixmax=ixmax
		sdsout % iymax=iymax
		sdsout % izmax=izmax
		sdsout % xmin=xmin
		sdsout % xmax=xmax
		sdsout % ymin=ymin
		sdsout % ymax=ymax
		sdsout % zmin=zmin
		sdsout % zmax=zmax
		sdsout % time=time
		sdsout % n=0
		sdsout % pioName='pres'	
		sdsout % type=type	

		if(writesds(sdsout).ne.0)goto 500
		
		time = time + dt
	
	enddo
	   
      writeData3d=0
500  continue
     deallocate(sdsout % data, stat = allostat)
     return
     end function writeData3d

      integer function writeData(nameFile,type)
      use plot_hdf
      implicit none
      character(*), intent(in)  :: nameFile
      integer,      intent(in)  :: type
      
      type(SDS2Dout) :: sdsout
      real(REAL8) :: xmin,xmax,ymin,ymax
	  character(256) buff
	  character(256) name
	  real(REAL8) :: dt,time
	  real(REAL8) :: value
	  real(REAL8) :: xc,yc,r,x,y
	  real(REAL8) :: dxdi,dydj
      integer :: ixmax
      integer :: iymax
      integer :: n,i,j
      integer :: allostat
      
       writeData=1
       
       if(type.ne.DATA_TYPE_DOUBLE.and.type.ne.DATA_TYPE_FLOAT.and.type.ne.DATA_TYPE_BYTE)then
         write(*,*)'writeData Found Wrong type = ',type
         return
       endif
         
 	   r=20
 	
 	   ixmax=720
 	   iymax=480
 	
 	   xmin=0
 	   xmax=1440
 	
 	   dxdi=(xmax-xmin)/real(ixmax, REAL8)
 	
 	   ymin=480
 	   ymax=1440
 	
 	   dydj=(ymax-ymin)/real(iymax, REAL8)
 	 	
 	   dt=1e-6_REAL8
 	
 	   time=0
      
		if(type == DATA_TYPE_BYTE)then
		    buff= nameFile // '.hdf'
		else
		    buff= nameFile // '.s2d'
		endif
		
		sdsout % path=buff
      
      allocate(sdsout % data(ixmax*iymax),            stat = allostat)
      if(allostat.ne.0)then
         write(*,*)' writeData out of memory'
         return
	  endif
	  
	yc=ymin
	xc=xmin
	
	do n=0,99
	    do j=0,iymax-1
	        y=ymin+0.5*dydj+real(j, REAL8)*dydj         ! /* locate y center of pixel */
	        do i=0,ixmax-1
	            x=xmin+0.5*dxdi+real(i, REAL8)*dxdi     !/* locate x center of pixel */
	            value=sqrt((x-xc)*(x-xc)+(y-yc)*(y-yc))
	            if(value < r)value = 1000
	            
	            sdsout % data(1+i+ixmax*(iymax-j-1))=value
	            
	        enddo
	    enddo
	    
	    yc = yc + (ymax-ymin)/100.
		xc = xc + (xmax-xmin)/100.
		
		
	    write(name,'(a,i5)')'dump',n+10000
	   
	    
		sdsout % name=name
		sdsout % ixmax=ixmax
		sdsout % iymax=iymax
		sdsout % xmin=xmin
		sdsout % xmax=xmax
		sdsout % ymin=ymin
		sdsout % ymax=ymax
		sdsout % zmin=0
		sdsout % zmax=0
		sdsout % time=time
		sdsout % n=n
		sdsout % pioName='pres'	
		sdsout % type=type	


		if(writesds(sdsout).ne.0)goto 500
		
		time = time + dt
	
	enddo
	   
      writeData=0
500  continue
     deallocate(sdsout % data, stat = allostat)
     return
     end function writeData

      
      