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


// KdTree.cpp
//
//   A general purpose KdTree which
//		a) holds 3D objects of generic type
//		b) supports ray tracing

// Author: Sam Buss based on work by Sam Buss and Alex Kulungowski
// Contact: sbuss@math.ucsd.edu

*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "KdTree.h"
#include "DoubleRecurse.h"

#define STACK_INCREMENT 5000

static int intcmp1(const void *xx,const  void *yy);
void *cMalloc(unsigned long length,int tag);
int WarningBatch(char *buff);
int cFree(char *data);
void *cRealloc(char *p,unsigned long r,int tag);

				
void KdTreeUpdateLeftRightCosts(struct  KdTree *t, ExtentTriple *et, long *numObjectsLeft, long *numObjectsRight, 
							   double *costLeft, double *costRight );
				
double KdTreeCalcTotalCosts(struct  KdTree *t, ExtentTripleArrayInfo *extents);

int KdTreeCalcSplitCost(struct  KdTree *t, double splitValue, double costLeft, double costRight, double* retCost );
				
int KdTreeCalcDoubleRecurseGS(struct  KdTree *t, double splitValue, double costLeft, double costRight, double *retCost );

int  KdTreeCalcMacdonaldBoothModifiedCoefs(struct  KdTree *t, double splitValue, double costLeft, double costRight, double* retCost );

int  KdTreeCalcMacdonaldBooth(struct  KdTree *t, double splitValue, double costLeft, double costRight, double* retCost );

void KdTreeMemoryError();
void KdTreeMemoryError2();
				
int KdTreeTraverse(struct  KdTree *t, VectorR3 *startPos,VectorR3 *dir, 
				PotentialObjectsListCallback *polcFunc, void *userData, double seekDistance, int obeySeekDistance  )
{
	t->CallbackFunction = (void*) polcFunc;
	t->UseListCallback = TRUE;

	return KdTreeTraverse1(t, startPos, dir, seekDistance, obeySeekDistance, userData  );
}


int KdTreeTraverse1(struct  KdTree *t, VectorR3 *startPos, VectorR3 *dir, double seekDistance, int obeySeekDistance, void *userData )
{
	double entryDist, exitDist;
	int entryFaceId, exitFaceId;
	VectorR3 dirInv;
	int signDirX;
	int signDirY;
	int signDirZ;
	KdTreeNode *currentNode;
	int intersectsAABB;
	long currentNodeIndex;			
	double minDistance;					
	double maxDistance;
	int hitParallel;
	double parallelHitMax;
	int stopDistanceActive;
	double stopDistance;
	long nearNodeIdx;
	long farNodeIdx;
	double splitDistance;
	
	struct stackKD *tStack=*(struct stackKD **)userData;
	
	struct Kd_TraverseNodeData *traverseStack= &tStack->td;


	dirInv.x =  0;
	dirInv.y =  0;
	dirInv.z =  0;
	
	signDirX = Sign(dir->x);
	if ( signDirX!=0 ) {
		dirInv.x = 1.0/dir->x;
	}
	signDirY = Sign(dir->y);
	if ( signDirY!=0 ) {
		dirInv.y = 1.0/dir->y;
	}
	signDirZ = Sign(dir->z);
	if ( signDirZ!=0 ) {
		dirInv.z = 1.0/dir->z;
	}

	intersectsAABB = aabbRayEntryExit(&t->BoundingBox, startPos, 
													signDirX, signDirY, signDirZ, &dirInv, 
													&entryDist, &entryFaceId, 
													&exitDist, &exitFaceId );
	if ( !intersectsAABB || exitDist<0.0 ) {
		return FALSE;
	}


	currentNodeIndex = 0;			
	assert ( currentNodeIndex != -1 ) ;				
    currentNode = &t->TreeNodes[currentNodeIndex];
	minDistance = Max(0.0, entryDist);					
	maxDistance = exitDist;
	hitParallel = FALSE;
	parallelHitMax = -1e60;
	stopDistanceActive = obeySeekDistance;
	stopDistance = seekDistance;
	if ( obeySeekDistance && maxDistance>seekDistance ) {
		if ( seekDistance<minDistance ) {
			return FALSE;
		}
		maxDistance = seekDistance;
	}
	assert ( minDistance<=maxDistance );
		
	dataStackPopAll(tStack->pop);
	
	while ( TRUE ) {
		
		if ( !(currentNode->NodeType == KD_LEAF)) {
			int thisSign;
			double thisDirInv;
			double thisStartPt;
			thisSign=0;
			thisDirInv=0;
			thisStartPt=0;
			switch ( currentNode->NodeType ) 
			{
			case KD_SPLIT_X:
				thisSign = signDirX;
				thisDirInv = dirInv.x;
				thisStartPt = startPos->x;
				break;
			case KD_SPLIT_Y:
				thisSign = signDirY;
				thisDirInv = dirInv.y;
				thisStartPt = startPos->y;
				break;
			case KD_SPLIT_Z:
				thisSign = signDirZ;
				thisDirInv = dirInv.z;
				thisStartPt = startPos->z;
				break;
			}
			if ( thisSign==0 ) {
				double thisSplitVal = KdTreeNode_SplitValue(currentNode);
				if ( thisSplitVal<thisStartPt ) {
					currentNodeIndex = KdTreeNode_RightChildIndex(currentNode);
				}
				else if ( thisSplitVal>thisStartPt ) {
					currentNodeIndex = KdTreeNode_LeftChildIndex(currentNode);
				}
				else {
					int leftIdx = KdTreeNode_LeftChildIndex(currentNode);
					int rightIdx = KdTreeNode_RightChildIndex(currentNode);
					if ( leftIdx == -1 ) {
						currentNodeIndex = rightIdx;
					}
					else if ( rightIdx == -1 ) {
						currentNodeIndex = leftIdx;
					}
					else {
						traverseStack=dataStackNext(tStack->pop);
						if(!traverseStack)return FALSE;
						traverseStack->NodeNumber=rightIdx;
						traverseStack->MinDistance=minDistance;
						traverseStack->MaxDistance=maxDistance;
						
						currentNodeIndex = leftIdx;
						hitParallel = TRUE;
						UpdateMax(&maxDistance,&parallelHitMax);
					}
				}
			}
			else {
				if ( thisSign>0 ) {
					nearNodeIdx = KdTreeNode_LeftChildIndex(currentNode);
					farNodeIdx = KdTreeNode_RightChildIndex(currentNode);
				}
				else {
					nearNodeIdx = KdTreeNode_RightChildIndex(currentNode);
					farNodeIdx = KdTreeNode_LeftChildIndex(currentNode);
				}
				splitDistance = (KdTreeNode_SplitValue(currentNode)-thisStartPt)*thisDirInv;
				if ( splitDistance<minDistance ) {
					currentNodeIndex = farNodeIdx;
				}
				else if ( splitDistance>maxDistance ) {
					currentNodeIndex = nearNodeIdx;
				}
				else if ( nearNodeIdx == -1 ) {
						minDistance = splitDistance;
						currentNodeIndex = farNodeIdx;
				}
				else {
					if ( farNodeIdx != -1 ) {
	
						traverseStack=dataStackNext(tStack->pop);
						if(!traverseStack)return FALSE;
						traverseStack->NodeNumber=farNodeIdx;
						traverseStack->MinDistance=splitDistance;
						traverseStack->MaxDistance=maxDistance;
					
						
					}
					maxDistance = splitDistance;
					currentNodeIndex = nearNodeIdx;
				}
			}
			if ( currentNodeIndex != -1 ) {
				currentNode = &t->TreeNodes[currentNodeIndex];
				continue;
			}
		}

		else {
			if (t->UseListCallback ) {
				int stopFlag;
				double newStopDist;
				stopFlag = (*((PotentialObjectsListCallback*)t->CallbackFunction))( 
										currentNode->Data.Leaf.NumObjects, 
										currentNode->Data.Leaf.ObjectList, 
										&newStopDist, userData );
				if ( stopFlag ) {
					stopDistanceActive = TRUE;
					stopDistance = newStopDist;
				}
			}
			else {
				
				/*
				double newStopDist;
				int i = currentNode->Data.Leaf.NumObjects;
				long* objectIdPtr = currentNode->Data.Leaf.ObjectList;
				for ( ; i>0; i-- ) {
				
					if ( (*((PotentialObjectCallback*)t->CallbackFunction))( 
												*objectIdPtr, &newStopDist, userData )  )  
					{
						stopDistanceActive = TRUE;
						stopDistance = newStopDist;
					}
					objectIdPtr++;
				
				}
				*/
			}
		}

		if ( dataStackDepth(tStack->pop) == 0 ) {
			return stopDistanceActive;
		}
		else {
			struct Kd_TraverseNodeData *topNode;
			topNode =dataStackPop(tStack->pop);
			if(!topNode)return FALSE;
			minDistance = topNode->MinDistance;
			if ( stopDistanceActive && minDistance>stopDistance ) {
				if ( !hitParallel || !(minDistance<parallelHitMax) ) {
					return TRUE;
				}
			}
			currentNodeIndex = topNode->NodeNumber;
			currentNode = &t->TreeNodes[currentNodeIndex];
			maxDistance = topNode->MaxDistance; 
		}

	}

}
int KdTreeInit(struct  KdTree *t)
{
	if(!t)return 1;
	t->ExtentTripleStorageMultiplier  = 4;
	KdTreeSetStoppingCriterion(t, 1000000, 4.0 );
	t->SplitAlgorithm = DoubleRecurseModifiedCoefs;
	t->SplitAlgorithm = DoubleRecurseGS;
	t->SplitAlgorithm = MacDonaldBoothModifiedCoefs;
	t->SplitAlgorithm = MacDonaldBooth;
	return 0;
}

