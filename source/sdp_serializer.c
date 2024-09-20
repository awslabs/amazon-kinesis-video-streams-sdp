/* Standard includes. */
#include <stdio.h>
#include <string.h>

/* Interface includes. */
#include "sdp_serializer.h"

SdpResult_t SdpSerializer_Init( SdpSerializerContext_t * pCtx,
                                char * pBuffer,
                                size_t bufferLength )
{
    SdpResult_t result = SDP_RESULT_OK;

    if( pCtx == NULL )
    {
        result = SDP_RESULT_BAD_PARAM;
    }

    if( result == SDP_RESULT_OK )
    {
        pCtx->pStart = pBuffer;
        pCtx->totalLength = bufferLength;
        pCtx->currentIndex = 0;
    }

    return result;
}
/*-----------------------------------------------------------*/

SdpResult_t SdpSerializer_AddBuffer( SdpSerializerContext_t * pCtx,
                                     uint8_t type,
                                     const char * pValue,
                                     size_t valueLength )
{
    int snprintfRetVal;
    SdpResult_t result = SDP_RESULT_OK;
    size_t remainingLength = 0;
    char * pWriteBuffer = NULL;

    if( ( pCtx == NULL ) ||
        ( pValue == NULL ) ||
        ( valueLength == 0 ) ||
        ( ( pCtx->pStart != NULL ) &&
          ( pCtx->currentIndex > pCtx->totalLength ) ) )
    {
        result = SDP_RESULT_BAD_PARAM;
    }

    if( result == SDP_RESULT_OK )
    {
        if( pCtx->pStart != NULL )
        {
            pWriteBuffer = &( pCtx->pStart[ pCtx->currentIndex ] );
            remainingLength = pCtx->totalLength - pCtx->currentIndex;
        }

        snprintfRetVal = snprintf( pWriteBuffer,
                                   remainingLength,
                                   "%c=%.*s\r\n",
                                   type,
                                   ( int ) valueLength, pValue );
        /* LCOV_EXCL_START */
        if( snprintfRetVal < 0 )
        {
            result = SDP_RESULT_SNPRINTF_ERROR; 
        }
        /* LCOV_EXCL_STOP  */                                    
        else if( ( pWriteBuffer != NULL ) && ( ( size_t ) snprintfRetVal >= remainingLength ) )
        {
            result = SDP_RESULT_OUT_OF_MEMORY;
        }
        else
        {
            pCtx->currentIndex += ( size_t ) snprintfRetVal;
        }
    }

    return result;
}
/*-----------------------------------------------------------*/

SdpResult_t SdpSerializer_AddU32( SdpSerializerContext_t * pCtx,
                                  uint8_t type,
                                  uint32_t value )
{
    int snprintfRetVal;
    SdpResult_t result = SDP_RESULT_OK;
    size_t remainingLength = 0;
    char * pWriteBuffer = NULL;

    if( ( pCtx == NULL ) ||
        ( ( pCtx->pStart != NULL ) &&
          ( pCtx->currentIndex > pCtx->totalLength ) ) )
    {
        result = SDP_RESULT_BAD_PARAM;
    }

    if( result == SDP_RESULT_OK )
    {
        if( pCtx->pStart != NULL )
        {
            pWriteBuffer = &( pCtx->pStart[ pCtx->currentIndex ] );
            remainingLength = pCtx->totalLength - pCtx->currentIndex;
        }

        snprintfRetVal = snprintf( pWriteBuffer,
                                   remainingLength,
                                   "%c"
                                   "=%" SDP_PRINT_FMT_UINT32 "\r\n",
                                   type,
                                   value );
        /* LCOV_EXCL_START */
        if( snprintfRetVal < 0 )
        {
            result = SDP_RESULT_SNPRINTF_ERROR; 
        }
        /* LCOV_EXCL_STOP  */  
        else if( ( pWriteBuffer != NULL ) && ( ( size_t ) snprintfRetVal >= remainingLength ) )
        {
            result = SDP_RESULT_OUT_OF_MEMORY;
        }
        else
        {
            pCtx->currentIndex += ( size_t ) snprintfRetVal;
        }
    }

    return result;
}
/*-----------------------------------------------------------*/

