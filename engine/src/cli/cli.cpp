#include "zeus/cli/cli.hpp"

#include <type_traits>
#include <queue>
#include <regex>
#include <algorithm>
#include <utility>
#include <sstream>
#include <deque>

namespace Zeus {

namespace CLI {

namespace {

std::regex const long_option{R"(--([A-Z]|[a-z]){1}(-?([0-9]|[A-Z]|[a-z])+)+(=.+)?)",
                            std::regex_constants::optimize};

std::regex const short_option{R"(-([A-Z]|[a-z])+)", 
                            std::regex_constants::optimize};

}

[[nodiscard]] bool isLongOption(std::string_view text) noexcept {
    return std::regex_search(text.data(), long_option,
                std::regex_constants::match_continuous);
}


[[nodiscard]] bool isShortOption(std::string_view text) noexcept {
    return std::regex_search(text.data(), short_option,
                std::regex_constants::match_continuous);
}

[[nodiscard]] std::pair<std::string, Internal::Token> nextToken(
        std::string_view argument) noexcept {

}

[[nodiscard]] std::deque<Internal::Token> lex(
        std::vector<std::string_view>::iterator begin,
        std::vector<std::string_view>::iterator end) noexcept {
    std::deque<Internal::Token> tokens;

    auto current_position = begin;

    while (current_position != end) {
        auto argument = *current_position;

        while (!argument.empty()) {
            Internal::Token token;
            std::tie(argument, token) = nextToken(argument);

            tokens.emplace_back(std::move(token));
        }

        std::next(current_position);
    }

    return tokens;
}

CommandLine parseCommandLine(int argc, char* argv[]) noexcept {
    CommandLine command_line;
    command_line.program_name = argv[0];

    // 1st argument is the program's name
    std::vector<std::string_view> views(argc - 1);

    for (std::size_t i = 1; i < std::size(views); ++i) {
        views[i] = argv[i];
    }

    auto tokens = lex(std::begin(views), std::end(views));

    return command_line;
}

} // namespace CLI

std::string to_string(CLI::Option const& option) noexcept {
    auto str = option.full_name + " = ";

    // TODO: FIX SO VALUE IS ALSO RETURNED
    return str;
}

} // namespace Zeus
