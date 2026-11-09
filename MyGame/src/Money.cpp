#include "Money.h"
#include <iomanip>
#include <sstream>

std::string FormatMoney(float amount)
{
    std::ostringstream stream;
    stream << "$" << std::fixed << std::setprecision(2) << amount;
    return stream.str();
}
