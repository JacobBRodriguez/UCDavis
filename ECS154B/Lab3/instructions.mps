	addi	$1, $0, 8	# $1 = 8

	ori	$2, $1, 5	# $2 = 13

	andi	$3, $2, -1	# $3 = 13

	add	$4, $1, $3	# $4 = 8 + 13 = 21

	sub	$5, $4, $3	# $5 = 21 - 13 = 8

	and	$6, $1, $2	# $6 = 8

	xor	$7, $3, $6	# $7 = 5

	sw	$4, 100($5)	# memory[100 + 8] = 21

	lw	$8, 100($5)	# $8 = memory[100 + 8] = 21

	slt	$9, $1, $2	# $9 = $1 < $2 = 8 < 13 = 1

	or	$10, $6, $9	# $10 = 9

	nor	$11, $5, $9	# $11 = -10 (0xFFFFFFF6)

	sll	$12, $9, 7	# $12 = 1 << 7 = 128

	srl	$13, $12, 2	# $13 = 128 >> 2 = 32

	jal	begin	# $31 = 60

	addi	$27, $0, 1	# should not execute

begin:	addi	$14, $0, 1	# $14 = 1

	addi $31, $31, 28	# $31 = 88

middle:	beq	$14, $0, end	# not taken first time

	sub	$14, $14, $9	# $14 = 0

	j	middle

	addi	$28, $0, 1	# should not execute

end:	jr	$31	# infinite loop

	addi	$29, $0, 1	# should not execute

	addi $30, $0, 1	# should not execute
