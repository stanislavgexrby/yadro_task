#include "tape.hpp"

int Tape::readDelay = 0;
int Tape::writeDelay = 0;
int Tape::rewindDelay = 0;
int Tape::shiftDelay = 0;

std::map<std::string, int> readConfig(const std::string& configFile) {
    std::ifstream file(configFile);
    std::map<std::string, int> config;
    std::string line;

    while (std::getline(file, line)) {
        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos) {
            std::string key = line.substr(0, delimiterPos);
            int value = std::stoi(line.substr(delimiterPos + 1));
            config[key] = value;
        }
    }
    return config;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> <config_file>" << std::endl;
        return 1;
    }

    std::string inputPath = argv[1];
    std::string outputPath = argv[2];
    std::string configPath = argv[3];

    try {
        auto config = readConfig(configPath);

        if (config.find("read_delay") == config.end() ||
            config.find("write_delay") == config.end() ||
            config.find("rewind_delay") == config.end() ||
            config.find("shift_delay") == config.end()) {
            throw std::runtime_error("Invalid config file");
        }

        Tape::setDelays(
            config["read_delay"],
            config["write_delay"],
            config["rewind_delay"],
            config["shift_delay"]
        );

        Tape inputTape(inputPath, false);
        Tape outputTape(outputPath, true);

        inputTape.rewind();
        outputTape.rewind();

        while (!inputTape.isEOT()) {
            int value = inputTape.read();
            std::cout << value << std::endl;
            outputTape.write(value);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}