MACRO(LIB_FINDER return_list)
    FILE(GLOB_RECURSE new_list *.o *.obj *.a *.lib *.dll *.so)

    set (EXCLUDE_DIR "/build/")
    
    SET(dir_list "")
    FOREACH(file_path ${new_list})

        string (FIND ${file_path} ${EXCLUDE_DIR} EXCLUDE_DIR_FOUND)
        
        if (${EXCLUDE_DIR_FOUND} EQUAL -1)
            SET(dir_list ${dir_list} ${file_path})
        endif ()

    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

MACRO(SUBDIRLIST result curdir)
    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
    SET(dirlist "")
    FOREACH(child ${children})
        IF(IS_DIRECTORY ${curdir}/${child})
        LIST(APPEND dirlist ${child})
    ENDIF()
    ENDFOREACH()
    SET(${result} ${dirlist})
ENDMACRO()

set(GODOT_CUSTOM_API_FILE "${CMAKE_CURRENT_SOURCE_DIR}/api/api.json")

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

FOREACH(subdir ${SUBDIRS})
    if(EXISTS "${subdir}/CMakeLists.txt")
        ADD_SUBDIRECTORY(${subdir})
    endif()
ENDFOREACH()

