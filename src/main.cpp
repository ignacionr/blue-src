#include <print>
#include <string>

#include "fetch.hpp"
#include "parse.hpp"

int main() {
    auto html_content = fetch::get("https://dolarhoy.com/");
    if (html_content.has_value()) {
        parse(
            html_content.value(), 
            [](std::string_view title, std::string_view label, std::string_view value) {
                std::print("{}, {}: {}\n", 
                    title.starts_with("D&#xF3;lar ") ? title.substr(11) : title,
                    label, value);
            });
    } else {
        std::print("Error fetching HTML content: {}\n", html_content.error());
    }
    return 0;
}
