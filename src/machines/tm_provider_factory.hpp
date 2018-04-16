#pragma once

#include <vector>
#include <memory>

namespace realmar::turing {
    class abstract_tm_provider;

    typedef std::vector<std::shared_ptr<abstract_tm_provider>> provider_list;

    class tm_provider_factory {
    private:
        // https://stackoverflow.com/questions/8057682/accessing-a-static-map-from-a-static-member-function-segmentation-fault-c?rq=1
        static provider_list& get_providers_internal() {
            static provider_list providers;
            return providers;
        }

    public:
        static void register_provider(const std::shared_ptr<abstract_tm_provider>& provider);

        static const provider_list& get_providers();
    };
}

#include "abstract_tm_provider.hpp"