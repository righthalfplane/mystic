/*
 *
 * RayTrace Software Package, release 3.0.  May 3, 2006.
 *
 * Mathematics Subpackage (VrMath)
 *
 * Author: Samuel R. Buss
 *
 * Software accompanying the book
 *		3D Computer Graphics: A Mathematical Introduction with OpenGL,
 *		by S. Buss, Cambridge University Press, 2003.
 *
 * Software is "as-is" and carries no warranty.  It may be used without
 *   restriction, but if you modify it, please change the filenames to
 *   prevent confusion between different versions.  Please acknowledge
 *   all use of the software in any publications or products based on it.
 *
 * Bug reports: Sam Buss, sbuss@ucsd.edu.
 * Web page: http://math.ucsd.edu/~sbuss/MathCG
 *
 

// KdTree.h
//
//   A general purpose KdTree which
//		a) holds 3D objects of generic type
//		b) supports ray tracing

// Author: Sam Buss based on work by Sam Buss and Alex Kulungowski
// Contact: sbuss@math.ucsd.edu

*/

#ifndef KDTREE_H
#define KDTREE_H
#include "Tools.h"

struct listStruct{
	long ImageMax;
	long ImageCount;
	long ImageSize;
	void **ImagePointer;
};

#define listStart(p1,p2) listStartR(&p1,sizeof(*p1),&p2)
#define listCheck(p0,p1,p2) listCheckR(p0,&p1,sizeof(*p1),&p2)

extern int listCheckR(long n,void *p1,long size,struct listStruct *t);
extern int listStartR(void *p1,long size,struct listStruct *p2);

#define Sign(x)  (((x) < 0) ? -1 : (((x) == 0) ? 0 : 1) )
#define Max(x,y) ((x > y) ? x : y)

double Lerp(double a,double b,double alpha);

struct Kd_TraverseNodeData {
/*
	friend class KdTree;

public:
	Kd_TraverseNodeData() {}
	Kd_TraverseNodeData( long nodeNum, double minDist, double maxDist );
	void Set( long nodeNum, double minDist, double maxDist  );

	long GetNodeNumber() const { return NodeNumber; }
	double GetMinDist() const { return MinDistance; }
	double GetMaxDist() const { return MaxDistance; }

private:
*/

	
	long NodeNumber;	
	double MinDistance;	
	double MaxDistance;
};

struct stackKD{
	struct Kd_TraverseNodeData td;
	struct dataStackStruct *pop;
};




double UpdateMin(double *x,double *y);
double UpdateMax(double *x,double *y);

enum KD_SplittingAxis {
	KD_SPLIT_X = 0,
	KD_SPLIT_Y = 1,
	KD_SPLIT_Z = 2,
	KD_LEAF = 3
};

enum {
	MacDonaldBooth = 0,					
	MacDonaldBoothModifiedCoefs = 1,	
	DoubleRecurseGS = 2,				
	DoubleRecurseModifiedCoefs=3		
};

typedef struct VectorR3{
	double x,y,z;
}VectorR3;

typedef struct AABBStruct{
	VectorR3 BoxMin;
	VectorR3 BoxMax;
}AABB;

#define AABBIsFlatX(a) (a->BoxMin.x==a->BoxMax.x)
#define AABBIsFlatY(a) (a->BoxMin.y==a->BoxMax.y)
#define AABBIsFlatZ(a) (a->BoxMin.z==a->BoxMax.z)
#define aabbGetMinX(a) (a->BoxMin.x)
#define aabbGetMinY(a) (a->BoxMin.y)
#define aabbGetMinZ(a) (a->BoxMin.z)
#define aabbGetMaxX(a) (a->BoxMax.x)
#define aabbGetMaxY(a) (a->BoxMax.y)
#define aabbGetMaxZ(a) (a->BoxMax.z)

void aaBBEnlargeToEnclose (AABB *i, AABB *aabbToEnclose );
double aaBBSurfaceArea(AABB *i);
void aaBBSetNewAxisMin (AABB *i, int axisNum, double newMin );
void aaBBSetNewAxisMax (AABB *i, int axisNum, double newMax );
int aabbRayEntryExit(AABB *i, VectorR3 *startPos, 
					   int signDirX, int signDirY, int signDirZ, VectorR3 *dirInv,
					   double *entryDist, int *entryFaceId,
					   double *exitDist, int *exitFaceId );
int aabbRayEntryExit2(AABB *i,VectorR3 *startPos, VectorR3 *dir,
						double *entryDist, int *entryFaceId,
						double *exitDist, int *exitFaceId );

typedef void ExtentFunction( long objectNum, AABB *boundingBox);
typedef int ExtentInBoxFunction( long objectNum, AABB *clippingBox, AABB *boundingBox);
typedef double ObjectCostFunction( long objectNum );

enum {
	TT_MAX = 0,
	TT_FLAT = 1,
	TT_MIN = 2
};