void KdTreeBuildTree(struct  KdTree *t,long numObjects, ExtentFunction *extentFunc, ExtentInBoxFunction *extentInBoxFunc)
{
	KdTreeNode *RootNode;
    long spaceAvailable;
	AABB *ObjectAabbPtr;
	
	ExtentTripleArrayInfo XextentList;
	ExtentTripleArrayInfo YextentList;
	ExtentTripleArrayInfo ZextentList;
	
	long i;
	
	assert (t->TreeBuilt == 0);
	t->TreeBuilt=1;
	t->NumObjects = numObjects;
	t->ExtentFunc = extentFunc;
	t->ExtentInBoxFunc = extentInBoxFunc;

	if ( t->UseConstantCost ) {
		t->TotalObjectCosts = t->ObjectConstantCost*t->NumObjects;
	}
	else {
		t->TotalObjectCosts = 0;
		for (i=0; i<t->NumObjects; i++) {
			t->TotalObjectCosts += (*t->UserCostFunction)(  i );
		}
	}

	t->ObjectAABBs = (AABB *)cMalloc( sizeof(AABB)*numObjects,3203);
	if(!t->ObjectAABBs)goto ErrorOut;

	ObjectAabbPtr = t->ObjectAABBs;
	for (i=0; i<numObjects; i++) {
		(*t->ExtentFunc)( i, ObjectAabbPtr);
		ObjectAabbPtr++;
	}

	t->BoundingBox = *t->ObjectAABBs;
	ObjectAabbPtr = t->ObjectAABBs+1;
	for (i=1; i<numObjects; i++) {
		aaBBEnlargeToEnclose(&t->BoundingBox, ObjectAabbPtr);
		ObjectAabbPtr++;
	}
	
	t->BoundingBoxSurfaceArea = aaBBSurfaceArea(&t->BoundingBox);

	t->ET_Lists = (ExtentTriple *)cMalloc(sizeof(ExtentTriple)*(3*2*t->ExtentTripleStorageMultiplier)*t->NumObjects,3203);
	if ( !t->ET_Lists ) {
		goto ErrorOut;
	}
	ExtentTripleArrayInfo_Init(&XextentList, t->ET_Lists, 0, 0 );
	ExtentTripleArrayInfo_Init(&YextentList, t->ET_Lists + (2*t->ExtentTripleStorageMultiplier)*t->NumObjects, 0, 0 );
	ExtentTripleArrayInfo_Init(&ZextentList, t->ET_Lists + (2*2*t->ExtentTripleStorageMultiplier)*t->NumObjects, 0, 0 );
	t->LeftRightStatus = (unsigned char *)cMalloc(sizeof(unsigned char)*t->NumObjects,3203);
	if(!t->LeftRightStatus)goto ErrorOut;

	ObjectAabbPtr = t->ObjectAABBs;
	for ( i=0; i<numObjects; i++ ) {
		ExtentTripleArrayInfo_AddToEnd(&XextentList, ObjectAabbPtr->BoxMin.x, ObjectAabbPtr->BoxMax.x, i );
		ExtentTripleArrayInfo_AddToEnd(&YextentList, ObjectAabbPtr->BoxMin.y, ObjectAabbPtr->BoxMax.y, i );
		ExtentTripleArrayInfo_AddToEnd(&ZextentList, ObjectAabbPtr->BoxMin.z, ObjectAabbPtr->BoxMax.z, i );
		ObjectAabbPtr++;
	}

	spaceAvailable = 2*(t->ExtentTripleStorageMultiplier-1)*t->NumObjects;

	ExtentTripleArrayInfo_Sort(&XextentList);
	ExtentTripleArrayInfo_Sort(&YextentList);
	ExtentTripleArrayInfo_Sort(&ZextentList);

	if(listStart(t->TreeNodes,t->TreeNodesList))goto ErrorOut;
	if(listCheck(0,t->TreeNodes,t->TreeNodesList))goto ErrorOut;
	KdTreeNextIndex(t);
	RootNode = &t->TreeNodes[0];
	RootNode->ParentIdx = -1;
	KdTreeBuildSubTree1 (t, 0, &t->BoundingBox, t->TotalObjectCosts, &XextentList, &YextentList, &ZextentList, spaceAvailable );

ErrorOut:

	if(t->ObjectAABBs)cFree((char *)t->ObjectAABBs);
	t->ObjectAABBs=NULL;
	if(t->LeftRightStatus)cFree((char *)t->LeftRightStatus);
	t->LeftRightStatus=NULL;
	if(t->ET_Lists)cFree((char *)t->ET_Lists);
	t->ET_Lists=NULL;
}

