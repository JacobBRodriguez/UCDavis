# To have it so that no forwarding occurs: there must be a gap of 5 instructions

# between when an instruction that writes to a register and when it is used.



	addi	$1, $0, 1	# $1 = 1

	addi $2, $0, 2	# $2 = 2

	addi $3, $0, 3	# $3 = 3

	addi $4, $0, 4	# $4 = 4

	addi $5, $0, 5	# $5 = 5

	addi $6, $0, 6	# $6 = 6

	ori	$7, $1, 8	# $7 = 9

	sw	$2, 18($2)	# memory[20] = 2

	add	$8, $2, $3	# $8 = 2 + 3 = 5

	and	$9, $1, $3	# $9 = 1

	sub	$10, $5, $2	# $10 = 3

	or	$11, $3, $6	# $11 = 7

	xor	$12, $6, $7	# $12 = 15

	slt	$13, $6, $7	# $13 = 1

	sll	$14, $1, 10	# $14 = 1024

	srl	$15, $2, 1	# $15 = 1

	andi	$16, $10, 2	# $16 = 2

	nor	$17, $7, $11	# $17 = -16 (0xFFFFFFF0)

	lw	$18, 5($12)	# $18 = 2
