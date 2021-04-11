find_package(PkgConfig)

if(PkgConfig_FOUND)
  pkg_check_modules(cryptopp REQUIRED libcryptopp IMPORTED_TARGET GLOBAL)
else()
  set(cryptopp_FOUND OFF)
endif()
