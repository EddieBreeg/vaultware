

add_library(botan STATIC IMPORTED GLOBAL)

target_include_directories(botan INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(${MSVC})
    set(BOTAN_LIB_FILE botan.lib)
else()
    set(BOTAN_LIB_FILE libbotan.a)
endif(${MSVC})


set_target_properties(botan PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/${BOTAN_LIB_FILE}
)