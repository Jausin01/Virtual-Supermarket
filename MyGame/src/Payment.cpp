#include "Payment.h"
#include "Money.h"
#include <cmath>

float ApplyMemberDiscount(float total)
{
    return total * (1.f - kMemberDiscount);
}

PaymentCheck CheckPayment(float total, float paid)
{
    const long totalCents = std::lround(total * 100.f);
    const long paidCents = std::lround(paid * 100.f);

    PaymentCheck check;
    check.enough = paidCents >= totalCents;
    check.difference = (float)std::labs(paidCents - totalCents) / 100.f;
    return check;
}

std::string DescribePayment(const PaymentCheck& check)
{
    if (!check.enough)
    {
        return "Insufficient!\nStill missing: " + FormatMoney(check.difference);
    }
    if (check.difference > 0.f)
    {
        return "Thank you!\nYour change is: " + FormatMoney(check.difference);
    }
    return "Thank you for exact payment!";
}
