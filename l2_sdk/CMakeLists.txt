INCLUDE_DIRECTORIES(.)

SET(L2_SRC l2_sdk.cpp 
           l2_sdk.h 
           base.cpp
           base.h
           data_struct.h
           HC_serial.cpp
           HC_serial.h
           L2DeviceHandle.cpp
           L2DeviceHandle.h)

ADD_LIBRARY(l2_sdk ${L2_SRC})
IF (WIN32)
ELSE()
    TARGET_LINK_LIBRARIES(l2_sdk pthread)
ENDIF()