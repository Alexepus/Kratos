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
	r->m_DataIn.Off = false;
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
	CreateRegion(2, 5, 5);
	CreateRegion(2, 5, 3);

	// When
	auto found = CRegion::GetNextByPriority(3);
	
	// Then
	ASSERT_NE(found, nullptr);
	ASSERT_EQ(found->ID, 1);
}

TEST_F(CRegionTest, ShouldGetNextByPriority_SkipOff)
{
	// Given
	CreateRegion(2, 5, 0)->m_DataIn.Off = true;
	CreateRegion(2, 5, 0)->m_DataIn.Off = true;
	CreateRegion(2, 5, 0);
	CreateRegion(2, 5, 0);

	// When
	auto found = CRegion::GetNextByPriority(3);

	// Then
	ASSERT_NE(found, nullptr);
	ASSERT_EQ(found->ID, 2);
}

TEST_F(CRegionTest, ShouldGetNextByPriority_SkipOffAndEdited)
{
	// Given
	CreateRegion(2, 5, 0)->m_DataIn.Off = true;
	CreateRegion(2, 5, 0)->m_DataIn.Off = true;
	auto edited = CreateRegion(2, 5, 0);
	CreateRegion(2, 5, 0);

	// When
	auto found = CRegion::GetNextByPriority(3, edited);

	// Then
	ASSERT_NE(found, nullptr);
	ASSERT_EQ(found->ID, 3);
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
	auto found = CRegion::GetNextByPriority(-1);

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

TEST_F(CRegionTest, Swap2)
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
	ASSERT_EQ(vec[0]->ID, 0);
	ASSERT_EQ(vec[1]->ID, 1);
}

TEST_F(CRegionTest, Swap3_0_2)
{
	// Given
	auto r0 = new CRegion();
	r0->m_DataIn.N_ = 1;
	auto r1 = new CRegion();
	r1->m_DataIn.N_ = 2;
	auto r2 = new CRegion();
	r2->m_DataIn.N_ = 3;

	// When
	CRegion::Swap(r0, r2);

	// Then
	auto vec = CRegion::GetAsVector();
	ASSERT_EQ(vec[0]->m_DataIn.N_, 3);
	ASSERT_EQ(vec[1]->m_DataIn.N_, 2);
	ASSERT_EQ(vec[2]->m_DataIn.N_, 1);
	ASSERT_EQ(vec[0]->ID, 0);
	ASSERT_EQ(vec[1]->ID, 1);
	ASSERT_EQ(vec[2]->ID, 2);
}

TEST_F(CRegionTest, Swap3_1_0)
{
	// Given
	auto r0 = new CRegion();
	r0->m_DataIn.N_ = 1;
	auto r1 = new CRegion();
	r1->m_DataIn.N_ = 2;
	auto r2 = new CRegion();
	r2->m_DataIn.N_ = 3;

	// When
	CRegion::Swap(r1, r0);

	// Then
	auto vec = CRegion::GetAsVector();
	ASSERT_EQ(vec[0]->m_DataIn.N_, 2);
	ASSERT_EQ(vec[1]->m_DataIn.N_, 1);
	ASSERT_EQ(vec[2]->m_DataIn.N_, 3);
	ASSERT_EQ(vec[0]->ID, 0);
	ASSERT_EQ(vec[1]->ID, 1);
	ASSERT_EQ(vec[2]->ID, 2);
}

TEST_F(CRegionTest, MoveRegionsIfPossible_1_2_Down)
{
	// Given
	auto r0 = new CRegion();
	r0->m_DataIn.N_ = 1;
	auto r1 = new CRegion();
	r1->m_DataIn.N_ = 2;
	auto r2 = new CRegion();
	r2->m_DataIn.N_ = 3;
	auto r3 = new CRegion();
	r3->m_DataIn.N_ = 4;

	// When
	CRegion::MoveRegionsIfPossible(std::vector<CRegion*>({r1, r2}), Directions::DownToEnd);

	// Then
	auto vec = CRegion::GetAsVector();
	ASSERT_EQ(vec.size(), 4);
	EXPECT_EQ(vec[0]->m_DataIn.N_, 1);
	EXPECT_EQ(vec[1]->m_DataIn.N_, 4);
	EXPECT_EQ(vec[2]->m_DataIn.N_, 2);
	EXPECT_EQ(vec[3]->m_DataIn.N_, 3);
}

TEST_F(CRegionTest, MoveRegionsIfPossible_0_2_Up)
{
	// Given
	auto r0 = new CRegion();
	r0->m_DataIn.N_ = 1;
	auto r1 = new CRegion();
	r1->m_DataIn.N_ = 2;
	auto r2 = new CRegion();
	r2->m_DataIn.N_ = 3;
	auto r3 = new CRegion();
	r3->m_DataIn.N_ = 4;

	// When
	CRegion::MoveRegionsIfPossible(std::vector<CRegion*>({ r0, r2 }), Directions::UpToBegin);

	// Then
	auto vec = CRegion::GetAsVector();
	ASSERT_EQ(vec.size(), 4);
	EXPECT_EQ(vec[0]->m_DataIn.N_, 1);
	EXPECT_EQ(vec[1]->m_DataIn.N_, 3);
	EXPECT_EQ(vec[2]->m_DataIn.N_, 2);
	EXPECT_EQ(vec[3]->m_DataIn.N_, 4);
}
