/* Unity includes. */
#include "unity.h"
#include "catch_assert.h"

/* Standard includes. */
#include <string.h>
#include <stdint.h>

#include "sdp_deserializer.h"
/* ===========================  EXTERN VARIABLES  =========================== */

#define SDP_TEST_BUFFER_SIZE ( 10000 )

SdpDeserializerContext_t deserializerContext;
char deserializerBuffer[ SDP_TEST_BUFFER_SIZE ];
size_t deserializerBufferLength = SDP_TEST_BUFFER_SIZE;

void setUp( void )
{
    memset( &( deserializerContext ), 0, sizeof( deserializerContext ) );
    memset( &( deserializerBuffer[ 0 ] ), 0, sizeof( deserializerBuffer ) );
    deserializerBufferLength = SDP_TEST_BUFFER_SIZE;
}

void tearDown( void )
{
    // clean stuff up here
}

/* ==============================  Test Cases  ============================== */

/**
 * @brief DeserializerContext is NULL.
 */
void test_SdpDeserializer_Init_NullCtx( void )
{
    SdpResult_t result;

    result = SdpDeserializer_Init( NULL, deserializerBuffer, SDP_TEST_BUFFER_SIZE );
    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief SdpMessage is NULL.
 */
void test_SdpDeserializer_Init_NullMsg( void )
{
    SdpResult_t result;

    result = SdpDeserializer_Init( &( deserializerContext ), NULL, SDP_TEST_BUFFER_SIZE );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EACH_EQUAL_HEX8( 0x00,
                                 &( deserializerContext ),
                                 sizeof( SdpDeserializerContext_t ) );
}

/*-----------------------------------------------------------*/

/**
 * @brief MessageLength is 0.
 */
void test_SdpDeserializer_Init_ZeroBufferLength( void )
{
    SdpResult_t result;

    result = SdpDeserializer_Init( &( deserializerContext ), deserializerBuffer, 0 );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EACH_EQUAL_HEX8( 0x00,
                                 &( deserializerContext ),
                                 sizeof( SdpDeserializerContext_t ) );
    TEST_ASSERT_EACH_EQUAL_HEX8( 0x00,
                                 deserializerBuffer,
                                 deserializerBufferLength );
}

/*-----------------------------------------------------------*/

/**
 * @brief Inputs are valid.
 */
void test_SdpDeserializer_Init_Pass( void )
{
    SdpResult_t result;

    result = SdpDeserializer_Init( &( deserializerContext ), deserializerBuffer, SDP_TEST_BUFFER_SIZE );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL( deserializerBuffer, deserializerContext.pStart );
    TEST_ASSERT_EQUAL( SDP_TEST_BUFFER_SIZE, deserializerContext.totalLength );
    TEST_ASSERT_EQUAL( 0, deserializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief SdpDeserializerContext is NULL.
 */
void test_SdpDeserializer_GetNext_Ctx_NULL( void )
{
    SdpResult_t result;
    const char * pValue;
    size_t valueLength;
    uint8_t type;

    result = SdpDeserializer_GetNext( NULL, &( type ), &( pValue ), &( valueLength ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief type is NULL.
 */
void test_SdpDeserializer_GetNext_Type_NULL( void )
{
    SdpResult_t result;
    const char * pValue;
    size_t valueLength;

    result = SdpDeserializer_GetNext( &( deserializerContext ), NULL, &( pValue ), &( valueLength ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EACH_EQUAL_HEX8( 0x00,
                                 &( deserializerContext ),
                                 sizeof( SdpDeserializerContext_t ) );
}

/*-----------------------------------------------------------*/

/**
 * @brief pValue is NULL.
 */
void test_SdpDeserializer_GetNext_Value_NULL( void )
{
    SdpResult_t result;
    size_t valueLength;
    uint8_t type;

    result = SdpDeserializer_GetNext( &( deserializerContext ), &( type ), NULL, &( valueLength ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EACH_EQUAL_HEX8( 0x00,
                                 &( deserializerContext ),
                                 sizeof( SdpDeserializerContext_t ) );
}

/*-----------------------------------------------------------*/

/**
 * @brief valueLength is NULL.
 */
void test_SdpDeserializer_GetNext_Len_NULL( void )
{
    SdpResult_t result;
    const char * pValue;
    uint8_t type;

    result = SdpDeserializer_GetNext( &( deserializerContext ), &( type ), &( pValue ), NULL );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EACH_EQUAL_HEX8( 0x00,
                                 &( deserializerContext ),
                                 sizeof( SdpDeserializerContext_t ) );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is malformed with incorrect line length.
 */
void test_SdpDeserializer_GetNext_Incorrect_Line_Len( void )
{
    SdpResult_t result;
    const char * pValue;
    size_t valueLength;
    uint8_t type;

    /* Initialize serializer context. */
    deserializerContext.totalLength = deserializerBufferLength;

    /* Each line should have atleast 3 char
     * Make last line to be of incorrect length. */
    deserializerContext.currentIndex = deserializerBufferLength - 2;

    result = SdpDeserializer_GetNext( &( deserializerContext ), &( type ), &( pValue ), &( valueLength ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_MESSAGE_MALFORMED_NOT_ENOUGH_INFO, result );
    TEST_ASSERT_EQUAL( NULL, deserializerContext.pStart );
    TEST_ASSERT_EQUAL( deserializerBufferLength, deserializerContext.totalLength );
    TEST_ASSERT_EQUAL( deserializerBufferLength - 2, deserializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is malformed without '='.
 */
void test_SdpDeserializer_GetNext_Incorrect_Message( void )
{
    SdpResult_t result;
    const char * pValue;
    size_t valueLength;
    uint8_t type;
    /* Incorrect message */
    char buffer[] = "oJode\r\n";
    size_t inputLength = strlen( buffer );

    /* Initialize serializer context. */
    deserializerContext.pStart = buffer;
    deserializerContext.totalLength = inputLength;

    result = SdpDeserializer_GetNext( &( deserializerContext ), &( type ), &( pValue ), &( valueLength ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_MESSAGE_MALFORMED_EQUAL_NOT_FOUND, result );
    TEST_ASSERT_EQUAL( buffer, deserializerContext.pStart );
    TEST_ASSERT_EQUAL( inputLength, deserializerContext.totalLength );
    TEST_ASSERT_EQUAL( 0, deserializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is malformed message ending without '\n'.
 */
void test_SdpDeserializer_GetNext_Incorrect_End( void )
{
    SdpResult_t result;
    const char * pValue;
    size_t valueLength;
    uint8_t type;
    /* Incorrect message without \n*/
    char buffer[] = "o=Jode\r";
    size_t inputLength = strlen( buffer );

    /* Initialize serializer context. */
    deserializerContext.pStart = buffer;
    deserializerContext.totalLength = inputLength;
    deserializerContext.currentIndex = 0;

    result = SdpDeserializer_GetNext( &( deserializerContext ), &( type ), &( pValue ), &( valueLength ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_MESSAGE_MALFORMED_NEWLINE_NOT_FOUND, result );
    TEST_ASSERT_EQUAL( buffer, deserializerContext.pStart );
    TEST_ASSERT_EQUAL( inputLength, deserializerContext.totalLength );
    TEST_ASSERT_EQUAL( 0, deserializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief The current index has reached the end of the message.
 */
void test_SdpDeserializer_GetNext_End( void )
{
    SdpResult_t result;
    const char * pValue;
    size_t valueLength;
    uint8_t type;
    char buffer[] = "v=2\r\n";
    size_t inputLength = strlen( buffer );

    /* Initialize serializer context. */
    deserializerContext.pStart = buffer;
    deserializerContext.totalLength = inputLength;
    deserializerContext.currentIndex = inputLength;

    result = SdpDeserializer_GetNext( &( deserializerContext ), &( type ), &( pValue ), &( valueLength ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_MESSAGE_END, result );
    TEST_ASSERT_EQUAL( buffer, deserializerContext.pStart );
    TEST_ASSERT_EQUAL( inputLength, deserializerContext.totalLength );
    TEST_ASSERT_EQUAL( inputLength, deserializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is empty.
 */
void test_SdpDeserializer_GetNext_Empty_Message( void )
{
    SdpResult_t result;
    const char * pValue;
    size_t valueLength;
    uint8_t type;
    char buffer[] = "v=\r\n";
    size_t inputLength = strlen( buffer );

    /* Initialize serializer context. */
    deserializerContext.pStart = buffer;
    deserializerContext.totalLength = inputLength;
    deserializerContext.currentIndex = 0;

    result = SdpDeserializer_GetNext( &( deserializerContext ), &( type ), &( pValue ), &( valueLength ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_MESSAGE_MALFORMED_NO_VALUE, result );
    TEST_ASSERT_EQUAL( buffer, deserializerContext.pStart );
    TEST_ASSERT_EQUAL( inputLength, deserializerContext.totalLength );
    TEST_ASSERT_EQUAL( 0, deserializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is correct ending with '\r\n''.
 */
void test_SdpDeserializer_GetNext_Pass_n( void )
{
    SdpResult_t result;
    const char * pValue;
    size_t valueLength;
    uint8_t type;
    char buffer[] = "v=2\r\n";
    size_t inputLength = strlen( buffer );

    /* Initialize serializer context. */
    deserializerContext.pStart = buffer;
    deserializerContext.totalLength = inputLength;
    deserializerContext.currentIndex = 0;

    result = SdpDeserializer_GetNext( &( deserializerContext ), &( type ), &( pValue ), &( valueLength ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL( SDP_TYPE_VERSION, type );
    TEST_ASSERT_EQUAL( 1, valueLength );
    TEST_ASSERT_EQUAL_STRING_LEN( "2", pValue, valueLength );
    TEST_ASSERT_EQUAL( buffer, deserializerContext.pStart );
    TEST_ASSERT_EQUAL( inputLength, deserializerContext.totalLength );
    TEST_ASSERT_EQUAL( inputLength, deserializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is correct ending with '\n'.
 */
void test_SdpDeserializer_GetNext_Pass_r( void )
{
    SdpResult_t result;
    const char * pValue;
    size_t valueLength;
    uint8_t type;
    char buffer[] = "v=2\n";
    size_t inputLength = strlen( buffer );

    /* Initialize serializer context. */
    deserializerContext.pStart = buffer;
    deserializerContext.totalLength = inputLength;
    deserializerContext.currentIndex = 0;

    result = SdpDeserializer_GetNext( &( deserializerContext ), &( type ), &( pValue ), &( valueLength ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL( SDP_TYPE_VERSION, type );
    TEST_ASSERT_EQUAL( 1, valueLength );
    TEST_ASSERT_EQUAL_STRING_LEN( "2", pValue, valueLength );
    TEST_ASSERT_EQUAL( buffer, deserializerContext.pStart );
    TEST_ASSERT_EQUAL( inputLength, deserializerContext.totalLength );
    TEST_ASSERT_EQUAL( inputLength, deserializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Validate SDP Deserializer Parse Originator fail functionality for Bad Parameters.
 */
void test_SdpDeserializer_ParseOriginator_BadParams( void )
{
    SdpResult_t result;
    char originatorBuffer[] ="larry ";
    size_t inputLength = strlen( originatorBuffer );
    SdpOriginator_t originator;

    result = SdpDeserializer_ParseOriginator( NULL, 0, &( originator ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );

    result = SdpDeserializer_ParseOriginator( originatorBuffer, inputLength, NULL );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is malformed message without SessionId.
 */
void test_SdpDeserializer_ParseOriginator_NoSessionId( void )
{
    SdpResult_t result;
    char name[] = "larry";
    char originatorBuffer[] ="larry ";
    size_t inputLength = strlen( originatorBuffer );
    SdpOriginator_t originator;

    result = SdpDeserializer_ParseOriginator( originatorBuffer, inputLength, &( originator ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_MESSAGE_MALFORMED_NOT_ENOUGH_INFO, result );
    TEST_ASSERT_EQUAL_STRING_LEN( name, originator.pUserName, originator.userNameLength );
    TEST_ASSERT_EQUAL( 5, originator.userNameLength );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is malformed message without SessionId and incorrect sscanfRetVal return value.
 */
void test_SdpDeserializer_ParseOriginator_NoSessionId2( void )
{
    SdpResult_t result;
    char originatorBuffer[] ="larry  ";
    size_t inputLength = strlen( originatorBuffer );
    SdpOriginator_t originator;

    result = SdpDeserializer_ParseOriginator( originatorBuffer, inputLength, &( originator ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_MESSAGE_MALFORMED_NO_SESSION_ID, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is malformed message without sessionVersion.
 */
void test_SdpDeserializer_ParseOriginator_NoSessionVersion( void )
{
    SdpResult_t result;
    char name[] = "larry";
    char originatorBuffer[] ="larry 2890844526 ";
    size_t inputLength = strlen( originatorBuffer );
    SdpOriginator_t originator;

    result = SdpDeserializer_ParseOriginator( originatorBuffer, inputLength, &( originator ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_MESSAGE_MALFORMED_NOT_ENOUGH_INFO, result );
    TEST_ASSERT_EQUAL_STRING_LEN( name, originator.pUserName, originator.userNameLength );
    TEST_ASSERT_EQUAL( 5, originator.userNameLength );
    TEST_ASSERT_EQUAL( 2890844526, originator.sessionId );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is malformed message without sessionVersion and incorrect sscanfRetVal return value.
 */
void test_SdpDeserializer_ParseOriginator_NoSessionVersion2( void )
{
    SdpResult_t result;
    char name[] = "larry";
    char originatorBuffer[] ="larry 2890844526  ";
    size_t inputLength = strlen( originatorBuffer );
    SdpOriginator_t originator;

    result = SdpDeserializer_ParseOriginator( originatorBuffer, inputLength, &( originator ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_MESSAGE_MALFORMED_NO_SESSION_VERSION, result );
    TEST_ASSERT_EQUAL_STRING_LEN( name, originator.pUserName, originator.userNameLength );
    TEST_ASSERT_EQUAL( 5, originator.userNameLength );
    TEST_ASSERT_EQUAL( 2890844526, originator.sessionId );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is malformed message without ConnectionInfo.
 */
void test_SdpDeserializer_ParseOriginator_NoConnectionInfo( void )
{
    SdpResult_t result;
    char name[] = "larry";
    char originatorBuffer[] ="larry 2890844526 2890842807 ";
    size_t inputLength = strlen( originatorBuffer );
    SdpOriginator_t originator;

    result = SdpDeserializer_ParseOriginator( originatorBuffer, inputLength, &( originator ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_MESSAGE_MALFORMED_NOT_ENOUGH_INFO, result );
    TEST_ASSERT_EQUAL_STRING_LEN( name, originator.pUserName, originator.userNameLength );
    TEST_ASSERT_EQUAL( 5, originator.userNameLength );
    TEST_ASSERT_EQUAL( 2890844526, originator.sessionId );
    TEST_ASSERT_EQUAL( 2890842807, originator.sessionVersion );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is valid.
 */
void test_SdpDeserializer_ParseOriginator_Pass( void )
{
    SdpResult_t result;
    char name[] = "larry";
    char addv4[] = "128.112.136.10";
    char originatorBuffer[] ="larry 2890844526 2890842807 IN IP4 128.112.136.10";
    size_t inputLength = strlen( originatorBuffer );
    SdpOriginator_t originator;

    result = SdpDeserializer_ParseOriginator( originatorBuffer, inputLength, &( originator ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL_STRING_LEN( name, originator.pUserName, originator.userNameLength );
    TEST_ASSERT_EQUAL( 5, originator.userNameLength );
    TEST_ASSERT_EQUAL( 2890844526, originator.sessionId );
    TEST_ASSERT_EQUAL( 2890842807, originator.sessionVersion );
    TEST_ASSERT_EQUAL( SDP_NETWORK_IN, originator.connectionInfo.networkType );
    TEST_ASSERT_EQUAL( SDP_ADDRESS_IPV4, originator.connectionInfo.addressType );
    TEST_ASSERT_EQUAL( strlen( addv4 ), originator.connectionInfo.addressLength );
    TEST_ASSERT_EQUAL_STRING_LEN( addv4,originator.connectionInfo.pAddress, originator.connectionInfo.addressLength );
}

/*-----------------------------------------------------------*/

/**
 * @brief Validate SDP Deserializer Parse Connection Info fail functionality for Bad Parameters.
 */
void test_SdpDeserializer_ParseConnectionInfo_BadParams( void )
{
    SdpResult_t result;
    char originatorBuffer[] ="INe IP4 128.112.136.10";
    size_t inputLength = strlen( originatorBuffer );
    SdpConnectionInfo_t connInfo;

    result = SdpDeserializer_ParseConnectionInfo( NULL, 0, &( connInfo ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );

    result = SdpDeserializer_ParseConnectionInfo( originatorBuffer, inputLength, NULL );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is malformed with incorrect networkType length.
 */
void test_SdpDeserializer_ParseConnectionInfo_IncorrectNetworkTypeLength( void )
{
    SdpResult_t result;
    char originatorBuffer[] ="INe IP4 128.112.136.10";
    size_t inputLength = strlen( originatorBuffer );
    SdpConnectionInfo_t ConnInfo;

    result = SdpDeserializer_ParseConnectionInfo( originatorBuffer, inputLength, &( ConnInfo ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_MESSAGE_MALFORMED_INVALID_NETWORK_TYPE, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is malformed with incorrect networkType.
 */
void test_SdpDeserializer_ParseConnectionInfo_IncorrectNetworkType( void )
{
    SdpResult_t result;
    char originatorBuffer[] ="IX IP4 128.112.136.10";
    size_t inputLength = strlen( originatorBuffer );
    SdpConnectionInfo_t ConnInfo;

    result = SdpDeserializer_ParseConnectionInfo( originatorBuffer, inputLength, &( ConnInfo ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_MESSAGE_MALFORMED_INVALID_NETWORK_TYPE, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is malformed with incorrect addressType.
 */
void test_SdpDeserializer_ParseConnectionInfo_IncorrectAddressType( void )
{
    SdpResult_t result;
    char originatorBuffer[] ="IN IPx 128.112.136.10";
    size_t inputLength = strlen( originatorBuffer );
    SdpConnectionInfo_t ConnInfo;

    result = SdpDeserializer_ParseConnectionInfo( originatorBuffer, inputLength, &( ConnInfo ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_MESSAGE_MALFORMED_INVALID_ADDRESS_TYPE, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is malformed with incorrect addressType length.
 */
void test_SdpDeserializer_ParseConnectionInfo_IncorrectAddressTypeLength( void )
{
    SdpResult_t result;
    char originatorBuffer[] ="IN IP4x 128.112.136.10";
    size_t inputLength = strlen( originatorBuffer );
    SdpConnectionInfo_t ConnInfo;

    result = SdpDeserializer_ParseConnectionInfo( originatorBuffer, inputLength, &( ConnInfo ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_MESSAGE_MALFORMED_INVALID_ADDRESS_TYPE, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is malformed with no pAddress.
 */
void test_SdpDeserializer_ParseConnectionInfo_NoAddress( void )
{
    SdpResult_t result;
    char originatorBuffer[] ="IN IP4";
    size_t inputLength = strlen( originatorBuffer );
    SdpConnectionInfo_t ConnInfo;

    result = SdpDeserializer_ParseConnectionInfo( originatorBuffer, inputLength, &( ConnInfo ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_MESSAGE_MALFORMED_NOT_ENOUGH_INFO, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is malformed with redundant information.
 */
void test_SdpDeserializer_ParseConnectionInfo_RedundantInfo( void )
{
    SdpResult_t result;
    char originatorBuffer[] ="IN IP4 192.168.1.1 192.168.2.2";
    size_t inputLength = strlen( originatorBuffer );
    SdpConnectionInfo_t ConnInfo;

    result = SdpDeserializer_ParseConnectionInfo( originatorBuffer, inputLength, &( ConnInfo ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_MESSAGE_MALFORMED_REDUNDANT_INFO, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is valid with IPv4 networkType.
 */
void test_SdpDeserializer_ParseConnectionInfo_PassIPv4( void )
{
    SdpResult_t result;
    char addv4[] = "128.112.136.10";
    char originatorBuffer[] ="IN IP4 128.112.136.10";
    size_t inputLength = strlen( originatorBuffer );
    SdpConnectionInfo_t ConnInfo;

    result = SdpDeserializer_ParseConnectionInfo( originatorBuffer, inputLength, &( ConnInfo ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL( SDP_NETWORK_IN, ConnInfo.networkType );
    TEST_ASSERT_EQUAL( SDP_ADDRESS_IPV4, ConnInfo.addressType );
    TEST_ASSERT_EQUAL( strlen( addv4 ), ConnInfo.addressLength );
    TEST_ASSERT_EQUAL_STRING_LEN( addv4, ConnInfo.pAddress, ConnInfo.addressLength );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is valid with IPv6 networkType.
 */
void test_SdpDeserializer_ParseConnectionInfo_PassIPv6( void )
{
    SdpResult_t result;
    char addv6[] = "2201:056D::112E:144A:1E24";
    char originatorBuffer[] ="IN IP6 2201:056D::112E:144A:1E24";
    size_t inputLength = strlen( originatorBuffer );
    SdpConnectionInfo_t ConnInfo;

    result = SdpDeserializer_ParseConnectionInfo( originatorBuffer, inputLength, &( ConnInfo ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL( SDP_NETWORK_IN, ConnInfo.networkType );
    TEST_ASSERT_EQUAL( SDP_ADDRESS_IPV6, ConnInfo.addressType );
    TEST_ASSERT_EQUAL( strlen( addv6 ), ConnInfo.addressLength );
    TEST_ASSERT_EQUAL( strlen( addv6 ), ConnInfo.addressLength );
    TEST_ASSERT_EQUAL_STRING_LEN( addv6, ConnInfo.pAddress, ConnInfo.addressLength );
}

/*-----------------------------------------------------------*/

/**
 * @brief Validate SDP Deserializer Parse Bandwidth Info fail functionality for Bad Parameters.
 */
void test_SdpDeserializer_ParseBandwidthInfo_BadParams( void )
{
    SdpResult_t result;
    char originatorBuffer[] ="X-YZ:";
    size_t inputLength = strlen( originatorBuffer );
    SdpBandwidthInfo_t bandwidth;

    result = SdpDeserializer_ParseBandwidthInfo( NULL, 0, &( bandwidth ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );

    result = SdpDeserializer_ParseBandwidthInfo( originatorBuffer, inputLength, NULL );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is malformed with no sdpBandwidthValue.
 */
void test_SdpDeserializer_ParseBandwidthInfo_NoBandwidthValue( void )
{
    SdpResult_t result;
    char originatorBuffer[] ="X-YZ:";
    size_t inputLength = strlen( originatorBuffer );
    SdpBandwidthInfo_t bandwidth;

    result = SdpDeserializer_ParseBandwidthInfo( originatorBuffer, inputLength, &( bandwidth ) );
    TEST_ASSERT_EQUAL( SDP_RESULT_MESSAGE_MALFORMED_INVALID_BANDWIDTH, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is malformed without colon.
 */
void test_SdpDeserializer_ParseBandwidthInfo_NoColon( void )
{
    SdpResult_t result;
    char originatorBuffer[] ="X-YZ";
    size_t inputLength = strlen( originatorBuffer );
    SdpBandwidthInfo_t bandwidth;

    result = SdpDeserializer_ParseBandwidthInfo( originatorBuffer, inputLength, &( bandwidth ) );
    TEST_ASSERT_EQUAL( SDP_RESULT_MESSAGE_MALFORMED_NOT_ENOUGH_INFO, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief The bandwidth valid.
 */
void test_SdpDeserializer_ParseBandwidthInfo_Pass( void )
{
    SdpResult_t result;
    char BwType[] = "X-YZ";
    char originatorBuffer[] ="X-YZ:128";
    size_t inputLength = strlen( originatorBuffer );
    SdpBandwidthInfo_t bandwidth;

    result = SdpDeserializer_ParseBandwidthInfo( originatorBuffer, inputLength, &( bandwidth ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL( strlen( BwType ), bandwidth.bwTypeLength );
    TEST_ASSERT_EQUAL( 128, bandwidth.sdpBandwidthValue );
    TEST_ASSERT_EQUAL_STRING_LEN( BwType, bandwidth.pBwType, bandwidth.bwTypeLength );
}

/*-----------------------------------------------------------*/

/**
 * @brief Validate SDP Deserializer Parse Time Active fail functionality for Bad Parameters.
 */
void test_SdpDeserializer_ParseTimeActive_BadParams( void )
{
    SdpResult_t result;
    char originatorBuffer[] ="0 0";
    size_t inputLength = strlen( originatorBuffer );
    SdpTimeDescription_t timeDescription;

    result = SdpDeserializer_ParseTimeActive( NULL, 0, &( timeDescription ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );

    result = SdpDeserializer_ParseTimeActive( originatorBuffer, inputLength, NULL );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is malformed without time.
 */
void test_SdpDeserializer_ParseTimeActive_WithoutTime( void )
{
    SdpResult_t result;
    char originatorBuffer[] =" ";
    size_t inputLength = strlen( originatorBuffer );
    SdpTimeDescription_t timeDescription;

    result = SdpDeserializer_ParseTimeActive( originatorBuffer, inputLength, &( timeDescription ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_MESSAGE_MALFORMED_INVALID_START_TIME, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is malformed without stopTime.
 */
void test_SdpDeserializer_ParseTimeActive_NoStopTime( void )
{
    SdpResult_t result;
    char originatorBuffer[] ="0 ";
    size_t inputLength = strlen( originatorBuffer );
    SdpTimeDescription_t timeDescription;

    result = SdpDeserializer_ParseTimeActive( originatorBuffer, inputLength, &( timeDescription ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_MESSAGE_MALFORMED_INVALID_STOP_TIME, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is malformed without space and stopTime.
 */
void test_SdpDeserializer_ParseTimeActive_NoSpace( void )
{
    SdpResult_t result;
    char originatorBuffer[] ="0";
    size_t inputLength = strlen( originatorBuffer );
    SdpTimeDescription_t timeDescription;

    result = SdpDeserializer_ParseTimeActive( originatorBuffer, inputLength, &( timeDescription ) );
    TEST_ASSERT_EQUAL( SDP_RESULT_MESSAGE_MALFORMED_NOT_ENOUGH_INFO, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief The message is valid.
 */
void test_SdpDeserializer_ParseTimeActive_Pass( void )
{
    SdpResult_t result;
    char originatorBuffer[] ="0 0";
    size_t inputLength = strlen( originatorBuffer );
    SdpTimeDescription_t timeDescription;

    result = SdpDeserializer_ParseTimeActive( originatorBuffer, inputLength, &( timeDescription ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL( 0, timeDescription.startTime );
    TEST_ASSERT_EQUAL( 0, timeDescription.stopTime );
}

/*-----------------------------------------------------------*/

/**
 * @brief Validate SDP Deserializer Parse Attribute fail functionality for Bad Parameters.
 */
void test_SdpDeserializer_ParseAttribute_BadParams( void )
{
    SdpResult_t result;
    char attributeString[] = "rtpmap:126 telephone-event/8000";
    size_t attributeStringLength = strlen( attributeString );
    SdpAttribute_t attribute;

    memset( &( attribute ), 0, sizeof( SdpAttribute_t ) );

    result = SdpDeserializer_ParseAttribute( NULL, 0, &( attribute ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );

    result = SdpDeserializer_ParseAttribute( attributeString, attributeStringLength, NULL );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief Inputs are valid.
 */
void test_SdpDeserializer_ParseAttribute_Pass( void )
{
    SdpResult_t result;
    char attributeString[] = "rtpmap:126 telephone-event/8000";
    size_t attributeStringLength = strlen( attributeString );
    char expectAttributeName[] = "rtpmap";
    size_t expectAttributeNameLength = strlen( expectAttributeName );
    char expectAttributeValue[] = "126 telephone-event/8000";
    size_t expectAttributeValueLength = strlen( expectAttributeValue );
    SdpAttribute_t attribute;

    memset( &( attribute ), 0, sizeof( SdpAttribute_t ) );

    result = SdpDeserializer_ParseAttribute( attributeString, attributeStringLength, &( attribute ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL_size_t( expectAttributeNameLength, attribute.attributeNameLength );
    TEST_ASSERT_EQUAL_size_t( expectAttributeValueLength, attribute.attributeValueLength );
    TEST_ASSERT_EQUAL_STRING_LEN( expectAttributeName, attribute.pAttributeName, attribute.attributeNameLength );
    TEST_ASSERT_EQUAL_STRING_LEN( expectAttributeValue, attribute.pAttributeValue, attribute.attributeValueLength );
}

/*-----------------------------------------------------------*/

/**
 * @brief Inputs are valid without attribute value.
 */
void test_SdpDeserializer_ParseAttribute_PassNoAttributeValue( void )
{
    SdpResult_t result;
    char attributeString[] = "recvonly";
    size_t attributeStringLength = strlen( attributeString );
    char expectAttributeName[] = "recvonly";
    size_t expectAttributeNameLength = strlen( expectAttributeName );
    SdpAttribute_t attribute;

    memset( &( attribute ), 0, sizeof( SdpAttribute_t ) );

    result = SdpDeserializer_ParseAttribute( attributeString, attributeStringLength, &( attribute ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL_size_t( expectAttributeNameLength, attribute.attributeNameLength );
    TEST_ASSERT_EQUAL_size_t( 0, attribute.attributeValueLength );
    TEST_ASSERT_EQUAL_STRING_LEN( expectAttributeName, attribute.pAttributeName, attribute.attributeNameLength );
    TEST_ASSERT_EQUAL_STRING_LEN( NULL, attribute.pAttributeValue, attribute.attributeValueLength );
}

/*-----------------------------------------------------------*/

/**
 * @brief Validate SDP Deserializer Parse Media fail functionality for Bad Parameters.
 */
void test_SdpDeserializer_ParseMedia_BadParams( void )
{
    SdpResult_t result;
    char mediaString[] = "video 9/2 UDP/TLS/RTP/SAVPF 96 97 102 103 104 105 106 107 108 109 127 125 39 40 45 46 98 99 100 101 112 113 116 117 118";
    size_t mediaStringLength = strlen( mediaString );
    SdpMedia_t media;

    memset( &( media ), 0, sizeof( SdpMedia_t ) );

    result = SdpDeserializer_ParseMedia( NULL, 0, &( media ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );

    result = SdpDeserializer_ParseMedia( mediaString, mediaStringLength, NULL );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief Inputs are valid.
 */
void test_SdpDeserializer_ParseMedia_Pass( void )
{
    SdpResult_t result;
    char mediaString[] = "video 9/2 UDP/TLS/RTP/SAVPF 96 97 102 103 104 105 106 107 108 109 127 125 39 40 45 46 98 99 100 101 112 113 116 117 118";
    size_t mediaStringLength = strlen( mediaString );
    char expectMediaName[] = "video";
    size_t expectMediaNameLength = strlen( expectMediaName );
    uint16_t expectPort = 9;
    uint16_t expectPortNum = 2;
    char expectProtocol[] = "UDP/TLS/RTP/SAVPF";
    size_t expectProtocolLength = strlen( expectProtocol );
    char expectFmt[] = "96 97 102 103 104 105 106 107 108 109 127 125 39 40 45 46 98 99 100 101 112 113 116 117 118";
    size_t expectFmtLength = strlen( expectFmt );

    SdpMedia_t media;

    memset( &( media ), 0, sizeof( SdpMedia_t ) );

    result = SdpDeserializer_ParseMedia( mediaString, mediaStringLength, &( media ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL_size_t( expectMediaNameLength, media.mediaLength );
    TEST_ASSERT_EQUAL_STRING_LEN( expectMediaName, media.pMedia, media.mediaLength );
    TEST_ASSERT_EQUAL_UINT16( expectPort, media.port );
    TEST_ASSERT_EQUAL_UINT16( expectPortNum, media.portNum );
    TEST_ASSERT_EQUAL_size_t( expectProtocolLength, media.protocolLength );
    TEST_ASSERT_EQUAL_STRING_LEN( expectProtocol, media.pProtocol, media.protocolLength );
    TEST_ASSERT_EQUAL_size_t( expectFmtLength, media.fmtLength );
    TEST_ASSERT_EQUAL_STRING_LEN( expectFmt, media.pFmt, media.fmtLength );
}

/*-----------------------------------------------------------*/

/**
 * @brief Inputs are valid without number of port.
 */
void test_SdpDeserializer_ParseMedia_PassNoPortNum( void )
{
    SdpResult_t result;
    char mediaString[] = "video 9 UDP/TLS/RTP/SAVPF 96 97 102 103 104 105 106 107 108 109 127 125 39 40 45 46 98 99 100 101 112 113 116 117 118";
    size_t mediaStringLength = strlen( mediaString );
    char expectMediaName[] = "video";
    size_t expectMediaNameLength = strlen( expectMediaName );
    uint16_t expectPort = 9;
    uint16_t expectPortNum = 0;
    char expectProtocol[] = "UDP/TLS/RTP/SAVPF";
    size_t expectProtocolLength = strlen( expectProtocol );
    char expectFmt[] = "96 97 102 103 104 105 106 107 108 109 127 125 39 40 45 46 98 99 100 101 112 113 116 117 118";
    size_t expectFmtLength = strlen( expectFmt );

    SdpMedia_t media;

    memset( &( media ), 0, sizeof( SdpMedia_t ) );

    result = SdpDeserializer_ParseMedia( mediaString, mediaStringLength, &( media ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL_size_t( expectMediaNameLength, media.mediaLength );
    TEST_ASSERT_EQUAL_STRING_LEN( expectMediaName, media.pMedia, media.mediaLength );
    TEST_ASSERT_EQUAL_UINT16( expectPort, media.port );
    TEST_ASSERT_EQUAL_UINT16( expectPortNum, media.portNum );
    TEST_ASSERT_EQUAL_size_t( expectProtocolLength, media.protocolLength );
    TEST_ASSERT_EQUAL_STRING_LEN( expectProtocol, media.pProtocol, media.protocolLength );
    TEST_ASSERT_EQUAL_size_t( expectFmtLength, media.fmtLength );
    TEST_ASSERT_EQUAL_STRING_LEN( expectFmt, media.pFmt, media.fmtLength );
}

/*-----------------------------------------------------------*/

/**
 * @brief Port is invalid so SdpDeserializer_ParseMedia returns error.
 */
void test_SdpDeserializer_ParseMedia_InvalidPort( void )
{
    SdpResult_t result;
    char mediaString[] = "video abc UDP/TLS/RTP/SAVPF 96 97 102 103 104 105 106 107 108 109 127 125 39 40 45 46 98 99 100 101 112 113 116 117 118";
    size_t mediaStringLength = strlen( mediaString );
    SdpMedia_t media;

    memset( &( media ), 0, sizeof( SdpMedia_t ) );

    result = SdpDeserializer_ParseMedia( mediaString, mediaStringLength, &( media ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_MESSAGE_MALFORMED_INVALID_PORT, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief Port number is invalid so SdpDeserializer_ParseMedia returns error.
 */
void test_SdpDeserializer_ParseMedia_InvalidPortNum( void )
{
    SdpResult_t result;
    char mediaString[] = "video 9/abc UDP/TLS/RTP/SAVPF 96 97 102 103 104 105 106 107 108 109 127 125 39 40 45 46 98 99 100 101 112 113 116 117 118";
    size_t mediaStringLength = strlen( mediaString );
    SdpMedia_t media;

    memset( &( media ), 0, sizeof( SdpMedia_t ) );

    result = SdpDeserializer_ParseMedia( mediaString, mediaStringLength, &( media ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_MESSAGE_MALFORMED_INVALID_PORTNUM, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief Input media lack format information.
 */
void test_SdpDeserializer_ParseMedia_InvalidMediaString( void )
{
    SdpResult_t result;
    char mediaString[] = "video 9/2 UDP/TLS/RTP/SAVPF";
    size_t mediaStringLength = strlen( mediaString );
    SdpMedia_t media;

    memset( &( media ), 0, sizeof( SdpMedia_t ) );

    result = SdpDeserializer_ParseMedia( mediaString, mediaStringLength, &( media ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_MESSAGE_MALFORMED_NOT_ENOUGH_INFO, result );
}

/*-----------------------------------------------------------*/
