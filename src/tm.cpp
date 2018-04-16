#include "presentation/utils.hpp"
#include "presentation/user_input.hpp"
#include "presentation/executor_runner.hpp"
#include "machines/tm_provider_factory.hpp"

using namespace realmar::turing;

int main() {
    print_copyright();

    auto providers = tm_provider_factory::get_providers();
    std::vector<std::string> machine_names;
    for (auto&& provider : providers)
        machine_names.emplace_back(provider->get_tm_name());

    auto machine_i = get_user_choice(machine_names);

    auto executor = providers.at(machine_i)->get_runner();
    executor->run();

    return 0;
}
