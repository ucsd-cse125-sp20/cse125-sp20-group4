#pragma once
#include "phases/phase.h"
#include <memory>
class PhaseFactory
{
public:
    static std::shared_ptr<Phase> create(std::string serial);
};