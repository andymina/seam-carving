.PHONY: build setup clean

build:
	@make -C build/
	@echo "Project built!"

setup:
	@pip install -r requirements.txt
	@git submodule update --init
	@mkdir build out
	@cmake -B build/ -S ./

clean:
	rm -rf build/ out/