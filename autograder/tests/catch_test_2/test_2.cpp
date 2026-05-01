//
// Created by rudri on 9/12/2020.
//
#include "catch.hpp"
#include "redirect_io.h"
#include "P2.h"
using namespace std;

static void test_2() {
    size_t n = 0;
    int value = 0;
    cin >> n;
    cin >> value;
    vector<int> v1(n);
    for (auto &item: v1)
        cin >> item;
    increase_values(v1, value);
    auto current = begin(v1);
    cout << *current++;
    while(current != end(v1))
        cout << " " << *current++;
    cout << endl;
}

TEST_CASE("Question #2") {
    execute_test("test_2.in", test_2);
}