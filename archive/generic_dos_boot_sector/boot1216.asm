;-----------------------------------------------------------------------------
; Generic boot sector for MS-DOS 6.00 & 6.22 and compatible operating systems.
; This is intended to be used on a FAT12 or FAT16 volume.
; This boot sector is herby released to the PUBLIC DOMAIN.
; Author: Willow Schlanger <wschlanger@gmail.com>
; Date: July 3, 2011
;-----------------------------------------------------------------------------

org 0x7c00
	jmp short bootstrap
	nop
OemName					db 'MSDOS5.0'	; compatible
BytesPerSector			dw 0x0200		; 512
SectorsPerCluster		db 1
ResSectors				dw 1
NumCopiesFAT			db 2
MaxAllocRootDir			dw 0x00e0
MaxSectors				dw 0x0b40		; 1.44*1000 kb
MediaDescriptor			db 0xf0
SectorsPerFat			dw 0x0009
SectorsPerTrack			dw 0x0012
Heads					dw 0x0002
HiddenSectors			dd 0
HugeSectors				dd 0
DriveNumber				db 0			; 0 or 0x80
AlwaysZero				db 0
BootSignature			db 0x29
VolumeId				dd 0x698122d6
VolumeLabel				db '           '
FileSystemType			db 'FAT12   '

bootstrap:
dUserDataLba equ $ + 0x0b				; These locations are for 100% MS-DOS compatibility, they should be moved though
dRootDirLba equ $ + 0x12
	cli									; This should be first instruction after jmp short (some OSes check for it)
	cld									; Clear direction flag
	sti									; We really want interrupts turned on though!
	
	mov bp,0x7c00
	lss sp,[cs:bp+InitialStack-$$]		; SS:SP -> 0:0x7b80

	push ss
	pop ds
	push ss
	pop es

; Back up interrupt 0x1e vector to 0:0x0532.
	mov si,0x1e*4
	mov di,0x0522
	mov eax,[si]
	mov [di+0x10],eax

; Copy interrupt 0x1e table to 0:0x522 in case we're doing a floppy disk boot.
	mov cx,11							; df=0; ES:DI -> 0:0x0522
	lds si,[si]
	rep movsb							; When this is done we will have CX=0 again
	mov ds,cx							; Clear DS (ES is zero also)
	mov al,[bp+SectorsPerTrack-$$]
	mov [di-11+4],al					; Set sectors per track
	mov byte [di-11+9],0x0f				; Set head settle time in milliseconds

; Activate our changes. Point vector 0x1e to 0:0x0522 and reset disk system(s).
	mov dword [0x1e*4],0x00000522
	call reset_disks

; Make [dRootDirLba] valid.
	movzx eax,word [bp+SectorsPerFat-$$]
	movzx ebx,byte [bp+NumCopiesFAT-$$]
	mul ebx
	movzx ebx,word [bp+ResSectors-$$]
	add eax,ebx
	add eax,[bp+HiddenSectors-$$]
	mov [bp+dRootDirLba-$$],eax

; Scan root directory.
	xor bx,bx		; dir entry number
	xor si,si		; offset within cur dir entry
.loop:
	cmp bx,[bp+MaxAllocRootDir-$$]
	jae abort_with_error
	
; BX is the current directory entry number.
	and si,511
	jnz .already_read
	push bx
	mov bx,0x7e00
	call attempt_read_sectors
	pop bx

	inc eax
.already_read:

	mov cx,11
;If we had more space we could STOP when we know for sure there
;won't be any more directory entries. But we're out of space !!
;If IO.SYS is missing, we will need to scan the whole root
;directory, which is fine...
;cmp [si],ch
;je abort_with_error
	push si
	lea di,[bp+IoSys-$$]

	add si,0x7e00
	repe cmpsb
	pop si
	je .found_iosys

	add si,byte 32

	inc bx
	jmp short .loop

.found_iosys:
	movzx ebx,word [si+0x7e1a]			; Get starting cluster number
	mov [0x51a],bx						; MS-DOS expects start IO.SYS cluster number here

	mov ax,[si+0x7e3a]					; MSDOS.SYS for MS-DOS 6.00 & 6.22 will follow IO.SYS, which will be first
	mov [0x53a],ax						; This must be here for MS-DOS to boot properly

	dec bx
	dec bx
	movzx eax,byte [bp+SectorsPerCluster-$$]
	mul ebx								; EAX gets IO.SYS start LBA OFFSET

	mov bx,[bp+MaxAllocRootDir-$$]
	shl bx,5
	add bx,511
	shr bx,9
	;;movzx ebx,bx						; not needed, high 2 bytes of EBX already 0 from above MOVZX EBX,WORD [...]
	add ebx,[bp+dRootDirLba-$$]
	mov [bp+dUserDataLba-$$],ebx

	add eax,ebx							; EAX gets true IO.SYS start LBA

