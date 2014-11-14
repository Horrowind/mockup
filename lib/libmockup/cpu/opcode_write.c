#include "cpu/memory.h"
#include "cpu/opcode_write.h"

#define A 0
#define X 1
#define Y 2
#define Z 3
#define S 4
#define D 5

#define op_write_addr_gen(name, num)					\
    void op_write_addr_##name##_b(cpu_t* cpu) {				\
	cpu->aa.l = op_readpc(cpu);					\
	cpu->aa.h = op_readpc(cpu);					\
	op_writedbr(cpu, cpu->aa.w, cpu->regs.r[num].w);		\
    }									\
									\
    void op_write_addr##name##_w(cpu_t* cpu) {				\
	cpu->aa.l = op_readpc(cpu);					\
	cpu->aa.h = op_readpc(cpu);					\
	op_writedbr(cpu, cpu->aa.w + 0, cpu->regs.r[num].w >> 0);	\
	op_writedbr(cpu, cpu->aa.w + 1, cpu->regs.r[num].w >> 8);	\
    }

op_write_addr_gen(sty, Y);
op_write_addr_gen(sta, A);
op_write_addr_gen(stx, X);
op_write_addr_gen(stz, Z);

#undef op_write_addr_gen

#define op_write_addrr_gen(name, num, i)				\
    void op_write_addrr_##name##_b(cpu_t* cpu) {			\
    cpu->aa.l = op_readpc(cpu);						\
    cpu->aa.h = op_readpc(cpu);						\
    op_writedbr(cpu, cpu->aa.w + cpu->regs.r[i].w, cpu->regs.r[num].w);	\
    }									\
     									\
    void op_write_addrr_##name##_w(cpu_t* cpu) {			\
	cpu->aa.l = op_readpc(cpu);					\
	cpu->aa.h = op_readpc(cpu);					\
	op_writedbr(cpu, cpu->aa.w + cpu->regs.r[i].w + 0, cpu->regs.r[num].w >> 0); \
	op_writedbr(cpu, cpu->aa.w + cpu->regs.r[i].w + 1, cpu->regs.r[num].w >> 8); \
    }

op_write_addrr_gen(stay, A, Y);
op_write_addrr_gen(stax, A, X);
op_write_addrr_gen(stz, Z, X);

#undef op_write_addrr_gen

#define op_write_longr_gen(name, i)					\
    void op_write_longr_##name##_b(cpu_t* cpu) {			\
	cpu->aa.l = op_readpc(cpu);					\
	cpu->aa.h = op_readpc(cpu);					\
	cpu->aa.b = op_readpc(cpu);					\
	op_writelong(cpu, cpu->aa.d + cpu->regs.r[i].w, cpu->regs.a.l);	\
    }									\
    									\
    void op_write_longr_##name##_w(cpu_t* cpu) {			\
	cpu->aa.l = op_readpc(cpu);					\
	cpu->aa.h = op_readpc(cpu);					\
	cpu->aa.b = op_readpc(cpu);					\
	op_writelong(cpu, cpu->aa.d + cpu->regs.r[i].w + 0, cpu->regs.a.l); \
	op_writelong(cpu, cpu->aa.d + cpu->regs.r[i].w + 1, cpu->regs.a.h); \
    }

op_write_longr_gen(sta, Z);
op_write_longr_gen(stax, X);

#undef op_write_longr_gen	 


#define op_write_dp_gen(name, num)				\
    void op_write_dp_##name##_b(cpu_t* cpu) {			\
	cpu->dp = op_readpc(cpu);				\
	op_writedp(cpu, cpu->dp, cpu->regs.r[num].w);		\
    }								\
    								\
    void op_write_dp_##name##_w(cpu_t* cpu) {			\
	cpu->dp = op_readpc(cpu);				\
	op_writedp(cpu, cpu->dp + 0, cpu->regs.r[num].w >> 0);	\
	op_writedp(cpu, cpu->dp + 1, cpu->regs.r[num].w >> 8);	\
    }

op_write_dp_gen(stz, Z);
op_write_dp_gen(sty, Y);
op_write_dp_gen(sta, A);
op_write_dp_gen(stx, X);

#undef op_write_dp_gen

#define op_write_dpr_gen(name, num, i)					\
    void op_write_dpr_##name##_b(cpu_t* cpu) {				\
	cpu->dp = op_readpc(cpu);					\
	op_writedp(cpu, cpu->dp + cpu->regs.r[i].w, cpu->regs.r[num].w);	\
    }									\
									\
    void op_write_dpr_##name##_w(cpu_t* cpu) {				\
	cpu->dp = op_readpc(cpu);					\
	op_writedp(cpu, cpu->dp + cpu->regs.r[i].w + 0, cpu->regs.r[num].w >> 0); \
	op_writedp(cpu, cpu->dp + cpu->regs.r[i].w + 1, cpu->regs.r[num].w >> 8); \
    }

op_write_dpr_gen(stz, Z, X);
op_write_dpr_gen(sty, Y, X);
op_write_dpr_gen(sta, A, X);
op_write_dpr_gen(stx, X, Y);

#undef op_write_dpr_gen

void op_sta_idp_b(cpu_t* cpu) {
  cpu->dp = op_readpc(cpu);
  cpu->aa.l = op_readdp(cpu, cpu->dp + 0);
  cpu->aa.h = op_readdp(cpu, cpu->dp + 1);
  op_writedbr(cpu, cpu->aa.w, cpu->regs.a.l);
}

