
find_package(Catch2 3 REQUIRED)


add_executable(unit_tests)
target_sources(unit_tests PRIVATE 
	operations.cpp
	conversions.cpp
	format.cpp
)


target_link_libraries(unit_tests PRIVATE SI Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(unit_tests)