# ---------------------------------------------------------------
# Programmer(s): Daniel R. Reynolds @ SMU
#                Cody J. Balos @ LLNL
# ---------------------------------------------------------------
# SUNDIALS Copyright Start
# Copyright (c) 2002-2020, Lawrence Livermore National Security
# and Southern Methodist University.
# All rights reserved.
#
# See the top-level LICENSE and NOTICE files for details.
#
# SPDX-License-Identifier: BSD-3-Clause
# SUNDIALS Copyright End
# ---------------------------------------------------------------
# CMakeLists.txt file for sunlinsol dense examples
# ---------------------------------------------------------------

# Example lists are tuples "name\;args\;type" where the type is
# 'develop' for examples excluded from 'make test' in releases

# Examples using SUNDIALS dense linear solver
set(sunlinsol_dense_examples
  "test_sunlinsol_dense\;10 0\;"
  "test_sunlinsol_dense\;100 0\;"
  "test_sunlinsol_dense\;500 0\;"
  "test_sunlinsol_dense\;1000 0\;"
)

# Dependencies for nvector examples
set(sunlinsol_dense_dependencies
  test_sunlinsol
  sundials_nvector
  sundials_matrix
  sundials_linearsolver
)

# If building F2003 tests
if (F2003_INTERFACE_ENABLE)
  set(sunlinsol_dense_fortran_examples
    "test_fsunlinsol_dense_mod\;\;")
endif()

# Add source directory to include directories
include_directories(. ..)

# Specify libraries to link against (through the target that was used to
# generate them) based on the value of the variable LINK_LIBRARY_TYPE
if(LINK_LIBRARY_TYPE MATCHES "static")
  set(NVECS_LIB sundials_nvecserial_static)
  set(SUNLINSOL_LIB sundials_sunlinsoldense_static)
  if (F2003_INTERFACE_ENABLE)
    list(APPEND NVECS_LIB sundials_fnvecserial_mod_static)
    list(APPEND SUNLINSOL_LIB sundials_fsunlinsoldense_mod_static)
  endif()
else()
  set(NVECS_LIB sundials_nvecserial_shared)
  set(SUNLINSOL_LIB sundials_sunlinsoldense_shared)
  if (F2003_INTERFACE_ENABLE)
    list(APPEND NVECS_LIB sundials_fnvecserial_mod_shared)
    list(APPEND SUNLINSOL_LIB sundials_fsunlinsoldense_mod_shared)
  endif()
endif()

# Set-up linker flags and link libraries
set(SUNDIALS_LIBS ${NVECS_LIB} ${SUNLINSOL_LIB} ${EXTRA_LINK_LIBS})


# Add the build and install targets for each example
foreach(example_tuple ${sunlinsol_dense_examples})

  # parse the example tuple
  list(GET example_tuple 0 example)
  list(GET example_tuple 1 example_args)
  list(GET example_tuple 2 example_type)

  # This is used to get around DLL linkage issue since we are
  # manually including sundials_nvector.c here, which is normally in
  # a library that is included.  If this is not set build system
  # thinks nvector is externally linked.
  if(WIN32)
    add_definitions(-DBUILD_SUNDIALS_LIBRARY)
  endif(WIN32)

  # check if this example has already been added, only need to add
  # example source files once for testing with different inputs
  if(NOT TARGET ${example})
    # example source files
    add_executable(${example} ${example}.c
      ../test_sunlinsol.c
      ../../../src/sundials/sundials_matrix.c
      ../../../src/sundials/sundials_linearsolver.c
      ../../../src/sundials/sundials_nvector.c)

    # folder to organize targets in an IDE
    set_target_properties(${example} PROPERTIES FOLDER "Examples")

    # libraries to link against
    target_link_libraries(${example} ${SUNDIALS_LIBS})
  endif()

  # check if example args are provided and set the test name
  if("${example_args}" STREQUAL "")
    set(test_name ${example})
  else()
    string(REGEX REPLACE " " "_" test_name ${example}_${example_args})
  endif()

  # add example to regression tests
  sundials_add_test(${test_name} ${example}
    TEST_ARGS ${example_args}
    EXAMPLE_TYPE ${example_type}
    NODIFF)

  if(EXAMPLES_INSTALL)
    install(FILES ${example}.c
      ../test_sunlinsol.h
      ../test_sunlinsol.c
      ../../../src/sundials/sundials_matrix.c
      ../../../src/sundials/sundials_linearsolver.c
      ../../../src/sundials/sundials_nvector.c
      DESTINATION ${EXAMPLES_INSTALL_PATH}/sunlinsol/dense)
  endif()

