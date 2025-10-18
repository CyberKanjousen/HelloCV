#include <iostream>

#include "crypto_tool.h"

int main() {
    std::cout
    << R"( _   _  _____  _      _       _____    ____  _     _)" << std::endl
    << R"(| |_| ||  ___|| |    | |     / ___ \  / ___|| |   | |)" << std::endl
    << R"(|  _  ||  ___|| |    | |    / /   \ \/ /    | |   | |)" << std::endl
    << R"(| | | || |___ | |___ | |___ \ \___/ /\ \___  \ \_/ /)" << std::endl
    << R"(|_| |_||_____||_____||_____| \_____/  \____|  \___/)" << std::endl
    << "第二阶段第二周考核任务" << std::endl
    << "CryptoTool By CyberKanjousen" << std::endl
    << "仅支持ASCII编码字符的加密/解密" << std::endl;
    crypto_tool::Menu menu;
    while (menu.ifRun) {
        menu.show_menu();
        switch (menu.menuArg) {
            case 0:
                menu.string_input();
                break;
            case 3:
                menu.road_input();
                break;
            case 4:
                menu.key_input();
                break;
            default:
                menu.menu_select();
                break;
        }
    }
    std::cout << "程序已退出" << std::endl;
    return 0;
}
