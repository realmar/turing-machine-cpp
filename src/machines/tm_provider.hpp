#pragma once

#include "abstract_tm_provider.hpp"
#include "../turing_machine.hpp"
#include "../presentation/executor_runner.hpp"
#include "../presentation/input_executor_runner.hpp"

namespace realmar::turing {
	template<int N, typename T>
	class tm_provider : public abstract_tm_provider {
	public:
		virtual turing_machine<N, T> get_tm() = 0;

		std::shared_ptr<abstract_turing_machine> get_abstract_tm() override {
			return std::make_shared<turing_machine<N, T>>(get_tm());
		}

		std::shared_ptr<executor_runner> get_runner() override {
			auto tm = get_tm();
			return std::make_shared<input_executor_runner<N, T>>(tm);
		}
	};
}