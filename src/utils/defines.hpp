#pragma once

#define IS_IN_VECTOR(v, x) (std::find((v).begin(), (v).end(), x) != (v).end())

#define WINDOWS  _WIN64 || _WIN32 || WIN32 || __WIN32 && !__CYGWIN__