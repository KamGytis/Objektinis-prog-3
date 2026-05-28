/**
 * @file vector_unit_tests.cpp
 * @brief Google Test testai nuosavai Vector klasei.
 *
 * Patikrina visą Vector funkcionalumą pagal std::vector specifikaciją.
 * Kiekviena testuojama grupė atitinka std::vector Member functions sekciją.
 */

#include <gtest/gtest.h>
#include "Vector.h"
#include <vector>
#include <string>
#include <sstream>
#include <numeric>

// 1. konstruktoriai


TEST(VectorConstructors, DefaultConstructor) {
    Vector<int> v;
    EXPECT_EQ(v.size(), 0u);
    EXPECT_EQ(v.capacity(), 0u);
    EXPECT_TRUE(v.empty());
}

TEST(VectorConstructors, CountValueConstructor) {
    Vector<int> v(5, 42);
    EXPECT_EQ(v.size(), 5u);
    for (size_t i = 0; i < 5; ++i)
        EXPECT_EQ(v[i], 42);
}

TEST(VectorConstructors, CountDefaultConstructor) {
    Vector<int> v(4);
    EXPECT_EQ(v.size(), 4u);
    for (size_t i = 0; i < 4; ++i)
        EXPECT_EQ(v[i], 0);
}

TEST(VectorConstructors, RangeConstructor) {
    std::vector<int> src = {1, 2, 3, 4, 5};
    Vector<int> v(src.begin(), src.end());
    EXPECT_EQ(v.size(), 5u);
    for (size_t i = 0; i < 5; ++i)
        EXPECT_EQ(v[i], (int)(i + 1));
}

TEST(VectorConstructors, InitializerListConstructor) {
    Vector<int> v = {10, 20, 30};
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
}

TEST(VectorConstructors, CopyConstructor) {
    Vector<int> a = {1, 2, 3};
    Vector<int> b(a);
    EXPECT_EQ(b.size(), 3u);
    EXPECT_EQ(b[0], 1);
    // Gili kopija - pakeitimas a neturi itakos b
    a[0] = 99;
    EXPECT_EQ(b[0], 1);
}

TEST(VectorConstructors, MoveConstructor) {
    Vector<int> a = {1, 2, 3};
    Vector<int> b(std::move(a));
    EXPECT_EQ(b.size(), 3u);
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(a.size(), 0u);  // originalas tuscias po perkėlimo
    EXPECT_EQ(a.data(), nullptr);
}

// 2. priskyrimo operatoriai

TEST(VectorAssignment, CopyAssignment) {
    Vector<int> a = {1, 2, 3};
    Vector<int> b;
    b = a;
    EXPECT_EQ(b.size(), 3u);
    a[0] = 99;
    EXPECT_EQ(b[0], 1);  // gili kopija
}

TEST(VectorAssignment, MoveAssignment) {
    Vector<int> a = {4, 5, 6};
    Vector<int> b;
    b = std::move(a);
    EXPECT_EQ(b.size(), 3u);
    EXPECT_EQ(b[0], 4);
    EXPECT_EQ(a.size(), 0u);
}

TEST(VectorAssignment, InitializerListAssignment) {
    Vector<int> v;
    v = {7, 8, 9};
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[2], 9);
}

TEST(VectorAssignment, SelfAssignment) {
    Vector<int> v = {1, 2, 3};
    v = v;  // neturi sukelti klaidos
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 1);
}

TEST(VectorAssignment, AssignCountValue) {
    Vector<int> v = {1, 2, 3};
    v.assign(5, 7);
    EXPECT_EQ(v.size(), 5u);
    for (size_t i = 0; i < 5; ++i)
        EXPECT_EQ(v[i], 7);
}

TEST(VectorAssignment, AssignRange) {
    std::vector<int> src = {10, 20, 30};
    Vector<int> v;
    v.assign(src.begin(), src.end());
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[1], 20);
}

TEST(VectorAssignment, AssignInitializerList) {
    Vector<int> v;
    v.assign({100, 200, 300});
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[2], 300);
}

// 3. elementu prieiga

TEST(VectorElementAccess, OperatorBrackets) {
    Vector<int> v = {10, 20, 30};
    EXPECT_EQ(v[0], 10);
    v[1] = 99;
    EXPECT_EQ(v[1], 99);
}

TEST(VectorElementAccess, AtInBounds) {
    Vector<int> v = {1, 2, 3};
    EXPECT_EQ(v.at(0), 1);
    EXPECT_EQ(v.at(2), 3);
}

TEST(VectorElementAccess, AtOutOfBounds) {
    Vector<int> v = {1, 2, 3};
    EXPECT_THROW(v.at(3), std::out_of_range);
    EXPECT_THROW(v.at(100), std::out_of_range);
}

