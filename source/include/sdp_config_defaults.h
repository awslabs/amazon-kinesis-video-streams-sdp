#ifndef SDP_CONFIG_DEFAULTS_H
#define SDP_CONFIG_DEFAULTS_H

/* SDP includes. */
#ifndef SDP_DO_NOT_USE_CUSTOM_CONFIG
    /* Include custom config file before other headers. */
    #include "sdp_config.h"
#endif

/**
 * @brief SDP print format for size_t.
 */
#ifndef SDP_PRINT_FMT_SIZE
    #define SDP_PRINT_FMT_SIZE          "lu"
#endif

/**
 * @brief SDP print format for uint64_t.
 */
#ifndef SDP_PRINT_FMT_UINT64
    #define SDP_PRINT_FMT_UINT64        "llu"
#endif

/**
 * @brief SDP print format for uint32_t.
 */
#ifndef SDP_PRINT_FMT_UINT32
    #define SDP_PRINT_FMT_UINT32        "lu"
#endif

/**
 * @brief SDP print format for uint16_t.
 */
#ifndef SDP_PRINT_FMT_UINT16
    #define SDP_PRINT_FMT_UINT16        "hu"
#endif

/**
 * @brief SDP print format for uint8_t.
 */
#ifndef SDP_PRINT_FMT_UINT8
    #define SDP_PRINT_FMT_UINT8         "u"
#endif

/**
 * @brief SDP print format for int.
 */
#ifndef SDP_PRINT_FMT_INT
    #define SDP_PRINT_FMT_INT           "d"
#endif

/**
 * @brief SDP print format for int64_t.
 */
#ifndef SDP_PRINT_FMT_INT64
    #define SDP_PRINT_FMT_INT64         "lld"
#endif

/**
 * @brief SDP print format for int32_t.
 */
#ifndef SDP_PRINT_FMT_INT32
    #define SDP_PRINT_FMT_INT32         "d"
#endif

/**
 * @brief SDP print format for int16_t.
 */
#ifndef SDP_PRINT_FMT_INT16
    #define SDP_PRINT_FMT_INT16         "d"
#endif

/**
 * @brief SDP print format for int8_t.
 */
#ifndef SDP_PRINT_FMT_INT8
    #define SDP_PRINT_FMT_INT8          "d"
#endif

/**
 * @brief SDP print format for char.
 */
#ifndef SDP_PRINT_FMT_CHAR
    #define SDP_PRINT_FMT_CHAR          "c"
#endif

/**
 * @brief SDP print format for string.
 */
#ifndef SDP_PRINT_FMT_STR
    #define SDP_PRINT_FMT_STR           "s"
#endif

/**
 * @brief SDP print format for pointer.
 */
#ifndef SDP_PRINT_FMT_PTR
    #define SDP_PRINT_FMT_PTR           "p"
#endif

/**
 * @brief SDP print format for uint32_t in hex.
 */
#ifndef SDP_PRINT_FMT_UINT32_HEX
    #define SDP_PRINT_FMT_UINT32_HEX    "x"
#endif

#endif /* SDP_CONFIG_DEFAULTS_H */
