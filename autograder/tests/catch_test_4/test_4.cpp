//
// Created by rudri on 9/12/2020.
//
#include "catch.hpp"
#include "redirect_io.h"
#include "P4.h"
#include <iostream>
#include <thread>
#include <vector>
#include <numeric>
using namespace std;

static void test_4() {
    linked_list_t<int> ll;
    // cantidad
    int nhilos = {};
    cin >> nhilos;
    // Hilos
    vector <thread> vhilos(nhilos);
    for (int i = 0; i < nhilos - nhilos/4; ++i)
        vhilos[i] = thread([&ll, i] { ll.push_front(i); });
    for (int i = nhilos - nhilos/4; i < nhilos; ++i)
        vhilos[i] = thread([&ll] { ll.pop_front(); });
    for (auto& hilo: vhilos) hilo.join();
    // Resultado


    cout << ll.size() << endl;
}

TEST_CASE("Question #4") {
    execute_test("test_4.in", test_4);
}