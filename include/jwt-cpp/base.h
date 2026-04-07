#ifndef JWT_CPP_BASE_H
#define JWT_CPP_BASE_H

#include <algorithm>
#include <array>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

#ifdef __has_cpp_attribute
#if __has_cpp_attribute(fallthrough)
#define JWT_FALLTHROUGH [[fallthrough]]
#endif
#endif

#ifndef JWT_FALLTHROUGH
#define JWT_FALLTHROUGH
#endif

namespace jwt {
	namespace details {
		template<typename string_type, typename = void>
		struct is_narrow_string : std::false_type {};

		template<typename string_type>
		struct is_narrow_string<
			string_type, typename std::enable_if<std::is_same<typename string_type::value_type, char>::value>::type>
			: std::true_type {};

		template<typename string_type, typename = void>
		struct has_size_string : std::false_type {};

		template<typename string_type>
		struct has_size_string<
			string_type,
			typename std::enable_if<std::is_integral<decltype(std::declval<const string_type>().size())>::value>::type>
			: std::true_type {};
	} // namespace details

	/**
	 * \brief character maps when encoding and decoding
	 */
	namespace alphabet {
		/**
		 * \brief valid list of character when working with [Base64](https://datatracker.ietf.org/doc/html/rfc4648#section-4)
		 *
		 * As directed in [X.509 Parameter](https://datatracker.ietf.org/doc/html/rfc7517#section-4.7) certificate chains are
		 * base64-encoded as per [Section 4 of RFC4648](https://datatracker.ietf.org/doc/html/rfc4648#section-4)
		 */
		struct base64 {
			static const std::array<char, 64>& data() {
				static constexpr std::array<char, 64> data{
					{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
					 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
					 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
					 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'}};
				return data;
			}
			static const std::array<int8_t, 256>& rdata() {
				static constexpr std::array<int8_t, 256> rdata{{
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
					52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0,	1,	2,	3,	4,	5,	6,
					7,	8,	9,	10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
					-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
					49, 50, 51, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				}};
				return rdata;
			}
			static constexpr const char* fill() { return "="; }
		};
		/**
		 * \brief valid list of character when working with [Base64URL](https://tools.ietf.org/html/rfc4648#section-5)
		 *
		 * As directed by [RFC 7519 Terminology](https://datatracker.ietf.org/doc/html/rfc7519#section-2) set the definition of Base64URL
		 * encoding as that in [RFC 7515](https://datatracker.ietf.org/doc/html/rfc7515#section-2) that states:
		 *
		 * > Base64 encoding using the URL- and filename-safe character set defined in
		 * > [Section 5 of RFC 4648 RFC4648](https://tools.ietf.org/html/rfc4648#section-5), with all trailing '=' characters omitted
		 */
		struct base64url {
			static const std::array<char, 64>& data() {
				static constexpr std::array<char, 64> data{
					{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
					 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
					 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
					 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_'}};
				return data;
			}
			static const std::array<int8_t, 256>& rdata() {
				static constexpr std::array<int8_t, 256> rdata{{
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1,
					52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0,	1,	2,	3,	4,	5,	6,
					7,	8,	9,	10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, 63,
					-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
					49, 50, 51, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				}};
				return rdata;
			}
			static constexpr const char* fill() { return "%3d"; }
		};
		namespace helper {
			/**
			 * \brief A General purpose base64url alphabet respecting the
			 * [URI Case Normalization](https://datatracker.ietf.org/doc/html/rfc3986#section-6.2.2.1)
			 *
			 * This is useful in situations outside of JWT encoding/decoding and is provided as a helper
			 */
			struct base64url_percent_encoding {
				static const std::array<char, 64>& data() {
					static constexpr std::array<char, 64> data{
						{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
						 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
						 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
						 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_'}};
					return data;
				}
				static const std::array<int8_t, 256>& rdata() {
					static constexpr std::array<int8_t, 256> rdata{{
						-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
						-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1,
						52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0,	1,	2,	3,	4,	5,	6,
						7,	8,	9,	10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, 63,
						-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
						49, 50, 51, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
						-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
						-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
						-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
						-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
						-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					}};
					return rdata;
				}
				static const std::vector<const char*>& fill() {
					static const std::vector<const char*> fill_values{"%3D", "%3d"};
					return fill_values;
				}
			};
		} // namespace helper

		inline uint32_t index(const std::array<int8_t, 256>& rdata, char symbol) {
			auto index = rdata[static_cast<unsigned char>(symbol)];
			if (index <= -1) { throw std::runtime_error("Invalid input: not within alphabet"); }
			return static_cast<uint32_t>(index);
		}
	} // namespace alphabet

	/**
	 * \brief A collection of fellable functions for working with base64 and base64url
	 */
	namespace base {
		namespace details {
			struct fill {
				const char* const value;
				size_t length;

				size_t size() const { return length; }
			};

			struct padding {
				size_t count = 0;
				size_t length = 0;

				padding() = default;
				padding(size_t count, size_t length) : count(count), length(length) {}

				padding operator+(const padding& p) { return padding(count + p.count, length + p.length); }

				friend bool operator==(const padding& lhs, const padding& rhs) {
					return lhs.count == rhs.count && lhs.length == rhs.length;
				}
			};

			template<typename string_type,
					 typename = typename std::enable_if<jwt::details::has_size_string<string_type>::value>::type>
			padding count_padding(const string_type& base, const std::vector<const char*>& fills) {
				for (const auto& filler : fills) {
					fill fill_struct{filler, std::strlen(filler)};

					if (base.size() < fill_struct.size()) continue;
					// Does the end of the input exactly match the fill pattern?
					const auto fill_value = string_type{fill_struct.value};
					if (base.substr(base.size() - fill_struct.size()) == fill_value) {
						return padding{1, fill_struct.size()} +
							   count_padding(base.substr(0, base.size() - fill_struct.size()), fills);
					}
				}

				return {};
			}

			template<typename string_type,
					 typename = typename std::enable_if<jwt::details::has_size_string<string_type>::value>::type>
			string_type encode(const string_type& bin, const std::array<char, 64>& alphabet, const char* fill) {
				size_t size = bin.size();
				string_type res;

				// clear incomplete bytes
				size_t fast_size = size - size % 3;
				for (size_t i = 0; i < fast_size;) {
					uint32_t octet_a = static_cast<unsigned char>(bin[i++]);
					uint32_t octet_b = static_cast<unsigned char>(bin[i++]);
					uint32_t octet_c = static_cast<unsigned char>(bin[i++]);

					uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

					res += alphabet[(triple >> 3 * 6) & 0x3F];
					res += alphabet[(triple >> 2 * 6) & 0x3F];
					res += alphabet[(triple >> 1 * 6) & 0x3F];
					res += alphabet[(triple >> 0 * 6) & 0x3F];
				}

				if (fast_size == size) return res;

				size_t mod = size % 3;

				uint32_t octet_a = fast_size < size ? static_cast<unsigned char>(bin[fast_size++]) : 0;
				uint32_t octet_b = fast_size < size ? static_cast<unsigned char>(bin[fast_size++]) : 0;
				uint32_t octet_c = fast_size < size ? static_cast<unsigned char>(bin[fast_size++]) : 0;

				uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

				switch (mod) {
				case 1:
					res += alphabet[(triple >> 3 * 6) & 0x3F];
					res += alphabet[(triple >> 2 * 6) & 0x3F];
					res += fill;
					res += fill;
					break;
				case 2:
					res += alphabet[(triple >> 3 * 6) & 0x3F];
					res += alphabet[(triple >> 2 * 6) & 0x3F];
					res += alphabet[(triple >> 1 * 6) & 0x3F];
					res += fill;
					break;
				default: break;
				}

				return res;
			}

			template<typename string_type>
			string_type decode(const string_type& base, const std::array<int8_t, 256>& rdata,
							   const std::vector<const char*>& fill_) {
				const auto pad = count_padding(base, fill_);
				if (pad.count > 2) throw std::runtime_error("Invalid input: too much fill");

				const size_t size = base.size() - pad.length;
				if ((size + pad.count) % 4 != 0) throw std::runtime_error("Invalid input: incorrect total size");

				size_t out_size = size / 4 * 3;
				string_type res;
				res.reserve(out_size);

				auto get_sextet = [&](size_t offset) { return alphabet::index(rdata, base[offset]); };

				size_t fast_size = size - size % 4;
				for (size_t i = 0; i < fast_size;) {
					uint32_t sextet_a = get_sextet(i++);
					uint32_t sextet_b = get_sextet(i++);
					uint32_t sextet_c = get_sextet(i++);
					uint32_t sextet_d = get_sextet(i++);

					uint32_t triple =
						(sextet_a << 3 * 6) + (sextet_b << 2 * 6) + (sextet_c << 1 * 6) + (sextet_d << 0 * 6);

					res += static_cast<char>((triple >> 2 * 8) & 0xFFU);
					res += static_cast<char>((triple >> 1 * 8) & 0xFFU);
					res += static_cast<char>((triple >> 0 * 8) & 0xFFU);
				}

				if (pad.count == 0) return res;

				uint32_t triple = (get_sextet(fast_size) << 3 * 6) + (get_sextet(fast_size + 1) << 2 * 6);

				switch (pad.count) {
				case 1:
					triple |= (get_sextet(fast_size + 2) << 1 * 6);
					res += static_cast<char>((triple >> 2 * 8) & 0xFFU);
					res += static_cast<char>((triple >> 1 * 8) & 0xFFU);
					break;
				case 2: res += static_cast<char>((triple >> 2 * 8) & 0xFFU); break;
				default: break;
				}

				return res;
			}

			template<typename string_type>
			string_type decode(const string_type& base, const std::array<int8_t, 256>& rdata, const char* fill) {
				return decode<string_type>(base, rdata, std::vector<const char*>{fill});
			}

			template<typename string_type>
			string_type pad(const string_type& base, const char* fill) {
				string_type padding;
				switch (base.size() % 4) {
				case 1: padding += fill; JWT_FALLTHROUGH;
				case 2: padding += fill; JWT_FALLTHROUGH;
				case 3: padding += fill; JWT_FALLTHROUGH;
				default: break;
				}

				return string_type(base + padding);
			}

			template<typename string_type>
			string_type trim(const string_type& base, const char* fill) {
				auto pos = base.find(fill);
				return base.substr(0, pos);
			}
		} // namespace details

		/**
		 * \brief Generic base64 encoding
		 * 
		 * A Generic base64 encode function that supports any "alphabet"
		 * such as jwt::alphabet::base64 
		 * 
		 * \code
		 * const auto b64 = jwt::base::encode<jwt::alphabet::base64>("example_data")
		 * \endcode
		 */
		template<typename alphabet, typename string_type = std::string>
		typename std::enable_if<jwt::details::is_narrow_string<string_type>::value, string_type>::type
		encode(const string_type& bin) {
			return details::encode<string_type>(bin, alphabet::data(), alphabet::fill());
		}
		/**
		 * \brief Generic base64 decoding
		 * 
		 * A Generic base64 decoding function that supports any "alphabet"
		 * such as jwt::alphabet::base64 
		 * 
		 * \code
		 * const auto b64 = jwt::base::decode<jwt::alphabet::base64>("ZXhhbXBsZV9kYXRh")
		 * \endcode
		 */
		template<typename alphabet, typename string_type = std::string>
		typename std::enable_if<jwt::details::is_narrow_string<string_type>::value, string_type>::type
		decode(const string_type& base) {
			return details::decode<string_type>(base, alphabet::rdata(), alphabet::fill());
		}
		/**
		 * \brief Generic base64 padding
		 * 
		 * A Generic base64 pad function that supports any "alphabet"
		 * such as jwt::alphabet::base64 
		 * 
		 * \code
		 * const auto b64 = jwt::base::pad<jwt::alphabet::base64>("ZXhhbXBsZV9kYQ")
		 * \endcode
		 */
		template<typename alphabet, typename string_type = std::string>
		typename std::enable_if<jwt::details::is_narrow_string<string_type>::value, string_type>::type
		pad(const string_type& base) {
			return details::pad<string_type>(base, alphabet::fill());
		}
		/**
		 * \brief Generic base64 trimming
		 * 
		 * A Generic base64 trim function that supports any "alphabet"
		 * such as jwt::alphabet::base64 
		 * 
		 * \code
		 * const auto b64 = jwt::base::trim<jwt::alphabet::base64>("ZXhhbXBsZV9kYQ==")
		 * \endcode
		 */
		template<typename alphabet, typename string_type = std::string>
		typename std::enable_if<jwt::details::is_narrow_string<string_type>::value, string_type>::type
		trim(const string_type& base) {
			return details::trim<string_type>(base, alphabet::fill());
		}
	} // namespace base
} // namespace jwt

#endif
