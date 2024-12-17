
#include "Simulate.h"

void replaceBrackets(std::string& input) {
    for (char& c : input) {
        if (c == '(') {
            c = '<'; 
        } else if (c == ')') {
            c = '>';
        }
    }
}

using SimulateVariant = std::variant<Simulate<Fixed<(int_fast32_t)32, 16>,Fixed<(int_fast32_t)32, 16>,Fixed<(int_fast32_t)32, 16>>, Simulate<Fixed<(int_fast32_t)32, 16>,Fixed<(int_fast32_t)32, 16>,Fixed<16,12>>, Simulate<Fixed<(int_fast32_t)32, 16>,Fixed<16,12>,Fixed<(int_fast32_t)32, 16>>, Simulate<Fixed<(int_fast32_t)32, 16>,Fixed<16,12>,Fixed<16,12>>, Simulate<Fixed<16,12>,Fixed<(int_fast32_t)32, 16>,Fixed<(int_fast32_t)32, 16>>, Simulate<Fixed<16,12>,Fixed<(int_fast32_t)32, 16>,Fixed<16,12>>, Simulate<Fixed<16,12>,Fixed<16,12>,Fixed<(int_fast32_t)32, 16>>, Simulate<Fixed<16,12>,Fixed<16,12>,Fixed<16,12>>>;

int main(int argc, char* argv[]) {
    // Считывание поля из файла (всегда обязательно, так как -DSIZE не задается - по тз опционально)
    if (argc < 5) {
        std::cout << "No suitable params";
        exit(1);
    }
    std::string file_name = argv[4];
    std::string path_file_name = "../" + file_name;
    std::ifstream file(path_file_name);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << path_file_name << std::endl;
        exit(1);
    }
    std::string line;
    while (std::getline(file, line)) {
        int X, Y;
        if (sscanf(line.c_str(), "S(%d,%d)", &Y, &X) == 2) {
            N_field = Y;
            M_field = X;
        }
    }
    file.close();
    
    std::unordered_map<std::string, int> params = { {"FAST_FIXED<32,16>,FAST_FIXED<32,16>,FAST_FIXED<32,16>", 0}, {"FAST_FIXED<32,16>,FAST_FIXED<32,16>,FIXED<16,12>", 1}, {"FAST_FIXED<32,16>,FIXED<16,12>,FAST_FIXED<32,16>", 2}, {"FAST_FIXED<32,16>,FIXED<16,12>,FIXED<16,12>", 3}, {"FIXED<16,12>,FAST_FIXED<32,16>,FAST_FIXED<32,16>", 4}, {"FIXED<16,12>,FAST_FIXED<32,16>,FIXED<16,12>", 5}, {"FIXED<16,12>,FIXED<16,12>,FAST_FIXED<32,16>", 6}, {"FIXED<16,12>,FIXED<16,12>,FIXED<16,12>", 7} };
    std::vector<SimulateVariant> arr = { Simulate<Fixed<(int_fast32_t)32, 16>,Fixed<(int_fast32_t)32, 16>,Fixed<(int_fast32_t)32, 16>>(), Simulate<Fixed<(int_fast32_t)32, 16>,Fixed<(int_fast32_t)32, 16>,Fixed<16,12>>(), Simulate<Fixed<(int_fast32_t)32, 16>,Fixed<16,12>,Fixed<(int_fast32_t)32, 16>>(), Simulate<Fixed<(int_fast32_t)32, 16>,Fixed<16,12>,Fixed<16,12>>(), Simulate<Fixed<16,12>,Fixed<(int_fast32_t)32, 16>,Fixed<(int_fast32_t)32, 16>>(), Simulate<Fixed<16,12>,Fixed<(int_fast32_t)32, 16>,Fixed<16,12>>(), Simulate<Fixed<16,12>,Fixed<16,12>,Fixed<(int_fast32_t)32, 16>>(), Simulate<Fixed<16,12>,Fixed<16,12>,Fixed<16,12>>() };

    std::string p_type;
    std::string v_type;
    std::string v_flow_type;

    for (int i = 1; i <= 3; ++i) {

        std::string arg = argv[i];


        if (arg.find("--p-type=") == 0) {
            p_type = arg.substr(9);
        } else if (arg.find("--v-type=") == 0) {
            v_type = arg.substr(9);
        } else if (arg.find("--v-flow-type=") == 0) {
            v_flow_type = arg.substr(14);
        }

    }

    std::string args_str = p_type + v_type + v_flow_type;
    replaceBrackets(args_str);

    const auto& it = params.find(args_str);
    if (it == params.end()) {
        std::cout << "No suitable params";
        exit(1);
    }

    std::visit([](auto& sim) { sim.execute(1'000'000); }, arr[it->second]);

    return 0;
}
