
.model small
.stack 100h

.data

cmd_size db 0
cmd_error_text db "You should pass 1 argument in cmd. Try again",10,13,'$'
max_len dw 3

cmd_max equ 10
number_str db 3 dup (0)
 
number_str_len db 0

copies db 3 dup (0), '$'
copy_number db 0

ntimes db 0

len db 0
number db 0 

base dw 10

program_start db "Start program",10,13,'$'
program_end db "End program",10,13,'$'

realloc_error_text db "Realloc error",10,13,'$'
load_error_text db "Can't load program", 10,13,'$'
no_file db "File not found",10,13,'$'

other db "Other program started",10,13,'$'

delim db 10,13,'$'

path db "Task7.exe",0


blockEPB dw 0 ;текущее окружение
	dw offset cmd_text, 0
	
	dw 005ch, 0 ;адреса fcb1 ,fcb2
	dw 006ch, 0
	
	cmd db 10, ' ' 		
	cmd_text db 125 dup (13)

EPBlen dw $-blockEPB	


data_seg_size=$-cmd_text

.code

string_output macro str
   mov dx,offset str
   mov ah, 09h
   int 21h       
endm 

start:
	;изменяет размер блока памяти
	mov ah,4ah
	mov bx, (code_seg_size/16+1)+(data_seg_size/16+1)+256/16+256/16
	int 21h
	jc realloc_error
	
	mov ax, @data
	mov es,ax
	
	xor cx,cx
	
	mov cl, ds:[80h]
	
	dec cl
	mov cmd_size, cl
	
	mov si, 82h
	lea di, cmd_text
	rep movsb

	
	mov ds,ax
	
	string_output program_start
		
	call parse_cmd
	call change_cmd_arguments
	
	
	string_output copies
	string_output delim
	
	
	call create_new_cmd
	
	mov bx, offset blockEPB
	mov ax,ds
	mov word ptr[blockEPB+4],ax
	mov ax,cs
	mov word ptr[blockEPB+8],ax
	mov word ptr[blockEPB+12],ax
	
	mov ax,4b00h
	lea dx, path
	lea bx, blockEPB
	int 21h
	jnb load_error
	jmp exit

realloc_error:
	string_output realloc_error_text
	jmp exit
	
load_error:
	string_output load_error_text
	

exit:
	mov ah,4ch
	int 21h

parse_cmd proc
	push cx
	push si
	push di
	
	xor ch,ch
	mov cl, cmd_size
	lea si, cmd_text
	
	lea di, number_str
	call get_number	
	cmp number_str,0
	je cmd_error

	lea di, copies
	call get_number

	cmp ds:[si], 13
    jne cmd_error
	
	pop di
	pop si
	pop cx
	ret 
cmd_error:
	string_output cmd_error_text
	jmp exit
	
parse_cmd endp


get_number proc
	push cx
	push di

find_stop_symbol:	
	mov al, ds:[si]
	cmp al, 13
	je found_stop_symbol     
	cmp al,' '
	je found_stop_symbol 
	cmp al,0
	je found_stop_symbol
	mov [di], al
	inc si
	inc di  
	loop find_stop_symbol

found_stop_symbol:	
	mov al, 0
	mov [di], 0
	inc si
	
	pop di
	pop cx
	ret
get_number endp	

atoi proc
	push cx
	push di
	push bx
	push ax
	
	call strlen
	
	xor ch,ch
	mov cl, len
create_number:
   	mov al, number
   	mul base 
    mov number, al
	mov al, ds:[di]
	sub al, '0'
	add number, al
	inc di
	
	loop create_number
	
	pop ax
	pop bx
	pop di
	pop cx
	ret
atoi endp	

itoa proc
	push ax
	push si 
	push bx 
	push di
	
	call zero_str
get_str:   
    xor ah,ah
	mov al, number
    mov bx, base	
	div bl     
	mov number, al
	add ah, '0'
	mov ds:[di], ah
	inc di
	cmp al, 0
	jne get_str
	
	
	pop di
	call strlen
	
	push di
	mov si,di
	
	xor ah,ah
	mov al, len
	add di, ax
	dec di
	call reverse
	
	pop di
	pop bx
	pop si
	pop ax
	ret
itoa endp	



strlen proc 
    push ax
	push cx
	push di
	
	
	mov al, 0
	mov cx,max_len
	repne scasb
	
	mov bx, max_len
	sub bx, cx 
	dec bx
	mov len, bl
	
	pop di
	pop cx
	pop ax
    ret 
strlen endp  
 
    
reverse proc
	push ax
	push si
	push di
	
reverse_m:
	mov al, ds:[si]
	mov ah, ds:[di]
	mov ds:[si], ah
	mov ds:[di], al
	inc si
	dec di
	cmp si, di
	jl reverse_m
	
	pop di
	pop si
	pop ax
	ret
reverse endp

    
zero_str proc 
    push cx
    push ax
    push di
    
    mov cx, max_len
    mov al, 0   
    rep stosb 
    
    pop di
    pop ax
    pop cx
    ret
zero_str endp     


change_cmd_arguments proc
	push di

	lea di, number_str
	call atoi
	dec number
	cmp number, 0
	jne not_zero
	jmp exit

not_zero:	
	lea di, number_str
	call itoa
	
	lea di, copies
	cmp copies,0
	jne continue_copies
	mov copies[0], byte ptr '0'
continue_copies:
	call atoi
	inc number
	call itoa 
	
	pop di
	ret
change_cmd_arguments endp  

create_new_cmd proc
	push di
	push cx
	push si
	xor ch,ch 
	 
	lea di, cmd_text
	push max_len
	mov max_len, cmd_max
	call zero_str
	pop max_len
	mov [di], byte ptr ' '
	inc di
	
	push di
	lea di, number_str
	call strlen ;len=strlen(number_str)
	pop di 
	
	mov cl, len
	lea si, number_str
	rep movsb
	
	mov [di], byte ptr ' ' 
	inc di
	
	push di
	lea di, copies
	call strlen ;len=strlen(copies)
	pop di
	
	mov cl,len
	lea si, copies
	rep movsb
	
	pop si
	pop cx
	pop di
	ret
create_new_cmd endp 


code_seg_size =$-start

end start


