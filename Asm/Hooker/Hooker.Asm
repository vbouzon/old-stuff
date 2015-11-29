.386
.model flat, stdcall  ;32 bit memory model

.code

adresse dd ?
api dd ?
nom_dll db "USER32.dll"
nom_fonction db "MessageBoxA"
assume fs:nothing

start:
		mov eax, fs : [30h]
		mov ebx,[eax+08h]	
		
		mov eax,ebx		
		add eax, [ebx+3ch]	
		
		mov eax,[eax+80h]		
		add eax,ebx

cherche_encore_dll:		
		lea esi, nom_dll		
		mov edi, [eax+0ch]		
		and edi,edi			
		jz iat_finie		
		add edi, ebx		
		mov ecx,0ah	
		repz cmpsb
		and ecx,ecx
		jnz passe_dll_suivante
		mov cl,byte ptr [edi]
		and cl,cl
		jnz passe_dll_suivante
		jmp bingo1

passe_dll_suivante:
		add eax,14h		
		jmp cherche_encore_dll

bingo1:					
		xor edx,edx
		push eax

		mov eax,[eax]
		add eax,ebx

cherche_encore_fonction:
		mov edi,[eax]
		and edi,edi
		jz fonctions_finies		
		add edi,ebx
		add edi,02h
		lea esi,nom_fonction
		mov ecx,0Bh
		repz cmpsb
		and ecx,ecx
		jnz passe_fonction_suivante
		mov cl,byte ptr [edi]
		and cl,cl
		jnz passe_fonction_suivante
		jmp bingo2

passe_fonction_suivante:
		inc edx
		add eax,04h
		jmp cherche_encore_fonction

bingo2:
		pop eax
		mov eax,[eax+10h]
		add eax,ebx
		mov ecx,edx

boucle_index:
		test ecx,ecx
		jz fin
		add eax,04h
		dec ecx
		jmp boucle_index
		
iat_finie:
		xor eax,eax
		jmp fin

fonctions_finies:
		add esp,04h
		xor eax,eax

fin:
		mov IAT,eax
		mov ebx,[eax]
		mov API,ebx
		mov [eax],Hookage
		xor eax,eax
		mov eax,41000h
		jmp eax
		
hookage:
	push ebp
	mov ebp,esp
	mov eax,[ebp+12]
	
re:
	xor ebx,ebx
	mov bl,byte ptr [eax]
	and bl,bl
	jz jump
	cmp bl,61h
	jl car_suivant
	cmp bl,7ah
	jg car_suivant
	sub bl,20h
	mov byte ptr [eax],bl
	
car_suivant:
	inc eax
	jmp re
	
jump:
	mov esp, ebp
	pop ebp
	jmp api

end start