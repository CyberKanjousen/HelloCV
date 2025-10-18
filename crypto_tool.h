//
// Created by cyberkanjousen on 2025/10/17.
//

#ifndef ENCRYPTAPPLICATION_CRYPTO_TOOL_H
#define ENCRYPTAPPLICATION_CRYPTO_TOOL_H
#include <string>

namespace crypto_tool {
    class Menu {
    public:
        void show_menu() const;
        void menu_select();
        void initialize();
        void road_input();
        void key_input();
        void string_input();
        int menuArg = 1;
        bool ifRun = true;
        std::string str;
        std::string fileRoad;
        short key = 0;
    private:
        bool ifFile = true;
        bool ifEncrypt = true;
    };
    class FileHandler {
    public:
        static std::string read_file(const std::string& road);
        static void save_file(const std::string& content, const std::string &file_road);
    };
    class Crypto {
    public:
        static std::string encrypt(long key, const std::string& str, const std::string& mode);

        static std::string decrypt(long key, const std::string &str, const std::string &mode);
    };
} // crypto_tool

#endif //ENCRYPTAPPLICATION_CRYPTO_TOOL_H