TEST(VectorElementAccess, FrontBack) {
    Vector<int> v = {5, 6, 7};
    EXPECT_EQ(v.front(), 5);
    EXPECT_EQ(v.back(), 7);
    v.front() = 99;
    EXPECT_EQ(v[0], 99);
}

TEST(VectorElementAccess, DataPointer) {
    Vector<int> v = {1, 2, 3};
    int* ptr = v.data();
    EXPECT_EQ(ptr[0], 1);
    ptr[0] = 42;
    EXPECT_EQ(v[0], 42);
}

// 4. iteratoriai

TEST(VectorIterators, BeginEnd) {
    Vector<int> v = {1, 2, 3, 4, 5};
    int sum = 0;
    for (auto it = v.begin(); it != v.end(); ++it)
        sum += *it;
    EXPECT_EQ(sum, 15);
}

TEST(VectorIterators, RangeFor) {
    Vector<int> v = {2, 4, 6};
    int sum = 0;
    for (const auto& x : v) sum += x;
    EXPECT_EQ(sum, 12);
}

TEST(VectorIterators, ConstIterator) {
    const Vector<int> v = {10, 20, 30};
    int sum = 0;
    for (auto it = v.cbegin(); it != v.cend(); ++it)
        sum += *it;
    EXPECT_EQ(sum, 60);
}

TEST(VectorIterators, ReverseIterator) {
    Vector<int> v = {1, 2, 3};
    std::vector<int> rev(v.rbegin(), v.rend());
    EXPECT_EQ(rev[0], 3);
    EXPECT_EQ(rev[1], 2);
    EXPECT_EQ(rev[2], 1);
}

TEST(VectorIterators, StdAlgorithms) {
    Vector<int> v = {5, 3, 1, 4, 2};
    std::sort(v.begin(), v.end());
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[4], 5);
}

// 5. talpa (Capacity)

TEST(VectorCapacity, Empty) {
    Vector<int> v;
    EXPECT_TRUE(v.empty());
    v.push_back(1);
    EXPECT_FALSE(v.empty());
}

TEST(VectorCapacity, Size) {
    Vector<int> v;
    EXPECT_EQ(v.size(), 0u);
    v.push_back(1);
    v.push_back(2);
    EXPECT_EQ(v.size(), 2u);
}

TEST(VectorCapacity, Reserve) {
    Vector<int> v;
    v.reserve(100);
    EXPECT_GE(v.capacity(), 100u);
    EXPECT_EQ(v.size(), 0u);  // dydis nepakito
}

TEST(VectorCapacity, ReserveNoShrink) {
    Vector<int> v;
    v.reserve(100);
    size_t cap = v.capacity();
    v.reserve(10);  // mažesnis - neturėtų sumažėti
    EXPECT_EQ(v.capacity(), cap);
}

TEST(VectorCapacity, ShrinkToFit) {
    Vector<int> v;
    v.reserve(100);
    v.push_back(1);
    v.push_back(2);
    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), v.size());
}

TEST(VectorCapacity, CapacityGrowth) {
    Vector<int> v;
    size_t reallocs = 0;
    size_t prev_cap = 0;
    for (int i = 0; i < 1000; ++i) {
        if (v.capacity() == v.size()) ++reallocs;
        v.push_back(i);
        prev_cap = v.capacity();
    }
    // Turėtų būti logaritminis perskirstymų skaičius
    EXPECT_LT(reallocs, 25u);  // <= log2(1000) + 1
}

// 6. modifikatoriai (Modifiers)

TEST(VectorModifiers, Clear) {
    Vector<int> v = {1, 2, 3};
    v.clear();
    EXPECT_EQ(v.size(), 0u);
    EXPECT_TRUE(v.empty());
    // Capacity išsaugoma
    EXPECT_GE(v.capacity(), 3u);
}

TEST(VectorModifiers, PushBack) {
    Vector<int> v;
    for (int i = 0; i < 10; ++i) v.push_back(i);
    EXPECT_EQ(v.size(), 10u);
    EXPECT_EQ(v[9], 9);
}

TEST(VectorModifiers, PushBackMove) {
    Vector<std::string> v;
    std::string s = "testas";
    v.push_back(std::move(s));
    EXPECT_EQ(v[0], "testas");
    EXPECT_TRUE(s.empty() || s == "");  // po perkėlimo
}

TEST(VectorModifiers, PopBack) {
    Vector<int> v = {1, 2, 3};
    v.pop_back();
    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(v.back(), 2);
}

TEST(VectorModifiers, InsertSingle) {
    Vector<int> v = {1, 2, 4, 5};
    auto it = v.insert(v.begin() + 2, 3);
    EXPECT_EQ(v.size(), 5u);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(*it, 3);
}