typedef struct ExtentTriple {
/*
	void Set( TripleType theType, double value, long objectID );
	void SetMin( double value, long objectID );
	void SetMax( double value, long objectID );
	void SetFlat( double value, long objectID );

	bool IsMin() const { return ExtentType==TT_MIN; }
	bool IsMax() const { return ExtentType==TT_MAX; }
	bool IsFlat() const { return ExtentType==TT_FLAT; }

	friend bool operator<(const ExtentTriple& x, const ExtentTriple y );
	ExtentTriple operator=( const ExtentTriple& );

private:
*/
	int  ExtentType;	
	double ExtentValue;	
	long ObjectID;	

}ExtentTriple;


#define tripleIsMax(t) (t->ExtentType==TT_MAX)
#define tripleIsMin(t) (t->ExtentType==TT_MIN)
#define tripleIsFlat(t) (t->ExtentType==TT_FLAT)


typedef struct  ExtentTripleArrayInfo {
/*
	ExtentTripleArrayInfo() { Init( 0, 0, 0 ); }
	ExtentTripleArrayInfo( ExtentTriple* tripleArray, long numMaxMins, long numFlats )
		{ Init( tripleArray, numMaxMins, numFlats ); }
	void Init ( ExtentTriple* tripleArray, long numMaxMins, long numFlats );

	void SetNumbers( long numMaxMins, long numFlats );
	
	// Add either a min and a max or a flat.
	long AddToEnd ( double min, double max, long objectID );

	void Sort() { ShellSort( TripleArray, NumTriples() ); }

	// You must maintain this condition:
	// EndOfArray will *always* equal TripleArray+NumTriples().

	const long NumObjects () const { return NumMaxMins + NumFlats; }
	const long NumTriples () const { return (NumMaxMins<<1) + NumFlats; }

private:
*/
	ExtentTriple *TripleArray;		
	ExtentTriple *EndOfArray;		

	long NumMaxMins;				
	long NumFlats;		
				
}ExtentTripleArrayInfo;

ExtentTripleArrayInfo ExtentTripleArrayInfo_Init_s(ExtentTriple *,long,long);
void ExtentTripleArrayInfo_Init(ExtentTripleArrayInfo *,ExtentTriple *,long,long);
long ExtentTripleArrayInfo_AddToEnd (ExtentTripleArrayInfo *i, double min, double max, long objectID );
void ExtentTripleArray_SetFlat (ExtentTriple *i, double min, long objectID );
void ExtentTripleArray_SetMin (ExtentTriple *i, double min, long objectID );
void ExtentTripleArray_SetMax (ExtentTriple *i, double max, long objectID );
void ExtentTripleArrayInfo_SetNumbers(ExtentTripleArrayInfo *i, long numMaxMins, long numFlats );
void ExtentTripleArrayInfo_Sort(ExtentTripleArrayInfo *i);
/*
long ExtentTripleArrayInfo_NumObjects(ExtentTripleArrayInfo *i);
long ExtentTripleArrayInfo_NumTriples(ExtentTripleArrayInfo *i);
*/


#define ExtentTripleArrayInfo_NumObjects(i) ((i)->NumMaxMins + (i)->NumFlats)
#define ExtentTripleArrayInfo_NumTriples(i) (((i)->NumMaxMins<<1) + (i)->NumFlats)


typedef struct InternalNodeValues {
	long LeftChildIdx;		
	long RightChildIdx;			
	double SplitValue;			
}InternalNodeValues;

typedef struct LeafNodeValues {
	long* ObjectList;			
	long NumObjects;			
}LeafNodeValues;


typedef struct KdTreeNode {
/*
	bool IsLeaf() const { return (NodeType==KD_LEAF); }
	int SplitAxis() const { assert(NodeType!=KD_LEAF); return (int)NodeType; }
	bool IsRoot() const { return (ParentIdx == -1); }
	bool LeftChildEmpty() const { assert(NodeType!=KD_LEAF); return (Data.Split.LeftChildIdx == -1); }
	bool RightChildEmpty() const { assert(NodeType!=KD_LEAF); return (Data.Split.RightChildIdx == -1); }
	long GetNumObjects() const { assert (NodeType==KD_LEAF); return Data.Leaf.NumObjects; }

	double SplitValue() const { return Data.Split.SplitValue; }

	// Returns pointer to the left child, or Null if the left child is an empty leaf.
	long LeftChildIndex() const { return Data.Split.LeftChildIdx; }

	// Returns pointer to the right child, or Null if the left child is an empty leaf.
	long RightChildIndex() const { return Data.Split.RightChildIdx; }

	// Returns point to the parent, or Null if is the root.
	long ParentIndex() const { return ParentIdx; }

private:
*/

	int NodeType;			/* The type of node */

	long ParentIdx;				/*  Equals -1 if this is root and there is no parent */


	union {
		InternalNodeValues Split;	/* The values for an internal node */
		LeafNodeValues Leaf;		/*  The values for a leaf */
	} Data;

} KdTreeNode;

#define KdTreeNode_SplitValue(t)  (t->Data.Split.SplitValue)
#define KdTreeNode_RightChildIndex(t)  (t->Data.Split.RightChildIdx)
#define KdTreeNode_LeftChildIndex(t)  (t->Data.Split.LeftChildIdx)

