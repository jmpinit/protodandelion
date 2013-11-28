#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "beta.h"

void beta_load(Beta* beta, const char* filename) {
	FILE *fp = fopen(filename, "rb");
	int size_loaded = fread(beta->memory, sizeof(uint32_t), beta->memsize, fp);

	if(size_loaded > beta->memsize) {
		printf("beta_load: expected %d u32s but got %d (too much).\n", beta->memsize, size_loaded);
		exit(1);
	} else {
		printf("loaded %d u32s into beta.\n", size_loaded);
	}
}

void beta_dump_info(Beta* beta) {
	printf("PC = %d (%x)\n", beta->pc, beta->pc);
}

void beta_dump_registers(Beta* beta) {
	for(int i=0; i < 31; i++) {
		printf("R%d\t= %x\n", i, beta->registers[i]);
	}
}

void beta_dump_memory(Beta* beta) {
	for(int i=0; i < beta->memsize; i++)
		printf("%x) %x\n", i, beta->memory[i]);
}

Beta* beta_create(int memsize) {
	Beta* newbeta = calloc(1, sizeof(Beta));
	uint32_t* memory = calloc(memsize, sizeof(uint32_t));

	newbeta->memsize = memsize;
	newbeta->memory = memory;

	return newbeta;
}

uint32_t beta_read_reg(Beta* beta, uint8_t index) {
	index &= 0x1F;	// register indices are 5 bits

	if(index == 31)
		return 0;
	else
		return beta->registers[index];
}

void beta_write_reg(Beta* beta, uint32_t value, uint8_t index) {
	printf("R%d = 0x%x\n", index, value);
	index &= 0x1F;	// register indices are 5 bits
	if(index != 31) beta->registers[index] = value;
}

void beta_tick(Beta* beta) {
	// get next instruction
	uint32_t instruction = beta->memory[beta->pc/4];

	// decode instruction fields
	uint8_t opcode		= instruction & 0xFC000000;
	uint8_t reg_c		= instruction & 0x03E00000;
	uint8_t reg_a		= instruction & 0x001F0000;
	uint8_t reg_b		= instruction & 0x0000F800;
	int16_t literal		= instruction & 0xFFFF;

	// get data
	uint32_t val_c	= beta_read_reg(beta, reg_c);
	uint32_t val_a	= beta_read_reg(beta, reg_a);
	uint32_t val_b	= beta_read_reg(beta, reg_b);

	// execute instruction
	uint32_t res, ea;
	switch(opcode) {
		case ADD:	beta_write_reg(beta, val_a + val_b, reg_c);		break;
		case ADDC:	beta_write_reg(beta, val_a + literal, reg_c);	break;
		case AND:	beta_write_reg(beta, val_a & val_b, reg_c);		break;
		case ANDC:	beta_write_reg(beta, val_a & literal, reg_c);	break;
		case BEQ:
			beta_write_reg(beta, beta->pc+4, reg_c);
			ea = beta->pc + ((int32_t)literal) * 4;
			if(val_a == 0) beta->pc = ea;
			break;
		case BNE:
			beta_write_reg(beta, beta->pc+4, reg_c);
			ea = beta->pc + ((int32_t)literal) * 4;
			if(val_a != 0) beta->pc = ea;
			break;
		case CMPEQ:
			beta_write_reg(beta, (val_a == val_b)? 1: 0, reg_c);
			break;
		case CMPEQC:
			beta_write_reg(beta, (val_a == literal)? 1: 0, reg_c);
			break;
		case CMPLE: break;
			beta_write_reg(beta, (val_a <= val_b)? 1: 0, reg_c);
		case CMPLEC: break;
			beta_write_reg(beta, (val_a <= literal)? 1: 0, reg_c);
		case CMPLT: break;
			beta_write_reg(beta, (val_a < val_b)? 1: 0, reg_c);
		case CMPLTC: break;
			beta_write_reg(beta, (val_a < literal)? 1: 0, reg_c);
		case DIV:	beta_write_reg(beta, val_a / val_b, reg_c);		break;
		case DIVC:	beta_write_reg(beta, val_a / literal, reg_c);	break;
		case JMP:
			beta_write_reg(beta, beta->pc+4, reg_c);
			ea = val_a & 0xFFFFFFFC;
			beta->pc = ea;
			break;
		case LD:
			ea = val_a + (int32_t)literal;
			beta_write_reg(beta, beta->memory[ea >> 2], reg_c);
			break;
		case LDR:
			ea = beta->pc + ((int32_t)literal) * 4;
			beta_write_reg(beta, beta->memory[ea >> 2], reg_c);
			break;
		case MUL:	beta_write_reg(beta, val_a * val_b, reg_c);		break;
		case MULC:	beta_write_reg(beta, val_a * literal, reg_c);	break;
		case OR:	beta_write_reg(beta, val_a | val_b, reg_c);		break;
		case ORC:	beta_write_reg(beta, val_a | literal, reg_c);	break;
		case SHL:	beta_write_reg(beta, val_a << val_b, reg_c);	break;
		case SHLC:	beta_write_reg(beta, val_a << literal, reg_c);	break;
		case SHR:	beta_write_reg(beta, val_a >> val_b, reg_c);	break;
		case SHRC:	beta_write_reg(beta, val_a >> literal, reg_c);	break;
		case SRA:
			res = val_a >> val_b;

			if(val_a & (1 << 31))
				res |= 0xFFFFFFFF << (32 - val_b);

			beta_write_reg(beta, res, reg_c);

			break;
		case SRAC:
			res = val_a >> literal;

			if(val_a & (1 << 31))
				res |= 0xFFFFFFFF << (32 - literal);

			beta_write_reg(beta, res, reg_c);

			break;
		case SUB:	beta_write_reg(beta, val_a - val_b, reg_c);			break;
		case SUBC:	beta_write_reg(beta, val_a - literal, reg_c);		break;
		case ST:
			ea = val_a + (int32_t)literal;
			beta->memory[ea >> 2] = val_c;
			break;
		case XOR:	beta_write_reg(beta, val_a ^ val_b, reg_c);			break;
		case XORC:	beta_write_reg(beta, val_a ^ literal, reg_c);		break;
		case XNOR:	beta_write_reg(beta, ~(val_a ^ val_b), reg_c);		break;
		case XNORC: beta_write_reg(beta, ~(val_a ^ literal), reg_c);	break;
		default: break;
	}

	// move on
	beta->pc += 4;
}
