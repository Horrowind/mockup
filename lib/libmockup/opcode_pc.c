template<int bit, int val> void R65816::op_branch() {
  if((bool)(regs.p & bit) != val) {
    rd.l = op_readpc();
  } else {
    rd.l = op_readpc();
    regs.pc.w = regs.pc.d + (int8)rd.l;
  }
}

void R65816::op_bra() {
  rd.l = op_readpc();
  aa.w = regs.pc.d + (int8)rd.l;
  regs.pc.w = aa.w;
}

void R65816::op_brl() {
  rd.l = op_readpc();
  rd.h = op_readpc();
  regs.pc.w = regs.pc.d + (int16)rd.w;
}

void R65816::op_jmp_addr() {
  rd.l = op_readpc();
  rd.h = op_readpc();
  regs.pc.w = rd.w;
}

void R65816::op_jmp_long() {
  rd.l = op_readpc();
  rd.h = op_readpc();
  rd.b = op_readpc();
  regs.pc.d = rd.d & 0xffffff;
}

void R65816::op_jmp_iaddr() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  rd.l = op_readaddr(aa.w + 0);
  rd.h = op_readaddr(aa.w + 1);
  regs.pc.w = rd.w;
}

void R65816::op_jmp_iaddrx() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  rd.l = op_readpbr(aa.w + regs.x.w + 0);
  rd.h = op_readpbr(aa.w + regs.x.w + 1);
  regs.pc.w = rd.w;
}

void R65816::op_jmp_iladdr() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  rd.l = op_readaddr(aa.w + 0);
  rd.h = op_readaddr(aa.w + 1);
  rd.b = op_readaddr(aa.w + 2);
  regs.pc.d = rd.d & 0xffffff;
}

void R65816::op_jsr_addr() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  regs.pc.w--;
  op_writestack(regs.pc.h);
  op_writestack(regs.pc.l);
  regs.pc.w = aa.w;
}

void R65816::op_jsr_long_e() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_writestackn(regs.pc.b);
  aa.b = op_readpc();
  regs.pc.w--;
  op_writestackn(regs.pc.h);
  op_writestackn(regs.pc.l);
  regs.pc.d = aa.d & 0xffffff;
  regs.s.h = 0x01;
}

void R65816::op_jsr_long_n() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_writestackn(regs.pc.b);
  aa.b = op_readpc();
  regs.pc.w--;
  op_writestackn(regs.pc.h);
  op_writestackn(regs.pc.l);
  regs.pc.d = aa.d & 0xffffff;
}

void R65816::op_jsr_iaddrx_e() {
  aa.l = op_readpc();
  op_writestackn(regs.pc.h);
  op_writestackn(regs.pc.l);
  aa.h = op_readpc();
  rd.l = op_readpbr(aa.w + regs.x.w + 0);
  rd.h = op_readpbr(aa.w + regs.x.w + 1);
  regs.pc.w = rd.w;
  regs.s.h = 0x01;
}

void R65816::op_jsr_iaddrx_n() {
  aa.l = op_readpc();
  op_writestackn(regs.pc.h);
  op_writestackn(regs.pc.l);
  aa.h = op_readpc();
  rd.l = op_readpbr(aa.w + regs.x.w + 0);
  rd.h = op_readpbr(aa.w + regs.x.w + 1);
  regs.pc.w = rd.w;
}

void R65816::op_rti_e() {
  regs.p = op_readstack() | 0x30;
  rd.l = op_readstack();
  rd.h = op_readstack();
  regs.pc.w = rd.w;
}

void R65816::op_rti_n() {
  regs.p = op_readstack();
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
  rd.l = op_readstack();
  rd.h = op_readstack();
  rd.b = op_readstack();
  regs.pc.d = rd.d & 0xffffff;
  update_table();
}

void R65816::op_rts() {
  rd.l = op_readstack();
  rd.h = op_readstack();
  regs.pc.w = ++rd.w;
}

void R65816::op_rtl_e() {
  rd.l = op_readstackn();
  rd.h = op_readstackn();
  rd.b = op_readstackn();
  regs.pc.b = rd.b;
  regs.pc.w = ++rd.w;
  regs.s.h = 0x01;
}

void R65816::op_rtl_n() {
  rd.l = op_readstackn();
  rd.h = op_readstackn();
  rd.b = op_readstackn();
  regs.pc.b = rd.b;
  regs.pc.w = ++rd.w;
}
