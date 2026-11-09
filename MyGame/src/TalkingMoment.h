#pragma once

/// Steps of the conversation with the cashier, in the order they are shown.
enum class TalkingMoment
{
    Introduction,   // "Do you have a membership card?"
    PayMoment,      // The player types how much they are paying
    GetName,        // Members only
    GetPhoneNumber, // Members only
    GetEmail,       // Members only
    GetReceipt,     // Receipt is saved, player leaves
};
