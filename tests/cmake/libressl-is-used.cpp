#if __has_include(<tls.h>)
   #include <tls.h>
#endif

#include "jwt-cpp/jwt.h"

int main() {
   tls_init();
   jwt::date date;
   return 0;
}
