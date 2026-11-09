#pragma once
#include <string>
#include <utility>
#include <vector>

/// Everything printed on one receipt.
struct Receipt
{
    std::string customerName;
    std::string phoneNumber;
    std::string email;
    float totalPaid = 0.f;
    std::vector<std::pair<std::string, int>> items; // product name, quantity
};

/// Receipts are kept in a fixed number of text files (receipt1.txt ... receipt5.txt).
namespace ReceiptStore
{
    constexpr int kSlotCount = 5;

    /// File name of a slot (0-based index).
    std::string PathForSlot(int slot);

    /// True when the slot's file exists and is not empty.
    bool SlotHasContent(int slot);

    /// Whole text of a slot, or a short message when the file is missing.
    std::string ReadSlot(int slot);

    /// Writes the receipt into the first empty slot (slot 0 when all are full).
    /// Returns false if the file could not be written.
    bool Save(const Receipt& receipt);
}