typedef int PotentialObjectsListCallback( int numberOfObjects, long *objectNums, double *retStopDistance, void *userdata );

struct  KdTree
{

	int ExtentTripleStorageMultiplier;	
	long NumObjects;
	double TotalObjectCosts;
	double StoppingCostPerRay;
	int UseConstantCost;
	double ObjectConstantCost;
	
	double CF_MinOnAxis;	
	double CF_MaxOnAxis;			
	double CF_FirstAxisLenInv;		
	double CF_OldCost;				
	double CF_TotalNodeObjectCosts;	
	double CF_LogTNOCinv;			
	double CF_Area;					
	double CF_EndArea;				
	double CF_Wrap;					
	double CF_C, CF_D;				
	double CF_ExponentToBeat;		
	
	int SplitAlgorithm;
	
	ExtentFunction *ExtentFunc;	 
	ExtentInBoxFunction *ExtentInBoxFunc;
	ObjectCostFunction *UserCostFunction;
	AABB *ObjectAABBs;
	AABB BoundingBox;
	double BoundingBoxSurfaceArea;
	ExtentTriple *ET_Lists;
	unsigned char* LeftRightStatus;

	KdTreeNode *TreeNodes;
	long TreeBuilt;
	struct listStruct TreeNodesList;
	
	int UseListCallback;	
	void *CallbackFunction;		
	
};


void KdTreeBuildTree(struct  KdTree *t,long numObjects, ExtentFunction *extentFunc, ExtentInBoxFunction *extentInBoxFunc);

void KdTreeBuildSubTree1(struct  KdTree *t, long baseIndex, AABB *aabb, double totalObjectCost,
					ExtentTripleArrayInfo *xExtents, ExtentTripleArrayInfo *yExtents, 
					ExtentTripleArrayInfo *zExtents, long spaceAvailable );

double KdTreeCalcTotalCosts(struct  KdTree *t, ExtentTripleArrayInfo *extents);

void KdTreeCalcBestSplit1(struct  KdTree *t,AABB *aabb,VectorR3 *deltaBox, double totalObjectCost, 
					ExtentTripleArrayInfo *xExtents, ExtentTripleArrayInfo *yExtents, 
					ExtentTripleArrayInfo *zExtents,
					int *splitAxisID, double *splitValue, 
					long *numTriplesToLeft, long *numObjectsToLeft, long *numObjectsToRight, 
					double *costObjectsToLeft, double *costObjectsToRight );
					
int KdTreeCalcBestSplit(struct  KdTree *t, double totalObjectCosts, double costToBeat, 
						ExtentTripleArrayInfo *extents, 
						double minOnAxis, double maxOnAxis, 
						double secondAxisLen, double thirdAxisLen,
						double *retNewBestCost, double *retSplitValue, 
						long *retNumTriplesToLeft, long *retNumObjectsToLeft, long *retNumObjectsToRight,
						double *retCostObjectsToLeft, double *retCostObjectsToRight );
					
void KdTreeUpdateLeftRightCosts(struct  KdTree *t, ExtentTriple *et, long *numObjectsLeft, long *numObjectsRight, 
							   double *costLeft, double *costRight );
					
void KdTreeCopyTriplesForSubtree(struct  KdTree *t, unsigned char leftRightFlag, int axisNumber,
											ExtentTripleArrayInfo *fromExtents, 
											ExtentTripleArrayInfo *toExtents );
											
void KdTreeMakeAabbsForSubtree(struct  KdTree *t, unsigned char leftRightFlag, ExtentTripleArrayInfo *theExtents,
									AABB *theAabb );
									
void KdTreeInitMacdonaldBooth(struct  KdTree *t, double minOnAxis, double maxOnAxis, 
										double secondAxisLen, double thirdAxisLen,
										double costToBeat, double totalObjectCosts );
										
void KdTreeInitDoubleRecurse(struct  KdTree *t, double minOnAxis, double maxOnAxis, 
										double secondAxisLen, double thirdAxisLen,
										double costToBeat, double totalObjectCosts );
					
void KdTreeInitSplitCostFunction(struct  KdTree *t, double minOnAxis, double maxOnAxis, 
											double secondAxisLen, double thirdAxisLen,
											double costToBeat, double totalObjectCosts );
void KdTreeMemoryError2(void);				

long KdTreeNextIndex(struct  KdTree *t);

void KdTreeSetObjectCost (struct  KdTree *t, double cost );

int KdTreeInit(struct  KdTree *t);

void KdTreeSetStoppingCriterion(struct  KdTree *t, long numRays, double numAccesses );

int KdTreeTraverse(struct  KdTree *t, VectorR3 *startPos,VectorR3 *dir, 
				PotentialObjectsListCallback *polcFunc, void *userData, double seekDistance, int obeySeekDistance  );
						
int KdTreeTraverse1(struct  KdTree *t, VectorR3 *startPos, VectorR3 *dir, double seekDistance, int obeySeekDistance, void *userData );

int KdTreeDestroy(struct  KdTree *t);

#endif

