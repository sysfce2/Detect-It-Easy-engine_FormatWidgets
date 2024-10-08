include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../MultiSearch/multisearch.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../FormatDialogs/dialogeditstring.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../Controls/xtableview.cmake)

set(SEARCHSTRINGSWIDGET_SOURCES
    ${MULTISEARCH_SOURCES}
    ${DIALOGEDITSTRING_SOURCES}
    ${XTABLEVIEW_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dialogsearchstrings.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogsearchstrings.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogsearchstrings.ui
    ${CMAKE_CURRENT_LIST_DIR}/searchstringswidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/searchstringswidget.h
    ${CMAKE_CURRENT_LIST_DIR}/searchstringswidget.ui
    )
