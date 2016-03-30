#ifndef EEPROM_HELPERS_H
#define EEPROM_HELPERS_H

#ifdef __cplusplus
extern "C" {
#endif
  uint8_t eeprom_read(int addr);
  void eeprom_update(int addr, uint8_t val);
#ifdef __cplusplus
}
#endif

#endif
