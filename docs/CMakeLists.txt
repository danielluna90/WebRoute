set(DOXYGEN_SKIP_DOT TRUE)
find_package(Doxygen)

if (NOT DOXYGEN_FOUND OR DOXYGEN_VERSION VERSION_LESS "1.10.0")
    message(STATUS "Documentation generation requires Doxygen 1.10.0 or later")
else()
    configure_file(Doxyfile.in Doxyfile @ONLY)
    add_custom_command(OUTPUT "html/index.html"
                       COMMAND "${DOXYGEN_EXECUTABLE}"
                       WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
                       MAIN_DEPENDENCY Doxyfile
                       COMMENT "Generating HTML documentation"
                       VERBATIM)

    add_custom_target(docs ALL SOURCES "html/index.html")
endif()