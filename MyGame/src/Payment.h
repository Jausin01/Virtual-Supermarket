#pragma once
#include <string>

/// Members get this fraction taken off the total.
constexpr float kMemberDiscount = 0.10f;

/// Total after the membership discount.
float ApplyMemberDiscount(float total);

/// Outcome of comparing what the customer offers with what they owe.
struct PaymentCheck
{
    bool enough = false;
    float difference = 0.f; // Change to give back when enough, amount still missing otherwise
};

/// Compares the amounts in whole cents, so values like 9.99 never fail because of float rounding.
PaymentCheck CheckPayment(float total, float paid);

/// Text the cashier says about a payment attempt.
std::string DescribePayment(const PaymentCheck& check);
