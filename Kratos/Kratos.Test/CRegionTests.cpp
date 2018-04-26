#include "stdafx.h"
#include <gtest/gtest.h>
#include "../CRegion.h"

struct CRegionTest : testing::Test
{
	void SetUp() override
	{
		CRegion::DeleteAllRegions();
	}

	void TearDown() override
	{
		CRegion::DeleteAllRegions();
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

TEST(CRegion, ShouldDeleteAllRegions)
{
	// Given
	new CRegion();
	new CRegion();

	// When
	CRegion::DeleteAllRegions();

	// Then
	ASSERT_EQ(CRegion::m_NReg, 0);
	ASSERT_EQ(CRegion::GetFirst(), nullptr);
	ASSERT_EQ(CRegion::m_pEnd, nullptr);
}

TEST_F(CRegionTest, ShouldCountInstances)
{
	CRegion::DeleteAllRegions();
	CRegion reg;
	EXPECT_EQ(CRegion::m_NReg, 1);
	{
		CRegion reg2;
		EXPECT_EQ(CRegion::m_NReg, 2);
	}
	EXPECT_EQ(CRegion::m_NReg, 1);
}

TEST_F(CRegionTest, ShouldGetNextByPriority_SamePriority)
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

TEST_F(CRegionTest, ShouldGetNextByPriority_DifferentPriorities)
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

TEST_F(CRegionTest, CanMoveAnyRegion_MoveUp_ShouldReturnTrue)
{
	// Given
	auto r0 = new CRegion();
	auto r1 = new CRegion();
	auto r2 = new CRegion();
	auto r3 = new CRegion();

	std::vector<CRegion*> regsToMove = { r1, r2, r3 };

	// When
	auto result = CRegion::CanMoveAnyRegion(regsToMove, Directions::UpToBegin);

	// Then
	ASSERT_TRUE(result);
}

TEST_F(CRegionTest, CanMoveAnyRegion_MoveUp_ShouldReturnFalse)
{
	// Given
	auto r0 = new CRegion();
	auto r1 = new CRegion();
	auto r2 = new CRegion();
	auto r3 = new CRegion();

	std::vector<CRegion*> regsToMove = { r0, r1, r2 };

	// When
	auto result = CRegion::CanMoveAnyRegion(regsToMove, Directions::UpToBegin);

	// Then
	ASSERT_FALSE(result);
}

TEST_F(CRegionTest, CanMoveAnyRegion_MoveDown_ShouldReturnTrue)
{
	// Given
	auto r0 = new CRegion();
	auto r1 = new CRegion();
	auto r2 = new CRegion();
	auto r3 = new CRegion();

	std::vector<CRegion*> regsToMove = { r0, r2 };

	// When
	auto result = CRegion::CanMoveAnyRegion(regsToMove, Directions::DownToEnd);

	// Then
	ASSERT_TRUE(result);
}

TEST_F(CRegionTest, CanMoveAnyRegion_MoveDown_ShouldReturnFalse)
{
	// Given
	auto r0 = new CRegion();
	auto r1 = new CRegion();
	auto r2 = new CRegion();
	auto r3 = new CRegion();

	std::vector<CRegion*> regsToMove = { r2, r3 };

	// When
	auto result = CRegion::CanMoveAnyRegion(regsToMove, Directions::DownToEnd);

	// Then
	ASSERT_FALSE(result);
}

TEST_F(CRegionTest, Swap)
{
	// Given
	auto r0 = new CRegion();
	r0->m_DataIn.N_ = 1;
	auto r1 = new CRegion();
	r1->m_DataIn.N_ = 2;

	// When
	CRegion::Swap(r0, r1);

	// Then
	auto vec = CRegion::GetAsVector();
	ASSERT_EQ(vec[0]->m_DataIn.N_, 2);
	ASSERT_EQ(vec[1]->m_DataIn.N_, 1);
}
