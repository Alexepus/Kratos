#include "stdafx.h"
#include <gtest/gtest.h>
#include "../CRegion.h"

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

struct CRegionsGenerated : testing::Test
{
	void SetUp() override
	{

	}

	void TearDown() override
	{
	}
};

CRegion* CreateRegion(int priority, int maxPassages, int passagesDone)
{
	auto r = new CRegion();
	r->m_DataIn.Priority = priority;
	r->m_DataIn.N_ = maxPassages;
	r->m_DataIn.Curr_N = passagesDone;
	return r;
}

TEST(CRegion, ShouldGetNextByPriority_SamePriority)
{
	// Given
	CreateRegion(2, 5, 5);
	CreateRegion(2, 5, 4);
	CreateRegion(2, 5, 3);
	CreateRegion(2, 5, 3);

	// When
	auto found = CRegion::GetNextByPriority();
	
	// Then
	ASSERT_NE(found, nullptr);
	ASSERT_EQ(found->ID, 2);
}

TEST(CRegion, ShouldGetNextByPriority_DifferentPriorities)
{
	// Given
	CreateRegion(20, 5, 3);
	CreateRegion(3, 5, 3);
	CreateRegion(6, 5, 3);
	CreateRegion(2, 5, 3);
	CreateRegion(4, 5, 3);

	// When
	auto found = CRegion::GetNextByPriority();

	// Then
	ASSERT_NE(found, nullptr);
	ASSERT_EQ(found->ID, 3);
}
