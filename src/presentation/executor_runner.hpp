#pragma once

class executor_runner {
public:
	virtual ~executor_runner() = default;
	virtual void run() = 0;
};