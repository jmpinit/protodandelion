#include <stdint.h>

#define REG_XP		30
#define REG_ZERO	31

typedef enum {
	ADD		= 0x20,
	ADDC	= 0x30,
	AND		= 0x28,
	ANDC	= 0x38,
	BEQ		= 0x1C,
	BNE		= 0x1D,
	CMPEQ	= 0x24,
	CMPEQC	= 0x34,
	CMPLE	= 0x26,
	CMPLEC	= 0x36,
	CMPLT	= 0x25,
	CMPLTC	= 0x35,
	DIV		= 0x23,
	DIVC	= 0x33,
	JMP		= 0x1B,
	LD		= 0x18,
	LDR		= 0x1F,
	MUL		= 0x22,
	MULC	= 0x32,
	OR		= 0x29,
	ORC		= 0x39,
	SHL		= 0x2C,
	SHLC	= 0x3C,
	SHR		= 0x2D,
	SHRC	= 0x3D,
	SRA		= 0x2E,
	SRAC	= 0x3E,
	SUB		= 0x21,
	SUBC	= 0x31,
	ST		= 0x19,
	XOR		= 0x2A,
	XORC	= 0x3A,
	XNOR	= 0x2B,
	XNORC	= 0x3B
} Instruction;

typedef struct Beta {
	unsigned int memsize;
	uint32_t* memory;
	uint32_t pc;
	uint32_t registers[31];
} Beta;

// simulation
Beta*		beta_create(int memsize);
void		beta_tick(Beta*);
uint32_t	beta_read_reg(Beta*, uint8_t index);
void		beta_write_reg(Beta*, uint8_t index, uint32_t value);

// control
void		beta_load(Beta*, const char* filename);

// debug
void		beta_dump_info(Beta*);
void		beta_dump_registers(Beta*);
