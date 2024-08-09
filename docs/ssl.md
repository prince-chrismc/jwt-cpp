# Cryptography Libraries

The underlying cryptography libraries describe [here](../README.md#ssl-compatibility) can be selected when configuring CMake by explicitly setting `JWT_SSL_LIBRARY` to one of three values. The default is to use OpenSSL.

- OpenSSL
- LibreSSL
- wolfSSL

Here's an example:

```sh
cmake . -DJWT_SSL_LIBRARY:STRING=wolfSSL 
```

## Supported Versions

These are the version which are currently being tested:

| OpenSSL           | LibreSSL       | wolfSSL        |
| ----------------- | -------------- | -------------- |
| ![1.0.2u][o1.0.2] | ![3.6.2][l3.6] | ![5.4.0][w5.4] |
| ![1.1.0i][o1.1.0] | ![3.7.2][l3.7] | ![5.5.4][w5.5] |
| ![1.1.1w][o1.1.1] | ![3.8.2][l3.8] | ![5.6.6][w5.6] |
| ![3.2.0][o3.2]    |                |                |


> [!NOTE]
> A complete list of versions tested in the past can be found [here](https://github.com/Thalhammer/jwt-cpp/tree/badges).

[o1.0.2]: https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/Thalhammer/jwt-cpp/badges/openssl/1.0.2u/shields.json
[o1.1.0]: https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/Thalhammer/jwt-cpp/badges/openssl/1.1.0i/shields.json
[o1.1.1]: https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/Thalhammer/jwt-cpp/badges/openssl/1.1.1w/shields.json
[o3.2]: https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/Thalhammer/jwt-cpp/badges/openssl/3.2.0/shields.json
[l3.6]: https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/Thalhammer/jwt-cpp/badges/libressl/3.6.2/shields.json
[l3.7]: https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/Thalhammer/jwt-cpp/badges/libressl/3.7.2/shields.json
[l3.8]: https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/Thalhammer/jwt-cpp/badges/libressl/3.8.2/shields.json
[w5.4]: https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/Thalhammer/jwt-cpp/badges/wolfssl/5.6.6/shields.json
[w5.5]: https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/Thalhammer/jwt-cpp/badges/wolfssl/5.5.4/shields.json
[w5.6]: https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/Thalhammer/jwt-cpp/badges/wolfssl/5.4.0/shields.json


## Notes

JWT-CPP relies on the OpenSSL API, as a result both LibreSSL and wolfSSL need to include their respective compatibility layers.
Most system already have OpenSSL so it's important to make sure when compiling your application it only includes one. Otherwise you may have missing symbols when linking.