; Note. For our DOS, only the low sector is needed, but we want this boot sector to
; work with MS-DOS 6.00 & 6.22 as well, so we read the first 3 sectors of IO.SYS.
	mov cx,3
	mov bx,0x700
.do_read:	
	call attempt_read_sectors
	add bh,2
	inc eax
	loop .do_read

; Transfer control to MS-DOS. Set AX:BX to LBA of user data region.
	mov ax,[bp+dUserDataLba-$$+2];bp+dRootDirLba-$$+2]
	mov bx,[bp+dUserDataLba-$$];bp+dRootDirLba-$$]
	mov dl,[bp+DriveNumber-$$]
	mov ch,[bp+MediaDescriptor-$$]		; We won't use this but MS-DOS will access it
	lds si,[0x0532]
	jmp 0x70:0

reset_disks:
	mov ah,0x00
	mov dl,[bp+DriveNumber-$$]
	int 0x13							; Reset the drive we are booting from
	ret

abort_with_error:
; It's safe to patch the code here with a JMP to another location, if required.
	lea si,[bp+ErrorMessage-$$]
.print:
	lodsb
	or al,al
	jz .done
	mov ah,0x0e
	mov bx,7
	int 0x10
	jmp short .print
.done:
; Wait for keypress
	xor ax,ax
	int 0x16
;	jmp do_reboot
; Restore interrupt 0x1e vector and reboot. Entry: IF=1, DF=0, DS=0, SS=0, BP=0x7c00.
	mov eax,[0x0532]					; Get old interrupt 0x1e vector
	mov [0x1e*4],eax
	call reset_disks
	int 0x19

; In: EAX=LBA.
; Out: ES:BX <- sector data (we attempt to read DI sectors, max 64)
attempt_read_sectors:
	mov di,1							; We want to read ONE Sector
try_read_sectors:
	pushad
	mov si,5							; Try 5 times max
	mov [bp+Int13Packet-$$+4],bx
.retry:
; Attempt disk read using IBM/MS INT 13 Disk Extensions (i.e. LBA mode), first.
; This will fail when not supported, then we fall back to CHS mode.
; There is not enough space here to detect these extensions, so we just try it
; first and if it works, great!
	push eax
	mov word [bp+Int13Packet-$$+0],0x0010
	mov [bp+Int13Packet-$$+2],di		; Number of sectors to transfer
	mov word [bp+Int13Packet-$$+6],es
	mov [bp+Int13Packet-$$+8],eax		; LBA (low 32 bits)
	xor eax,eax
	mov [bp+Int13Packet-$$+12],eax		; LBA (high 32 bits, clear to 0 for FAT12/FAT16)
	mov ah,0x42
	mov dl,[bp+DriveNumber-$$]
	stc
	pusha
	lea si,[bp+Int13Packet-$$]
	int 0x13
	popa
	pop eax
	jnc .done

.no_ibm_ms:
	mov bx,[bp+Int13Packet-$$+4]
push eax
	push bx
	call translate_lba
	pop bx
	mov ax,di							; Set AL to DI[7..0]
	mov ah,0x02
	mov dl,[bp+DriveNumber-$$]
	stc
	pusha
	int 0x13
	popa
pop eax
	sti
	jnc .done
	dec si
	jz near abort_with_error

	call wait_retry
	call reset_disks
	jmp short .retry

.done:
	popad
	ret

; Warning: IF must be 1 when this is called !
wait_retry:
	push eax
	push cx
	mov cx,20				; Wait 20 ticks (about a second)
.wait:
	mov ax,[0x467]			; We really need the space, so I changed EAX to AX.
.wait_tick:
	hlt
	cmp [0x467],ax			; We really need the space, so I changed EAX to AX.
	jz .wait_tick
	loop .wait
	pop cx
	pop eax
	ret

; In: EAX=LBA
; Out:
;   CL[5..0] = sector number (1 based)
;   CL[7..6] = cyl number bits 9..8
;   CH       = cyl number bits 7..0
;   DH       = head number.
;   Modifies EAX, EDX, EBX, CX.
translate_lba:
	xor edx,edx
	movzx ebx,word [bp+SectorsPerTrack-$$]
	div ebx
	mov cx,dx							; Save sector number (0 based)
	xor dx,dx							; High word of EDX is already 0
	mov bx,[bp+Heads-$$]				; High word of EBX already is 0
	div ebx
; Cyl number = AX
; Head number = DX
; Sector number = CX (0 based)
	mov dh,dl							; Get head number
	inc cx								; CX should be 1..63 inclusive, here !
	mov ch,al							; Get low 8 bits of cyl number
	shl ah,6
	or cl,ah							; Put bits 8-9 of cyl in bits 6-7 of CL
	ret

; No room left for a real error message !
ErrorMessage:	db "Push Enter",13,10,0
IoSys:			db 'IO      SYS'

Int13Packet		equ $$-16

times 0x1fa-$+$$ db 0
InitialStack:	dw 0x7b80,0				; This will serve as our magic signature
	dw 0xaa55

