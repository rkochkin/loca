#include "options.h"

bool Options::LoadFromCmdLine(int argc, char** argv) {
    const option longopts[] = {{"number", optional_argument, nullptr, 'n'},
                               {"resolution", optional_argument, nullptr, 'r'},
                               {"period", optional_argument, nullptr, 'p'},
                               {nullptr}};

    int res = 0;
    while ((res = getopt_long(argc, argv, "n:r:p:", longopts, nullptr)) != -1) {
        switch (res) {
            case 'n':
                if (auto c = GetCoord(optarg)) {
                    m_cellNumber = *c;
                } else {
                    return false;
                }
                break;
            case 'r':
                if (auto c = GetCoord(optarg)) {
                    m_windowResolution = *c;
                } else {
                    return false;
                }
                break;
            case 'p':
                try {
                    m_quantTime = std::chrono::milliseconds(std::stol(optarg));
                } catch (std::exception& e) {
                    return false;
                }
            case '?':
                std::cerr << "Error found!" << std::endl;
                break;
            default:
                return false;
        }
    }

    return true;
}
Coord Options::GetCellNumber() const { return m_cellNumber; }

Coord Options::GetWindowResolution() const { return m_windowResolution; }

std::chrono::microseconds Options::GetQuantTime() { return m_quantTime; }

std::optional<Coord> Options::GetCoord(const std::string& coordString) {
    Coord c;
    std::regex const rx("^\\s*([0-9]+)x([0-9]+)\\s*$");
    std::smatch sm;
    if (std::regex_search(coordString, sm, rx)) {
        try {
            c.X = std::stoi(sm.str(1));
            c.Y = std::stoi(sm.str(2));
        } catch (std::exception& e) {
            return std::nullopt;
        }
    } else {
        return std::nullopt;
    }

    return c;
}
