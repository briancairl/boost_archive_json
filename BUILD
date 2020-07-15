cc_library(
  name="picojson_wrapper",
  hdrs=["include/boost/archive/picojson_wrapper.h"],
  srcs=["src/picojson_wrapper.cpp"],
  strip_include_prefix="include/",
  deps=["@picojson//:picojson", "@boost//:serialization",],
  visibility=["//visibility:private"],
)

cc_library(
  name="json_oarchive",
  hdrs=["include/boost/archive/json_oarchive.h"],
  srcs=["src/json_oarchive.cpp"],
  strip_include_prefix="include/",
  deps=[":picojson_wrapper", "@boost//:serialization",],
  visibility=["//visibility:public"],
)

cc_library(
  name="json_iarchive",
  hdrs=["include/boost/archive/json_iarchive.h"],
  srcs=["src/json_iarchive.cpp"],
  strip_include_prefix="include/",
  deps=[":picojson_wrapper", "@boost//:serialization",],
  visibility=["//visibility:public"],
)