TEST(VectorModifiers, InsertCount) {
    Vector<int> v = {1, 5};
    v.insert(v.begin() + 1, 3, 2);  // idesime 3 x "2"
    EXPECT_EQ(v.size(), 5u);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 2);
    EXPECT_EQ(v[3], 2);
    EXPECT_EQ(v[4], 5);
}

TEST(VectorModifiers, InsertRange) {
    Vector<int> v = {1, 5};
    std::vector<int> src = {2, 3, 4};
    v.insert(v.begin() + 1, src.begin(), src.end());
    EXPECT_EQ(v.size(), 5u);
    for (int i = 0; i < 5; ++i) EXPECT_EQ(v[i], i + 1);
}

TEST(VectorModifiers, InsertInitializerList) {
    Vector<int> v = {1, 5};
    v.insert(v.begin() + 1, {2, 3, 4});
    EXPECT_EQ(v.size(), 5u);
    for (int i = 0; i < 5; ++i) EXPECT_EQ(v[i], i + 1);
}

TEST(VectorModifiers, EraseSingle) {
    Vector<int> v = {1, 2, 3, 4};
    auto it = v.erase(v.begin() + 1);
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[1], 3);
    EXPECT_EQ(*it, 3);
}

TEST(VectorModifiers, EraseRange) {
    Vector<int> v = {1, 2, 3, 4, 5};
    v.erase(v.begin() + 1, v.begin() + 4);  // istrina 2,3,4
    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 5);
}

TEST(VectorModifiers, EmplaceBack) {
    Vector<std::pair<int,int>> v;
    v.emplace_back(1, 2);
    v.emplace_back(3, 4);
    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(v[0].first, 1);
    EXPECT_EQ(v[1].second, 4);
}

TEST(VectorModifiers, Emplace) {
    Vector<std::pair<int,int>> v = {{1,2}, {5,6}};
    v.emplace(v.begin() + 1, 3, 4);
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[1].first, 3);
}

TEST(VectorModifiers, ResizeSmaller) {
    Vector<int> v = {1, 2, 3, 4, 5};
    v.resize(3);
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[2], 3);
}

TEST(VectorModifiers, ResizeLarger) {
    Vector<int> v = {1, 2};
    v.resize(5, 99);
    EXPECT_EQ(v.size(), 5u);
    EXPECT_EQ(v[2], 99);
    EXPECT_EQ(v[4], 99);
}

TEST(VectorModifiers, Swap) {
    Vector<int> a = {1, 2, 3};
    Vector<int> b = {4, 5};
    a.swap(b);
    EXPECT_EQ(a.size(), 2u);
    EXPECT_EQ(b.size(), 3u);
    EXPECT_EQ(a[0], 4);
    EXPECT_EQ(b[0], 1);
}

// 7. palyginimo operatoriai (Non-member comparison)

TEST(VectorComparison, Equality) {
    Vector<int> a = {1, 2, 3};
    Vector<int> b = {1, 2, 3};
    Vector<int> c = {1, 2, 4};
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
}

TEST(VectorComparison, LexicographicOrder) {
    Vector<int> a = {1, 2, 3};
    Vector<int> b = {1, 2, 4};
    Vector<int> c = {1, 2, 3, 0};
    EXPECT_TRUE(a < b);
    EXPECT_TRUE(a < c);
    EXPECT_TRUE(b > a);
    EXPECT_TRUE(a <= a);
    EXPECT_TRUE(a >= a);
}

// 8. globalios funkcijos

TEST(VectorNonMember, GlobalSwap) {
    Vector<int> a = {1, 2};
    Vector<int> b = {3, 4, 5};
    swap(a, b);
    EXPECT_EQ(a.size(), 3u);
    EXPECT_EQ(b.size(), 2u);
}

TEST(VectorNonMember, EraseByValue) {
    Vector<int> v = {1, 2, 3, 2, 4, 2};
    auto removed = erase(v, 2);
    EXPECT_EQ(removed, 3u);
    EXPECT_EQ(v.size(), 3u);
    for (const auto& x : v) EXPECT_NE(x, 2);
}

TEST(VectorNonMember, EraseIf) {
    Vector<int> v = {1, 2, 3, 4, 5, 6};
    auto removed = erase_if(v, [](int x) { return x % 2 == 0; });
    EXPECT_EQ(removed, 3u);
    EXPECT_EQ(v.size(), 3u);
    for (const auto& x : v) EXPECT_EQ(x % 2, 1);
}

// 9. atminties perskyrimas

