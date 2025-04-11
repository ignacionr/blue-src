#include <print>
#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>

#include "fetch.hpp"
#include "parse.hpp"

std::string clean_category(std::string_view category) {
    std::string_view base_result {};
    if (category.starts_with("D&#xF3;lar ")) {
        base_result = category.substr(11);
    }
    else {
        base_result = category;
    }
    std::string result;
    for (char c : base_result) {
        if (!std::isalpha(c) && !std::isdigit(c)) {
            result += '_';
        }
        else if (std::isupper(c)) {
            result += static_cast<char>(std::tolower(c));
        }
        else {
            result += c;
        }
    }
    return result;
}

std::string clean_number(std::string_view number) {
    std::string result;
    for (char c : number) {
        if (std::isdigit(c) || c == '.') {
            result += c;
        }
        else if (c == ',') {
            result += '.';
        }
    }
    return result;
}

int main(int argc, char *argv[]) {
    bool save_to_files = false;
    if (argc > 1) {
        std::string arg1 = argv[1];
        if (arg1 == "--save" || arg1 == "-s") {
            save_to_files = true;
        }
    }
    auto html_content = fetch::get("https://dolarhoy.com/");
    if (html_content.has_value()) {
        std::map<std::string, std::map<std::string, double>> data_map;
        parse(
            html_content.value(), 
            [&data_map](std::string_view title, std::string_view label, std::string_view value) {
                auto cleaned_title = clean_category(title);
                auto cleaned_label = clean_category(label);
                auto cleaned_value = clean_number(value);
                if (!cleaned_value.empty()) {
                    double numeric_value = std::strtod(cleaned_value.c_str(), nullptr);
                    data_map[cleaned_title][cleaned_label] = numeric_value;
                }
            });
        for (const auto& [title, values] : data_map) {
            std::println("{}", title);
            for (const auto& [label, value] : values) {
                std::println("  {}: {}", label, value);
            }
            if (save_to_files) {
                std::ofstream file(title);
                if (file.is_open()) {
                    std::print(file, "{{ \"title\": \"{}\", \"values\": {{", title);
                    bool first = true;
                    for (const auto& [label, value] : values) {
                        if (!first) {
                            std::print(file, ", ");
                        }
                        first = false;
                        std::print(file, "  \"{}\": {}", label, value);
                    }
                    std::print(file, "}} }}\n");
                } else {
                    std::cerr << "Error opening file: " << title + ".txt" << "\n";
                }
            }
        }
    } else {
        std::print("Error fetching HTML content: {}\n", html_content.error());
    }
    return 0;
}