SdpResult_t SdpSerializer_AddU64( SdpSerializerContext_t * pCtx,
                                  uint8_t type,
                                  uint64_t value )
{
    int snprintfRetVal;
    SdpResult_t result = SDP_RESULT_OK;
    size_t remainingLength = 0;
    char * pWriteBuffer = NULL;

    if( ( pCtx == NULL ) ||
        ( ( pCtx->pStart != NULL ) &&
          ( pCtx->currentIndex > pCtx->totalLength ) ) )
    {
        result = SDP_RESULT_BAD_PARAM;
    }

    if( result == SDP_RESULT_OK )
    {
        if( pCtx->pStart != NULL )
        {
            pWriteBuffer = &( pCtx->pStart[ pCtx->currentIndex ] );
            remainingLength = pCtx->totalLength - pCtx->currentIndex;
        }

        snprintfRetVal = snprintf( pWriteBuffer,
                                   remainingLength,
                                   "%c"
                                   "=%" SDP_PRINT_FMT_UINT64 "\r\n",
                                   type,
                                   value );
        /* LCOV_EXCL_START */
        if( snprintfRetVal < 0 )
        {
            result = SDP_RESULT_SNPRINTF_ERROR; 
        }
        /* LCOV_EXCL_STOP  */   
        else if( ( pWriteBuffer != NULL ) && ( ( size_t ) snprintfRetVal >= remainingLength ) )
        {
            result = SDP_RESULT_OUT_OF_MEMORY;
        }
        else
        {
            pCtx->currentIndex += ( size_t ) snprintfRetVal;
        }
    }

    return result;
}
/*-----------------------------------------------------------*/

SdpResult_t SdpSerializer_AddOriginator( SdpSerializerContext_t * pCtx,
                                         uint8_t type,
                                         const SdpOriginator_t * pOriginator )
{
    int snprintfRetVal;
    SdpResult_t result = SDP_RESULT_OK;
    size_t remainingLength = 0;
    char * pWriteBuffer = NULL;

    if( ( pCtx == NULL ) ||
        ( ( pCtx->pStart != NULL ) &&
          ( pCtx->currentIndex > pCtx->totalLength ) ) ||
        ( pOriginator == NULL ) ||
        ( pOriginator->connectionInfo.pAddress == NULL ) ||
        ( pOriginator->connectionInfo.networkType != SDP_NETWORK_IN ) ||
        ( ( pOriginator->connectionInfo.addressType != SDP_ADDRESS_IPV4 ) &&
          ( pOriginator->connectionInfo.addressType != SDP_ADDRESS_IPV6 ) ) )
    {
        result = SDP_RESULT_BAD_PARAM;
    }

    if( result == SDP_RESULT_OK )
    {
        if( pCtx->pStart != NULL )
        {
            pWriteBuffer = &( pCtx->pStart[ pCtx->currentIndex ] );
            remainingLength = pCtx->totalLength - pCtx->currentIndex;
        }

        snprintfRetVal = snprintf( pWriteBuffer,
                                   remainingLength,
                                   "%c"
                                   "=%.*s"
                                   " %" SDP_PRINT_FMT_UINT64
                                   " %" SDP_PRINT_FMT_UINT64
                                   " %.*s"
                                   " %.*s"
                                   " %.*s\r\n",
                                   type,
                                   ( int ) pOriginator->userNameLength, pOriginator->pUserName,
                                   pOriginator->sessionId,
                                   pOriginator->sessionVersion,
                                   2, "IN",
                                   3, pOriginator->connectionInfo.addressType == SDP_ADDRESS_IPV4 ? "IP4" : "IP6",
                                   ( int ) pOriginator->connectionInfo.addressLength, pOriginator->connectionInfo.pAddress );

        /* LCOV_EXCL_START */
        if( snprintfRetVal < 0 )
        {
            result = SDP_RESULT_SNPRINTF_ERROR; 
        }
        /* LCOV_EXCL_STOP  */   
        else if( ( pWriteBuffer != NULL ) && ( ( size_t ) snprintfRetVal >= remainingLength ) )
        {
            result = SDP_RESULT_OUT_OF_MEMORY;
        }
        else
        {
            pCtx->currentIndex += ( size_t ) snprintfRetVal;
        }
    }

    return result;
}
/*-----------------------------------------------------------*/

