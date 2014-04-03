AUX_SOURCE_DIRECTORY(./ srcControl)

ADD_LIBRARY(control STATIC ${srcControl})

TARGET_LINK_LIBRARIES(control event_core)
