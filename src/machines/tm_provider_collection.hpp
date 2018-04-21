#pragma once

#include <vector>
#include <memory>

namespace realmar::turing {
	class abstract_tm_provider;

	typedef std::vector<std::shared_ptr<abstract_tm_provider>> provider_list;

	class tm_provider_collection {
	private:
		provider_list _providers;
	public:
		virtual ~tm_provider_collection() = default;
		tm_provider_collection();
		const provider_list& get_providers() const;
	};
}

#include "abstract_tm_provider.hpp"