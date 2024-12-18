set(CMAKE_C_FLAGS )
set(CMAKE_C_FLAGS_DEBUG )
set(CMAKE_C_FLAGS_RELEASE )
set(CMAKE_CXX_FLAGS_RELEASE )
set(CMAKE_CXX_FLAGS_DEBUG )
set(CMAKE_ASM_FLAGS_RELEASE )
set(CMAKE_ASM_FLAGS_DEBUG )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include("${CMAKE_CURRENT_LIST_DIR}//toolchain.cmake")
include("${CMAKE_CURRENT_LIST_DIR}//rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}//file.cmake")
set(library_list )
# Handle files with suffix s
if(FILE_GROUP_assemble)
    add_library(assemble OBJECT ${FILE_GROUP_assemble})
    assemble_rule(assemble)
    list(APPEND library_list "$<TARGET_OBJECTS:assemble>")
endif()

# Handle files with suffix S
if(FILE_GROUP_assembleWithPreprocess)
    add_library(assembleWithPreprocess OBJECT ${FILE_GROUP_assembleWithPreprocess})
    assembleWithPreprocess_rule(assembleWithPreprocess)
    list(APPEND library_list "$<TARGET_OBJECTS:assembleWithPreprocess>")
endif()

# Handle files with suffix [cC]
if(FILE_GROUP_compile)
    add_library(compile OBJECT ${FILE_GROUP_compile})
    compile_rule(compile)
    list(APPEND library_list "$<TARGET_OBJECTS:compile>")
endif()

# Handle files with suffix cpp
if(FILE_GROUP_compile_cpp)
    add_library(compile_cpp OBJECT ${FILE_GROUP_compile_cpp})
    compile_cpp_rule(compile_cpp)
    list(APPEND library_list "$<TARGET_OBJECTS:compile_cpp>")
endif()

if (BUILD_LIBRARY)
    message(STATUS "Building LIBRARY")
    add_library(temp ${library_list})
    foreach(lib ${FILE_GROUP_link})
        target_link_libraries(temp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/${lib})
    endforeach()
    add_custom_command(
      TARGET temp
    COMMAND ${CMAKE_COMMAND} -E make_directory ${output_dir}
    COMMAND ${CMAKE_COMMAND} -E copy libtemp.a ${output_dir}/${image_name})
else()
    message(STATUS "Building STANDARD")
    add_executable(${image_name} ${library_list})
    foreach(lib ${FILE_GROUP_link})
        target_link_libraries(${image_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/${lib})
    endforeach()
    link_rule(${image_name})
        add_custom_target(
        Bin2Hex
        ALL
        ${MP_BIN2HEX} ${image_name}
    )
    add_dependencies(Bin2Hex ${image_name})

    add_custom_command(
      TARGET ${image_name}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${output_dir}
    COMMAND ${CMAKE_COMMAND} -E copy ${image_name} ${output_dir}/${image_name})
endif()
