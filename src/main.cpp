#include <functional>
#include <memory>
#include <string>

#include "../include/WordGraph.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"


using namespace ftxui;


Flanky::WordGraph word_graph;
std::string title_label = "Assignment 3 - Next Word Detection - 2022035";
std::string button1_label = "---";
std::string button2_label = "---";
std::string button3_label = "---";
std::string button4_label = "---";
std::string old_word;
std::string old_word_link;
std::string input_str;

std::string sanitize_string(const std::string& str)
{
    std::string result;
    for (const char ch : str)
    {
        if (ch == '\n')
        {
            result.push_back(' ');
        } else
        {
            result.push_back(ch);
        }
    }
    return result;
}

void on_input_change()
{
    if (!std::isalnum(input_str.back())) return;

    const auto sanitized_str = sanitize_string(input_str);

    const auto first_word_start = sanitized_str.find_last_of(' ') + 1;
    const auto second_word_start = sanitized_str.find_last_of(' ', first_word_start - 2) + 1;
    const auto third_word_start = sanitized_str.find_last_of(' ', second_word_start - 2) + 1;
    const auto first_word = sanitized_str.substr(first_word_start);
    const auto second_word = sanitized_str.substr(second_word_start, first_word_start - second_word_start - 1);
    const auto third_word = sanitized_str.substr(third_word_start, second_word_start - third_word_start - 1);

    if (third_word == second_word || third_word == first_word) return;

    if (old_word != third_word && old_word_link != second_word)
    {
        word_graph.createLinks(third_word, second_word);
    }

    old_word = third_word;
    old_word_link = second_word;

    const auto words = word_graph.getTopFourLinks(first_word);

    button1_label = words.empty() ? "---" : words[0];
    button2_label = words.size() >= 2 ? words[1] : "---";
    button3_label = words.size() >= 3 ? words[2] : "---";
    button4_label = words.size() >= 4 ? words[3] : "---";
}

InputOption input_option()
{
    auto option = InputOption::Default();

    option.on_change = on_input_change;

    option.transform = [&option](const InputState& state)
    {
        if (state.focused)
        {
            option.cursor_position = static_cast<int>(input_str.length());
        }

        return state.element;
    };

    return option;
}

void on_click(const Component& inputComponent, const std::string& word)
{
    if (word == "---")
    {
        return;
    }

    input_str.replace(
        input_str.length(),
        std::string::npos,
        input_str.ends_with(' ') ? word : ' ' + word
    );

    on_input_change();

    inputComponent->TakeFocus();
}

int main()
{
    auto screen = ScreenInteractive::Fullscreen();

    auto inputComponent = Input(
        &input_str,
        "Enter text here...",
        input_option()
    ) | flex;

    auto buttonComponents = Container::Horizontal({
        Button(&button1_label, [&inputComponent] { on_click(inputComponent, button1_label); }) | flex,
        Button(&button2_label, [&inputComponent] { on_click(inputComponent, button2_label); }) | flex,
        Button(&button3_label, [&inputComponent] { on_click(inputComponent, button3_label); }) | flex,
        Button(&button4_label, [&inputComponent] { on_click(inputComponent, button4_label); }) | flex,
        Button("Exit", screen.ExitLoopClosure()) | flex | size(WIDTH, LESS_THAN, 20)
    });

    const auto layout = Container::Vertical({
        inputComponent,
        buttonComponents
    });

    auto component = Renderer(layout, [&]
    {
        return vbox({
            text(title_label) | center,
            separator(),
            inputComponent->Render() | vscroll_indicator | border,
            separator(),
            buttonComponents->Render(),
        }) | flex | border;
    });

    component |= CatchEvent([&](const Event& event)
    {
        if (event == Event::Character('q'))
        {
            screen.ExitLoopClosure()();
            return true;
        }

        if (event == Event::F1)
        {
            on_click(inputComponent, button1_label);
            return true;
        }

        if (event == Event::F2)
        {
            on_click(inputComponent, button2_label);
            return true;
        }

        if (event == Event::F3)
        {
            on_click(inputComponent, button3_label);
            return true;
        }

        if (event == Event::F4)
        {
            on_click(inputComponent, button4_label);
            return true;
        }

        return false;
    });

    screen.Loop(component);

    return EXIT_SUCCESS;
}
