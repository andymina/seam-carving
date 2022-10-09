.PHONY: setup build clean demo cpp py js default
default: demo

setup:
	@pip install -r requirements.txt
	@git submodule update --init
	@./vcpkg/bootstrap-vcpkg.sh
	@./vcpkg/vcpkg integrate install
	@./vcpkg/vcpkg install opencv pybind11 doctest
	@mkdir build out

tests:
	@cmake -B build/ -S ./ -DSC_EXPORT=tests
	@make -C build/

demo:
	@cmake -B build/ -S ./ -DSC_EXPORT=demo
	@make -C build/

cpp:
	@cmake -B build/ -S ./ -DSC_EXPORT=cpp

py:
	@cmake -B build/ -S ./ -DSC_EXPORT=py
	@make -C build/

js:
	@cmake -B build/ -S ./ -DSC_EXPORT=js

clean:
	rm -rf build/ out/
