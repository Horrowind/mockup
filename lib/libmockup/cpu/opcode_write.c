#include "cpu/memory.h"
#include "cpu/opcode_write.h"

#define op_write_addr_gen(name, num)					\
    void op_write_addr_##name##_b(cpu_t* cpu) {				\
	cpu->aa.l = op_readpc(cpu);					\
	cpu->aa.h = op_readpc(cpu);					\
	op_writedbr(cpu, cpu->aa.w, cpu->regs.r[n]);			\
    }									\
									\
    void op_write_addr##name##_w(cpu_t* cpu) {				\
	cpu->aa.l = op_readpc(cpu);					\
	cpu->aa.h = op_readpc(cpu);					\
	op_writedbr(cpu, cpu->aa.w + 0, cpu->regs.r[n] >> 0);		\
	op_writedbr(cpu, cpu->aa.w + 1, cpu->regs.r[n] >> 8);		\
    }


#undef op_write_addr_gen

#define op_write_addrr_gen(name, num, i)				\
    void op_write_addrr_##name##_b(cpu_t* cpu) {			\
    cpu->aa.l = op_readpc(cpu);						\
    cpu->aa.h = op_readpc(cpu);						\
    op_writedbr(cpu, cpu->aa.w + cpu->regs.r[i], cpu->regs.r[n]);	\
    }									\
     									\
     void op_write_addrr_##name##_w(cpu_t* cpu) {				\
					cpu->aa.l = op_readpc(cpu);	\
					cpu->aa.h = op_readpc(cpu);	\
					op_writedbr(cpu, cpu->aa.w + cpu->regs.r[i] + 0, cpu->regs.r[n] >> 0); \
					op_writedbr(cpu, cpu->aa.w + cpu->regs.r[i] + 1, cpu->regs.r[n] >> 8); \
					}

#undef op_write_addrr_gen

#define op_write_longr_gen(name, i)					\
    void op_write_longr_##name##_b(cpu_t* cpu) {			\
	cpu->aa.l = op_readpc(cpu);					\
	cpu->aa.h = op_readpc(cpu);					\
	cpu->aa.b = op_readpc(cpu);					\
	op_writelong(cpu, cpu->aa.d + cpu->regs.r[i], cpu->regs.a.l);	\
    }									\
    									\
    void op_write_longr_##name##_w(cpu_t* cpu) {			\
	cpu->aa.l = op_readpc(cpu);					\
	cpu->aa.h = op_readpc(cpu);					\
	cpu->aa.b = op_readpc(cpu);					\
	op_writelong(cpu, cpu->aa.d + cpu->regs.r[i] + 0, cpu->regs.a.l); \
	op_writelong(cpu, cpu->aa.d + cpu->regs.r[i] + 1, cpu->regs.a.h); \
    }

#undef op_write_longr_gen	 


#define op_write_dp_gen(name, num)				\
    void op_write_dp_##name##_b(cpu_t* cpu) {			\
	cpu->dp = op_readpc(cpu);				\
	op_writedp(cpu->dp, cpu->regs.r[n]);			\
    }								\
    								\
    void op_write_dp_##name##_w(cpu_t* cpu) {			\
	cpu->dp = op_readpc(cpu);				\
	op_writedp(cpu->dp + 0, cpu->regs.r[n] >> 0);		\
	op_writedp(cpu->dp + 1, cpu->regs.r[n] >> 8);		\
    }
#undef op_write_dp_gen

#define op_write_dpr_gen(name, num, i)					\
    void op_write_dpr_##name##_b(cpu_t* cpu) {				\
	cpu->dp = op_readpc(cpu);					\
	op_writedp(cpu->dp + cpu->regs.r[i], cpu->regs.r[n]);		\
    }									\
									\
    void op_write_dpr_##name##_w(cpu_t* cpu) {				\
	cpu->dp = op_readpc(cpu);					\
	op_writedp(cpu->dp + cpu->regs.r[i] + 0, cpu->regs.r[n] >> 0);	\
	op_writedp(cpu->dp + cpu->regs.r[i] + 1, cpu->regs.r[n] >> 8);	\
    }

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
