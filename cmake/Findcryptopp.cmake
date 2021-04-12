find_package(PkgConfig)

if(PkgConfig_FOUND)
  pkg_check_modules(cryptopp libcryptopp libcrypto++ IMPORTED_TARGET GLOBAL)
else()
  set(cryptopp_FOUND OFF)
endif()
