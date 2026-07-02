#include <gtest/gtest.h>
#include "Matrix.h"
#include <vector>
#include <string>
#include <utility>

#if defined(_WIN32)
#include "pch.h"
#else
#include "gmock/gmock.h"
#endif

// ---------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------

TEST(MatrixConstruction, DefaultConstructorIsEmpty)
{
    Matrix<double> m;
    EXPECT_EQ(m.nRows(), 0u);
    EXPECT_EQ(m.nCols(), 0u);
}

TEST(MatrixConstruction, SizeConstructorSetsDimensions)
{
    Matrix<double> m(3, 4);
    EXPECT_EQ(m.nRows(), 3u);
    EXPECT_EQ(m.nCols(), 4u);
}

TEST(MatrixConstruction, SizeConstructorValueInitializesElements)
{
    // Tx() for double is 0.0
    Matrix<double> m(2, 2);
    for (size_t i = 0; i < m.nRows(); ++i)
        for (size_t j = 0; j < m.nCols(); ++j)
            EXPECT_DOUBLE_EQ(m(i, j), 0.0);
}

TEST(MatrixConstruction, FillConstructorSetsAllElements)
{
    Matrix<double> m(3, 2, 7.5);
    for (size_t i = 0; i < m.nRows(); ++i)
        for (size_t j = 0; j < m.nCols(); ++j)
            EXPECT_DOUBLE_EQ(m(i, j), 7.5);
}

TEST(MatrixConstruction, ZeroSizeMatrixIsValid)
{
    Matrix<double> m(0, 0);
    EXPECT_EQ(m.nRows(), 0u);
    EXPECT_EQ(m.nCols(), 0u);
}

// ---------------------------------------------------------------------
// Element access
// ---------------------------------------------------------------------

TEST(MatrixAccess, ReadWriteRoundTrip)
{
    Matrix<int> m(2, 3);
    m(0, 0) = 1;
    m(0, 1) = 2;
    m(0, 2) = 3;
    m(1, 0) = 4;
    m(1, 1) = 5;
    m(1, 2) = 6;

    EXPECT_EQ(m(0, 0), 1);
    EXPECT_EQ(m(0, 1), 2);
    EXPECT_EQ(m(0, 2), 3);
    EXPECT_EQ(m(1, 0), 4);
    EXPECT_EQ(m(1, 1), 5);
    EXPECT_EQ(m(1, 2), 6);
}

TEST(MatrixAccess, ConstOperatorParensReturnsCorrectValue)
{
    Matrix<int> m(2, 2, 9);
    const Matrix<int>& cm = m;
    EXPECT_EQ(cm(0, 0), 9);
    EXPECT_EQ(cm(1, 1), 9);
}

#ifndef NDEBUG
TEST(MatrixAccessDeathTest, OutOfBoundsRowAsserts)
{
    Matrix<int> m(2, 2);
    EXPECT_DEATH(m(2, 0), "");
}

TEST(MatrixAccessDeathTest, OutOfBoundsColAsserts)
{
    Matrix<int> m(2, 2);
    EXPECT_DEATH(m(0, 2), "");
}
#endif

// ---------------------------------------------------------------------
// fill()
// ---------------------------------------------------------------------

TEST(MatrixFill, FillsAllElementsWithGivenValue)
{
    Matrix<double> m(4, 5);
    m.fill(3.14);
    for (size_t i = 0; i < m.nRows(); ++i)
        for (size_t j = 0; j < m.nCols(); ++j)
            EXPECT_DOUBLE_EQ(m(i, j), 3.14);
}

TEST(MatrixFill, FillOnEmptyMatrixDoesNothing)
{
    Matrix<double> m;
    EXPECT_NO_THROW(m.fill(1.0));
}

// ---------------------------------------------------------------------
// setSize()
// ---------------------------------------------------------------------

TEST(MatrixSetSize, TwoArgResizeUpdatesDimensions)
{
    Matrix<double> m;
    m.setSize(5, 6);
    EXPECT_EQ(m.nRows(), 5u);
    EXPECT_EQ(m.nCols(), 6u);
}

TEST(MatrixSetSize, TwoArgResizeDefaultInitializes)
{
    Matrix<double> m;
    m.setSize(2, 2);
    for (size_t i = 0; i < m.nRows(); ++i)
        for (size_t j = 0; j < m.nCols(); ++j)
            EXPECT_DOUBLE_EQ(m(i, j), 0.0);
}

TEST(MatrixSetSize, ThreeArgResizeFillsWithValue)
{
    Matrix<double> m;
    m.setSize(2, 3, 8.0);
    for (size_t i = 0; i < m.nRows(); ++i)
        for (size_t j = 0; j < m.nCols(); ++j)
            EXPECT_DOUBLE_EQ(m(i, j), 8.0);
}

TEST(MatrixSetSize, SingleArgMakesSquareMatrix)
{
    Matrix<double> m;
    m.setSizeSquare(4);
    EXPECT_EQ(m.nRows(), 4u);
    EXPECT_EQ(m.nCols(), 4u);
}

TEST(MatrixSetSize, SingleArgWithValueFillsSquareMatrix)
{
    Matrix<double> m;
    m.setSizeSquare(3, 2.5);
    EXPECT_EQ(m.nRows(), 3u);
    EXPECT_EQ(m.nCols(), 3u);
    for (size_t i = 0; i < m.nRows(); ++i)
        for (size_t j = 0; j < m.nCols(); ++j)
            EXPECT_DOUBLE_EQ(m(i, j), 2.5);
}

