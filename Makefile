export_targets = demo cpp py
test_targets = tests load-tests gen-tests
.PHONY: setup build clean $(targets)

default: demo

setup:
	@pip install -r requirements.txt
	@git submodule update --init
	@./vcpkg/bootstrap-vcpkg.sh
	@./vcpkg/vcpkg integrate install
	@./vcpkg/vcpkg install opencv pybind11 doctest nlohmann-json
	@mkdir build out

$(export_targets) $(test_targets):
	@cmake -B build/ -S ./ -DSC_EXPORT=$@
	@make -C build/

clean:
	@rm -rf build/ out/
