export_targets = demo cpp py
test_targets = test load-tests gen-tests
targets = $(export_targets) $(test_targets)
vcpkg_dependencies = opencv pybind11 gtest nlohmann-json
.PHONY: setup build clean $(targets)

default: demo

setup:
	@pip install -r requirements.txt
	@git submodule update --init
	@./vcpkg/bootstrap-vcpkg.sh
	@./vcpkg/vcpkg integrate install
	@./vcpkg/vcpkg install $(vcpkg_dependencies)
	@mkdir build out

$(targets):
	@cmake -B build/ -S ./ -DSC_EXPORT=$@ -DSC_TEST_TARGET=$(TEST_TARGET)
	@make -C build/

clean:
	@rm -rf build/ out/

