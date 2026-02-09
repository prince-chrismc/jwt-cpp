/// @file hs512.cpp
#include <iostream>
#include <jwt-cpp/jwt.h>
#include <openssl/bn.h>
#include <openssl/rand.h>

BIGNUM* make_bn();

int main() {
	BIGNUM* cipher_raw = make_bn();
	std::unique_ptr<BIGNUM, decltype(&BN_free)> cipher(cipher_raw, BN_free);

	/* [use HMAC algo with BIGNUM] */
	auto token = jwt::create()
					 .set_issuer("auth0")
					 .set_type("JWT")
					 .set_id("hs512-create-example")
					 .set_issued_now()
					 .set_expires_in(std::chrono::seconds{36000})
					 .set_payload_claim("sample", jwt::claim(std::string{"test"}))
					 .sign(jwt::algorithm::hs512(cipher.get()));
	/* [use HMAC algo with BIGNUM] */

	std::cout << "token:\n" << token << std::endl;

	auto verify = jwt::verify().allow_algorithm(jwt::algorithm::hs512(cipher.get())).with_issuer("auth0");

	auto decoded = jwt::decode(token);

	verify.verify(decoded);

	for (auto& e : decoded.get_header_json())
		std::cout << e.first << " = " << e.second << std::endl;
	for (auto& e : decoded.get_payload_json())
		std::cout << e.first << " = " << e.second << std::endl;
}

BIGNUM* make_bn() {
	// https://stackoverflow.com/a/70790564
	unsigned char n1[64];
	unsigned char n2[64];

	RAND_bytes(n1, sizeof n1);
	RAND_bytes(n2, sizeof n2);

	BIGNUM* bn1 = BN_bin2bn(n1, sizeof n1, NULL);
	BIGNUM* bn2 = BN_bin2bn(n2, sizeof n2, NULL);
	BIGNUM* bn3 = BN_new();

	// create context
	BN_CTX* ctx = BN_CTX_new();
	BN_mul(bn3, bn1, bn2, ctx);
	BN_CTX_free(ctx);

	BN_free(bn1);
	BN_free(bn2);

	return bn3;
}
