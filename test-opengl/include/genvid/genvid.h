/* Copyright 2016-2018 Genvid Technologies Inc. All Rights Reserved. */
#pragma once

#if !defined(GENVID_API)
// Default shared library bindings.
#if defined(_WIN32)
//   Windows DLL bindings.
#if defined(GENVID_API_COMPILING_SHARED)
#define GENVID_API __declspec(dllexport)
#else
#define GENVID_API __declspec(dllimport)
#endif
#endif
#endif

#if !defined(GENVID_API)
//#error Genvid might be statically linked (unsupported).
#define GENVID_API
#endif

#if defined(__cplusplus)
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

//==========
//= GENVID =
//============================================================================

/**
 * The different pixel formats used for non auto-capture video frames.
 */
typedef enum GenvidPixelFormat {
	GenvidPixelFormat_UNKNOWN,	 //!< Unknown pixel format.
	GenvidPixelFormat_R8G8B8,	  //!< RGB 24 bits, 8 bits per channel.
	GenvidPixelFormat_R8G8B8A8,	//!< RGBA 32 bits, 8 bits per channel
	GenvidPixelFormat_R10G10B10A2, //!< RGBA 32 bits, 10 bits per color channel, with 2 bits for alpha.
	GenvidPixelFormat_NV12,		   //!< NV12 Planar format: Y plane, followed by a combined UV plane.
	GenvidPixelFormat_B8G8R8A8,	//!< BGRA 32 bits, 8 bits per channel
	GENVID_NUM_PIXELFORMAT		   //!< Number of pixel formats supported.
} GenvidPixelFormat;

/**
*
* Verify that the GenvidPixelFormat is valid
*
*/
inline unsigned char IsPixelFormatSupported(const GenvidPixelFormat in_PixelFormat)
{
	return ((in_PixelFormat > GenvidPixelFormat_UNKNOWN) && (in_PixelFormat < GENVID_NUM_PIXELFORMAT));
}

/**
 * The different audio sample formats used.
 */
typedef enum GenvidAudioFormat {
	GenvidAudioFormat_UNKNOWN, //!< The audio format is unknown.
	GenvidAudioFormat_S16LE,   //!< The audio format is 16 bits signed.
	GenvidAudioFormat_F32LE,   //!< The audio format is 32 bits floating point.
	GenvidAudioFormat_NUM_AUDIOFORMAT
} GenvidAudioFormat;

/**
*
* Verify that the GenvidAudioFormat is valid
*
*/
inline unsigned char IsAudioFormatSupported(const GenvidAudioFormat in_PixelFormat)
{
	return ((in_PixelFormat > GenvidAudioFormat_UNKNOWN) && (in_PixelFormat < GenvidAudioFormat_NUM_AUDIOFORMAT));
}

/**
 * @typedef GenvidTimecode
 * @brief A timecode value used to specify a location in the stream.
 */
typedef int64_t GenvidTimecode;

/**
 * The various error codes that the various Genvid routines can return.
 *
 * GenvidStatus_Success means that there was no error.
 */
typedef enum GenvidStatus {
	GenvidStatus_Success = 0, //!< Success
	// Non-terminal failures.
	GenvidStatus_ConnectionInProgress, //!< A connection is in progress.
	GenvidStatus_Incomplete,		   //!< The call cannot be completed.
	// Terminal failures.
	GenvidStatus_UnknownError,		//!< An unexpected error has occured.
	GenvidStatus_InvalidState,		//!< The SDK is in an invalid state for this call to complete.
	GenvidStatus_InvalidParameter,  //!< A parameter is invalid.
	GenvidStatus_AllocationFailed,  //!< Failed to allocate memory.
	GenvidStatus_AlreadyExists,		//!< A resource with the same name already exists.
	GenvidStatus_ConnectionFailed,  //!< The SDK failed to connect to the services.
	GenvidStatus_ConnectionTimeout, //!< A connection operation timed out.
	GenvidStatus_Disconnected,		//!< The SDK was disconnected from the service.
	// No initialized
	GenvidStatus_UninitializedSDK,   //!< Genvid_Initialize have not been called.
	GenvidStatus_InvalidImageFormat, //!< The image format is unknown.
} GenvidStatus;

