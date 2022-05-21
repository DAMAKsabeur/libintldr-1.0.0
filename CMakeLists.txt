# Create a library called "Hello" which includes the source file "hello.cxx".
# The extension is already found. Any number of sources could be listed here.
set(LIB_INTLDR_SRC
${CMAKE_CURRENT_SOURCE_DIR}/src/intldr_flash_acess.c
${CMAKE_CURRENT_SOURCE_DIR}/src/intldr.c
)
SET(HEADER_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/intldr.h
)
add_library (intldr ${LIB_INTLDR_SRC})

# Make sure the compiler can find include files for our Hello library
# when other libraries or executables link to Hello
target_include_directories (intldr PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
install (TARGETS intldr DESTINATION lib)
install (FILES ${HEADER_FILES} DESTINATION include)
