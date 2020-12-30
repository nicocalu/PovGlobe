#pragma once

#include "helper.hpp"

#include <chrono>
using namespace std::chrono_literals;

class Globe;

class ApplicationBase {


public:
	virtual void initialize(Globe& globe);

	virtual void process(Framebuffer& framebuffer, float time);

	virtual std::chrono::duration<float, std::milli> getTargetCycleTime() const;

private:
	int m_last_pixel;
};