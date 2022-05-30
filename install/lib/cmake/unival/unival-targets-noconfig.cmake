#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "unival::unival" for configuration ""
set_property(TARGET unival::unival APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(unival::unival PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libunival.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS unival::unival )
list(APPEND _IMPORT_CHECK_FILES_FOR_unival::unival "${_IMPORT_PREFIX}/lib/libunival.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
