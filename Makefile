.PHONY: setup build clean demo cpp py js

setup:
	@pip install -r requirements.txt
	@git submodule update --init
	@./vcpkg/bootstrap-vcpkg.sh
	@./vcpkg/vcpkg integrate install
	@./vcpkg/vcpkg install opencv pybind11
	@mkdir build out

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

build:
	@make -C build/
	@echo "Project built!"

clean:
	rm -rf build/ out/
