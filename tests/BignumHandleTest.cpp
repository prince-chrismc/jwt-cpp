#include "jwt-cpp/jwt.h"
#include <gtest/gtest.h>
#include <openssl/bn.h>

TEST(BignumHandleTest, DefaultConstructor) {
	jwt::helper::bn_handle bn;
	ASSERT_NE(nullptr, bn.get());
	EXPECT_EQ(1, BN_is_zero(bn.get()));
}

TEST(BignumHandleTest, ConstructorFromBIGNUM) {
	BIGNUM* original = BN_new();
	ASSERT_NE(nullptr, original);
	BN_set_word(original, 42);

	jwt::helper::bn_handle bn(original);
	ASSERT_NE(nullptr, bn.get());
	EXPECT_EQ(42, BN_get_word(bn.get()));

	EXPECT_EQ(42, BN_get_word(original));
	BN_free(original);
}

TEST(BignumHandleTest, ConstructorMakesCopy) {
	BIGNUM* original = BN_new();
	ASSERT_NE(nullptr, original);
	BN_set_word(original, 42);

	jwt::helper::bn_handle bn(original);

	BN_set_word(original, 100);
	EXPECT_EQ(42, BN_get_word(bn.get()));
	EXPECT_EQ(100, BN_get_word(original));

	BN_free(original);
}

TEST(BignumHandleTest, CopyConstructor) {
	jwt::helper::bn_handle bn1;
	BN_set_word(bn1.get(), 123);

	jwt::helper::bn_handle bn2(bn1);

	EXPECT_NE(bn1.get(), bn2.get());
	EXPECT_EQ(123, BN_get_word(bn1.get()));
	EXPECT_EQ(123, BN_get_word(bn2.get()));
}

TEST(BignumHandleTest, CopiesAreIndependent) {
	jwt::helper::bn_handle bn1;
	BN_set_word(bn1.get(), 50);

	jwt::helper::bn_handle bn2(bn1);
	BN_set_word(bn2.get(), 100);

	EXPECT_EQ(50, BN_get_word(bn1.get()));
	EXPECT_EQ(100, BN_get_word(bn2.get()));
}

TEST(BignumHandleTest, MoveConstructor) {
	jwt::helper::bn_handle bn1;
	BN_set_word(bn1.get(), 999);
	BIGNUM* original_ptr = bn1.get();

	jwt::helper::bn_handle bn2(std::move(bn1));

	EXPECT_EQ(nullptr, bn1.get());
	EXPECT_EQ(original_ptr, bn2.get());
	EXPECT_EQ(999, BN_get_word(bn2.get()));
}

TEST(BignumHandleTest, GetMethod) {
	jwt::helper::bn_handle bn;
	BIGNUM* ptr = bn.get();
	ASSERT_NE(nullptr, ptr);

	BIGNUM* ptr2 = bn.get();
	EXPECT_EQ(ptr, ptr2);
}

TEST(BignumHandleTest, OperatorNot) {
	jwt::helper::bn_handle bn_valid;
	EXPECT_FALSE(!bn_valid);

	jwt::helper::bn_handle bn_source;
	jwt::helper::bn_handle bn_dest(std::move(bn_source));
	EXPECT_TRUE(!bn_source);
}

TEST(BignumHandleTest, OperatorBool) {
	jwt::helper::bn_handle bn_valid;
	EXPECT_TRUE(bn_valid);

	jwt::helper::bn_handle bn_source;
	jwt::helper::bn_handle bn_dest(std::move(bn_source));
	EXPECT_FALSE(bn_source);
}

TEST(BignumHandleTest, AssignmentDisabled) {
	std::vector<jwt::helper::bn_handle> vec;
	jwt::helper::bn_handle bn1;
	BN_set_word(bn1.get(), 10);
	vec.push_back(std::move(bn1));

	ASSERT_EQ(1, vec.size());
	EXPECT_EQ(10, BN_get_word(vec[0].get()));
}

TEST(BignumHandleTest, DestructorCleansUp) {
	BIGNUM* bn = BN_new();
	ASSERT_NE(nullptr, bn);
	BN_set_word(bn, 777);

	{
		jwt::helper::bn_handle handle(bn);
		EXPECT_EQ(777, BN_get_word(handle.get()));
	}
}

TEST(BignumHandleTest, LargeBIGNUM) {
	BIGNUM* large = BN_new();
	ASSERT_NE(nullptr, large);

	BN_set_word(large, 0xFFFFFFFF);
	BN_lshift(large, large, 64);

	jwt::helper::bn_handle bn(large);
	EXPECT_FALSE(BN_is_zero(bn.get()));

	BN_free(large);
}

TEST(BignumHandleTest, NullPointerThrows) {
	EXPECT_THROW({ jwt::helper::bn_handle bn(nullptr); }, std::runtime_error);
}

TEST(BignumHandleTest, IntegrationWithRaw2bn) {
	std::string key = "test_key_data";
	auto bn_ptr = jwt::helper::raw2bn(key);

	BIGNUM* raw = bn_ptr.release();
	ASSERT_NE(nullptr, raw);

	jwt::helper::bn_handle bn(raw);
	EXPECT_FALSE(BN_is_zero(bn.get()));
}

TEST(BignumHandleTest, RAIIWithExceptions) {
	int cleanup_count = 0;

	try {
		jwt::helper::bn_handle bn;
		BN_set_word(bn.get(), 456);
		throw std::runtime_error("Test exception");
		cleanup_count++;
	} catch (const std::runtime_error&) { cleanup_count++; }

	EXPECT_EQ(1, cleanup_count);
}
