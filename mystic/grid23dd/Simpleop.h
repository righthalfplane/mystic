/*    Operators   */

#define LEL0   101	/* Load Long   from External Data    */
#define LEI0   102	/* Load Int    from External Data    */
#define LEB0   103	/* Load Byte   from External Data    */
#define LEF0   104	/* Load Float  from External Data    */
#define LED0   105	/* Load Double from External Data    */
#define LEP0   106	/* Load Pointer from External Data   */
#define LET0   107	/* Load Structure from External Data */
#define LSL0   108	/* Load Long   from Stack Data    */
#define LSI0   109	/* Load Int    from Stack Data    */
#define LSB0   110	/* Load Byte   from Stack Data    */
#define LSF0   111	/* Load Float  from Stack Data    */
#define LSD0   112	/* Load Double from Stack Data    */
#define LSP0   113	/* Load Pointer from Stack Data   */
#define LST0   114	/* Load Structure from Stack Data */
#define LEA0   115	/* Load External Data Address	  */
#define LSA0   116	/* Load Stack Data Address	  */

#define LIC0   150	/* Load Character Indirect	*/
#define LII0   151	/* Load Int Indirect		*/
#define LIL0   152	/* Load Long Indirect		*/
#define LIP0   153	/* Load Pointer Indirect	*/
#define LID0   154	/* Load Double Indirect		*/
#define LIF0   155	/* Load float Indirect		*/
#define LIT0   156	/* Load Structure Indirect	*/

#define SIC2   170	/* Store Character Indirect	*/
#define SII2   171	/* Store Int Indirect		*/
#define SIL2   172	/* Store Long Indirect		*/
#define SIP2   173	/* Store Pointer Indirect	*/
#define SID2   174	/* Store Double Indirect	*/
#define SIF2   175	/* Store float Indirect		*/
#define SIT2   176	/* Store Structure Indirect	*/

#define MSP    200	/* Move Stack Pointer More	*/
#define FE     201	/* Move Stack Pointer Less	*/
#define RET    202	/* Return From Subroutine	*/
#define CF     203	/* Call Subroutine 		*/
#define JP     204	/* Jump To Address		*/
#define JT     205	/* Jump True To Address		*/
#define JF     206	/* Jump False To Address	*/
#define POPP   300      /* Pop Pointer From Stack       */
#define PUSHP  301      /* Push Pointer to Stack	*/
#define POPD   302      /* Pop Double From Stack 	*/
#define PUSHD  303      /* Push Double to Stack	*/
#define POPS   304      /* Pop Structure From Stack 	*/
#define PUSHS  305      /* Push Structure to Stack	*/
#define POPL   306      /* Pop Long From Stack 		*/
#define PUSHL  307      /* Push Long to Stack		*/

#define CVTCC4  501      /* Cast Char to Char		*/
#define CVTIC4  502      /* Cast Int to Char		*/
#define CVTLC4  503      /* Cast Long to Char		*/
#define CVTFC4  504      /* Cast Float to Char		*/
#define CVTDC4  505      /* Cast Double to Char		*/

#define CVTCD4  506      /* Cast Char to Double		*/
#define CVTID4  507      /* Cast Int to Double		*/
#define CVTLD4  508      /* Cast Long to Double		*/
#define CVTFD4  509      /* Cast Float to Double	*/
#define CVTDD4  510      /* Cast Double  to Double	*/

#define CVTCL4  511      /* Cast Char to Long		*/
#define CVTIL4  512      /* Cast Int to Long		*/
#define CVTLL4  513      /* Cast Long to Long		*/
#define CVTFL4  514      /* Cast Float to Long		*/
#define CVTDL4  515      /* Cast Double  to Long	*/
#define CVTPL4  516      /* Cast Pointer  to Long	*/

#define CVTCF4  517      /* Cast Char to Float		*/
#define CVTIF4  518      /* Cast Int to Float		*/
#define CVTLF4  519      /* Cast Long to Float		*/
#define CVTFF4  520      /* Cast Float to Float	*/
#define CVTDF4  521      /* Cast Double  to Float	*/

#define CVTLD0  600      /* Cast Long  to Double	*/
#define CVTLD2  601      /* Cast Long  to Double	*/

#define LONG_MOD	700
#define LONG_OR		701
#define LONG_XOR	702
#define LONG_AND	703
#define LONG_LEFT	704
#define LONG_RIGHT	705
#define LONG_PLUS	706
#define LONG_MINUS	707
#define LONG_MULT	708
#define LONG_DIV	709
#define LONG_LESS	710
#define LONG_GREATER	711
#define LONG_NE		712
#define LONG_EQ		713
#define LONG_LE		714
#define LONG_GE		715

#define DOUBLE_MOD	800
#define DOUBLE_OR	801
#define DOUBLE_XOR	802
#define DOUBLE_AND	803
#define DOUBLE_LEFT	804
#define DOUBLE_RIGHT	805
#define DOUBLE_PLUS	806
#define DOUBLE_MINUS	807
#define DOUBLE_MULT	808
#define DOUBLE_DIV	809
#define DOUBLE_LESS	810
#define DOUBLE_GREATER	811
#define DOUBLE_NE	812
#define DOUBLE_EQ	813
#define DOUBLE_LE	814
#define DOUBLE_GE	815

#define POINTER_MOD	900
#define POINTER_OR	901
#define POINTER_XOR	902
#define POINTER_AND	903
#define POINTER_LEFT	904
#define POINTER_RIGHT	905
#define POINTER_PLUS	906
#define POINTER_MINUS	907
#define POINTER_MULT	908
#define POINTER_DIV	909
#define POINTER_LESS	910
#define POINTER_GREATER	911
#define POINTER_NE	912
#define POINTER_EQ	913
#define POINTER_LE	914
#define POINTER_GE	915

#define PSCALE0		1000
#define PSCALE2		1001
#define PDIVIDE0	1002

#define NZD		1100
#define NZF		1101

#define UML0		1200	/* Unary Minus Long	*/
#define UMD0		1201	/* Unary Minus Long	*/
#define UCL0		1202	/* Unary Complement Long */
#define NOT_L0		1203	/* Unary ! Long */
#define NOT_D0		1204	/* Unary ! Double */
#define NOT_P0		1205	/* Unary ! Pointer */

#define ACP		1300	/* Structure Member OffSet */

#define JUMP_CASE	1400	/* Case Statement Jump		*/
#define JUMP_DEFAULT	1401	/* Default Statement Jump	*/

