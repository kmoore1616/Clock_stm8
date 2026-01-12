                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ISO C Compiler
                                      3 ; Version 4.5.0 #15242 (Linux)
                                      4 ;--------------------------------------------------------
                                      5 	.module main
                                      6 	
                                      7 ;--------------------------------------------------------
                                      8 ; Public variables in this module
                                      9 ;--------------------------------------------------------
                                     10 	.globl _main
                                     11 ;--------------------------------------------------------
                                     12 ; ram data
                                     13 ;--------------------------------------------------------
                                     14 	.area DATA
                                     15 ;--------------------------------------------------------
                                     16 ; ram data
                                     17 ;--------------------------------------------------------
                                     18 	.area INITIALIZED
                                     19 ;--------------------------------------------------------
                                     20 ; Stack segment in internal ram
                                     21 ;--------------------------------------------------------
                                     22 	.area SSEG
      000001                         23 __start__stack:
      000001                         24 	.ds	1
                                     25 
                                     26 ;--------------------------------------------------------
                                     27 ; absolute external ram data
                                     28 ;--------------------------------------------------------
                                     29 	.area DABS (ABS)
                                     30 
                                     31 ; default segment ordering for linker
                                     32 	.area HOME
                                     33 	.area GSINIT
                                     34 	.area GSFINAL
                                     35 	.area CONST
                                     36 	.area INITIALIZER
                                     37 	.area CODE
                                     38 
                                     39 ;--------------------------------------------------------
                                     40 ; interrupt vector
                                     41 ;--------------------------------------------------------
                                     42 	.area HOME
      008000                         43 __interrupt_vect:
      008000 82 00 80 07             44 	int s_GSINIT ; reset
                                     45 ;--------------------------------------------------------
                                     46 ; global & static initialisations
                                     47 ;--------------------------------------------------------
                                     48 	.area HOME
                                     49 	.area GSINIT
                                     50 	.area GSFINAL
                                     51 	.area GSINIT
      008007 CD 85 AE         [ 4]   52 	call	___sdcc_external_startup
      00800A 4D               [ 1]   53 	tnz	a
      00800B 27 03            [ 1]   54 	jreq	__sdcc_init_data
      00800D CC 80 04         [ 2]   55 	jp	__sdcc_program_startup
      008010                         56 __sdcc_init_data:
                                     57 ; stm8_genXINIT() start
      008010 AE 00 00         [ 2]   58 	ldw x, #l_DATA
      008013 27 07            [ 1]   59 	jreq	00002$
      008015                         60 00001$:
      008015 72 4F 00 00      [ 1]   61 	clr (s_DATA - 1, x)
      008019 5A               [ 2]   62 	decw x
      00801A 26 F9            [ 1]   63 	jrne	00001$
      00801C                         64 00002$:
      00801C AE 00 00         [ 2]   65 	ldw	x, #l_INITIALIZER
      00801F 27 09            [ 1]   66 	jreq	00004$
      008021                         67 00003$:
      008021 D6 80 2C         [ 1]   68 	ld	a, (s_INITIALIZER - 1, x)
      008024 D7 00 00         [ 1]   69 	ld	(s_INITIALIZED - 1, x), a
      008027 5A               [ 2]   70 	decw	x
      008028 26 F7            [ 1]   71 	jrne	00003$
      00802A                         72 00004$:
                                     73 ; stm8_genXINIT() end
                                     74 	.area GSFINAL
      00802A CC 80 04         [ 2]   75 	jp	__sdcc_program_startup
                                     76 ;--------------------------------------------------------
                                     77 ; Home
                                     78 ;--------------------------------------------------------
                                     79 	.area HOME
                                     80 	.area HOME
      008004                         81 __sdcc_program_startup:
      008004 CC 80 2D         [ 2]   82 	jp	_main
                                     83 ;	return from main will return to caller
                                     84 ;--------------------------------------------------------
                                     85 ; code
                                     86 ;--------------------------------------------------------
                                     87 	.area CODE
                                     88 ;	main.c: 37: void main(void){
                                     89 ;	-----------------------------------------
                                     90 ;	 function main
                                     91 ;	-----------------------------------------
      00802D                         92 _main:
                                     93 ;	main.c: 694: __endasm;
      00802D                         94 Setup:
                                     95 ;	Misc Variables
      00802D 3F 51            [ 1]   96 	CLR	0x51 ; Regular Mode = 0, AlarmSet = 1, TimeSet = 2;
      00802F 72 12 00 52      [ 1]   97 	BSET	0x52,#1 ; Allows button pressses to be registered. Cleared when button pressed, set every second
      008033 3F 53            [ 1]   98 	CLR	0x53 ; BeeperOn|AlmGoing|AlmSet|Count|UpdateSec|R Disp|A Disp|C Disp|
      008035 72 16 00 53      [ 1]   99 	BSET	0x53,#3 ; UpdateSec Enabled on start
      008039 72 18 00 53      [ 1]  100 	BSET	0x53,#4 ; Count Enabled on start
      00803D 72 1A 00 53      [ 1]  101 	BSET	0x53,#5 ; Alm Enabled on start
      008041 3F 54            [ 1]  102 	CLR	0x54 ; Used toAlm hold cursor position on clk change
      008043 3F 55            [ 1]  103 	CLR	0x55 ; Index to hold what number to increment
      008045 3F 56            [ 1]  104 	CLR	0x56 ; Used toAlm hold cursor position on clk change
      008047 3F 57            [ 1]  105 	CLR	0x57 ; Index to hold what number to increment
                                    106 ;	Timer setup
      008049 35 00 50 C6      [ 1]  107 	MOV	0x50C6, #0x00 ; Set clock to 16mhz
      00804D 35 10 52 50      [ 1]  108 	MOV	0x5250, #0b00010000 ; Set clock registers
      008051 35 00 52 60      [ 1]  109 	MOV	0x5260, #0x00 ; Dont divide clock
      008055 35 0F 52 61      [ 1]  110 	MOV	0x5261, #0x0F ; Dont divide clock
      008059 35 4E 52 62      [ 1]  111 	MOV	0x5262, #0x4E ; ARRH
      00805D 35 20 52 63      [ 1]  112 	MOV	0x5263, #0x20; ARRL
      008061 35 00 14 B4      [ 1]  113 	MOV	5300, #0x00; Set clock registers
      008065 35 04 53 0E      [ 1]  114 	MOV	0x530E, #0x04
      008069 35 03 53 0F      [ 1]  115 	MOV	0x530F, #0x03 ; ARRH
      00806D 35 E8 53 10      [ 1]  116 	MOV	0x5310, #0xE8 ; ARRL
      008071 72 11 53 04      [ 1]  117 	BRES	0x5304, #0
      008075 72 10 53 00      [ 1]  118 	BSET	0x5300, #0
      008079 72 5F 50 3F      [ 1]  119 	CLR	0x503F
                                    120 ;	Clock Settings =====================================================
      00807D AE 03 E8         [ 2]  121 	LDW	X,#1000 ; Timer for 1s
      008080 3F 60            [ 1]  122 	CLR	0x60 ; Sec Low
      008082 3F 61            [ 1]  123 	CLR	0x61 ; Sec High
      008084 3F 62            [ 1]  124 	CLR	0x62 ; Min Low
      008086 3F 63            [ 1]  125 	CLR	0x63 ; Min High
      008088 35 02 00 64      [ 1]  126 	MOV	0x64,#2 ; Hr LOW
      00808C 35 01 00 65      [ 1]  127 	MOV	0x65,#1 ; Hr High Set to 12oclock
      008090 3F 66            [ 1]  128 	CLR	0x66 ; ASec Low
      008092 3F 67            [ 1]  129 	CLR	0x67 ; ASec High
      008094 3F 68            [ 1]  130 	CLR	0x68 ; AMin Low
      008096 3F 69            [ 1]  131 	CLR	0x69 ; AMin High
      008098 3F 6A            [ 1]  132 	CLR	0x6A ; AHr Low
      00809A 3F 6B            [ 1]  133 	CLR	0x6B ; AHr High ; Set alarm to midnight
                                    134 ;	Gpio Setup ; ========================================================
      00809C 35 FF 50 02      [ 1]  135 	MOV	0x5002, #0xFF
      0080A0 35 FF 50 03      [ 1]  136 	MOV	0x5003, #0xFF
      0080A4 35 FF 50 11      [ 1]  137 	MOV	0x5011, #0xFF
      0080A8 35 FF 50 12      [ 1]  138 	MOV	0x5012, #0XFF
      0080AC 35 FF 50 0D      [ 1]  139 	MOV	0x500D, #0xFF
                                    140 ;	LCD Setup ===========================================================
      0080B0 CD 83 94         [ 4]  141 	CALL	ClearShift
      0080B3 A6 30            [ 1]  142 	LD	A, #0b00110000; Fx Set Cmd
      0080B5 CD 83 B5         [ 4]  143 	CALL	PopulateShift
      0080B8 CD 83 ED         [ 4]  144 	CALL	SendCommand
      0080BB CD 83 94         [ 4]  145 	CALL	ClearShift
      0080BE A6 0C            [ 1]  146 	LD	A, #0b00001100; Display On
      0080C0 CD 83 B5         [ 4]  147 	CALL	PopulateShift
      0080C3 CD 83 ED         [ 4]  148 	CALL	SendCommand
      0080C6 CD 83 94         [ 4]  149 	CALL	ClearShift
      0080C9 A6 06            [ 1]  150 	LD	A, #0b00000110; Entry mode Set
      0080CB CD 83 B5         [ 4]  151 	CALL	PopulateShift
      0080CE CD 83 ED         [ 4]  152 	CALL	SendCommand
      0080D1 CD 83 94         [ 4]  153 	CALL	ClearShift
      0080D4 CD 84 33         [ 4]  154 	CALL	ClearScreen
      0080D7 CD 84 AA         [ 4]  155 	CALL	ResetClk
      0080DA                        156 Delayloop:
      0080DA 72 0A 50 0B 24   [ 2]  157 	BTJT	0x500B,#5,Skip ; If pressed changes what mode user is in
      0080DF 72 03 00 52 1F   [ 2]  158 	BTJF	0x52,#1,Skip
      0080E4 CD 84 AA         [ 4]  159 	CALL	ResetClk
      0080E7 3F 54            [ 1]  160 	CLR	0x54 ; Reset Cursor Pos
      0080E9 72 13 00 52      [ 1]  161 	BRES	0x52,#1
      0080ED 3C 51            [ 1]  162 	INC	0x51 ; Change Mode If button is hit
      0080EF 72 11 00 53      [ 1]  163 	BRES	0x53,#0
      0080F3 72 13 00 53      [ 1]  164 	BRES	0x53,#1
      0080F7 72 15 00 53      [ 1]  165 	BRES	0x53,#2
      0080FB B6 51            [ 1]  166 	LD	A,0x51 ; Check what mode user is currently in
      0080FD A1 03            [ 1]  167 	CP	A,#3
      0080FF 26 02            [ 1]  168 	JRNE	Skip ; Overflow to Normal mode
      008101 3F 51            [ 1]  169 	CLR	0x51
      008103                        170 Skip:
      008103 72 01 53 04 D2   [ 2]  171 	BTJF	0x5304, #0, Delayloop ; Wait for a ms
      008108 72 11 53 04      [ 1]  172 	BRES	0x5304, #0 ; Reset UIF
      00810C 72 0D 00 53 03   [ 2]  173 	BTJF	0x53,#6,NoBeep
      008111 CD 85 92         [ 4]  174 	CALL	MakeBeep
      008114                        175 NoBeep:
      008114 CD 81 FA         [ 4]  176 	CALL	ClockMode ; Goto Clock Logic
      008117 5A               [ 2]  177 	DECW	X ; Keep track of timing
      008118 27 03            [ 1]  178 	JREQ	EndLoop
      00811A CC 80 DA         [ 2]  179 	JP	Delayloop
      00811D                        180 EndLoop:
      00811D 72 12 00 52      [ 1]  181 	BSET	0x52,#1 ; Second over allow for button presses again
      008121 72 11 53 04      [ 1]  182 	BRES	0x5304, #0 ; Reset UIF
      008125 90 1E 00 53      [ 1]  183 	BCPL	0x53,#7 ; Toggle alm every second
      008129 AE 03 E8         [ 2]  184 	LDW	X, #1000 ; Reset timing register
      00812C 72 09 00 53 1F   [ 2]  185 	BTJF	0x53,#4,SkipSec ; Skip incrementation if changing clock
      008131 3C 60            [ 1]  186 	INC	0x60 ; Increment seconds
      008133 72 02 00 53 06   [ 2]  187 	BTJT	0x53,#1,SkipAdj
      008138 CD 84 64         [ 4]  188 	CALL	AdjustTime ; Adjust for time overflows
      00813B CD 81 53         [ 4]  189 	CALL	CheckAlarm
      00813E                        190 SkipAdj:
      00813E 72 07 00 53 0D   [ 2]  191 	BTJF	0x53,#3,SkipSec ; Skip updating display if user is in alarm mode (keeps track in background)
      008143 CD 84 27         [ 4]  192 	CALL	Backspace ; Print seconds
      008146 B6 60            [ 1]  193 	LD	A, 0x60
      008148 AB 30            [ 1]  194 	ADD	A,#48 ; Adjust to ASCII
      00814A CD 83 B5         [ 4]  195 	CALL	PopulateShift
      00814D CD 84 08         [ 4]  196 	CALL	SendLetter
      008150                        197 SkipSec:
      008150 CC 80 DA         [ 2]  198 	JP	Delayloop
      008153                        199 CheckAlarm:
      008153 72 0B 00 53 1C   [ 2]  200 	BTJF	0x53,#5,ChkDone ; alarm disabled?
      008158 B6 6B            [ 1]  201 	LD	A,0x6B ; alarm hr high
      00815A B1 65            [ 1]  202 	CP	A,0x65
      00815C 26 16            [ 1]  203 	JRNE	ChkDone
      00815E B6 6A            [ 1]  204 	LD	A,0x6A ; alarm hr low
      008160 B1 64            [ 1]  205 	CP	A,0x64
      008162 26 10            [ 1]  206 	JRNE	ChkDone
      008164 B6 69            [ 1]  207 	LD	A,0x69 ; alarm min high
      008166 B1 63            [ 1]  208 	CP	A,0x63
      008168 26 0A            [ 1]  209 	JRNE	ChkDone
      00816A B6 68            [ 1]  210 	LD	A,0x68 ; alarm min low
      00816C B1 62            [ 1]  211 	CP	A,0x62
      00816E 26 04            [ 1]  212 	JRNE	ChkDone
      008170 72 1C 00 53      [ 1]  213 	BSET	0x53,#6 ; start beeping
      008174                        214 ChkDone:
      008174 81               [ 4]  215 	RET
      008175                        216 RegularMode:
      008175 3F 56            [ 1]  217 	CLR	0x56
      008177 72 16 00 53      [ 1]  218 	BSET	0x53,#3 ; Make sure seconds are being updated
      00817B 72 18 00 53      [ 1]  219 	BSET	0x53,#4 ; And the clock is ticking
      00817F 72 04 00 53 35   [ 2]  220 	BTJT	0x53,#2,SkipDispUpdReg ; Can I update Display?
      008184 A6 0C            [ 1]  221 	LD	A, #0b00001100; Update it: (Cursor Off)
      008186 CD 83 B5         [ 4]  222 	CALL	PopulateShift
      008189 CD 83 ED         [ 4]  223 	CALL	SendCommand
      00818C CD 83 94         [ 4]  224 	CALL	ClearShift
      00818F 72 14 00 53      [ 1]  225 	BSET	0x53,#2
      008193 72 11 00 53      [ 1]  226 	BRES	0x53,#0
      008197 72 13 00 53      [ 1]  227 	BRES	0x53,#1
      00819B 72 0B 00 53 0E   [ 2]  228 	BTJF	0x53,#5,NoAlmIcon
      0081A0 A6 2A            [ 1]  229 	LD	A,#0x2A
      0081A2 CD 83 B5         [ 4]  230 	CALL	PopulateShift
      0081A5 CD 84 08         [ 4]  231 	CALL	SendLetter
      0081A8 CD 84 27         [ 4]  232 	CALL	Backspace
      0081AB CC 81 B9         [ 2]  233 	JP	SkipDispUpdReg
      0081AE                        234 NoAlmIcon:
      0081AE A6 20            [ 1]  235 	LD	A,#0x20
      0081B0 CD 83 B5         [ 4]  236 	CALL	PopulateShift
      0081B3 CD 84 08         [ 4]  237 	CALL	SendLetter
      0081B6 CD 84 27         [ 4]  238 	CALL	Backspace
      0081B9                        239 SkipDispUpdReg:
      0081B9 72 0E 50 0B 25   [ 2]  240 	BTJT	0x500B,#7, SkipAlmToggle
      0081BE 72 03 00 52 20   [ 2]  241 	BTJF	0x52,#1, SkipAlmToggle
      0081C3 72 13 00 52      [ 1]  242 	BRES	0x52,#1
      0081C7 90 1A 00 53      [ 1]  243 	BCPL	0x53,#5 ; Toggle Alarm
      0081CB 72 0A 00 53 0B   [ 2]  244 	BTJT	0x53,#5, EnableAlm
      0081D0                        245 DisableAlm:
      0081D0 72 1B 00 53      [ 1]  246 	BRES	0x53,#5
      0081D4 72 15 00 53      [ 1]  247 	BRES	0x53,#2
      0081D8 CC 81 E3         [ 2]  248 	JP	SkipAlmToggle
      0081DB                        249 EnableAlm:
      0081DB 72 1A 00 53      [ 1]  250 	BSET	0x53,#5
      0081DF 72 15 00 53      [ 1]  251 	BRES	0x53,#2
      0081E3                        252 SkipAlmToggle:
      0081E3 72 0C 50 0B 11   [ 2]  253 	BTJT	0x500B,#6, SkipSnooze
      0081E8 72 03 00 52 0C   [ 2]  254 	BTJF	0x52,#1, SkipSnooze
      0081ED 72 13 00 52      [ 1]  255 	BRES	0x52,#1
      0081F1 72 1D 00 53      [ 1]  256 	BRES	0x53,#6 ;Snooze alarm
      0081F5 72 1F 00 53      [ 1]  257 	BRES	0x53,#7
      0081F9                        258 SkipSnooze:
      0081F9 81               [ 4]  259 	RET
      0081FA                        260 ClockMode:
      0081FA 72 00 00 51 08   [ 2]  261 	BTJT	0x51,#0,AlarmSet
      0081FF 72 02 00 51 40   [ 2]  262 	BTJT	0x51,#1,ClockSet
      008204 CC 81 75         [ 2]  263 	JP	RegularMode
      008207                        264 AlarmSet:
      008207 72 17 00 53      [ 1]  265 	BRES	0x53,#3
      00820B 72 19 00 53      [ 1]  266 	BRES	0x53,#4
      00820F 72 02 00 53 2C   [ 2]  267 	BTJT	0x53,#1,SkipDispUpdAlm
      008214 CD 85 1E         [ 4]  268 	CALL	DispAlarm
      008217 A6 0E            [ 1]  269 	LD	A, #0b00001110; Cursor On
      008219 CD 83 B5         [ 4]  270 	CALL	PopulateShift
      00821C CD 83 ED         [ 4]  271 	CALL	SendCommand
      00821F CD 83 94         [ 4]  272 	CALL	ClearShift
      008222 72 11 00 53      [ 1]  273 	BRES	0x53,#0 ; Set mode to alarm
      008226 72 15 00 53      [ 1]  274 	BRES	0x53,#2
      00822A 72 12 00 53      [ 1]  275 	BSET	0x53,#1
      00822E A6 41            [ 1]  276 	LD	A,#0x41
      008230 CD 83 B5         [ 4]  277 	CALL	PopulateShift
      008233 CD 84 08         [ 4]  278 	CALL	SendLetter
      008236 CD 84 27         [ 4]  279 	CALL	Backspace
      008239 CD 84 4B         [ 4]  280 	CALL	ReturnHome
      00823C 3F 54            [ 1]  281 	CLR	0x54
      00823E 3F 55            [ 1]  282 	CLR	0x55
      008240                        283 SkipDispUpdAlm:
      008240 CD 83 09         [ 4]  284 	CALL	AlarmSetLogic
      008243 81               [ 4]  285 	RET
      008244                        286 ClockSet:
      008244 3F 56            [ 1]  287 	CLR	0x56
      008246 89               [ 2]  288 	PUSHW	X
      008247 72 17 00 53      [ 1]  289 	BRES	0x53,#3
      00824B 72 19 00 53      [ 1]  290 	BRES	0x53,#4
      00824F 72 00 00 53 29   [ 2]  291 	BTJT	0x53,#0,SkipDispUpdClk
      008254 A6 0E            [ 1]  292 	LD	A, #0b00001110
      008256 CD 83 B5         [ 4]  293 	CALL	PopulateShift
      008259 CD 83 ED         [ 4]  294 	CALL	SendCommand
      00825C CD 83 94         [ 4]  295 	CALL	ClearShift
      00825F 72 13 00 53      [ 1]  296 	BRES	0x53,#1
      008263 72 15 00 53      [ 1]  297 	BRES	0x53,#2
      008267 72 10 00 53      [ 1]  298 	BSET	0x53,#0
      00826B A6 43            [ 1]  299 	LD	A,#0x43
      00826D CD 83 B5         [ 4]  300 	CALL	PopulateShift
      008270 CD 84 08         [ 4]  301 	CALL	SendLetter
      008273 CD 84 27         [ 4]  302 	CALL	Backspace
      008276 CD 84 4B         [ 4]  303 	CALL	ReturnHome
      008279 3F 54            [ 1]  304 	CLR	0x54
      00827B 3F 55            [ 1]  305 	CLR	0x55
      00827D                        306 SkipDispUpdClk:
      00827D 72 0E 50 0B 33   [ 2]  307 	BTJT	0x500B,#7,SkipClkMov
      008282 72 03 00 52 2E   [ 2]  308 	BTJF	0x52,#1,SkipClkMov
      008287 72 13 00 52      [ 1]  309 	BRES	0x52,#1
      00828B 3C 54            [ 1]  310 	INC	0x54
      00828D 3C 55            [ 1]  311 	INC	0x55
      00828F B6 54            [ 1]  312 	LD	A,0x54
      008291 A1 02            [ 1]  313 	CP	A,#2
      008293 27 0B            [ 1]  314 	JREQ	MovTwo
      008295 A1 05            [ 1]  315 	CP	A,#5
      008297 27 07            [ 1]  316 	JREQ	MovTwo
      008299 A1 08            [ 1]  317 	CP	A,#8
      00829B 27 0E            [ 1]  318 	JREQ	ToStart
      00829D CC 82 A5         [ 2]  319 	JP	MovOne
      0082A0                        320 MovTwo:
      0082A0 CD 84 3F         [ 4]  321 	CALL	MovRight
      0082A3 3C 54            [ 1]  322 	INC	0x54
      0082A5                        323 MovOne:
      0082A5 CD 84 3F         [ 4]  324 	CALL	MovRight
      0082A8 CC 82 B5         [ 2]  325 	JP	SkipClkMov
      0082AB                        326 ToStart:
      0082AB 3F 54            [ 1]  327 	CLR	0x54
      0082AD 3F 55            [ 1]  328 	CLR	0x55
      0082AF CD 84 3F         [ 4]  329 	CALL	MovRight
      0082B2 CD 84 4B         [ 4]  330 	CALL	ReturnHome
      0082B5                        331 SkipClkMov:
      0082B5 72 0C 50 0B 4D   [ 2]  332 	BTJT	0x500B,#6,SkipClkInc
      0082BA 72 03 00 52 48   [ 2]  333 	BTJF	0x52,#1,SkipClkInc
      0082BF 72 13 00 52      [ 1]  334 	BRES	0x52,#1
      0082C3 A6 05            [ 1]  335 	LD	A, #5
      0082C5 B0 55            [ 1]  336 	SUB	A, 0x55
      0082C7 5F               [ 1]  337 	CLRW	X
      0082C8 41               [ 1]  338 	EXG	A,XL
      0082C9 6C 60            [ 1]  339 	INC	(0x60,X)
      0082CB B6 55            [ 1]  340 	LD	A, 0x55
      0082CD A1 00            [ 1]  341 	CP	A,#0
      0082CF 27 1E            [ 1]  342 	JREQ	AdjTwo
      0082D1 A1 02            [ 1]  343 	CP	A,#2
      0082D3 27 0F            [ 1]  344 	JREQ	AdjSix
      0082D5 A1 04            [ 1]  345 	CP	A,#4
      0082D7 27 0B            [ 1]  346 	JREQ	AdjSix
      0082D9 E6 60            [ 1]  347 	LD	A,(0x60,X)
      0082DB A1 0A            [ 1]  348 	CP	A,#10
      0082DD 26 18            [ 1]  349 	JRNE	PrintVal
      0082DF 6F 60            [ 1]  350 	CLR	(0x60,X)
      0082E1 CC 82 F7         [ 2]  351 	JP	PrintVal
      0082E4                        352 AdjSix:
      0082E4 E6 60            [ 1]  353 	LD	A,(0x60,X)
      0082E6 A1 06            [ 1]  354 	CP	A,#6
      0082E8 26 0D            [ 1]  355 	JRNE	PrintVal
      0082EA 6F 60            [ 1]  356 	CLR	(0x60,X)
      0082EC CC 82 F7         [ 2]  357 	JP	PrintVal
      0082EF                        358 AdjTwo:
      0082EF E6 60            [ 1]  359 	LD	A,(0x60,X)
      0082F1 A1 03            [ 1]  360 	CP	A,#3
      0082F3 26 02            [ 1]  361 	JRNE	PrintVal
      0082F5 6F 60            [ 1]  362 	CLR	(0x60,X)
      0082F7                        363 PrintVal:
      0082F7 E6 60            [ 1]  364 	LD	A,(0x60,X)
      0082F9 AB 30            [ 1]  365 	ADD	A,#48 ; Adjust to ASCII
      0082FB CD 83 B5         [ 4]  366 	CALL	PopulateShift
      0082FE CD 84 08         [ 4]  367 	CALL	SendLetter
      008301 CD 84 27         [ 4]  368 	CALL	Backspace
      008304 CD 83 94         [ 4]  369 	CALL	ClearShift
      008307                        370 SkipClkInc:
      008307 85               [ 2]  371 	POPW	X
      008308 81               [ 4]  372 	RET
      008309                        373 AlarmSetLogic:
      008309 89               [ 2]  374 	PUSHW	X
      00830A 72 0E 50 0B 33   [ 2]  375 	BTJT	0x500B,#7,SkipAlmMov ; Is move button pressed?
      00830F 72 03 00 52 2E   [ 2]  376 	BTJF	0x52,#1,SkipAlmMov
      008314 72 13 00 52      [ 1]  377 	BRES	0x52,#1
      008318 3C 56            [ 1]  378 	INC	0x56 ; Inc cursor position
      00831A 3C 57            [ 1]  379 	INC	0x57 ; inc number index
      00831C B6 56            [ 1]  380 	LD	A,0x56
      00831E A1 02            [ 1]  381 	CP	A,#2 ; Is Colon?
      008320 27 0B            [ 1]  382 	JREQ	MovTwoAlm ; Yes Move Two
      008322 A1 05            [ 1]  383 	CP	A,#5 ; Is Colon?
      008324 27 07            [ 1]  384 	JREQ	MovTwoAlm ; Yes Move Two
      008326 A1 08            [ 1]  385 	CP	A,#8 ; Is at end?
      008328 27 0E            [ 1]  386 	JREQ	ToStartAlm ; Yes reset Cursor
      00832A CC 83 32         [ 2]  387 	JP	MovOneAlm ; Else move cursor Regularly
      00832D                        388 MovTwoAlm:
      00832D CD 84 3F         [ 4]  389 	CALL	MovRight
      008330 3C 56            [ 1]  390 	INC	0x56
      008332                        391 MovOneAlm:
      008332 CD 84 3F         [ 4]  392 	CALL	MovRight
      008335 CC 83 42         [ 2]  393 	JP	SkipAlmMov
      008338                        394 ToStartAlm:
      008338 3F 56            [ 1]  395 	CLR	0x56
      00833A 3F 57            [ 1]  396 	CLR	0x57
      00833C CD 84 3F         [ 4]  397 	CALL	MovRight ; Adjust to correct position before moving home
      00833F CD 84 4B         [ 4]  398 	CALL	ReturnHome
      008342                        399 SkipAlmMov:
      008342 72 0C 50 0B 4B   [ 2]  400 	BTJT	0x500B,#6,SkipAlmInc ; Is increment button pressed?
      008347 72 03 00 52 46   [ 2]  401 	BTJF	0x52,#1,SkipAlmInc
      00834C 72 13 00 52      [ 1]  402 	BRES	0x52,#1
      008350 B6 57            [ 1]  403 	LD	A, 0x57
                                    404 ;SUB	A, 0x57 ; Adjust for registers being revesed from inx. (Hrs->sec vs sec->hrs) ** Try uncommenting this
      008352 5F               [ 1]  405 	CLRW	X
      008353 41               [ 1]  406 	EXG	A,XL
      008354 6C 66            [ 1]  407 	INC	(0x66,X) ; Adjust each alarm register
      008356 B6 57            [ 1]  408 	LD	A, 0x57 ; Below just checks for overflow
      008358 A1 00            [ 1]  409 	CP	A,#0
      00835A 27 1E            [ 1]  410 	JREQ	AdjTwoAlm
      00835C A1 02            [ 1]  411 	CP	A,#2
      00835E 27 0F            [ 1]  412 	JREQ	AdjSixAlm
      008360 A1 04            [ 1]  413 	CP	A,#4
      008362 27 0B            [ 1]  414 	JREQ	AdjSixAlm
      008364 E6 66            [ 1]  415 	LD	A,(0x66,X)
      008366 A1 0A            [ 1]  416 	CP	A,#10
      008368 26 18            [ 1]  417 	JRNE	PrintValAlm
      00836A 6F 66            [ 1]  418 	CLR	(0x66,X)
      00836C CC 83 82         [ 2]  419 	JP	PrintValAlm
      00836F                        420 AdjSixAlm:
      00836F E6 66            [ 1]  421 	LD	A,(0x66,X)
      008371 A1 06            [ 1]  422 	CP	A,#6
      008373 26 0D            [ 1]  423 	JRNE	PrintValAlm
      008375 6F 66            [ 1]  424 	CLR	(0x66,X)
      008377 CC 83 82         [ 2]  425 	JP	PrintValAlm
      00837A                        426 AdjTwoAlm:
      00837A E6 66            [ 1]  427 	LD	A,(0x66,X)
      00837C A1 03            [ 1]  428 	CP	A,#3
      00837E 26 02            [ 1]  429 	JRNE	PrintValAlm
      008380 6F 66            [ 1]  430 	CLR	(0x66,X)
      008382                        431 PrintValAlm:
                                    432 ; Throw new value to display
      008382 E6 66            [ 1]  433 	LD	A,(0x66,X)
      008384 AB 30            [ 1]  434 	ADD	A,#48 ; Adjust to ASCII
      008386 CD 83 B5         [ 4]  435 	CALL	PopulateShift
      008389 CD 84 08         [ 4]  436 	CALL	SendLetter
      00838C CD 84 27         [ 4]  437 	CALL	Backspace
      00838F CD 83 94         [ 4]  438 	CALL	ClearShift
      008392                        439 SkipAlmInc:
      008392 85               [ 2]  440 	POPW	X
      008393 81               [ 4]  441 	RET
                                    442 ;	Clears the shift register
      008394                        443 ClearShift:
      008394 88               [ 1]  444 	PUSH	A
      008395 A6 08            [ 1]  445 	LD	A,#8
      008397 72 13 50 0F      [ 1]  446 	BRES	0x500F, #1
      00839B                        447 ClearLoop:
      00839B 72 16 50 0F      [ 1]  448 	BSET	0x500F, #3
      00839F 9D               [ 1]  449 	NOP
      0083A0 72 17 50 0F      [ 1]  450 	BRES	0x500F, #3
      0083A4 4A               [ 1]  451 	DEC	A
      0083A5 27 03            [ 1]  452 	JREQ	ClearDone
      0083A7 CC 83 9B         [ 2]  453 	JP	ClearLoop
      0083AA                        454 ClearDone:
      0083AA 72 14 50 0F      [ 1]  455 	BSET	0x500F, #2
      0083AE 9D               [ 1]  456 	NOP
      0083AF 72 15 50 0F      [ 1]  457 	BRES	0x500F, #2
      0083B3 84               [ 1]  458 	POP	A
      0083B4 81               [ 4]  459 	RET
                                    460 ;	Populates shift register with value in A
      0083B5                        461 PopulateShift:
      0083B5 CD 85 A1         [ 4]  462 	CALL	DumbDelay
      0083B8 89               [ 2]  463 	PUSHW	X
      0083B9 B7 50            [ 1]  464 	LD	0x50, A
      0083BB AE 00 08         [ 2]  465 	LDW	X,#8
      0083BE                        466 PopLoop:
      0083BE 27 22            [ 1]  467 	JREQ	PopDone
      0083C0 72 0E 00 50 03   [ 2]  468 	BTJT	0x50,#7,ShiftO
      0083C5 CC 83 CF         [ 2]  469 	JP	ShiftZ
      0083C8                        470 ShiftO:
      0083C8 72 12 50 0F      [ 1]  471 	BSET	0x500F, #1
      0083CC CC 83 D3         [ 2]  472 	JP	PulseClk
      0083CF                        473 ShiftZ:
      0083CF 72 13 50 0F      [ 1]  474 	BRES	0x500F, #1
      0083D3                        475 PulseClk:
      0083D3 72 16 50 0F      [ 1]  476 	BSET	0x500F, #3
      0083D7 9D               [ 1]  477 	NOP
      0083D8 72 17 50 0F      [ 1]  478 	BRES	0x500F, #3
      0083DC 38 50            [ 1]  479 	SLL	0x50
      0083DE 5A               [ 2]  480 	DECW	X
      0083DF CC 83 BE         [ 2]  481 	JP	PopLoop
      0083E2                        482 PopDone:
      0083E2 72 14 50 0F      [ 1]  483 	BSET	0x500F, #2
      0083E6 9D               [ 1]  484 	NOP
      0083E7 72 15 50 0F      [ 1]  485 	BRES	0x500F, #2
      0083EB 85               [ 2]  486 	POPW	X
      0083EC 81               [ 4]  487 	ret
      0083ED                        488 SendCommand:
      0083ED 72 1B 50 0F      [ 1]  489 	BRES	0x500F, #5 ; Clear RS
      0083F1 72 1D 50 0F      [ 1]  490 	BRES	0x500F, #6 ; Clear E
      0083F5 72 1C 50 0F      [ 1]  491 	BSET	0x500F, #6 ; Set E to send instruction
      0083F9 9D               [ 1]  492 	NOP
      0083FA 9D               [ 1]  493 	NOP
      0083FB 9D               [ 1]  494 	NOP
      0083FC 72 1D 50 0F      [ 1]  495 	BRES	0x500F, #6 ; Clear E
      008400 9D               [ 1]  496 	NOP
      008401 9D               [ 1]  497 	NOP
      008402 9D               [ 1]  498 	NOP	; Little delay
      008403 72 1B 50 0F      [ 1]  499 	BRES	0x500F, #5 ; Clear RS
      008407 81               [ 4]  500 	RET
      008408                        501 SendLetter:
      008408 72 1B 50 0F      [ 1]  502 	BRES	0x500F, #5 ; Clear RS
      00840C 72 1D 50 0F      [ 1]  503 	BRES	0x500F, #6 ; Clear E
      008410 72 1C 50 0F      [ 1]  504 	BSET	0x500F, #6 ; Set E to send instruction
      008414 72 1A 50 0F      [ 1]  505 	BSET	0x500F, #5 ; Set RS
      008418 9D               [ 1]  506 	NOP
      008419 9D               [ 1]  507 	NOP
      00841A 9D               [ 1]  508 	NOP
      00841B 72 1D 50 0F      [ 1]  509 	BRES	0x500F, #6 ; Clear E
      00841F 9D               [ 1]  510 	NOP
      008420 9D               [ 1]  511 	NOP
      008421 9D               [ 1]  512 	NOP
      008422 72 1B 50 0F      [ 1]  513 	BRES	0x500F, #5 ; Clear RS
      008426 81               [ 4]  514 	RET
      008427                        515 Backspace:
      008427 CD 83 94         [ 4]  516 	CALL	ClearShift
      00842A A6 10            [ 1]  517 	LD	A, #0b00010000; Shift cursor left
      00842C CD 83 B5         [ 4]  518 	CALL	PopulateShift
      00842F CD 83 ED         [ 4]  519 	CALL	SendCommand
      008432 81               [ 4]  520 	RET
      008433                        521 ClearScreen:
      008433 A6 01            [ 1]  522 	LD	A, #1 ; Clear Display
      008435 CD 83 B5         [ 4]  523 	CALL	PopulateShift
      008438 CD 83 ED         [ 4]  524 	CALL	SendCommand
      00843B CD 83 94         [ 4]  525 	CALL	ClearShift
      00843E 81               [ 4]  526 	RET
      00843F                        527 MovRight:
      00843F CD 83 94         [ 4]  528 	CALL	ClearShift
      008442 A6 14            [ 1]  529 	LD	A, #0b00010100
      008444 CD 83 B5         [ 4]  530 	CALL	PopulateShift
      008447 CD 83 ED         [ 4]  531 	CALL	SendCommand
      00844A 81               [ 4]  532 	RET
      00844B                        533 ReturnHome:
      00844B CD 84 27         [ 4]  534 	CALL	Backspace
      00844E CD 84 27         [ 4]  535 	CALL	Backspace
      008451 CD 84 27         [ 4]  536 	CALL	Backspace
      008454 CD 84 27         [ 4]  537 	CALL	Backspace
      008457 CD 84 27         [ 4]  538 	CALL	Backspace
      00845A CD 84 27         [ 4]  539 	CALL	Backspace
      00845D CD 84 27         [ 4]  540 	CALL	Backspace
      008460 CD 84 27         [ 4]  541 	CALL	Backspace
      008463 81               [ 4]  542 	RET
      008464                        543 AdjustTime:
      008464 B6 60            [ 1]  544 	LD	A,0x60
      008466 A1 0A            [ 1]  545 	CP	A,#10
      008468 26 3F            [ 1]  546 	JRNE	AdjTimeDoneSec
      00846A                        547 SecLowOv:
      00846A 3C 61            [ 1]  548 	INC	0x61
      00846C B6 61            [ 1]  549 	LD	A,0x61
      00846E 3F 60            [ 1]  550 	CLR	0x60
      008470 A1 06            [ 1]  551 	CP	A,#6
      008472 26 31            [ 1]  552 	JRNE	AdjTimeDone
      008474                        553 SecHighOver:
      008474 3C 62            [ 1]  554 	INC	0x62
      008476 B6 62            [ 1]  555 	LD	A,0x62
      008478 3F 61            [ 1]  556 	CLR	0x61
      00847A A1 0A            [ 1]  557 	CP	A,#10
      00847C 26 27            [ 1]  558 	JRNE	AdjTimeDone
      00847E                        559 MinLowOver:
      00847E 3C 63            [ 1]  560 	INC	0x63
      008480 B6 63            [ 1]  561 	LD	A,0x63
      008482 3F 62            [ 1]  562 	CLR	0x62
      008484 A1 06            [ 1]  563 	CP	A,#6
      008486 26 1D            [ 1]  564 	JRNE	AdjTimeDone
      008488                        565 MinHighOver:
      008488 3C 64            [ 1]  566 	INC	0x64
      00848A 3F 63            [ 1]  567 	CLR	0x63
      00848C A1 0A            [ 1]  568 	CP	A,#10
      00848E B6 65            [ 1]  569 	LD	A,0x65
      008490 A1 02            [ 1]  570 	CP	A,#2
      008492 27 09            [ 1]  571 	JREQ	DayDone
      008494 26 0F            [ 1]  572 	JRNE	AdjTimeDone
      008496                        573 HrLowOver:
      008496 3C 65            [ 1]  574 	INC	0x65
      008498 3F 64            [ 1]  575 	CLR	0x64
      00849A CC 84 A5         [ 2]  576 	JP	AdjTimeDone
      00849D                        577 DayDone:
      00849D B6 64            [ 1]  578 	LD	A,0x64
      00849F A1 04            [ 1]  579 	CP	A,#4
      0084A1 3F 65            [ 1]  580 	CLR	0x65
      0084A3 3F 64            [ 1]  581 	CLR	0x64
      0084A5                        582 AdjTimeDone:
      0084A5 CD 84 AA         [ 4]  583 	CALL	ResetClk
      0084A8 81               [ 4]  584 	RET
      0084A9                        585 AdjTimeDoneSec:
      0084A9 81               [ 4]  586 	RET
      0084AA                        587 ResetClk:
      0084AA CD 84 33         [ 4]  588 	CALL	ClearScreen
      0084AD B6 65            [ 1]  589 	LD	A, 0x65
      0084AF AB 30            [ 1]  590 	ADD	A,#48 ; Adjust to ASCII
      0084B1 CD 83 B5         [ 4]  591 	CALL	PopulateShift
      0084B4 CD 84 08         [ 4]  592 	CALL	SendLetter
      0084B7 CD 83 94         [ 4]  593 	CALL	ClearShift
      0084BA B6 64            [ 1]  594 	LD	A, 0x64
      0084BC AB 30            [ 1]  595 	ADD	A,#48 ; Adjust to ASCII
      0084BE CD 83 B5         [ 4]  596 	CALL	PopulateShift
      0084C1 CD 84 08         [ 4]  597 	CALL	SendLetter
      0084C4 CD 83 94         [ 4]  598 	CALL	ClearShift
      0084C7 A6 3A            [ 1]  599 	LD	A,#0x3A
      0084C9 CD 83 B5         [ 4]  600 	CALL	PopulateShift
      0084CC CD 84 08         [ 4]  601 	CALL	SendLetter
      0084CF CD 83 94         [ 4]  602 	CALL	ClearShift
      0084D2 B6 63            [ 1]  603 	LD	A, 0x63
      0084D4 AB 30            [ 1]  604 	ADD	A,#48 ; Adjust to ASCII
      0084D6 CD 83 B5         [ 4]  605 	CALL	PopulateShift
      0084D9 CD 84 08         [ 4]  606 	CALL	SendLetter
      0084DC CD 83 94         [ 4]  607 	CALL	ClearShift
      0084DF B6 62            [ 1]  608 	LD	A, 0x62
      0084E1 AB 30            [ 1]  609 	ADD	A,#48 ; Adjust to ASCII
      0084E3 CD 83 B5         [ 4]  610 	CALL	PopulateShift
      0084E6 CD 84 08         [ 4]  611 	CALL	SendLetter
      0084E9 CD 83 94         [ 4]  612 	CALL	ClearShift
      0084EC A6 3A            [ 1]  613 	LD	A,#0x3A
      0084EE CD 83 B5         [ 4]  614 	CALL	PopulateShift
      0084F1 CD 84 08         [ 4]  615 	CALL	SendLetter
      0084F4 CD 83 94         [ 4]  616 	CALL	ClearShift
      0084F7 B6 61            [ 1]  617 	LD	A, 0x61
      0084F9 AB 30            [ 1]  618 	ADD	A,#48 ; Adjust to ASCII
      0084FB CD 83 B5         [ 4]  619 	CALL	PopulateShift
      0084FE CD 84 08         [ 4]  620 	CALL	SendLetter
      008501 CD 83 94         [ 4]  621 	CALL	ClearShift
      008504 B6 60            [ 1]  622 	LD	A, 0x60
      008506 AB 30            [ 1]  623 	ADD	A,#48 ; Adjust to ASCII
      008508 CD 83 B5         [ 4]  624 	CALL	PopulateShift
      00850B CD 84 08         [ 4]  625 	CALL	SendLetter
      00850E CD 83 94         [ 4]  626 	CALL	ClearShift
      008511 72 11 00 53      [ 1]  627 	BRES	0x53,#0
      008515 72 13 00 53      [ 1]  628 	BRES	0x53,#1
      008519 72 15 00 53      [ 1]  629 	BRES	0x53,#2
                                    630 ;CALL	ClockMode
      00851D 81               [ 4]  631 	RET
      00851E                        632 DispAlarm:
      00851E CD 84 33         [ 4]  633 	CALL	ClearScreen
      008521 B6 66            [ 1]  634 	LD	A, 0x66
      008523 AB 30            [ 1]  635 	ADD	A,#48 ; Adjust to ASCII
      008525 CD 83 B5         [ 4]  636 	CALL	PopulateShift
      008528 CD 84 08         [ 4]  637 	CALL	SendLetter
      00852B CD 83 94         [ 4]  638 	CALL	ClearShift
      00852E B6 67            [ 1]  639 	LD	A, 0x67
      008530 AB 30            [ 1]  640 	ADD	A,#48 ; Adjust to ASCII
      008532 CD 83 B5         [ 4]  641 	CALL	PopulateShift
      008535 CD 84 08         [ 4]  642 	CALL	SendLetter
      008538 CD 83 94         [ 4]  643 	CALL	ClearShift
      00853B A6 3A            [ 1]  644 	LD	A,#0x3A
      00853D CD 83 B5         [ 4]  645 	CALL	PopulateShift
      008540 CD 84 08         [ 4]  646 	CALL	SendLetter
      008543 CD 83 94         [ 4]  647 	CALL	ClearShift
      008546 B6 68            [ 1]  648 	LD	A, 0x68
      008548 AB 30            [ 1]  649 	ADD	A,#48 ; Adjust to ASCII
      00854A CD 83 B5         [ 4]  650 	CALL	PopulateShift
      00854D CD 84 08         [ 4]  651 	CALL	SendLetter
      008550 CD 83 94         [ 4]  652 	CALL	ClearShift
      008553 B6 69            [ 1]  653 	LD	A, 0x69
      008555 AB 30            [ 1]  654 	ADD	A,#48 ; Adjust to ASCII
      008557 CD 83 B5         [ 4]  655 	CALL	PopulateShift
      00855A CD 84 08         [ 4]  656 	CALL	SendLetter
      00855D CD 83 94         [ 4]  657 	CALL	ClearShift
      008560 A6 3A            [ 1]  658 	LD	A,#0x3A
      008562 CD 83 B5         [ 4]  659 	CALL	PopulateShift
      008565 CD 84 08         [ 4]  660 	CALL	SendLetter
      008568 CD 83 94         [ 4]  661 	CALL	ClearShift
      00856B B6 6A            [ 1]  662 	LD	A, 0x6A
      00856D AB 30            [ 1]  663 	ADD	A,#48 ; Adjust to ASCII
      00856F CD 83 B5         [ 4]  664 	CALL	PopulateShift
      008572 CD 84 08         [ 4]  665 	CALL	SendLetter
      008575 CD 83 94         [ 4]  666 	CALL	ClearShift
      008578 B6 6B            [ 1]  667 	LD	A, 0x6B
      00857A AB 30            [ 1]  668 	ADD	A,#48 ; Adjust to ASCII
      00857C CD 83 B5         [ 4]  669 	CALL	PopulateShift
      00857F CD 84 08         [ 4]  670 	CALL	SendLetter
      008582 CD 83 94         [ 4]  671 	CALL	ClearShift
      008585 72 11 00 53      [ 1]  672 	BRES	0x53,#0
      008589 72 13 00 53      [ 1]  673 	BRES	0x53,#1
      00858D 72 15 00 53      [ 1]  674 	BRES	0x53,#2
                                    675 ;CALL	ClockMode
      008591 81               [ 4]  676 	RET
      008592                        677 MakeBeep:
      008592 72 0F 00 53 05   [ 2]  678 	BTJF	0x53,#7,SkipBeep
      008597 90 18 50 0F      [ 1]  679 	BCPL	0x500F, #4 ; Toggle Port D, Pin 4 (Creates 500Hz Tone)
      00859B 81               [ 4]  680 	RET
      00859C                        681 SkipBeep:
      00859C 72 19 50 0F      [ 1]  682 	BRES	0x500F,#4
      0085A0 81               [ 4]  683 	RET
      0085A1                        684 DumbDelay:
      0085A1 89               [ 2]  685 	PUSHW	X
      0085A2 AE FF FF         [ 2]  686 	LDW	X, #0xFFFF
      0085A5                        687 DumbLoop:
      0085A5 5A               [ 2]  688 	DECW	X
      0085A6 27 03            [ 1]  689 	JREQ	DumbDone
      0085A8 CC 85 A5         [ 2]  690 	JP	DumbLoop
      0085AB                        691 DumbDone:
      0085AB 85               [ 2]  692 	POPW	X
      0085AC 81               [ 4]  693 	RET
                                    694 ;	main.c: 696: }
      0085AD 81               [ 4]  695 	ret
                                    696 	.area CODE
                                    697 	.area CONST
                                    698 	.area INITIALIZER
                                    699 	.area CABS (ABS)
