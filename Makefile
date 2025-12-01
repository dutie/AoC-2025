# Advent of Code C++ Template

.PHONY: all build clean scaffold run bench list help

all: build

# ------------------------
# Helper for positional args
# ------------------------

# For a given target name, extract YEAR and DAY from MAKECMDGOALS
#   $(2) is the target name (e.g., scaffold, run, example, fetch, bench)
define SET_YEAR_DAY
$1: YEAR := $(word 2,$(MAKECMDGOALS))
$1: DAY  := $(word 3,$(MAKECMDGOALS))
endef

# Check YEAR/DAY present in shell
define REQUIRE_YEAR_DAY
	@if [ -z "$(YEAR)" ] || [ -z "$(DAY)" ]; then \
		echo "Usage: make $@ <YEAR> <DAY>"; \
		exit 1; \
	fi
endef

# Dummy rule to swallow extra positional args (2025, 1, …)
# so make doesn't treat them as real targets.
%:
	@:

# ------------------------
# Build / clean
# ------------------------


# Build the project
build:
	@mkdir -p build
	@cd build && cmake .. && $(MAKE) -j$$(nproc 2>/dev/null || echo 4)

# Clean build files
clean:
	@rm -rf build
	@echo "✓ Cleaned"

# ------------------------
# Scaffold
# ------------------------

$(eval $(call SET_YEAR_DAY,scaffold))

scaffold:
	$(REQUIRE_YEAR_DAY)
	@python3 scripts/scaffold.py "$(YEAR)" "$(DAY)"

# ------------------------
# Run
# ------------------------

$(eval $(call SET_YEAR_DAY,run))

run: build
	$(REQUIRE_YEAR_DAY)
	@./build/aoc "$(YEAR)" "$(DAY)"

# ------------------------
# Example
# ------------------------

$(eval $(call SET_YEAR_DAY,example))

example: build
	$(REQUIRE_YEAR_DAY)
	@./build/aoc "$(YEAR)" "$(DAY)" --example

# ------------------------
# Bench
# ------------------------

$(eval $(call SET_YEAR_DAY,bench))

bench: build
	@if [ -n "$(YEAR)" ] && [ -n "$(DAY)" ]; then \
		./build/aoc "$(YEAR)" "$(DAY)" --bench; \
	else \
		python3 scripts/benchmark_all.py; \
	fi

# ------------------------
# Fetch
# ------------------------

$(eval $(call SET_YEAR_DAY,fetch))

fetch:
	$(REQUIRE_YEAR_DAY)
	@python3 scripts/fetch_input.py "$(YEAR)" "$(DAY)"

# ------------------------
# Misc
# ------------------------

list: build
	@./build/aoc --list

help:
	@echo "Advent of Code C++ Template"
	@echo ""
	@echo "  make build            Build project"
	@echo "  make clean            Remove build files"
	@echo "  make scaffold Y D     Create new day"
	@echo "  make fetch Y D        Download input"
	@echo "  make run Y D          Run solution"
	@echo "  make example Y D      Run with example input"
	@echo "  make bench [Y D]      Benchmark (all or one)"
	@echo "  make list             List all solutions"