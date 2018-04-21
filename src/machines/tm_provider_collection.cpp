#include "tm_provider_collection.hpp"
#include "multiply_machine_provider.hpp"
#include "subtract_machine_provider.hpp"
#include "sum_machine_provider.hpp"

using namespace realmar::turing;

#define REGISTER_PROVIDER(name) _providers.emplace_back(std::make_shared<name##_machine_provider>());

const provider_list& tm_provider_collection::get_providers() const {
	return _providers;
}

tm_provider_collection::tm_provider_collection() {
	REGISTER_PROVIDER(multiply)
		REGISTER_PROVIDER(subtract)
		REGISTER_PROVIDER(sum)
}
