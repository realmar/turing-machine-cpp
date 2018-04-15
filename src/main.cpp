#include <memory>
#include "machines/subtract_machine_provider.hpp"
#include "presentation/input_executor.hpp"
#include "machines/sum_machine_provider.hpp"
#include "machines/multiply_machine_provider.hpp"

using namespace realmar::turing;

int main() {
    auto machine_i = get_user_choice({
                                             "sum machine",
                                             "subtract machine",
                                             "multiply machine"
                                     });

    std::cout << "Enter the two numbers on which the operation should be performed" << std::endl;
    std::cout << "Enter first number: " << std::endl;
    auto a = get_number();

    std::cout << "Enter second number: " << std::endl;
    auto b = get_number();

    std::shared_ptr<generic_input_executor> executor;
    switch (machine_i) {
        case 0: {
            sum_machine_provider provider(a, b);
            auto tm = provider.get();
            executor = std::make_shared<input_executor<1, int>>(tm);
        }
            break;
        case 1: {
            subtract_machine_provider provider(a, b);
            auto tm = provider.get();
            executor = std::make_shared<input_executor<2, int>>(tm);
        }
            break;
        case 2: {
            multiply_machine_provider provider(a, b);
            auto tm = provider.get();
            executor = std::make_shared<input_executor<3, int>>(tm);
        }
            break;
        default:
            break;
    }

    executor->run();

    return 0;
}
