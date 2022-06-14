#ifndef _VRML_

#define _VRML_

#define DEFAULT			0
#define OVERALL			1
#define PER_PART		2
#define PER_PART_INDEXED	3
#define PER_FACE		4
#define PER_FACE_INDEXED	5
#define PER_VERTEX		6
#define PER_VERTEX_INDEXED	7

#define T_VRML_ALL		0
#define T_VRML_SIDES		1
#define T_VRML_BOTTOM		2
#define T_VRML_TOP		3

#define T_VRML_REPEAT		0
#define T_VRML_CLAMP		1

#define VRML_BY_FACE		1
#define VRML_BY_VERTEX		2

#define T_VRML_EAT			0
#define T_VRML_Separator		1
#define T_VRML_TransformSeparator	2
#define T_VRML_Group			3
#define T_VRML_Rotation			4
#define T_VRML_Translation		5
#define T_VRML_Scale			6
#define T_VRML_MaterialBinding		7
#define T_VRML_Material			8
#define T_VRML_MatrixTransform		9
#define T_VRML_Coordinate3		10
#define T_VRML_IndexedFaceSet		11
#define T_VRML_Normal			12
#define T_VRML_NormalBinding		13
#define T_VRML_Use			14
#define T_VRML_Transform		15
#define T_VRML_Sphere			16
#define T_VRML_Cylinder			17
#define T_VRML_Cube			18
#define T_VRML_Cone			19
#define T_VRML_Switch			20
#define T_VRML_IndexedLineSet		21
#define T_VRML_TextureCoordinate2	22
#define T_VRML_Texture2Transform	23
#define T_VRML_Texture2 		24

extern char *vrmlTypeNames[25];

struct checker{
    char *data;
    long count;
    long countmax;
};


struct vrmlObjects{
    long type;
    long nameNumber;
    int (*killObject)(struct vrmlObjects *o);
    struct vrmlObjects *(*duplicateObject)(struct vrmlObjects *o);
/*--------------------------------------------------------------------*/
};

struct vrmlNamelist{
    char *name;
    long objectNumber;
    struct vrmlObjects *o;
};

struct vrmlList{
    struct vrmlObjects *vrml;
    struct vrmlNamelist *list;
    long nameCount;
    long nameMax;
    long MostNodes;
    long MostNormals;
    long MostCells;
};

struct vrmlSeparator{
    long type;
    long nameNumber;
    int (*killObject)(struct vrmlObjects *o);
    struct vrmlObjects *(*duplicateObject)(struct vrmlObjects *o);
/*--------------------------------------------------------------------*/
    long whichChild;
    struct vrmlObjects **o;
    long Count;
    long CountMax;
};

struct vrmlRotation{
    long type;
    long nameNumber;
    int (*killObject)(struct vrmlObjects *o);
    struct vrmlObjects *(*duplicateObject)(struct vrmlObjects *o);
/*--------------------------------------------------------------------*/
    double axis[3];
    double angle;
};

struct vrmlTranslation{
    long type;
    long nameNumber;
    int (*killObject)(struct vrmlObjects *o);
    struct vrmlObjects *(*duplicateObject)(struct vrmlObjects *o);
/*--------------------------------------------------------------------*/
    double offset[3];
};

struct vrmlScale{
    long type;
    long nameNumber;
    int (*killObject)(struct vrmlObjects *o);
    struct vrmlObjects *(*duplicateObject)(struct vrmlObjects *o);
/*--------------------------------------------------------------------*/
   double scale[3];
};

struct vrmlTransForm{
    long type;
    long nameNumber;
    int (*killObject)(struct vrmlObjects *o);
    struct vrmlObjects *(*duplicateObject)(struct vrmlObjects *o);
/*--------------------------------------------------------------------*/
    double translation[3];
    double rotation[4];
    double scaleFactor[3];
    double scaleOrientation[4];
    double center[4];
};

struct vrmlMaterialBinding{
    long type;
    long nameNumber;
    int (*killObject)(struct vrmlObjects *o);
    struct vrmlObjects *(*duplicateObject)(struct vrmlObjects *o);
/*--------------------------------------------------------------------*/
    long value;
};

struct vrmlMaterial{
    long type;
    long nameNumber;
    int (*killObject)(struct vrmlObjects *o);
    struct vrmlObjects *(*duplicateObject)(struct vrmlObjects *o);
/*--------------------------------------------------------------------*/
	double *ambientdata;
	long ambient;
	long ambientmax;

	double *diffusedata;
	long diffuse;
	long diffusemax;

	double *speculardata;
	long specular;
	long specularmax;

	double *emissivedata;
	long emissive;
	long emissivemax;

	double *shinedata;
	long shine;
	long shinemax;

	double *transdata;
	long trans;
	long transmax;

	long *flagsdata;
	long flags;
	long flagsmax;

	long max;
};

struct vrmlMatrixTransform{
    long type;
    long nameNumber;
    int (*killObject)(struct vrmlObjects *o);
    struct vrmlObjects *(*duplicateObject)(struct vrmlObjects *o);
/*--------------------------------------------------------------------*/
    double matrix[16];
};

