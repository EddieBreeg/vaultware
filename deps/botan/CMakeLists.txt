

add_library(botan STATIC IMPORTED GLOBAL)

target_include_directories(botan INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(${WIN32})
    set(BOTAN_LIB_FILE botan.lib)
else()
    set(BOTAN_LIB_FILE libbotan-2.a)
endif(${WIN32})


set_target_properties(botan PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/${BOTAN_LIB_FILE}
)