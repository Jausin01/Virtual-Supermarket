#pragma once
#include <string>

/// Formats an amount as dollars with two decimals, e.g. 9.99 -> "$9.99".
std::string FormatMoney(float amount);
