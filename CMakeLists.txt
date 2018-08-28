PROJECT(Experiments)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

set(ROOT_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}")
set(EXTERNAL_FOLDER "${ROOT_FOLDER}/../3p")

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
# add_cxx_compiler_flag(-Wshadow)

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

ENABLE_TESTING()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Libraries used by rocksdb
set(LIB_ROCKSDB "${EXTERNAL_FOLDER}/lib/librocksdb.a")
set(LIB_ZLIB "${EXTERNAL_FOLDER}/lib/libz.a")
set(LIB_LZ4 "${EXTERNAL_FOLDER}/lib/liblz4.a")
set(LIB_BZ2 "${EXTERNAL_FOLDER}/lib/libbz2.a")
set(LIB_SNAPPY "${EXTERNAL_FOLDER}/lib/libsnappy.a")
set(LIB_JEMALLOC "${EXTERNAL_FOLDER}/lib/libjemalloc.a")
set(LIB_TBB "${EXTERNAL_FOLDER}/lib/libtbb.so")
set(LIB_TBB_MALLOC "${EXTERNAL_FOLDER}/lib/libtbb_malloc.so")
set(LIB_CELERO "${EXTERNAL_FOLDER}/lib/libcelero.a")

# This option make sure that we use the local boost version. Note that if the 
# system boost is installed then CMake might use that boost version.
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_NO_SYSTEM_PATHS TRUE) 
if (Boost_NO_SYSTEM_PATHS)
  set(BOOST_ROOT "${EXTERNAL_FOLDER}/")
  set(BOOST_INCLUDE_DIRS "${BOOST_ROOT}/include")
  set(BOOST_LIBRARY_DIRS "${BOOST_ROOT}/lib")
endif (Boost_NO_SYSTEM_PATHS)

message("${BOOST_ROOT}")

find_package(Boost 1.60 REQUIRED regex date_time system filesystem thread graph program_options iostreams context)

# Include folder
message(${EXTERNAL_FOLDER})
include_directories ("${EXTERNAL_FOLDER}/include")

if (Boost_FOUND)
  include_directories(${BOOST_INCLUDE_DIRS})

  set(UNITTEST_SRC_FILES tBalancedBrackets tParamPack tTuplePerfTests tStructPerfTests  tMoveConstructor tFile  tConstructorStyle)

  foreach (src_file ${UNITTEST_SRC_FILES})
    ADD_EXECUTABLE(${src_file} ${src_file}.cpp)
    TARGET_LINK_LIBRARIES(${src_file} ${Boost_LIBRARIES}  ${LIB_GTEST} ${LIB_GTEST_MAIN} ${LIB_CELERO} -lpthread)
    ADD_TEST(${src_file} ./${src_file})
  endforeach (src_file)
  
  message(${Boost_LIBRARIES})
  
  set(UNITTEST_SRC_FILES tTuplevsStruct serializationPerf tOptimizeString policy_based_design)
  foreach (src_file ${UNITTEST_SRC_FILES})
    ADD_EXECUTABLE(${src_file} ${src_file}.cpp)
    TARGET_LINK_LIBRARIES(${src_file} ${Boost_LIBRARIES} ${LIB_CELERO} -lpthread)
    ADD_TEST(${src_file} ./${src_file})
  endforeach (src_file)

  set(UNITTEST_SRC_FILES readline)
  foreach (src_file ${UNITTEST_SRC_FILES})
    ADD_EXECUTABLE(${src_file} ${src_file}.cpp)
    TARGET_LINK_LIBRARIES(${src_file} -lreadline)
    ADD_TEST(${src_file} ./${src_file})
  endforeach (src_file)

  set(UNITTEST_SRC_FILES tparallel_for)
  foreach (src_file ${UNITTEST_SRC_FILES})
    ADD_EXECUTABLE(${src_file} ${src_file}.cpp)
    TARGET_LINK_LIBRARIES(${src_file} ${BOOST_LIBRARIES}  ${LIB_GTEST} ${LIB_GTEST_MAIN} -ltbb -lpthread)
    ADD_TEST(${src_file} ./${src_file})
  endforeach (src_file)

  set(LINUX_BREW "/home/hdang/.linuxbrew")
  set(FB_FOLLY "${LINUX_BREW}/lib/libfolly.a")
  set(FB_FOLLY_INIT "${LINUX_BREW}/lib/libfollyinit.a")
  set(FB_FOLLY_LOGGING "${LINUX_BREW}/lib/libfollylogging.a")
  set(SRC_FILES fb_futures)
  foreach (src_file ${SRC_FILES})
    ADD_EXECUTABLE(${src_file} ${src_file}.cpp)
    TARGET_LINK_LIBRARIES(${src_file} ${FB_INIT} ${FB_FOLLY} ${FB_FOLLY_LOGGING} -ldl -lglog -lpthread -levent)
  endforeach (src_file)

  # set(UNITTEST_SRC_FILES tcilk_basic)
  # foreach (src_file ${UNITTEST_SRC_FILES})
  #   ADD_EXECUTABLE(${src_file} ${src_file}.cpp)
  #   TARGET_LINK_LIBRARIES(${src_file} ${BOOST_LIBRARIES}  ${LIB_GTEST} ${LIB_GTEST_MAIN} -ltbb -lpthread)
  #   ADD_TEST(${src_file} ./${src_file})
  # endforeach (src_file)
endif (Boost_FOUND)
