#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

class SPIClass {
public:
  static bool called_begin;
  static void begin() { called_begin = true; }
};

extern SPIClass SPI;

#endif
