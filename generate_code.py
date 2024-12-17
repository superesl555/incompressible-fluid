import sys
import re

cpp_template = """
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

using SimulateVariant = std::variant<{{types}}>;

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
    
    std::unordered_map<std::string, int> params = { {{params}} };
    std::vector<SimulateVariant> arr = { {{types_vec}} };

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
"""

if len(sys.argv) < 2:
    print("Usage: python generate_code.py <TYPES>")
    sys.exit(1)

def replace_fast_fixed(input_string):
    input_string = input_string.replace("FIXED", "Fixed")
    def replacement(match):
        N = int(match.group(1))
        K = match.group(2)

        if N <= 8:
            int_type = f"(int_fast8_t){N}"
        elif N <= 16:
            int_type = f"(int_fast16_t){N}"
        elif N <= 32:
            int_type = f"(int_fast32_t){N}"
        elif N <= 64:
            int_type = f"(int_fast64_t){N}"
        else:
            int_type = f"(int_fast128_t){N}"

        return f"Fixed<{int_type}, {K}>"

    output_string = re.sub(r'FAST_Fixed<(\d+),\s*(\d+)>', replacement, input_string)
    return output_string

def parse_types(input_string):
    type_strings = re.findall(r'FAST_FIXED\(\d+,\d+\)|FIXED\(\d+,\d+\)|double|float', input_string)
    types = []

    for type_string in type_strings:
        if type_string.startswith("FAST_FIXED"):
            types.append(type_string)
        elif type_string.startswith("FIXED"):
            types.append(type_string)
        elif type_string == "double":
            types.append("double")
        elif type_string == "float":
            types.append("float")
        else:
            print(f"Unknown type: {type_string}")

    return types


types_value = sys.argv[1]
parsed_types = parse_types(types_value)

parsed_types = list(set(parsed_types))

for i in range(len(parsed_types)):
    parsed_types[i] = parsed_types[i].replace('(', '<').replace(')', '>')


groups = []
if (len(parsed_types) > 0):
    for i in range(len(parsed_types)):
        for j in range(len(parsed_types)):
            for k in range(len(parsed_types)):
                groups.append(parsed_types[i] + "," + parsed_types[j] + "," + parsed_types[k])


params_value = ', '.join(f'{{"{param}", {i}}}' for i, param in enumerate(groups))

groups_modify = []
for element in groups:
    groups_modify.append(replace_fast_fixed(element))
types_value = ', '.join(f'Simulate<{t}>' for t in groups_modify)
types_value_vec = ', '.join(f'Simulate<{t}>()' for t in groups_modify)


rendered_code = cpp_template.replace("{{types}}", types_value).replace("{{params}}", params_value).replace("{{types_vec}}", types_value_vec)


with open("main.cpp", "w") as cpp_file:
    cpp_file.write(rendered_code)

print(f"Сгенерирован файл main.cpp с параметрами TYPES: {types_value} и PARAMS: {params_value}")