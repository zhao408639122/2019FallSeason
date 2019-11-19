#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_MEM_SIZE 64 //The max memory size - 64 words (256 bytes)

// define the func segment of the instructions belong to the type R
#define R_SLL_FUNC 0x00 // sll func = 000000b = 0x00
#define R_SRL_FUNC 0x02 // srl func = 000010b = 0x02
#define R_SRA_FUNC 0x03 // sra func = 000011b = 0x03
#define R_SLLV_FUNC 0x04 // sllv func = 000100b = 0x04
#define R_SRLV_FUNC 0x06 // srlv func = 000110 = 0x06
#define R_SRAV_FUNC 0x07 // srav func = 000111 = 0x07
#define R_JR_FUNC 0x08 // jr func = 001000 = 0x08
#define R_ADD_FUNC 0x20  //add func = 100000b = 0x20
#define R_ADDU_FUNC 0x21 //addu func = 100001b = 0x21
#define R_SUB_FUNC 0x22 //sub func = 100010b = 0x22
#define R_SUBU_FUNC 0x23 // subu func = 100011b = 0x23
#define R_AND_FUNC 0x24  //and func = 100100b = 0x24
#define R_OR_FUNC 0x25 // or func = 100101b = 0x25
#define R_XOR_FUNC 0x26 // xor func = 100110b = 0x26
#define R_NOR_FUNC 0x27 // nor func = 100111b = 0x27
#define R_SLT_FUNC 0x2A // stl func = 101010b = 0x2A
#define R_SLTU_FUNC 0x2B // SLTU func = 101011b = 0x2B

//....

// define the opcode of the instructions of type I
#define I_ADDI_OPCODE 0x8 //addi opcode = 001000b = 0x08
#define I_ADDIU_OPCODE 0x9 //addiu opcode = 001001b = 0x9
#define I_ANDI_OPCODE 0x0C //andi opcode = 001100b = 0x0C
#define I_ORI_OPCODE 0x0D //ori opcode = 001101b = 0x0D
#define I_XORI_OPCODE 0x0E //xori opcode = 001110b = 0x0E
#define I_LUI_OPCODE 0x0F //lui opcode = 001111b = 0x0F
#define I_LW_OPCODE 0x23   //lw opcode = 100011b = 0x23
#define I_SW_OPCODE 0x2B //sw opcode = 101011b = 0x2B
#define I_BEQ_OPCODE 0x04 //beq opcode = 000100b = 0x04
#define I_BNE_OPCODE 0x05 //bne opcode = 000101b = 0x05
#define I_SLTI_OPCODE 0x0A //slti opcode = 001010b = 0x0A
#define I_SLTIU_OPCODE 0x0B //sltiu opcode = 001011b = 0x0B
//....

//define the opcode of the instructions belong to type J
#define J_J_OPCODE 0x2   //j opcode = 000010b = 0x2
#define J_JAL_OPCODE 0x3 //jal opcode = 000011b = 0x3

typedef struct memory
{
	uint32_t addr[MAX_MEM_SIZE];
} MEMORY;

typedef struct cpu
{
	//Control registers
	uint32_t PC; //Program counter
	uint32_t IR; //Instruction regiser

	//General purpose register
	uint32_t R[32]; // 32 Registers
} CPU;

typedef struct computer
{
	CPU cpu;
	MEMORY memory;
} COMPUTER;

// represente the type of the instruction R I J
enum
{
	TYPE_R,
	TYPE_I,
	TYPE_J
};

typedef struct instruction
{
	int ins_type;

	int op;
	int rs;
	int rt;
	int rd;
	int shamt;
	int func;
	int imm;
	int address;

} INSTRUCTION;

// Memory Structure
/*
	0  |-------C-------|
	1  |-------O-------|   CODE Segment
	.  |-------D-------|
	5  |-------E-------|
	6  |-------D-------|
	7  |-------A-------|   DATA Segment
	.  |-------T-------|
	10 |-------A-------|
	11 |---------------|
	12 |---------------|
	.  |---------------|
	59 |-------S-------|
	60 |-------T-------|
	61 |-------A-------|   STACK
	62 |-------C-------|
	63 |-------K-------|
*/

