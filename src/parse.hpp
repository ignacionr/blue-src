#pragma once
#include <regex>
#include <string>
#include <ranges>
#include <algorithm>

void parse(const std::string &html_contents, auto && callback)
{
    static const std::regex block_regex{R"(<a class="title"[^>]*>([^<]+)<\/a>\s*<div class="values">([\s\S]*?)(?:<a class="title"|<div class="tile update">|<\/div>\s*<\/div>\s*<div class="tile is-child">))"};
    static const std::regex value_regex{R"(<div class="label">([^<]+)<\/div>\s*<div class="val">([^<]+)<\/div>)"};
    
    auto blocks = std::ranges::subrange(
        std::sregex_iterator(html_contents.begin(), html_contents.end(), block_regex),
        std::sregex_iterator()
    );

    for (const auto& block_match : blocks) {
        auto raw_title = block_match[1].str();
        auto inner_content = block_match[2].str();

        // Todos los matches
        auto values = std::ranges::subrange(
            std::sregex_iterator(inner_content.begin(), inner_content.end(), value_regex),
            std::sregex_iterator()
        );

        // Convertido a pares, y filtrados los vacíos
        auto processed_values = values 
            | std::views::transform([](const auto& match) -> std::pair<std::string, std::string> {
                return {match[1].str(), match[2].str()};
            })
            | std::views::filter([](const auto& pair) { return !pair.first.empty(); });

        for (const auto& [label, value] : processed_values) {
            callback(raw_title, label, value);
        }
    }
}
