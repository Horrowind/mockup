extern "C" {
#include "r65816/cpu.h"
}

//#include "cpu.hpp"


int main() {

    r65816_cpu_t cpu;
    r65816_cpu_load(&cpu, "smw.sfc");
    return 0; 
}
