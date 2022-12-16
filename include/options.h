#pragma once

#include "coord.h"
#include <chrono>
#include <getopt.h>
#include <iostream>
#include <optional>
#include <regex>


class Options {
public:
    Options() = default;

    bool LoadFromCmdLine(int argc, char* argv[]);

    [[nodiscard]] Coord GetCellNumber() const;

    [[nodiscard]] Coord GetWindowResolution() const;

    [[nodiscard]] std::chrono::microseconds GetQuantTime();

private:
    Coord m_cellNumber{100, 100};
    Coord m_windowResolution{800, 800};
    std::chrono::milliseconds m_quantTime{100};

    static std::optional<Coord> GetCoord(const std::string& coordString);
};