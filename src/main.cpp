#include "sorting.hpp"

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
        size_t length_of_tape;
        //size_t size_of_ram;
        std::cout << "Enter legnth of tape:" << std::endl;
        std::cin >> length_of_tape;
        std::cout << "Enter size of RAM:" << std::endl;
        std::cin >> size_of_ram;



        Tape input_tape(input_path, length_of_tape, false);
        std::cout << "input  : ";
        print_tape(input_tape);
        Tape output_tape(output_path, length_of_tape, true);

        input_tape.rewind();
        output_tape.rewind();

        Sort(input_tape, output_tape);

        std::cout << std::endl;
        std::cout << "output : ";
        print_tape(output_tape);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}