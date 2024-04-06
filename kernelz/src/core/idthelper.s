.macro HandleException num
    HandleException\num:
        push $\num
        jmp interrupthandler
.endm

.macro HandleInterruptRequest num
    HandleInterruptRequest\num:
        push 0
        push $\num + 0x20
        jmp interrupthandler
.endm

HandleException 0
HandleException 1
HandleException 2
HandleException 3
HandleException 4
HandleException 5
HandleException 6
HandleException 7
HandleException 8
HandleException 9
HandleException 10
HandleException 11
HandleException 12
HandleException 13
HandleException 14
HandleException 15

HandleInterruptRequest 0
HandleInterruptRequest 1
HandleInterruptRequest 2
HandleInterruptRequest 3
HandleInterruptRequest 4
HandleInterruptRequest 5
HandleInterruptRequest 6
HandleInterruptRequest 7
HandleInterruptRequest 8
HandleInterruptRequest 9
HandleInterruptRequest 10
HandleInterruptRequest 11
HandleInterruptRequest 12
HandleInterruptRequest 13
HandleInterruptRequest 14
HandleInterruptRequest 15
HandleInterruptRequest 31
HandleInterruptRequest 221 

interrupthandler:

.global IgnoreInterrupt
IgnoreInterrupt:
    iret
