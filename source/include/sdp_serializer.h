#ifndef SDP_SERIALIZER_H
#define SDP_SERIALIZER_H

#include "sdp_data_types.h"

typedef struct SdpSerializerContext
{
    char * pStart;
    size_t totalLength;
    size_t currentIndex;
} SdpSerializerContext_t;

/*-----------------------------------------------------------*/

SdpResult_t SdpSerializer_Init( SdpSerializerContext_t * pCtx,
                                char * pBuffer,
                                size_t bufferLength );

SdpResult_t SdpSerializer_AddBuffer( SdpSerializerContext_t * pCtx,
                                     uint8_t type,
                                     const char * pValue,
                                     size_t valueLength );

SdpResult_t SdpSerializer_AddU32( SdpSerializerContext_t * pCtx,
                                  uint8_t type,
                                  uint32_t value );

SdpResult_t SdpSerializer_AddU64( SdpSerializerContext_t * pCtx,
                                  uint8_t type,
                                  uint64_t value );

SdpResult_t SdpSerializer_AddOriginator( SdpSerializerContext_t * pCtx,
                                         uint8_t type,
                                         const SdpOriginator_t * pOriginator );

SdpResult_t SdpSerializer_AddConnectionInfo( SdpSerializerContext_t * pCtx,
                                             uint8_t type,
                                             const SdpConnectionInfo_t * pConnInfo );

SdpResult_t SdpSerializer_AddBandwidthInfo( SdpSerializerContext_t * pCtx,
                                            uint8_t type,
                                            const SdpBandwidthInfo_t * pBandwidthInfo );

SdpResult_t SdpSerializer_AddTimeActive( SdpSerializerContext_t * pCtx,
                                         uint8_t type,
                                         const SdpTimeDescription_t * pTimeDescription );

SdpResult_t SdpSerializer_AddAttribute( SdpSerializerContext_t * pCtx,
                                        uint8_t type,
                                        const SdpAttribute_t * pAttribute );

SdpResult_t SdpSerializer_AddMedia( SdpSerializerContext_t * pCtx,
                                    uint8_t type,
                                    const SdpMedia_t * pMedia );

SdpResult_t SdpSerializer_Finalize( SdpSerializerContext_t * pCtx,
                                    const char ** pSdpMessage,
                                    size_t * pSdpMessageLength );

/* Currently repeat times ("r="), time zone adjustment ("z=") and, encryption
 * keys ("k=") are not supported. */

/*-----------------------------------------------------------*/

#endif /* SDP_SERIALIZER_H */
