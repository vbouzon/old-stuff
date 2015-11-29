.386
.model flat, stdcall


.data

getproc dd ?
loadlib dd ?
base_dll dd ?
exp_dll dd ?
name_table_ptr_addresse1 dd ?
name_table_ptr_addresse2 dd ?
count1 dd ?
count2 dd ?
user32 db "kernel32",0h
write db "WriteFile",0h
.code
assume fs:nothing

Run:
push    30h
pop     edx
mov     edx,fs:[edx]	; Adresse de PEB (dans TEB)
mov     eax,[edx + 0ch]	; PEB_LBR_DATA
mov     esi,[eax + 1Ch]	; InitializationOrderModule
mov     eax,[esi]	; Liste chaînée
mov     edx,[eax + 8]	; Deuxieme element
mov     [base_dll], edx	; On le stocke dans la pile

mov     edx,[edx+3ch]	; Récupération du header de PE
add     edx,[base_dll]	; + Adresse de Base
mov     edx,[edx+78h]	; Récupération de l’adresse de “export table”
add     edx,[base_dll]	; + Base
mov     [exp_dll],edx	; Stockage dans la pile


xor     eax,eax
add     edx,20h	; Name table PTR
mov     edx,[edx]
add     edx,[base_dll]	; + Base
mov     [name_table_ptr_addresse1],edx
mov     [name_table_ptr_addresse2],edx	; On stocke dans notre variable

search_fct1:
xor ecx, ecx
mov ebx, [name_table_ptr_addresse1]
mov ebx, [ebx]         	     	; Notre variable
add ebx, [base_dll]            	; +base
cmp  dword ptr [ebx], 'PteG' 
jne _return1
cmp  dword ptr [ebx+4h], 'Acor' ; strcmp
jne _return1
jmp search_fct2
_return1:
add word ptr [name_table_ptr_addresse1], 4h ; Adress++
add word ptr [count1], 1h ; count++
jmp search_fct1; on boucle

search_fct2:
xor ecx, ecx
mov ebx, [name_table_ptr_addresse2]
mov ebx, [ebx]         	     	; Notre variable
add ebx, [base_dll]            	; +base
cmp  dword ptr [ebx], 'daoL' 
jne _return2
cmp  dword ptr [ebx+4h], 'rbiL' ; strcmp
jne _return2
jmp next
_return2:
add word ptr [name_table_ptr_addresse2], 4h ; Adress++
add word ptr [count2], 1h ; count++
jmp search_fct2; on boucle


next:
mov ebx, [count1]		; Transformation des index en adresse
imul ebx, 4h			; relative
mov [count1], ebx      
mov ebx, [exp_dll]
add ebx, 1ch
mov ebx,[ebx]
add ebx ,[base_dll]
add ebx, [count1]
mov ebx, [ebx]
add ebx, [base_dll]
mov [getproc],ebx

mov ebx, [count2]		; Transformation des index en adresse
imul ebx, 4h			; relative
mov [count2], ebx      
mov ebx, [exp_dll]
add ebx, 1ch
mov ebx,[ebx]
add ebx ,[base_dll]
add ebx, [count2]
mov ebx, [ebx]
add ebx, [base_dll]
mov [loadlib],ebx
push offset user32
call dword ptr [loadlib]
push offset write
push eax
call dword ptr [getproc]
end Run