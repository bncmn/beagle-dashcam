# Build the app, using the HAL

include_directories(include)
file(GLOB MY_SOURCES "src/*.cpp")
add_executable(bbg_dashcam ${MY_SOURCES})

# Make use of the HAL library
target_link_libraries(bbg_dashcam LINK_PRIVATE hal)

# Copy executable to final location (change `hello_world` to project name as needed)
add_custom_command(TARGET bbg_dashcam POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy 
     "$<TARGET_FILE:bbg_dashcam>"
     "~/cmpt433/public/myApps/bbg_dashcam" 
  COMMENT "Copying ARM executable to public NFS directory")

# Build capture.c (using Makefule inside cameraDriver folder)
add_custom_target(capturer
  COMMAND ${CMAKE_MAKE_PROGRAM}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/app/src/cameraDriver
)

# Copy capture executable to final location and run 'make clean' inside cameraDriver folder.
add_custom_command(TARGET capturer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/app/src/cameraDriver/capture
            "~/cmpt433/public/myApps"
    COMMAND ${CMAKE_MAKE_PROGRAM} -C ${CMAKE_SOURCE_DIR}/app/src/cameraDriver clean
    COMMENT "Copying capture to public NFS directory and cleaning camera source folder"
)

add_dependencies(bbg_dashcam capturer)