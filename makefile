# Makefile for Orion-X3/Orion-X4/mx and derivatives
# Written in 2011
# This makefile is licensed under the WTFPL


WARNINGS        = -Wno-padded -Wno-cast-align -Wno-unreachable-code -Wno-switch-enum -Wno-packed -Wno-missing-noreturn -Wno-float-equal -Wno-unused-macros -Werror=return-type -Wextra -Wno-unused-parameter -Wno-trigraphs

COMMON_CFLAGS   = -Wall

CXXFLAGS        = $(COMMON_CFLAGS) -Wno-old-style-cast -std=c++17 -fno-exceptions -fno-rtti -ferror-limit=0

CXXSRC          = $(shell find source -iname "*.cpp" -print)
CXXOBJ          = $(CXXSRC:.cpp=.cpp.o)
CXXDEPS         = $(CXXOBJ:.o=.d)

INCLUDES        = -Isource/include

.PHONY: all clean
.DEFAULT_GOAL = all


all: build/instrad_test
	@build/instrad_test build/samples/player.bin

build/instrad_test: $(CXXOBJ)
	@$(CXX) $(CXXFLAGS) -o $@ $^

%.cpp.o: %.cpp makefile
	@echo "  $(notdir $<)"
	@$(CXX) $(CXXFLAGS) $(WARNINGS) $(INCLUDES) -MMD -MP -c -o $@ $<

clean:
	@find source -iname "*.cpp.d" | xargs rm
	@find source -iname "*.cpp.o" | xargs rm

-include $(CXXDEPS)













