#include <SFML/Graphics.hpp>
#include <iostream>
#include "src/AppState.h"
#include "src/Cart.h"
#include "src/CashierScreen.h"
#include "src/ProductCatalog.h"
#include "src/ProductsScreen.h"
#include "src/ReceiptsScreen.h"
#include "src/StartScreen.h"
#include "src/SupermarketMenuScreen.h"
#include "src/TextureCache.h"
#include "src/UiHelpers.h"

namespace
{
    constexpr unsigned kWindowWidth = 1920;
    constexpr unsigned kWindowHeight = 1080;
    constexpr unsigned kFrameRate = 60;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({ kWindowWidth, kWindowHeight }), "Virtual Supermarket!");
    window.setFramerateLimit(kFrameRate);

    sf::Font font;
    if (!LoadFont(font, "fonts/VT323-Regular.ttf"))
    {
        std::cerr << "Run the game from the folder that contains the Images/ and fonts/ folders.\n";
    }

    TextureCache textures;
    const ProductCatalog catalog = LoadProductCatalog("Products.txt");
    Cart cart;

    // Each screen runs its own loop and tells us where to go next through `state`.
    AppState state = AppState::Start;
    bool firstScreen = true;

    while (window.isOpen())
    {
        switch (state)
        {
        case AppState::Start:
            RunStartScreen(window, state, font, textures, !firstScreen);
            break;
        case AppState::Menu:
            RunSupermarketMenuScreen(window, state, font, textures, cart);
            break;
        case AppState::Products:
            RunProductsScreen(window, state, font, textures, catalog, cart);
            break;
        case AppState::Cashier:
            RunCashierScreen(window, state, font, textures, cart);
            break;
        case AppState::Receipts:
            RunReceiptsScreen(window, state, font, textures);
            break;
        }
        firstScreen = false;
    }

    return 0;
}
