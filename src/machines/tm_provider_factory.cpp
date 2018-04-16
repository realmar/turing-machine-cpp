#include "tm_provider_factory.hpp"

using namespace realmar::turing;

void realmar::turing::tm_provider_factory::register_provider(const std::shared_ptr<abstract_tm_provider>& provider) {
    get_providers_internal().emplace_back(provider);
}

const provider_list& realmar::turing::tm_provider_factory::get_providers() {
    return get_providers_internal();
}
