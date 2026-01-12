/*
 * Time Stuff
 * 50C6 Master Clock divider
 * 5250 Tim1 Cr1
 * 5260
 *
 * 5300 Tim2 CR1
 * 530E Tim2 Prscl
 * 530F Tim2 AARH
 *
 * GPIO 
 * PA:
 *  ODR 5000
 *  IDR 5001
 *  DDR 5002
 *  CR1 5003
 *  CR2 5004
 * PD:
 *  ODR 500F
 *  IDR 5010
 *  DDR 5011
 *  CR1 5012
 *  CR2 5013
 *
 *  ; Shift Reg
 *  ; SER -> D1
 *  ; RCLK -> D2
 *  ; SCLK -> D3
 *
 *  ; LCD Reg 
 *  ; RS -> D5
 *  ; RW -> GND 
 *  ; E -> D6 
 */


void main(void){
    __asm
    Setup:
        ; Misc Variables
        CLR 0x51 ; Regular Mode = 0, AlarmSet = 1, TimeSet = 2;
        BSET 0x52,#1 ; Allows button pressses to be registered. Cleared when button pressed, set every second
        CLR 0x53 ; BeeperOn|AlmGoing|AlmSet|Count|UpdateSec|R Disp|A Disp|C Disp|
        BSET 0x53,#3 ; UpdateSec Enabled on start
        BSET 0x53,#4 ; Count Enabled on start
        BSET 0x53,#5 ; Alm Enabled on start
        CLR 0x54 ; Used toAlm  hold cursor position on clk change
        CLR 0x55 ; Index to hold what number to increment
        CLR 0x56 ; Used toAlm  hold cursor position on clk change
        CLR 0x57 ; Index to hold what number to increment
        ; Timer setup 
        MOV 0x50C6, #0x00 ; Set clock to 16mhz
        MOV 0x5250, #0b00010000 ; Set clock registers
        MOV 0x5260, #0x00 ; Dont divide clock
        MOV 0x5261, #0x0F ; Dont divide clock
        MOV 0x5262, #0x4E ; ARRH
        MOV 0x5263, #0x20; ARRL

        MOV 5300, #0x00; Set clock registers
        MOV 0x530E, #0x04
        MOV 0x530F, #0x03 ; ARRH
        MOV 0x5310, #0xE8 ; ARRL
        BRES 0x5304, #0
        BSET 0x5300, #0
        CLR 0x503F


        ; Clock Settings  =====================================================
        LDW X,#1000 ; Timer for 1s 
        CLR 0x60 ; Sec Low
        CLR 0x61 ; Sec High
        CLR 0x62 ; Min Low
        CLR 0x63 ; Min High
        MOV 0x64,#2 ; Hr LOW 
        MOV 0x65,#1 ; Hr High Set to 12oclock

        CLR 0x66 ; ASec Low
        CLR 0x67 ; ASec High
        CLR 0x68 ; AMin Low
        CLR 0x69 ; AMin High
        CLR 0x6A ; AHr Low
        CLR 0x6B ; AHr High ; Set alarm to midnight
        ; Gpio Setup ; ========================================================
        MOV 0x5002, #0xFF
        MOV 0x5003, #0xFF

        MOV 0x5011, #0xFF
        MOV 0x5012, #0XFF

        MOV 0x500D, #0xFF
        
        ; LCD Setup ===========================================================
        CALL ClearShift
        LD A, #0b00110000; Fx Set Cmd
        CALL PopulateShift
        CALL SendCommand

        CALL ClearShift
        LD A, #0b00001100; Display On
        CALL PopulateShift
        CALL SendCommand
        CALL ClearShift

        LD A, #0b00000110; Entry mode Set
        CALL PopulateShift
        CALL SendCommand
        CALL ClearShift

        CALL ClearScreen
        CALL ResetClk
        

    Delayloop:
        BTJT 0x500B,#5,Skip ; If pressed changes what mode user is in
        BTJF 0x52,#1,Skip
        CALL ResetClk
        CLR 0x54 ; Reset Cursor Pos
        BRES 0x52,#1
        INC 0x51 ; Change Mode If button is hit
        BRES 0x53,#0
        BRES 0x53,#1
        BRES 0x53,#2
        LD A,0x51 ; Check what mode user is currently in
        CP A,#3
        JRNE Skip ; Overflow to Normal mode
        CLR 0x51
    Skip:
        BTJF 0x5304, #0, Delayloop ; Wait for a ms
        BRES 0x5304, #0 ; Reset UIF
        BTJF 0x53,#6,NoBeep
        CALL MakeBeep 
    NoBeep:
        CALL ClockMode ; Goto Clock Logic
        DECW X ; Keep track of timing
        JREQ EndLoop
        JP Delayloop
    EndLoop:
        BSET 0x52,#1 ; Second over allow for button presses again
        BRES 0x5304, #0 ; Reset UIF
        BCPL 0x53,#7 ; Toggle alm every second
        LDW X, #1000 ; Reset timing register 
        BTJF 0x53,#4,SkipSec ; Skip incrementation if changing clock
        INC 0x60 ; Increment seconds
        BTJT 0x53,#1,SkipAdj
        CALL AdjustTime ; Adjust for time overflows
        CALL CheckAlarm
    SkipAdj:
        BTJF 0x53,#3,SkipSec ; Skip updating display if user is in alarm mode (keeps track in background)
        CALL Backspace  ; Print seconds
        LD A, 0x60
        ADD A,#48 ; Adjust to ASCII
        CALL PopulateShift
        CALL SendLetter
    SkipSec:
        JP Delayloop

CheckAlarm:
    BTJF 0x53,#5,ChkDone         ; alarm disabled?

    LD  A,0x6B   ; alarm hr high
    CP  A,0x65
    JRNE ChkDone
    LD  A,0x6A   ; alarm hr low
    CP  A,0x64
    JRNE ChkDone
    LD  A,0x69   ; alarm min high
    CP  A,0x63
    JRNE ChkDone
    LD  A,0x68   ; alarm min low
    CP  A,0x62
    JRNE ChkDone
    BSET 0x53,#6             ; start beeping
ChkDone:
    RET

    RegularMode:
        CLR 0x56
        BSET 0x53,#3 ; Make sure seconds are being updated
        BSET 0x53,#4 ; And the clock is ticking
        BTJT 0x53,#2,SkipDispUpdReg ; Can I update Display?
        LD A, #0b00001100; Update it: (Cursor Off)
        CALL PopulateShift
        CALL SendCommand
        CALL ClearShift
        BSET 0x53,#2
        BRES 0x53,#0
        BRES 0x53,#1
        BTJF 0x53,#5,NoAlmIcon
        LD A,#0x2A 
        CALL PopulateShift
        CALL SendLetter
        CALL Backspace
        JP SkipDispUpdReg
    NoAlmIcon:
        LD A,#0x20 
        CALL PopulateShift
        CALL SendLetter
        CALL Backspace
    SkipDispUpdReg:
        BTJT 0x500B,#7, SkipAlmToggle
        BTJF 0x52,#1, SkipAlmToggle
        BRES 0x52,#1
        BCPL 0x53,#5 ; Toggle Alarm
        BTJT 0x53,#5, EnableAlm
    DisableAlm:
        BRES 0x53,#5
        BRES 0x53,#2
        JP SkipAlmToggle
    EnableAlm:
        BSET 0x53,#5
        BRES 0x53,#2
    SkipAlmToggle:
        BTJT 0x500B,#6, SkipSnooze
        BTJF 0x52,#1, SkipSnooze
        BRES 0x52,#1
        BRES 0x53,#6 ;Snooze alarm 
        BRES 0x53,#7
    SkipSnooze: 
        RET
    ClockMode:
        BTJT 0x51,#0,AlarmSet
        BTJT 0x51,#1,ClockSet
        JP RegularMode
    AlarmSet:
        BRES 0x53,#3
        BRES 0x53,#4
        BTJT 0x53,#1,SkipDispUpdAlm
        CALL DispAlarm
        LD A, #0b00001110; Cursor On
        CALL PopulateShift
        CALL SendCommand
        CALL ClearShift
        BRES 0x53,#0 ; Set mode to alarm
        BRES 0x53,#2
        BSET 0x53,#1
        LD A,#0x41
        CALL PopulateShift
        CALL SendLetter
        CALL Backspace
        CALL ReturnHome
        CLR 0x54
        CLR 0x55
    SkipDispUpdAlm:
        CALL AlarmSetLogic
        RET
    ClockSet:
        CLR 0x56
        PUSHW X
        BRES 0x53,#3
        BRES 0x53,#4
        BTJT 0x53,#0,SkipDispUpdClk
        LD A, #0b00001110
        CALL PopulateShift
        CALL SendCommand
        CALL ClearShift
        BRES 0x53,#1
        BRES 0x53,#2
        BSET 0x53,#0
        LD A,#0x43
        CALL PopulateShift
        CALL SendLetter
        CALL Backspace
        CALL ReturnHome
        CLR 0x54
        CLR 0x55
    SkipDispUpdClk:
        BTJT 0x500B,#7,SkipClkMov
        BTJF 0x52,#1,SkipClkMov
        BRES 0x52,#1
        INC 0x54
        INC 0x55
        LD A,0x54
        CP A,#2
        JREQ MovTwo
        CP A,#5
        JREQ MovTwo
        CP A,#8
        JREQ ToStart
        JP MovOne
    MovTwo:
        CALL MovRight
        INC 0x54
    MovOne:
        CALL MovRight
        JP SkipClkMov
    ToStart:
        CLR 0x54
        CLR 0x55
        CALL MovRight
        CALL ReturnHome
    SkipClkMov:
        BTJT 0x500B,#6,SkipClkInc
        BTJF 0x52,#1,SkipClkInc
        BRES 0x52,#1
        LD A, #5
        SUB A, 0x55 
        CLRW X
        EXG A,XL
        INC (0x60,X)
        LD A, 0x55
        CP A,#0
        JREQ AdjTwo
        CP A,#2
        JREQ AdjSix
        CP A,#4
        JREQ AdjSix
        LD A,(0x60,X)
        CP A,#10
        JRNE PrintVal
        CLR (0x60,X)
        JP PrintVal
    AdjSix:
        LD A,(0x60,X)
        CP A,#6
        JRNE PrintVal
        CLR (0x60,X)
        JP PrintVal
    AdjTwo:
        LD A,(0x60,X)
        CP A,#3
        JRNE PrintVal
        CLR (0x60,X)
    PrintVal:
        LD A,(0x60,X)
        ADD A,#48 ; Adjust to ASCII
        CALL PopulateShift
        CALL SendLetter
        CALL Backspace
        CALL ClearShift
    SkipClkInc:
        POPW X
        RET

AlarmSetLogic:
    PUSHW X
    BTJT 0x500B,#7,SkipAlmMov ; Is move button pressed?
    BTJF 0x52,#1,SkipAlmMov
    BRES 0x52,#1
    INC 0x56 ; Inc cursor position
    INC 0x57 ; inc number index
    LD A,0x56
    CP A,#2 ; Is Colon?
    JREQ MovTwoAlm ; Yes Move Two
    CP A,#5 ; Is Colon?
    JREQ MovTwoAlm ; Yes Move Two
    CP A,#8 ; Is at end?
    JREQ ToStartAlm ; Yes reset Cursor
    JP MovOneAlm ; Else move cursor Regularly
MovTwoAlm:
    CALL MovRight
    INC 0x56
MovOneAlm:
    CALL MovRight
    JP SkipAlmMov
ToStartAlm:
    CLR 0x56
    CLR 0x57
    CALL MovRight ; Adjust to correct position before moving home
    CALL ReturnHome
SkipAlmMov:
    BTJT 0x500B,#6,SkipAlmInc ; Is increment button pressed?
    BTJF 0x52,#1,SkipAlmInc
    BRES 0x52,#1
    LD A, 0x57
    ;SUB A, 0x57      ; Adjust for registers being revesed from inx. (Hrs->sec vs sec->hrs) ** Try uncommenting this
    CLRW X
    EXG A,XL
    INC (0x66,X) ; Adjust each alarm register
    LD A, 0x57 ; Below just checks for overflow
    CP A,#0
    JREQ AdjTwoAlm
    CP A,#2
    JREQ AdjSixAlm
    CP A,#4
    JREQ AdjSixAlm
    LD A,(0x66,X)
    CP A,#10
    JRNE PrintValAlm
    CLR (0x66,X)
    JP PrintValAlm
AdjSixAlm:
    LD A,(0x66,X)
    CP A,#6
    JRNE PrintValAlm
    CLR (0x66,X)
    JP PrintValAlm
AdjTwoAlm:
    LD A,(0x66,X)
    CP A,#3
    JRNE PrintValAlm
    CLR (0x66,X)
PrintValAlm: ; Throw new value to display
    LD A,(0x66,X)
    ADD A,#48 ; Adjust to ASCII
    CALL PopulateShift
    CALL SendLetter
    CALL Backspace
    CALL ClearShift
SkipAlmInc:
    POPW X
    RET
    
        
    ; Clears the shift register
    ClearShift:
        PUSH A
        LD A,#8
        BRES 0x500F, #1
    ClearLoop:
        BSET 0x500F, #3
        NOP
        BRES 0x500F, #3
        DEC A
        JREQ ClearDone
        JP ClearLoop
    ClearDone:
        BSET 0x500F, #2
        NOP
        BRES 0x500F, #2
        POP A
        RET

    ; Populates shift register with value in A
    PopulateShift:
        CALL DumbDelay
        PUSHW X
        LD 0x50, A
        LDW X,#8
    PopLoop:
        JREQ PopDone
        BTJT 0x50,#7,ShiftO
        JP ShiftZ
    ShiftO:
        BSET 0x500F, #1
        JP PulseClk
    ShiftZ:
        BRES 0x500F, #1
    PulseClk:
        BSET 0x500F, #3
        NOP
        BRES 0x500F, #3
        SLL 0x50
        DECW X
        JP PopLoop
    PopDone:
        BSET 0x500F, #2
        NOP
        BRES 0x500F, #2
        POPW X
        ret
    
    SendCommand:
        BRES 0x500F, #5 ; Clear RS
        BRES 0x500F, #6 ; Clear E

        BSET 0x500F, #6 ; Set E to send instruction
        NOP
        NOP
        NOP

        BRES 0x500F, #6 ; Clear E
        NOP
        NOP
        NOP ; Little delay
        BRES 0x500F, #5 ; Clear RS

        RET

    SendLetter:
        BRES 0x500F, #5 ; Clear RS
        BRES 0x500F, #6 ; Clear E

        BSET 0x500F, #6 ; Set E to send instruction
        BSET 0x500F, #5 ; Set RS
        NOP
        NOP
        NOP

        BRES 0x500F, #6 ; Clear E
        NOP
        NOP
        NOP
        BRES 0x500F, #5 ; Clear RS

        RET

    Backspace:
        CALL ClearShift
        LD A, #0b00010000; Shift cursor left 
        CALL PopulateShift
        CALL SendCommand
        RET

    ClearScreen:
        LD A, #1 ;         Clear Display
        CALL PopulateShift
        CALL SendCommand
        CALL ClearShift
        RET

    MovRight:
        CALL ClearShift
        LD A, #0b00010100
        CALL PopulateShift
        CALL SendCommand
        RET

    ReturnHome:
        CALL Backspace
        CALL Backspace
        CALL Backspace
        CALL Backspace
        CALL Backspace
        CALL Backspace
        CALL Backspace
        CALL Backspace
        RET


    AdjustTime:
        LD A,0x60
        CP A,#10
        JRNE AdjTimeDoneSec
    SecLowOv:
        INC 0x61
        LD A,0x61
        CLR 0x60
        CP A,#6
        JRNE AdjTimeDone
    SecHighOver:
        INC 0x62
        LD A,0x62
        CLR 0x61
        CP A,#10
        JRNE AdjTimeDone 
    MinLowOver:
        INC 0x63
        LD A,0x63
        CLR 0x62
        CP A,#6
        JRNE AdjTimeDone
    MinHighOver:
        INC 0x64
        CLR 0x63
        CP A,#10
        LD A,0x65
        CP A,#2
        JREQ DayDone
        JRNE AdjTimeDone
    HrLowOver:
        INC 0x65
        CLR 0x64
        JP AdjTimeDone
    DayDone:
        LD A,0x64
        CP A,#4
        CLR 0x65
        CLR 0x64
    AdjTimeDone:
        CALL ResetClk
        RET
    AdjTimeDoneSec:
        RET

    ResetClk:
        CALL ClearScreen
        LD A, 0x65
        ADD A,#48 ; Adjust to ASCII
        CALL PopulateShift
        CALL SendLetter
        CALL ClearShift
        
        LD A, 0x64
        ADD A,#48 ; Adjust to ASCII
        CALL PopulateShift
        CALL SendLetter
        CALL ClearShift

        LD A,#0x3A
        CALL PopulateShift
        CALL SendLetter
        CALL ClearShift
        LD A, 0x63
        ADD A,#48 ; Adjust to ASCII
        CALL PopulateShift
        CALL SendLetter
        CALL ClearShift
        
        LD A, 0x62
        ADD A,#48 ; Adjust to ASCII
        CALL PopulateShift
        CALL SendLetter
        CALL ClearShift

        LD A,#0x3A
        CALL PopulateShift
        CALL SendLetter
        CALL ClearShift

        LD A, 0x61
        ADD A,#48 ; Adjust to ASCII
        CALL PopulateShift
        CALL SendLetter
        CALL ClearShift

        LD A, 0x60
        ADD A,#48 ; Adjust to ASCII
        CALL PopulateShift
        CALL SendLetter
        CALL ClearShift

        BRES 0x53,#0 
        BRES 0x53,#1 
        BRES 0x53,#2 
        ;CALL ClockMode

        RET

    DispAlarm:
        CALL ClearScreen
        LD A, 0x66
        ADD A,#48 ; Adjust to ASCII
        CALL PopulateShift
        CALL SendLetter
        CALL ClearShift
        
        LD A, 0x67
        ADD A,#48 ; Adjust to ASCII
        CALL PopulateShift
        CALL SendLetter
        CALL ClearShift

        LD A,#0x3A
        CALL PopulateShift
        CALL SendLetter
        CALL ClearShift
        LD A, 0x68
        ADD A,#48 ; Adjust to ASCII
        CALL PopulateShift
        CALL SendLetter
        CALL ClearShift
        
        LD A, 0x69
        ADD A,#48 ; Adjust to ASCII
        CALL PopulateShift
        CALL SendLetter
        CALL ClearShift

        LD A,#0x3A
        CALL PopulateShift
        CALL SendLetter
        CALL ClearShift

        LD A, 0x6A
        ADD A,#48 ; Adjust to ASCII
        CALL PopulateShift
        CALL SendLetter
        CALL ClearShift

        LD A, 0x6B
        ADD A,#48 ; Adjust to ASCII
        CALL PopulateShift
        CALL SendLetter
        CALL ClearShift

        BRES 0x53,#0 
        BRES 0x53,#1 
        BRES 0x53,#2 
        ;CALL ClockMode

        RET

    

    MakeBeep:
        BTJF 0x53,#7,SkipBeep
        BCPL 0x500F, #4            ; Toggle Port D, Pin 4 (Creates 500Hz Tone)
        RET
    SkipBeep:
        BRES 0x500F,#4
        RET

    DumbDelay:
        PUSHW X
        LDW X, #0xFFFF
    DumbLoop:
        DECW X
        JREQ DumbDone
        JP DumbLoop 
    DumbDone:
        POPW X
        RET

    __endasm;

}

