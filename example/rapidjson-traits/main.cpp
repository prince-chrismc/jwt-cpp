#define RAPIDJSON_HAS_STDSTRING 1

#include <jwt-cpp/jwt.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>

#include <iostream>

using sec = std::chrono::seconds;
using min = std::chrono::minutes;

std::string make_rapid_token() {
  struct rapid_traits {
    struct value : rapidjson::Document{
      value() = default;
      value(std::string str):rapidjson::Document(rapidjson::kStringType){
        this->SetString(str.data(), this->GetAllocator());
      }
      value(int64_t i):rapidjson::Document(rapidjson::kNumberType){
        this->SetInt(i);
      }
      value(const value& other) {
        this->CopyFrom(other, this->GetAllocator());
      };
      value(value&& other) : rapidjson::Document(std::move(other)){}

      value& operator=(const value& other) {
        this->CopyFrom(other, this->GetAllocator());
        return *this;
      };

      size_t count(std::string str){
        return std::count_if(this->Begin(), this->End(),
         [str](value* val){ return str == val->GetString(); });
      }
    };

    struct object_type : value{
       using value::value;
       using mapped_type = value;
       using key_type = std::string;
    };
    struct array_type : value{
       using value::value;
       using value_type = value;
    };
		using string_type = std::string;
		using number_type = double;
		using integer_type = int64_t;
		using boolean_type = bool;

    using value_type = value;

    static jwt::json::type get_type(const value_type &val) {
      using jwt::json::type;

      if (val.IsBool())
        return type::boolean;
      else if (val.IsInt())
        return type::integer;
      else if (val.IsDouble())
        return type::number;
      else if (val.IsString())
        return type::string;
      else if (val.IsArray())
        return type::array;
      else if (val.IsObject())
        return type::object;
      else
        throw std::logic_error("invalid type");
    }

    static object_type as_object(const value_type &val) {
      if (!val.IsObject())
        throw std::bad_cast();
      object_type obj;
      obj.CopyFrom(val, obj.GetAllocator());
      return obj;
    }

    static array_type as_array(const value_type &val) {
      if (!val.IsArray())
        throw std::bad_cast();
      array_type arr;
      arr.CopyFrom(val, arr.GetAllocator());
      return arr;
    }

    static string_type as_string(const value_type &val) {
      if (!val.IsString())
        throw std::bad_cast();
      return val.GetString();
    }

    static number_type as_number(const value_type &val) {
      if (!val.IsDouble())
        throw std::bad_cast();
      return val.GetDouble();
    }

    static integer_type as_int(const value_type &val) {
      if (!val.IsInt())
        throw std::bad_cast();
      return val.GetInt64();
    }

    static bool as_bool(const value_type &val) {
      if (!val.IsBool())
        throw std::bad_cast();
      return val.GetBool();
    }

    static bool parse(value_type &val, std::string str) {
      val.Parse(str);
      return val.HasParseError();
    }

    static value_type parse(const std::string& content)
   {
      value_type json;
      json.Parse(content.c_str());
      return json;
   }

    static std::string serialize(const value_type &val) { 
            rapidjson::StringBuffer buffer;

      buffer.Clear();

      rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

      val.Accept(writer);

      return buffer.GetString(); }
  };

  const auto time = jwt::date::clock::now();

  return jwt::create<rapid_traits>()
      .set_type("JWT")
      .set_issuer("auth.mydomain.io")
      .set_audience("mydomain.io")
      .set_issued_at(time)
      .set_not_before(time + sec{15})
      .set_expires_at(time + sec{15} + min{2})
      .set_payload_claim("boolean", rapid_traits::parse("true"))
      .set_payload_claim("integer", rapid_traits::parse("12345"))
      .set_payload_claim("precision", rapid_traits::parse("12.345"))
      // .set_payload_claim("strings", list)
      // .set_payload_claim("array", {big_numbers.begin(), big_numbers.end()})
      .set_payload_claim("object", rapid_traits::parse(R"##({"api":{"array":[1,2,3],"null":null}})##"))
      .sign(jwt::algorithm::none{});
}

int main(int argc, const char **argv) {
  const auto token = make_rapid_token();
  auto decoded = jwt::decode(token);

  for (auto &e : decoded.get_payload_claims())
    std::cout << e.first << " = " << e.second << std::endl;

  // const auto api_array =
  //     decoded.get_payload_claims()["object"].to_json().get("api").get("array");
  // std::cout << "api array = " << api_array << std::endl;
}
