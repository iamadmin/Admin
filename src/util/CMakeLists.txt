AUX_SOURCE_DIRECTORY(./ srcUtil)
AUX_SOURCE_DIRECTORY(./config srcUtilConfig)

ADD_LIBRARY(util STATIC ${srcUtil} ${srcUtilConfig})
TARGET_LINK_LIBRARIES(util mysqlclient)


