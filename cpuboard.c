/*
*	Project-based Learning II (CPU)
*
*	Program:	instruction set simulator of the Educational CPU Board
*	File Name:	cpuboard.c
*	Descrioption:	simulation(emulation) of an instruction
*/

#include "cpuboard.h"
#include <stdio.h>


/*=============================================================================
*   Simulation of a Single Instruction
*===========================================================================*/


//==========================================================================
// Grobal variable
//==========================================================================
Uword obj_code;  // Grobal variable
Uword AA,BB; //operand
Uword code_Reference_Value;




//==========================================================================
// functions
//==========================================================================
void return_obj_code(Cpub *cpub)
{
	obj_code = cpub->mem[cpub->pc];
	cpub->pc += 1;
}


void code_judge(Uword obj_code)
{
	AA = obj_code & 0x8;
	BB = obj_code & 0x7;
	code_Reference_Value = obj_code & 0xf0;
}


void LD(Cpub *cpub,Uword code) //(B)->A
{
	switch(AA){
		case 0b00000000: //A=ACC
		switch(BB){
			case 0b00000000: //B=ACC
			cpub->acc = cpub->acc;
			break;
			case 0b00000001: //B=IX
			cpub->acc = cpub->ix;
			break;
			case 0b00000010: //B=d
			cpub->acc = cpub->mem[cpub->pc];
			cpub->pc += 1;
			break;
			case 0b00000100: //B=[d] prog_mem
			cpub->acc = cpub->mem[0x000 + cpub->mem[cpub->pc]];
			cpub->pc += 1;
			break;
			case 0b00000101: //B=(d) data_mem
			cpub->acc = cpub->mem[0x100 + cpub->mem[cpub->pc]];
			cpub->pc += 1;
			break;
			case 0b00000110: ///B=[IX+d] prog_mem
			cpub->acc = cpub->mem[0x000 +  cpub->ix + cpub->mem[cpub->pc]];
			cpub->pc += 1;
			break;
			case 0b00000111: //B=(IX+d) data_mem
			cpub->acc = cpub->mem[0x100 +  cpub->ix + cpub->mem[cpub->pc]];
			cpub->pc += 1;
			break;
			default:
			break;
		}
		break;
		case 0b00001000: //A=IX
		switch(BB){
			case 0b00000000: //B=ACC
			cpub->ix = cpub->acc;
			break;
			case 0b00000001: //B=IX
			cpub->ix = cpub->ix;
			break;
			case 0b00000010: //B=d
			cpub->ix = cpub->mem[cpub->pc];
			cpub->pc += 1;
			break;
			case 0b00000100: //B=[d] prog_mem
			cpub->ix = cpub->mem[0x000 + cpub->mem[cpub->pc]];
			cpub->pc += 1;
			break;
			case 0b00000101: //B=(d) data_mem
			cpub->ix = cpub->mem[0x100 + cpub->mem[cpub->pc]];
			cpub->pc += 1;
			break;
			case 0b00000110: ///B=[IX+d] prog_mem
			cpub->ix = cpub->mem[0x000 +  cpub->ix + cpub->mem[cpub->pc]];
			cpub->pc +=1;
			break;
			case 0b00000111: //B=(IX+d) data_mem
			cpub->ix = cpub->mem[0x100 +  cpub->ix + cpub->mem[cpub->pc]];
			cpub->pc += 1;
			break;
			default:
			break;
		}
		break;
		default:
		break;
	}
}


