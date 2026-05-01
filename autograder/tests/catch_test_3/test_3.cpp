//
// Created by rudri on 9/12/2020.
//
#include "catch.hpp"
#include "redirect_io.h"
#include "P3.h"

using namespace std;

mutex mtx;

static void test_3() {
    int first = 0;
    int last = 0;
    cin >> first >> last;
    vector<int> primes;
    get_primes(primes, first, last);
    auto current = begin(primes);
    cout << *current++;
    while(current != end(primes))
        cout << " " << *current++;
    cout << endl;
}

TEST_CASE("Question #3") {
    execute_test("test_3.in", test_3);
}