#if !__has_include(<wolfssl/ssl.h>)
#error "missing wolfSSL's SSL header!"
#endif

// See https://github.com/Thalhammer/jwt-cpp/pull/352
#ifndef EXTERNAL_OPTS_OPENVPN
#error "missing wolfSSL's OPENSSL_EXTRA macro!"
#endif

// In order to support https://github.com/Thalhammer/jwt-cpp/issues/308
// This takes v5.6.0 as it improve the BIGNUM compatibility with OpenSSL.
// See https://github.com/wolfSSL/wolfssl/pull/6165
#if defined(LIBWOLFSSL_VERSION_HEX) && LIBWOLFSSL_VERSION_HEX < 0x05006000
#error "min version of wolfSSL is 5.6.0 or higher!"
#endif

#include "jwt-cpp/jwt.h"

#include <wolfssl/ssl.h>

int main() {
	wolfSSL_library_init();
	jwt::date date;
	return 0;
}
