PROJECT(TOOLS)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

set(PROJECTS_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/../")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Read the git tags to determine the project version
include(GetGitVersion)
get_git_version(GIT_VERSION)

# Tell the user what versions we are using
string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" VERSION ${GIT_VERSION})
message("-- Version: ${VERSION}")

# The version of the libraries
set(GENERIC_LIB_VERSION ${VERSION})
string(SUBSTRING ${VERSION} 0 1 GENERIC_LIB_SOVERSION)

include(CheckCXXCompilerFlag)
include(AddCXXCompilerFlag)
include(CXXFeatureCheck)

# set(CMAKE_BUILD_TYPE Debug)

set (CMAKE_BUILD_TYPE Release)
add_cxx_compiler_flag(-O4)

add_cxx_compiler_flag(-std=c++14)
add_cxx_compiler_flag(-Wall)
add_cxx_compiler_flag(-Wextra)
add_cxx_compiler_flag(-Wshadow)
add_cxx_compiler_flag(-Werror RELEASE)
add_cxx_compiler_flag(-Werror RELWITHDEBINFO)
add_cxx_compiler_flag(-Werror MINSIZEREL)

add_cxx_compiler_flag(-pedantic)
add_cxx_compiler_flag(-pedantic-errors)

add_cxx_compiler_flag(-Wshorten-64-to-32)
add_cxx_compiler_flag(-Wfloat-equal)
add_cxx_compiler_flag(-Wint-to-void-pointer-cast)
add_cxx_compiler_flag(-fstrict-aliasing)
if (HAVE_CXX_FLAG_FSTRICT_ALIASING)
  add_cxx_compiler_flag(-Wstrict-aliasing)
endif()
add_cxx_compiler_flag(-Wthread-safety)
if (HAVE_WTHREAD_SAFETY)
  add_definitions(-DHAVE_WTHREAD_SAFETY)
  cxx_feature_check(THREAD_SAFETY_ATTRIBUTES)
endif()

add_cxx_compiler_flag(-DFMT_HEADER_ONLY)
add_cxx_compiler_flag(-DCEREAL_RAPIDJSON_HAS_CXX11_RVALUE_REFS)

set(CMAKE_CXX_COMPILER "${PROJECTS_FOLDER}3p/llvm/bin/clang++")

ENABLE_TESTING()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Used libraries
set(LIB_GTEST "${PROJECTS_FOLDER}/3p/gtest/lib/libgtest.a")
set(LIB_GTEST_MAIN "${PROJECTS_FOLDER}/3p/gtest/lib/libgtest_main.a")
set(LIB_LEVELDB "${PROJECTS_FOLDER}/3p/leveldb/out-static/libleveldb.a")

# Libraries used by rocksdb
set(LIB_ROCKSDB "${PROJECTS_FOLDER}/3p/rocksdb/librocksdb.a")
set(LIB_ZLIB "${PROJECTS_FOLDER}/3p/zlib/lib/libz.a")
set(LIB_LZ4 "${PROJECTS_FOLDER}/3p/lz4/lib/liblz4.a")
set(LIB_BZ2 "${PROJECTS_FOLDER}/3p/bzip2/lib/libbz2.a")
set(LIB_SNAPPY "${PROJECTS_FOLDER}/3p/snappy/lib/libsnappy.a")
set(LIB_JEMALLOC "${PROJECTS_FOLDER}/3p/jemalloc/lib/libjemalloc.a")

set(LIB_TBB "${PROJECTS_FOLDER}/3p/tbb/lib/intel64/gcc4.4/libtbb.so")

# This option make sure that we use the local boost version. Note that if the 
# system boost is installed then CMake might use that boost version.
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_NO_SYSTEM_PATHS TRUE) 
if (Boost_NO_SYSTEM_PATHS)
  set(BOOST_ROOT "${PROJECTS_FOLDER}/3p/boost")
  set(BOOST_INCLUDE_DIRS "${BOOST_ROOT}/include")
  set(BOOST_LIBRARY_DIRS "${BOOST_ROOT}/lib")
endif (Boost_NO_SYSTEM_PATHS)

message(${BOOST_ROOT})

find_package(Boost 1.60 REQUIRED regex date_time system filesystem thread graph program_options iostreams)

# Include folder
message(${PROJECTS_FOLDER})
include_directories (${PROJECTS_FOLDER})
include_directories ("${PROJECTS_FOLDER}/3p/gtest/include")
include_directories ("${PROJECTS_FOLDER}/3p/fmt/include")
include_directories ("${PROJECTS_FOLDER}/3p/leveldb/include")

include_directories ("${PROJECTS_FOLDER}/3p/rocksdb/include")
include_directories ("${PROJECTS_FOLDER}/3p/zlib/include")
include_directories ("${PROJECTS_FOLDER}/3p/lz4/include")
include_directories ("${PROJECTS_FOLDER}/3p/bzip2/include")
include_directories ("${PROJECTS_FOLDER}/3p/jemalloc/include")

include_directories ("${PROJECTS_FOLDER}/3p/cereal/include")
include_directories ("${PROJECTS_FOLDER}/3p/sparsehash/include/")
include_directories ("${PROJECTS_FOLDER}/3p/tbb/include/")

# Use POCO static library
include_directories("${PROJECTS_FOLDER}/3p/poco/include")
set(LIB_POCO_FOUNDATION "${PROJECTS_FOLDER}/3p/poco/lib/libPocoFoundation.a")

if (Boost_FOUND)
  include_directories(${BOOST_INCLUDE_DIRS})
  set(UNITTEST_SRC_FILES tSmartPtr tStringSearch tParamPack tTuplePerfTests tStructPerfTests tTuplevsStruct tMoveConstructor tFile)
  foreach (src_file ${UNITTEST_SRC_FILES})
    ADD_EXECUTABLE(${src_file} ${src_file}.cpp)
    TARGET_LINK_LIBRARIES(${src_file} ${BOOST_LIBRARIES}  ${LIB_GTEST} ${LIB_GTEST_MAIN} -lpthread)
    ADD_TEST(${src_file} ./${src_file})
  endforeach (src_file)

  set(UNITTEST_SRC_FILES tparallel_for)
  foreach (src_file ${UNITTEST_SRC_FILES})
    ADD_EXECUTABLE(${src_file} ${src_file}.cpp)
    TARGET_LINK_LIBRARIES(${src_file} ${BOOST_LIBRARIES}  ${LIB_GTEST} ${LIB_GTEST_MAIN} -ltbb -lpthread)
    ADD_TEST(${src_file} ./${src_file})
  endforeach (src_file)

  set(UNITTEST_SRC_FILES tcilk_basic)
  foreach (src_file ${UNITTEST_SRC_FILES})
    ADD_EXECUTABLE(${src_file} ${src_file}.cpp)
    TARGET_LINK_LIBRARIES(${src_file} ${BOOST_LIBRARIES}  ${LIB_GTEST} ${LIB_GTEST_MAIN} -ltbb -lpthread)
    ADD_TEST(${src_file} ./${src_file})
  endforeach (src_file)
endif (Boost_FOUND)
