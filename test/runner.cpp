#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#include <iostream>

int main(int argc, char **argv) {
	doctest::Context context;

	int res = context.run();

	if (context.shouldExit())
		return res;

	return res;
}