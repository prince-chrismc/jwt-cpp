#include "jwt-cpp/base.h"
#include <gtest/gtest.h>

TEST(BaseTest, Base64Index) {
	ASSERT_EQ(0, jwt::alphabet::index(jwt::alphabet::base64::rdata(), 'A'));
	ASSERT_EQ(32, jwt::alphabet::index(jwt::alphabet::base64::rdata(), 'g'));
	ASSERT_EQ(62, jwt::alphabet::index(jwt::alphabet::base64::rdata(), '+'));

	std::size_t index = 0;
	for (auto c : jwt::alphabet::base64::data()) {
		ASSERT_EQ(index, jwt::alphabet::index(jwt::alphabet::base64::rdata(), c));
		index++;
	}

	std::size_t noBaseCharCount = 0;
	for (std::size_t i = 0; i < jwt::alphabet::base64::rdata().size(); i++) {
		if (jwt::alphabet::base64::rdata().at(i) == -1) { noBaseCharCount++; }
	}
	ASSERT_EQ(jwt::alphabet::base64::rdata().size() - jwt::alphabet::base64::data().size(), noBaseCharCount);
}

TEST(BaseTest, Base64URLIndex) {
	ASSERT_EQ(0, jwt::alphabet::index(jwt::alphabet::base64url::rdata(), 'A'));
	ASSERT_EQ(32, jwt::alphabet::index(jwt::alphabet::base64url::rdata(), 'g'));
	ASSERT_EQ(62, jwt::alphabet::index(jwt::alphabet::base64url::rdata(), '-'));

	std::size_t index = 0;
	for (auto c : jwt::alphabet::base64url::data()) {
		ASSERT_EQ(index, jwt::alphabet::index(jwt::alphabet::base64url::rdata(), c));
		index++;
	}

	std::size_t noBaseCharCount = 0;
	for (std::size_t i = 0; i < jwt::alphabet::base64url::rdata().size(); i++) {
		if (jwt::alphabet::base64url::rdata().at(i) == -1) { noBaseCharCount++; }
	}
	ASSERT_EQ(jwt::alphabet::base64url::rdata().size() - jwt::alphabet::base64url::data().size(), noBaseCharCount);
}

TEST(BaseTest, BaseDetailsCountPadding) {
	using jwt::base::details::padding;
	ASSERT_EQ(padding{}, jwt::base::details::count_padding<std::string>("ABC", {"~"}));
	ASSERT_EQ((padding{3, 3}), jwt::base::details::count_padding<std::string>("ABC~~~", {"~"}));
	ASSERT_EQ((padding{5, 5}), jwt::base::details::count_padding<std::string>("ABC~~~~~", {"~"}));

	ASSERT_EQ(padding{}, jwt::base::details::count_padding<std::string>("ABC", {"~", "!"}));
	ASSERT_EQ((padding{1, 1}), jwt::base::details::count_padding<std::string>("ABC!", {"~", "!"}));
	ASSERT_EQ((padding{1, 1}), jwt::base::details::count_padding<std::string>("ABC~", {"~", "!"}));
	ASSERT_EQ((padding{3, 3}), jwt::base::details::count_padding<std::string>("ABC~~!", {"~", "!"}));
	ASSERT_EQ((padding{3, 3}), jwt::base::details::count_padding<std::string>("ABC!~~", {"~", "!"}));
	ASSERT_EQ((padding{5, 5}), jwt::base::details::count_padding<std::string>("ABC~~!~~", {"~", "!"}));

	ASSERT_EQ((padding{2, 6}), jwt::base::details::count_padding<std::string>("MTIzNA%3d%3d", {"%3d", "%3D"}));
	ASSERT_EQ((padding{2, 6}), jwt::base::details::count_padding<std::string>("MTIzNA%3d%3D", {"%3d", "%3D"}));
	ASSERT_EQ((padding{2, 6}), jwt::base::details::count_padding<std::string>("MTIzNA%3D%3d", {"%3d", "%3D"}));
	ASSERT_EQ((padding{2, 6}), jwt::base::details::count_padding<std::string>("MTIzNA%3D%3D", {"%3d", "%3D"}));

	// Some fake scenarios

	ASSERT_EQ(padding{}, jwt::base::details::count_padding<std::string>("", {"~"}));
	ASSERT_EQ(padding{}, jwt::base::details::count_padding<std::string>("ABC", {"~", "~~!"}));
	ASSERT_EQ(padding{}, jwt::base::details::count_padding<std::string>("ABC!", {"~", "~~!"}));
	ASSERT_EQ((padding{1, 1}), jwt::base::details::count_padding<std::string>("ABC~", {"~", "~~!"}));
	ASSERT_EQ((padding{1, 3}), jwt::base::details::count_padding<std::string>("ABC~~!", {"~", "~~!"}));
	ASSERT_EQ((padding{2, 2}), jwt::base::details::count_padding<std::string>("ABC!~~", {"~", "~~!"}));
	ASSERT_EQ((padding{3, 5}), jwt::base::details::count_padding<std::string>("ABC~~!~~", {"~", "~~!"}));
	ASSERT_EQ(padding{}, jwt::base::details::count_padding<std::string>("ABC~~!~~", {}));
}

TEST(BaseTest, Base64Decode) {
	ASSERT_EQ("1", jwt::base::decode<jwt::alphabet::base64>(std::string{"MQ=="}));
	ASSERT_EQ("12", jwt::base::decode<jwt::alphabet::base64>(std::string{"MTI="}));
	ASSERT_EQ("123", jwt::base::decode<jwt::alphabet::base64>(std::string{"MTIz"}));
	ASSERT_EQ("1234", jwt::base::decode<jwt::alphabet::base64>(std::string{"MTIzNA=="}));
}

