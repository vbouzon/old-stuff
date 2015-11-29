.386
.model flat, stdcall  ;32 bit memory model

.code
tableau db 017h, 0B9h, 03Fh, 005h, 0B9h, 0E8h, 0DCh, 0BDh, 089h, 053h, 052h, 030h, 071h, 0EFh, 0DEh, 069h
	db 095h, 010h, 08Bh, 07Ch, 04Eh, 05Eh, 02Ah, 005h, 0AAh, 04Fh, 0B0h, 0A8h, 080h, 0E1h, 0BEh, 0C3h
	db 0C8h, 036h, 0A9h, 06Eh, 07Eh, 0D2h, 0A7h, 0D0h, 031h, 091h, 0E7h, 02Dh, 09Ch, 05Ch, 029h, 070h
	db 016h, 04Ch, 021h, 0D7h, 04Fh, 095h, 032h, 059h, 098h, 032h, 06Bh, 07Ah, 014h, 0EAh, 02Ch, 0E4h
	db 00Bh, 0BDh, 084h, 0BAh, 08Eh, 0E6h, 0B0h, 0A8h, 07Bh, 0CBh, 049h, 09Ch, 0F4h, 043h, 0E3h, 05Ch
	db 005h, 02Ch, 045h, 0B6h, 0CAh, 001h, 0A0h, 065h, 0BFh, 0BEh, 02Ah, 036h, 032h, 0ACh, 035h, 0B7h
	db 014h, 006h, 0CFh, 02Bh, 07Eh, 05Ah, 09Bh, 075h, 049h, 0A6h, 07Ah, 02Eh, 00Dh, 0CFh, 0D5h, 0ABh
	db 07Ah, 08Ch, 0A7h, 0E7h, 0F1h, 0E2h, 0DBh, 0D6h, 0E2h, 063h, 041h, 0EEh, 0A4h, 0B7h, 0B7h, 095h
	db 0DBh, 0CDh, 04Eh, 061h, 0F1h, 09Bh, 011h, 00Fh, 01Ah, 0B2h, 034h, 0E6h, 018h, 0D8h, 04Bh, 042h
	db 0F3h, 032h, 036h, 079h, 0E4h, 0FAh, 0AFh, 007h, 009h, 0E9h, 05Dh, 072h, 02Ah, 002h, 0B0h, 0EAh
	db 0C4h, 004h, 04Ch, 0F9h, 02Bh, 007h, 0D2h, 0E4h, 0C6h, 0ABh, 03Ch, 09Dh, 060h, 07Dh, 05Fh, 084h
	db 0F0h, 041h, 0EFh, 0C8h, 0D6h, 015h, 056h, 0F6h, 0C0h, 024h, 074h, 0D0h, 00Ah, 007h, 0F4h, 090h 
	db 014h, 048h, 03Ah, 0D2h, 00Ch, 01Dh, 0A8h, 0E5h, 0ECh, 0C1h, 020h, 053h, 05Ah, 039h, 0E7h, 025h 
	db 09Ch, 089h, 06Dh, 0A4h, 0C6h, 09Dh, 013h, 06Fh, 07Eh, 027h, 0DEh, 0B1h, 07Ah, 0C7h, 0C5h, 020h 
	db 043h, 0F8h, 05Eh, 082h, 0B9h, 087h, 01Eh, 015h, 0A7h, 004h, 0A0h, 02Eh, 05Eh, 0F3h, 071h, 0DFh 
	db 03Fh, 0D5h, 01Bh, 017h, 06Ch, 015h, 0AEh, 07Fh, 0FDh, 0BCh, 0DBh, 010h, 0F1h, 06Ah, 06Bh, 099h

start:
		xor eax,eax
		xor ebx,ebx
		xor ecx,ecx
		xor edx,edx
		push 66666655h
		mov edx,401050h
creparti:		
		mov al, byte ptr [edx]
		mov ah, byte ptr [tableau+ecx]
		xor al,ah
		mov byte ptr [edx],al
		xor eax,eax
		inc edx
		inc ecx
		cmp ecx,256
		jne tst
		xor ecx,ecx		
tst:
		pop eax
		cmp eax,edx
		je fin
		push eax
		jmp creparti
		
fin:	
		xor eax,eax
		mov eax,444444h
		jmp eax
	
		end start	