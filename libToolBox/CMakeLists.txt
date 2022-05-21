# Create a library called "Hello" which includes the source file "hello.cxx".
# The extension is already found. Any number of sources could be listed here.
set(LIB_TOOLBOX_SRC
${CMAKE_CURRENT_SOURCE_DIR}/src/ipl_tb_cmd_mb_field.c 
${CMAKE_CURRENT_SOURCE_DIR}/src/ipl_tb_cmd_defaults.c
${CMAKE_CURRENT_SOURCE_DIR}/src/ipl_tb_cmd_corrupt.c
${CMAKE_CURRENT_SOURCE_DIR}/src/ipl_tb_cmd_set_path.c 
${CMAKE_CURRENT_SOURCE_DIR}/src/ipl_tb_internal.c
${CMAKE_CURRENT_SOURCE_DIR}/src/ipl_tb_cmd_system.c
${CMAKE_CURRENT_SOURCE_DIR}/src/ipl_tb_init.c 
${CMAKE_CURRENT_SOURCE_DIR}/src/ipl_tb_term.c 
${CMAKE_CURRENT_SOURCE_DIR}/src/ipl_tb_main.c 
${CMAKE_CURRENT_SOURCE_DIR}/src/ipl_tb_cmd_read.c 
${CMAKE_CURRENT_SOURCE_DIR}/src/ipl_tb_cmd_write.c 
${CMAKE_CURRENT_SOURCE_DIR}/src/ipl_tb_cmd_versions.c
${CMAKE_CURRENT_SOURCE_DIR}/src/ipl_tb_cmd_quit.c 
${CMAKE_CURRENT_SOURCE_DIR}/src/ipl_tb_cmd_reboot.c 
${CMAKE_CURRENT_SOURCE_DIR}/src/ipl_tb_mtd_read.c 
${CMAKE_CURRENT_SOURCE_DIR}/src/ipl_tb_mtd_erase.c 
${CMAKE_CURRENT_SOURCE_DIR}/src/ipl_tb_mtd_write.c
${CMAKE_CURRENT_SOURCE_DIR}/src/ipl_tb_nand_test.c
${CMAKE_CURRENT_SOURCE_DIR}/src/ipl_tb_background_test.c
${CMAKE_CURRENT_SOURCE_DIR}/src/linenoise.c
)
add_library (ToolBox ${LIB_TOOLBOX_SRC})

# Make sure the compiler can find include files for our Hello library
# when other libraries or executables link to Hello
target_include_directories (ToolBox PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
