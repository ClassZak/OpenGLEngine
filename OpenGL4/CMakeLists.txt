cmake_minimum_required(VERSION 3.28)

# Автоматическая настройка для Windows 7
if(WIN32 AND CMAKE_SYSTEM_VERSION VERSION_EQUAL 6.1)
	# Принудительная настройка для Windows 7
	set(CMAKE_GENERATOR "Ninja" CACHE INTERNAL "" FORCE)
	set(CMAKE_MAKE_PROGRAM "C:/gcc-10.4890.0-no-debug/bin/ninja.exe" CACHE FILEPATH "Path to Ninja" FORCE)
	set(CMAKE_CXX_COMPILER "C:/gcc-10.4890.0-no-debug/bin/g++.exe" CACHE FILEPATH "CXX compiler" FORCE)
	set(CMAKE_CXX_COMPILER_WORKS 1 CACHE BOOL "" FORCE)
	set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY CACHE STRING "" FORCE)
	
	# Установка стандарта C20 для GCC
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()



set(CMAKE_CXX_STANDARD 20)

project(OpenGL4 VERSION 1.0
		DESCRIPTION "Application for data transfering"
		LANGUAGES CXX
)



# OpenGL и GLFW
find_package(OpenGL	REQUIRED)
find_package(glfw3)
find_package(glew)
if(NOT glfw3_FOUND)
	message(STATUS "GLFW not found, using fallback method")
	if(WIN32)
		set(GLFW_ROOT "G:/Programs/Libraries/glfw-3.4.bin.WIN64/glfw-3.4.bin.WIN64")
		set(GLFW_INCLUDE_DIR "${GLFW_ROOT}/include")
		set(GLFW_INCLUDE_DEPS_DIR "${GLFW_ROOT}/deps")
		set(GLFW_LIB_DIR "${GLFW_ROOT}/lib-vc2022")
		
		add_library(glfw SHARED IMPORTED)
		set_target_properties(glfw PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES "${GLFW_INCLUDE_DIR};${GLFW_INCLUDE_DEPS_DIR}"
			IMPORTED_IMPLIB "${GLFW_LIB_DIR}/glfw3.lib"
			IMPORTED_LOCATION "${GLFW_LIB_DIR}/glfw3.dll"
		)
	endif()
endif()
if(NOT glew_FOUND)
	message(STATUS "GLEW not found, using fallback method")
	if(WIN32)
		set(GLEW_ROOT "G:/Programs/Libraries/glew-2.1.0-win32/glew-2.1.0")
		set(GLEW_INCLUDE_DIR "${GLEW_ROOT}/include")
		set(GLEW_LIB_DIR "${GLEW_ROOT}/lib/Release/x64")
		set(GLEW_BIN_DIR "${GLEW_ROOT}/bin/Release/x64")

		if(NOT EXISTS "${GLEW_INCLUDE_DIR}/GL/glew.h")
			message(FATAL_ERROR "GLEW header not found: ${GLEW_INCLUDE_DIR}/GL/glew.h")
		endif()
		if(NOT EXISTS "${GLEW_LIB_DIR}/glew32.lib")
			message(FATAL_ERROR "GLEW library not found: ${GLEW_LIB_DIR}/glew32.lib")
		endif()
		if(NOT EXISTS "${GLEW_BIN_DIR}/glew32.dll")
			message(FATAL_ERROR "GLEW library not found: ${GLEW_BIN_DIR}/glew32.dll")
		endif()
		add_library(glew SHARED IMPORTED)
		set_target_properties(glew PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES "${GLEW_INCLUDE_DIR}"
			IMPORTED_IMPLIB "${GLEW_LIB_DIR}/glew32.lib"
			IMPORTED_LOCATION "${GLEW_BIN_DIR}/glew32.dll"
		)



	endif()
endif()



add_executable(
	${PROJECT_NAME}
	${CMAKE_SOURCE_DIR}/src/OpenGL4.cpp

	${CMAKE_SOURCE_DIR}/src/utils/GLMacro.h
	${CMAKE_SOURCE_DIR}/src/utils/functions.hpp
	${CMAKE_SOURCE_DIR}/src/utils/functions.cpp

	${CMAKE_SOURCE_DIR}/src/Vertex/AVertex.hpp
	${CMAKE_SOURCE_DIR}/src/Vertex/2DVertex.hpp
	${CMAKE_SOURCE_DIR}/src/Vertex/VertexUtils.hpp

	${CMAKE_SOURCE_DIR}/src/Shader/Shader.hpp
	${CMAKE_SOURCE_DIR}/src/Shader/Shader.cpp



)

target_link_libraries(${PROJECT_NAME} PRIVATE
	OpenGL::GL
	glfw
	glew
)


# Для GLEW
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${GLEW_BIN_DIR}/glew32.dll"
		$<TARGET_FILE_DIR:${PROJECT_NAME}>
	COMMENT "Copying glew32.dll to output directory"
)


# Платформенно-специфичные настройки
if(WIN32)
	# Флаги для Windows 7
	if(CMAKE_SYSTEM_VERSION VERSION_EQUAL 6.1)
		add_compile_definitions(_WIN32_WINNT=0x0601)
		target_compile_options(${PROJECT_NAME} PRIVATE -O0)
	endif()
else()
	# Настройки для Linux/MacOS
	target_compile_options(${PROJECT_NAME} PRIVATE -O0)
endif()

# Настройки для MSVC (Windows 10/11)
if(MSVC)
	# Активация политики для Edit and Continue
	cmake_policy(SET CMP0141 NEW)
	target_compile_options(${PROJECT_NAME} PRIVATE "/std:c++20")
	
	set_property(TARGET ${PROJECT_NAME} PROPERTY
		MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug>:EditAndContinue>"
	)
	set_target_properties(${PROJECT_NAME} PROPERTIES
		LINK_FLAGS_DEBUG "/INCREMENTAL"
	)
	
	# Флаги оптимизации
	target_compile_options(${PROJECT_NAME} PRIVATE
		"$<$<CONFIG:Debug>:/Od>"
	)
endif()
