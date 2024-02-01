#ifndef SDP_DESERIALIZER_H
#define SDP_DESERIALIZER_H

#include "sdp_data_types.h"

typedef struct SdpDeserializerContext
{
    const char * pStart;
    size_t totalLength;
    size_t currentIndex;
} SdpDeserializerContext_t;

/*-----------------------------------------------------------*/

SdpResult_t SdpDeserializer_Init( SdpDeserializerContext_t * pCtx,
                                  const char * pSdpMessage,
                                  size_t sdpMessageLength );

SdpResult_t SdpDeserializer_GetNext( SdpDeserializerContext_t * pCtx,
                                     uint8_t * pType,
                                     const char ** pValue,
                                     size_t * pValueLength );

SdpResult_t SdpDeserializer_ParseOriginator( const char * pValue,
                                             size_t valueLength,
                                             SdpOriginator_t * pOriginator );

SdpResult_t SdpDeserializer_ParseConnectionInfo( const char * pValue,
                                                 size_t valueLength,
                                                 SdpConnectionInfo_t * pConnInfo );

SdpResult_t SdpDeserializer_ParseBandwidthInfo( const char * pValue,
                                                size_t valueLength,
                                                SdpBandwidthInfo_t * pBandwidthInfo );

SdpResult_t SdpDeserializer_ParseTimeActive( const char * pValue,
                                             size_t valueLength,
                                             SdpTimeDescription_t * pTimeDescription );

SdpResult_t SdpDeserializer_ParseAttribute( const char * pValue,
                                            size_t valueLength,
                                            SdpAttribute_t * pAttribute );

SdpResult_t SdpDeserializer_ParseMedia( const char * pValue,
                                        size_t valueLength,
                                        SdpMedia_t * pMedia );
/*-----------------------------------------------------------*/

#endif /* SDP_DESERIALIZER_H */
