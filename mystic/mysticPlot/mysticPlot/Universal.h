
#ifndef __STARTUNIVERSAL__
#define __STARTUNIVERSAL__

#include "File8.h"

#define universal_WindowGlobal			100
#define universal_WindowSendFile		101
#define universal_WindowSagePIO2D		102
#define universal_WindowSagePIO3D		103
#define universal_WindowRaster2D		104
#define universal_WindowTEK				105
#define universal_WindowNewFile			106
#define universal_WindowNewEdit			107
#define universal_WindowRender			108
#define universal_WindowVolumetric		109
#define universal_WindowDRAW			110
#define universal_WindowDump			111
#define universal_WindowSageBLK3D		112

#define universal_SetDefaults		200
#define universal_GetDefaults		201
#define universal_ReadData			202
#define universal_SaveData			203
#define universal_Stop				204
#define universal_MoveToStatic		205
#define universal_MoveToDynamic		206


#define universal_TypeRect  			300
#define universal_TypeString  			301
#define universal_TypeInfo  			302
#define universal_TypeInt  				303
#define universal_TypeStringPtr  		304
#define universal_TypeDouble  			305
#define universal_TypeTable  			306
#define universal_TypeFilesPtr  		307
#define universal_TypeLong  			308
#define universal_TypePaletteList  		309
#define universal_TypeDoubleArray  		310
#define universal_TypeCursorPtr  		311
#define universal_TypeFunctionPtr   	312
#define universal_TypeIntArray  		314
#define universal_TypeRealArrayPtr  	315
#define universal_TypeBytePtr       	316
#define universal_TypeBytes       		317
#define universal_TypeDoubleArrayPtr	318
#define universal_TypeDoubleLongPtr		319


struct universalPalettes{
    char *name;
    unsigned char *pal;
};

struct universalListOfPalettes{
	int palCount;
	int palMax;
	struct universalPalettes *pals;
};

struct universalPreferencesStruct{
	struct universalListOfPalettes palList;
	uRect SaveFileRect;
	uRect LoadFileRect;
	uRect SaveImageRect;
	uRect dataSelectRect;
	uRect newDialogRect;
	uRect dumpSelectRect;
	uRect cursorSelectRect;
	int SaveSDS64Bits;
	int LocalOpenDialog;
	int TextOutputFormat;
	int UseStartConfiguration;
	char *startConfiguration;
	int flipSDS2dY;
	int VoiceRecognition;
	long BlockSizeGlobal;
	char *OpenWithApplication;
/* scratch variables */	
	struct CursorStruct Cursor;
	struct FileList *Files;
	struct FileList *Files2;
	int config;
	char *path;
	char *readWritePath;
	char *relativePath;
};


struct universalTableListStruct{
	char *name;
	long tag;
	int dataType;
	void *data;
	long space;
	char *command;
};

typedef struct universalTableListStruct *universalListPtr;

struct universalTableStruct{
	char *tableName;
	long tableTag;
	void *tableStatic;
	long tableStaticSpace;
	void *tableDynamic;
	long tableDynamicSpace;
	struct universalTableListStruct *TableList;
	int TableCount;
};

typedef struct universalTableStruct *universalTablePtr;

int setUniversalDefaults(void);
int startUniversal(void);
int stopUniversal(void);
int saveUniversal(IconPtr myIcon);
char *getUniversalPath(void);
int doUniversalWindows(int operation,int flag,FILE8 *in);
int saveUniversalHeader(char *dataType,long tag,long length,FILE8 *out);
int getUniversalHeader(char *dataType,long *tag,long *length,FILE8 *in);


int skipUniversal(long length,FILE8 *in);


int saveUniversalRect(uRect *Save,long tag,FILE8 *out);
int getUniversalRect(uRect *Save,FILE8 *in);

int saveUniversalString(char *data,long tag,FILE8 *out);
int getUniversalString(char *data,long length,long space,FILE8 *out);

int saveUniversalBytes(char *Save,long tag,long length,FILE8 *out);

int getUniversalStringPtr(char **data,long length,FILE8 *out);

int saveUniversalReal(void *Save,long tag,int type,long space,FILE8 *out);
int getUniversalReal(void *data,int type,long length,long *space,FILE8 *out);

int getUniversalDouble(void *Save,int type,long length,long space,FILE8 *in);
int saveUniversalDouble(void *Save,long tag,int type,long space,FILE8 *out);

int doUniversalNewFile(IconPtr myIcon,int flag,FILE8 *inOut);
int doUniversalFileSend(IconPtr myIcon,int flag,FILE8 *inOut);
int doUniversalSagePIO2D(IconPtr myIcon,int flag,FILE8 *inOut);
int doUniversalSagePIO3D(IconPtr myIcon,int flag,FILE8 *inOut);
/* int doUniversalSageBLK3D(IconPtr myIcon,int flag,FILE8 *inOut); */
int doUniversalRaster2D(IconPtr myIcon,int flag,FILE8 *inOut);
int doUniversalTek(IconPtr myIcon,int flag,FILE8 *inOut);
int doUniversalGlobal(IconPtr myIcon,int flag,FILE8 *in);
int doUniversalNewEdit(IconPtr myIcon,int flag,FILE8 *inOut);
int doUniversalRender(IconPtr myIcon,int flag,FILE8 *inOut);
int doUniversalVolumetric(IconPtr myIcon,int flag,FILE8 *inOut);
int doUniversalDraw(IconPtr myIcon,int flag,FILE8 *inOut);
int doUniversalDump(IconPtr myIcon,int flag,FILE8 *inOut);


IconPtr NewBLK3DWindow(IconPtr myIcon);
IconPtr NewPIO3DWindow(IconPtr myIcon);
IconPtr NewPIO2DWindow(IconPtr myIcon);
IconPtr NewNetWindow(IconPtr myIcon);
IconPtr NewFileSendWindow(IconPtr myIcon);
IconPtr NewTekWindow(IconPtr myIcon);
IconPtr NewNewWindow(IconPtr myIcon);
IconPtr NewEditWindow(IconPtr myIcon);
IconPtr NewRenderWindow(IconPtr myIcon);
IconPtr NewVolumetricWindow(IconPtr myIcon);
IconPtr NewDrawWindow(IconPtr myIcon);

int getUniversalTable(universalTablePtr Table,FILE8 *in);
int putUniversalTable(int windowType,universalTablePtr Table,FILE8 *out);
int moveUniversalTable(universalTablePtr Table,int flag);
int freeUniversalTable(universalTablePtr Table);

int writeUniversal(IconPtr myIcon,int flag);
int readUniversal(char *path,int flag);

int saveUniversalPath(char *path);

int testPreferenceFile(char *path,unsigned int *crcval,int flag);

int getUniversalPaletteItem(int n,char **name,unsigned char **pal);

int deleteUniversalPaletteByName(char *name);

int addUniversalPalette(char *name,unsigned char *r,unsigned char *g,unsigned char *b);

int sortUniversalPalette(void);

int getUniversalPaletteByName(char *name,unsigned char **pal);

int putENDOFLINE(FILE *out);

int FilesOpenUniversal(struct FileInfo2 *Files);

extern struct universalPreferencesStruct uPref;


#define BS 8
#define FS 28

#endif

