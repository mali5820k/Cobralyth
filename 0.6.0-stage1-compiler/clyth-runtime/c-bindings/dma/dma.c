#include "dma.h"

uint8_t clyth_dma_read8(uintptr_t address) {
    return *(volatile uint8_t*)address;
}

uint16_t clyth_dma_read16(uintptr_t address) {
    return *(volatile uint16_t*)address;
}

uint32_t clyth_dma_read32(uintptr_t address) {
    return *(volatile uint32_t*)address;
}

uint64_t clyth_dma_read64(uintptr_t address) {
    return *(volatile uint64_t*)address;
}

void clyth_dma_write8(uintptr_t address, uint8_t value) {
    *(volatile uint8_t*)address = value;
}

void clyth_dma_write16(uintptr_t address, uint16_t value) {
    *(volatile uint16_t*)address = value;
}

void clyth_dma_write32(uintptr_t address, uint32_t value) {
    *(volatile uint32_t*)address = value;
}

void clyth_dma_write64(uintptr_t address, uint64_t value) {
    *(volatile uint64_t*)address = value;
}
