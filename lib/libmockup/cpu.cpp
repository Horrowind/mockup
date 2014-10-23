#include "cpu.h"

#ifdef __cplusplus
extern "C" {
#endif

void cpu_step(cpu_t* cpu) {
    cpu->cpu_wrapper->step();
}

void cpu_init(cpu_t* cpu, rom_t* rom) {
    cpu->rom = rom;
    cpu->ram = new uint8_t[0x20000];
    cpu->sreg = new uint8_t[0x2500];
    cpu_clear_ram();
}

void cpu_clear_ram() {

void cpu_show_state(cpu_t* cpu, char[256] ouput) {

}

#ifdef __cplusplus
} //extern "C"
#endif
