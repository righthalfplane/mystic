struct ImageSaveList{
    struct FileInfo2 *Files;
	int To256Colors;
	int UseImageNames;
	int TypeControlValue;
	long ImageIncrement;
	char *prefix;
	char *suffix;
	long SequenceNumber;
	char *savename;
	long *imageList;
	char *directory;
	long count;
	int type;
	uRect *DRect;
};


int ImageFreeList(struct ImageSaveList *Images);

struct ImageSaveList *SaveAllImages(char *name,struct FileInfo2 *Files);

int doSaveImages(IconPtr myIcon,struct ImageSaveList *Images);

struct ImageSaveList *CreateImageList(long count);

int doSaveQuickTimeOptions(IconPtr myIcon,struct ImageSaveList *Images);


#define IMAGE_PICT					1
#define IMAGE_PICT32				2
#define IMAGE_HDF					3
#define IMAGE_HDF_MERGE				4
#define IMAGE_TRUE					5
#define IMAGE_TRUE_MERGE			6
#define IMAGE_QUICKTIME8			7
#define IMAGE_QUICKTIME32			8
#define IMAGE_SDS2D					9
#define IMAGE_SDS2D_MERGE			10
#define IMAGE_JPG					11
#define IMAGE_HDF_TO_256			12
#define IMAGE_HDF_TO_256_MERGE		14
#define IMAGE_BMP					15
#define IMAGE_AVI					16
#define IMAGE_TEK					17
#define IMAGE_RGB					18
