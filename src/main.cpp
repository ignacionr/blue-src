#include <print>
#include <string>
#include <cstdlib>

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

int main() {
    auto html_content = fetch::get("https://dolarhoy.com/");
    if (html_content.has_value()) {
        parse(
            html_content.value(), 
            [](std::string_view title, std::string_view label, std::string_view value) {
                std::print("{}, {}: {}\n", 
                    clean_category(title),
                    clean_category(label), 
                    clean_number(value));
            });
    } else {
        std::print("Error fetching HTML content: {}\n", html_content.error());
    }
    return 0;
}
