//
// Created by cyberkanjousen on 2025/10/17.
//

#include "crypto_tool.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <unistd.h>

#define ERR(STRING) std::cout << "\033[31m" << STRING << "\033[m" << std::endl

namespace crypto_tool {
    void Menu::initialize() {
        menuArg = 1;
        ifFile = true;
        ifEncrypt = true;
        str = "";
        fileRoad = "";
        key = 0;
    }

    void Menu::show_menu() const {
        switch (menuArg) {
            case 0:
                std::cout
                << "请输入字符串" << std::endl
                << ">";
                break;
            case 1:
                std::cout
                << "选择操作对象类型" << std::endl
                << "1:文本文件（.txt文件格式，ASCII编码）" << std::endl
                << "2:文本字符串（ASCII编码）" << std::endl
                << "3:退出" << std::endl
                << ">";
                break;
            case 2:
                std::cout
                << "选择加密/解密" << std::endl
                << "1:解密" << std::endl
                << "2:加密" << std::endl
                << "3:退出" << std::endl
                << ">";
                break;
            case 3:
                std::cout
                << "选择文件路径" << std::endl
                << ">";
                break;
            case 4:
                std::cout
                << "输入密钥并选择" << (ifEncrypt? "加": "解") << "密模式" << std::endl
                << "可选:" << std::endl
                << "凯撒密码: <number(0-94)>-cc 如: 42-cc" << std::endl
                << "异或密码: <number(0-94)>-xor 如: 42-xor" << std::endl
                << ">";
                break;
            default:
                break;
        }
    }

    void Menu::menu_select() {
        std::string input;
        char *ptr;
        std::getline(std::cin, input);
        char input_char[2];
        input_char[0] = input[0];
        const long ret = strtol(input_char, &ptr, 10);
        switch (menuArg) {
            case 1:
                switch (ret) {
                case 1:
                        ifFile = true;
                        menuArg = 3;
                        break;
                case 2:
                        ifFile = false;
                        menuArg = 0;
                        break;
                case 3:
                        ifRun = false;
                        break;
                default:
                        ERR("错误: 无效的选项");
                        break;
                }
                break;
            case 2:
                switch (ret) {
                case 1:
                        ifEncrypt = false;
                        menuArg = 4;
                        break;
                case 2:
                        ifEncrypt = true;
                        menuArg = 4;
                        break;
                case 3:
                        ifRun = false;
                        break;
                default:
                        ERR("错误: 无效的选项");
                        break;
                }
                break;
            default:
                break;
        }
    }

    void Menu::road_input() {
        std::string input;
        std::cin >> input;
        if (input.substr(input.size() - 4, 4) != ".txt") {
            ERR("错误: 目标文件必须是.txt文件");
            return;
        }
        try {
            if (const std::string ret = FileHandler::read_file(input); !ret.empty()) {
                str = ret;
                fileRoad = input;
            } else {
                ERR("错误：目标文件不能为空");
            }
            menuArg = 2;
        } catch (std::exception& e) {
            ERR(e.what());
            ERR("错误：文件路径无效");
        }
    }

    void Menu::string_input() {
        std::string input;
        std::getline(std::cin, input);
        str = input;
        menuArg = 2;
    }


    void Menu::key_input() {
        std::string input;
        std::string res;
        char *ptr;
        std::getline(std::cin, input);
        const long ret = strtol(input.c_str(), &ptr, 10);
        if (ret < 0
            || ret > 94
            || input.empty()
            || !input.contains('-')
            || (input.substr(input.find('-') + 1, input.size() - input.find_last_of('-') - 1) != "cc"
                && input.substr(input.find_last_of('-') + 1, input.size() - input.find_last_of('-') - 1) != "xor"
            )
        ) {
            ERR("错误: 输入密钥无效");
            return;
        }
        if (ifEncrypt) {
            try{
                res = Crypto::encrypt(ret, str, input.substr(input.find_last_of('-') + 1, input.size() - input.find_last_of('-') - 1));
                std::cout << "加密完成，结果为: " << res << std::endl;
            } catch (const std::exception &e) {
                ERR(e.what());
                ERR("加密失败：原文存在非法字符");
                initialize();
            }
        } else {
            try{
                res = Crypto::decrypt(ret, str, input.substr(input.find_last_of('-') + 1, input.size() - input.find_last_of('-') - 1));
                std::cout << "解密完成，结果为: " << res << std::endl;
            } catch (const std::exception &e) {
                ERR(e.what());
                ERR("解密失败：原文存在非法字符");
                initialize();
            }
        }
        try{
            if (ifFile) {
                const std::string newFileRoad = fileRoad.insert(fileRoad.find_last_of('.'), "-output");
                FileHandler::save_file(res, newFileRoad);
            }
            initialize();
        } catch (std::exception& e) {
            ERR(e.what());
            ERR("错误: 保存失败");
        }
    }

    std::string FileHandler::read_file(const std::string& road) {
            std::ifstream in(road);
            const std::istreambuf_iterator begin(in);
            const std::istreambuf_iterator<char> end;
            std::string str(begin, end);
            in.close();
            return str;
    }

    void FileHandler::save_file(const std::string &content, const std::string &file_road) {
        std::ofstream out(file_road);
        out << content;
        out.close();
        std::cout << "文件已经保存至'" << file_road << "'" << std::endl;
    }

    std::string Crypto::encrypt(const long key, const std::string& str, const std::string& mode) {
        std::string res;
        if (mode == "cc") {
            for (const unsigned char s : str) {
                if (int i = s; i >= 32 && i <= 126) {
                    i += static_cast<int>(key);
                    i = i > 126? i - 95: i;
                    res += static_cast<char>(i);
                } else {
                    res += static_cast<char>(s);
                }
            }
        } else if (mode == "xor") {
            for (const unsigned char s : str) {
                unsigned int i = static_cast<unsigned char>(s);
                i ^= static_cast<unsigned int>(key);
                char h[6];
                sprintf(h, "0x0%x", i);
                res += '[';
                res.append(h);
                res += ']';
            }
        }
        return res;
    }

    std::string Crypto::decrypt(const long key, const std::string& str, const std::string& mode) {
        std::string res;
        if (mode == "cc") {
            for (const unsigned char s : str) {
                if (int i = s; i >= 32 && i <= 126) {
                    i -= static_cast<int>(key);
                    i = i < 32? i + 95 : i;
                    res += static_cast<char>(i);
                } else {
                    res += static_cast<char>(s);
                }
            }
        } else if (mode == "xor") {
            bool ifTran = false;
            std::string h;
            for (const unsigned char s : str) {
                if (s == '[') {
                    ifTran = true;
                } else if (s == ']') {
                    ifTran = false;
                    char *ptr;
                    unsigned int i = std::strtol(h.c_str(), &ptr, 16);
                    i ^= static_cast<unsigned int>(key);
                    res += static_cast<char>(i);
                    h = "";
                } else if (ifTran) {
                    h += static_cast<char>(s);
                }
            }
        }
        return res;
    }
} // crypto_tool