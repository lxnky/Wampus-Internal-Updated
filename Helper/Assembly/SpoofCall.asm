EXTERNDEF CallReturns:QWORD
EXTERNDEF CallFakestack:QWORD
EXTERNDEF CallFakestackSize:QWORD
 
.data
	CallReturns QWORD 32 dup (?)
	CallFakestack dq ?
	CallFakestackSize dq ?

.code
 
DoSpoofCall proc
 
; push arguments
push 0 ;stack align
push r9 ;save 4th arg
push r8 ;save 3th arg
push rdx ;save 2th arg
push rcx ;save 1th arg
 
mov rax, rdx ; mov function address to rax
mov rcx, rsp ; mov stack value to rcx
add rcx, 50h ; adjust stack
mov rdx, rcx ; store stack to compare later
 
;check for arguments length
cmp qword ptr[rsp], 13041052h ; check if it's last argument (our fake argument)
jz end_args_calc ; we found our fake argument
 
mov rax, r8; ; new possible function address
cmp qword ptr[rsp + 8], 13041052h ; check if it's last argument (our fake argument)
jz end_args_calc ; we found our fake argument
 
mov rax, r9; ; new possible function address
cmp r8, 13041052h ; check if it's last argument (our fake argument)
jz end_args_calc ; we found our fake argument
 
mov rax, [rcx] ; new possible function address
cmp r9, 13041052h ; check if it's last argument (our fake argument)
jz end_args_calc ; we found our fake argument
 
;stackwalk to found our fake argument
check_for_args_end:
mov rax, [rcx + 8] ; new possible function address
cmp qword ptr[rcx], 13041052h ; check if it's last argument (our fake argument)
jz end_args_calc ; we found our fake argument
add rcx, 8 ; add stack argument
jmp check_for_args_end
 
end_args_calc:
 
sub rcx, rdx ;stack arguments size in bytes
 
mov r8, cleanup_call
push r8 ; push our cleanup return address
 
;create fake callstack
mov r10, rcx
mov rcx, CallFakestackSize
cmp rcx, 0
jz skip_fakecallstack
 
lea r8, [rcx * 8]
sub rsp, r8
 
;backup register
mov r8, rsi
mov r9, rdi
 
mov rsi, CallFakestack
mov rdi, rsp
rep movsq
 
;restore registers
mov rsi, r8
mov rdi, r9
 
skip_fakecallstack:
mov rcx, r10
 
;stack align
mov r8, rcx
and r8, 15
mov r8, rcx
jnz stack_aligned
push 0
add r8, 8
stack_aligned:
 
;build callstack
push_value:
cmp rcx, 0
jz do_call
push [rdx + rcx - 8]
sub rcx, 8
jmp push_value
 
do_call:
 
add r8, 20h
;psuedo sub rsp, 20h
push 0
push 0
push 0
push 0
 
;push proper return address
lea r9, CallReturns
mov r9, [r9 + r8]
push r9
 
mov r9, [rdx - 38h] ;restore original r9
mov r8, [rdx - 40h] ;restore original r8
mov rcx, [rdx - 50h] ;restore original rcx
mov rdx, [rdx - 48h] ;restore original rdx
jmp rax ; call function
 
cleanup_call:
 
add rsp, 28h
ret
 
DoSpoofCall endp
 
end