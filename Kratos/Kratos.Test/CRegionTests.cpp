#include "stdafx.h"
#include <gtest/gtest.h>
#include "../Region.h"

TEST(CRegion, ShouldCountInstances)
{
	CRegion reg;
	EXPECT_EQ(CRegion::m_NReg, 1);
	{
		CRegion reg2;
		EXPECT_EQ(CRegion::m_NReg, 2);
	}
	EXPECT_EQ(CRegion::m_NReg, 1);
}