void KdTreeBuildSubTree1(struct  KdTree *t, long baseIndex, AABB *aabb, double totalObjectCost,
					ExtentTripleArrayInfo *xExtents, ExtentTripleArrayInfo *yExtents, 
					ExtentTripleArrayInfo *zExtents, long spaceAvailable )
{

	VectorR3 deltaAABB;
	int splitAxisID;	
	ExtentTripleArrayInfo *splitExtentList;	
	double splitValue;				
	long numTriplesToLeft;			
	long numObjectsToLeft;			
	long numObjectsToRight;			
	double costObjectsToLeft;		
	double costObjectsToRight;		
	KdTreeNode *baseNode;
	KdTreeNode *childNode;
	long numInLeaf;
	long *objectArray;
	ExtentTriple *triple;
	long childIndex;
	long i;
	AABB childAabb;
	ExtentTriple *etPtr;
	long n;
	
	AABB leftChildAabb;
	AABB rightChildAabb;
	AABB *smallerChildAabb;
	AABB *largerChildAabb;
	long leftChildIndex;
	long rightChildIndex;
	KdTreeNode *leftChildNode;
	KdTreeNode *rightChildNode;
	long smallerChildIdx;
	long largerChildIdx;
	unsigned char leftRightFlag;
	long newSpaceAvailable;		
	double smallerTotalCost;	
	double largerTotalCost;		
	long smallerNumObjects;
	long largerNumObjects;
	ExtentTripleArrayInfo newXextents;
	ExtentTripleArrayInfo newYextents;
	ExtentTripleArrayInfo newZextents;
	

	
	
	deltaAABB.x = aabb->BoxMax.x-aabb->BoxMin.x;
	deltaAABB.y = aabb->BoxMax.y-aabb->BoxMin.y;
	deltaAABB.z = aabb->BoxMax.z-aabb->BoxMin.z;
/*
	// Step 1.
	// Try all three axes to find the best split decision
*/
	KdTreeCalcBestSplit1(t, aabb, &deltaAABB, totalObjectCost, xExtents, yExtents, zExtents,
					&splitAxisID, &splitValue, 
					&numTriplesToLeft, &numObjectsToLeft, &numObjectsToRight, 
					&costObjectsToLeft, &costObjectsToRight );
					
	splitExtentList = NULL;
	switch ( splitAxisID ) {
		case KD_LEAF:
			{
/*
				// No splitting occurs
				// Copy object triples into an array
*/
				baseNode = t->TreeNodes+baseIndex;
				baseNode->NodeType = KD_LEAF;
				numInLeaf = ExtentTripleArrayInfo_NumObjects(xExtents);
				assert ( ExtentTripleArrayInfo_NumObjects(xExtents) == numInLeaf && ExtentTripleArrayInfo_NumObjects(xExtents) == numInLeaf );
				baseNode->Data.Leaf.NumObjects = numInLeaf;
				
				objectArray = (long *)cMalloc( sizeof(long)*numInLeaf,3203);
				if(!objectArray)goto ErrorOut;

				baseNode->Data.Leaf.ObjectList = objectArray;
				triple = xExtents->TripleArray;		
				for ( i=0; i<numInLeaf; triple++ ) {
					if ( !( tripleIsMax(triple) ) ) {
						*(objectArray++) = triple->ObjectID;
						i++;
					}
				}
			}
			return;									
		case KD_SPLIT_X:
			splitExtentList = xExtents;
			break;
		case KD_SPLIT_Y:
			splitExtentList = yExtents;
			break;
		case KD_SPLIT_Z:
			splitExtentList = zExtents;
			break;
	}

/*
	// Step 2. Handle splits where one subtree is empty
*/
	if ( numObjectsToLeft==0 || numObjectsToRight==0 ) {
		assert ( numObjectsToLeft!=0 || numObjectsToRight!=0 );
/*
		// One child is empty
*/
		childIndex = KdTreeNextIndex(t);
		baseNode =  &t->TreeNodes[baseIndex];
		childNode =  &t->TreeNodes[childIndex];
		childNode->ParentIdx = baseIndex;
		baseNode->NodeType = splitAxisID;
		baseNode->Data.Split.SplitValue = splitValue;
		childAabb = *aabb;
		if ( numTriplesToLeft==0 ) {
			baseNode->Data.Split.LeftChildIdx = -1;
			baseNode->Data.Split.RightChildIdx = childIndex;
			aaBBSetNewAxisMin(&childAabb, splitAxisID, splitValue );
		}
		else {
			baseNode->Data.Split.LeftChildIdx = childIndex;
			baseNode->Data.Split.RightChildIdx = -1;
			aaBBSetNewAxisMax(&childAabb, splitAxisID, splitValue );
		}
		KdTreeBuildSubTree1(t, childIndex, &childAabb, totalObjectCost,
						   xExtents, yExtents, zExtents, spaceAvailable );
	ErrorOut:
	
		return;
	}
/*
	// Step 3. 
	// Two subtrees must be formed.
	// Decide which objects go left and right - Store info in LeftRightStatus[]
*/
	etPtr = splitExtentList->TripleArray;
	for ( i=0; i<numTriplesToLeft; i++, etPtr++ ) {
		/*  It is on the left, don't know if on right yet, so set as not. */
		t->LeftRightStatus[ etPtr->ObjectID ] = 1;			/* Set first bit, reset second bit */
	}
	n = ExtentTripleArrayInfo_NumTriples(splitExtentList);
	for ( ; i<n; i++, etPtr++ ) {
		if ( etPtr->ExtentType == TT_MAX ) {
			/*  On right side.  Maybe on left side too. */
			t->LeftRightStatus[ etPtr->ObjectID ] |= 2;		/* Set second bit */
		}
		else {
			/* On right side only */
			t->LeftRightStatus[ etPtr->ObjectID ] = 2;			/* Set second bit, reset first bit */
		}
	}

	/*  Step 4.  Build the child AABB's */
	leftChildAabb = *aabb;
	aaBBSetNewAxisMax(&leftChildAabb, splitAxisID, splitValue );
	rightChildAabb = *aabb;
	aaBBSetNewAxisMin(&rightChildAabb, splitAxisID, splitValue );
/*
	// Step 5.
	// Allocate the left and right children
	// Set entries in baseNode for internal node
	// Set all other tree pointers. (Indices)
*/
	leftChildIndex = KdTreeNextIndex(t);		/* Warning: NextIndex() can trigger memory movement */
	rightChildIndex = KdTreeNextIndex(t);
	baseNode = &t->TreeNodes[baseIndex];
	leftChildNode = &t->TreeNodes[leftChildIndex];
	rightChildNode = &t->TreeNodes[rightChildIndex];
	baseNode->NodeType = splitAxisID;
	baseNode->Data.Split.LeftChildIdx = leftChildIndex;
	baseNode->Data.Split.RightChildIdx = rightChildIndex;
	baseNode->Data.Split.SplitValue = splitValue;
	leftChildNode->ParentIdx = baseIndex;
	rightChildNode->ParentIdx = baseIndex;
	if (  numObjectsToLeft<numObjectsToRight ) {
		leftRightFlag = 1;
		newSpaceAvailable = spaceAvailable-2*numObjectsToLeft;
		smallerChildAabb = &leftChildAabb;
		largerChildAabb = &rightChildAabb;
		smallerChildIdx = leftChildIndex;
		largerChildIdx = rightChildIndex;
		smallerTotalCost = costObjectsToLeft;
		largerTotalCost = costObjectsToRight;
		smallerNumObjects = numObjectsToLeft;
		largerNumObjects = numObjectsToRight;
	}
	else {
		leftRightFlag = 2;
		newSpaceAvailable = spaceAvailable-2*numObjectsToRight;
		smallerChildAabb = &rightChildAabb;
		largerChildAabb = &leftChildAabb;
		smallerChildIdx = rightChildIndex;
		largerChildIdx = leftChildIndex;
		smallerTotalCost = costObjectsToRight;
		largerTotalCost = costObjectsToLeft;
		smallerNumObjects = numObjectsToRight;
		largerNumObjects = numObjectsToLeft;
	}
	if ( newSpaceAvailable<0 ) {
		KdTreeMemoryError2();
	}
	/* Step 7.  Allocate space for the smaller subtree */
	ExtentTripleArrayInfo_Init(&newXextents, xExtents->EndOfArray, 0, 0 );
	ExtentTripleArrayInfo_Init(&newYextents, yExtents->EndOfArray, 0, 0 );
	ExtentTripleArrayInfo_Init(&newZextents, zExtents->EndOfArray, 0, 0 );
	
	/* Create the AABB's for the smaller subtree */
	KdTreeMakeAabbsForSubtree(t, leftRightFlag, xExtents, smallerChildAabb );
	/*  Copy the extent triples for the smaller subtree */
	KdTreeCopyTriplesForSubtree(t, leftRightFlag, 0, xExtents, &newXextents ); 
	KdTreeCopyTriplesForSubtree(t, leftRightFlag, 1, yExtents, &newYextents ); 
	KdTreeCopyTriplesForSubtree(t, leftRightFlag, 2, zExtents, &newZextents ); 
	/* Recalculate total cost if necessary, i.e., if some objects go missing */
	if ( ExtentTripleArrayInfo_NumObjects(&newXextents)!=smallerNumObjects ) {
		smallerTotalCost = KdTreeCalcTotalCosts(t, &newXextents );
	}
	
	/* Step 8. */
	leftRightFlag = 3-leftRightFlag;
	/* Create the AABB's for the larger subtree */
	KdTreeMakeAabbsForSubtree(t, leftRightFlag, xExtents, largerChildAabb );
/*
	// Copy the extent triples for the larger subtree
*/
	KdTreeCopyTriplesForSubtree(t, leftRightFlag, 0, xExtents, xExtents ); 
	KdTreeCopyTriplesForSubtree(t, leftRightFlag, 1, yExtents, yExtents ); 
	KdTreeCopyTriplesForSubtree(t, leftRightFlag, 2, zExtents, zExtents ); 
	leftRightFlag = 3-leftRightFlag;
/*
	// Recalculate total cost if necessary, i.e., if some objects go missing
*/
	if ( ExtentTripleArrayInfo_NumObjects(xExtents)!=largerNumObjects ) {
		largerTotalCost = KdTreeCalcTotalCosts(t, xExtents );
	}
/*
	// Step 9.
	// Invoke BuildSubTree recursively for the two subtrees
*/
	KdTreeBuildSubTree1(t, smallerChildIdx, smallerChildAabb, smallerTotalCost,
					&newXextents, &newYextents, &newZextents, newSpaceAvailable);
	KdTreeBuildSubTree1(t, largerChildIdx, largerChildAabb, largerTotalCost,
					xExtents, yExtents, zExtents, spaceAvailable );

}


