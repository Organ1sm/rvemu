#include "Emulator.hpp"

rvemu::Emulator::Emulator(const std::string &fileName) : cpu_(fileName) { }

void rvemu::Emulator::runEmulator() { cpu_.steps(); }