TEST(VectorReallocation, CountReallocations) {
    Vector<int> v;
    int reallocs = 0;
    size_t prev_cap = v.capacity();

    for (int i = 1; i <= 100000; ++i) {
        v.push_back(i);
        if (v.capacity() != prev_cap) {
            ++reallocs;
            prev_cap = v.capacity();
        }
    }
    // std::vector paprastai daro ~17 perskirstymų 100000 elementams
    EXPECT_LT(reallocs, 25);
    std::cout << "Vector perskirstymu skaicus (100000 el.): " << reallocs << "\n";
}

TEST(VectorReallocation, CompareWithStdVector) {
    // Lyginti perskirstymų skaičių su std::vector
    Vector<int> myV;
    std::vector<int> stdV;
    int myReallocs = 0, stdReallocs = 0;
    size_t myCap = 0, stdCap = 0;

    for (int i = 1; i <= 1000000; ++i) {
        if (myV.capacity() == myV.size()) ++myReallocs;
        if (stdV.capacity() == stdV.size()) ++stdReallocs;
        myV.push_back(i);
        stdV.push_back(i);
    }

    std::cout << "Vector perskirstymai (1M el.):     mano=" << myReallocs
              << ", std=" << stdReallocs << "\n";
    // Abu turėtų turėti panašų skaičių
    EXPECT_LT(myReallocs, 30);
    EXPECT_LT(stdReallocs, 30);
}

// 10. spartos palyginimas

TEST(VectorPerformance, PushBack10k) {
    auto t0 = std::chrono::high_resolution_clock::now();
    Vector<int> v;
    for (int i = 1; i <= 10000; ++i) v.push_back(i);
    auto dt = std::chrono::duration<double>(
        std::chrono::high_resolution_clock::now() - t0).count();
    EXPECT_EQ(v.size(), 10000u);
    EXPECT_EQ(v.back(), 10000);
    std::cout << "Vector push_back 10k:   " << dt << " s\n";
}

TEST(VectorPerformance, PushBack100k) {
    auto t0 = std::chrono::high_resolution_clock::now();
    Vector<int> v;
    for (int i = 1; i <= 100000; ++i) v.push_back(i);
    auto dt = std::chrono::duration<double>(
        std::chrono::high_resolution_clock::now() - t0).count();
    EXPECT_EQ(v.size(), 100000u);
    std::cout << "Vector push_back 100k:  " << dt << " s\n";
}

TEST(VectorPerformance, PushBack1M) {
    auto t0 = std::chrono::high_resolution_clock::now();
    Vector<int> v;
    for (int i = 1; i <= 1000000; ++i) v.push_back(i);
    auto dt = std::chrono::duration<double>(
        std::chrono::high_resolution_clock::now() - t0).count();
    EXPECT_EQ(v.size(), 1000000u);
    std::cout << "Vector push_back 1M:    " << dt << " s\n";
}

TEST(VectorPerformance, StdVectorPushBack1M) {
    auto t0 = std::chrono::high_resolution_clock::now();
    std::vector<int> v;
    for (int i = 1; i <= 1000000; ++i) v.push_back(i);
    auto dt = std::chrono::duration<double>(
        std::chrono::high_resolution_clock::now() - t0).count();
    EXPECT_EQ(v.size(), 1000000u);
    std::cout << "std::vector push_back 1M: " << dt << " s\n";
}

// 11. kraštutiniai atvejai (Edge Cases)

TEST(VectorEdgeCases, EmptyVectorIterators) {
    Vector<int> v;
    EXPECT_EQ(v.begin(), v.end());
    EXPECT_EQ(v.rbegin(), v.rend());
}

TEST(VectorEdgeCases, MoveAndPushBack) {
    Vector<int> a = {1, 2, 3};
    Vector<int> b = std::move(a);
    b.push_back(4);
    EXPECT_EQ(b.size(), 4u);
    EXPECT_EQ(b[3], 4);
}

TEST(VectorEdgeCases, StringElements) {
    Vector<std::string> v;
    v.push_back("labas");
    v.push_back("pasauli");
    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(v[0], "labas");
    v.insert(v.begin() + 1, "grazus");
    EXPECT_EQ(v[1], "grazus");
    EXPECT_EQ(v[2], "pasauli");
}

TEST(VectorEdgeCases, NestedVector) {
    Vector<Vector<int>> v;
    v.push_back({1, 2, 3});
    v.push_back({4, 5});
    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(v[0].size(), 3u);
    EXPECT_EQ(v[1][0], 4);
}

TEST(VectorEdgeCases, ReserveBeforePush) {
    Vector<int> v;
    v.reserve(1000);
    size_t cap = v.capacity();
    for (int i = 0; i < 1000; ++i) v.push_back(i);
    // Neturėtų būti perskirstymo
    EXPECT_EQ(v.capacity(), cap);
    EXPECT_EQ(v.size(), 1000u);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}