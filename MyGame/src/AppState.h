#pragma once

/// Screen the player is currently on. Screens share one AppState so a
/// nested screen can tell its caller where to go next.
enum class AppState
{
    Start,
    Menu,
    Products,
    Cashier,
    Receipts,
};
