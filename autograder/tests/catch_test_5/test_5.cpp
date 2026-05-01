//
// Created by rudri on 9/12/2020.
//
#include "catch.hpp"
#include "redirect_io.h"
#include "P5.h"
#include <iostream>
#include <thread>
#include <vector>
#include <numeric>
using namespace std;

static void test_5() {
  utec::stadium nacional(10, 40, 40, 80, 80);
  auto r1 = nacional.reserve(utec::tribune::south, 1, 1);
  std::cout << std::boolalpha << r1 << std::endl; // true
  auto r2 = nacional.reserve(utec::tribune::south, 1, 1);
  std::cout << std::boolalpha << r2 << std::endl; // false
  auto r3 = nacional.reserve(utec::tribune::north, 1, 4);
  std::cout << std::boolalpha << r3 << std::endl; // true
  auto r4 = nacional.reserve(utec::tribune::west, 1, 4);
  std::cout << std::boolalpha << r4 << std::endl; // true
  auto r5 = nacional.reserve(utec::tribune::north, 1, 4);
  std::cout << std::boolalpha << r5 << std::endl; // false
}

TEST_CASE("Question #5") {
    execute_test("test_5.in", test_5);
}