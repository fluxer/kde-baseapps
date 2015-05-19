add_executable(
    unit_tests
    version_checker_test.cpp
  )

target_link_libraries(
    unit_tests
    ${QT_QTTEST_LIBRARY_RELEASE}
    ${QT_QTCORE_LIBRARY_RELEASE}
  )
