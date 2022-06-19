#ifndef __VOICECOMMAND__
#define __VOICECOMMAND__


struct VoiceCommands{
	char *CommandText;
	int  CommandIndex;
};

#define c_Rotate_Faster 		0
#define c_Rotate_Slower 		1
#define c_Rotate_Left 			2
#define c_Rotate_Right 			3
#define c_Rotate_Up 			4
#define c_Rotate_Down 			5
#define c_Move_Faster 			6
#define c_Move_Slower 			7
#define c_Move_Forward 			8
#define c_Move_Backward 		9
#define c_Move_Right 			10
#define c_Move_Left 			11
#define c_Move_Up 				12
#define c_Move_Down 			14
#define c_Stop 					15
#define c_Help 					16
#define c_Start 				17
#define c_Cursor_On 			18
#define c_Cursor_Off 			19
#define c_Cursor_Right 			20
#define c_Cursor_Left 			21
#define c_Cursor_Up 			22
#define c_Cursor_Down 			23
#define c_Cursor_Faster 		24
#define c_Cursor_Slower 		25
#define c_Cursor_Select 		26
#define c_Cursor_Center 		27
#define c_Cursor_Quadrant_One 	28
#define c_Cursor_Quadrant_Two 	29
#define c_Cursor_Quadrant_Three 30
#define c_Cursor_Quadrant_Four 	31
#define c_Do_Again 				32
#define c_Move_Object_Forward   50
#define c_Move_Object_Backward  51
#define c_Move_Object_Right     52
#define c_Move_Object_Left      53
#define c_Move_Object_Up        54
#define c_Move_Object_Down      55
#define c_Add_Object            56
#define c_Delete_Object         57

#define c_Animate_Forward		58
#define c_Animate_Backward		59
#define c_Step_Forward			60
#define c_Step_Backward			61
#define c_Animate_Faster		62
#define c_Animate_Slower		63
#define c_Zoom_In				64
#define c_Zoom_Out				65




int startLanguage(struct VoiceCommands *Language,long Count,IconPtr myIconSpeech);
int stopLanguage(void);

#endif
