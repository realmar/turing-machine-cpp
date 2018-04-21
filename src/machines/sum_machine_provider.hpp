#pragma once

#include "tm_provider.hpp"

namespace realmar::turing {
	class sum_machine_provider : public tm_provider<1, int> {
	public:
		std::string get_tm_name() override;
		turing_machine<1, int> get_tm() override;
	};
}