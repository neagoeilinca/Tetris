#include <gtest/gtest.h>

#include <set>

#include "RandomPieceTypeGenerator.h"

TEST(RandomPieceTypeGeneratorTests, GeneratesOnlyRealPieceTypes)
{
    RandomPieceTypeGenerator generator;

    for (int i = 0; i < 1000; i++)
    {
        EPieceType type = generator.nextPieceType();

        EXPECT_GE(type, I);
        EXPECT_LE(type, Z);
    }
}

TEST(RandomPieceTypeGeneratorTests, EventuallyGeneratesEveryPieceType)
{
    RandomPieceTypeGenerator generator;
    std::set<EPieceType> generatedTypes;

    for (int i = 0; i < 1000; i++)
    {
        generatedTypes.insert(generator.nextPieceType());
    }

    EXPECT_EQ(7u, generatedTypes.size());
}