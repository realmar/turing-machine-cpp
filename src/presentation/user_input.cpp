#include <iostream>
#include "user_input.hpp"

void realmar::turing::print_invalid_input() {
	std::cout << "invalid input" << std::endl;
}

int realmar::turing::get_user_choice(std::vector<std::string> choices) {
	long input_i = -1;

	while (input_i == -1) {
		std::cout << "Available commands:" << std::endl;
		auto i = 0;
		for (auto&& choice : choices)
			std::cout << "    " << i++ << " : " << choice << std::endl;
		std::cout << std::endl;

		input_i = get_positive_number();
		const auto choices_size = static_cast<long>(choices.size());
		if (input_i > choices_size - 1) {
			print_invalid_input();
			input_i = -1;
		}
	}

	return input_i;
}

int realmar::turing::get_number() {
	auto input_i = -1;
	auto valid_input = false;
	while (!valid_input) {
		std::string input;
		std::cin >> input;

		try {
			input_i = std::stoi(input);
			valid_input = true;
		} catch (const std::invalid_argument& e) {
			std::cout << "Enter a valid number: " << e.what() << std::endl;
		}
	}

	return input_i;
}

int realmar::turing::get_positive_number() {
	auto input_i = -1;
	while (input_i == -1) {
		std::cout << "Enter a positive number: (0 is allowed)" << std::endl;
		input_i = get_number();
		if (input_i < 0) {
			std::cout << "Given number is negative" << std::endl;
			input_i = -1;
		}
	}

	return input_i;
}