/**
 * @def GENVID_FAILED(status)
 * @param status The code to check.
 *
 * A utility macro which indicates whether or not the @p status code is a terminal failure.
 * (The situation can't lead to a success state given enough time.)
 */
#define GENVID_FAILED(status) (!((GenvidStatus_Success <= status) && (status <= GenvidStatus_ConnectionInProgress) && (status <= GenvidStatus_Incomplete)))

/**
 * The various Reduce operations supported.
 *
 * The value GENVID_NUM_REDUCE_OP is not a valid type. It corresponds
 * to the total number of different operations possible.
  */
typedef enum GenvidReduceOp {
	GenvidReduceOp_Min = 0,
	GenvidReduceOp_Max,
	GenvidReduceOp_Count,
	GenvidReduceOp_Sum,
	GENVID_NUM_REDUCE_OP, //!< The total number of operations.  Not a real type.
} GenvidReduceOp;

enum
{
	GENVID_MAX_KEY_FIELDS = 16, //!< The maximum number of strings allowed in a key.
	GENVID_MAX_RESULTS	= 16, //!< The maximum number of results allowed in a single GenvidEventSummary.
};

/**
 * A structure representing the event key used in a GenvidEventSummary report.
 */
typedef struct GenvidEventKey
{

	/** A list of strings composing the key. */
	const char * fields[GENVID_MAX_KEY_FIELDS];

	/** The number of valid strings in #fields. */
	int numFields;

} GenvidEventKey;

/**
 * A structure representing the event value returned in a GenvidEventSummary report.
 */
typedef struct GenvidEventValue
{

	/** The reduce operation used to generate the value. */
	GenvidReduceOp reduce;

	/** A 64-bit floating-point number generated by the #reduce operation. */
	double value;

} GenvidEventValue;

/**
 * The structure summarizing a single result of a Reduce operation.
 */
typedef struct GenvidEventResult
{

	/** An array of strings forming the key. */
	GenvidEventKey key;

	/** A series of values, each for a different reduce operation. */
	GenvidEventValue values[GENVID_NUM_REDUCE_OP];

	/** The number of valid entries in #values. */
	int numValues;

} GenvidEventResult;

/**
 * The structure summarizing all of the results of a Reduce operation.
 */
typedef struct GenvidEventSummary
{

	/** A string identifying the Reduce operation. */
	const char * id;

	/** A series of results. */
	GenvidEventResult results[GENVID_MAX_RESULTS];

	/** The number of valid entries in #results. */
	int numResults;

} GenvidEventSummary;

/**
 * @typedef GenvidEventSummaryCallback
 * @brief A callback invoked when event summaries are ready.
 * @param "const GenvidEventSummary* summary" The GenvidEventSummary structure that list all
 *        results available for a specific key.
 * @param "void* userData" The userData that was set when subscribing.
 *         See Genvid_Subscribe() for more information.
 *
 * Any memory held by @p summary is invalidated when the callback returns.
 */
typedef void (*GenvidEventSummaryCallback)(const GenvidEventSummary * summary, void * userData);

/**
* The structure for commands sent to the game.
*/
typedef struct GenvidCommandResult
{

	/** A string identifying the command operation. */
	const char * id;

	/** A string representing the command parameters. */
	const char * value;

} GenvidCommandResult;

/**
 * @typedef GenvidCommandCallback
 * @brief A callback invoked when a command is ready.
 * @param "const GenvidCommandResult* result" The command's content in a GenvidCommandResult structure.
 * @param "void* userData" The userData that was set when subscribing. See Genvid_SubscribeCommand() for
 * more information.
 *
 * Any memory held by @p result is invalidated when the callback returns.
 */
typedef void (*GenvidCommandCallback)(const GenvidCommandResult * result, void * userData);