void KdTreeCalcBestSplit1(struct  KdTree *t,AABB *aabb,VectorR3 *deltaBox, double totalObjectCost, 
					ExtentTripleArrayInfo *xExtents, ExtentTripleArrayInfo *yExtents, 
					ExtentTripleArrayInfo *zExtents,
					int *splitAxisID, double *splitValue, 
					long *numTriplesToLeft, long *numObjectsToLeft, long *numObjectsToRight, 
					double *costObjectsToLeft, double *costObjectsToRight )
{
	double surfaceArea;
	double minImprove;
	double costToBeat;
	int foundBetter;
	double bestCostSoFar; 


	assert( ExtentTripleArrayInfo_NumObjects(xExtents) == ExtentTripleArrayInfo_NumObjects(yExtents) );
	assert( ExtentTripleArrayInfo_NumObjects(yExtents) == ExtentTripleArrayInfo_NumObjects(zExtents) );

	*splitAxisID = KD_LEAF;	

	surfaceArea = 2.0*(deltaBox->x*deltaBox->y + deltaBox->x*deltaBox->z + deltaBox->y*deltaBox->z);
	minImprove = (t->BoundingBoxSurfaceArea/surfaceArea)*t->StoppingCostPerRay;
	costToBeat = totalObjectCost - minImprove;
	if ( costToBeat<=1.0+1.0e-7 ) {
		return;						
	}


	foundBetter = FALSE;
	bestCostSoFar = totalObjectCost; 
	if ( KdTreeCalcBestSplit(t, totalObjectCost, costToBeat, xExtents, 
						aabbGetMinX(aabb), aabbGetMaxX(aabb), deltaBox->y, deltaBox->z,
						&bestCostSoFar, splitValue, 
						numTriplesToLeft, numObjectsToLeft, numObjectsToRight,
						costObjectsToLeft, costObjectsToRight ) )
	{
		foundBetter = TRUE;
		*splitAxisID = KD_SPLIT_X;
		costToBeat = bestCostSoFar;
	}
	if ( KdTreeCalcBestSplit(t, totalObjectCost, costToBeat, yExtents, 
						aabbGetMinY(aabb), aabbGetMaxY(aabb), deltaBox->x, deltaBox->z,
						&bestCostSoFar, splitValue, 
						numTriplesToLeft, numObjectsToLeft, numObjectsToRight,
						costObjectsToLeft, costObjectsToRight ) )
	{
		foundBetter = TRUE;
		*splitAxisID = KD_SPLIT_Y;
		costToBeat = bestCostSoFar;
	}
	if ( KdTreeCalcBestSplit(t, totalObjectCost, costToBeat, zExtents, 
						aabbGetMinZ(aabb), aabbGetMaxZ(aabb), deltaBox->x, deltaBox->y,
						&bestCostSoFar, splitValue, 
						numTriplesToLeft, numObjectsToLeft, numObjectsToRight,
						costObjectsToLeft, costObjectsToRight ) )
	{
		foundBetter = TRUE;
		*splitAxisID = KD_SPLIT_Z;
	}

}

/*
// Returns TRUE if a new better split is found on the axis.
// The other return values will NOT be changed unless "TRUE" is returned.
*/
int KdTreeCalcBestSplit(struct  KdTree *t, double totalObjectCosts, double costToBeat, 
						ExtentTripleArrayInfo *extents, 
						double minOnAxis, double maxOnAxis, 
						double secondAxisLen, double thirdAxisLen,
						double *retNewBestCost, double *retSplitValue, 
						long *retNumTriplesToLeft, long *retNumObjectsToLeft, long *retNumObjectsToRight,
						double *retCostObjectsToLeft, double *retCostObjectsToRight )
{

	int foundBetter;
	double bestCost;	
	long numTriplesLeft;				
	long numTriples; 
	long numObjectsLeft;					
	long numObjectsRight;	
	double costLeft;					
	double costRight;		
	ExtentTriple *etPtr;
	int inFirstHalf;			
	int inFirstHalfCosts;			
	double midPoint;

	double thisSplitValue;
	int sameSplitValue;
	int thisType;





	if ( minOnAxis>=maxOnAxis ) {
		return FALSE;		
	}

	KdTreeInitSplitCostFunction(t, minOnAxis, maxOnAxis, secondAxisLen, thirdAxisLen,
							costToBeat, totalObjectCosts );

	foundBetter = FALSE;
	bestCost = costToBeat;			
	numTriplesLeft = 0;				
	numTriples = ExtentTripleArrayInfo_NumTriples(extents); 
	numObjectsLeft = 0;					
	numObjectsRight = ExtentTripleArrayInfo_NumObjects(extents);	
	costLeft = 0.0;					
	costRight = totalObjectCosts;		
	etPtr = extents->TripleArray;
	inFirstHalf = TRUE;			
	inFirstHalfCosts = TRUE;			
	midPoint = 0.5*(minOnAxis+maxOnAxis);
	while ( numTriplesLeft<numTriples ) {
		
		thisSplitValue = etPtr->ExtentValue;
		sameSplitValue = TRUE;
		thisType = etPtr->ExtentType;
		if ( inFirstHalf && thisSplitValue>midPoint ) {
			inFirstHalf = FALSE;
		}

		
		while ( numTriplesLeft<numTriples ) {
			if ( !( ((thisType==TT_MAX 
						|| (thisType==TT_FLAT && inFirstHalf)))
					&& sameSplitValue ) )
			{
				break;
			}
            KdTreeUpdateLeftRightCosts(t, etPtr, &numObjectsLeft, &numObjectsRight, &costLeft, &costRight );
			etPtr++;
			numTriplesLeft++;
			thisType = etPtr->ExtentType;
			sameSplitValue = (etPtr->ExtentValue <= thisSplitValue);
		}
/*

		// Ready to call the cost function
		// If the cost function gives better value, save everything appropriately
*/
		if ( KdTreeCalcSplitCost(t, thisSplitValue, costLeft, costRight, &bestCost ) ) {
			foundBetter = TRUE;
			*retNewBestCost = bestCost;
			*retSplitValue = thisSplitValue;
			*retNumTriplesToLeft = numTriplesLeft;
			*retNumObjectsToLeft = numObjectsLeft;
			*retNumObjectsToRight = numObjectsRight;
			*retCostObjectsToLeft = costLeft;
			*retCostObjectsToRight = costRight;
		}
/*
		// Skip past all TT_MIN's, and TT_FLAT's in second half, with this split value
*/
		while ( numTriplesLeft<numTriples ) {
			if ( !( sameSplitValue &&  
					(thisType==TT_MIN 
					|| (thisType==TT_FLAT && !inFirstHalf)) ) ) 
			{
				break;
			}
/*
			// Move rightward a triple
*/
			KdTreeUpdateLeftRightCosts(t, etPtr, &numObjectsLeft, &numObjectsRight, &costLeft, &costRight );
			etPtr++;
			numTriplesLeft++;
			thisType = etPtr->ExtentType;
			sameSplitValue = (etPtr->ExtentValue <= thisSplitValue);
		}

	}

	return foundBetter;
}

