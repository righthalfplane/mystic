


extern  int 		yylinecount ;
extern	int 		Xresolution ;
extern	int 		Yresolution ;
extern	Light		*Lights;
extern	int		nLights ;
extern	Vec		BackgroundColor ;
extern	Surface		* CurrentSurface ;
extern	Object		**Prims; 
extern	long 		maxprims;
extern	int		nPrims ;
extern 	Flt		rayeps ;
extern	char *		Progname ;
extern 	int		maxQueueSize ;
extern 	int		totalQueues ;
extern	int		totalQueueResets ;
extern 	int		tickflag ;
extern  int		nChecked ;
extern 	int		nEnqueued ;

extern 	Flt		minweight ;
extern 	int		maxlevel ;
extern 	int		nRays ;
extern	int		nShadows ;
extern	int		nReflected ;
extern 	int		nRefracted ;
/*
extern 	struct 	Global g;
*/

extern	Object *	MakeCone() ;
extern	Object *	MakeSphere() ;
extern	Object *	MakePatch() ;
extern	Object *	MakePoly() ;
extern	Object *	MakeQuad() ;
extern	Object *	MakeQuadN() ;
extern	Object *	MakeQuadNC() ;
extern	Object *	MakeTri() ;
extern	Object *	MakeTriN() ;
extern	Object *	MakeTriNC() ;
extern	Object *	MakeRing() ;

extern long doStripes(void *,void *,void *);
extern long doImages(void *,void *,void *);
extern long doBumps(void *,void *,void *);

extern 	Flt VecNormalize(Vec vec);
extern	Vec		Slab[] ;
extern	ObjectProcs	NullProcs ;
extern 	Object *	Root ;
