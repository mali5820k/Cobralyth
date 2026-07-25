#ifndef CLYTH_RUNTIME_DMA_H
#define CLYTH_RUNTIME_DMA_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t  clyth_dma_read8(uintptr_t address);
uint16_t clyth_dma_read16(uintptr_t address);
uint32_t clyth_dma_read32(uintptr_t address);
uint64_t clyth_dma_read64(uintptr_t address);

void clyth_dma_write8(uintptr_t address, uint8_t value);
void clyth_dma_write16(uintptr_t address, uint16_t value);
void clyth_dma_write32(uintptr_t address, uint32_t value);
void clyth_dma_write64(uintptr_t address, uint64_t value);

#ifdef __cplusplus
}
#endif

#endif