TEST(MatrixSetSize, CanGrowThenShrink)
{
    Matrix<double> m;
    m.setSize(10, 10, 1.0);
    m.setSize(2, 2, 5.0);
    EXPECT_EQ(m.nRows(), 2u);
    EXPECT_EQ(m.nCols(), 2u);
    for (size_t i = 0; i < m.nRows(); ++i)
        for (size_t j = 0; j < m.nCols(); ++j)
            EXPECT_DOUBLE_EQ(m(i, j), 5.0);
}

// ---------------------------------------------------------------------
// Copy semantics
// ---------------------------------------------------------------------

TEST(MatrixCopy, CopyConstructorDuplicatesData)
{
    Matrix<int> a(2, 2);
    a(0, 0) = 1; a(0, 1) = 2; a(1, 0) = 3; a(1, 1) = 4;

    Matrix<int> b(a);
    EXPECT_EQ(b.nRows(), a.nRows());
    EXPECT_EQ(b.nCols(), a.nCols());
    for (size_t i = 0; i < a.nRows(); ++i)
        for (size_t j = 0; j < a.nCols(); ++j)
            EXPECT_EQ(b(i, j), a(i, j));
}

TEST(MatrixCopy, CopyIsDeepNotShallow)
{
    Matrix<int> a(2, 2, 1);
    Matrix<int> b(a);
    b(0, 0) = 99;
    EXPECT_EQ(a(0, 0), 1);   // original must be unaffected
    EXPECT_EQ(b(0, 0), 99);
}

TEST(MatrixCopy, CopyAssignmentSameShapeSucceeds)
{
    Matrix<int> a(2, 2);
    a(0, 0) = 1; a(0, 1) = 2; a(1, 0) = 3; a(1, 1) = 4;

    Matrix<int> b(2, 2, 0);
    b = a;

    for (size_t i = 0; i < a.nRows(); ++i)
        for (size_t j = 0; j < a.nCols(); ++j)
            EXPECT_EQ(b(i, j), a(i, j));
}

TEST(MatrixCopy, CopyAssignmentDoesNotAliasSource)
{
    Matrix<int> a(2, 2, 1);
    Matrix<int> b(2, 2, 0);
    b = a;
    b(0, 0) = 42;
    EXPECT_EQ(a(0, 0), 1);
}

TEST(MatrixCopy, CopyAssignmentMismatchedShapeThrows)
{
    Matrix<int> a(2, 2);
    Matrix<int> b(3, 3);
    EXPECT_THROW(b = a, std::runtime_error);
}

TEST(MatrixCopy, SelfAssignmentPreservesData)
{
    Matrix<int> a(2, 2);
    a(0, 0) = 1; a(0, 1) = 2; a(1, 0) = 3; a(1, 1) = 4;

    a = a;

    EXPECT_EQ(a(0, 0), 1);
    EXPECT_EQ(a(0, 1), 2);
    EXPECT_EQ(a(1, 0), 3);
    EXPECT_EQ(a(1, 1), 4);
}

// ---------------------------------------------------------------------
// Move semantics
// ---------------------------------------------------------------------

TEST(MatrixMove, MoveConstructorTransfersData)
{
    Matrix<int> a(2, 2);
    a(0, 0) = 1; a(0, 1) = 2; a(1, 0) = 3; a(1, 1) = 4;

    Matrix<int> b(std::move(a));
    EXPECT_EQ(b.nRows(), 2u);
    EXPECT_EQ(b.nCols(), 2u);
    EXPECT_EQ(b(0, 0), 1);
    EXPECT_EQ(b(1, 1), 4);
}

TEST(MatrixMove, MoveAssignmentTransfersData)
{
    Matrix<int> a(2, 2);
    a(0, 0) = 1; a(0, 1) = 2; a(1, 0) = 3; a(1, 1) = 4;

    Matrix<int> b;
    b = std::move(a);

    EXPECT_EQ(b.nRows(), 2u);
    EXPECT_EQ(b.nCols(), 2u);
    EXPECT_EQ(b(0, 0), 1);
    EXPECT_EQ(b(1, 1), 4);
}

TEST(MatrixMove, MoveAssignmentAllowsDifferentShapes)
{
    // Unlike copy assignment, move assignment (compiler-generated)
    // does not require matching dimensions.
    Matrix<int> a(2, 2, 1);
    Matrix<int> b(5, 5, 0);
    b = std::move(a);
    EXPECT_EQ(b.nRows(), 2u);
    EXPECT_EQ(b.nCols(), 2u);
}

// ---------------------------------------------------------------------
// Iterators
// ---------------------------------------------------------------------

TEST(MatrixIterators, BeginEndCoverAllElementsInRowMajorOrder)
{
    Matrix<int> m(2, 3);
    int v = 0;
    for (size_t i = 0; i < m.nRows(); ++i)
        for (size_t j = 0; j < m.nCols(); ++j)
            m(i, j) = v++;

    std::vector<int> collected(m.begin(), m.end());
    std::vector<int> expected = { 0, 1, 2, 3, 4, 5 };
    EXPECT_EQ(collected, expected);
}

TEST(MatrixIterators, DistanceMatchesTotalElementCount)
{
    Matrix<double> m(4, 5);
    EXPECT_EQ(static_cast<size_t>(std::distance(m.begin(), m.end())), 20u);
}

// ---------------------------------------------------------------------
// Templating on other types
// ---------------------------------------------------------------------

TEST(MatrixTemplate, WorksWithIntType)
{
    Matrix<int> m(2, 2, 5);
    EXPECT_EQ(m(0, 0), 5);
    EXPECT_EQ(m(1, 1), 5);
}

TEST(MatrixTemplate, WorksWithStringType)
{
    Matrix<std::string> m(2, 2, std::string("x"));
    EXPECT_EQ(m(0, 0), "x");
    m(0, 1) = "hello";
    EXPECT_EQ(m(0, 1), "hello");
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}