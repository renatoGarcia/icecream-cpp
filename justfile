# Windows has no `sh`, so run recipes with PowerShell there. Every command below is a single
# line and only uses `cmake` itself for file operations, so it works in both shells.
set windows-shell := ["powershell.exe", "-NoLogo", "-Command"]

# justfile_directory() uses backslashes on Windows, and CPM writes the path into a generated CMake
# script, where `\a` in `D:\a\...` is an invalid escape. CMake accepts forward slashes on every OS.
source_dir := replace(justfile_directory(), "\\", "/")

# Build files go under the repository (already ignored by `/build*/`), not under /tmp,
# so the paths exist on every OS.
work_dir := source_dir / "build-consumer-tests"
build_dir := work_dir / "cpm"
build_dir_exe := work_dir / "cpm-exe"
build_dir_inst := work_dir / "installed"
install_build_dir := work_dir / "install-build"
install_prefix := work_dir / "install-prefix"

# Boost is hidden from CMake so these tests do not depend on whether the machine (a dev shell or a
# CI runner) happens to have it installed. Nothing in a consumer should need it.
no_boost := "-DCMAKE_DISABLE_FIND_PACKAGE_Boost=ON"

# Show the available recipes (default when running `just` with no arguments)
default:
    @just --list

# Seam 1: CPM consumer with BUILD_TESTING=ON and no Boost must configure
test-cpm:
    cmake -E rm -rf "{{build_dir}}"
    cmake -S consumer_tests/cpm -B "{{build_dir}}" "-DCPM_icecream-cpp_SOURCE={{source_dir}}" -DBUILD_TESTING=ON {{no_boost}}

# Seam 2: same CPM consumer must also build against icecream-cpp::icecream-cpp
test-cpm-build:
    cmake -E rm -rf "{{build_dir_exe}}"
    cmake -S consumer_tests/cpm -B "{{build_dir_exe}}" "-DCPM_icecream-cpp_SOURCE={{source_dir}}" -DBUILD_TESTING=ON {{no_boost}} -DCONSUMER_LINK_TARGET=ON
    cmake --build "{{build_dir_exe}}" --config Release

# Seam 3: install Icecream-cpp, then build a find_package() consumer against it
test-installed:
    cmake -E rm -rf "{{build_dir_inst}}" "{{install_build_dir}}" "{{install_prefix}}"
    cmake -S . -B "{{install_build_dir}}"
    cmake --install "{{install_build_dir}}" --prefix "{{install_prefix}}" --config Release
    cmake -S consumer_tests/installed -B "{{build_dir_inst}}" "-DCMAKE_PREFIX_PATH={{install_prefix}}"
    cmake --build "{{build_dir_inst}}" --config Release

# Run all the consumer tests
test-all: test-cpm test-cpm-build test-installed