void ST(Cpub *cpub,Uword code)
{
	switch(AA){
		case 0b00000000: //A=ACC
		switch(BB){
			case 0b00000100: //B=[d] prog_mem
			cpub->mem[0x000 + cpub->mem[cpub->pc]] = cpub->acc;
			cpub->pc += 1;
			break;
			case 0b00000101: //B=(d) data_mem
			cpub->mem[0x100 + cpub->mem[cpub->pc]] = cpub->acc;
			cpub->pc += 1;
			break;
			case 0b00000110: ///B=[IX+d] prog_mem
			cpub->mem[0x000 +  cpub->ix + cpub->mem[cpub->pc]] = cpub->acc;
			cpub->pc += 1;
			break;
			case 0b00000111: //B=(IX+d) data_mem
			cpub->mem[0x100 +  cpub->ix + cpub->mem[cpub->pc]] = cpub->acc;
			cpub->pc += 1;
			break;
			default:
			break;
		}
		break;
		case 0b00001000: //A=IX
		switch(BB){
			case 0b00000100: //B=[d] prog_mem
			cpub->mem[0x000 + cpub->mem[cpub->pc]] = cpub->ix;
			cpub->pc += 1;
			break;
			case 0b00000101: //B=(d) data_mem
			cpub->mem[0x100 + cpub->mem[cpub->pc]] = cpub->ix;
			cpub->pc += 1;
			break;
			case 0b00000110: ///B=[IX+d] prog_mem
			cpub->mem[ 0x000 + cpub->ix + cpub->mem[cpub->pc]] = cpub->ix;
			cpub->pc += 1;
			break;
			case 0b00000111: //B=(IX+d) data_mem
			cpub->mem[ 0x100 + cpub->ix + cpub->mem[cpub->pc]] = cpub->ix;
			cpub->pc += 1;
			break;
			default:
			break;
		}
		break;
		default:
		break;
	}
}


void set_znv_flag(Cpub *cpub, Uword A, Bit msb_A, Bit msb_B)
{
	cpub->zf = (A == 0)? 1 : 0;  // zero flag
	cpub->nf = A >> 7;  // negative flag
	cpub->vf = (msb_A == msb_B && msb_A != cpub->nf)? 1 : 0;  // overflow flag
}


unsigned int returnaddr(Cpub *cpub){ //pcをプログラム領域のアドレス値にする関数
	Addr area;
	unsigned int addr;
	addr = cpub -> pc;
	area = 0x000;
	addr |= area;
	return addr;
}


