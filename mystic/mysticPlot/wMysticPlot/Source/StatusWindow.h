typedef struct statStruct{
	/*  Must Match other check status */
	char lastMessage[256];
	int hit;
	int gotname;
	int iconnect;
	int iwrite;
	int iread;
	int iaccept;
	int sending;
	IconPtr rastorAppend;
	IconPtr tekAppend;
	IconPtr trueAppend;
	int saveRastorToFile;
	int saveTekToFile;
	int saveTrueToFile;
	/*  Must Match other check status */
};

typedef struct statStruct *statPtr;

int SizeIt(IconPtr win);

#define BUTTON_NUMBER	500

IconPtr StartStatusWindow(char *header);
StopStatusWindow(IconPtr myIcon);
int WriteStatusWindow(IconPtr StatusWindow,char *buff);
int CheckStatusWindow(IconPtr myIcon);
