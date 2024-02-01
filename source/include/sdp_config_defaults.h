#ifndef SDP_CONFIG_DEFAULTS_H
#define SDP_CONFIG_DEFAULTS_H

/* SDP includes. */
#ifndef SDP_DO_NOT_USE_CUSTOM_CONFIG
    /* Include custom config file before other headers. */
    #include "sdp_config.h"
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

#endif /* SDP_CONFIG_DEFAULTS_H */
