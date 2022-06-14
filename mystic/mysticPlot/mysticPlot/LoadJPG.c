#define EXTERN22 extern
#include "Xappl.h"
#include "jpeglib.h"

#include <setjmp.h>


extern char WarningBuff[1024];
extern int WarningBatch(char *Message);
extern int SetBuffers(long Length);
extern int cFree(char *ptr);
int doJPG(char *filename,unsigned char *image,int xsize,int ysize);
int GetJPGImage(char *name,long *xsizer,long *ysizer,unsigned char **image,int flag);


struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;


METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* (*cinfo->err->output_message) (cinfo); */
  
  WarningBatch("JPG Library Error\n");

  longjmp(myerr->setjmp_buffer, 1);
}



int GetJPGImage(char *name,long *xsizer,long *ysizer,unsigned char **image,int flag)
{

	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	JSAMPARRAY buffer;		/* Output row buffer */
	int row_stride;		/* physical row width in output buffer */
	unsigned long xsize,ysize,length;
	unsigned char *bs,*bp,*bo,*im;
	FILE *in;
	int ret,k;

	if(!name || !image || !xsizer || !ysizer)return 1;


	im=NULL;
	
	if(!(in=fopen(name,"rb"))){
	    sprintf(WarningBuff,"GetJPGImage Could Not Open File %s To Read\n",name);
		WarningBatch(WarningBuff);
	    return 1;
	}
	ret=1;

	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;

	if (setjmp(jerr.setjmp_buffer)) {
		goto ErrorOut;
	}

	jpeg_create_decompress(&cinfo);

 
	jpeg_stdio_src(&cinfo, in);

	(void) jpeg_read_header(&cinfo, TRUE);

    xsize=cinfo.image_width;	/* scaled image width */
    ysize=cinfo.image_height;	/* scaled image height */

	length=xsize*ysize*3;
	
	*xsizer = (long)xsize;
	*ysizer = (long)ysize;

	if(flag == 0){
		ret=0;
		goto ErrorOut;
	}
	
	if(*image == NULL){
	    im=(unsigned char *)cMalloc((long)length,9039);
	    if(!(im)){
	        sprintf(WarningBuff,"GetJPGImage - %s Out Of Memory - length %ld\n",name,length);
		    WarningBatch(WarningBuff);
		    goto ErrorOut;
	    }
	}else{
	    im = *image;
	}

	(void) jpeg_start_decompress(&cinfo);

	row_stride = cinfo.output_width * cinfo.output_components;

	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	bs = bp = im;
	while (cinfo.output_scanline < cinfo.output_height) {

		(void) jpeg_read_scanlines(&cinfo, buffer, 1);

		bo=buffer[0];
		bp=bs;
		if(cinfo.out_color_space == JCS_GRAYSCALE){
			for(k=0;k<row_stride;++k){
				*bp++=*bo;
				*bp++=*bo;
				*bp++=*bo++;
			}
			bs += row_stride*3;
		}else{
			for(k=0;k<row_stride;++k){
				*bp++=*bo++;
			}
			bs += row_stride;
		}

	}

 
  (void) jpeg_finish_decompress(&cinfo);

	if(*image == NULL){
	    *image = im;
	}
	
	/* if(*image)flip2(*image,(int)xsize,(int)ysize); */

	ret=0;
ErrorOut:
	if(ret){
	    sprintf(WarningBuff,"GetJPGImage - Error Reading JPG file %s\n",name);
		WarningBatch(WarningBuff);
	    if(*image == NULL){
	        if(im)cFree((char *)im);
	    }
	}
	if(in)fclose(in);
	jpeg_destroy_decompress(&cinfo);
	return ret;
}

int doJPG(char *filename,unsigned char *image,int xsize,int ysize)
{
	FILE *out;
	int ret;
	struct jpeg_compress_struct cinfo;
	struct my_error_mgr jerr;
	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
	int row_stride;		/* physical row width in image buffer */
	int quality=100;

	if(!filename || !image)return 1;

	ret = 1;
	out = NULL;

	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;

	jpeg_create_compress(&cinfo);

	if (setjmp(jerr.setjmp_buffer)) {
		goto ErrorOut;
	}


	if ((out = fopen(filename, "wb")) == NULL) {
		sprintf(WarningBuff, "can't open %s to write\n", filename);
		WarningBatch(WarningBuff);
		goto ErrorOut;
	}

	jpeg_stdio_dest(&cinfo, out);

	cinfo.image_width = xsize;
	cinfo.image_height = ysize;
	cinfo.input_components = 3;		/* # of color components per pixel */
	cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */

	jpeg_set_defaults(&cinfo);

  /* Now you can set any non-default parameters you wish to.
   * Here we just illustrate the use of quality (quantization table) scaling:
   */


	jpeg_set_quality(&cinfo,quality,TRUE);



	jpeg_start_compress(&cinfo, TRUE);

	row_stride = xsize * 3;	/* JSAMPLEs per row in image_buffer */

	while (cinfo.next_scanline < cinfo.image_height) {
		row_pointer[0] = & image[cinfo.next_scanline * row_stride];
		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}


	jpeg_finish_compress(&cinfo);
 
	ret = 0;
ErrorOut:
	if(out){
		fclose(out);
	}
	jpeg_destroy_compress(&cinfo);
	return ret;
}