void KdTreeUpdateLeftRightCosts(struct  KdTree *t, ExtentTriple *et, long *numObjectsLeft, long *numObjectsRight, 
							   double *costLeft, double *costRight )
{
	double cost;
	if ( t->UseConstantCost ) {
		cost =  t->ObjectConstantCost;
	}
	else {
		cost = ( *t->UserCostFunction)(et->ObjectID);
	}

	switch ( et->ExtentType ) {
	case TT_MAX:
		(*numObjectsRight) --;
		(*costRight) -= cost;
		break;
	case TT_FLAT:
		(*numObjectsRight)--;
		(*costRight) -= cost;
		(*numObjectsLeft)++;
		(*costLeft) += cost;
		break;
	case TT_MIN:
		(*numObjectsLeft)++;
		(*costLeft) += cost;
		break;
	}
}

struct tt{
	long currentNodeIndex;
};

int KdTreeDestroy(struct  KdTree *t)
{
	
	struct pushpopStruct *pop;
	long currentNodeIndex;
	struct tt IdxStack;
	
	
	if(!t || !t->TreeNodes)return 1;
	
	
	pop=pushpopStart(&IdxStack);
	if(!pop)goto ErrorOut;		
	
	currentNodeIndex = 0;
	while ( TRUE ) {
		if ( currentNodeIndex != -1 ) {
			KdTreeNode *currentNode = &t->TreeNodes[currentNodeIndex];
			if (currentNode->NodeType == KD_LEAF){
				cFree((char *) currentNode->Data.Leaf.ObjectList);
			} else {
				if ( !(currentNode->Data.Split.LeftChildIdx == -1) ) {
						IdxStack.currentNodeIndex=currentNode->Data.Split.LeftChildIdx;
						pushpopPush(&IdxStack,pop);
				}
				currentNodeIndex = currentNode->Data.Split.RightChildIdx;
				continue;
			}
		}
		if(pushpopPop(&IdxStack,pop))break;
		currentNodeIndex = IdxStack.currentNodeIndex;
	}
	
	if(t->TreeNodes)cFree((char *)t->TreeNodes);
	t->TreeNodes=NULL;
	
ErrorOut:

	pushpopEND(pop);
	
	return 0;
}

#ifdef JUNK02
// Destructor


/***********************************************************************************************
 * Tree traversal functions.
 ***********************************************************************************************/

// Traverse: Simple traversal scheme.
//	 startPos - beginning of the ray.
//	 dir - direction of the ray.
//   Returns "TRUE" if traversal aborted by the callback function returning "TRUE"

/*
bool KdTree::Traverse( const VectorR3& startPos, const VectorR3& dir, 
				PotentialObjectCallback* pocFunc, void *userData, double seekDistance, bool obeySeekDistance  )
{
	CallbackFunction = (void*) pocFunc;
	UseListCallback = false;

	return Traverse( startPos, dir, seekDistance, obeySeekDistance, userData );
}
*/


#endif

void KdTreeMakeAabbsForSubtree(struct  KdTree *t, unsigned char leftRightFlag,ExtentTripleArrayInfo *theExtents,
									AABB *theAabb)
{
	ExtentTriple *etPtr = theExtents->TripleArray;
	long i;
	long n = ExtentTripleArrayInfo_NumTriples(theExtents);
	int stillIn;
	int flatX;
	int flatY;
	int flatZ;
	
	for ( i=0; i<n; i++, etPtr++ ) {
		long objectID = etPtr->ObjectID;
		if ( (t->LeftRightStatus[ objectID ] & leftRightFlag) != 0 ) {
			/* Don't bother if a Max on the left, or a Min on the right. */
			/*		In these cases, the extent will be computed anyway */
			if ( !((etPtr->ExtentType==(TT_MIN) && leftRightFlag==2)
				|| (etPtr->ExtentType==(TT_MAX) && leftRightFlag==1)) )
			{
				assert (0<=objectID && objectID<t->NumObjects);
				stillIn = (*t->ExtentInBoxFunc)(objectID, theAabb, &t->ObjectAABBs[objectID]);
				flatX = AABBIsFlatX((&t->ObjectAABBs[objectID]));
				flatY = AABBIsFlatY((&t->ObjectAABBs[objectID]));
				flatZ = AABBIsFlatZ((&t->ObjectAABBs[objectID]));
				if ( !stillIn ||(flatX&&flatY) || (flatY&&flatZ) || (flatX&&flatZ) ) {
					/*  Remove from being in this subtree (bitwise OR with complement of leftRightFlag) */
					t->LeftRightStatus[objectID] &= ~leftRightFlag;
				}
			}
		}
	}
}



void KdTreeCopyTriplesForSubtree(struct  KdTree *t, unsigned char leftRightFlag, int axisNumber,
											ExtentTripleArrayInfo *fromExtents, 
											ExtentTripleArrayInfo *toExtents )
{
	ExtentTriple *fromET = fromExtents->TripleArray;
	ExtentTriple *toET = toExtents->TripleArray;
	long n = ExtentTripleArrayInfo_NumTriples(fromExtents);			/* Number of "from" items left */
	long iF = 0;								/* Number of "to" flats created */
	long iM = 0;								/* Number of "to" max/mins created */
	for ( ; n>0; n--, fromET++ ) {
		long objectID = fromET->ObjectID;
		assert ( 0<=objectID && objectID<t->NumObjects );
		if ( t->LeftRightStatus[objectID] & leftRightFlag ) {
			toET->ObjectID = objectID;
			toET->ExtentType = fromET->ExtentType;
			switch ( fromET->ExtentType ) 
			{
			case TT_MIN:
				{
					AABB *theAABB = &t->ObjectAABBs[objectID];
					double newMinExtent = ((double *)(&theAABB->BoxMin))[axisNumber];
					double newMaxExtent = ((double *)(&theAABB->BoxMax))[axisNumber];
					toET->ExtentValue = newMinExtent;
					assert ( newMinExtent<=newMaxExtent );
					if ( newMinExtent < newMaxExtent ) {
						iM++;
					}
					else {
						toET->ExtentType = TT_FLAT;
						iF++;
					}
				}
				toET++;
				break;
			case TT_FLAT:
				toET->ExtentValue = fromET->ExtentValue;
				iF++;
				toET++;
				break;
			case TT_MAX:
				{
					const AABB *theAABB = &t->ObjectAABBs[objectID];
					double newMinExtent = ((double *)(&theAABB->BoxMin))[axisNumber];
					double newMaxExtent = ((double *)(&theAABB->BoxMax))[axisNumber];
					toET->ExtentValue = newMaxExtent;
					assert ( newMinExtent<=newMaxExtent );
					if ( newMinExtent < newMaxExtent ) {
						iM++;
						toET++;
					}
					/* If newMinExtent>=newMaxExtent, then this was changed to a flat already. */
				}
				break;
			default:
				assert(0);
			}
		}
	}
	assert ( (iM&0x01) == 0 );
	ExtentTripleArrayInfo_SetNumbers(toExtents, iM>>1, iF);
	
	/* Now sort the new array of triples */
	ExtentTripleArrayInfo_Sort(toExtents);

}



