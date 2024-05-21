#ifndef SDP_CONFIG_H
#define SDP_CONFIG_H

#include <inttypes.h>

/**
 * @brief SDP print format for uint64_t.
 */
#ifndef SDP_PRINT_FMT_UINT64
    #define SDP_PRINT_FMT_UINT64        PRIu64
#endif

/**
 * @brief SDP print format for uint32_t.
 */
#ifndef SDP_PRINT_FMT_UINT32
    #define SDP_PRINT_FMT_UINT32        PRIu32
#endif

/**
 * @brief SDP print format for uint16_t.
 */
#ifndef SDP_PRINT_FMT_UINT16
    #define SDP_PRINT_FMT_UINT16        "hu"
#endif

#endif /* SDP_CONFIG_H */
