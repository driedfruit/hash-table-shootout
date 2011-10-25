RAW_TARGETS=$(shell cat Targets YourTargets)

BUILD_TARGETS=$(RAW_TARGETS:%=build/%)

YOUR_RAW_TARGETS=$(shell cat YourTargets)

YOUR_BUILD_TARGETS=$(YOUR_RAW_TARGETS:%=build/%)

make:
	@echo "*** Select a build target:"
	@echo ""
	@echo " all: $(BUILD_TARGETS)"
	@echo " benchmark: (re)run a benchmark"
	@echo " charts: make beautiful charts"
	@echo " mine: $(YOUR_BUILD_TARGETS)"
	@echo " my-benchmark: rebuild Your targets and (re)run a benchmark"

build:
	mkdir build

all: build $(BUILD_TARGETS)

clean:
	rm $(BUILD_TARGETS)

output: $(BUILD_TARGETS)
	python bench.py

charts: output
	python make_chart_data.py < output | python make_html.py

benchmark:
	rm -f output
	make output

mine: $(YOUR_BUILDS)

my-benchmark:
	@rm $(YOUR_BUILD_TARGETS) output
	make $(YOUR_BUILD_TARGETS)
	make charts

include Makefile.rules
