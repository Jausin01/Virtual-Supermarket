#include "ReceiptStore.h"
#include "Money.h"
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace
{
    std::string GetCurrentDateTime()
    {
        std::time_t now = std::time(nullptr);
        std::tm localTime{};
#ifdef _WIN32
        localtime_s(&localTime, &now);
#else
        localtime_r(&now, &localTime);
#endif

        std::ostringstream stream;
        stream << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
        return stream.str();
    }

    int FindFreeSlot()
    {
        for (int slot = 0; slot < ReceiptStore::kSlotCount; ++slot)
        {
            if (!ReceiptStore::SlotHasContent(slot))
            {
                return slot;
            }
        }
        return 0; // All slots are used: overwrite the first one.
    }
}

namespace ReceiptStore
{
    std::string PathForSlot(int slot)
    {
        return "receipt" + std::to_string(slot + 1) + ".txt";
    }

    bool SlotHasContent(int slot)
    {
        std::ifstream file(PathForSlot(slot));
        return file.is_open() && file.peek() != std::ifstream::traits_type::eof();
    }

    std::string ReadSlot(int slot)
    {
        std::ifstream file(PathForSlot(slot));
        if (!file.is_open())
        {
            return "Receipt file not found.";
        }

        std::stringstream content;
        content << file.rdbuf();
        return content.str();
    }

    bool Save(const Receipt& receipt)
    {
        const std::string path = PathForSlot(FindFreeSlot());

        std::ofstream file(path, std::ios::trunc);
        if (!file.is_open())
        {
            std::cerr << "Failed to open file: " << path << "\n";
            return false;
        }

        file << "=== Supermarket Receipt ===\n";
        file << "Customer Name: " << receipt.customerName << "\n";
        file << "Phone Number: " << receipt.phoneNumber << "\n";
        file << "Email: " << receipt.email << "\n";
        file << "Date & Time: " << GetCurrentDateTime() << "\n\n";

        file << "Items Purchased:\n";
        for (const auto& item : receipt.items)
        {
            file << "- " << item.first << " x" << item.second << "\n";
        }

        file << "\nTotal Paid: " << FormatMoney(receipt.totalPaid) << "\n";
        file << "===========================\n";
        return true;
    }
}