// represent the 32 general registers
enum
{
	$zero,
	$at,
	$v0,
	$v1,
	$a0,
	$a1,
	$a2,
	$a3,
	$t0,
	$t1,
	$t2,
	$t3,
	$t4,
	$t5,
	$t6,
	$t7,
	$s0,
	$s1,
	$s2,
	$s3,
	$s4,
	$s5,
	$s6,
	$s7,
	$t8,
	$t9,
	$k0,
	$k1,
	$gp,
	$sp,
	$fp,
	$ra
};
char *reg[] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2",
			   "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
			   "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"                                                                                                      };

int computer_load_init(COMPUTER *, char *);
int cpu_cycle(COMPUTER *);
int fetch(COMPUTER *);

int print_cpu(COMPUTER *);
int print_memory(COMPUTER *);
int print_instruction(int, uint32_t);
int decode(uint32_t, INSTRUCTION *);
int execute(COMPUTER *, INSTRUCTION *);

int isEnd = 0; // halt flag

int main(int argc, char **args)
{

	printf("----------------------------------------------------------------\n");
	printf("|Simple von Neumann Computer for MIPS (Zili Shao@CSE,CUHK)|\n");
	printf("----------------------------------------------------------------\n");

	if (argc != 3)
	{
		printf("\nUsage: ./cpu program start_addr\n");
		printf("\t program: program file name; start_addr: the start address for initial PC\n \n");
		exit(-1);
	}

	COMPUTER comp;
	// print_memory(&comp);
	//Initialize: Load the program into the memory, and initialize all regisrters;
	if (computer_load_init(&comp, args[1]) < 0)
	{
		printf("Error: computer_poweron_init()\n");
		exit(-1);
	}
	// print_memory(&comp);
	// exit(0);
	//Set PC and start the cpu execution cycle
	comp.cpu.PC = atoi(args[2]);
	if (comp.cpu.PC >= MAX_MEM_SIZE || comp.cpu.PC < 0)
	{
		printf("Error: start_addr should be in 0-63.\n");
		exit(-1);
	}

	// Execute CPU cyles: fetch, decode, execution, and increment PC; Repeat
	while (1)
	{
		printf("\n\nBefore\n");
		print_cpu(&comp);

		if (cpu_cycle(&comp) < 0)
			break;

		printf("\nAfter\n");
		print_cpu(&comp);
	}
	// print_memory(&comp);
	return 0;
}

int cpu_cycle(COMPUTER *cp)
{
	//initialise the instruction struct
	INSTRUCTION ins = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
	if (fetch(cp) < 0)
		return -1;
	if (decode(cp->cpu.IR, &ins) < 0)
		return -1;
	if (execute(cp, &ins) < 0)
		return -1;
	return 0;
}

int fetch(COMPUTER *cp)
{
	uint32_t pc = cp->cpu.PC;

	if (pc < 0 || pc > 63)
	{
		printf("PC is not in 0-63.\n");
		return -1;
	}
	else
	{
		cp->cpu.IR = cp->memory.addr[pc];
		cp->cpu.PC++;
		printf("**********************************************************************");
		printf("\nFETCH: the instruction is : 0x%x\n", cp->cpu.IR);
		return 0;
	}
}

//MIPS Instruction category
//R op(6)<<26 rs(5)<<21  rt(5)<<16  rd(5)<<11  shamt(5)<<6 func(6)
//I op(6)<<26 rs(5)<<21  rt(5)<<16  imm(16)
//J op(6)<<26 address(26)

