#include <stdint.h>
#include <stdlib.h>

#include "beta.h"

Beta* beta_create(int memsize) {
	Beta* newbeta = calloc(1, sizeof(Beta));
	uint32_t* memory = calloc(memsize, sizeof(uint32_t));
	newbeta->memory = memory;
	return newbeta;
}

uint32_t beta_read_reg(Beta* beta, uint8_t index) {
	return 0;
}

void beta_write_reg(Beta* beta, uint8_t index, uint32_t value) {

}

void beta_tick(Beta* beta) {
	// get next instruction
	uint32_t instruction = beta->memory[beta->pc];

	// decode instruction fields
	uint8_t opcode	= instruction & 0xFC000000;
	uint8_t reg_c	= instruction & 0x03E00000;
	uint8_t reg_a	= instruction & 0x001F0000;
	uint8_t reg_b	= instruction & 0x0000F800;
	uint8_t literal	= instruction & 0xFFFF;

	// get data
	uint32_t val_c	= beta_read_reg(beta, reg_c);
	uint32_t val_a	= beta_read_reg(beta, reg_a);
	uint32_t val_b	= beta_read_reg(beta, reg_b);

	// execute instruction
	uint32_t res;
	switch(opcode) {
		case ADD:	beta_write_reg(beta, val_a + val_b, reg_c);		break;
		case ADDC:	beta_write_reg(beta, val_a + literal, reg_c);		break;
		case AND:	beta_write_reg(beta, val_a & val_b, reg_c);		break;
		case ANDC:	beta_write_reg(beta, val_a & literal, reg_c);		break;
		case BEQ:	break;
		case CMPEQ:	break;
		case CMPEQC:break;
		case CMPLE: break;
		case CMPLEC: break;
		case CMPLT: break;
		case CMPLTC: break;
		case DIV:	beta_write_reg(beta, val_a / val_b, reg_c);		break;
		case DIVC:	beta_write_reg(beta, val_a / literal, reg_c);		break;
		case JMP:	break;
		case LD: break;
		case LDR: break;
		case MUL:	beta_write_reg(beta, val_a * val_b, reg_c);		break;
		case MULC:	beta_write_reg(beta, val_a * literal, reg_c);		break;
		case OR:	beta_write_reg(beta, val_a | val_b, reg_c);		break;
		case ORC:	beta_write_reg(beta, val_a | literal, reg_c);		break;
		case SHL:	beta_write_reg(beta, val_a << val_b, reg_c);		break;
		case SHLC:	beta_write_reg(beta, val_a << literal, reg_c);	break;
		case SHR:	beta_write_reg(beta, val_a >> val_b, reg_c);		break;
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
		case SUB:	beta_write_reg(beta, val_a - val_b, reg_c);		break;
		case SUBC:	beta_write_reg(beta, val_a - literal, reg_c);		break;
		case ST: break;
		case XOR:	beta_write_reg(beta, val_a ^ val_b, reg_c);		break;
		case XORC:	beta_write_reg(beta, val_a ^ literal, reg_c);		break;
		case XNOR:	beta_write_reg(beta, ~(val_a ^ val_b), reg_c);		break;
		case XNORC: beta_write_reg(beta, ~(val_a ^ literal), reg_c);	break;
		default: break;
	}
}