double KdTreeCalcTotalCosts(struct  KdTree *t, ExtentTripleArrayInfo *extents)
{
	long n = ExtentTripleArrayInfo_NumObjects(extents);
	double totalCosts;
	ExtentTriple *etPtr;
	long i;


	if ( t->UseConstantCost ) {
		return t->ObjectConstantCost*n;
	}
	else {
		totalCosts = 0;
		etPtr = extents->TripleArray;
		for ( i=0; i<n; i++, etPtr++ ) {
			if ( etPtr->ExtentType != TT_MAX ) {
				totalCosts += (*t->UserCostFunction)(  etPtr->ObjectID );
			}
		}
		return totalCosts;
	}
}



void KdTreeInitSplitCostFunction(struct  KdTree *t, double minOnAxis, double maxOnAxis, 
											double secondAxisLen, double thirdAxisLen,
											double costToBeat, double totalObjectCosts )
{
	switch ( t->SplitAlgorithm ) {
	case MacDonaldBooth:
	case MacDonaldBoothModifiedCoefs:
		/* MacDonald-Booth method */
		KdTreeInitMacdonaldBooth(t, minOnAxis, maxOnAxis, secondAxisLen, thirdAxisLen,
							costToBeat,totalObjectCosts);
		break;
	case DoubleRecurseGS:
	case DoubleRecurseModifiedCoefs:
		KdTreeInitDoubleRecurse(t, minOnAxis, maxOnAxis, secondAxisLen, thirdAxisLen,
							 costToBeat, totalObjectCosts );
		break;
	}
}



void KdTreeInitMacdonaldBooth(struct  KdTree *t, double minOnAxis, double maxOnAxis, 
										double secondAxisLen, double thirdAxisLen,
										double costToBeat, double totalObjectCosts )
{
	t->CF_MinOnAxis = minOnAxis;
	t->CF_MaxOnAxis = maxOnAxis;
	t->CF_FirstAxisLenInv = 1.0/(maxOnAxis-minOnAxis);
	t->CF_OldCost = costToBeat;
	t->CF_TotalNodeObjectCosts = totalObjectCosts;
	t->CF_EndArea = secondAxisLen*thirdAxisLen;
	t->CF_Wrap = 2.0*(maxOnAxis-minOnAxis)*(secondAxisLen+thirdAxisLen);
	t->CF_Area = 2.0*t->CF_EndArea +t->CF_Wrap;
}



void KdTreeInitDoubleRecurse(struct  KdTree *t, double minOnAxis, double maxOnAxis, 
										double secondAxisLen, double thirdAxisLen,
										double costToBeat, double totalObjectCosts )
{
	t->CF_MinOnAxis = minOnAxis;
	t->CF_MaxOnAxis = maxOnAxis;
	t->CF_FirstAxisLenInv = 1.0/(maxOnAxis-minOnAxis);
	t->CF_OldCost = costToBeat;
	t->CF_TotalNodeObjectCosts = totalObjectCosts;
	t->CF_LogTNOCinv = 1.0/log(t->CF_TotalNodeObjectCosts);
	t->CF_EndArea = secondAxisLen*thirdAxisLen;
	t->CF_Wrap = 2.0*(maxOnAxis-minOnAxis)*(secondAxisLen+thirdAxisLen);
	t->CF_Area = 2.0*t->CF_EndArea + t->CF_Wrap;

	/* Calculate double recurse cost exponent to beat */
	if ( t->CF_EndArea > 1.0e-14*t->CF_Area ) {
		t->CF_D = -t->CF_Area/(2.0*t->CF_EndArea);
		t->CF_C = 1.0 - t->CF_D;
		t->CF_ExponentToBeat = log((costToBeat-t->CF_D)/t->CF_C) * t->CF_LogTNOCinv;
		if(0>t->CF_ExponentToBeat || t->CF_ExponentToBeat>1.0){
		   t->CF_ExponentToBeat=t->CF_ExponentToBeat;
		}
		assert ( 0<t->CF_ExponentToBeat && t->CF_ExponentToBeat<1.0 );
	}
	else {
		t->CF_EndArea = 0.0;		/* End area is small enough to treat as being exactly zero */
	}
}



int KdTreeCalcSplitCost(struct  KdTree *t, double splitValue, double costLeft, double costRight, double* retCost )
{
	switch ( t->SplitAlgorithm ) {
	case MacDonaldBooth:
		/*  MacDonald-Booth method */
		return KdTreeCalcMacdonaldBooth(t, splitValue, costLeft, costRight, retCost );
	case MacDonaldBoothModifiedCoefs:
		/*  MacDonald-Booth method with modified coefs */
		return KdTreeCalcMacdonaldBoothModifiedCoefs(t, splitValue, costLeft, costRight, retCost );
	case DoubleRecurseGS:
	case DoubleRecurseModifiedCoefs:
		/* Buss double-recurse method */
		return KdTreeCalcDoubleRecurseGS(t, splitValue, costLeft, costRight, retCost );
		break;
	default:
		assert(0);
		return FALSE;
	}
}


int  KdTreeCalcMacdonaldBooth(struct  KdTree *t, double splitValue, double costLeft, double costRight, double* retCost )
{
	double gamma = (splitValue-t->CF_MinOnAxis)*t->CF_FirstAxisLenInv;
	double surfaceAreaLeft = 2.0*t->CF_EndArea + gamma*t->CF_Wrap;
	double surfaceAreaRight = 2.0*t->CF_EndArea + (1.0-gamma)*t->CF_Wrap;
	double newCost = 1.0 + (surfaceAreaLeft*costLeft + surfaceAreaRight*costRight)/t->CF_Area;
	if ( newCost<t->CF_OldCost ) {
		*retCost = newCost;
		t->CF_OldCost = newCost;
		return TRUE;
	}
	else {
		return FALSE;
	}
}