// classify the category of the instruction, and decompose the instruction into several part including opcode, register and imm etc.
int decode(uint32_t instr, INSTRUCTION *ins)
{
	printf("DECODE: ");
	// get the opcode
	int op = (instr & (strtol("111111", NULL, 2) << 26)) >> 26;
	if (op == 63)
	{
		// halt
		printf("HALT!\n");
		return -1;
	}
	ins->op = op;
	if (op == 0)
	{ 	// opcode = 000000
		// R type
		ins->ins_type = TYPE_R;
		ins->rs = ((instr & (strtol("11111", NULL, 2) << 21)) >> 21);
		ins->rt = ((instr & (strtol("11111", NULL, 2) << 16)) >> 16);
		ins->rd = ((instr & (strtol("11111", NULL, 2) << 11)) >> 11);
		ins->shamt = ((instr & (strtol("11111", NULL, 2) << 6)) >> 6);
			ins->func = (instr & (strtol("111111", NULL, 2)));

			printf("[type : R]   op = %d, rs = %d, rt = %d, rd = %d, shamt = %d, func = %d\n", ins->op, ins->rs,
				ins->rt, ins->rd, ins->shamt, ins->func);
		}
		else if ((op == 2) || (op == 3))
		{ 	// j(opcode = 000010) jal(opcode = 000011)
			// J type
			ins->ins_type = TYPE_J;
			ins->address = (instr & (strtol("11111111111111111111111111", NULL, 2)));

			printf("[type : J]   op = %d, addr = %d\n", ins->op, ins->address);
		}
		else
		{
			// I type
			ins->ins_type = TYPE_I;
			// get the rs
			ins->rs = ((instr & (strtol("11111", NULL, 2) << 21)) >> 21);
			// get the rt
			ins->rt = ((instr & (strtol("11111", NULL, 2) << 16)) >> 16);
			ins->imm = (instr & (strtol("1111111111111111", NULL, 2)));
			// 处理立即数的符号位
			if( (ins->imm & (1 << 15)) == (1 << 15) ) {
				// ins->imm &= 0b11111111111111110111111111111111;
				ins->imm |= 0b11111111111111111000000000000000;
			}
			printf("[type : I]   op = %d, rs = %d, rt = %d, imm = %d\n", ins->op, ins->rs, ins->rt, ins->imm);
		}
		return 0;
	}

	int execute(COMPUTER *cp, INSTRUCTION *ins)
	{
		printf("EXECUTE : ");
		switch (ins->ins_type) {
		case TYPE_R:
			switch (ins->func) {
				// add 		[rd <- rs + rt](signed number)
				case R_ADD_FUNC: {
					cp->cpu.R[ins->rd] = (int)cp->cpu.R[ins->rs] + (int)cp->cpu.R[ins->rt];

					printf("add	[rd <- rs + rt](signed number)\n");
					break;
				}
				// addu		[rd <- rs + rt](unsigned number)
				case R_ADDU_FUNC: {
					cp->cpu.R[ins->rd] = cp->cpu.R[ins->rs] + cp->cpu.R[ins->rt];

					printf("addu [rd <- rs + rt](unsigned number)\n");
					break;
				}
				// and		[rd <- rs & rt]
				case R_AND_FUNC: {
					cp->cpu.R[ins->rd] = cp->cpu.R[ins->rs] & cp->cpu.R[ins->rt];

					printf("and	[rd <- rs & rt]\n");
					break;
				}
				// SLL  rd <- rt << shamt  left
				case R_SLL_FUNC: {
					cp->cpu.R[ins->rd] = cp->cpu.R[ins->rt] << ins->shamt;//shamt is int 
					
					printf("sll [rd <- rt << shamt]\n");
					break;
				}
				// SRL rd <- rt >> shamt  right
				case R_SRL_FUNC: {
					cp->cpu.R[ins->rd] =  cp->cpu.R[ins->rt] >> ins->shamt;
					
					printf("srl [rd <- rt >> shamt]\n"); 
					break;
				}
				// SRA rd <- rt >> sham  
				case R_SRA_FUNC: {
					cp->cpu.R[ins->rd] = ((int)cp->cpu.R[ins->rt] )>> ins->shamt; // transform u32->int 
					
					printf("sra [rd <- rt >> shamt](int)\n"); 					
					break;
				}
				// SLLV rd <- rt << rs 
				case R_SLLV_FUNC: {
					cp->cpu.R[ins->rd] = cp->cpu.R[ins->rt] << cp->cpu.R[ins->rs];
					
					printf("sllv [rd <- rt << rs]\n");
					break;
				}
				// SRLV rd <- rt >> rs 
				case R_SRLV_FUNC: {
					cp->cpu.R[ins->rd] = cp->cpu.R[ins->rt] >> cp->cpu.R[ins->rs];
					
					printf("srlv [rd <- rt >> rs](logical)\n"); 
					break;
				}
				// SRAV rd <- rt >> rs
				case R_SRAV_FUNC: {
					cp->cpu.R[ins->rd] = ((int)cp->cpu.R[ins->rt]) >> cp->cpu.R[ins->rs];
					
					printf("SRAV rd <- rt >> rs\n");
					break;
				}
				// JR PC <- rs
				case R_JR_FUNC: {
					cp->cpu.PC = cp->cpu.R[ins->rs];
					
					printf("jr [PC <- rs]\n");
					break;
				}
				// SUB  rd <- rs - rt
				case R_SUB_FUNC: {
					if (cp->cpu.R[ins->rs] >> 31 != cp->cpu.R[ins->rt] >> 31)
						if(((int)cp->cpu.R[ins->rs] - (int)cp->cpu.R[ins->rt])>>31 != cp->cpu.R[ins->rs]>>31) {
							printf("sub overflow\n");
							break;
						}
					cp->cpu.R[ins->rd] = (int)cp->cpu.R[ins->rs] - (int)cp->cpu.R[ins->rt];
					printf("sub [rd <- rs - rt](int)\n");
					break;
				}
				// SUBU  rd <- rs - rt
				case R_SUBU_FUNC: {
					cp->cpu.R[ins->rd] = cp->cpu.R[ins->rs] - cp->cpu.R[ins->rt];
					
					printf("subu [rd <- rs - rt]\n"); 
					break;
				}
				// OR  rd <- rs | rt
				case R_OR_FUNC: {
					cp->cpu.R[ins->rd] = cp->cpu.R[ins->rs] | cp->cpu.R[ins->rt];
					
					printf("or [rd <- rs | rt]\n");
					break;
				}
				// XOR rd <- rs xor rt
				case R_XOR_FUNC: {
					cp->cpu.R[ins->rd] = cp->cpu.R[ins->rs] ^ cp->cpu.R[ins->rt];
					
					printf("xor [rd <- rs ^ rt]\n");
					break;
				}
				// NOR  rd <- not(rs | rt)
				case R_NOR_FUNC: {
					cp->cpu.R[ins->rd] = ~(cp->cpu.R[ins->rs] | cp->cpu.R[ins->rt]);
					
					printf("nor [rd <- not(rs | rt)]\n");
					break;
				}
				// SLT if (rs < rt) rd=1 else rd=0
				case R_SLT_FUNC: {
					cp->cpu.R[ins->rd] = (int)cp->cpu.R[ins->rs] < (int)cp->cpu.R[ins->rt] ? 1:0;
					
					printf("slt [if (rs < rt) rd=1 else rd=0](int)\n");
					break;
				}
				// SLTU if (rs < rt) rd=1 else rd=0 
				case R_SLTU_FUNC: {
					cp->cpu.R[ins->rd] = cp->cpu.R[ins->rs] < cp->cpu.R[ins->rt] ? 1:0;
					
					printf("sltu [if (rs < rt) rd=1 else rd=0]\n"); 
					break;
				}
			}
			break;
		case TYPE_J:
			switch (ins->op) {
				// j PC <- (PC+4)[31..28],address,0,0  address=10000/4
				case J_J_OPCODE: {
					cp->cpu.PC = (cp->cpu.PC - 1) & strtol("1111000000000000000000", NULL, 2) + (ins->address << 2);
					printf("PC <- (PC + 4)[31..28], address, 0, 0   ；address = 10000 / 4\n");
					break;
				}
				// jal $31<-PC+4锛汸C <- (PC+4)[31..28],address,0,0 address=10000/4
				case J_JAL_OPCODE: {
					cp->cpu.R[$ra] = cp->cpu.PC;
					cp->cpu.PC = ((cp->cpu.PC - 1) & strtol("1111000000000000000000", NULL, 2)) + (ins->address << 2);
					printf("$31 <- PC + 4；PC <- (PC + 4)[31..28], address, 0, 0 ；address = 10000/4\n");
					break;
				}
			}
		break;
		case TYPE_I:
			switch (ins->op) {
				// addiu	[rt <- rs + (zero-extend)immediate]
				case I_ADDIU_OPCODE: {
					cp->cpu.R[ins->rt] = cp->cpu.R[ins->rs] + ins->imm;

					printf("addiu [rt <- rs + (zero-extend)immediate]\n");
					break;
				}
				// lw		[rt <- memory[rs+(sign-extend)immediate]]
				case I_LW_OPCODE: {
					cp->cpu.R[ins->rt] = cp->memory.addr[cp->cpu.R[ins->rs] + ins->imm];

					printf("lw [rt <- memory[rs+(sign-extend)immediate]]\n");
					break;
				}
				// ADDI rt <- rs + (sign-extend)immediate
				case I_ADDI_OPCODE: {
					if ((cp->cpu.R[ins->rs] & (1 << 31)) == (ins->imm & (1 << 31))) {
						if ( ((cp->cpu.R[ins->rs] + ins->imm) & (1 << 31)) != (cp->cpu.R[ins->rs] & (1 << 31)) ) {
							printf("addiu [rt <- rs + (sign-extend)immediate]\n");
							printf("overflow error\n");
							break;
						}
					}
					cp->cpu.R[ins->rt] = cp->cpu.R[ins->rs] + ins->imm;
					printf("addi [rt <- rs + (sign-extend)immediate]\n");
					break;
				}
				// ANDI  rt <- rs & (zero-extend)immediate
				case I_ANDI_OPCODE: {
					cp->cpu.R[ins->rt] = cp->cpu.R[ins->rs] & ins->imm;
					printf("andi [rt <- rs & (zero-extend)immediate]\n");
					break;
				}
				// ORI rt <- rs | (zero-extend)immediate
				case I_ORI_OPCODE: {
					cp->cpu.R[ins->rt] = cp->cpu.R[ins->rs] | ins->imm;
					printf("ORI [rt <- rs & (zero-extend)immediate]\n");
					break;
				}
				// XORI rt <- rs xor (zero-extend)immediate
				case I_XORI_OPCODE: {
					cp->cpu.R[ins->rt] = cp->cpu.R[ins->rs] ^ ins->imm;
					printf("XORI [rt <- rs xor (zero-extend)immediate]\n");
					break;
				}
				// LUI rt <- immediate*65536 
				case I_LUI_OPCODE: {
					// 如果做过符号扩展，那15-32位都是1，左移16位后符号位还是1
					// 如果没做过符号扩展，那15-32位都是0，左移16位后符号还是0
					cp->cpu.R[ins->rt] = ins->imm << 16;
					printf("LUI [rt <- immediate*65536]\n");
					break;
				}
				// SW memory[rs + (sign-extend)immediate] <- rt 
				case I_SW_OPCODE: {
					if ((cp->cpu.R[ins->rs] & (1 << 31)) == (1 << 31)) {
						if ((ins->imm & (1 << 31)) == (1 << 31)) {
							cp->memory.addr[*(int *)(&cp->cpu.R[ins->rs]) + *(int *)(&ins->imm)] = cp->cpu.R[ins->rt];
						} else {
							cp->memory.addr[*(int *)(&cp->cpu.R[ins->rs]) + ins->imm] = cp->cpu.R[ins->rt];
						}
					} else {
						if ((ins->imm & (1 << 31)) == (1 << 31)) {
							cp->memory.addr[cp->cpu.R[ins->rs] + *(int *)(&ins->imm)] = cp->cpu.R[ins->rt];
						} else {
							cp->memory.addr[cp->cpu.R[ins->rs] + ins->imm] = cp->cpu.R[ins->rt];
						}
					}
					printf("SW memory[rs + (sign-extend)immediate] <- rt\n");
					break;
				}
				// BEQ if (rs == rt) PC <- PC+4 + (sign-extend)immediate<<2 
				case I_BEQ_OPCODE: {
					if (cp->cpu.R[ins->rs] == cp->cpu.R[ins->rt]) {
						if ((ins->imm & (1 << 31)) == (1 << 31)) {
							cp->cpu.PC += *(int *)(&ins->imm) << 2;
						} else {
							cp->cpu.PC += ins->imm << 2;
						}
					}
					printf("BEQ if (rs == rt) PC <- PC+4 + (sign-extend)immediate<<2\n");
					break;
				}
				// BNE if (rs != rt) PC <- PC+4 + (sign-extend)immediate<<2 
				case I_BNE_OPCODE: {
					if (cp->cpu.R[ins->rs] != cp->cpu.R[ins->rt]) {
						if ((ins->imm & (1 << 31)) == (1 << 31)) {
							cp->cpu.PC += *(int *)(&ins->imm) << 2;
						} else {
							cp->cpu.PC += ins->imm << 2;
						}
					}
					printf("BNE if (rs != rt) PC <- PC+4 + (sign-extend)immediate<<2\n");
					break;
				}
				// SLTI if (rs <(sign-extend)immediate) rt=1 else rt=0
				case I_SLTI_OPCODE: {
					if (*(int *)(&cp->cpu.R[ins->rs]) < *(int *)(&ins->imm)) {
						cp->cpu.R[ins->rt] = 1;
					} else {
						cp->cpu.R[ins->rt] = 0;
					}
					printf("SLTI if (rs <(sign-extend)immediate) rt=1 else rt=0\n");
					break;
				}
				// SLTIU  if (rs <(zero-extend)immediate) rt=1 else rt=0
				case I_SLTIU_OPCODE: {
					if ((cp->cpu.R[ins->rs] & (1 << 31)) == (1 << 31)) {
						cp->cpu.R[ins->rt] = 0;
						printf("SLTIU  if (rs <(zero-extend)immediate) rt=1 else rt=0\n");
						break;
					}
					if (*(int *)(&cp->cpu.R[ins->rs]) < ins->imm) {
						cp->cpu.R[ins->rt] = 1;
					} else {
						cp->cpu.R[ins->rt] = 0;
					}
					printf("SLTIU  if (rs <(zero-extend)immediate) rt=1 else rt=0\n");
					break;
				}
			}
		break;
	}
	printf("**********************************************************************");
	return 0;
}

