#pragma once

#include "pch.h"

class Utils {
public:
    static std::string LoadTextFile(std::string filename)
    {
        std::ifstream fin(filename);

        if (!fin.is_open()) {
            throw std::runtime_error("Failed to open file : " + filename);
        }

        std::stringstream text;
        text << fin.rdbuf();
        return text.str();
    }
};