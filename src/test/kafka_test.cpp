/*
 * Souffle - A Datalog Compiler
 * Copyright (c) 2020, The Souffle Developers. All rights reserved.
 * Licensed under the Universal Permissive License v 1.0 as shown at:
 * - https://opensource.org/licenses/UPL
 * - <souffle root>/licenses/SOUFFLE-UPL.txt
 */

/************************************************************************
 *
 * @file kafka_test.cpp
 *
 ***********************************************************************/

#include "../Kafka.h"

#include "test.h"

#include <vector>

namespace souffle {
namespace kafka {
namespace test {

using souffle::kafka::detail::into;

TEST(Kafka, ByteIdentity) {
    const auto a = Vec<Byte>({0});
    const auto a1 = a;
    const auto b = into<Vec<Byte>>(std::move(a1));
    EXPECT_EQ(a, b);
}
TEST(Kafka, SizeTIdentity) {
    const auto a = static_cast<SizeT>(0);
    const auto a1 = a;
    const auto b = into<SizeT>(std::move(a1));
    EXPECT_EQ(a, b);
}
TEST(Kafka, SizeTIntoZero) {
    const auto a = static_cast<SizeT>(0);
    const auto a1 = a;
    const auto b = into<SizeT, Vec<Byte>, SizeT>(std::move(a1));
    EXPECT_EQ(a, b);
}
TEST(Kafka, SizeTIntoOne) {
    const auto a = static_cast<SizeT>(1);
    const auto a1 = a;
    const auto b = into<SizeT, Vec<Byte>, SizeT>(std::move(a1));
    EXPECT_EQ(a, b);
}
TEST(Kafka, SizeTIntoTwo) {
    const auto a = static_cast<SizeT>(2);
    const auto a1 = a;
    const auto b = into<SizeT, Vec<Byte>, SizeT>(std::move(a1));
    EXPECT_EQ(a, b);
}
TEST(Kafka, SizeTMaxInto) {
    const auto a = std::numeric_limits<SizeT>::max();
    const auto a1 = a;
    const auto b = into<SizeT, Vec<Byte>, SizeT>(std::move(a1));
    EXPECT_EQ(a, b);
}
TEST(Kafka, SizeTMinInto) {
    const auto a = std::numeric_limits<SizeT>::min();
    const auto a1 = a;
    const auto b = into<SizeT, Vec<Byte>, SizeT>(std::move(a1));
    EXPECT_EQ(a, b);
}

TEST(Kafka, StringIdentity) {
    const auto a = String("hello world");
    const auto a1 = a;
    const auto b = into<String>(std::move(a1));
    EXPECT_EQ(a, b);
}
TEST(Kafka, StringInto) {
    const auto a = String("hello world");
    const auto a1 = a;
    const auto b = into<String, Vec<Byte>, String>(std::move(a1));
    EXPECT_EQ(a, b);
}
TEST(Kafka, StringEmptyInto) {
    const auto a = String("");
    const auto a1 = a;
    const auto b = into<String, Vec<Byte>, String>(std::move(a1));
    EXPECT_EQ(a, b);
}
TEST(Kafka, VecStringIdentity) {
    const auto a = Vec<String>({"hello", "world"});
    const auto a1 = a;
    const auto b = into<Vec<String>>(std::move(a1));
    EXPECT_EQ(a, b);
}
TEST(Kafka, VecStringIntoMultiple) {
    const auto a = Vec<String>({"hello", "world"});
    const auto a1 = a;
    const auto b = into<Vec<String>, Vec<Byte>, Vec<String>>(std::move(a1));
    EXPECT_EQ(a, b);
}
TEST(Kafka, VecStringIntoSingle) {
    const auto a = Vec<String>({"hello world"});
    const auto a1 = a;
    const auto b = into<Vec<String>, Vec<Byte>, Vec<String>>(std::move(a1));
    EXPECT_EQ(a, b);
}
TEST(Kafka, VecStringIntoSingleEmpty) {
    const auto a = Vec<String>({""});
    const auto a1 = a;
    const auto b = into<Vec<String>, Vec<Byte>, Vec<String>>(std::move(a1));
    EXPECT_EQ(a, b);
}
TEST(Kafka, VecStringIntoEmpty) {
    const auto a = Vec<String>({});
    const auto a1 = a;
    const auto b = into<Vec<String>, Vec<Byte>, Vec<String>>(std::move(a1));
    EXPECT_EQ(a, b);
}

}  // end namespace test
}  // end namespace kafka
}  // end namespace souffle