SdpResult_t SdpSerializer_AddConnectionInfo( SdpSerializerContext_t * pCtx,
                                             uint8_t type,
                                             const SdpConnectionInfo_t * pConnInfo )
{
    int snprintfRetVal;
    SdpResult_t result = SDP_RESULT_OK;
    size_t remainingLength = 0;
    char * pWriteBuffer = NULL;

    if( ( pCtx == NULL ) ||
        ( ( pCtx->pStart != NULL ) &&
          ( pCtx->currentIndex > pCtx->totalLength ) ) ||
        ( pConnInfo == NULL ) ||
        ( pConnInfo->networkType != SDP_NETWORK_IN ) ||
        ( pConnInfo->pAddress == NULL ) ||
        ( ( pConnInfo->addressType != SDP_ADDRESS_IPV4 ) &&
          ( pConnInfo->addressType != SDP_ADDRESS_IPV6 ) ) )
    {
        result = SDP_RESULT_BAD_PARAM;
    }

    if( result == SDP_RESULT_OK )
    {
        if( pCtx->pStart != NULL )
        {
            pWriteBuffer = &( pCtx->pStart[ pCtx->currentIndex ] );
            remainingLength = pCtx->totalLength - pCtx->currentIndex;
        }

        snprintfRetVal = snprintf( pWriteBuffer,
                                   remainingLength,
                                   "%c=%.*s %.*s %.*s\r\n",
                                   type,
                                   2, "IN",
                                   3, pConnInfo->addressType == SDP_ADDRESS_IPV4 ? "IP4" : "IP6",
                                   ( int ) pConnInfo->addressLength, pConnInfo->pAddress );

        /* LCOV_EXCL_START */
        if( snprintfRetVal < 0 )
        {
            result = SDP_RESULT_SNPRINTF_ERROR; 
        }
        /* LCOV_EXCL_STOP  */   
        else if( ( pWriteBuffer != NULL ) && ( ( size_t ) snprintfRetVal >= remainingLength ) )
        {
            result = SDP_RESULT_OUT_OF_MEMORY;
        }
        else
        {
            pCtx->currentIndex += ( size_t ) snprintfRetVal;
        }
    }

    return result;
}
/*-----------------------------------------------------------*/

SdpResult_t SdpSerializer_AddBandwidthInfo( SdpSerializerContext_t * pCtx,
                                            uint8_t type,
                                            const SdpBandwidthInfo_t * pBandwidthInfo )
{
    int snprintfRetVal;
    SdpResult_t result = SDP_RESULT_OK;
    size_t remainingLength = 0;
    char * pWriteBuffer = NULL;

    if( ( pCtx == NULL ) ||
        ( ( pCtx->pStart != NULL ) &&
          ( pCtx->currentIndex > pCtx->totalLength ) ) ||
        ( pBandwidthInfo == NULL ) ||
        ( pBandwidthInfo->pBwType == NULL ) )
    {
        result = SDP_RESULT_BAD_PARAM;
    }

    if( result == SDP_RESULT_OK )
    {
        if( pCtx->pStart != NULL )
        {
            pWriteBuffer = &( pCtx->pStart[ pCtx->currentIndex ] );
            remainingLength = pCtx->totalLength - pCtx->currentIndex;
        }

        snprintfRetVal = snprintf( pWriteBuffer,
                                   remainingLength,
                                   "%c=%.*s"
                                   ":%" SDP_PRINT_FMT_UINT64 "\r\n",
                                   type,
                                   ( int ) pBandwidthInfo->bwTypeLength, pBandwidthInfo->pBwType,
                                   pBandwidthInfo->sdpBandwidthValue );

        /* LCOV_EXCL_START */
        if( snprintfRetVal < 0 )
        {
            result = SDP_RESULT_SNPRINTF_ERROR; 
        }
        /* LCOV_EXCL_STOP  */   
        else if( ( pWriteBuffer != NULL ) && ( ( size_t ) snprintfRetVal >= remainingLength ) )
        {
            result = SDP_RESULT_OUT_OF_MEMORY;
        }
        else
        {
            pCtx->currentIndex += ( size_t ) snprintfRetVal;
        }
    }

    return result;
}
/*-----------------------------------------------------------*/

