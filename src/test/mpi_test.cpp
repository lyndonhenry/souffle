/*
 * Souffle - A Datalog Compiler
 * Copyright (c) 2018, The Souffle Developers. All rights reserved
 * Licensed under the Universal Permissive License v 1.0 as shown at:
 * - https://opensource.org/licenses/UPL
 * - <souffle root>/licenses/SOUFFLE-UPL.txt
 */

/************************************************************************
 *
 * @file mpi_test.cpp
 *
 * Tests for mpi.h and mpi.cpp.
 *
 ***********************************************************************/

#include <vector>

#include "Mpi.h"
#include "test.h"

namespace souffle {

namespace test {

TEST(mpi, mpi) {
    // TODO (lyndonhenry): should add more to this test to get full coverage of Mpi.h

    {
        EXPECT_EQ(mpi::commRank(), 0);
        EXPECT_EQ(mpi::commSize(), 1);
    }

    {
        std::vector<int> sendBuffer = {0, 1, 2};
        std::vector<int> recvBuffer;
        (void)mpi::isend(sendBuffer, 0, mpi::tagOf("isendRecvVectorInt"));
        mpi::recv(recvBuffer, 0, mpi::tagOf("isendRecvVectorInt"));
        EXPECT_EQ(sendBuffer, recvBuffer);
    }

    {
        std::string sendBuffer = "abc";
        std::string recvBuffer;
        (void)mpi::isend(sendBuffer, 0, mpi::tagOf("isendRecvString"));
        mpi::recv(recvBuffer, 0, mpi::tagOf("isendRecvString"));
        EXPECT_EQ(sendBuffer, recvBuffer);
    }
}
}
}
