# Create a shared library
add_library(WireLinkBackend SHARED
	abi/abi.cpp
	internal/internal.cpp
)

# Create an alias for better namespacing
add_library(WireLink::Backend ALIAS WireLinkBackend)

# Set C++ standard using target_compile_features
target_compile_features(WireLinkBackend PUBLIC cxx_std_17)

target_include_directories(WireLinkBackend
	PUBLIC
		${CMAKE_SOURCE_DIR}/include #/WireLinkBackend/abi
		${CMAKE_CURRENT_BINARY_DIR}/include #/WireLinkBackend/not abi
)

target_include_directories(WireLinkBackend
	PRIVATE
		${CMAKE_SOURCE_DIR}/src #/WireLinkBackend/not abi
)

# Compiler flags
	# Warning flags - MSVC
	target_compile_options(WireLinkBackend PRIVATE
		$<$<CXX_COMPILER_ID:MSVC>:/W4>
		$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<BOOL:${TREAT_WARNINGS_AS_ERROR}>>:/WX>
	)

	# Warning flags - GCC/Clang  
	target_compile_options(WireLinkBackend PRIVATE
		$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall>
		$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<BOOL:${TREAT_WARNINGS_AS_ERROR}>>:-Werror>
	)

	# Optimization flags - MSVC
	target_compile_options(WireLinkBackend PRIVATE
		#no optimization with debug information
		$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/Od>
		# optimize for speed
		$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/O2>
		$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebInfo>>:/O2>
		# optimize for size
		$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:MinSizeRel>>:/Os>
	)

	# Optimization flags - GCC/Clang
	target_compile_options(WireLinkBackend PRIVATE
		# no optimization with debug information
		$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:Debug>>:-O0 -g>
		# aggressive optimization and define NDEBUG
		$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:Release>>:-O3 -DNDEBUG>
		# balanced optimization with debug information, define NDEBUG
		$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:RelWithDebInfo>>:-O2 -g -DNDEBUG>
		# optimize for size, define NDEBUG
		$<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:MinSizeRel>>:-Os -DNDEBUG>
	)

target_link_libraries(WireLinkBackend PRIVATE Threads::Threads)

# Hide all symbols by default, export only those we mark
set_target_properties(WireLinkBackend PROPERTIES
	C_VISIBILITY_PRESET hidden
	CXX_VISIBILITY_PRESET hidden
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	EXPORT_NAME Backend
)

# Generate an export header (defines WIRELINKBACKEND_EXPORT macro)
include(GenerateExportHeader)
generate_export_header(WireLinkBackend
	EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/WireLinkBackend/export_macro.h
)