void op_sta_idp_w(cpu_t* cpu) {
  cpu->dp = op_readpc(cpu);
  cpu->aa.l = op_readdp(cpu, cpu->dp + 0);
  cpu->aa.h = op_readdp(cpu, cpu->dp + 1);
  op_writedbr(cpu, cpu->aa.w + 0, cpu->regs.a.l);
  op_writedbr(cpu, cpu->aa.w + 1, cpu->regs.a.h);
}

void op_sta_ildp_b(cpu_t* cpu) {
  cpu->dp = op_readpc(cpu);
  cpu->aa.l = op_readdp(cpu, cpu->dp + 0);
  cpu->aa.h = op_readdp(cpu, cpu->dp + 1);
  cpu->aa.b = op_readdp(cpu, cpu->dp + 2);
  op_writelong(cpu, cpu->aa.d, cpu->regs.a.l);
}

void op_sta_ildp_w(cpu_t* cpu) {
  cpu->dp = op_readpc(cpu);
  cpu->aa.l = op_readdp(cpu, cpu->dp + 0);
  cpu->aa.h = op_readdp(cpu, cpu->dp + 1);
  cpu->aa.b = op_readdp(cpu, cpu->dp + 2);
  op_writelong(cpu, cpu->aa.d + 0, cpu->regs.a.l);
  op_writelong(cpu, cpu->aa.d + 1, cpu->regs.a.h);
}

void op_sta_idpx_b(cpu_t* cpu) {
  cpu->dp = op_readpc(cpu);
  cpu->aa.l = op_readdp(cpu, cpu->dp + cpu->regs.x.w + 0);
  cpu->aa.h = op_readdp(cpu, cpu->dp + cpu->regs.x.w + 1);
  op_writedbr(cpu, cpu->aa.w, cpu->regs.a.l);
}

void op_sta_idpx_w(cpu_t* cpu) {
  cpu->dp = op_readpc(cpu);
  cpu->aa.l = op_readdp(cpu, cpu->dp + cpu->regs.x.w + 0);
  cpu->aa.h = op_readdp(cpu, cpu->dp + cpu->regs.x.w + 1);
  op_writedbr(cpu, cpu->aa.w + 0, cpu->regs.a.l);
  op_writedbr(cpu, cpu->aa.w + 1, cpu->regs.a.h);
}

void op_sta_idpy_b(cpu_t* cpu) {
  cpu->dp = op_readpc(cpu);
  cpu->aa.l = op_readdp(cpu, cpu->dp + 0);
  cpu->aa.h = op_readdp(cpu, cpu->dp + 1);
  op_writedbr(cpu, cpu->aa.w + cpu->regs.y.w, cpu->regs.a.l);
}

void op_sta_idpy_w(cpu_t* cpu) {
  cpu->dp = op_readpc(cpu);
  cpu->aa.l = op_readdp(cpu, cpu->dp + 0);
  cpu->aa.h = op_readdp(cpu, cpu->dp + 1);
  op_writedbr(cpu, cpu->aa.w + cpu->regs.y.w + 0, cpu->regs.a.l);
  op_writedbr(cpu, cpu->aa.w + cpu->regs.y.w + 1, cpu->regs.a.h);
}

void op_sta_ildpy_b(cpu_t* cpu) {
  cpu->dp = op_readpc(cpu);
  cpu->aa.l = op_readdp(cpu, cpu->dp + 0);
  cpu->aa.h = op_readdp(cpu, cpu->dp + 1);
  cpu->aa.b = op_readdp(cpu, cpu->dp + 2);
  op_writelong(cpu, cpu->aa.d + cpu->regs.y.w, cpu->regs.a.l);
}

void op_sta_ildpy_w(cpu_t* cpu) {
  cpu->dp = op_readpc(cpu);
  cpu->aa.l = op_readdp(cpu, cpu->dp + 0);
  cpu->aa.h = op_readdp(cpu, cpu->dp + 1);
  cpu->aa.b = op_readdp(cpu, cpu->dp + 2);
  op_writelong(cpu, cpu->aa.d + cpu->regs.y.w + 0, cpu->regs.a.l);
  op_writelong(cpu, cpu->aa.d + cpu->regs.y.w + 1, cpu->regs.a.h);
}

void op_sta_sr_b(cpu_t* cpu) {
  cpu->sp = op_readpc(cpu);
  op_writesp(cpu, cpu->sp, cpu->regs.a.l);
}

void op_sta_sr_w(cpu_t* cpu) {
  cpu->sp = op_readpc(cpu);
  op_writesp(cpu, cpu->sp + 0, cpu->regs.a.l);
  op_writesp(cpu, cpu->sp + 1, cpu->regs.a.h);
}

void op_sta_isry_b(cpu_t* cpu) {
  cpu->sp = op_readpc(cpu);
  cpu->aa.l = op_readsp(cpu, cpu->sp + 0);
  cpu->aa.h = op_readsp(cpu, cpu->sp + 1);
  op_writedbr(cpu, cpu->aa.w + cpu->regs.y.w, cpu->regs.a.l);
}

void op_sta_isry_w(cpu_t* cpu) {
  cpu->sp = op_readpc(cpu);
  cpu->aa.l = op_readsp(cpu, cpu->sp + 0);
  cpu->aa.h = op_readsp(cpu, cpu->sp + 1);
  op_writedbr(cpu, cpu->aa.w + cpu->regs.y.w + 0, cpu->regs.a.l);
  op_writedbr(cpu, cpu->aa.w + cpu->regs.y.w + 1, cpu->regs.a.h);
}

#undef A
#undef X
#undef Y
#undef Z
#undef S
#undef D
