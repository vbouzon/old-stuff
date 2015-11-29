.386
.model flat, stdcall 

include windows.inc
include kernel32.inc
include user32.inc
include Comctl32.inc
include shell32.inc

includelib kernel32.lib
includelib user32.lib
includelib Comctl32.lib
includelib shell32.lib


.data 
msg db ?
msg3 db ?
msg35 db ?
msg356 db ?
msgeheh db ?
msg2 db "Looper :))",0h
msg1 db "Cracker ;)",0h
.code
start:
xor eax,eax
mov eax,403000h
mov byte ptr [eax],"T"
inc eax
mov byte ptr [eax]," "
inc eax
mov byte ptr [eax],"c"
inc eax
mov byte ptr [eax],"o"
inc eax
mov byte ptr [eax],"n"
inc eax
push 0
push eax
push 0
push 0
call MessageBox

invoke ExitProcess,0

end start
