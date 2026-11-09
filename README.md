# Virtual Supermarket

A mouse-driven shopping simulator written in **C++17** with **SFML 3**.
Walk into the store, fill a cart from seven shelves of products, pay at the cashier
(with an optional membership discount) and read your saved receipts.

Solo academic project, LaSalle College Vancouver (Game Programming).

## Features

- 45 products in 7 categories, loaded from a plain text file (`MyGame/Products.txt`)
- Shelves with hover info (name and price), paging for categories with more than 9 items
- Cart with a "Getting" / "Returning" mode to add or remove items
- Cashier dialogue: membership card (10% off), typed payment with live feedback
  ("Insufficient", change due, exact payment), member name / phone / email
- Receipts saved to text files (5 slots) and readable in-game with mouse-wheel scrolling
- Fade transitions between screens

## Controls

Everything is done with the mouse, except the cashier, where you type the amount and
your details and press **Enter** to confirm each step.

## Build

You need a C++17 compiler and [SFML 3](https://www.sfml-dev.org/).

### CMake (Windows, Linux, macOS)

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/VirtualSupermarket        # run from the build folder: assets are copied next to it
```

If SFML 3 is not installed, CMake downloads and builds it automatically.
Run the tests with `ctest --test-dir build`.

### Visual Studio 2022

1. Download SFML 3.0.0 (Visual C++ 17, 64-bit) and extract it to `External/SFML-3.0.0/`
   so that `External/SFML-3.0.0/include` and `External/SFML-3.0.0/lib` exist.
2. Open `MyGame.sln`, choose **x64**, build and run.
   The SFML DLLs are copied next to the executable after each build.

The game reads `Images/`, `fonts/` and `Products.txt` from its working directory.

## Project structure

```
MyGame/
  main.cpp                    Window setup and the screen state machine
  Products.txt                Product list: Name|Price|Category
  src/
    AppState.h                Which screen is active
    StartScreen / SupermarketMenuScreen / ProductsScreen /
    CashierScreen / ReceiptsScreen   One file pair per screen

    Product, ProductCatalog   Product data and Products.txt parsing
    Cart                      Cart contents and total
    Payment, Money            Discount, change calculation and money formatting
    ReceiptStore              Reading and writing receipt files
    ProductLayout             Where each product picture sits on the shelf (data table)

    TextureCache              Loads each image once
    UiHelpers                 Buttons, mouse input, text centering
    Transitions               Fade in / fade out
tests/tests.cpp               Tests for the logic that needs no window
```

## Assets

The game art is used for learning and portfolio purposes only. Some of it (for example the
Power-Ups shelf) is based on Nintendo's Super Mario series, which belongs to its owners.
The code has no license attached yet; add one before reusing it.
