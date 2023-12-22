#ifndef SDP_DATA_TYPES_H
#define SDP_DATA_TYPES_H

/* Standard includes. */
#include <stdint.h>
#include <stddef.h>

#include "sdp_config_defaults.h"

#define SDP_TYPE_VERSION         'v'
#define SDP_TYPE_ORIGINATOR      'o'
#define SDP_TYPE_SESSION_NAME    's'
#define SDP_TYPE_SESSION_INFO    'i'
#define SDP_TYPE_URI             'u'
#define SDP_TYPE_EMAIL           'e'
#define SDP_TYPE_PHONE           'p'
#define SDP_TYPE_CONNINFO        'c'
#define SDP_TYPE_BANDWIDTH       'b'
#define SDP_TYPE_TIME_ACTIVE     't'
#define SDP_TYPE_REPEAT_TIMES    'r'
#define SDP_TYPE_TIME_ZONE       'z'
#define SDP_TYPE_ENCRYPTION_KEY  'k'
#define SDP_TYPE_ATTRIBUTE       'a'
#define SDP_TYPE_MEDIA           'm'

/*-----------------------------------------------------------*/

typedef enum SdpResult
{
    SDP_RESULT_OK,
    SDP_RESULT_BASE = 0x51000000,
    SDP_RESULT_BAD_PARAM,
    SDP_RESULT_MESSAGE_END,
    SDP_RESULT_MESSAGE_MALFORMED,
    SDP_RESULT_MESSAGE_MALFORMED_NO_ENOUGH_INFO,
    SDP_RESULT_MESSAGE_MALFORMED_EQUAL_NOT_FOUND,
    SDP_RESULT_MESSAGE_MALFORMED_NEWLINE_NOT_FOUND,
    SDP_RESULT_MESSAGE_MALFORMED_NO_VALUE,
    SDP_RESULT_MESSAGE_MALFORMED_NO_SESSION_ID,
    SDP_RESULT_MESSAGE_MALFORMED_NO_SESSION_VERSION,
    SDP_RESULT_MESSAGE_MALFORMED_INVALID_NETWORK_TYPE,
    SDP_RESULT_MESSAGE_MALFORMED_INVALID_ADDRESS_TYPE,
    SDP_RESULT_MESSAGE_MALFORMED_REDUNDANT_INFO,
    SDP_RESULT_MESSAGE_MALFORMED_INVALID_BANDWIDTH,
    SDP_RESULT_MESSAGE_MALFORMED_INVALID_START_TIME,
    SDP_RESULT_MESSAGE_MALFORMED_INVALID_STOP_TIME,
    SDP_RESULT_MESSAGE_MALFORMED_INVALID_PORT,
    SDP_RESULT_MESSAGE_MALFORMED_INVALID_PORTNUM,
    SDP_RESULT_OUT_OF_MEMORY,
    SDP_RESULT_SNPRINTF_ERROR
} SdpResult_t;

typedef enum SdpNetworkType
{
    SDP_NETWORK_IN,
    SDP_NETWORK_UNKNOWN,
} SdpNetworkType_t;

typedef enum SdpAddressType
{
    SDP_ADDRESS_IPV4,
    SDP_ADDRESS_IPV6,
    SDP_ADDRESS_UNKNOWN,
} SdpAddressType_t;

/*-----------------------------------------------------------*/

typedef struct SdpConnectionInfo
{
    SdpNetworkType_t networkType;
    SdpAddressType_t addressType;
    const char * pAddress;
    size_t addressLength;
} SdpConnectionInfo_t;

typedef struct SdpOriginator
{
    const char * pUserName;
    size_t userNameLength;
    uint64_t sessionId;
    uint64_t sessionVersion;
    SdpConnectionInfo_t connectionInfo;
} SdpOriginator_t;

typedef struct SdpBandwidthInfo
{
    const char * pBwType;
    size_t bwTypeLength;
    uint64_t sdpBandwidthValue; // kbps
} SdpBandwidthInfo_t;

typedef struct SdpTimeDuration
{
    uint64_t startTime;
    uint64_t stopTime;
} SdpTimeDescription_t;

typedef struct SdpAttribute
{
    const char * pAttributeName;
    size_t attributeNameLength;
    const char * pAttributeValue;
    size_t attributeValueLength;
} SdpAttribute_t;

typedef struct SdpMedia
{
    const char * pMedia;
    size_t mediaLength;
    uint16_t port;
    uint16_t portNum;
    const char * pProtocol;
    size_t protocolLength;
    const char * pFmt;
    size_t fmtLength;
} SdpMedia_t;

/*-----------------------------------------------------------*/

#endif /* SDP_DATA_TYPES_H */
