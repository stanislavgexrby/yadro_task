#include "tape.hpp"

int Tape::read_delay = 0;
int Tape::write_delay = 0;
int Tape::rewind_delay = 0;
int Tape::shift_delay = 0;

std::map<std::string, int> apply_conf(const std::string& configFile) {
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

    std::string input_path = argv[1];
    std::string output_path = argv[2];
    std::string conf_path = argv[3];

    try {
        auto config = apply_conf(conf_path);

        if (config.find("read_delay") == config.end() ||
            config.find("write_delay") == config.end() ||
            config.find("rewind_delay") == config.end() ||
            config.find("shift_delay") == config.end()) {
            throw std::runtime_error("Invalid config file");
        }

        Tape::set_delays(
            config["read_delay"],
            config["write_delay"],
            config["rewind_delay"],
            config["shift_delay"]
        );

        Tape input_tape(input_path, false);
        Tape output_tape(output_path, true);

        input_tape.rewind();
        output_tape.rewind();

        while (!input_tape.is_EOT()) {
            int value = input_tape.read();
            std::cout << value << std::endl;
            output_tape.write(value);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}