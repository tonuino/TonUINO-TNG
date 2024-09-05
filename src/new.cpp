#include "constants.hpp"
#if defined(TonUINO_Every) and defined(ARDUINO_AVR_NANO_EVERY)

#include <stdlib.h>

// C++14 introduces additional delete operators
#if __cplusplus >= 201402L

void operator delete(void * ptr, size_t)
{
  ::operator delete(ptr);
}

void operator delete[](void * ptr, size_t)
{
  ::operator delete(ptr);
}

#endif // End language is C++14 or greater

#endif // TonUINO_Every