//======================================================================
// step
//======================================================================
int step(Cpub *cpub)
{
	return_obj_code(cpub);  // 命令fetch
	code_judge(obj_code);

	switch(obj_code){
		case 0x00 :	//NOP: do nothing
		break;
		case 0x10 : //OUT (ACC) -> OBUF
		cpub->obuf.buf = cpub->acc;
		break;
		case 0x1f : //IN (IBUF) -> ACC
		cpub->acc = cpub->ibuf->buf;
		break;
		case 0x20 : //RCF 0 -> CF
		cpub->cf = 0;
		break;
		case 0x2f : //SCF 1 -> CF
		cpub->cf = 1;
		break;
		case 0x0f : //HALT
		return RUN_HALT;
		default:
		break;
	}

	switch(code_Reference_Value){
		case 0x60: //LD
		LD(cpub,obj_code);
		break;
		case 0x70: //ST
		ST(cpub,obj_code);
		default:
		break;
	}

	if( obj_code & 0b10000000 ){  // msb == 1
		/* operand fetch */
		Uword *A;
		A = ((obj_code & 0b00001000) == 0)? &(cpub->acc): &(cpub->ix);
		Uword B, d_pc = 0;
		switch( obj_code & 0b00000111 ){
			// B = ACC
			case 0x0 :  //  goto case 8
			case 0x8 :  B = cpub->acc;  break;
			// B = IX
			case 0x1 :  //  goto case 9
			case 0x9 :  B = cpub->ix;  break;
			// B = d
			case 0x2 :  // goto case A
			case 0xA :  // goto case 3
			case 0x3 :  // goto case B
			case 0xB :  B = cpub->mem[cpub->pc];  d_pc++;  break;
			// B = [d]  (.text)
			case 0x4 :  // goto case C
			case 0xC :  B = cpub->mem[0x000+cpub->mem[cpub->pc]];  d_pc++;  break;
			// B = (d)  (.data)
			case 0x5 :  // goto case D
			case 0xD :  B = cpub->mem[0x100+cpub->mem[cpub->pc]];  d_pc++;  break;
			// B = [IX+d]  (.text)
			case 0x6 :  // goto case E
			case 0xE :  B = cpub->mem[0x000+cpub->ix+cpub->mem[cpub->pc]];
			d_pc++;
			break;
			// B = (IX+d)  (.data)
			case 0x7 :  // goto case F
			case 0xF :
			B = cpub->mem[0x100+cpub->ix+cpub->mem[cpub->pc]];
			d_pc++;
			break;
			default : break;
		}
		Bit msb_A = *A >> 7;  // most significant bit of A
		Bit msb_B = B >> 7;  // most significant bit of B
		int is_carry_set;  // for setting caeey flag

		/* calculate */
		switch( obj_code & 0b11110000 ){  // 下4桁を無効化(0)
			// ADD ===============================================================
			case 0xB0 :
			*A = *A + B;  // (A) + (B) -> (A)
			set_znv_flag(cpub, *A, msb_A, msb_B);
			break;
			// ADC  ===============================================================
			case 0x90 :
			is_carry_set = *A + B + cpub->cf;  // for carry flag
			*A = *A + B + cpub->cf;  // (A) + (B) + CF -> (A)
			cpub->cf = (0x00 <= is_carry_set && is_carry_set <= 0xff)? 0 : 1;
			set_znv_flag(cpub, *A, msb_A, msb_B);
			break;
			// SUB  ===============================================================
			case 0xA0 :
			*A = *A - B;  // (A) - (B) -> (A)
			set_znv_flag(cpub, *A, msb_A, msb_B);
			break;
			// SBC  ===============================================================
			case 0x80 :
			is_carry_set = *A + (~B+1) - cpub->cf;  // for carry flag
			*A = *A + (~B+1) - cpub->cf;  // (A) + (B) + CF -> (A)
			cpub->cf = (0x00 <= is_carry_set && is_carry_set <= 0xff)? 0 : 1;
			set_znv_flag(cpub, *A, msb_A, !msb_B&0x01);
			break;
			// CMP  ===============================================================
			case 0xF0 :
			set_znv_flag(cpub, *A-B, msb_A, msb_B);
			break;
			// AND  ===============================================================
			case 0xE0 :
			*A = *A & B;  // (A) ^ (B) -> (A)
			set_znv_flag(cpub, *A, 0, 1);
			break;
			// OR  ===============================================================
			case 0xD0 :
			*A = *A | B;  // (A) v (B) -> (A)
			set_znv_flag(cpub, *A, 0, 1);
			break;
			// EOR  ===============================================================
			case 0xC0 :
			*A = *A ^ B;  // (A) ⊻ (B) -> (A)
			set_znv_flag(cpub, *A, 0, 1);
			break;
			// default  ===========================================================
			default : break;
		}
		cpub->pc += d_pc;
		return RUN_STEP;
	}else if( (obj_code & 0b11110000) == 0x40 ){  // shift or rotate
		Uword *A;
		A = ((obj_code & 0b00001000) == 0)? &(cpub->acc): &(cpub->ix);
		Bit msb = *A >> 7;
		Bit lsb = *A & 0x01;
		Bit n_msb, n_lsb;
		switch( obj_code & 0b00000011 ){  // 上6桁を無効化(0)
			// _RA  ===============================================================
			case 0x00 :
			n_msb = ((obj_code & 0b00000100) == 0)? msb : cpub->cf;
			cpub->cf = lsb;
			*A >>= 1;
			*A |= (n_msb == 0)? 0x00 : 0x80;
			set_znv_flag(cpub, *A, 0, 1);
			break;
			// _LA  ===============================================================
			case 0x01:
			n_lsb = ((obj_code & 0b00000100) == 0)? 0 : cpub->cf;
			cpub->cf = msb;
			*A <<= 1;
			*A |= (n_lsb == 0)? 0x00 : 0x01;
			set_znv_flag(cpub, *A, msb, msb);
			break;
			// _RL  ===============================================================
			case 0x02 :
			n_msb = ((obj_code & 0b00000100) == 0)? 0 : lsb;
			cpub->cf = lsb;
			*A >>= 1;
			*A |= (n_msb == 0)? 0x00 : 0x80;
			set_znv_flag(cpub, *A, 0, 1);
			break;
			// _LL  ===============================================================
			case 0x03 :
			n_lsb = ((obj_code & 0b00000100) == 0)? cpub->cf : msb;
			cpub->cf = msb;
			*A <<= 1;
			*A |= (n_lsb == 0)? 0x00 : 0x01;
			set_znv_flag(cpub, *A, 0, 1);
			break;
		}
		return RUN_STEP;
	}

	switch(obj_code) {
		case  0x30 :  // BA
		cpub ->pc =  cpub->mem[ returnaddr(cpub) ] ;  //次のプログラムを新しいpc値と
		break;
		case 0x31 :  // BNZ
		if( cpub -> zf == 0 ) {
			return_obj_code(cpub);
			cpub ->pc = obj_code;
		}
		else if( cpub -> zf != 0 ) {
			cpub->pc += 1;
		}
		break;
		case 0x32 :  // BZP
		if( cpub -> nf == 0 ) {
			cpub ->pc =  cpub->mem[ returnaddr(cpub) ] ;
		}
		else if(cpub -> nf != 0 ) {
			cpub->pc += 1;
		}
		break;
		case 0x33 :  // BP
		if( ( cpub -> nf || cpub -> zf)   == 0 ) {
			cpub ->pc =  cpub->mem[ returnaddr(cpub) ] ;
		}
		else if( (cpub -> nf || cpub -> zf)   != 0 ) {
			cpub->pc += 1;
		}
		break;
		case 0x34 :  // BNI
		if(  cpub->obuf.flag == 0  ) {
			cpub ->pc =  cpub->mem[ returnaddr(cpub) ] ;
		}
		else if( cpub->obuf.flag != 0  ) {
			cpub->pc += 1;
		}
		break;
		case 0x35 :  // BNC
		if(  cpub -> cf == 0 ) {
			cpub ->pc =  cpub->mem[ returnaddr(cpub) ];
		}
		else if( cpub -> cf != 0  ) {
			cpub->pc += 1;
		}
		break;
		case 0x36 :  // BGE
		if( (cpub -> vf ^ cpub -> nf) == 0  ) {
			cpub ->pc =  cpub->mem[ returnaddr(cpub) ] ;
		}
		else if( (cpub -> vf ^ cpub -> nf)  != 0  ) {
			cpub->pc += 1;
		}
		break;
		case 0x37 :  // BGT
		if( ( (cpub -> vf ^ cpub -> nf) || cpub -> zf )== 0 ) {
			cpub ->pc =  cpub->mem[ returnaddr(cpub) ] ;
		}
		else if( ( (cpub -> vf ^ cpub -> nf) || cpub -> zf ) != 0 ) {
			cpub->pc += 1;
		}
		break;
		case 0x38 :  // BVF
		if( cpub -> vf == 1 ) {
			cpub ->pc =  cpub->mem[ returnaddr(cpub) ] ;
		}
		else if( cpub -> vf  != 1 ) {
			cpub->pc += 1;
		}
		break;
		case 0x39 :  // BZ
		if( cpub -> zf == 1 ) {
			cpub ->pc =  cpub->mem[ returnaddr(cpub) ] ;
		}
		else if( cpub -> zf != 1 ) {
			cpub->pc += 1;
		}
		break;
		case 0x3A :  // BN
		if( cpub -> nf == 1 ) {
			cpub ->pc =  cpub->mem[ returnaddr(cpub) ] ;
		}
		else if( cpub -> nf  != 1 ) {
			cpub->pc += 1;
		}
		break;
		case 0x3B :  // BZN
		if( (cpub -> nf || cpub -> zf) == 1 ) {
			cpub ->pc =  cpub->mem[ returnaddr(cpub) ] ;
		}
		else if( (cpub -> nf || cpub -> zf) != 1 ) {
			cpub->pc += 1;
		}
		break;
		case 0x3C :  // BNZ
		if( cpub->obuf.flag == 1 ) {
			cpub ->pc =  cpub->mem[ returnaddr(cpub) ] ;
		}
		else if( cpub->obuf.flag != 1 ) {
			cpub->pc += 1;
		}
		break;
		case 0x3D :  // BC
		if( cpub -> cf == 1 ) {
			cpub ->pc =  cpub->mem[ returnaddr(cpub) ] ;
		}
		else if( cpub -> cf != 1 ) {
			cpub->pc += 1;
		}
		break;
		case 0x3E :  // BLT
		if( (cpub -> vf ^ cpub -> nf) == 1 ) {
			cpub ->pc =  cpub->mem[ returnaddr(cpub) ] ;
		}
		else if( (cpub -> vf ^ cpub -> nf)  != 1 ) {
			cpub->pc += 1;
		}
		break;
		case 0x3F :  // BLE
		if( ( (cpub -> vf ^ cpub -> nf) || cpub -> zf ) == 1 ) {
			cpub ->pc =  cpub->mem[ returnaddr(cpub) ] ;
		}
		else if( ( (cpub -> vf ^ cpub -> nf) || cpub -> zf ) != 1 ) {
			cpub->pc += 1;
		}
		break;
	}

}
