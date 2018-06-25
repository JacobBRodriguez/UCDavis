	ori	$1, $0, 1	# $1 = 1

	add	$2, $1, $1	# $2 = 2. forwarding both ALU inputs from same source

	xor	$3, $1, $2	# $3 = 3. forwarding both ALU inputs from different sources

	addi	$4, $3, 14	# $4 = 17. forwarding only A value

	sub	$5, $1, $3	# $5 = -2. forwarding only B value

	sw	$4, 14($5)	# memory[12] = 17. need to forward both $4 and $5

	lw	$6, 14($5)	# $6 = 17. forward $5

	and	$7, $6, $4	# $7 = 17. stall or forward

	beq	$7, $1, begin	# not taken. forward $7 or stall

	slt	$8, $6, $7	# $8 = 0. forward $7 and $6

	beq	$8, $8, begin	# taken. forward $8 or stall

	addi	$17, $0, 1	# should not execute

begin:	addi	$9, $0, 1	# $9 = 1

	lw	$10, 12($0)	# $10 = 17

	sw	$10, 19($10)	# memory[36] = 17. forward $10 or stall

	lw	$10, 12($0)	# $10 = 17

	beq	$10, $1, begin	# not taken. stall for $10

	jal	middle	# $31 = 72

	addi	$18, $0, 1	# should not execute

middle:	addi	$31, $31, 44	# $31 = 116

	sw	$31, 0($0)	# memory[0] = 116

	andi	$11, $4, 16	# $11 = 16

	nor	$12, $2, $5	# $12 = 1

	or	$13, $2, $4	# $13 = 19

	sll	$14, $9, 5	# $14 = 32

	srl	$15, $2, 1	# $15 = 1

	lw	$16, 0($0)	# $16 = 116

	jr	$16	# must stall here

	addi	$19, $0, 1	# should not execute

end:	j	end	# infiinte loop to mark we are done

	addi	$20, $0, 1	# should not execute

	addi	$21, $0, 1	# should not execute



# for full credit: $1 to $16 have correct values, $17 to $21 have not changed

# program ends in infinite loop at end
