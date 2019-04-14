TEX_TARGET = sample # Add file to be pdflatexed here
TEX_SOURCE_DIR = doc/paper
TEX_OUTPUT_DIR = pdf
TEX_PDF = $(patsubst %, $(TEX_OUTPUT_DIR)/%.pdf, $(TEX_TARGET))

auto: help

help: FORCE
	@ echo "This is a Makefile for NASH project. You have the following options:"
	@ echo "- make setup: \tSetup the basic infrastructure of the project;"
	@ echo "- make format: \tFormat all the files;"
	@ echo "- make doc: \tCompile all the documents in \`doc/\`;"
	@ echo "- make clean: \tClean up the compiled files;"
	@ echo "- make help: \tPrint this help message."

setup: setup-pre-commit

format: format-lib format-app format-test

format-lib:
	@ echo "Formatting nash library files"
	@ find ./src/intern -name '*.cpp' | xargs -I '{}' clang-format -i '{}'
	@ find ./src/extern -name '*.h' | xargs -I '{}' clang-format -i '{}'
	
format-app:
	@ echo "Formatting nash application sources"
	@ find ./app -name '*.cpp' | xargs -I '{}' clang-format -i '{}'
	
format-test:
	@ echo "Formatting nash test sources"
	@ find ./test -name '*.cpp' | xargs -I '{}' clang-format -i '{}'

setup-pre-commit:
	@ echo "Copying pre-commit hook to \`.git/hooks/\`."
	@ cp ./script/hooks/pre-commit .git/hooks/pre-commit

remove-pre-commit:
	@ echo "Removing pre-commit hook from \`.git/hooks/\`."
	@ rm -f .git/hooks/pre-commit

doc: $(TEX_PDF)

$(TEX_OUTPUT_DIR)/%.pdf: $(TEX_SOURCE_DIR)/%.tex
	@ pdflatex $<
	@ mkdir -p $(TEX_OUTPUT_DIR)/
	@ mv $(*F).pdf $(TEX_OUTPUT_DIR)/$(*F).pdf

clean: clean-build clean-doc

clean-build:
	@ echo "Cleaning \`build/\` directory"
	@ rm -rf build/

clean-doc:
	@ echo "Cleaning compiled docs in \`doc/\` directory"
	@ rm -f *.aux *.log
	@ rm -rf $(TEX_OUTPUT_DIR)

FORCE:
