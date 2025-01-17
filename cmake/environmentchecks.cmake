# Copyright libCellML Contributors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.cmake_minimum_required (VERSION 3.1)

include(CheckCXXCompilerFlag)

get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

find_package(Python ${PREFERRED_PYTHON_VERSION} COMPONENTS Interpreter Development)

find_program(CLANG_FORMAT_EXE NAMES ${PREFERRED_CLANG_FORMAT_NAMES} clang-format)
find_program(CLANG_TIDY_EXE NAMES ${PREFERRED_CLANG_TIDY_NAMES} clang-tidy)
find_program(FIND_EXE NAMES ${PREFERRED_FIND_NAMES} find)
find_program(GCOV_EXE NAMES ${PREFERRED_GCOV_NAMES} gcov)
find_program(GIT_EXE NAMES ${PRFERRED_GIT_NAMES} git)
find_program(LLVM_COV_EXE NAMES ${PREFERRED_LLVM_COV_NAMES} llvm-cov HINTS ${LLVM_BIN_DIR} /Library/Developer/CommandLineTools/usr/bin/)
find_program(LLVM_PROFDATA_EXE NAMES ${PREFERRED_LLVM_PROFDATA_NAMES} llvm-profdata HINTS ${LLVM_BIN_DIR} /Library/Developer/CommandLineTools/usr/bin/)
find_program(VALGRIND_EXE NAMES ${PREFERRED_VALGRIND_NAMES} valgrind)

find_package(Doxygen)
find_package(Sphinx)
find_package(SWIG 3)

set(_ORIGINAL_CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS})

set(CMAKE_REQUIRED_FLAGS -fprofile-instr-generate)
check_cxx_compiler_flag("-fprofile-instr-generate -fcoverage-mapping" LLVM_COVERAGE_COMPILER_FLAGS_OK)

set(CMAKE_REQUIRED_FLAGS "-fprofile-arcs -ftest-coverage")
check_cxx_compiler_flag("-fprofile-arcs -ftest-coverage" GCC_COVERAGE_COMPILER_FLAGS_OK)

set(CMAKE_REQUIRED_FLAGS ${_ORIGINAL_CMAKE_REQUIRED_FLAGS})

mark_as_advanced(
  CLANG_TIDY_EXE
  CLANG_FORMAT_EXE
  FIND_EXE
  GCC_COVERAGE_COMPILER_FLAGS_OK
  GCOV_EXE
  GIT_EXE
  LLVM_COV_EXE
  LLVM_COVERAGE_COMPILER_FLAGS_OK
  LLVM_PROFDATA_EXE
  SWIG_EXECUTABLE
  VALGRIND_EXE
)

# Find libxml2
set(HAVE_LIBXML2_CONFIG FALSE)
if(MSVC)
  # If we want to use config packages on Windows with Visual Studio,
  # we need to have two find_package calls and explicitly state that
  # we wish to use Config mode in the first call.  Finding LibXml2 in config mode
  # is the preferred method so we will try this first quietly.
  #
  # This does change how we get information about include paths and such so we
  # need to track how we found LibXml2.
  find_package(LibXml2 CONFIG QUIET)
  if(LibXml2_FOUND)
    set(HAVE_LIBXML2_CONFIG TRUE)
    foreach(_XML2_VAR LIBXML2_LIBRARY LIBXML2_INCLUDE_DIR LIBXML2_XMLLINT_EXECUTABLE)
      if(DEFINED ${_XML2_VAR} AND NOT ${${_XML2_VAR}})
        unset(${_XML2_VAR} CACHE)
      endif()
    endforeach()
  else()
    find_package(LibXml2 REQUIRED)
    if(LibXml2_FOUND)
      unset(LibXml2_DIR CACHE)
    endif()
  endif()
else()
  find_package(LibXml2 REQUIRED)
endif()

if(CLANG_FORMAT_EXE AND GIT_EXE)
  set(CLANG_FORMAT_TESTING_AVAILABLE TRUE INTERNAL BOOL "Executables required to run the ClangFormat test are available.")
endif()

if(CLANG_TIDY_EXE)
  set(CLANG_TIDY_AVAILABLE TRUE INTERNAL BOOL "Executable required to perform static analysis is available.")
endif()

if(FIND_EXE AND GCOV_EXE AND Python_Interpreter_FOUND AND GCC_COVERAGE_COMPILER_FLAGS_OK)
  set(COVERAGE_TESTING_AVAILABLE TRUE INTERNAL BOOL "Executables required to run the coverage testing are available.")
endif()

if(SWIG_EXECUTABLE)
  set(BINDINGS_AVAILABLE TRUE INTERNAL BOOL "Executable required to generate bindings is available.")
endif()

if(VALGRIND_EXE AND Python_Interpreter_FOUND)
  set(VALGRIND_TESTING_AVAILABLE TRUE INTERNAL BOOL "Executable required to run valgrind testing is available.")
endif()

if(LLVM_PROFDATA_EXE AND LLVM_COV_EXE AND FIND_EXE AND LLVM_COVERAGE_COMPILER_FLAGS_OK)
  set(LLVM_COVERAGE_TESTING_AVAILABLE TRUE INTERNAL BOOL "Executables required to run the llvm coverage testing are available.")
endif()
