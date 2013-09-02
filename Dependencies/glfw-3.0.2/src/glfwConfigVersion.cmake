
set(PACKAGE_VERSION "3.0.2")

if ("${PACKAGE_FIND_VERSION_MAJOR}" EQUAL "3")
	set(PACKAGE_VERSION_COMPATIBLE TRUE)
	if ("${PACKAGE_FIND_VERSION_MINOR}" EQUAL 0)
		set(PACKAGE_VERSION_EXACT TRUE)
	endif()
else()
	set(PACKAGE_VERSION_COMPATIBLE FALSE)
endif()

