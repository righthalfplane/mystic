#ifndef _BLOCKS_H_

#define _BLOCKS_H_ 1
#include "Xappl.h"
#include "uLib.h"
#include "uDialog.h"
#include "ScrollList.h"
#include "uFiles.h"
#include "BatchFile.h"
#include "FileManager.h"
#include "Linedata.h"
#include "Message.h"
#include "Streamline.h"
#include "uGraph.h"

#define B_X  				0x00000001
#define B_Y  				0x00000002
#define B_Z  				0x00000004
#define B_XVEL 				0x00000008
#define B_YVEL				0x00000010
#define B_ZVEL  			0x00000020
#define B_DAUGHTER  		0x00000040
#define B_ACTIVE  			0x00000080
#define B_LEVEL  			0x00000100
#define B_VALUE  			0x00000200
#define B_GRADIENTS_2D  	0x00000400
#define B_GRADIENTS_3D  	0x00000800


#define B_2D_STANDARD       (B_X | B_Y | B_DAUGHTER |  B_ACTIVE | B_LEVEL | B_VALUE)

#define B_3D_STANDARD       (B_X | B_Y | B_Z | B_DAUGHTER |  B_ACTIVE | B_LEVEL | B_VALUE)

int BlocksStart(struct FileInfo2 *Files,unsigned long flags);
int BlocksRead(struct FileInfo2 *Files,unsigned long flags,long start,long BlockSize,long CurrentFrame);
int BlocksEnd(struct FileInfo2 *Files,unsigned long flags);

int BlksStart(struct FileInfo2 *Files,unsigned long flags);
int BlksRead(struct FileInfo2 *Files,unsigned long flags,long start,long BlockSize,long CurrentFrame);
int BlksEnd(struct FileInfo2 *Files,unsigned long flags);

#endif