/**
 * Returns the version number of the Genvid library, storing the @c major, @c minor,
 * @c version, and @c build values at the specified addresses.
 * A NULL pointer ignores that value.
 *
 * Returns a string representation of the version.
 */
extern GENVID_API const char * Genvid_GetVersion(uint32_t * major, uint32_t * minor, uint32_t * version, uint32_t * build);

/**
 * Returns a string representation of the ::GenvidStatus code.
 */
extern GENVID_API const char * Genvid_StatusToString(const GenvidStatus status);

/**
* Initializes the Genvid library.
*
* This function must be called at least once prior to any other call.
* It will start the initialization of the asynchronous handler.
*
* When you are done with the library, call Genvid_Terminate().
*/
extern GENVID_API GenvidStatus Genvid_Initialize();

/**
 * Deallocates any internal data used by the Genvid library.
 *
 * Calling this routine invalidates all previous settings.
 *
 * You must call Genvid_Initialize() before using any routine again.
 */
extern GENVID_API GenvidStatus Genvid_Terminate();

/**
 * Returns the current timecode based on the system clock.
 */
extern GENVID_API GenvidTimecode Genvid_GetCurrentTimecode();

/**
* Returns the timecode last returned by Genvid_GetCurrentTimecode().
 */
extern GENVID_API GenvidTimecode Genvid_GetPreviousTimecode();

/**
 * Allocates a stream using the specified unique identifier @p streamID.
 *
 * The call is synchronous.
 *
 * This routine attempts to create a stream using @p streamID as the name of the stream.
 *
 * Returns one of the following values:
 *
 * ::GenvidStatus_UninitializedSDK: The Genvid library was not initialized before calling it.
 *
 * ::GenvidStatus_Success: The operation completed properly.
 *
 * ::GenvidStatus_AlreadyExists: A stream with the same name has already been created.
 */
extern GENVID_API GenvidStatus Genvid_CreateStream(const char * streamID);

/**
 * Invalidates and deallocates internal data used by the specified @p streamID.
 *
 * The call is synchronous.
 *
 * This routine attempts to stop then destroy a stream using @p streamID as the name of the stream.
 *
 * Returns one of the following values:
 *
 * ::GenvidStatus_UninitializedSDK: The Genvid library was not initialized before calling it.
 *
 * ::GenvidStatus_Success: The operation completed properly.
 *
 * ::GenvidStatus_UnknownError: The stream could not be unregistered.
 */
extern GENVID_API GenvidStatus Genvid_DestroyStream(const char * streamID);

/**
* Assigns the integer value @p paramValue to a parameter named @p paramKey for the
* stream @p streamID.
*
* Using a NULL @p streamID works for system parameters.
*
* See the <a href="../engine.html#nativeapiconfiguration">Engine configuration</a> section for
* more details within the subsection Common Parameters for each parameter available.
*/
extern GENVID_API GenvidStatus Genvid_SetParameterInt(const char * streamID, const char * paramKey, int paramValue);

/**
* Assigns the floating-point value @p paramValue to a parameter named @p paramKey for the
* stream @p streamID.
*
* Using a NULL @p streamID works for system parameters.
*
* See the <a href="../engine.html#nativeapiconfiguration">Engine configuration</a> section for
* more details within the subsection Common Parameters for each parameter available.
*/
extern GENVID_API GenvidStatus Genvid_SetParameterFloat(const char * streamID, const char * paramKey, float paramValue);

/**
* Assigns the pointer value @p paramValue to a parameter named @p paramKey for the
* stream @p streamID.
*
* Using a NULL @p streamID works for system parameters.
*
* See the <a href="../engine.html#nativeapiconfiguration">Engine configuration</a> section for
* more details within the subsection Common Parameters for each parameter available.
*/
extern GENVID_API GenvidStatus Genvid_SetParameterPointer(const char * streamID, const char * paramKey, void * paramValue);

