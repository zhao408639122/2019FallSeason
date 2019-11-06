#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>

// represent the 32 general registers
enum
{
	$zero, 		// $reg00
	$at,		// $reg01
	$v0,		// $reg02
	$v1,		// $reg03
	$a0,		// $reg04
	$a1,		// $reg05
	$a2,		// $reg06
	$a3,		// $reg07
	$t0,		// $reg08
	$t1,		// $reg09
	$t2,		// $reg10
	$t3,		// $reg11
	$t4,		// $reg12
	$t5,		// $reg13
	$t6,		// $reg14
	$t7,		// $reg15
	$s0,		// $reg16
	$s1,		// $reg17
	$s2,		// $reg18
	$s3,		// $reg19
	$s4,		// $reg20
	$s5,		// $reg21
	$s6,		// $reg22
	$s7,		// $reg23
	$t8,		// $reg24
	$t9,		// $reg25
	$k0,		// $reg26
	$k1,		// $reg27
	$gp,		// $reg28
	$sp,		// $reg29
	$fp,		// $reg30
	$ra			// $reg31
};
int print_instruction(uint32_t *);

int main(int argc, char **args)
{
	printf("------------------------------------------------------------------------------------------\n");
	printf("|Program file genration for Simple von Neumann Computer for MIPS (Zili Shao@CSE,CUHK)|\n");
	printf("------------------------------------------------------------------------------------------\n\n");

	if (argc != 2)
	{
		printf("\nUsage: ./prog-gen file-name\n\n");
		exit(-1);
	}

	int fd;

	if ((fd = open(args[1], O_CREAT | O_TRUNC | O_RDWR)) < 0)
	{
		printf("Error: open().\n");
		exit(-1);
	}

	uint32_t memory[64];
	// input your instructions here, according to the MIPS instruction set
	// memory[0] = 0x02000001;		//Instruction: addi            R0,           R0,             1
	//             Byte 4 (opcode)|Byte 3 (Reg) | Byte 2 (Reg) |Byte 0 (Immediate)|
	//               2            |  0          |  0           | 1                |

	// MIPS Instruction category
	// R op(6)<<26 rs(5)<<21  rt(5)<<16  rd(5)<<11  shamt(5)<<6 func(6)
	// I op(6)<<26 rs(5)<<21  rt(5)<<16  imm(16)
	// J op(6)<<26 address(26)

	// lw $t2, 4($t1)  op = 100011, rs = $t1, rt = $t2, imm = 4
	memory[0] = (strtol("100011", NULL, 2) << 26) + ($t1 << 21) + ($t2 << 16) + 62;

	// addiu $t1, 100($t2) op = 001001, rs = $t2, rt = $t1, imm = 100
	memory[1] = (strtol("001001", NULL, 2) << 26) + ($t2 << 21) + ($t1 << 16) + 100;

	// and $t3, $t1, $t2  op = 000000, rs = $t1, rt = $t2, rd = $t3
	memory[2] = (strtol("000000", NULL, 2) << 26) + ($t1 << 21) + ($t2 << 16) + ($t3 << 11) + (strtol("100100", NULL, 2));

	// add [rd <- rs + rt] op = 000000, rs = $t1, rt = $t2, rd = $t4
	memory[3] = (strtol("000000", NULL, 2) << 26) + ($t1 << 21) + ($t2 << 16) + ($t4 << 11) + (strtol("100000", NULL, 2));

	// addu [rd <- rs + rt] op = 000000, rs = $t1, rt = $t2, rd = $t5
	memory[4] = (strtol("000000", NULL, 2) << 26) + ($t1 << 21) + ($t2 << 16) + ($t5 << 11) + (strtol("100001", NULL, 2));

	// SLL  rd <- rt << shamt  left op = 000000, rt = $t2, rd = $t5
	memory[5] = (strtol("000000", NULL, 2) << 26) + (strtol("00000", NULL, 2) << 21) + ($t2 << 16) + ($t5 << 11) + (strtol("00001", NULL, 2) << 6) + (strtol("000000", NULL, 2));

	// SRAV rd <- rt >> rs op = 000000, rs = $t2, rt = $t5, rd = $t6
	memory[6] = (strtol("000000", NULL, 2) << 26) + ($t2 << 21) + ($t5 << 16) + ($t6 << 11) + (strtol("000111", NULL, 2));

	// SUBU  rd <- rs - rt op = 000000, rs = $t3, rt = $t4, rd = $t6
	memory[7] = (strtol("000000", NULL, 2) << 26) + ($t3 << 21) + ($t4 << 16) + ($t6 << 11) + (strtol("100011", NULL, 2));

	// jal $31<-PC+4；PC <- (PC+4)[31..28],address,0,0
	memory[8] = (strtol("000011", NULL, 2) << 26) + 3;
	
	// BNE if (rs != rt) PC <- PC+4 + (sign-extend)immediate<<2
	memory[9] = (strtol("000101", NULL, 2) << 26) + ($t3 << 21) + ($t4 << 16) + 1;

	// SLT if (rs < rt) rd=1 else rd=0 op = 000000, rs = $t3, rt = $t6, rd = $t7
	memory[12] = (strtol("000000", NULL, 2) << 26) + ($t3 << 21) + ($t6 << 16) + ($t7 << 11) + (strtol("101010", NULL, 2));
	
	// JR PC <- rs op = 000000, rs = $t3
	memory[13] = (strtol("000000", NULL, 2) << 26) + ($ra << 21) + (strtol("001000", NULL, 2));
	// 7->8->12->13->9->14

	// Print the instruction
	// print_instruction(&memory[0]);
	// the last instruction will be halt, indicates the end of the program
	memory[14] = (strtol("111111", NULL, 2) << 26); //Instruction: halt
	// static data n1 = 1
	memory[62] = 11;

	/* write the memory contents into the image file*/
	int wcount;
	if ((wcount = write(fd, memory, 64 * 4)) < 0)
	{
		printf("Error: write()\n");
		exit(-1);
	}
	else if (wcount != 64 * 4)
	{
		printf("Error: write bytes not equal 256.\n");
		exit(-1);
	}

	// for (int i = 0; i < 64; i++)
	// {
	// 	printf("memory[%02d]:0x%08x\n", i, memory[i]);
	// }

	close(fd);

	return 0;
}

int print_instruction(uint32_t *p_i)
{
	uint8_t *p = (uint8_t *)p_i;

	uint8_t low_addr_value = *(p + 3);
	uint8_t sec_addr_value = *(p + 2);
	uint8_t third_addr_value = *(p + 1);
	uint8_t high_addr_value = *(p);

	/* MIPS32 is big endian -  the most significant byte first (lowest address) and the least significant byte last (highest address) */
	printf("Instruction- 0x%x; LowAddr- %#x, Second- %#x, Third- %#x, HighAddr- %#x\n",
		   *p_i, low_addr_value, sec_addr_value, third_addr_value, high_addr_value);
	return 0;
}

// https://www.cnblogs.com/blacksunny/p/7192416.html
// https://blog.csdn.net/qq_41848006/article/details/82256626