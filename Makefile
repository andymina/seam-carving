.PHONY: setup build clean

setup:
	@git submodule update --init
	@./vcpkg/bootstrap-vcpkg.sh
	@./vcpkg/vcpkg install opencv pybind11
	@mkdir build out
	@cmake -B build/ -S ./

build:
	@make -C build/
	@echo "Project built!"

clean:
	rm -rf build/ out/