/**
 * For the specified @p streamID, retrieves the integer value of a stream parameter
 * named @p paramKey and stores it at the address pointed to by @p paramValue.
 *
 * Using a NULL @p streamID works for system parameters.
 *
 * To get a key value from Consul, @p streamID must be written as genvid.kv.
 * @p paramKey is used as the key value name and @p paramValue as the container
 * to receive the key value content.
 *
 * Returns one of the following values:
 *
 * ::GenvidStatus_UninitializedSDK: The Genvid library was not initialized before calling it.
 *
 * ::GenvidStatus_InvalidState: The @p streamID was not found.
 *
 * ::GenvidStatus_InvalidParameter: One of the parameters is invalid.
 *
 * See the <a href="../engine.html#nativeapiconfiguration">Engine configuration</a> section for
 * more details within the subsection Common Parameters for each parameter available.
 */
extern GENVID_API GenvidStatus Genvid_GetParameterInt(const char * streamID, const char * paramKey, int * paramValue);

/**
* For the specified @p streamID, retrieves the floating-point value of a stream parameter
* named @p paramKey and stores it at the address pointed to by @p paramValue.
*
* Using a NULL @p streamID works for system parameters.
*
* To get a key value from Consul, @p streamID must be written as genvid.kv.
* @p paramKey is used as the key value name and @p paramValue as the container
* to receive the key value content.
*
* Returns one of the following values:
*
* ::GenvidStatus_UninitializedSDK: The Genvid library was not initialized before calling it.
*
* ::GenvidStatus_InvalidState: The @p streamID was not found.
*
* ::GenvidStatus_InvalidParameter: One of the parameters is invalid.
*
* See the <a href="../engine.html#nativeapiconfiguration">Engine configuration</a> section for
* more details within the subsection Common Parameters for each parameter available.
*/
extern GENVID_API GenvidStatus Genvid_GetParameterFloat(const char * streamID, const char * paramKey, float * paramValue);

/**
* For the specified @p streamID, retrieves the pointer value of a stream parameter
* named @p paramKey and stores it at the address pointed to by @p paramValue.
*
* Using a NULL @p streamID works for system parameters.
*
* See the <a href="../engine.html#nativeapiconfiguration">Engine configuration</a> section for
* more details within the subsection Common Parameters for each parameter available.
*/
extern GENVID_API GenvidStatus Genvid_GetParameterPointer(const char * streamID, const char * paramKey, void ** paramValue);

/**
* For the specified @p id, retrieves the key value value of Consul
* named @p paramKey and stores it at the address pointed to by @p dstBuffer.
* @p dstBufferSize is needed to indicate the size of the retrieved string.
*
* To get a key value from Consul, @p streamID must be written as genvid.kv.
* @p paramKey is used as the key value name and @p paramValue as the container
* to receive the key value content.
*
* Returns one of the following values:
*
* ::GenvidStatus_UninitializedSDK: The Genvid library was not initialized before calling it.
*
* ::GenvidStatus_InvalidState: The @p streamID was not found.
*
* ::GenvidStatus_InvalidParameter: One of the parameters is invalid.
*
* ::GenvidStatus_Incomplete: The @p dstBufferSize is too small.  *This would result in a non-null terminated string!*
*
*/
extern GENVID_API GenvidStatus Genvid_GetParameterUTF8(const char * id, const char * paramKey, char * dstBuffer, size_t dstBufferSize);

/**
 * Sends a buffer of audio samples associated with the stream @p streamID at time @p timecode.
 *
 * The audio-sample format samples must be specified before calling this routine.
 * See Genvid_SetParameterInt() for more information.
 *
 * Using a negative @p timecode value calls Genvid_GetCurrentTimecode() and uses that
 * value for @p timecode.
 *
 * It returns one of the following values:
 *
 * ::GenvidStatus_UninitializedSDK: The Genvid library was not initialized before calling it.
 *
 * ::GenvidStatus_InvalidState: The asynchronous handler was not initialized.
 *
 * ::GenvidStatus_InvalidParameter: The stream @p streamID was not found.
 *
 * ::GenvidStatus_Success: The operation completed properly.
 */
