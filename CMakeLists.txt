AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/IpTracker.cpp")
AC_ADD_SCRIPT_LOADER("IpTracker" "${CMAKE_CURRENT_LIST_DIR}/src/loader.h")

AC_ADD_CONFIG_FILE("${CMAKE_CURRENT_LIST_DIR}/conf/ip-tracker.conf.dist")