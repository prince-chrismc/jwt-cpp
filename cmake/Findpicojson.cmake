# Findpicojson.cmake
# Finds the picojson header-only library
#
# Imported Targets:
#   picojson::picojson
#
# Result Variables:
#   picojson_FOUND
#   picojson_INCLUDE_DIR

find_path(picojson_INCLUDE_DIR
  NAMES picojson.h
  HINTS
    /usr/local/include
    /usr/include
    ${picojson_ROOT}
    $ENV{picojson_ROOT}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(picojson
  REQUIRED_VARS picojson_INCLUDE_DIR
)

if(picojson_FOUND AND NOT TARGET picojson::picojson)
  add_library(picojson::picojson INTERFACE IMPORTED)
  set_target_properties(picojson::picojson PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${picojson_INCLUDE_DIR}"
  )
endif()

mark_as_advanced(picojson_INCLUDE_DIR)