SdpResult_t SdpSerializer_AddTimeActive( SdpSerializerContext_t * pCtx,
                                         uint8_t type,
                                         const SdpTimeDescription_t * pTimeDescription )
{
    int snprintfRetVal;
    SdpResult_t result = SDP_RESULT_OK;
    size_t remainingLength = 0;
    char * pWriteBuffer = NULL;

    if( ( pCtx == NULL ) ||
        ( ( pCtx->pStart != NULL ) &&
          ( pCtx->currentIndex > pCtx->totalLength ) ) ||
        ( pTimeDescription == NULL ) )
    {
        result = SDP_RESULT_BAD_PARAM;
    }

    if( result == SDP_RESULT_OK )
    {
        if( pCtx->pStart != NULL )
        {
            pWriteBuffer = &( pCtx->pStart[ pCtx->currentIndex ] );
            remainingLength = pCtx->totalLength - pCtx->currentIndex;
        }

        snprintfRetVal = snprintf( pWriteBuffer,
                                   remainingLength,
                                   "%c"
                                   "=%" SDP_PRINT_FMT_UINT64
                                   " %" SDP_PRINT_FMT_UINT64 "\r\n",
                                   type,
                                   pTimeDescription->startTime,
                                   pTimeDescription->stopTime );

        /* LCOV_EXCL_START */
        if( snprintfRetVal < 0 )
        {
            result = SDP_RESULT_SNPRINTF_ERROR; 
        }
        /* LCOV_EXCL_STOP  */ 
        else if( ( pWriteBuffer != NULL ) && ( ( size_t ) snprintfRetVal >= remainingLength ) )
        {
            result = SDP_RESULT_OUT_OF_MEMORY;
        }
        else
        {
            pCtx->currentIndex += ( size_t ) snprintfRetVal;
        }
    }

    return result;
}
/*-----------------------------------------------------------*/

SdpResult_t SdpSerializer_AddAttribute( SdpSerializerContext_t * pCtx,
                                        uint8_t type,
                                        const SdpAttribute_t * pAttribute )
{
    int snprintfRetVal;
    SdpResult_t result = SDP_RESULT_OK;
    size_t remainingLength = 0;
    char * pWriteBuffer = NULL;

    if( ( pCtx == NULL ) ||
        ( ( pCtx->pStart != NULL ) &&
          ( pCtx->currentIndex > pCtx->totalLength ) ) ||
        ( pAttribute == NULL ) ||
        ( pAttribute->pAttributeName == NULL ) )
    {
        result = SDP_RESULT_BAD_PARAM;
    }

    if( result == SDP_RESULT_OK )
    {
        if( pCtx->pStart != NULL )
        {
            pWriteBuffer = &( pCtx->pStart[ pCtx->currentIndex ] );
            remainingLength = pCtx->totalLength - pCtx->currentIndex;
        }

        if( pAttribute->pAttributeValue != NULL )
        {
            snprintfRetVal = snprintf( pWriteBuffer,
                                       remainingLength,
                                       "%c=%.*s:%.*s\r\n",
                                       type,
                                       ( int ) pAttribute->attributeNameLength, pAttribute->pAttributeName,
                                       ( int ) pAttribute->attributeValueLength, pAttribute->pAttributeValue );
        }
        else
        {
            snprintfRetVal = snprintf( pWriteBuffer,
                                       remainingLength,
                                       "%c=%.*s\r\n",
                                       type,
                                       ( int ) pAttribute->attributeNameLength, pAttribute->pAttributeName );
        }

        /* LCOV_EXCL_START */
        if( snprintfRetVal < 0 )
        {
            result = SDP_RESULT_SNPRINTF_ERROR; 
        }
        /* LCOV_EXCL_STOP  */ 
        else if( ( pWriteBuffer != NULL ) && ( ( size_t ) snprintfRetVal >= remainingLength ) )
        {
            result = SDP_RESULT_OUT_OF_MEMORY;
        }
        else
        {
            pCtx->currentIndex += ( size_t ) snprintfRetVal;
        }
    }

    return result;
}
/*-----------------------------------------------------------*/