int computer_load_init(COMPUTER *cp, char *file)
{
	//load the image file
	int fd;
	int ret;

	// open the file
	if ((fd = open(file, O_RDONLY)) < 0)
	{
		printf("Error: open().\n");
		exit(-1);
	}

	// read from the program file (the program file <= 256 bytes) into the memory
	if ((ret = read(fd, &cp->memory, MAX_MEM_SIZE * 4)) < 0)
	{
		printf("Error: read().\n");
		exit(-1);
	}
	else if (ret > (MAX_MEM_SIZE * 4))
	{
		printf("Error: read() - Program is too big. \n");
		exit(-1);
	}

	//Initialize all registers
	cp->cpu.PC = 0; //Program counter
	cp->cpu.IR = 0; //Instruction regiser

	//General purpose register

	// constant value 0  $0 = $zero
	cp->cpu.R[0] = 0;
	// reserved for assembler  $1 = $at
	cp->cpu.R[1] = 0;
	// values for results and expression evaluation($2 - $3)
	cp->cpu.R[2] = 0; // $v0
	cp->cpu.R[3] = 0; // $v1

	// arguments ($4 - $7)
	cp->cpu.R[4] = 0; // $a0
	cp->cpu.R[5] = 0; // $a1
	cp->cpu.R[6] = 0; // $a2
	cp->cpu.R[7] = 0; // $a3

	// temporary, free ($8 - $15)
	for (int i = 8; i <= 15; i++)
	{
		cp->cpu.R[i] = 0; // $t0 - $t7
	}

	// reserved or saved ($16 - $23)
	for (int i = 16; i <= 23; i++)
	{
		cp->cpu.R[i] = 0; // $s0 - $s7
	}

	// temporary, free ($24 - $25)
	cp->cpu.R[24] = 0; // $t8
	cp->cpu.R[25] = 0; // $t9

	// reserved for operating system ($26 - $27)
	cp->cpu.R[26] = 0; // $k0
	cp->cpu.R[27] = 0; // $k1

	// global pointer ($28)
	cp->cpu.R[28] = 0; // $gp

	// stack pointer ($29)
	cp->cpu.R[29] = 0; // $sp

	// frame pointer ($30)
	cp->cpu.R[30] = 0; // $fp = $s8(reserved)

	// return address ($31)
	cp->cpu.R[31] = 0; // $ra

	return 0;
}

