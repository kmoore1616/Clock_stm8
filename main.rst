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
                                     11 	.globl _rtc_isr
                                     12 ;--------------------------------------------------------
                                     13 ; ram data
                                     14 ;--------------------------------------------------------
                                     15 	.area DATA
                                     16 ;--------------------------------------------------------
                                     17 ; ram data
                                     18 ;--------------------------------------------------------
                                     19 	.area INITIALIZED
                                     20 ;--------------------------------------------------------
                                     21 ; Stack segment in internal ram
                                     22 ;--------------------------------------------------------
                                     23 	.area SSEG
      000001                         24 __start__stack:
      000001                         25 	.ds	1
                                     26 
                                     27 ;--------------------------------------------------------
                                     28 ; absolute external ram data
                                     29 ;--------------------------------------------------------
                                     30 	.area DABS (ABS)
                                     31 
                                     32 ; default segment ordering for linker
                                     33 	.area HOME
                                     34 	.area GSINIT
                                     35 	.area GSFINAL
                                     36 	.area CONST
                                     37 	.area INITIALIZER
                                     38 	.area CODE
                                     39 
                                     40 ;--------------------------------------------------------
                                     41 ; interrupt vector
                                     42 ;--------------------------------------------------------
                                     43 	.area HOME
      008000                         44 __interrupt_vect:
      008000 82 00 80 1B             45 	int s_GSINIT ; reset
      008004 82 00 00 00             46 	int 0x000000 ; trap
      008008 82 00 00 00             47 	int 0x000000 ; int0
      00800C 82 00 00 00             48 	int 0x000000 ; int1
      008010 82 00 00 00             49 	int 0x000000 ; int2
      008014 82 00 80 41             50 	int _rtc_isr ; int3
                                     51 ;--------------------------------------------------------
                                     52 ; global & static initialisations
                                     53 ;--------------------------------------------------------
                                     54 	.area HOME
                                     55 	.area GSINIT
                                     56 	.area GSFINAL
                                     57 	.area GSINIT
      00801B CD 86 5B         [ 4]   58 	call	___sdcc_external_startup
      00801E 4D               [ 1]   59 	tnz	a
      00801F 27 03            [ 1]   60 	jreq	__sdcc_init_data
      008021 CC 80 18         [ 2]   61 	jp	__sdcc_program_startup
      008024                         62 __sdcc_init_data:
                                     63 ; stm8_genXINIT() start
      008024 AE 00 00         [ 2]   64 	ldw x, #l_DATA
      008027 27 07            [ 1]   65 	jreq	00002$
      008029                         66 00001$:
      008029 72 4F 00 00      [ 1]   67 	clr (s_DATA - 1, x)
      00802D 5A               [ 2]   68 	decw x
      00802E 26 F9            [ 1]   69 	jrne	00001$
      008030                         70 00002$:
      008030 AE 00 00         [ 2]   71 	ldw	x, #l_INITIALIZER
      008033 27 09            [ 1]   72 	jreq	00004$
      008035                         73 00003$:
      008035 D6 80 40         [ 1]   74 	ld	a, (s_INITIALIZER - 1, x)
      008038 D7 00 00         [ 1]   75 	ld	(s_INITIALIZED - 1, x), a
      00803B 5A               [ 2]   76 	decw	x
      00803C 26 F7            [ 1]   77 	jrne	00003$
      00803E                         78 00004$:
                                     79 ; stm8_genXINIT() end
                                     80 	.area GSFINAL
      00803E CC 80 18         [ 2]   81 	jp	__sdcc_program_startup
                                     82 ;--------------------------------------------------------
                                     83 ; Home
                                     84 ;--------------------------------------------------------
                                     85 	.area HOME
                                     86 	.area HOME
      008018                         87 __sdcc_program_startup:
      008018 CC 80 57         [ 2]   88 	jp	_main
                                     89 ;	return from main will return to caller
                                     90 ;--------------------------------------------------------
                                     91 ; code
                                     92 ;--------------------------------------------------------
                                     93 	.area CODE
                                     94 ;	main.c: 43: void rtc_isr (void) __interrupt(3){
                                     95 ;	-----------------------------------------
                                     96 ;	 function rtc_isr
                                     97 ;	-----------------------------------------
      008041                         98 _rtc_isr:
      008041 4F               [ 1]   99 	clr	a
      008042 62               [ 2]  100 	div	x, a
                                    101 ;	main.c: 51: __endasm;
      008043 C6 50 01         [ 1]  102 	LD	A, 0x5001
      008046 72 02 00 70 0B   [ 2]  103 	BTJT	0x70,#1, NoSecUpd
      00804B 72 09 00 53 06   [ 2]  104 	BTJF	0x53,#4,NoSecUpd; Skip incrementation if changing clock
      008050 3C 60            [ 1]  105 	INC	0x60
      008052 72 12 00 70      [ 1]  106 	BSET	0x70,#1 ; Debounce
      008056                        107 NoSecUpd:
                                    108 ;	main.c: 52: }
      008056 80               [11]  109 	iret
                                    110 ;	main.c: 55: void main(void){
                                    111 ;	-----------------------------------------
                                    112 ;	 function main
                                    113 ;	-----------------------------------------
      008057                        114 _main:
                                    115 ;	main.c: 754: __endasm;
      008057                        116 Setup:
                                    117 ;	Misc Variables
      008057 3F 51            [ 1]  118 	CLR	0x51 ; Regular Mode = 0, AlarmSet = 1, TimeSet = 2;
      008059 3F 70            [ 1]  119 	CLR	0x70 ; SWQ FLAG
      00805B 72 12 00 52      [ 1]  120 	BSET	0x52,#1 ; Allows button pressses to be registered. Cleared when button pressed, set every second
      00805F 72 14 00 52      [ 1]  121 	BSET	0x52,#2 ; Doubles debounce cuz yeah
      008063 3F 53            [ 1]  122 	CLR	0x53 ; BeeperOn|AlmGoing|AlmSet|Count|UpdateSec|R Disp|A Disp|C Disp|
      008065 72 16 00 53      [ 1]  123 	BSET	0x53,#3 ; UpdateSec Enabled on start
      008069 72 18 00 53      [ 1]  124 	BSET	0x53,#4 ; Count Enabled on start
      00806D 72 1A 00 53      [ 1]  125 	BSET	0x53,#5 ; Alm Enabled on start
      008071 72 1D 00 53      [ 1]  126 	BRES	0x53,#6 ; Beeper not on
      008075 3F 54            [ 1]  127 	CLR	0x54 ; Used toAlm hold cursor position on clk change
      008077 3F 55            [ 1]  128 	CLR	0x55 ; Index to hold what number to increment
      008079 3F 56            [ 1]  129 	CLR	0x56 ; Used toAlm hold cursor position on clk change
      00807B 3F 57            [ 1]  130 	CLR	0x57 ; Index to hold what number to increment
                                    131 ;	Timer setup
      00807D 35 00 50 C6      [ 1]  132 	MOV	0x50C6, #0x00 ; Set clock to 16mhz
      008081 35 10 52 50      [ 1]  133 	MOV	0x5250, #0b00010000 ; Set clock registers
      008085 35 00 52 60      [ 1]  134 	MOV	0x5260, #0x00 ; Dont divide clock
      008089 35 0F 52 61      [ 1]  135 	MOV	0x5261, #0x0F ; Dont divide clock
      00808D 35 4E 52 62      [ 1]  136 	MOV	0x5262, #0x4E ; ARRH
      008091 35 20 52 63      [ 1]  137 	MOV	0x5263, #0x20; ARRL
      008095 35 00 14 B4      [ 1]  138 	MOV	5300, #0x00; Set clock registers
      008099 35 04 53 0E      [ 1]  139 	MOV	0x530E, #0x04
      00809D 35 03 53 0F      [ 1]  140 	MOV	0x530F, #0x03 ; ARRH
      0080A1 35 E8 53 10      [ 1]  141 	MOV	0x5310, #0xE8 ; ARRL
      0080A5 72 11 53 04      [ 1]  142 	BRES	0x5304, #0
      0080A9 72 10 53 00      [ 1]  143 	BSET	0x5300, #0
      0080AD 72 5F 50 3F      [ 1]  144 	CLR	0x503F
                                    145 ;	Interrupt Settings =================================================
      0080B1 9A               [ 1]  146 	RIM	; Clear interrupt masks
      0080B2 35 02 50 A0      [ 1]  147 	MOV	0x50A0 ,#0b00000010 ; Set falling edge for port A
                                    148 ;	Clock Settings =====================================================
      0080B6 AE 00 64         [ 2]  149 	LDW	X,#100; Timer for Xms
      0080B9 3F 60            [ 1]  150 	CLR	0x60 ; Sec Low
      0080BB 3F 61            [ 1]  151 	CLR	0x61 ; Sec High
      0080BD 3F 62            [ 1]  152 	CLR	0x62 ; Min Low
      0080BF 3F 63            [ 1]  153 	CLR	0x63 ; Min High
      0080C1 35 02 00 64      [ 1]  154 	MOV	0x64,#2 ; Hr LOW
      0080C5 35 01 00 65      [ 1]  155 	MOV	0x65,#1 ; Hr High Set to 12oclock
      0080C9 3F 66            [ 1]  156 	CLR	0x66 ; ASec Low
      0080CB 3F 67            [ 1]  157 	CLR	0x67 ; ASec High
      0080CD 3F 68            [ 1]  158 	CLR	0x68 ; AMin Low
      0080CF 3F 69            [ 1]  159 	CLR	0x69 ; AMin High
      0080D1 3F 6A            [ 1]  160 	CLR	0x6A ; AHr Low
      0080D3 3F 6B            [ 1]  161 	CLR	0x6B ; AHr High ; Set alarm to midnight
                                    162 ;	Gpio Setup ; ========================================================
      0080D5 72 16 50 02      [ 1]  163 	BSET	0x5002, #3
      0080D9 72 16 50 03      [ 1]  164 	BSET	0x5003, #3
      0080DD 72 13 50 02      [ 1]  165 	BRES	0x5002, #1 ; PA_DDR bit3 = 0
      0080E1 72 13 50 03      [ 1]  166 	BRES	0x5003, #1 ; PA_CR1 bit3 = 1
      0080E5 72 5F 50 04      [ 1]  167 	CLR	0x5004
      0080E9 72 12 50 04      [ 1]  168 	BSET	0x5004, #1 ; PA_CR2 bit3 = 1
      0080ED 35 FF 50 11      [ 1]  169 	MOV	0x5011, #0xFF
      0080F1 35 FF 50 12      [ 1]  170 	MOV	0x5012, #0XFF
      0080F5 35 FF 50 0D      [ 1]  171 	MOV	0x500D, #0xFF
                                    172 ;	LCD Setup ===========================================================
      0080F9 CD 84 40         [ 4]  173 	CALL	ClearShift
      0080FC A6 30            [ 1]  174 	LD	A, #0b00110000; Fx Set Cmd
      0080FE CD 84 61         [ 4]  175 	CALL	PopulateShift
      008101 CD 84 99         [ 4]  176 	CALL	SendCommand
      008104 CD 84 40         [ 4]  177 	CALL	ClearShift
      008107 A6 0C            [ 1]  178 	LD	A, #0b00001100; Display On
      008109 CD 84 61         [ 4]  179 	CALL	PopulateShift
      00810C CD 84 99         [ 4]  180 	CALL	SendCommand
      00810F CD 84 40         [ 4]  181 	CALL	ClearShift
      008112 A6 06            [ 1]  182 	LD	A, #0b00000110; Entry mode Set
      008114 CD 84 61         [ 4]  183 	CALL	PopulateShift
      008117 CD 84 99         [ 4]  184 	CALL	SendCommand
      00811A CD 84 40         [ 4]  185 	CALL	ClearShift
      00811D CD 84 DF         [ 4]  186 	CALL	ClearScreen
      008120 CD 85 57         [ 4]  187 	CALL	ResetClk
      008123                        188 Delayloop:
      008123 72 0A 50 0B 2B   [ 2]  189 	BTJT	0x500B,#5,Skip ; If pressed changes what mode user is in C5
      008128 72 03 00 52 26   [ 2]  190 	BTJF	0x52,#1,Skip
      00812D 72 05 00 52 21   [ 2]  191 	BTJF	0x52,#2,Skip
      008132 CD 85 57         [ 4]  192 	CALL	ResetClk
      008135 72 13 00 52      [ 1]  193 	BRES	0x52,#1
      008139 72 15 00 52      [ 1]  194 	BRES	0x52,#2
      00813D 3C 51            [ 1]  195 	INC	0x51 ; Change Mode If button is hit
      00813F 72 11 00 53      [ 1]  196 	BRES	0x53,#0
      008143 72 13 00 53      [ 1]  197 	BRES	0x53,#1
      008147 72 15 00 53      [ 1]  198 	BRES	0x53,#2
      00814B B6 51            [ 1]  199 	LD	A,0x51 ; Check what mode user is currently in
      00814D A1 03            [ 1]  200 	CP	A,#3
      00814F 26 02            [ 1]  201 	JRNE	Skip ; Overflow to Normal mode (0)
      008151 3F 51            [ 1]  202 	CLR	0x51
      008153                        203 Skip:
      008153 72 01 53 04 CB   [ 2]  204 	BTJF	0x5304, #0, Delayloop ; Wait for a ms
      008158 72 11 53 04      [ 1]  205 	BRES	0x5304, #0 ; Reset UIF
      00815C 72 0D 00 53 03   [ 2]  206 	BTJF	0x53,#6,NoBeep
      008161 CD 86 3F         [ 4]  207 	CALL	MakeBeep
      008164                        208 NoBeep:
      008164 CD 82 80         [ 4]  209 	CALL	ClockMode ; Goto Clock Logic
      008167 5A               [ 2]  210 	DECW	X ; Keep track of timing
      008168 27 03            [ 1]  211 	JREQ	EndLoop
      00816A CC 81 23         [ 2]  212 	JP	Delayloop
      00816D                        213 EndLoop:
      00816D 72 02 00 52 07   [ 2]  214 	BTJT	0x52,#1,SetTwo
      008172 72 12 00 52      [ 1]  215 	BSET	0x52,#1 ; Second over allow for button presses again
      008176 CC 81 7D         [ 2]  216 	JP	SkipTwo
      008179                        217 SetTwo:
      008179 72 14 00 52      [ 1]  218 	BSET	0x52,#2
      00817D                        219 SkipTwo:
      00817D 72 13 00 70      [ 1]  220 	BRES	0x70,#1 ; Reset debounce logic
      008181 72 11 53 04      [ 1]  221 	BRES	0x5304, #0 ; Reset UIF
      008185 CD 81 B1         [ 4]  222 	CALL	CheckAlarm
      008188 90 1E 00 53      [ 1]  223 	BCPL	0x53,#7 ; Toggle alm every second
      00818C AE 00 64         [ 2]  224 	LDW	X, #100; Reset timing register
      00818F 72 09 00 53 1A   [ 2]  225 	BTJF	0x53,#4,SkipSec ; Skip incrementation if changing clock
                                    226 ;	INC 0x60 ; Increment seconds NOT ACCURATE ONLY USE IF RTC NOT PRESENT
      008194 72 02 00 53 03   [ 2]  227 	BTJT	0x53,#1,SkipAdj
      008199 CD 85 10         [ 4]  228 	CALL	AdjustTime ; Adjust for time overflows
      00819C                        229 SkipAdj:
      00819C 72 07 00 53 0D   [ 2]  230 	BTJF	0x53,#3,SkipSec ; Skip updating display if user is in alarm mode (keeps track in background)
      0081A1 CD 84 D3         [ 4]  231 	CALL	Backspace ; Print seconds
      0081A4 B6 60            [ 1]  232 	LD	A, 0x60
      0081A6 AB 30            [ 1]  233 	ADD	A,#48 ; Adjust to ASCII
      0081A8 CD 84 61         [ 4]  234 	CALL	PopulateShift
      0081AB CD 84 B4         [ 4]  235 	CALL	SendLetter
      0081AE                        236 SkipSec:
      0081AE CC 81 23         [ 2]  237 	JP	Delayloop
      0081B1                        238 CheckAlarm:
      0081B1 B6 51            [ 1]  239 	LD	A,0x51
      0081B3 A1 00            [ 1]  240 	CP	A,#0 ; Is in regular mode?
      0081B5 26 21            [ 1]  241 	JRNE	ChkDone
      0081B7 72 0B 00 53 1C   [ 2]  242 	BTJF	0x53,#5,ChkDone ; alarm disabled?
      0081BC B6 6B            [ 1]  243 	LD	A,0x6B ; alarm hr high
      0081BE B1 65            [ 1]  244 	CP	A,0x65
      0081C0 26 16            [ 1]  245 	JRNE	ChkDone
      0081C2 B6 6A            [ 1]  246 	LD	A,0x6A ; alarm hr low
      0081C4 B1 64            [ 1]  247 	CP	A,0x64
      0081C6 26 10            [ 1]  248 	JRNE	ChkDone
      0081C8 B6 69            [ 1]  249 	LD	A,0x69 ; alarm min high
      0081CA B1 63            [ 1]  250 	CP	A,0x63
      0081CC 26 0A            [ 1]  251 	JRNE	ChkDone
      0081CE B6 68            [ 1]  252 	LD	A,0x68 ; alarm min low
      0081D0 B1 62            [ 1]  253 	CP	A,0x62
      0081D2 26 04            [ 1]  254 	JRNE	ChkDone
      0081D4 72 1C 00 53      [ 1]  255 	BSET	0x53,#6 ; start beeping
      0081D8                        256 ChkDone:
      0081D8 81               [ 4]  257 	RET
      0081D9                        258 RegularMode:
      0081D9 72 16 00 53      [ 1]  259 	BSET	0x53,#3 ; Make sure seconds are being updated
      0081DD 72 18 00 53      [ 1]  260 	BSET	0x53,#4 ; And the clock is ticking
      0081E1 72 04 00 53 35   [ 2]  261 	BTJT	0x53,#2,SkipDispUpdReg ; Can I update Display?
      0081E6 A6 0C            [ 1]  262 	LD	A, #0b00001100; Update it: (Cursor Off)
      0081E8 CD 84 61         [ 4]  263 	CALL	PopulateShift
      0081EB CD 84 99         [ 4]  264 	CALL	SendCommand
      0081EE CD 84 40         [ 4]  265 	CALL	ClearShift
      0081F1 72 14 00 53      [ 1]  266 	BSET	0x53,#2
      0081F5 72 11 00 53      [ 1]  267 	BRES	0x53,#0
      0081F9 72 13 00 53      [ 1]  268 	BRES	0x53,#1
      0081FD 72 0B 00 53 0E   [ 2]  269 	BTJF	0x53,#5,NoAlmIcon
      008202 A6 2A            [ 1]  270 	LD	A,#0x2A
      008204 CD 84 61         [ 4]  271 	CALL	PopulateShift
      008207 CD 84 B4         [ 4]  272 	CALL	SendLetter
      00820A CD 84 D3         [ 4]  273 	CALL	Backspace
      00820D CC 82 1B         [ 2]  274 	JP	SkipDispUpdReg
      008210                        275 NoAlmIcon:
      008210 A6 20            [ 1]  276 	LD	A,#0x20
      008212 CD 84 61         [ 4]  277 	CALL	PopulateShift
      008215 CD 84 B4         [ 4]  278 	CALL	SendLetter
      008218 CD 84 D3         [ 4]  279 	CALL	Backspace
      00821B                        280 SkipDispUpdReg:
      00821B 72 0E 50 0B 36   [ 2]  281 	BTJT	0x500B,#7, SkipAlmToggle
      008220 72 03 00 52 31   [ 2]  282 	BTJF	0x52,#1, SkipAlmToggle
      008225 72 05 00 52 2C   [ 2]  283 	BTJF	0x52,#2, SkipAlmToggle
      00822A 72 13 00 52      [ 1]  284 	BRES	0x52,#1
      00822E 72 15 00 52      [ 1]  285 	BRES	0x52,#2
      008232 90 1A 00 53      [ 1]  286 	BCPL	0x53,#5 ; Toggle Alarm
      008236 72 0A 00 53 13   [ 2]  287 	BTJT	0x53,#5, EnableAlm
      00823B                        288 DisableAlm:
      00823B 72 1F 00 53      [ 1]  289 	BRES	0x53,#7
      00823F 72 1D 00 53      [ 1]  290 	BRES	0x53,#6
      008243 72 1B 00 53      [ 1]  291 	BRES	0x53,#5
      008247 72 15 00 53      [ 1]  292 	BRES	0x53,#2
      00824B CC 82 56         [ 2]  293 	JP	SkipAlmToggle
      00824E                        294 EnableAlm:
      00824E 72 1A 00 53      [ 1]  295 	BSET	0x53,#5
      008252 72 15 00 53      [ 1]  296 	BRES	0x53,#2
      008256                        297 SkipAlmToggle:
      008256 72 0C 50 0B 24   [ 2]  298 	BTJT	0x500B,#6, SkipSnooze
      00825B 72 03 00 52 1F   [ 2]  299 	BTJF	0x52,#1, SkipSnooze
      008260 72 05 00 52 1A   [ 2]  300 	BTJF	0x52,#2, SkipSnooze
      008265 72 13 00 52      [ 1]  301 	BRES	0x52,#1
      008269 72 15 00 52      [ 1]  302 	BRES	0x52,#2
      00826D 72 0D 00 53 0D   [ 2]  303 	BTJF	0x53,#6,SkipSnooze ; only snooze alarm that is going
      008272 3C 68            [ 1]  304 	INC	0x68 ; Add a minute to the alarm
      008274 CD 85 10         [ 4]  305 	CALL	AdjustTime
      008277 72 1D 00 53      [ 1]  306 	BRES	0x53,#6 ;Silence alarm
      00827B 72 1F 00 53      [ 1]  307 	BRES	0x53,#7
      00827F                        308 SkipSnooze:
      00827F 81               [ 4]  309 	RET
      008280                        310 ClockMode:
      008280 72 00 00 51 08   [ 2]  311 	BTJT	0x51,#0,AlarmSet
      008285 72 02 00 51 40   [ 2]  312 	BTJT	0x51,#1,ClockSet
      00828A CC 81 D9         [ 2]  313 	JP	RegularMode
      00828D                        314 AlarmSet:
      00828D 72 16 00 53      [ 1]  315 	BSET	0x53,#3 ; Stop Updating seconds *************************************
      008291 72 19 00 53      [ 1]  316 	BRES	0x53,#4 ; Stop Counting
      008295 72 02 00 53 2C   [ 2]  317 	BTJT	0x53,#1,SkipDispUpdAlm ; Is alarm already configured?
      00829A CD 85 CB         [ 4]  318 	CALL	DispAlarm ; No configure it
      00829D A6 0E            [ 1]  319 	LD	A, #0b00001110; Cursor On
      00829F CD 84 61         [ 4]  320 	CALL	PopulateShift
      0082A2 CD 84 99         [ 4]  321 	CALL	SendCommand
      0082A5 CD 84 40         [ 4]  322 	CALL	ClearShift
      0082A8 72 11 00 53      [ 1]  323 	BRES	0x53,#0 ; Set mode to alarm
      0082AC 72 15 00 53      [ 1]  324 	BRES	0x53,#2
      0082B0 72 12 00 53      [ 1]  325 	BSET	0x53,#1
      0082B4 A6 41            [ 1]  326 	LD	A,#0x41
      0082B6 CD 84 61         [ 4]  327 	CALL	PopulateShift
      0082B9 CD 84 B4         [ 4]  328 	CALL	SendLetter
      0082BC CD 84 D3         [ 4]  329 	CALL	Backspace
      0082BF CD 84 F7         [ 4]  330 	CALL	ReturnHome
      0082C2 3F 56            [ 1]  331 	CLR	0x56 ; Reset alarm variables
      0082C4 3F 57            [ 1]  332 	CLR	0x57
      0082C6                        333 SkipDispUpdAlm:
      0082C6 CD 83 A1         [ 4]  334 	CALL	AlarmSetLogic ; Set alarm
      0082C9 81               [ 4]  335 	RET
      0082CA                        336 ClockSet:
      0082CA 3F 56            [ 1]  337 	CLR	0x56 ; Reset cursor position
      0082CC 89               [ 2]  338 	PUSHW	X ; Perserve X
      0082CD 72 17 00 53      [ 1]  339 	BRES	0x53,#3 ; Stop updating seconds
      0082D1 72 19 00 53      [ 1]  340 	BRES	0x53,#4 ; Stop counting
      0082D5 72 00 00 53 29   [ 2]  341 	BTJT	0x53,#0,SkipDispUpdClk ; Is clock configured
      0082DA A6 0E            [ 1]  342 	LD	A, #0b00001110 ; No confiure it
      0082DC CD 84 61         [ 4]  343 	CALL	PopulateShift
      0082DF CD 84 99         [ 4]  344 	CALL	SendCommand
      0082E2 CD 84 40         [ 4]  345 	CALL	ClearShift
      0082E5 72 13 00 53      [ 1]  346 	BRES	0x53,#1
      0082E9 72 15 00 53      [ 1]  347 	BRES	0x53,#2
      0082ED 72 10 00 53      [ 1]  348 	BSET	0x53,#0
      0082F1 A6 43            [ 1]  349 	LD	A,#0x43
      0082F3 CD 84 61         [ 4]  350 	CALL	PopulateShift
      0082F6 CD 84 B4         [ 4]  351 	CALL	SendLetter
      0082F9 CD 84 D3         [ 4]  352 	CALL	Backspace
      0082FC CD 84 F7         [ 4]  353 	CALL	ReturnHome
      0082FF 3F 54            [ 1]  354 	CLR	0x54 ; Reset cursor
      008301 3F 55            [ 1]  355 	CLR	0x55
      008303                        356 SkipDispUpdClk:
      008303 72 0E 50 0B 3C   [ 2]  357 	BTJT	0x500B,#7,SkipClkMov ; Button Press?
      008308 72 03 00 52 37   [ 2]  358 	BTJF	0x52,#1,SkipClkMov
      00830D 72 05 00 52 32   [ 2]  359 	BTJF	0x52,#2,SkipClkMov
      008312 72 13 00 52      [ 1]  360 	BRES	0x52,#1 ; Debounce
      008316 72 15 00 52      [ 1]  361 	BRES	0x52,#2 ; Debounce
      00831A 3C 54            [ 1]  362 	INC	0x54 ; Cursor move logic ==================
      00831C 3C 55            [ 1]  363 	INC	0x55
      00831E B6 54            [ 1]  364 	LD	A,0x54
      008320 A1 02            [ 1]  365 	CP	A,#2
      008322 27 0B            [ 1]  366 	JREQ	MovTwo
      008324 A1 05            [ 1]  367 	CP	A,#5
      008326 27 07            [ 1]  368 	JREQ	MovTwo
      008328 A1 08            [ 1]  369 	CP	A,#8
      00832A 27 0E            [ 1]  370 	JREQ	ToStart
      00832C CC 83 34         [ 2]  371 	JP	MovOne
      00832F                        372 MovTwo:
      00832F CD 84 EB         [ 4]  373 	CALL	MovRight
      008332 3C 54            [ 1]  374 	INC	0x54
      008334                        375 MovOne:
      008334 CD 84 EB         [ 4]  376 	CALL	MovRight
      008337 CC 83 44         [ 2]  377 	JP	SkipClkMov
      00833A                        378 ToStart:
      00833A 3F 54            [ 1]  379 	CLR	0x54
      00833C 3F 55            [ 1]  380 	CLR	0x55
      00833E CD 84 EB         [ 4]  381 	CALL	MovRight
      008341 CD 84 F7         [ 4]  382 	CALL	ReturnHome
      008344                        383 SkipClkMov:
                                    384 ; Clock change logic ========================
      008344 72 0C 50 0B 56   [ 2]  385 	BTJT	0x500B,#6,SkipClkInc
      008349 72 03 00 52 51   [ 2]  386 	BTJF	0x52,#1,SkipClkInc
      00834E 72 05 00 52 4C   [ 2]  387 	BTJF	0x52,#2,SkipClkInc
      008353 72 13 00 52      [ 1]  388 	BRES	0x52,#1
      008357 72 15 00 52      [ 1]  389 	BRES	0x52,#2
      00835B A6 05            [ 1]  390 	LD	A, #5
      00835D B0 55            [ 1]  391 	SUB	A, 0x55
      00835F 5F               [ 1]  392 	CLRW	X
      008360 41               [ 1]  393 	EXG	A,XL
      008361 6C 60            [ 1]  394 	INC	(0x60,X)
      008363 B6 55            [ 1]  395 	LD	A, 0x55
      008365 A1 00            [ 1]  396 	CP	A,#0
      008367 27 1E            [ 1]  397 	JREQ	AdjTwo
      008369 A1 02            [ 1]  398 	CP	A,#2
      00836B 27 0F            [ 1]  399 	JREQ	AdjSix
      00836D A1 04            [ 1]  400 	CP	A,#4
      00836F 27 0B            [ 1]  401 	JREQ	AdjSix
      008371 E6 60            [ 1]  402 	LD	A,(0x60,X)
      008373 A1 0A            [ 1]  403 	CP	A,#10
      008375 26 18            [ 1]  404 	JRNE	PrintVal
      008377 6F 60            [ 1]  405 	CLR	(0x60,X)
      008379 CC 83 8F         [ 2]  406 	JP	PrintVal
      00837C                        407 AdjSix:
      00837C E6 60            [ 1]  408 	LD	A,(0x60,X)
      00837E A1 06            [ 1]  409 	CP	A,#6
      008380 26 0D            [ 1]  410 	JRNE	PrintVal
      008382 6F 60            [ 1]  411 	CLR	(0x60,X)
      008384 CC 83 8F         [ 2]  412 	JP	PrintVal
      008387                        413 AdjTwo:
      008387 E6 60            [ 1]  414 	LD	A,(0x60,X)
      008389 A1 03            [ 1]  415 	CP	A,#3
      00838B 26 02            [ 1]  416 	JRNE	PrintVal
      00838D 6F 60            [ 1]  417 	CLR	(0x60,X)
      00838F                        418 PrintVal:
      00838F E6 60            [ 1]  419 	LD	A,(0x60,X)
      008391 AB 30            [ 1]  420 	ADD	A,#48 ; Adjust to ASCII
      008393 CD 84 61         [ 4]  421 	CALL	PopulateShift
      008396 CD 84 B4         [ 4]  422 	CALL	SendLetter
      008399 CD 84 D3         [ 4]  423 	CALL	Backspace
      00839C CD 84 40         [ 4]  424 	CALL	ClearShift
      00839F                        425 SkipClkInc:
      00839F 85               [ 2]  426 	POPW	X
      0083A0 81               [ 4]  427 	RET
      0083A1                        428 AlarmSetLogic:
      0083A1 89               [ 2]  429 	PUSHW	X
      0083A2 72 0E 50 0B 3C   [ 2]  430 	BTJT	0x500B,#7,SkipAlmMov ; Is move button pressed?
      0083A7 72 03 00 52 37   [ 2]  431 	BTJF	0x52,#1,SkipAlmMov
      0083AC 72 05 00 52 32   [ 2]  432 	BTJF	0x52,#2,SkipAlmMov
      0083B1 72 13 00 52      [ 1]  433 	BRES	0x52,#1
      0083B5 72 15 00 52      [ 1]  434 	BRES	0x52,#2
      0083B9 3C 56            [ 1]  435 	INC	0x56 ; Inc cursor position
      0083BB 3C 57            [ 1]  436 	INC	0x57 ; inc number index
      0083BD B6 56            [ 1]  437 	LD	A,0x56
      0083BF A1 02            [ 1]  438 	CP	A,#2 ; Is Colon?
      0083C1 27 0B            [ 1]  439 	JREQ	MovTwoAlm ; Yes Move Two
      0083C3 A1 05            [ 1]  440 	CP	A,#5 ; Is Colon?
      0083C5 27 07            [ 1]  441 	JREQ	MovTwoAlm ; Yes Move Two
      0083C7 A1 08            [ 1]  442 	CP	A,#8 ; Is at end?
      0083C9 27 0E            [ 1]  443 	JREQ	ToStartAlm ; Yes reset Cursor
      0083CB CC 83 D3         [ 2]  444 	JP	MovOneAlm ; Else move cursor Regularly
      0083CE                        445 MovTwoAlm:
      0083CE CD 84 EB         [ 4]  446 	CALL	MovRight
      0083D1 3C 56            [ 1]  447 	INC	0x56
      0083D3                        448 MovOneAlm:
      0083D3 CD 84 EB         [ 4]  449 	CALL	MovRight
      0083D6 CC 83 E3         [ 2]  450 	JP	SkipAlmMov
      0083D9                        451 ToStartAlm:
      0083D9 3F 56            [ 1]  452 	CLR	0x56
      0083DB 3F 57            [ 1]  453 	CLR	0x57
      0083DD CD 84 EB         [ 4]  454 	CALL	MovRight ; Adjust to correct position before moving home
      0083E0 CD 84 F7         [ 4]  455 	CALL	ReturnHome
      0083E3                        456 SkipAlmMov:
      0083E3 72 0C 50 0B 56   [ 2]  457 	BTJT	0x500B,#6,SkipAlmInc ; Is increment button pressed?
      0083E8 72 03 00 52 51   [ 2]  458 	BTJF	0x52,#1,SkipAlmInc
      0083ED 72 05 00 52 4C   [ 2]  459 	BTJF	0x52,#2,SkipAlmInc
      0083F2 72 13 00 52      [ 1]  460 	BRES	0x52,#1
      0083F6 72 15 00 52      [ 1]  461 	BRES	0x52,#2
      0083FA A6 05            [ 1]  462 	LD	A, #5
      0083FC B0 57            [ 1]  463 	SUB	A, 0x57 ; Adjust for registers being revesed from inx. (Hrs->sec vs sec->hrs)
      0083FE 5F               [ 1]  464 	CLRW	X
      0083FF 41               [ 1]  465 	EXG	A,XL
      008400 6C 66            [ 1]  466 	INC	(0x66,X) ; Adjust each alarm register
      008402 B6 57            [ 1]  467 	LD	A, 0x57 ; Below just checks for overflow
      008404 A1 00            [ 1]  468 	CP	A,#0
      008406 27 1E            [ 1]  469 	JREQ	AdjTwoAlm
      008408 A1 02            [ 1]  470 	CP	A,#2
      00840A 27 0F            [ 1]  471 	JREQ	AdjSixAlm
      00840C A1 04            [ 1]  472 	CP	A,#4
      00840E 27 0B            [ 1]  473 	JREQ	AdjSixAlm
      008410 E6 66            [ 1]  474 	LD	A,(0x66,X)
      008412 A1 0A            [ 1]  475 	CP	A,#10
      008414 26 18            [ 1]  476 	JRNE	PrintValAlm
      008416 6F 66            [ 1]  477 	CLR	(0x66,X)
      008418 CC 84 2E         [ 2]  478 	JP	PrintValAlm
      00841B                        479 AdjSixAlm:
      00841B E6 66            [ 1]  480 	LD	A,(0x66,X)
      00841D A1 06            [ 1]  481 	CP	A,#6
      00841F 26 0D            [ 1]  482 	JRNE	PrintValAlm
      008421 6F 66            [ 1]  483 	CLR	(0x66,X)
      008423 CC 84 2E         [ 2]  484 	JP	PrintValAlm
      008426                        485 AdjTwoAlm:
      008426 E6 66            [ 1]  486 	LD	A,(0x66,X)
      008428 A1 03            [ 1]  487 	CP	A,#3
      00842A 26 02            [ 1]  488 	JRNE	PrintValAlm
      00842C 6F 66            [ 1]  489 	CLR	(0x66,X)
      00842E                        490 PrintValAlm:
                                    491 ; Throw new value to display
      00842E E6 66            [ 1]  492 	LD	A,(0x66,X)
      008430 AB 30            [ 1]  493 	ADD	A,#48 ; Adjust to ASCII
      008432 CD 84 61         [ 4]  494 	CALL	PopulateShift
      008435 CD 84 B4         [ 4]  495 	CALL	SendLetter
      008438 CD 84 D3         [ 4]  496 	CALL	Backspace
      00843B CD 84 40         [ 4]  497 	CALL	ClearShift
      00843E                        498 SkipAlmInc:
      00843E 85               [ 2]  499 	POPW	X
      00843F 81               [ 4]  500 	RET
                                    501 ;	Clears the shift register
      008440                        502 ClearShift:
      008440 88               [ 1]  503 	PUSH	A
      008441 A6 08            [ 1]  504 	LD	A,#8
      008443 72 13 50 0F      [ 1]  505 	BRES	0x500F, #1
      008447                        506 ClearLoop:
      008447 72 16 50 0F      [ 1]  507 	BSET	0x500F, #3
      00844B 9D               [ 1]  508 	NOP
      00844C 72 17 50 0F      [ 1]  509 	BRES	0x500F, #3
      008450 4A               [ 1]  510 	DEC	A
      008451 27 03            [ 1]  511 	JREQ	ClearDone
      008453 CC 84 47         [ 2]  512 	JP	ClearLoop
      008456                        513 ClearDone:
      008456 72 14 50 0F      [ 1]  514 	BSET	0x500F, #2
      00845A 9D               [ 1]  515 	NOP
      00845B 72 15 50 0F      [ 1]  516 	BRES	0x500F, #2
      00845F 84               [ 1]  517 	POP	A
      008460 81               [ 4]  518 	RET
                                    519 ;	Populates shift register with value in A
      008461                        520 PopulateShift:
      008461 CD 86 4E         [ 4]  521 	CALL	DumbDelay
      008464 89               [ 2]  522 	PUSHW	X
      008465 B7 50            [ 1]  523 	LD	0x50, A
      008467 AE 00 08         [ 2]  524 	LDW	X,#8
      00846A                        525 PopLoop:
      00846A 27 22            [ 1]  526 	JREQ	PopDone
      00846C 72 0E 00 50 03   [ 2]  527 	BTJT	0x50,#7,ShiftO
      008471 CC 84 7B         [ 2]  528 	JP	ShiftZ
      008474                        529 ShiftO:
      008474 72 12 50 0F      [ 1]  530 	BSET	0x500F, #1
      008478 CC 84 7F         [ 2]  531 	JP	PulseClk
      00847B                        532 ShiftZ:
      00847B 72 13 50 0F      [ 1]  533 	BRES	0x500F, #1
      00847F                        534 PulseClk:
      00847F 72 16 50 0F      [ 1]  535 	BSET	0x500F, #3
      008483 9D               [ 1]  536 	NOP
      008484 72 17 50 0F      [ 1]  537 	BRES	0x500F, #3
      008488 38 50            [ 1]  538 	SLL	0x50
      00848A 5A               [ 2]  539 	DECW	X
      00848B CC 84 6A         [ 2]  540 	JP	PopLoop
      00848E                        541 PopDone:
      00848E 72 14 50 0F      [ 1]  542 	BSET	0x500F, #2
      008492 9D               [ 1]  543 	NOP
      008493 72 15 50 0F      [ 1]  544 	BRES	0x500F, #2
      008497 85               [ 2]  545 	POPW	X
      008498 81               [ 4]  546 	ret
      008499                        547 SendCommand:
      008499 72 1B 50 0F      [ 1]  548 	BRES	0x500F, #5 ; Clear RS
      00849D 72 1D 50 0F      [ 1]  549 	BRES	0x500F, #6 ; Clear E
      0084A1 72 1C 50 0F      [ 1]  550 	BSET	0x500F, #6 ; Set E to send instruction
      0084A5 9D               [ 1]  551 	NOP
      0084A6 9D               [ 1]  552 	NOP
      0084A7 9D               [ 1]  553 	NOP
      0084A8 72 1D 50 0F      [ 1]  554 	BRES	0x500F, #6 ; Clear E
      0084AC 9D               [ 1]  555 	NOP
      0084AD 9D               [ 1]  556 	NOP
      0084AE 9D               [ 1]  557 	NOP	; Little delay
      0084AF 72 1B 50 0F      [ 1]  558 	BRES	0x500F, #5 ; Clear RS
      0084B3 81               [ 4]  559 	RET
      0084B4                        560 SendLetter:
      0084B4 72 1B 50 0F      [ 1]  561 	BRES	0x500F, #5 ; Clear RS
      0084B8 72 1D 50 0F      [ 1]  562 	BRES	0x500F, #6 ; Clear E
      0084BC 72 1C 50 0F      [ 1]  563 	BSET	0x500F, #6 ; Set E to send instruction
      0084C0 72 1A 50 0F      [ 1]  564 	BSET	0x500F, #5 ; Set RS
      0084C4 9D               [ 1]  565 	NOP
      0084C5 9D               [ 1]  566 	NOP
      0084C6 9D               [ 1]  567 	NOP
      0084C7 72 1D 50 0F      [ 1]  568 	BRES	0x500F, #6 ; Clear E
      0084CB 9D               [ 1]  569 	NOP
      0084CC 9D               [ 1]  570 	NOP
      0084CD 9D               [ 1]  571 	NOP
      0084CE 72 1B 50 0F      [ 1]  572 	BRES	0x500F, #5 ; Clear RS
      0084D2 81               [ 4]  573 	RET
      0084D3                        574 Backspace:
      0084D3 CD 84 40         [ 4]  575 	CALL	ClearShift
      0084D6 A6 10            [ 1]  576 	LD	A, #0b00010000; Shift cursor left
      0084D8 CD 84 61         [ 4]  577 	CALL	PopulateShift
      0084DB CD 84 99         [ 4]  578 	CALL	SendCommand
      0084DE 81               [ 4]  579 	RET
      0084DF                        580 ClearScreen:
      0084DF A6 01            [ 1]  581 	LD	A, #1 ; Clear Display
      0084E1 CD 84 61         [ 4]  582 	CALL	PopulateShift
      0084E4 CD 84 99         [ 4]  583 	CALL	SendCommand
      0084E7 CD 84 40         [ 4]  584 	CALL	ClearShift
      0084EA 81               [ 4]  585 	RET
      0084EB                        586 MovRight:
      0084EB CD 84 40         [ 4]  587 	CALL	ClearShift
      0084EE A6 14            [ 1]  588 	LD	A, #0b00010100
      0084F0 CD 84 61         [ 4]  589 	CALL	PopulateShift
      0084F3 CD 84 99         [ 4]  590 	CALL	SendCommand
      0084F6 81               [ 4]  591 	RET
      0084F7                        592 ReturnHome:
      0084F7 CD 84 D3         [ 4]  593 	CALL	Backspace
      0084FA CD 84 D3         [ 4]  594 	CALL	Backspace
      0084FD CD 84 D3         [ 4]  595 	CALL	Backspace
      008500 CD 84 D3         [ 4]  596 	CALL	Backspace
      008503 CD 84 D3         [ 4]  597 	CALL	Backspace
      008506 CD 84 D3         [ 4]  598 	CALL	Backspace
      008509 CD 84 D3         [ 4]  599 	CALL	Backspace
      00850C CD 84 D3         [ 4]  600 	CALL	Backspace
      00850F 81               [ 4]  601 	RET
      008510                        602 AdjustTime:
      008510 B6 60            [ 1]  603 	LD	A,0x60 ; Are seconds low (00:00:0X) overflow?
      008512 A1 0A            [ 1]  604 	CP	A,#10
      008514 26 40            [ 1]  605 	JRNE	AdjTimeDoneSec
      008516                        606 SecLowOv:
      008516 3C 61            [ 1]  607 	INC	0x61 ; Yes inc sec high (00:00:X0)
      008518 B6 61            [ 1]  608 	LD	A,0x61
      00851A 3F 60            [ 1]  609 	CLR	0x60 ; Are sec high overflow?
      00851C A1 06            [ 1]  610 	CP	A,#6
      00851E 26 32            [ 1]  611 	JRNE	AdjTimeDone
      008520                        612 SecHighOver:
      008520 3C 62            [ 1]  613 	INC	0x62 ; Are mins low over?
      008522 B6 62            [ 1]  614 	LD	A,0x62
      008524 3F 61            [ 1]  615 	CLR	0x61
      008526 A1 0A            [ 1]  616 	CP	A,#10
      008528 26 28            [ 1]  617 	JRNE	AdjTimeDone
      00852A                        618 MinLowOver:
      00852A 3C 63            [ 1]  619 	INC	0x63 ; Are mins high over?
      00852C B6 63            [ 1]  620 	LD	A,0x63
      00852E 3F 62            [ 1]  621 	CLR	0x62
      008530 A1 06            [ 1]  622 	CP	A,#6
      008532 26 1E            [ 1]  623 	JRNE	AdjTimeDone
      008534                        624 MinHighOver:
      008534 3C 64            [ 1]  625 	INC	0x64 ; hour low++
      008536 3F 63            [ 1]  626 	CLR	0x63 ; min high = 0
                                    627 ;	carry hour low if it hit 10
      008538 B6 64            [ 1]  628 	LD	A,0x64
      00853A A1 0A            [ 1]  629 	CP	A,#10
      00853C 26 04            [ 1]  630 	JRNE	Check24
      00853E 3F 64            [ 1]  631 	CLR	0x64
      008540 3C 65            [ 1]  632 	INC	0x65
      008542                        633 Check24:
                                    634 ;	reset only at 24:00
      008542 B6 65            [ 1]  635 	LD	A,0x65
      008544 A1 02            [ 1]  636 	CP	A,#2
      008546 26 0A            [ 1]  637 	JRNE	AdjTimeDone
      008548 B6 64            [ 1]  638 	LD	A,0x64
      00854A A1 04            [ 1]  639 	CP	A,#4
      00854C 26 04            [ 1]  640 	JRNE	AdjTimeDone
      00854E 3F 65            [ 1]  641 	CLR	0x65
      008550 3F 64            [ 1]  642 	CLR	0x64
      008552                        643 AdjTimeDone:
      008552 CD 85 57         [ 4]  644 	CALL	ResetClk
      008555 81               [ 4]  645 	RET
      008556                        646 AdjTimeDoneSec:
      008556 81               [ 4]  647 	RET
      008557                        648 ResetClk:
      008557 CD 84 DF         [ 4]  649 	CALL	ClearScreen
      00855A B6 65            [ 1]  650 	LD	A, 0x65
      00855C AB 30            [ 1]  651 	ADD	A,#48 ; Adjust to ASCII
      00855E CD 84 61         [ 4]  652 	CALL	PopulateShift
      008561 CD 84 B4         [ 4]  653 	CALL	SendLetter
      008564 CD 84 40         [ 4]  654 	CALL	ClearShift
      008567 B6 64            [ 1]  655 	LD	A, 0x64
      008569 AB 30            [ 1]  656 	ADD	A,#48 ; Adjust to ASCII
      00856B CD 84 61         [ 4]  657 	CALL	PopulateShift
      00856E CD 84 B4         [ 4]  658 	CALL	SendLetter
      008571 CD 84 40         [ 4]  659 	CALL	ClearShift
      008574 A6 3A            [ 1]  660 	LD	A,#0x3A
      008576 CD 84 61         [ 4]  661 	CALL	PopulateShift
      008579 CD 84 B4         [ 4]  662 	CALL	SendLetter
      00857C CD 84 40         [ 4]  663 	CALL	ClearShift
      00857F B6 63            [ 1]  664 	LD	A, 0x63
      008581 AB 30            [ 1]  665 	ADD	A,#48 ; Adjust to ASCII
      008583 CD 84 61         [ 4]  666 	CALL	PopulateShift
      008586 CD 84 B4         [ 4]  667 	CALL	SendLetter
      008589 CD 84 40         [ 4]  668 	CALL	ClearShift
      00858C B6 62            [ 1]  669 	LD	A, 0x62
      00858E AB 30            [ 1]  670 	ADD	A,#48 ; Adjust to ASCII
      008590 CD 84 61         [ 4]  671 	CALL	PopulateShift
      008593 CD 84 B4         [ 4]  672 	CALL	SendLetter
      008596 CD 84 40         [ 4]  673 	CALL	ClearShift
      008599 A6 3A            [ 1]  674 	LD	A,#0x3A
      00859B CD 84 61         [ 4]  675 	CALL	PopulateShift
      00859E CD 84 B4         [ 4]  676 	CALL	SendLetter
      0085A1 CD 84 40         [ 4]  677 	CALL	ClearShift
      0085A4 B6 61            [ 1]  678 	LD	A, 0x61
      0085A6 AB 30            [ 1]  679 	ADD	A,#48 ; Adjust to ASCII
      0085A8 CD 84 61         [ 4]  680 	CALL	PopulateShift
      0085AB CD 84 B4         [ 4]  681 	CALL	SendLetter
      0085AE CD 84 40         [ 4]  682 	CALL	ClearShift
      0085B1 B6 60            [ 1]  683 	LD	A, 0x60
      0085B3 AB 30            [ 1]  684 	ADD	A,#48 ; Adjust to ASCII
      0085B5 CD 84 61         [ 4]  685 	CALL	PopulateShift
      0085B8 CD 84 B4         [ 4]  686 	CALL	SendLetter
      0085BB CD 84 40         [ 4]  687 	CALL	ClearShift
      0085BE 72 11 00 53      [ 1]  688 	BRES	0x53,#0
      0085C2 72 13 00 53      [ 1]  689 	BRES	0x53,#1
      0085C6 72 15 00 53      [ 1]  690 	BRES	0x53,#2
                                    691 ;CALL	ClockMode
      0085CA 81               [ 4]  692 	RET
      0085CB                        693 DispAlarm:
      0085CB CD 84 DF         [ 4]  694 	CALL	ClearScreen
      0085CE B6 6B            [ 1]  695 	LD	A, 0x6B
      0085D0 AB 30            [ 1]  696 	ADD	A,#48 ; Adjust to ASCII
      0085D2 CD 84 61         [ 4]  697 	CALL	PopulateShift
      0085D5 CD 84 B4         [ 4]  698 	CALL	SendLetter
      0085D8 CD 84 40         [ 4]  699 	CALL	ClearShift
      0085DB B6 6A            [ 1]  700 	LD	A, 0x6A
      0085DD AB 30            [ 1]  701 	ADD	A,#48 ; Adjust to ASCII
      0085DF CD 84 61         [ 4]  702 	CALL	PopulateShift
      0085E2 CD 84 B4         [ 4]  703 	CALL	SendLetter
      0085E5 CD 84 40         [ 4]  704 	CALL	ClearShift
      0085E8 A6 3A            [ 1]  705 	LD	A,#0x3A
      0085EA CD 84 61         [ 4]  706 	CALL	PopulateShift
      0085ED CD 84 B4         [ 4]  707 	CALL	SendLetter
      0085F0 CD 84 40         [ 4]  708 	CALL	ClearShift
      0085F3 B6 69            [ 1]  709 	LD	A, 0x69
      0085F5 AB 30            [ 1]  710 	ADD	A,#48 ; Adjust to ASCII
      0085F7 CD 84 61         [ 4]  711 	CALL	PopulateShift
      0085FA CD 84 B4         [ 4]  712 	CALL	SendLetter
      0085FD CD 84 40         [ 4]  713 	CALL	ClearShift
      008600 B6 68            [ 1]  714 	LD	A, 0x68
      008602 AB 30            [ 1]  715 	ADD	A,#48 ; Adjust to ASCII
      008604 CD 84 61         [ 4]  716 	CALL	PopulateShift
      008607 CD 84 B4         [ 4]  717 	CALL	SendLetter
      00860A CD 84 40         [ 4]  718 	CALL	ClearShift
      00860D A6 3A            [ 1]  719 	LD	A,#0x3A
      00860F CD 84 61         [ 4]  720 	CALL	PopulateShift
      008612 CD 84 B4         [ 4]  721 	CALL	SendLetter
      008615 CD 84 40         [ 4]  722 	CALL	ClearShift
      008618 B6 67            [ 1]  723 	LD	A, 0x67
      00861A AB 30            [ 1]  724 	ADD	A,#48 ; Adjust to ASCII
      00861C CD 84 61         [ 4]  725 	CALL	PopulateShift
      00861F CD 84 B4         [ 4]  726 	CALL	SendLetter
      008622 CD 84 40         [ 4]  727 	CALL	ClearShift
      008625 B6 66            [ 1]  728 	LD	A, 0x66
      008627 AB 30            [ 1]  729 	ADD	A,#48 ; Adjust to ASCII
      008629 CD 84 61         [ 4]  730 	CALL	PopulateShift
      00862C CD 84 B4         [ 4]  731 	CALL	SendLetter
      00862F CD 84 40         [ 4]  732 	CALL	ClearShift
      008632 72 11 00 53      [ 1]  733 	BRES	0x53,#0
      008636 72 13 00 53      [ 1]  734 	BRES	0x53,#1
      00863A 72 15 00 53      [ 1]  735 	BRES	0x53,#2
                                    736 ;CALL	ClockMode
      00863E 81               [ 4]  737 	RET
      00863F                        738 MakeBeep:
      00863F 72 0F 00 53 05   [ 2]  739 	BTJF	0x53,#7,SkipBeep
      008644 90 18 50 0F      [ 1]  740 	BCPL	0x500F, #4 ; Toggle Port D, Pin 4 (Creates 500Hz Tone)
      008648 81               [ 4]  741 	RET
      008649                        742 SkipBeep:
      008649 72 19 50 0F      [ 1]  743 	BRES	0x500F,#4
      00864D 81               [ 4]  744 	RET
      00864E                        745 DumbDelay:
      00864E 89               [ 2]  746 	PUSHW	X
      00864F AE FF FF         [ 2]  747 	LDW	X, #0xFFFF
      008652                        748 DumbLoop:
      008652 5A               [ 2]  749 	DECW	X
      008653 27 03            [ 1]  750 	JREQ	DumbDone
      008655 CC 86 52         [ 2]  751 	JP	DumbLoop
      008658                        752 DumbDone:
      008658 85               [ 2]  753 	POPW	X
      008659 81               [ 4]  754 	RET
                                    755 ;	main.c: 756: }
      00865A 81               [ 4]  756 	ret
                                    757 	.area CODE
                                    758 	.area CONST
                                    759 	.area INITIALIZER
                                    760 	.area CABS (ABS)
