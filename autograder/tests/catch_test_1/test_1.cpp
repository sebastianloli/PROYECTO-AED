//
// Created by rudri on 9/12/2020.
//
#include "catch.hpp"
#include "redirect_io.h"
#include "P1.h"
using namespace std;

static void test_1() {
    size_t n = 0;
    cin >> n;
    vector<long long> v1(n);
    for (auto &item: v1)
        cin >> item;
    long long total = sumar(v1);
    cout << total << endl;
}

TEST_CASE("Question #1") {
    execute_test("test_1.in", test_1);
}