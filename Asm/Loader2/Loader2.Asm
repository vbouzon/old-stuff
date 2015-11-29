.386
.model flat, stdcall  ;32 bit memory model

.code
tableau db 017h


start:
	push 0
	push 0
	xor ecx,ecx
	mov ecx,444444h
debut:
	cmp ecx,666666h
	pop ebx
	pop eax
	je fin
	inc al
	add bl,byte ptr[tableau+eax]
	push ecx
	xor ecx,ecx
	mov cl,byte ptr[tableau+ebx]
	mov dl,byte ptr[tableau+eax]
	mov byte ptr[tableau+ebx],dl
	mov byte ptr[tableau+eax],cl
	pop ecx
	xor edx,edx
	add dl,byte ptr[tableau+eax]
	add dl,byte ptr[tableau+ebx]
	push eax
	push ebx
	mov al,byte ptr [ecx]
	mov bl,byte ptr [tableau + edx]
	xor eax,ebx
	mov byte ptr [ecx],al
	inc ecx
	jmp debut
	
fin:
	xor eax,eax
	mov eax, 66666h
	jmp eax
	
end start