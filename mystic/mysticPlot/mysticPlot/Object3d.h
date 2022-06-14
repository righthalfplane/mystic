#define CONTOUR_INCREMENT 100

struct tree{
	double value;
	int Count;
	int MaxCount;
	long *List;	
	struct tree *left;
	struct tree *right;
};

struct cdata{
    double *xdata;
    double *ydata;
    double *vdata;
    long nndata;
    long nlevel;
};

struct cdatal{
    struct cdata *cList;
    long count;
    long cmax;
};