int  KdTreeCalcMacdonaldBoothModifiedCoefs(struct  KdTree *t, double splitValue, double costLeft, double costRight, double* retCost )
{
	double gamma = (splitValue-t->CF_MinOnAxis)*t->CF_FirstAxisLenInv;
	double surfaceAreaLeft = 2.0*t->CF_EndArea + gamma*t->CF_Wrap;
	double surfaceAreaRight = 2.0*t->CF_EndArea + (1.0-gamma)*t->CF_Wrap;
	double modFade = t->CF_TotalNodeObjectCosts/t->TotalObjectCosts;
	double fracLeft = costLeft/(costLeft+costRight);
	double fracRight = 1.0-fracLeft;
	double newCost = 1.0;
	newCost += (1.0-modFade)*(surfaceAreaLeft*costLeft + surfaceAreaRight*costRight)/t->CF_Area;
	newCost += modFade*((fracLeft+fracRight*t->CF_EndArea/surfaceAreaRight)*costLeft
						+(fracRight+fracLeft*t->CF_EndArea/surfaceAreaLeft)*costRight);

	if ( newCost<t->CF_OldCost ) {
		*retCost = newCost;
		t->CF_OldCost = newCost;
		return TRUE;
	}
	else {
		return FALSE;
	}
}
int KdTreeCalcDoubleRecurseGS(struct  KdTree *t, double splitValue, double costLeft, double costRight, double *retCost )
{
	double gamma;
	double surfaceAreaLeft;
	double surfaceAreaRight;
	double A;
	double B;
	double alpha;
	double beta;
	double modFade;
	double fracLeft;
	double fracRight;
	double newCost;
	double C, D;
	double newExponent;
	int betterCost;
	
	gamma = (splitValue-t->CF_MinOnAxis)*t->CF_FirstAxisLenInv;
	surfaceAreaLeft = 2.0*t->CF_EndArea + gamma*t->CF_Wrap;
	surfaceAreaRight = 2.0*t->CF_EndArea + (1.0-gamma)*t->CF_Wrap;
	A = surfaceAreaLeft/t->CF_Area;
	B = surfaceAreaRight/t->CF_Area;
	alpha = costLeft/t->CF_TotalNodeObjectCosts;
	alpha = costLeft/t->CF_TotalNodeObjectCosts;
	beta = costRight/t->CF_TotalNodeObjectCosts;
	
	if ( t->SplitAlgorithm==DoubleRecurseModifiedCoefs) {
		modFade = t->CF_TotalNodeObjectCosts/t->TotalObjectCosts;
		fracLeft = costLeft/(costLeft+costRight);
		fracRight = 1.0-fracLeft;
	    A = Lerp(A, fracLeft+fracRight*t->CF_EndArea/surfaceAreaRight, modFade);
	    B = Lerp(B, fracRight+fracLeft*t->CF_EndArea/surfaceAreaLeft, modFade);
	}

	if ( costLeft==0.0 || costRight==0.0 ) {
		if ( costLeft==0.0 && B<1.0 ) {
			newCost = 1.0/(1.0-B);
		}
		else if ( costRight==0.0 && A<1.0 ) {
			newCost = 1.0/(1.0-A);
		}
		else {
			return FALSE;
		}
		if ( newCost<t->CF_OldCost ) {
			*retCost = newCost;
			t->CF_OldCost = newCost;
			if ( t->CF_EndArea!=0.0 ) {
				t->CF_ExponentToBeat = log( (newCost-t->CF_D)/t->CF_C ) * t->CF_LogTNOCinv;
				assert ( 0<t->CF_ExponentToBeat && t->CF_ExponentToBeat<1.0 );
			}
			return TRUE;
		}
		return FALSE;
	} 

	if ( t->CF_EndArea==0.0 ) {
		if ( alpha!=0.0 && beta!=0.0 ) {
			double newCost = 1.0 - log(t->CF_TotalNodeObjectCosts)/(A*log(alpha)+B*log(beta));
			if ( newCost<t->CF_OldCost ) {
				*retCost = newCost;
				t->CF_OldCost = newCost;
				return TRUE;
			}
		}
		return FALSE;
	}

	betterCost = FindDoubleRecurseSoln2( A, B, alpha, beta, &C, &newExponent, &D, t->CF_ExponentToBeat );
	if ( betterCost ) {
		t->CF_ExponentToBeat = newExponent;
		assert ( 0<t->CF_ExponentToBeat && t->CF_ExponentToBeat<1.0 );
		t->CF_OldCost = C * pow(t->CF_TotalNodeObjectCosts,newExponent) + D;
		*retCost = t->CF_OldCost;
		return TRUE;
	}

	return FALSE;
}
double Lerp(double a,double b,double alpha)
{
	double  beta = 1.0-alpha;
	double c;
	if ( beta>alpha ) {
		c = b;
		c *= alpha/beta;
		c += a;
		c *= beta;
	}else {
		c = a;
		c *= beta/alpha;
		c += b;
		c *= alpha;
	}
	return  c;
}
void KdTreeSetObjectCost (struct  KdTree *t, double cost )
{
	t->UseConstantCost = TRUE;
	t->ObjectConstantCost = cost;
	assert (cost>0.0);
}


void KdTreeMemoryError(void);

void KdTreeMemoryError()
{
	assert(0);
	fprintf(stderr,"KdTree construction: Failed to allocate memory.\n");
	exit(0);
}


void KdTreeMemoryError2()
{
	assert(0);
	fprintf(stderr,"KdTree: Memory overflow. Need to increase storage multiplier.\n");
	exit(0);
}
/*
long ExtentTripleArrayInfo_NumObjects(ExtentTripleArrayInfo *i)
{
	return i->NumMaxMins + i->NumFlats;
}
long ExtentTripleArrayInfo_NumTriples(ExtentTripleArrayInfo *i)
{
	return (i->NumMaxMins<<1) + i->NumFlats;
}
*/
void ExtentTripleArrayInfo_Init(ExtentTripleArrayInfo *i,ExtentTriple *tripleArray,long numMaxMins, long numFlats)
{
	i->TripleArray = tripleArray;
	i->NumMaxMins = numMaxMins;
	i->NumFlats = numFlats;
	i->EndOfArray = i->TripleArray + (i->NumMaxMins<<1) + i->NumFlats;
}
long ExtentTripleArrayInfo_AddToEnd (ExtentTripleArrayInfo *i, double min, double max, long objectID )
{
	if ( min==max ) {
		
		ExtentTripleArray_SetFlat (i->EndOfArray++, min,objectID);
		i->NumFlats++;
		return 1L;
	}
	else {
		
		ExtentTripleArray_SetMin (i->EndOfArray++, min,objectID);
		ExtentTripleArray_SetMax (i->EndOfArray++, max,objectID);
		i->NumMaxMins++;
		return 2L;
	}

}
long KdTreeNextIndex(struct  KdTree *t)
{
	 long i;
	 
	 i = t->TreeNodesList.ImageCount;	 
	 
	 if(listCheck(i+1,t->TreeNodes,t->TreeNodesList))goto ErrorOut;
	 
	 t->TreeNodesList.ImageCount++;
 
	 return i;
ErrorOut:
	return -1L;
}
void KdTreeSetStoppingCriterion(struct  KdTree *t, long numRays, double numAccesses )
{
	t->StoppingCostPerRay = numAccesses/(double)numRays;
}

void ExtentTripleArrayInfo_SetNumbers(ExtentTripleArrayInfo *i, long numMaxMins, long numFlats )
{
	i->NumMaxMins = numMaxMins;
	i->NumFlats = numFlats;
	i->EndOfArray = i->TripleArray+(i->NumFlats+(i->NumMaxMins<<1));
	return;
}

void ExtentTripleArrayInfo_Sort(ExtentTripleArrayInfo *i)
{
	
	qsort( i->TripleArray, (i->NumMaxMins<<1) + i->NumFlats, sizeof(struct ExtentTriple),intcmp1);
}
static int intcmp1(const void *xx,const  void *yy)
{
	register ExtentTriple *x=(ExtentTriple *)xx;
	register ExtentTriple *y=(ExtentTriple *)yy;
	
	if ( x->ExtentValue < y->ExtentValue ) {
		return -1;
	}else if ( x->ExtentValue == y->ExtentValue ) {
		if ( x->ExtentType < y->ExtentType ) {
			return -1;
		}else if ( x->ExtentType == y->ExtentType) { 
		    if(x->ObjectID < y->ObjectID)return -1;
		    if(x->ObjectID == y->ObjectID)return 0;
			return 1;
		}else{
		    return 1;
		}
	}else{
	    return 1;
	}

}
void ExtentTripleArray_SetFlat (ExtentTriple *i, double value, long objectID )
{
	i->ExtentType = TT_FLAT;
	i->ExtentValue = value;
	i->ObjectID = objectID;

}
void ExtentTripleArray_SetMin (ExtentTriple *i, double value, long objectID )
{
	i->ExtentType = TT_MIN;
	i->ExtentValue = value;
	i->ObjectID = objectID;

}