struct vrmlCoordinate3{
    long type;
    long nameNumber;
    int (*killObject)(struct vrmlObjects *o);
    struct vrmlObjects *(*duplicateObject)(struct vrmlObjects *o);
/*--------------------------------------------------------------------*/
    double *pointdata;
    long points;
    long pointsmax;
};

struct vrmlIndexedFaceSet{
    long type;
    long nameNumber;
    int (*killObject)(struct vrmlObjects *o);
    struct vrmlObjects *(*duplicateObject)(struct vrmlObjects *o);
/*--------------------------------------------------------------------*/
	long *coorddata;
	long coord;
	long coordmax;
	long *matdata;
	long mat;
	long matmax;
	long *normdata;
	long norm;
	long normmax;
	long *textdata;
	long text;
	long textmax;
	long FirstCell;
	long Material;
	long nx;
	long ny;
};

struct vrmlNormal{
    long type;
    long nameNumber;
    int (*killObject)(struct vrmlObjects *o);
    struct vrmlObjects *(*duplicateObject)(struct vrmlObjects *o);
/*--------------------------------------------------------------------*/
    double *normaldata;
    long normal;
    long normalmax;
};

struct vrmlNormalBinding{
    long type;
    long nameNumber;
    int (*killObject)(struct vrmlObjects *o);
    struct vrmlObjects *(*duplicateObject)(struct vrmlObjects *o);
/*--------------------------------------------------------------------*/
    long value;
};

struct vrmlUse{
    long type;
    long nameNumber;
    int (*killObject)(struct vrmlObjects *o);
    struct vrmlObjects *(*duplicateObject)(struct vrmlObjects *o);
/*--------------------------------------------------------------------*/
    struct vrmlObjects *o;
    long objectNumber;
};

struct vrmlSphere{
    long type;
    long nameNumber;
    int (*killObject)(struct vrmlObjects *o);
    struct vrmlObjects *(*duplicateObject)(struct vrmlObjects *o);
/*--------------------------------------------------------------------*/
    double radius;
    long Material;
    long FirstCell;
    long nx;
    long ny;
};

struct vrmlCylinder{
    long type;
    long nameNumber;
    int (*killObject)(struct vrmlObjects *o);
    struct vrmlObjects *(*duplicateObject)(struct vrmlObjects *o);
/*--------------------------------------------------------------------*/
    long parts;
    double radius;
    double height;
    long Material;
    long FirstCell;
    long nx;
    long ny;
};

struct vrmlDisk{
    long type;
    long nameNumber;
    int (*killObject)(struct vrmlObjects *o);
    struct vrmlObjects *(*duplicateObject)(struct vrmlObjects *o);
/*--------------------------------------------------------------------*/
    long parts;
    double radius;
    double height;
    long Material;
    long FirstCell;
    long nx;
    long ny;
};


struct vrmlCube{
    long type;
    long nameNumber;
    int (*killObject)(struct vrmlObjects *o);
    struct vrmlObjects *(*duplicateObject)(struct vrmlObjects *o);
/*--------------------------------------------------------------------*/
    double width;
    double height;
    double depth;
    long Material;
    long FirstCell;
};

struct vrmlCone{
    long type;
    long nameNumber;
    int (*killObject)(struct vrmlObjects *o);
    struct vrmlObjects *(*duplicateObject)(struct vrmlObjects *o);
/*--------------------------------------------------------------------*/
    long parts;
    double bottomRadius;
    double height;
    long Material;
    long FirstCell;
    long nx;
    long ny;
};

struct vrmlIndexedLineSet{
    long type;
    long nameNumber;
    int (*killObject)(struct vrmlObjects *o);
    struct vrmlObjects *(*duplicateObject)(struct vrmlObjects *o);
/*--------------------------------------------------------------------*/
	long *coorddata;
	long coord;
	long coordmax;
	long *matdata;
	long mat;
	long matmax;
	long *normdata;
	long norm;
	long normmax;
	long *textdata;
	long text;
	long textmax;
};

struct vrmlTextureCoordinate2{
    long type;
    long nameNumber;
    int (*killObject)(struct vrmlObjects *o);
    struct vrmlObjects *(*duplicateObject)(struct vrmlObjects *o);
/*--------------------------------------------------------------------*/
    double *pointdata;
    long points;
    long pointsmax;
};

struct vrmlTexture2Transform{
    long type;
    long nameNumber;
    int (*killObject)(struct vrmlObjects *o);
    struct vrmlObjects *(*duplicateObject)(struct vrmlObjects *o);
/*--------------------------------------------------------------------*/
    double translation[2];
    double rotation[1];
    double scaleFactor[2];
    double center[2];
};

struct vrmlTexture2{
    long type;
    long nameNumber;
    int (*killObject)(struct vrmlObjects *o);
    struct vrmlObjects *(*duplicateObject)(struct vrmlObjects *o);
/*--------------------------------------------------------------------*/
    char *filename;
    long *image;
    long xsize;
    long ysize;
    long componets;
    long wrapS;
    long wrapT;
};

extern struct vrmlList *GetVRML(char *FilesNames,char *directory);
extern struct vrmlList *duplicateVRML(struct vrmlList *list);
extern int freeVrmlList(struct vrmlList *list);


#endif