extern GENVID_API GenvidStatus Genvid_SubmitAudioData(const GenvidTimecode timecode, const char * streamID, const void * audioData, const int audioDataSize);

/**
 * Sends @p gameDataSize bytes of @p gameData associated with the stream @p streamID.
 *
 * Using a negative @p timecode value calls Genvid_GetCurrentTimecode() and uses that
 * value for @p timecode.
 *
 * It returns one of the following values:
 *
 * ::GenvidStatus_UninitializedSDK: The Genvid library was not initialized before calling it.
 *
 * ::GenvidStatus_InvalidState: The asynchronous handler was not initialized.
 *
 * ::GenvidStatus_InvalidParameter: The stream @p streamID was not found.
 *
 * ::GenvidStatus_Success: The operation completed properly.
 */
extern GENVID_API GenvidStatus Genvid_SubmitGameData(const GenvidTimecode timecode, const char * streamID, const void * gameData, const int gameDataSize);

/**
 * Sends @p gameDataSize bytes of @p annotation associated with the stream @p streamID.
 *
 * Using a negative @p timecode value calls Genvid_GetCurrentTimecode() and uses that
 * value for @p timecode.
 *
 * Annotations are dropped if the asynchronous handler isn't initialized first.
 *
 * It returns one of the following values:
 *
 * ::GenvidStatus_UninitializedSDK: The Genvid library was not initialized before calling it.
 *
 * ::GenvidStatus_InvalidState: The asynchronous handler was not initialized.
 *
 * ::GenvidStatus_InvalidParameter: The stream @p streamID was not found.
 *
 * ::GenvidStatus_Success: The operation completed properly.
 */
extern GENVID_API GenvidStatus Genvid_SubmitAnnotation(const GenvidTimecode timecode, const char * streamID, const void * annotationData, const int annotationDataSize);

/**
 * Sends @p notificationDataSize bytes of @p notification.
 *
 * Notifications are dropped if the asynchronous handler isn't initialized first.
 *
 * It returns one of the following values:
 *
 * ::GenvidStatus_UninitializedSDK: The Genvid library was not initialized before calling it.
 *
 * ::GenvidStatus_InvalidState: The asynchronous handler was not initialized.
 *
 * ::GenvidStatus_UnknownError: The SDK failed to publish. The error from the messaging bus is in the
 * logs.
 *
 * ::GenvidStatus_InvalidParameter: The stream @p streamID was not found.
 *
 * ::GenvidStatus_Disconnected: The messaging system is currently disconnected.
 *
 * ::GenvidStatus_Success: The operation completed properly.
 */
extern GENVID_API GenvidStatus Genvid_SubmitNotification(const char * notificationID, const void * notificationData, const int notificationDataSize);

/**
 * Sends a video frame explicitly using @p videoDataSize bytes of @p videoData.
 *
 * The video-data format must be specified before calling this routine.
 * See Genvid_SetParameterInt() for more information.
 *
 * When automatic video source capture is used, @p videoData must be set to NULL
 * and @p videoDataSize must be set to 0.
 * See Genvid_SetParameterPointer() for more information.
 *
 * Using a negative @p timecode value calls Genvid_GetCurrentTimecode() and uses that
 * value for @p timecode.
 *
 * It returns one of the following values:
 *
 * ::GenvidStatus_UninitializedSDK: The Genvid library was not initialized before calling it.
 *
 * ::GenvidStatus_InvalidState: The asynchronous handler was not initialized.
 *
 * ::GenvidStatus_InvalidImageFormat: The image format is not supported.
 *
 * ::GenvidStatus_InvalidParameter: The stream @p streamID was not found.
 *
 * ::GenvidStatus_Success: The operation completed properly.
 */
extern GENVID_API GenvidStatus Genvid_SubmitVideoData(const GenvidTimecode timecode, const char * streamID, const void * videoData, const int videoDataSize);