void ExtentTripleArray_SetMax (ExtentTriple *i, double value, long objectID )
{
	i->ExtentType = TT_MAX;
	i->ExtentValue = value;
	i->ObjectID = objectID;

}
void aaBBSetNewAxisMin (AABB *i, int axisNum, double newMin )
{
	switch (axisNum) {
	case 0:
		i->BoxMin.x = newMin;
		break;
	case 1:
		i->BoxMin.y = newMin;
		break;
	case 2:
		i->BoxMin.z = newMin;
		break;
	}
}
void aaBBSetNewAxisMax (AABB *i, int axisNum, double newMax )
{
	switch (axisNum) {
	case 0:
		i->BoxMax.x = newMax;
		break;
	case 1:
		i->BoxMax.y = newMax;
		break;
	case 2:
		i->BoxMax.z = newMax;
		break;
	}
}
void aaBBEnlargeToEnclose (AABB *i, AABB *aabbToEnclose )
{
	UpdateMin( &aabbToEnclose->BoxMin.x, &i->BoxMin.x );
	UpdateMin( &aabbToEnclose->BoxMin.y, &i->BoxMin.y );
	UpdateMin( &aabbToEnclose->BoxMin.z, &i->BoxMin.z );
	UpdateMax( &aabbToEnclose->BoxMax.x, &i->BoxMax.x );
	UpdateMax( &aabbToEnclose->BoxMax.y, &i->BoxMax.y );
	UpdateMax( &aabbToEnclose->BoxMax.z, &i->BoxMax.z );
}
double aaBBSurfaceArea(AABB *i)
{
	VectorR3 delta = i->BoxMax;
	delta.x -= i->BoxMin.x;
	delta.y -= i->BoxMin.y;
	delta.z -= i->BoxMin.z;
	return 2.0*(delta.x*delta.y + delta.x*delta.z + delta.y*delta.z);
}

int aabbRayEntryExit(AABB *i, VectorR3 *startPos, 
					   int signDirX, int signDirY, int signDirZ, VectorR3 *dirInv,
					   double *entryDist, int *entryFaceId,
					   double *exitDist, int *exitFaceId )
{
	double *maxEnterDist=entryDist;
	int *maxEnterAxis = entryFaceId;
	double *minExitDist = exitDist;
	int *minExitAxis = exitFaceId;
	double newEnterDist;
	double newExitDist;


	double mx, mn;
	if ( signDirX!=0 ) {
		if ( signDirX==1 ) {
			mx = i->BoxMax.x;
			mn = i->BoxMin.x;
		}
		else {
			mx = i->BoxMin.x;
			mn = i->BoxMax.x;
		}
		*maxEnterDist = (mn-startPos->x)*dirInv->x;
		*minExitDist = (mx-startPos->x)*dirInv->x;
		*maxEnterAxis = 0;
		*minExitAxis = 0;
	}
	else {
		if ( startPos->x<i->BoxMin.x || startPos->x>i->BoxMax.x ) {
			return FALSE;
		}
		*maxEnterDist = -1e60;
		*minExitDist = 1e60;
		*maxEnterAxis = -1;
		*minExitAxis = -1;
	}

	if ( signDirY!=0 ) {
		if ( signDirY==1 ) {
			mx = i->BoxMax.y;
			mn = i->BoxMin.y;
		}
		else {
			mx = i->BoxMin.y;
			mn = i->BoxMax.y;
		}
		newEnterDist = (mn-startPos->y)*dirInv->y;
		newExitDist = (mx-startPos->y)*dirInv->y;
		if ( *maxEnterDist<newEnterDist ) {
			*maxEnterDist = newEnterDist;
			*maxEnterAxis = 1;
		}
		if ( *minExitDist>newExitDist ) {
			*minExitDist = newExitDist;
			*minExitAxis = 1;
		}
	}
	else {
		if ( startPos->y<i->BoxMin.y || startPos->y>i->BoxMax.y ) {
			return FALSE;
		}
	}

	if ( signDirZ!=0 ) {
		if ( signDirZ==1 ) {
			mx = i->BoxMax.z;
			mn = i->BoxMin.z;
		}
		else {
			mx = i->BoxMin.z;
			mn = i->BoxMax.z;
		}
		newEnterDist = (mn-startPos->z)*dirInv->z;
		newExitDist = (mx-startPos->z)*dirInv->z;
		if ( *maxEnterDist<newEnterDist ) {
			*maxEnterDist = newEnterDist;
			*maxEnterAxis = 2;
		}
		if ( *minExitDist>newExitDist ) {
			*minExitDist = newExitDist;
			*minExitAxis = 2;
		}
	}
	else {
		if ( startPos->z<i->BoxMin.z || startPos->z>i->BoxMax.z ) {
			return FALSE;
		}
	}

	if ( *minExitDist < *maxEnterDist ) {
		return FALSE;
	}
	return TRUE;
}
/*
int aabbRayEntryExit(VectorR3 *startPos, VectorR3 *dir,
						double *entryDist, int *entryFaceId,
						double *exitDist, int *exitFaceId )
{
	VectorR3 dirInv;
	int signDirX = Sign(dir.x);
	if ( signDirX!=0 ) {
		dirInv.x = 1.0/dir.x;
	}
	int signDirY = Sign(dir.y);
	if ( signDirY!=0 ) {
		dirInv.y = 1.0/dir.y;
	}
	int signDirZ = Sign(dir.z);
	if ( signDirZ!=0 ) {
		dirInv.z = 1.0/dir.z;
	}
	return RayEntryExit( startPos, signDirX, signDirY, signDirZ, dirInv,
						 entryDist, entryFaceId, exitDist, exitFaceId );
}
*/
double UpdateMin(double *x,double *y)
{
	if ( *x<*y ) {
		*y = *x;
	}
	return *y;
}
double UpdateMax(double *x,double *y)
{
	if ( *x>*y ) {
		*y = *x;
	}
	return *y;
}
int listStartR(void *p1,long size,struct listStruct *p2)
{
	void **pp;
	
	if(!p1 || (size <= 0) || !p2)return 1;
	
	p2->ImageMax=0;
	p2->ImageCount=0;
	p2->ImageSize=size;
	p2->ImagePointer=p1;
	pp=(void **)p1;
	*pp=NULL;
	return 0;
}
int listCheckR(long n,void *p1,long size,struct listStruct *t)
{

    unsigned char *stackData1;
    unsigned char *stackData;
    long stackMax;
	void **pp;
	
	if(!p1 || (size <= 0) || !t)return 1;
	
	pp=(void **)p1;
	
	if(size != t->ImageSize){
	    WarningBatch("listCheck Size Miss-match\n");
		return 1;
	}

	if(n < t->ImageMax)return 0;
	
	stackMax=STACK_INCREMENT+2*t->ImageMax;
	
	stackData1=(unsigned char *)(*pp);
	
	stackData=NULL;
	
	if(stackData1){
	    stackData=(unsigned char *)cRealloc((char *)stackData1,stackMax*size,8269);
	    if(!stackData){
	        goto ErrorOut;
	    }
	    zerol((char *)&stackData[t->ImageMax*size],STACK_INCREMENT*size);
	}else{
	    stackData=(unsigned char *)cMalloc(stackMax*size,8184);
	    if(!stackData){
	        goto ErrorOut;
	    }
	    zerol((char *)stackData,size);
	}
	
	t->ImageMax=stackMax;
	t->ImagePointer=(void **)stackData;
	*pp=stackData;
	
	return 0;
ErrorOut:
	WarningBatch("listCheck Out of Memory\n");
	return 1;
}
