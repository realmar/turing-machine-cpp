#pragma once

#include "tm_step_executor.hpp"

namespace realmar::turing {
	template<int N, typename T>
	class tm_auto_executor : public tm_step_executor<N, T> {
	public:
		explicit tm_auto_executor(turing_machine<N, T>& tm) : tm_step_executor<N, T>(tm) {}

		execution_result execute_to_finish() {
			auto result = not_finished;
			while (result == not_finished) {
				result = this->next();
			}

			return result;
		}
	};
}
