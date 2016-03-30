#include <EEPROM.h>
#include "eeprom_helpers.h"

uint8_t eeprom_read(int addr) {
  return EEPROM.read(addr);
}

void eeprom_update(int addr, uint8_t val) {
  return EEPROM.update(addr, val);
}