endforeach(example_tuple ${sunlinsol_dense_examples})

# Add the build and install targets for each example
foreach(example_tuple ${sunlinsol_dense_fortran_examples})

  # parse the example tuple
  list(GET example_tuple 0 example)
  list(GET example_tuple 1 example_args)
  list(GET example_tuple 2 example_type)

  # This is used to get around DLL linkage issue since we are
  # manually including sundials_nvector.c here, which is normally in
  # a library that is included.  If this is not set build system
  # thinks nvector is externally linked.
  if(WIN32)
    add_definitions(-DBUILD_SUNDIALS_LIBRARY)
  endif(WIN32)

  # check if this example has already been added, only need to add
  # example source files once for testing with different inputs
  if(NOT TARGET ${example})
    # example source files
    add_executable(${example} ${example}.f90
      ${sundials_SOURCE_DIR}/examples/test_utilities.f90
      ${sundials_SOURCE_DIR}/examples/sunlinsol/test_sunlinsol.f90
      $<TARGET_OBJECTS:sundials_generic_${LINK_LIBRARY_TYPE}_obj>
      $<TARGET_OBJECTS:sundials_fgeneric_${LINK_LIBRARY_TYPE}_obj>)

    # folder to organize targets in an IDE
    set_target_properties(${example} PROPERTIES FOLDER "Examples")

    # set fortran module directory to avoid name collisions
    set_target_properties(${example} PROPERTIES Fortran_MODULE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    # libraries to link against
    target_link_libraries(${example} ${SUNDIALS_LIBS})

    # check if example args are provided and set the test name
    if("${example_args}" STREQUAL "")
      set(test_name ${example})
    else()
      string(REGEX REPLACE " " "_" test_name ${example}_${example_args})
    endif()

    # add example to regression tests
    sundials_add_test(${test_name} ${example}
      TEST_ARGS ${example_args}
      EXAMPLE_TYPE ${example_type}
      NODIFF)
  endif()

endforeach(example_tuple ${sunlinsol_dense_fortran_examples})

if(EXAMPLES_INSTALL)

  # Install the README file
  install(FILES DESTINATION ${EXAMPLES_INSTALL_PATH}/sunlinsol/dense)

  # Prepare substitution variables for Makefile and/or CMakeLists templates
  set(SOLVER_LIB "sundials_sunlinsoldense")
  set(LIBS "${LIBS} -lsundials_sunmatrixdense")

  # Set the link directory for the dense sunmatrix library
  # The generated CMakeLists.txt does not use find_library() locate it
  set(EXTRA_LIBS_DIR "${libdir}")

  examples2string(sunlinsol_dense_examples EXAMPLES)
  examples2string(sunlinsol_dense_dependencies EXAMPLES_DEPENDENCIES)

  # Regardless of the platform we're on, we will generate and install
  # CMakeLists.txt file for building the examples. This file  can then
  # be used as a template for the user's own programs.

  # generate CMakelists.txt in the binary directory
  configure_file(
    ${PROJECT_SOURCE_DIR}/examples/templates/cmakelists_serial_C_ex.in
    ${PROJECT_BINARY_DIR}/examples/sunlinsol/dense/CMakeLists.txt
    @ONLY
    )

  # install CMakelists.txt
  install(
    FILES ${PROJECT_BINARY_DIR}/examples/sunlinsol/dense/CMakeLists.txt
    DESTINATION ${EXAMPLES_INSTALL_PATH}/sunlinsol/dense
    )

  # On UNIX-type platforms, we also  generate and install a makefile for
  # building the examples. This makefile can then be used as a template
  # for the user's own programs.

  if(UNIX)
    # generate Makefile and place it in the binary dir
    configure_file(
      ${PROJECT_SOURCE_DIR}/examples/templates/makefile_serial_C_ex.in
      ${PROJECT_BINARY_DIR}/examples/sunlinsol/dense/Makefile_ex
      @ONLY
      )
    # install the configured Makefile_ex as Makefile
    install(
      FILES ${PROJECT_BINARY_DIR}/examples/sunlinsol/dense/Makefile_ex
      DESTINATION ${EXAMPLES_INSTALL_PATH}/sunlinsol/dense
      RENAME Makefile
      )
  endif(UNIX)

endif(EXAMPLES_INSTALL)