/**
 * Subscribes to the event channel with the reduce-operation name @p id.
 *
 * The @p callback parameter specifies the function to call when data is
 * available.
 *
 * The @p userData is a generic user pointer that is sent to
 * @p callback when invoked.
 *
 * The call is asynchronous.
 *
 * It returns one of the following values:
 *
 * ::GenvidStatus_UninitializedSDK: The Genvid library was not initialized before calling it.
 *
 * ::GenvidStatus_InvalidParameter: @p id is null.
 *
 * ::GenvidStatus_InvalidState: The asynchronous handler was not initialized.
 *
 * ::GenvidStatus_InvalidParameter: One of the parameters is invalid.
 *
 * See Genvid_Unsubscribe() for more information.
 */
extern GENVID_API GenvidStatus Genvid_Subscribe(const char * id, GenvidEventSummaryCallback callback, void * userData);

/**
 * Unsubscribes from event channels matching the specified @p id, @p callback, and @p userData.
 *
 * Sending a NULL or empty @p id will match any id.
 *
 * Sending a NULL @p callback will match any callback.
 *
 * Sending a NULL @p userData will match any userData.
 *
 * The above rules enable unsubscribing from multiple subscriptions in the same call.
 *
 * The call is asynchronous.
 *
 * It returns one of the following values:
 *
 * ::GenvidStatus_UninitializedSDK: The Genvid library was not initialized before calling it.
 *
 * ::GenvidStatus_InvalidState: The asynchronous handler was not initialized.
 *
 * ::GenvidStatus_InvalidParameter: One of the parameters is invalid.
 *
 * See Genvid_Subscribe() for more information.
 */
extern GENVID_API GenvidStatus Genvid_Unsubscribe(const char * id, GenvidEventSummaryCallback callback, void * userData);

/**
* Synchronously checks for event results.
*
* This routine calls any appropriate callback previously set using Genvid_Subscribe()
* or Genvid_SubscribeCommand().
*
* Returns one of the following values:
*
* ::GenvidStatus_Success: The operation has completed properly and events are available.
*
* ::GenvidStatus_ConnectionTimeout: No events were available at this time.
*/
extern GENVID_API GenvidStatus Genvid_CheckForEvents();

/**
 * Subscribes to the command with the name @p id.
 *
 * The @p callback parameter specifies the function to call when commands
 * are received.
 *
 * The @p userData is a generic user pointer that is sent to
 * @p callback when it is invoked.
 *
 * The call is asynchronous.
 *
 * It returns one of the following values:
 *
 * ::GenvidStatus_UninitializedSDK: The Genvid library was not initialized before calling it.
 *
 * ::GenvidStatus_InvalidParameter: @p id is null.
 *
 * ::GenvidStatus_InvalidState: The asynchronous handler was not initialized.
 *
 * ::GenvidStatus_InvalidParameter: One of the parameters is invalid.
 *
 * See Genvid_UnsubscribeCommand() for more information.
 */
extern GENVID_API GenvidStatus Genvid_SubscribeCommand(const char * id, GenvidCommandCallback callback, void * userData);

/**
 * Unsubscribes from commands matching the specified @p id, @p callback, and @p userData.
 *
 * Sending a NULL or empty @p id will match any id.
 *
 * Sending a NULL @p callback will match any callback.
 *
 * Sending a NULL @p userData will match any userData.
 *
 * The above rules enable unsubscribing from multiple subscriptions in the same call.
 *
 * The call is asynchronous.
 *
 * It returns one of the following values:
 *
 * ::GenvidStatus_UninitializedSDK: The Genvid library was not initialized before calling it.
 *
 * ::GenvidStatus_InvalidState: The asynchronous handler was not initialized.
 *
 * ::GenvidStatus_InvalidParameter: One of the parameters is invalid.
 *
 * See Genvid_SubscribeCommand() for more information.
 */
extern GENVID_API GenvidStatus Genvid_UnsubscribeCommand(const char * id, GenvidCommandCallback callback, void * userData);

#if defined(__cplusplus)
} // extern "C"
#endif