TEST(BaseTest, Base64DecodeURL) {
	ASSERT_EQ("1", jwt::base::decode<jwt::alphabet::base64url>(std::string{"MQ%3d%3d"}));
	ASSERT_EQ("12", jwt::base::decode<jwt::alphabet::base64url>(std::string{"MTI%3d"}));
	ASSERT_EQ("123", jwt::base::decode<jwt::alphabet::base64url>(std::string{"MTIz"}));
	ASSERT_EQ("1234", jwt::base::decode<jwt::alphabet::base64url>(std::string{"MTIzNA%3d%3d"}));
}

TEST(BaseTest, Base64DecodeURLCaseInsensitive) {
	ASSERT_EQ("1", jwt::base::decode<jwt::alphabet::helper::base64url_percent_encoding>(std::string{"MQ%3d%3d"}));
	ASSERT_EQ("1", jwt::base::decode<jwt::alphabet::helper::base64url_percent_encoding>(std::string{"MQ%3D%3d"}));
	ASSERT_EQ("1", jwt::base::decode<jwt::alphabet::helper::base64url_percent_encoding>(std::string{"MQ%3d%3D"}));
	ASSERT_EQ("12", jwt::base::decode<jwt::alphabet::helper::base64url_percent_encoding>(std::string{"MTI%3d"}));
	ASSERT_EQ("123", jwt::base::decode<jwt::alphabet::helper::base64url_percent_encoding>(std::string{"MTIz"}));
	ASSERT_EQ("1234", jwt::base::decode<jwt::alphabet::helper::base64url_percent_encoding>(std::string{"MTIzNA%3d%3d"}));
	ASSERT_EQ("1234", jwt::base::decode<jwt::alphabet::helper::base64url_percent_encoding>(std::string{"MTIzNA%3D%3D"}));
}

TEST(BaseTest, Base64Encode) {
	ASSERT_EQ("MQ==", jwt::base::encode<jwt::alphabet::base64>(std::string{"1"}));
	ASSERT_EQ("MTI=", jwt::base::encode<jwt::alphabet::base64>(std::string{"12"}));
	ASSERT_EQ("MTIz", jwt::base::encode<jwt::alphabet::base64>(std::string{"123"}));
	ASSERT_EQ("MTIzNA==", jwt::base::encode<jwt::alphabet::base64>(std::string{"1234"}));
}

TEST(BaseTest, Base64EncodeURL) {
	ASSERT_EQ("MQ%3d%3d", jwt::base::encode<jwt::alphabet::base64url>(std::string{"1"}));
	ASSERT_EQ("MTI%3d", jwt::base::encode<jwt::alphabet::base64url>(std::string{"12"}));
	ASSERT_EQ("MTIz", jwt::base::encode<jwt::alphabet::base64url>(std::string{"123"}));
	ASSERT_EQ("MTIzNA%3d%3d", jwt::base::encode<jwt::alphabet::base64url>(std::string{"1234"}));
}

TEST(BaseTest, Base64Pad) {
	ASSERT_EQ("MQ==", jwt::base::pad<jwt::alphabet::base64>(std::string{"MQ"}));
	ASSERT_EQ("MTI=", jwt::base::pad<jwt::alphabet::base64>(std::string{"MTI"}));
	ASSERT_EQ("MTIz", jwt::base::pad<jwt::alphabet::base64>(std::string{"MTIz"}));
	ASSERT_EQ("MTIzNA==", jwt::base::pad<jwt::alphabet::base64>(std::string{"MTIzNA"}));
}

TEST(BaseTest, Base64PadURL) {
	ASSERT_EQ("MQ%3d%3d", jwt::base::pad<jwt::alphabet::base64url>(std::string{"MQ"}));
	ASSERT_EQ("MTI%3d", jwt::base::pad<jwt::alphabet::base64url>(std::string{"MTI"}));
	ASSERT_EQ("MTIz", jwt::base::pad<jwt::alphabet::base64url>(std::string{"MTIz"}));
	ASSERT_EQ("MTIzNA%3d%3d", jwt::base::pad<jwt::alphabet::base64url>(std::string{"MTIzNA"}));
}

TEST(BaseTest, Base64Trim) {
	ASSERT_EQ("MQ", jwt::base::trim<jwt::alphabet::base64>(std::string{"MQ=="}));
	ASSERT_EQ("MTI", jwt::base::trim<jwt::alphabet::base64>(std::string{"MTI="}));
	ASSERT_EQ("MTIz", jwt::base::trim<jwt::alphabet::base64>(std::string{"MTIz"}));
	ASSERT_EQ("MTIzNA", jwt::base::trim<jwt::alphabet::base64>(std::string{"MTIzNA=="}));
}

TEST(BaseTest, Base64TrimURL) {
	ASSERT_EQ("MQ", jwt::base::trim<jwt::alphabet::base64url>(std::string{"MQ%3d%3d"}));
	ASSERT_EQ("MTI", jwt::base::trim<jwt::alphabet::base64url>(std::string{"MTI%3d"}));
	ASSERT_EQ("MTIz", jwt::base::trim<jwt::alphabet::base64url>(std::string{"MTIz"}));
	ASSERT_EQ("MTIzNA", jwt::base::trim<jwt::alphabet::base64url>(std::string{"MTIzNA%3d%3d"}));
}