SdpResult_t SdpSerializer_AddMedia( SdpSerializerContext_t * pCtx,
                                    uint8_t type,
                                    const SdpMedia_t * pMedia )
{
    int snprintfRetVal;
    SdpResult_t result = SDP_RESULT_OK;
    size_t remainingLength = 0;
    char * pWriteBuffer = NULL;

    if( ( pCtx == NULL ) ||
        ( ( pCtx->pStart != NULL ) &&
          ( pCtx->currentIndex > pCtx->totalLength ) ) ||
        ( pMedia == NULL ) ||
        ( pMedia->pProtocol == NULL ) ||
        ( pMedia->pFmt == NULL ) )
    {
        result = SDP_RESULT_BAD_PARAM;
    }

    if( result == SDP_RESULT_OK )
    {
        if( pCtx->pStart != NULL )
        {
            pWriteBuffer = &( pCtx->pStart[ pCtx->currentIndex ] );
            remainingLength = pCtx->totalLength - pCtx->currentIndex;
        }

        if( pMedia->portNum != 0 )
        {
            snprintfRetVal = snprintf( pWriteBuffer,
                                       remainingLength,
                                       "%c"
                                       "=%.*s"
                                       " %" SDP_PRINT_FMT_UINT16
                                       "/%" SDP_PRINT_FMT_UINT16
                                       " %.*s"
                                       " %.*s\r\n",
                                       type,
                                       ( int ) pMedia->mediaLength, pMedia->pMedia,
                                       pMedia->port,
                                       pMedia->portNum,
                                       ( int ) pMedia->protocolLength, pMedia->pProtocol,
                                       ( int ) pMedia->fmtLength, pMedia->pFmt );
        }
        else
        {
            snprintfRetVal = snprintf( pWriteBuffer,
                                       remainingLength,
                                       "%c"
                                       "=%.*s"
                                       " %" SDP_PRINT_FMT_UINT16
                                       " %.*s"
                                       " %.*s\r\n",
                                       type,
                                       ( int ) pMedia->mediaLength, pMedia->pMedia,
                                       pMedia->port,
                                       ( int ) pMedia->protocolLength, pMedia->pProtocol,
                                       ( int ) pMedia->fmtLength, pMedia->pFmt );
        }

        /* LCOV_EXCL_START */
        if( snprintfRetVal < 0 )
        {
            result = SDP_RESULT_SNPRINTF_ERROR; 
        }
        /* LCOV_EXCL_STOP  */  
        else if( ( pWriteBuffer != NULL ) && ( ( size_t ) snprintfRetVal >= remainingLength ) )
        {
            result = SDP_RESULT_OUT_OF_MEMORY;
        }
        else
        {
            pCtx->currentIndex += ( size_t ) snprintfRetVal;
        }
    }

    return result;
}
/*-----------------------------------------------------------*/

SdpResult_t SdpSerializer_Finalize( SdpSerializerContext_t * pCtx,
                                    const char ** pSdpMessage,
                                    size_t * pSdpMessageLength )
{
    SdpResult_t result = SDP_RESULT_OK;

    if( ( pCtx == NULL ) ||
        ( ( pCtx->pStart != NULL ) &&
          ( pCtx->currentIndex > pCtx->totalLength ) ) ||
        ( pSdpMessage == NULL ) ||
        ( pSdpMessageLength == NULL ) )
    {
        result = SDP_RESULT_BAD_PARAM;
    }

    if( result == SDP_RESULT_OK )
    {
        *pSdpMessage = pCtx->pStart;
        *pSdpMessageLength = pCtx->currentIndex;
    }

    return result;
}
/*-----------------------------------------------------------*/
