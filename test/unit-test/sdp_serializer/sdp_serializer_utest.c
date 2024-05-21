/* Unity includes. */
#include "unity.h"
#include "catch_assert.h"

/* Standard includes. */
#include <string.h>
#include <stdint.h>

#include "sdp_serializer.h"
/* ===========================  EXTERN VARIABLES  =========================== */

#define SDP_TEST_BUFFER_SIZE ( 10000 )

SdpSerializerContext_t serializerContext;
char serializerBuffer[ SDP_TEST_BUFFER_SIZE] ;
size_t serializerBufferLength = SDP_TEST_BUFFER_SIZE;

void setUp( void )
{
    memset( &( serializerContext ), 0, sizeof( serializerContext ) );
    memset( &( serializerBuffer[ 0 ] ), 0, sizeof( serializerBuffer ) );
    serializerBufferLength = SDP_TEST_BUFFER_SIZE;
}

void tearDown( void )
{
}

/* ==============================  Test Cases  ============================== */

/**
 * @brief Inputs are valid.
 */
void test_SdpSerializer_Init_Pass( void )
{
    SdpResult_t result;
    char buffer[ 1 ];
    size_t bufferLength = 1;

    result = SdpSerializer_Init( &( serializerContext ), &( buffer[ 0 ] ), bufferLength );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL( &( buffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( 0, serializerContext.currentIndex );
    TEST_ASSERT_EQUAL( 1, serializerContext.totalLength );
}

/*-----------------------------------------------------------*/

/**
 * @brief Context is NULL.
 */
void test_SdpSerializer_Init_NullCtx( void )
{
    SdpResult_t result;
    char buffer[ 1 ];
    size_t bufferLength = 1;

    result = SdpSerializer_Init( NULL, &( buffer[ 0 ] ), bufferLength );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief Input buffer is NULL.
 */
void test_SdpSerializer_Init_NullBuffer( void )
{
    SdpResult_t result;
    size_t bufferLength = 1;

    result = SdpSerializer_Init( &( serializerContext ), NULL, bufferLength );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL( NULL, serializerContext.pStart );
    TEST_ASSERT_EQUAL( 0, serializerContext.currentIndex );
    TEST_ASSERT_EQUAL( 1, serializerContext.totalLength );
}

/*-----------------------------------------------------------*/

/**
 * @brief Inputs are valid.
 */
void test_SdpSerializer_AddBuffer_Pass( void )
{
    SdpResult_t result;
    char expectOutput[] = "s=-\r\n";
    char inputString[] = "-";
    size_t inputLength = 1;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    result = SdpSerializer_AddBuffer( &( serializerContext ),
                                      's',
                                      &( inputString[ 0 ] ),
                                      inputLength );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL_STRING( &( expectOutput ), &( serializerBuffer[ 0 ] ) );
}

/*-----------------------------------------------------------*/

/**
 * @brief Context is NULL.
 */
void test_SdpSerializer_AddBuffer_NullContext( void )
{
    SdpResult_t result;
    char inputString[] = "-";
    size_t inputLength = 1;

    result = SdpSerializer_AddBuffer( NULL,
                                      's',
                                      &( inputString[ 0 ] ),
                                      inputLength );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
}
/*-----------------------------------------------------------*/

/**
 * @brief Context is invalid.
 */
void test_SdpSerializer_AddBuffer_InvalidContext( void )
{
    SdpResult_t result;
    char inputString[] = "-";
    size_t inputLength = 1;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    /* Move currentIndex past totalLength. */
    serializerContext.currentIndex = serializerBufferLength + 1;

    result = SdpSerializer_AddBuffer( &( serializerContext ),
                                      's',
                                      &( inputString[ 0 ] ),
                                      inputLength );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EACH_EQUAL_HEX8( 0x00,
                                 &( serializerBuffer[ 0 ] ),
                                 serializerBufferLength );
}

/*-----------------------------------------------------------*/

/**
 * @brief Input buffer is NULL.
 */
void test_SdpSerializer_AddBuffer_NullBuffer( void )
{
    SdpResult_t result;
    size_t inputLength = 1;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    result = SdpSerializer_AddBuffer( &( serializerContext ),
                                      's',
                                      NULL,
                                      inputLength );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( 0, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Input string length is 0.
 */
void test_SdpSerializer_AddBuffer_ZeroBufferLength( void )
{
    SdpResult_t result;
    char inputString[] = "-";
    size_t inputLength = 0;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    result = SdpSerializer_AddBuffer( &( serializerContext ),
                                      's',
                                      &( inputString[ 0 ] ),
                                      inputLength );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( 0, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Output buffer is NULL.
 */
void test_SdpSerializer_AddBuffer_NullOutputBuffer( void )
{
    SdpResult_t result;
    char inputString[] = "-";
    size_t inputLength = sizeof( inputString );
    char expectOutput[] = "s=-\r\n";

    /* Initialize serializer context. */
    serializerContext.pStart = NULL;
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    result = SdpSerializer_AddBuffer( &( serializerContext ),
                                      's',
                                      &( inputString[ 0 ] ),
                                      inputLength );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL( NULL, serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( strlen( expectOutput ), serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief The remaining size of output buffer is not enough for adding buffer.
 */
void test_SdpSerializer_AddBuffer_OutputBufferNotEnough( void )
{
    SdpResult_t result;
    char inputString[] = "abcdefghijklmnopqrstuvwxyz";
    size_t inputLength = sizeof( inputString );

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = inputLength - 1;
    serializerContext.currentIndex = 0;

    result = SdpSerializer_AddBuffer( &( serializerContext ),
                                      's',
                                      &( inputString[ 0 ] ),
                                      inputLength );

    TEST_ASSERT_EQUAL( SDP_RESULT_OUT_OF_MEMORY, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief Inputs are valid.
 */
void test_SdpSerializer_AddU32_Pass( void )
{
    SdpResult_t result;
    char expectOutput[] = "v=123\r\n";
    uint32_t version = 123;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    result = SdpSerializer_AddU32( &( serializerContext ),
                                   'v',
                                   version );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL_STRING( &( expectOutput ), &( serializerBuffer[ 0 ] ) );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( strlen( expectOutput ), serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Context is NULL.
 */
void test_SdpSerializer_AddU32_NullContext( void )
{
    SdpResult_t result;
    uint32_t version = 123;

    result = SdpSerializer_AddU32( NULL,
                                   'v',
                                   version );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief Context is invalid.
 */
void test_SdpSerializer_AddU32_InvalidContext( void )
{
    SdpResult_t result;
    uint32_t version = 123;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    /* Move currentIndex past totalLength. */
    serializerContext.currentIndex = serializerBufferLength + 1;

    result = SdpSerializer_AddU32( &( serializerContext ),
                                   'v',
                                   version );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EACH_EQUAL_HEX8( 0x00,
                                 &( serializerBuffer[ 0 ] ),
                                 serializerBufferLength );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( serializerBufferLength + 1, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Output buffer is NULL.
 */
void test_SdpSerializer_AddU32_NullOutputBuffer( void )
{
    SdpResult_t result;
    uint32_t version = 123;
    char expectOutput[] = "v=123\r\n";

    /* Initialize serializer context. */
    serializerContext.pStart = NULL;
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    result = SdpSerializer_AddU32( &( serializerContext ),
                                   'v',
                                   version );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL( NULL, serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( strlen( expectOutput ), serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief The remaining size of output buffer is not enough for adding buffer.
 */
void test_SdpSerializer_AddU32_OutputBufferNotEnough( void )
{
    SdpResult_t result;
    uint32_t version = 123;
    size_t inputLength = 5; /* "v=123\r\n" needs 5 bytes. */

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = inputLength - 1;
    serializerContext.currentIndex = 0;

    result = SdpSerializer_AddU32( &( serializerContext ),
                                   'v',
                                   version );

    TEST_ASSERT_EQUAL( SDP_RESULT_OUT_OF_MEMORY, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief Inputs are valid.
 */
void test_SdpSerializer_AddU64_Pass( void )
{
    SdpResult_t result;
    char expectOutput[] = "v=123\r\n";
    uint64_t version = 123;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    result = SdpSerializer_AddU64( &( serializerContext ),
                                   'v',
                                   version );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL_STRING( &( expectOutput ), &( serializerBuffer[ 0 ] ) );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( strlen( expectOutput ), serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Context is NULL.
 */
void test_SdpSerializer_AddU64_NullContext( void )
{
    SdpResult_t result;
    uint64_t version = 123;

    result = SdpSerializer_AddU64( NULL,
                                   'v',
                                   version );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief Context is invalid.
 */
void test_SdpSerializer_AddU64_InvalidContext( void )
{
    SdpResult_t result;
    uint64_t version = 123;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    /* Move currentIndex past totalLength. */
    serializerContext.currentIndex = serializerBufferLength + 1;

    result = SdpSerializer_AddU64( &( serializerContext ),
                                   'v',
                                   version );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EACH_EQUAL_HEX8( 0x00,
                                 &( serializerBuffer[ 0 ] ),
                                 serializerBufferLength );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( serializerBufferLength + 1, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Output buffer is NULL.
 */
void test_SdpSerializer_AddU64_NullOutputBuffer( void )
{
    SdpResult_t result;
    char expectOutput[] = "v=123\r\n";
    uint64_t version = 123;

    /* Initialize serializer context. */
    serializerContext.pStart = NULL;
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    result = SdpSerializer_AddU64( &( serializerContext ),
                                   'v',
                                   version );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL( NULL, serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( strlen( expectOutput ), serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief The remaining size of output buffer is not enough for adding buffer.
 */
void test_SdpSerializer_AddU64_OutputBufferNotEnough( void )
{
    SdpResult_t result;
    uint64_t version = 123;
    size_t inputLength = 5; /* "v=123\r\n" needs 5 bytes. */

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = inputLength - 1;
    serializerContext.currentIndex = 0;

    result = SdpSerializer_AddU64( &( serializerContext ),
                                   'v',
                                   version );

    TEST_ASSERT_EQUAL( SDP_RESULT_OUT_OF_MEMORY, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief Inputs are valid with IPv4 address.
 */
void test_SdpSerializer_AddOriginator_PassIPv4( void )
{
    SdpResult_t result;
    char expectOutput[] = "o=Jode 4294967296 4294967297 IN IP4 192.168.123.456\r\n";
    size_t outputLength = strlen( expectOutput );
    char expectName[] = "Jode";
    char expectAddr[] = "192.168.123.456";
    uint64_t expectSessionID = 4294967296;
    uint64_t expectSessionVersion = 4294967297;
    SdpOriginator_t originator;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    /* Initialize originator. */
    originator.pUserName = expectName;
    originator.userNameLength = strlen( expectName );
    originator.sessionId = expectSessionID;
    originator.sessionVersion = expectSessionVersion;
    originator.connectionInfo.networkType = SDP_NETWORK_IN;
    originator.connectionInfo.addressType = SDP_ADDRESS_IPV4;
    originator.connectionInfo.pAddress = expectAddr;
    originator.connectionInfo.addressLength = strlen( expectAddr );

    result = SdpSerializer_AddOriginator( &( serializerContext ),
                                          'o',
                                          &( originator ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL_STRING( &( expectOutput ), &( serializerBuffer[ 0 ] ) );
    TEST_ASSERT_EQUAL( outputLength, serializerContext.currentIndex );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( outputLength, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Inputs are valid with IPv6 address.
 */
void test_SdpSerializer_AddOriginator_PassIPv6( void )
{
    SdpResult_t result;
    char expectOutput[] = "o=Jode 4294967296 4294967297 IN IP6 2001::100\r\n";
    size_t outputLength = strlen( expectOutput );
    char expectName[] = "Jode";
    char expectAddr[] = "2001::100";
    uint64_t expectSessionID = 4294967296;
    uint64_t expectSessionVersion = 4294967297;
    SdpOriginator_t originator;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    /* Initialize originator. */
    originator.pUserName = expectName;
    originator.userNameLength = strlen( expectName );
    originator.sessionId = expectSessionID;
    originator.sessionVersion = expectSessionVersion;
    originator.connectionInfo.networkType = SDP_NETWORK_IN;
    originator.connectionInfo.addressType = SDP_ADDRESS_IPV6;
    originator.connectionInfo.pAddress = expectAddr;
    originator.connectionInfo.addressLength = strlen( expectAddr );

    result = SdpSerializer_AddOriginator( &( serializerContext ),
                                          'o',
                                          &( originator ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL_STRING( &( expectOutput ), &( serializerBuffer[ 0 ] ) );
    TEST_ASSERT_EQUAL( outputLength, serializerContext.currentIndex );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( strlen( expectOutput ), serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Context is NULL.
 */
void test_SdpSerializer_AddOriginator_NullContext( void )
{
    SdpResult_t result;
    char expectName[] = "Jode";
    char expectAddr[] = "192.168.123.456";
    uint64_t expectSessionID = 4294967296;
    uint64_t expectSessionVersion = 4294967297;
    SdpOriginator_t originator;

    /* Initialize originator. */
    originator.pUserName = expectName;
    originator.userNameLength = strlen( expectName );
    originator.sessionId = expectSessionID;
    originator.sessionVersion = expectSessionVersion;
    originator.connectionInfo.networkType = SDP_NETWORK_IN;
    originator.connectionInfo.addressType = SDP_ADDRESS_IPV4;
    originator.connectionInfo.pAddress = expectAddr;
    originator.connectionInfo.addressLength = strlen( expectAddr );

    result = SdpSerializer_AddOriginator( NULL,
                                          'o',
                                          &( originator ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief Context is invalid.
 */
void test_SdpSerializer_AddOriginator_InvalidContext( void )
{
    SdpResult_t result;
    char expectName[] = "Jode";
    char expectAddr[] = "192.168.123.456";
    uint64_t expectSessionID = 4294967296;
    uint64_t expectSessionVersion = 4294967297;
    SdpOriginator_t originator;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    /* Move currentIndex past totalLength. */
    serializerContext.currentIndex = serializerBufferLength + 1;

    /* Initialize originator. */
    originator.pUserName = expectName;
    originator.userNameLength = strlen( expectName );
    originator.sessionId = expectSessionID;
    originator.sessionVersion = expectSessionVersion;
    originator.connectionInfo.networkType = SDP_NETWORK_IN;
    originator.connectionInfo.addressType = SDP_ADDRESS_IPV4;
    originator.connectionInfo.pAddress = expectAddr;
    originator.connectionInfo.addressLength = strlen( expectAddr );

    result = SdpSerializer_AddOriginator( &( serializerContext ),
                                          'o',
                                          &( originator ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EACH_EQUAL_HEX8( 0x00,
                                 &( serializerBuffer[ 0 ] ),
                                 serializerBufferLength );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( serializerBufferLength + 1, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Input originator is NULL.
 */
void test_SdpSerializer_AddOriginator_NullOriginator( void )
{
    SdpResult_t result;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    result = SdpSerializer_AddOriginator( &( serializerContext ),
                                          'o',
                                          NULL );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( 0, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Input address of connection information is NULL.
 */
void test_SdpSerializer_AddOriginator_NullConnInfoAddr( void )
{
    SdpResult_t result;
    char expectName[] = "Jode";
    uint64_t expectSessionID = 4294967296;
    uint64_t expectSessionVersion = 4294967297;
    SdpOriginator_t originator;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    /* Initialize originator. */
    originator.pUserName = expectName;
    originator.userNameLength = strlen( expectName );
    originator.sessionId = expectSessionID;
    originator.sessionVersion = expectSessionVersion;
    originator.connectionInfo.networkType = SDP_NETWORK_IN;
    originator.connectionInfo.addressType = SDP_ADDRESS_IPV4;
    originator.connectionInfo.pAddress = NULL;
    originator.connectionInfo.addressLength = 0;

    result = SdpSerializer_AddOriginator( &( serializerContext ),
                                          'o',
                                          &( originator ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( 0, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Output buffer is NULL.
 */
void test_SdpSerializer_AddOriginator_NullOutputBuffer( void )
{
    SdpResult_t result;
    char expectOutput[] = "o=Jode 4294967296 4294967297 IN IP4 192.168.123.456\r\n";
    size_t outputLength = strlen( expectOutput );
    char expectName[] = "Jode";
    char expectAddr[] = "192.168.123.456";
    uint64_t expectSessionID = 4294967296;
    uint64_t expectSessionVersion = 4294967297;
    SdpOriginator_t originator;

    /* Initialize serializer context. */
    serializerContext.pStart = NULL;
    serializerContext.totalLength = 0;
    serializerContext.currentIndex = 0;

    /* Initialize originator. */
    originator.pUserName = expectName;
    originator.userNameLength = strlen( expectName );
    originator.sessionId = expectSessionID;
    originator.sessionVersion = expectSessionVersion;
    originator.connectionInfo.networkType = SDP_NETWORK_IN;
    originator.connectionInfo.addressType = SDP_ADDRESS_IPV4;
    originator.connectionInfo.pAddress = expectAddr;
    originator.connectionInfo.addressLength = strlen( expectAddr );

    result = SdpSerializer_AddOriginator( &( serializerContext ),
                                          'o',
                                          &( originator ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL( outputLength, serializerContext.currentIndex );
    TEST_ASSERT_EQUAL( NULL, serializerContext.pStart );
    TEST_ASSERT_EQUAL( 0, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( outputLength, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief The remaining size of output buffer is not enough for adding buffer.
 */
void test_SdpSerializer_AddOriginator_OutputBufferNotEnough( void )
{
    SdpResult_t result;
    char expectOutput[] = "o=Jode 4294967296 4294967297 IN IP4 192.168.123.456\r\n";
    size_t outputLength = strlen( expectOutput );
    char expectName[] = "Jode";
    char expectAddr[] = "192.168.123.456";
    uint64_t expectSessionID = 4294967296;
    uint64_t expectSessionVersion = 4294967297;
    SdpOriginator_t originator;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = outputLength - 1;
    serializerContext.currentIndex = 0;

    /* Initialize originator. */
    originator.pUserName = expectName;
    originator.userNameLength = strlen( expectName );
    originator.sessionId = expectSessionID;
    originator.sessionVersion = expectSessionVersion;
    originator.connectionInfo.networkType = SDP_NETWORK_IN;
    originator.connectionInfo.addressType = SDP_ADDRESS_IPV4;
    originator.connectionInfo.pAddress = expectAddr;
    originator.connectionInfo.addressLength = strlen( expectAddr );

    result = SdpSerializer_AddOriginator( &( serializerContext ),
                                          'o',
                                          &( originator ) );
    TEST_ASSERT_EQUAL( SDP_RESULT_OUT_OF_MEMORY, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief Network type in connection information is invalid.
 */
void test_SdpSerializer_AddOriginator_InvalidNetworkType( void )
{
    SdpResult_t result;
    char expectName[] = "Jode";
    char expectAddr[] = "192.168.123.456";
    uint64_t expectSessionID = 4294967296;
    uint64_t expectSessionVersion = 4294967297;
    SdpOriginator_t originator;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    /* Initialize originator. */
    originator.pUserName = expectName;
    originator.userNameLength = strlen( expectName );
    originator.sessionId = expectSessionID;
    originator.sessionVersion = expectSessionVersion;
    originator.connectionInfo.networkType = 0xFF;
    originator.connectionInfo.addressType = SDP_ADDRESS_IPV4;
    originator.connectionInfo.pAddress = expectAddr;
    originator.connectionInfo.addressLength = strlen( expectAddr );

    result = SdpSerializer_AddOriginator( &( serializerContext ),
                                          'o',
                                          &( originator ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( 0, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Address type in connection information is invalid.
 */
void test_SdpSerializer_AddOriginator_InvalidAddressType( void )
{
    SdpResult_t result;
    char expectName[] = "Jode";
    char expectAddr[] = "192.168.123.456";
    uint64_t expectSessionID = 4294967296;
    uint64_t expectSessionVersion = 4294967297;
    SdpOriginator_t originator;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    /* Initialize originator. */
    originator.pUserName = expectName;
    originator.userNameLength = strlen( expectName );
    originator.sessionId = expectSessionID;
    originator.sessionVersion = expectSessionVersion;
    originator.connectionInfo.networkType = SDP_NETWORK_IN;
    originator.connectionInfo.addressType = 0xFF;
    originator.connectionInfo.pAddress = expectAddr;
    originator.connectionInfo.addressLength = strlen( expectAddr );

    result = SdpSerializer_AddOriginator( &( serializerContext ),
                                          'o',
                                          &( originator ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( 0, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Inputs are valid with IPv4 address.
 */
void test_SdpSerializer_AddConnectionInfo_PassIPv4( void )
{
    SdpResult_t result;
    char expectOutput[] = "c=IN IP4 192.168.123.456\r\n";
    size_t outputLength = strlen( expectOutput );
    char expectAddr[] = "192.168.123.456";
    SdpConnectionInfo_t connInfo;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    /* Initialize originator. */
    connInfo.networkType = SDP_NETWORK_IN;
    connInfo.addressType = SDP_ADDRESS_IPV4;
    connInfo.pAddress = expectAddr;
    connInfo.addressLength = strlen( expectAddr );

    result = SdpSerializer_AddConnectionInfo( &( serializerContext ),
                                              'c',
                                              &( connInfo ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL_STRING( &( expectOutput ), &( serializerBuffer[ 0 ] ) );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( outputLength, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Inputs are valid with IPv6 address.
 */
void test_SdpSerializer_AddConnectionInfo_PassIPv6( void )
{
    SdpResult_t result;
    char expectOutput[] = "c=IN IP6 2001::100\r\n";
    size_t outputLength = strlen( expectOutput );
    char expectAddr[] = "2001::100";
    SdpConnectionInfo_t connInfo;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    /* Initialize originator. */
    connInfo.networkType = SDP_NETWORK_IN;
    connInfo.addressType = SDP_ADDRESS_IPV6;
    connInfo.pAddress = expectAddr;
    connInfo.addressLength = strlen( expectAddr );

    result = SdpSerializer_AddConnectionInfo( &( serializerContext ),
                                              'c',
                                              &( connInfo ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL_STRING( &( expectOutput ), &( serializerBuffer[ 0 ] ) );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( outputLength, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Context in NULL.
 */
void test_SdpSerializer_AddConnectionInfo_NullContext( void )
{
    SdpResult_t result;
    char expectAddr[] = "2001::100";
    SdpConnectionInfo_t connInfo;

    /* Initialize originator. */
    connInfo.networkType = SDP_NETWORK_IN;
    connInfo.addressType = SDP_ADDRESS_IPV6;
    connInfo.pAddress = expectAddr;
    connInfo.addressLength = strlen( expectAddr );

    result = SdpSerializer_AddConnectionInfo( NULL,
                                              'c',
                                              &( connInfo ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief Context in invalid.
 */
void test_SdpSerializer_AddConnectionInfo_InvalidContext( void )
{
    SdpResult_t result;
    char expectAddr[] = "2001::100";
    SdpConnectionInfo_t connInfo;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    /* Move currentIndex past totalLength. */
    serializerContext.currentIndex = serializerBufferLength + 1;

    /* Initialize originator. */
    connInfo.networkType = SDP_NETWORK_IN;
    connInfo.addressType = SDP_ADDRESS_IPV6;
    connInfo.pAddress = expectAddr;
    connInfo.addressLength = strlen( expectAddr );

    result = SdpSerializer_AddConnectionInfo( &( serializerContext ),
                                              'c',
                                              &( connInfo ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EACH_EQUAL_HEX8( 0x00,
                                 &( serializerBuffer[ 0 ] ),
                                 serializerBufferLength );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( serializerBufferLength + 1, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Connection information is NULL.
 */
void test_SdpSerializer_AddConnectionInfo_NullConnInfo( void )
{
    SdpResult_t result;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    result = SdpSerializer_AddConnectionInfo( &( serializerContext ),
                                              'c',
                                              NULL );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( 0, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Address is NULL.
 */
void test_SdpSerializer_AddConnectionInfo_NullAddress( void )
{
    SdpResult_t result;
    SdpConnectionInfo_t connInfo;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    /* Initialize originator. */
    connInfo.networkType = SDP_NETWORK_IN;
    connInfo.addressType = SDP_ADDRESS_IPV6;
    connInfo.pAddress = NULL;
    connInfo.addressLength = 0;

    result = SdpSerializer_AddConnectionInfo( &( serializerContext ),
                                              'c',
                                              &( connInfo ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( 0, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Network type is invalid.
 */
void test_SdpSerializer_AddConnectionInfo_InvalidNetworkType( void )
{
    SdpResult_t result;
    char expectAddr[] = "2001::100";
    SdpConnectionInfo_t connInfo;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    /* Initialize originator. */
    connInfo.networkType = 0xFF;
    connInfo.addressType = SDP_ADDRESS_IPV6;
    connInfo.pAddress = expectAddr;
    connInfo.addressLength = strlen( expectAddr );

    result = SdpSerializer_AddConnectionInfo( &( serializerContext ),
                                              'c',
                                              &( connInfo ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( 0, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Address type is invalid.
 */
void test_SdpSerializer_AddConnectionInfo_InvalidAddressType( void )
{
    SdpResult_t result;
    char expectAddr[] = "2001::100";
    SdpConnectionInfo_t connInfo;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    /* Initialize originator. */
    connInfo.networkType = SDP_NETWORK_IN;
    connInfo.addressType = 0xFF;
    connInfo.pAddress = expectAddr;
    connInfo.addressLength = strlen( expectAddr );

    result = SdpSerializer_AddConnectionInfo( &( serializerContext ),
                                              'c',
                                              &( connInfo ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( 0, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Output buffer is NULL.
 */
void test_SdpSerializer_AddConnectionInfo_NullOutputBuffer( void )
{
    SdpResult_t result;
    char expectOutput[] = "c=IN IP6 2001::100\r\n";
    size_t outputLength = strlen( expectOutput );
    char expectAddr[] = "2001::100";
    SdpConnectionInfo_t connInfo;

    /* Initialize serializer context. */
    serializerContext.pStart = NULL;
    serializerContext.totalLength = 0;
    serializerContext.currentIndex = 0;

    /* Initialize originator. */
    connInfo.networkType = SDP_NETWORK_IN;
    connInfo.addressType = SDP_ADDRESS_IPV6;
    connInfo.pAddress = expectAddr;
    connInfo.addressLength = strlen( expectAddr );

    result = SdpSerializer_AddConnectionInfo( &( serializerContext ),
                                              'c',
                                              &( connInfo ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL( NULL, serializerContext.pStart );
    TEST_ASSERT_EQUAL( 0, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( outputLength, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief The remaining size of output buffer is not enough for adding buffer.
 */
void test_SdpSerializer_AddConnectionInfo_OutputBufferNotEnough( void )
{
    SdpResult_t result;
    char expectOutput[] = "c=IN IP6 2001::100\r\n";
    size_t outputLength = strlen( expectOutput );
    char expectAddr[] = "2001::100";
    SdpConnectionInfo_t connInfo;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = outputLength - 1;
    serializerContext.currentIndex = 0;

    /* Initialize originator. */
    connInfo.networkType = SDP_NETWORK_IN;
    connInfo.addressType = SDP_ADDRESS_IPV6;
    connInfo.pAddress = expectAddr;
    connInfo.addressLength = strlen( expectAddr );

    result = SdpSerializer_AddConnectionInfo( &( serializerContext ),
                                              'c',
                                              &( connInfo ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OUT_OF_MEMORY, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief Input is valid.
 */
void test_SdpSerializer_AddBandwidthInfo_Pass( void )
{
    SdpResult_t result;
    char expectOutput[] = "b=CT:128\r\n";
    size_t outputLength = strlen( expectOutput );
    char expectBwType[] = "CT";
    SdpBandwidthInfo_t bwInfo;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    /* Initialize bandwidth information. */
    bwInfo.pBwType = expectBwType;
    bwInfo.bwTypeLength = strlen( expectBwType );
    bwInfo.sdpBandwidthValue = 128;

    result = SdpSerializer_AddBandwidthInfo( &( serializerContext ),
                                             'b',
                                             &( bwInfo ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL_STRING( &( expectOutput ), &( serializerBuffer[ 0 ] ) );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( outputLength, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Context is NULL.
 */
void test_SdpSerializer_AddBandwidthInfo_NullContext( void )
{
    SdpResult_t result;
    char expectBwType[] = "CT";
    SdpBandwidthInfo_t bwInfo;

    /* Initialize bandwidth information. */
    bwInfo.pBwType = expectBwType;
    bwInfo.bwTypeLength = strlen( expectBwType );
    bwInfo.sdpBandwidthValue = 128;

    result = SdpSerializer_AddBandwidthInfo( NULL,
                                             'b',
                                             &( bwInfo ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief Context is invalid.
 */
void test_SdpSerializer_AddBandwidthInfo_InvalidContext( void )
{
    SdpResult_t result;
    char expectBwType[] = "CT";
    SdpBandwidthInfo_t bwInfo;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    /* Move currentIndex past totalLength. */
    serializerContext.currentIndex = serializerBufferLength + 1;

    /* Initialize bandwidth information. */
    bwInfo.pBwType = expectBwType;
    bwInfo.bwTypeLength = strlen( expectBwType );
    bwInfo.sdpBandwidthValue = 128;

    result = SdpSerializer_AddBandwidthInfo( &( serializerContext ),
                                             'b',
                                             &( bwInfo ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EACH_EQUAL_HEX8( 0x00,
                                 &( serializerBuffer[ 0 ] ),
                                 serializerBufferLength );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( serializerBufferLength + 1, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Bandwidth is NULL.
 */
void test_SdpSerializer_AddBandwidthInfo_NullBandwidthInfo( void )
{
    SdpResult_t result;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    result = SdpSerializer_AddBandwidthInfo( &( serializerContext ),
                                             'b',
                                             NULL );
    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( 0, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Bandwidth type is NULL.
 */
void test_SdpSerializer_AddBandwidthInfo_NullBandwidthType( void )
{
    SdpResult_t result;
    SdpBandwidthInfo_t bwInfo;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    /* Initialize bandwidth information. */
    bwInfo.pBwType = NULL;
    bwInfo.bwTypeLength = 0;
    bwInfo.sdpBandwidthValue = 128;

    result = SdpSerializer_AddBandwidthInfo( &( serializerContext ),
                                             'b',
                                             &( bwInfo ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( 0, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Output buffer is NULL.
 */
void test_SdpSerializer_AddBandwidthInfo_NullOutputBuffer( void )
{
    SdpResult_t result;
    char expectOutput[] = "b=CT:128\r\n";
    size_t outputLength = strlen( expectOutput );
    char expectBwType[] = "CT";
    SdpBandwidthInfo_t bwInfo;

    /* Initialize serializer context. */
    serializerContext.pStart = NULL;
    serializerContext.totalLength = 0;
    serializerContext.currentIndex = 0;

    /* Initialize bandwidth information. */
    bwInfo.pBwType = expectBwType;
    bwInfo.bwTypeLength = strlen( expectBwType );
    bwInfo.sdpBandwidthValue = 128;

    result = SdpSerializer_AddBandwidthInfo( &( serializerContext ),
                                             'b',
                                             &( bwInfo ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL( NULL, serializerContext.pStart );
    TEST_ASSERT_EQUAL( 0, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( outputLength, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief The remaining size of output buffer is not enough for adding buffer.
 */
void test_SdpSerializer_AddBandwidthInfo_OutputBufferNotEnough( void )
{
    SdpResult_t result;
    char expectOutput[] = "b=CT:128\r\n";
    size_t outputLength = strlen( expectOutput );
    char expectBwType[] = "CT";
    SdpBandwidthInfo_t bwInfo;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = outputLength - 1;
    serializerContext.currentIndex = 0;

    /* Initialize bandwidth information. */
    bwInfo.pBwType = expectBwType;
    bwInfo.bwTypeLength = strlen( expectBwType );
    bwInfo.sdpBandwidthValue = 128;

    result = SdpSerializer_AddBandwidthInfo( &( serializerContext ),
                                             'b',
                                             &( bwInfo ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OUT_OF_MEMORY, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief Input is valid.
 */
void test_SdpSerializer_AddTimeActive_Pass( void )
{
    SdpResult_t result;
    char expectOutput[] = "t=4294967296 4294967297\r\n";
    size_t outputLength = strlen( expectOutput );
    uint64_t expectStartTime = 4294967296;
    uint64_t expectStopTime = 4294967297;
    SdpTimeDescription_t timeDescription;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    /* Initialize time active. */
    timeDescription.startTime = expectStartTime;
    timeDescription.stopTime = expectStopTime;

    result = SdpSerializer_AddTimeActive( &( serializerContext ),
                                          't',
                                          &( timeDescription ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL_STRING( &( expectOutput ), &( serializerBuffer[ 0 ] ) );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( outputLength, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Context is NULL.
 */
void test_SdpSerializer_AddTimeActive_NullContext( void )
{
    SdpResult_t result;
    uint64_t expectStartTime = 4294967296;
    uint64_t expectStopTime = 4294967297;
    SdpTimeDescription_t timeDescription;

    /* Initialize time active. */
    timeDescription.startTime = expectStartTime;
    timeDescription.stopTime = expectStopTime;

    result = SdpSerializer_AddTimeActive( NULL,
                                          't',
                                          &( timeDescription ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief Context is invalid.
 */
void test_SdpSerializer_AddTimeActive_InvalidContext( void )
{
    SdpResult_t result;
    uint64_t expectStartTime = 4294967296;
    uint64_t expectStopTime = 4294967297;
    SdpTimeDescription_t timeDescription;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    /* Move currentIndex past totalLength. */
    serializerContext.currentIndex = serializerBufferLength + 1;

    /* Initialize time active. */
    timeDescription.startTime = expectStartTime;
    timeDescription.stopTime = expectStopTime;

    result = SdpSerializer_AddTimeActive( &( serializerContext ),
                                          't',
                                          &( timeDescription ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EACH_EQUAL_HEX8( 0x00,
                                 &( serializerBuffer[ 0 ] ),
                                 serializerBufferLength );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( serializerBufferLength + 1, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Input time is NULL pointer.
 */
void test_SdpSerializer_AddTimeActive_NullTimeDescription( void )
{
    SdpResult_t result;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    result = SdpSerializer_AddTimeActive( &( serializerContext ),
                                          't',
                                          NULL );
    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( 0, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Output buffer is NULL.
 */
void test_SdpSerializer_AddTimeActive_NullOutputBuffer( void )
{
    SdpResult_t result;
    char expectOutput[] = "t=4294967296 4294967297\r\n";
    size_t outputLength = strlen( expectOutput );
    uint64_t expectStartTime = 4294967296;
    uint64_t expectStopTime = 4294967297;
    SdpTimeDescription_t timeDescription;

    /* Initialize serializer context. */
    serializerContext.pStart = NULL;
    serializerContext.totalLength = 0;
    serializerContext.currentIndex = 0;

    /* Initialize time active. */
    timeDescription.startTime = expectStartTime;
    timeDescription.stopTime = expectStopTime;

    result = SdpSerializer_AddTimeActive( &( serializerContext ),
                                          't',
                                          &( timeDescription ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL( NULL, serializerContext.pStart );
    TEST_ASSERT_EQUAL( 0, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( outputLength, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief The remaining size of output buffer is not enough for adding buffer.
 */
void test_SdpSerializer_AddTimeActive_OutputBufferNotEnough( void )
{
    SdpResult_t result;
    char expectOutput[] = "t=4294967296 4294967297\r\n";
    size_t outputLength = strlen( expectOutput );
    uint64_t expectStartTime = 4294967296;
    uint64_t expectStopTime = 4294967297;
    SdpTimeDescription_t timeDescription;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = outputLength - 1;
    serializerContext.currentIndex = 0;

    /* Initialize time active. */
    timeDescription.startTime = expectStartTime;
    timeDescription.stopTime = expectStopTime;

    result = SdpSerializer_AddTimeActive( &( serializerContext ),
                                          't',
                                          &( timeDescription ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OUT_OF_MEMORY, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief Input is valid with attribute value.
 */
void test_SdpSerializer_AddAttribute_Pass( void )
{
    SdpResult_t result;
    char expectOutput[] = "a=orient:landscape\r\n";
    size_t outputLength = strlen( expectOutput );
    char expectAttributeName[] = "orient";
    char expectAttributeValue[] = "landscape";
    SdpAttribute_t attribute;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    /* Initialize attribute. */
    attribute.pAttributeName = expectAttributeName;
    attribute.attributeNameLength = strlen( expectAttributeName );
    attribute.pAttributeValue = expectAttributeValue;
    attribute.attributeValueLength = strlen( expectAttributeValue );

    result = SdpSerializer_AddAttribute( &( serializerContext ),
                                         'a',
                                         &( attribute ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL_STRING( &( expectOutput ), &( serializerBuffer[ 0 ] ) );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( outputLength, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Input is valid without attribute value.
 */
void test_SdpSerializer_AddAttribute_PassNoValue( void )
{
    SdpResult_t result;
    char expectOutput[] = "a=recvonly\r\n";
    size_t outputLength = strlen( expectOutput );
    char expectAttributeName[] = "recvonly";
    SdpAttribute_t attribute;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    /* Initialize attribute. */
    attribute.pAttributeName = expectAttributeName;
    attribute.attributeNameLength = strlen( expectAttributeName );
    attribute.pAttributeValue = NULL;
    attribute.attributeValueLength = 0;

    result = SdpSerializer_AddAttribute( &( serializerContext ),
                                         'a',
                                         &( attribute ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL_STRING( &( expectOutput ), &( serializerBuffer[ 0 ] ) );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( outputLength, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Context is NULL.
 */
void test_SdpSerializer_AddAttribute_NullContext( void )
{
    SdpResult_t result;
    char expectAttributeName[] = "recvonly";
    SdpAttribute_t attribute;

    /* Initialize attribute. */
    attribute.pAttributeName = expectAttributeName;
    attribute.attributeNameLength = strlen( expectAttributeName );
    attribute.pAttributeValue = NULL;
    attribute.attributeValueLength = 0;

    result = SdpSerializer_AddAttribute( NULL,
                                         'a',
                                         &( attribute ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief Context is invalid.
 */
void test_SdpSerializer_AddAttribute_InvalidContext( void )
{
    SdpResult_t result;
    char expectAttributeName[] = "recvonly";
    SdpAttribute_t attribute;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    /* Move currentIndex past totalLength. */
    serializerContext.currentIndex = serializerBufferLength + 1;

    /* Initialize attribute. */
    attribute.pAttributeName = expectAttributeName;
    attribute.attributeNameLength = strlen( expectAttributeName );
    attribute.pAttributeValue = NULL;
    attribute.attributeValueLength = 0;

    result = SdpSerializer_AddAttribute( &( serializerContext ),
                                         'a',
                                         &( attribute ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EACH_EQUAL_HEX8( 0x00,
                                 &( serializerBuffer[ 0 ] ),
                                 serializerBufferLength );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( serializerBufferLength + 1, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Attribute is NULL.
 */
void test_SdpSerializer_AddAttribute_NullAttribute( void )
{
    SdpResult_t result;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    result = SdpSerializer_AddAttribute( &( serializerContext ),
                                         'a',
                                         NULL );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( 0, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Attribute name is NULL.
 */
void test_SdpSerializer_AddAttribute_NullAttributeName( void )
{
    SdpResult_t result;
    SdpAttribute_t attribute;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    /* Initialize attribute. */
    attribute.pAttributeName = NULL;
    attribute.attributeNameLength = 0;
    attribute.pAttributeValue = NULL;
    attribute.attributeValueLength = 0;

    result = SdpSerializer_AddAttribute( &( serializerContext ),
                                         'a',
                                         &( attribute ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( 0, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Output buffer is NULL.
 */
void test_SdpSerializer_AddAttribute_NullOutputBuffer( void )
{
    SdpResult_t result;
    char expectOutput[] = "a=recvonly\r\n";
    size_t outputLength = strlen( expectOutput );
    char expectAttributeName[] = "recvonly";
    SdpAttribute_t attribute;

    /* Initialize serializer context. */
    serializerContext.pStart = NULL;
    serializerContext.totalLength = 0;
    serializerContext.currentIndex = 0;

    /* Initialize attribute. */
    attribute.pAttributeName = expectAttributeName;
    attribute.attributeNameLength = strlen( expectAttributeName );
    attribute.pAttributeValue = NULL;
    attribute.attributeValueLength = 0;

    result = SdpSerializer_AddAttribute( &( serializerContext ),
                                         'a',
                                         &( attribute ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL( NULL, serializerContext.pStart );
    TEST_ASSERT_EQUAL( 0, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( outputLength, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief The remaining size of output buffer is not enough for adding buffer.
 */
void test_SdpSerializer_AddAttribute_OutputBufferNotEnough( void )
{
    SdpResult_t result;
    char expectOutput[] = "a=recvonly\r\n";
    size_t outputLength = strlen( expectOutput );
    char expectAttributeName[] = "recvonly";
    SdpAttribute_t attribute;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = outputLength - 1;
    serializerContext.currentIndex = 0;

    /* Initialize attribute. */
    attribute.pAttributeName = expectAttributeName;
    attribute.attributeNameLength = strlen( expectAttributeName );
    attribute.pAttributeValue = NULL;
    attribute.attributeValueLength = 0;

    result = SdpSerializer_AddAttribute( &( serializerContext ),
                                         'a',
                                         &( attribute ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OUT_OF_MEMORY, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief Inputs are all valid.
 */
void test_SdpSerializer_AddMedia_Pass( void )
{
    SdpResult_t result;
    const char expectOutput[] = "m=video 49170/2 RTP/AVP 31\r\n";
    size_t outputLength = strlen( expectOutput );
    const char expectMediaName[] = "video";
    uint16_t expectPort = 49170;
    uint16_t expectPortNum = 2;
    const char expectProtocol[] = "RTP/AVP";
    const char expectFmt[] = "31";
    SdpMedia_t media;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    /* Initialize media. */
    media.pMedia = expectMediaName;
    media.mediaLength = strlen( expectMediaName );
    media.port = expectPort;
    media.portNum = expectPortNum;
    media.pProtocol = expectProtocol;
    media.protocolLength = strlen( expectProtocol );
    media.pFmt = expectFmt;
    media.fmtLength = strlen( expectFmt );

    result = SdpSerializer_AddMedia( &( serializerContext ),
                                     'm',
                                     &( media ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL_STRING( &( expectOutput ), &( serializerBuffer[ 0 ] ) );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( outputLength, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Inputs (except for port number) are all valid.
 */
void test_SdpSerializer_AddMedia_PassNoPortNum( void )
{
    SdpResult_t result;
    const char expectOutput[] = "m=video 49170 RTP/AVP 31\r\n";
    size_t outputLength = strlen( expectOutput );
    const char expectMediaName[] = "video";
    uint16_t expectPort = 49170;
    const char expectProtocol[] = "RTP/AVP";
    const char expectFmt[] = "31";
    SdpMedia_t media;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    /* Initialize media. */
    media.pMedia = expectMediaName;
    media.mediaLength = strlen( expectMediaName );
    media.port = expectPort;
    media.portNum = 0;
    media.pProtocol = expectProtocol;
    media.protocolLength = strlen( expectProtocol );
    media.pFmt = expectFmt;
    media.fmtLength = strlen( expectFmt );

    result = SdpSerializer_AddMedia( &( serializerContext ),
                                     'm',
                                     &( media ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL_STRING( &( expectOutput ), &( serializerBuffer[ 0 ] ) );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( outputLength, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Context is NULL.
 */
void test_SdpSerializer_AddMedia_NullContext( void )
{
    SdpResult_t result;
    const char expectMediaName[] = "video";
    uint16_t expectPort = 49170;
    uint16_t expectPortNum = 2;
    const char expectProtocol[] = "RTP/AVP";
    const char expectFmt[] = "31";
    SdpMedia_t media;

    /* Initialize media. */
    media.pMedia = expectMediaName;
    media.mediaLength = strlen( expectMediaName );
    media.port = expectPort;
    media.portNum = expectPortNum;
    media.pProtocol = expectProtocol;
    media.protocolLength = strlen( expectProtocol );
    media.pFmt = expectFmt;
    media.fmtLength = strlen( expectFmt );

    result = SdpSerializer_AddMedia( NULL,
                                     'm',
                                     &( media ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief Context is invalid.
 */
void test_SdpSerializer_AddMedia_InvalidContext( void )
{
    SdpResult_t result;
    const char expectMediaName[] = "video";
    uint16_t expectPort = 49170;
    uint16_t expectPortNum = 2;
    const char expectProtocol[] = "RTP/AVP";
    const char expectFmt[] = "31";
    SdpMedia_t media;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    /* Move currentIndex past totalLength. */
    serializerContext.currentIndex = serializerBufferLength + 1;

    /* Initialize media. */
    media.pMedia = expectMediaName;
    media.mediaLength = strlen( expectMediaName );
    media.port = expectPort;
    media.portNum = expectPortNum;
    media.pProtocol = expectProtocol;
    media.protocolLength = strlen( expectProtocol );
    media.pFmt = expectFmt;
    media.fmtLength = strlen( expectFmt );

    result = SdpSerializer_AddMedia( &( serializerContext ),
                                     'm',
                                     &( media ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EACH_EQUAL_HEX8( 0x00,
                                 &( serializerBuffer[ 0 ] ),
                                 serializerBufferLength );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( serializerBufferLength + 1, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Media is NULL.
 */
void test_SdpSerializer_AddMedia_NullMedia( void )
{
    SdpResult_t result;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    result = SdpSerializer_AddMedia( &( serializerContext ),
                                     'm',
                                     NULL );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( 0, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Protocol in media is NULL.
 */
void test_SdpSerializer_AddMedia_NullProtocol( void )
{
    SdpResult_t result;
    const char expectMediaName[] = "video";
    uint16_t expectPort = 49170;
    uint16_t expectPortNum = 2;
    const char expectFmt[] = "31";
    SdpMedia_t media;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    /* Initialize media. */
    media.pMedia = expectMediaName;
    media.mediaLength = strlen( expectMediaName );
    media.port = expectPort;
    media.portNum = expectPortNum;
    media.pProtocol = NULL;
    media.protocolLength = 0;
    media.pFmt = expectFmt;
    media.fmtLength = strlen( expectFmt );

    result = SdpSerializer_AddMedia( &( serializerContext ),
                                     'm',
                                     &( media ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( 0, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Format is NULL.
 */
void test_SdpSerializer_AddMedia_NullFormat( void )
{
    SdpResult_t result;
    const char expectMediaName[] = "video";
    uint16_t expectPort = 49170;
    uint16_t expectPortNum = 2;
    const char expectProtocol[] = "RTP/AVP";
    SdpMedia_t media;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    serializerContext.currentIndex = 0;

    /* Initialize media. */
    media.pMedia = expectMediaName;
    media.mediaLength = strlen( expectMediaName );
    media.port = expectPort;
    media.portNum = expectPortNum;
    media.pProtocol = expectProtocol;
    media.protocolLength = strlen( expectProtocol );
    media.pFmt = NULL;
    media.fmtLength = 0;

    result = SdpSerializer_AddMedia( &( serializerContext ),
                                     'm',
                                     &( media ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( 0, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Output buffer is NULL.
 */
void test_SdpSerializer_AddMedia_NullOutputBuffer( void )
{
    SdpResult_t result;
    const char expectOutput[] = "m=video 49170/2 RTP/AVP 31\r\n";
    size_t outputLength = strlen( expectOutput );
    const char expectMediaName[] = "video";
    uint16_t expectPort = 49170;
    uint16_t expectPortNum = 2;
    const char expectProtocol[] = "RTP/AVP";
    const char expectFmt[] = "31";
    SdpMedia_t media;

    /* Initialize serializer context. */
    serializerContext.pStart = NULL;
    serializerContext.totalLength = 0;
    serializerContext.currentIndex = 0;

    /* Initialize media. */
    media.pMedia = expectMediaName;
    media.mediaLength = strlen( expectMediaName );
    media.port = expectPort;
    media.portNum = expectPortNum;
    media.pProtocol = expectProtocol;
    media.protocolLength = strlen( expectProtocol );
    media.pFmt = expectFmt;
    media.fmtLength = strlen( expectFmt );

    result = SdpSerializer_AddMedia( &( serializerContext ),
                                     'm',
                                     &( media ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL( NULL, serializerContext.pStart );
    TEST_ASSERT_EQUAL( 0, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( outputLength, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief The remaining size of output buffer is not enough for adding buffer.
 */
void test_SdpSerializer_AddMedia_OutputBufferNotEnough( void )
{
    SdpResult_t result;
    const char expectOutput[] = "m=video 49170/2 RTP/AVP 31\r\n";
    size_t outputLength = strlen( expectOutput );
    const char expectMediaName[] = "video";
    uint16_t expectPort = 49170;
    uint16_t expectPortNum = 2;
    const char expectProtocol[] = "RTP/AVP";
    const char expectFmt[] = "31";
    SdpMedia_t media;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = outputLength - 1;
    serializerContext.currentIndex = 0;

    /* Initialize media. */
    media.pMedia = expectMediaName;
    media.mediaLength = strlen( expectMediaName );
    media.port = expectPort;
    media.portNum = expectPortNum;
    media.pProtocol = expectProtocol;
    media.protocolLength = strlen( expectProtocol );
    media.pFmt = expectFmt;
    media.fmtLength = strlen( expectFmt );

    result = SdpSerializer_AddMedia( &( serializerContext ),
                                     'm',
                                     &( media ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OUT_OF_MEMORY, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief Inputs are all valid.
 */
void test_SdpSerializer_Finalize_Pass( void )
{
    SdpResult_t result;
    const char expectOutput[] = "m=video 49170/2 RTP/AVP 31\r\n";
    size_t expectOutputLength = strlen( expectOutput );
    const char * pBuffer;
    size_t length;

    /* Initialize serializer context. */
    strcpy( &( serializerBuffer[ 0 ] ), expectOutput );
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = expectOutputLength;
    serializerContext.currentIndex = expectOutputLength;

    result = SdpSerializer_Finalize( &( serializerContext ),
                                     &( pBuffer ),
                                     &( length ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL_STRING( &( expectOutput ), pBuffer );
    TEST_ASSERT_EQUAL( expectOutputLength, length );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( expectOutputLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( expectOutputLength, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Context is NULL.
 */
void test_SdpSerializer_Finalize_NullContext( void )
{
    SdpResult_t result;
    const char * pBuffer;
    size_t length;

    result = SdpSerializer_Finalize( NULL,
                                     &( pBuffer ),
                                     &( length ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
}

/*-----------------------------------------------------------*/

/**
 * @brief Input buffer is NULL.
 */
void test_SdpSerializer_Finalize_NullBuffer( void )
{
    SdpResult_t result;
    const char expectOutput[] = "m=video 49170/2 RTP/AVP 31\r\n";
    size_t expectOutputLength = strlen( expectOutput );
    size_t length;

    /* Initialize serializer context. */
    strcpy( &( serializerBuffer[ 0 ] ), expectOutput );
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = expectOutputLength;
    serializerContext.currentIndex = expectOutputLength;

    result = SdpSerializer_Finalize( &( serializerContext ),
                                     NULL,
                                     &( length ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( expectOutputLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( expectOutputLength, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Input length address is NULL.
 */
void test_SdpSerializer_Finalize_NullLength( void )
{
    SdpResult_t result;
    const char expectOutput[] = "m=video 49170/2 RTP/AVP 31\r\n";
    size_t expectOutputLength = strlen( expectOutput );
    const char * pBuffer;

    /* Initialize serializer context. */
    strcpy( &( serializerBuffer[ 0 ] ), expectOutput );
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = expectOutputLength;
    serializerContext.currentIndex = expectOutputLength;

    result = SdpSerializer_Finalize( &( serializerContext ),
                                     &( pBuffer ),
                                     NULL );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( expectOutputLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( expectOutputLength, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief Invalid context.
 */
void test_SdpSerializer_Finalize_InvalidContext( void )
{
    SdpResult_t result;
    const char * pBuffer = NULL;
    size_t length = 0;

    /* Initialize serializer context. */
    serializerContext.pStart = &( serializerBuffer[ 0 ] );
    serializerContext.totalLength = serializerBufferLength;
    /* Move currentIndex past totalLength. */
    serializerContext.currentIndex = serializerBufferLength + 1;

    result = SdpSerializer_Finalize( &( serializerContext ),
                                     &( pBuffer ),
                                     &( length ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_BAD_PARAM, result );
    TEST_ASSERT_EQUAL( NULL, pBuffer );
    TEST_ASSERT_EQUAL( 0, length );
    TEST_ASSERT_EQUAL( &( serializerBuffer[ 0 ] ), serializerContext.pStart );
    TEST_ASSERT_EQUAL( serializerBufferLength, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( serializerBufferLength + 1, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/

/**
 * @brief The buffer in context is NULL.
 */
void test_SdpSerializer_Finalize_NullContextBuffer( void )
{
    SdpResult_t result;
    const char * pBuffer = NULL;
    size_t length = 0;

    /* Initialize serializer context. */
    serializerContext.pStart = NULL;
    serializerContext.totalLength = 0;
    /* Move currentIndex past totalLength. */
    serializerContext.currentIndex = serializerBufferLength + 1;

    result = SdpSerializer_Finalize( &( serializerContext ),
                                     &( pBuffer ),
                                     &( length ) );

    TEST_ASSERT_EQUAL( SDP_RESULT_OK, result );
    TEST_ASSERT_EQUAL( NULL, pBuffer );
    TEST_ASSERT_EQUAL( serializerBufferLength + 1, length );
    TEST_ASSERT_EQUAL( NULL, serializerContext.pStart );
    TEST_ASSERT_EQUAL( 0, serializerContext.totalLength );
    TEST_ASSERT_EQUAL( serializerBufferLength + 1, serializerContext.currentIndex );
}

/*-----------------------------------------------------------*/
