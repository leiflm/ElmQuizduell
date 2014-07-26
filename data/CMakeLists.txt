foreach(CERT qkgermany)
    install (FILES "${CMAKE_CURRENT_SOURCE_DIR}/certs/${CERT}.pem"
        DESTINATION "${CERTS_DIR}")
endforeach(CERT)