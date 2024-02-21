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