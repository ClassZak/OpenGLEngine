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

project(Jelly VERSION 1.0
		DESCRIPTION "Application for data transfering"
		LANGUAGES CXX
)



# OpenGL и GLFW
find_package(OpenGL	REQUIRED)
if(WIN32)
	find_package(glfw3)
	find_package(glew)
	if(NOT glfw3_FOUND)
		message(STATUS "GLFW not found, using fallback method")
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
	if(NOT glew_FOUND)
		message(STATUS "GLEW not found, using fallback method")
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
else()
	# Linux-specific setup
	# Ищем GLFW
	find_path(GLFW_INCLUDE_DIR GLFW/glfw3.h
		PATHS /usr/include /usr/local/include
		DOC "Path to GLFW include directory"
	)

	find_library(GLFW_LIBRARY
		NAMES glfw glfw3
		PATHS /usr/lib /usr/local/lib /usr/lib/x86_64-linux-gnu
		DOC "Path to GLFW library"
	)

	if(GLFW_INCLUDE_DIR AND GLFW_LIBRARY)
		message(STATUS "Found GLFW: include=${GLFW_INCLUDE_DIR}, lib=${GLFW_LIBRARY}")
		add_library(glfw UNKNOWN IMPORTED)
		set_target_properties(glfw PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES "${GLFW_INCLUDE_DIR}"
			IMPORTED_LOCATION "${GLFW_LIBRARY}"
		)
	else()
		message(FATAL_ERROR "GLFW not found. Install with: sudo apt install libglfw3-dev")
	endif()

	# Ищем GLEW
	find_path(GLEW_INCLUDE_DIR GL/glew.h
		PATHS /usr/include /usr/local/include
		DOC "Path to GLEW include directory"
	)

	find_library(GLEW_LIBRARY
		NAMES GLEW glew
		PATHS /usr/lib /usr/local/lib /usr/lib/x86_64-linux-gnu
		DOC "Path to GLEW library"
	)

	if(GLEW_INCLUDE_DIR AND GLEW_LIBRARY)
		message(STATUS "Found GLEW: include=${GLEW_INCLUDE_DIR}, lib=${GLEW_LIBRARY}")
		add_library(glew UNKNOWN IMPORTED)
		set_target_properties(glew PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES "${GLEW_INCLUDE_DIR}"
			IMPORTED_LOCATION "${GLEW_LIBRARY}"
		)
	else()
		message(FATAL_ERROR "GLEW not found. Install with: sudo apt install libglew-dev")
	endif()
endif()












set(SOURCES
	${CMAKE_SOURCE_DIR}/src/main.cpp
)

# Добавляем исполняемый файл
add_executable(${PROJECT_NAME}
	${SOURCES}
	
	${CMAKE_SOURCE_DIR}/src/OpenGLClass/AOpenGLClass.hpp
	${CMAKE_SOURCE_DIR}/src/OpenGLClass/IndexBufferObject.hpp
	${CMAKE_SOURCE_DIR}/src/OpenGLClass/Renderer.hpp
	${CMAKE_SOURCE_DIR}/src/OpenGLClass/Renderer.cpp
	${CMAKE_SOURCE_DIR}/src/OpenGLClass/VertexArrayObject.hpp
	${CMAKE_SOURCE_DIR}/src/OpenGLClass/VertexBufferObject.hpp
	${CMAKE_SOURCE_DIR}/src/OpenGLClass/VertexBufferLayout.hpp
	${CMAKE_SOURCE_DIR}/src/OpenGLClass/Shader.hpp
	${CMAKE_SOURCE_DIR}/src/OpenGLClass/Shader.cpp
	
	
	${CMAKE_SOURCE_DIR}/src/Shape/Interfaces/IDrawable.hpp
	${CMAKE_SOURCE_DIR}/src/Shape/Interfaces/IHasIndexBufferObject.hpp
	${CMAKE_SOURCE_DIR}/src/Shape/Interfaces/IHasVertexBufferObject.hpp
	${CMAKE_SOURCE_DIR}/src/Shape/Interfaces/IHasVertexArrayObject.hpp
	
	${CMAKE_SOURCE_DIR}/src/Shape/AShape.hpp
	${CMAKE_SOURCE_DIR}/src/Shape/Circle.hpp
	${CMAKE_SOURCE_DIR}/src/Shape/CircleSector.hpp
	${CMAKE_SOURCE_DIR}/src/Shape/Jelly.hpp
	${CMAKE_SOURCE_DIR}/src/Shape/Jelly.cpp
	${CMAKE_SOURCE_DIR}/src/Shape/Line.hpp
	${CMAKE_SOURCE_DIR}/src/Shape/Quadrangle.hpp
	${CMAKE_SOURCE_DIR}/src/Shape/SimpleRect.hpp
	
	
	${CMAKE_SOURCE_DIR}/src/utils/functions.hpp
	${CMAKE_SOURCE_DIR}/src/utils/functions.cpp
	${CMAKE_SOURCE_DIR}/src/utils/GLMacro.h
	
	${CMAKE_SOURCE_DIR}/src/Vertex/AVertex.hpp
	${CMAKE_SOURCE_DIR}/src/Vertex/Vertex2D.hpp
	${CMAKE_SOURCE_DIR}/src/Vertex/VertexUtils.hpp



	
)

# Для Windows добавляем ресурсный файл
if(WIN32)
	# Создаем ресурсный файл
	set(RESOURCE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/resources.rc)
	
	# Добавляем ресурсы к исполняемому файлу
	target_sources(${PROJECT_NAME} PRIVATE ${RESOURCE_FILE})
	
	# Устанавливаем свойства для Windows
	set_target_properties(${PROJECT_NAME} PROPERTIES
		WIN32_EXECUTABLE TRUE
		LINK_FLAGS "/ENTRY:mainCRTStartup"
	)
	target_compile_definitions(${PROJECT_NAME} PRIVATE -DUNICODE -D_UNICODE)
endif()

target_include_directories(
	${PROJECT_NAME}		PRIVATE
	${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	OpenGL::GL
	glfw
	glew
)
if(UNIX)
	target_link_libraries(${PROJECT_NAME} PRIVATE
		X11 pthread dl GL
	)
endif()


# Для GLEW
if(WIN32)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${GLEW_BIN_DIR}/glew32.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>
		COMMENT "Copying glew32.dll to output directory"
	)
endif()


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





# Консоль для debug версии
if(WIN32)
	# Установка точки входа и подсистемы
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		# Debug: консольное приложение
		set_target_properties(${PROJECT_NAME} PROPERTIES
			LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:CONSOLE"
		)
	else()
		# Release: оконное приложение
		set_target_properties(${PROJECT_NAME} PROPERTIES
			LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS"
		)
	endif()
endif()

# Debug режим для компиляции
target_compile_definitions(Jelly PRIVATE
	$<$<CONFIG:Debug>:DEBUG_MODE>
)



