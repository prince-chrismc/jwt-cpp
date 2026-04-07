#ifndef JWT_CPP_BOOSTJSON_TRAITS_H
#define JWT_CPP_BOOSTJSON_TRAITS_H

#define JWT_DISABLE_PICOJSON
#include "jwt-cpp/jwt.h"

#include <boost/json.hpp>
// if not boost JSON standalone then error...

namespace jwt {
	/**
	 * \brief Namespace containing all the json_trait implementations for a jwt::basic_claim.
	*/
	namespace traits {
		namespace json = boost::json;
		/// basic_claim's JSON trait implementation for Boost.JSON
		struct boost_json {
			using value_type = json::value;
			using object_type = json::object;
			using array_type = json::array;
			using number_type = double;
			using integer_type = std::int64_t;
			using boolean_type = bool;

			class string_type : public json::string {
			public:
				// Inherit constructors (crucial for generic code)
				using json::string::string;
				
				// Copy/Move from the base class
				string_type(json::string&& other) : json::string(std::move(other)) {}
				string_type(const json::string& other) : json::string(other) {}

				// 3. ADDED: Construct from std::string or std::string_view
				// This allows: json_string_wrapper my_str = std_str;
				string_type(std::string_view s, json::storage_ptr sp = {}) 
					: json::string(s.data(), s.size(), std::move(sp)) {}

                // Map 'append' to Boost's 'insert'
                string_type& append(std::string_view s) {
                    this->insert(this->size(), s);
                    return *this;
                }

				// Overload for specific ranges if needed
				string_type& append(const char* s, size_t n) {
					this->insert(this->size(), {s, n});
					return *this;
				}

				// Now operator+= can safely use append or just the base +=
				string_type& operator+=(std::string_view s) {
					return this->append(s);
				}

				// Add the missing substr implementation
				// Returns a new string (matches std::string behavior)
				string_type substr(size_t pos = 0, size_t count = npos) const {
					auto view = this->subview(pos, count);
					return string_type(view.data(), view.size(), this->get_allocator());
				}

				string_type& operator=(std::string_view s) {
					this->assign(s.data(), s.size());
					return *this;
				}

				// Explicitly handle std::string if needed (though string_view covers it)
				string_type& operator=(const std::string& s) {
					this->assign(s.data(), s.size());
					return *this;
				}

				// Keep your existing boost::json assignment
				string_type& operator=(const boost::json::string& other) {
					boost::json::string::operator=(other);
					return *this;
				}

				// Comparison with const char* (C-strings)
				bool operator==(const char* s) const noexcept {
					return static_cast<const json::string&>(*this) == s;
				}

				// Comparison with other json::strings or wrappers
				bool operator==(const json::string& other) const noexcept {
					return static_cast<const json::string&>(*this) == other;
				}
			};

            // --- Operators MUST be inside namespace jwt::traits ---
            // These allow the trait checks in jwt.h to pass via ADL
            friend string_type operator+(string_type lhs, std::string_view rhs) {
                lhs.append(rhs);
                return lhs;
            }

            friend string_type operator+(std::string_view lhs, const string_type& rhs) {
                string_type res(lhs, rhs.get_allocator());
                res.append(rhs);
                return res;
            }

            friend string_type operator+(string_type lhs, const string_type& rhs) {
                lhs.append(rhs);
                return lhs;
            }

			static jwt::json::type get_type(const value_type& val) {
				using jwt::json::type;

				if (val.kind() == json::kind::bool_) return type::boolean;
				if (val.kind() == json::kind::int64) return type::integer;
				if (val.kind() == json::kind::uint64) // boost internally tracks two types of integers
					return type::integer;
				if (val.kind() == json::kind::double_) return type::number;
				if (val.kind() == json::kind::string) return type::string;
				if (val.kind() == json::kind::array) return type::array;
				if (val.kind() == json::kind::object) return type::object;

				throw std::logic_error("invalid type");
			}

			static object_type as_object(const value_type& val) {
				if (val.kind() != json::kind::object) throw std::bad_cast();
				return val.get_object();
			}

			static array_type as_array(const value_type& val) {
				if (val.kind() != json::kind::array) throw std::bad_cast();
				return val.get_array();
			}

			static string_type as_string(const value_type& val) {
				if (val.kind() != json::kind::string) throw std::bad_cast();
				return string_type{val.get_string()};
			}

			static integer_type as_integer(const value_type& val) {
				switch (val.kind()) {
				case json::kind::int64: return val.get_int64();
				case json::kind::uint64: return static_cast<int64_t>(val.get_uint64());
				default: throw std::bad_cast();
				}
			}

			static boolean_type as_boolean(const value_type& val) {
				if (val.kind() != json::kind::bool_) throw std::bad_cast();
				return val.get_bool();
			}

			static number_type as_number(const value_type& val) {
				if (val.kind() != json::kind::double_) throw std::bad_cast();
				return val.get_double();
			}

			static bool parse(value_type& val, string_type str) {
				val = json::parse(str);
				return true;
			}

			static std::string serialize(const value_type& val) { return json::serialize(val); }
		};

		

	} // namespace traits
} // namespace jwt

namespace std {
    inline jwt::traits::boost_json::string_type operator+(
        jwt::traits::boost_json::string_type lhs, 
        const jwt::traits::boost_json::string_type& rhs) 
    {
        lhs.append(rhs);
        return lhs;
    }

    inline jwt::traits::boost_json::string_type operator+(
        jwt::traits::boost_json::string_type lhs, 
        std::string_view rhs) 
    {
        lhs.append(rhs);
        return lhs;
    }
}

#endif // JWT_CPP_BOOSTJSON_TRAITS_H