int print_cpu(COMPUTER *cp)
{
	printf("CPU Registers: PC-%d, [PC]-0x%x\n", cp->cpu.PC, cp->cpu.IR);
	for (int i = 0; i < 32; i++)
	{
		if (i != 0 && i % 4 == 0)
		{
			printf("\n");
		}
		printf("$reg%02d:0x%x, ", i, cp->cpu.R[i]);
		//printf("%s: %x ,",reg[i],cp->cpu.R[i]);
	}
	printf("\n");
	return 0;
}

int print_memory(COMPUTER *cp)
{
	//print the memory contents
	int i;
	for (i = 0; i < 64; i++)
	{
		print_instruction(i, cp->memory.addr[i]);
		// printf("%d = %d\n",i, cp->memory.addr[i]);
	}
	return 0;
}

int print_instruction(int i, uint32_t inst)
{
	int8_t *p = (int8_t *)&inst;

	int8_t low_addr_value = *p;
	int8_t sec_addr_value = *(p + 1);
	int8_t third_addr_value = *(p + 2);
	int8_t high_addr_value = *(p + 3);
	/* Intel32 is little endian - the least significant byte first (lowest address) and the most significant byte last (highest address) */
	printf("[%d]: Instruction-0x%x;LowAddr-%d,Second-%d,Third-%d,HighAddr-%d\n",
		   i, inst, low_addr_value, sec_addr_value, third_addr_value, high_addr_value);
	return 0;
}
