
struct EditList{
    char **LineBuffer;
    long LineMax;
    long LineCount;
    long row;
    long column;

};

typedef struct EditList *EditPtr;


EditPtr EditNewFile(char *path);

int EditListClose(EditPtr e);

char *EditListLine(EditPtr e,long Line);

int EditListDeleteOne(EditPtr e,long row,long column);

int EditListInsert(EditPtr e,char *data,long row,long column);

int EditGetLast(EditPtr e,long *row,long *column);

char *EditgetSelection(EditPtr e,long frow,long fcolumn,long lrow,long lcolumn);

int EditkillSelection(EditPtr e,long frow,long fcolumn,long lrow,long lcolumn);

int EditListAppendLine(EditPtr e,char *data);
