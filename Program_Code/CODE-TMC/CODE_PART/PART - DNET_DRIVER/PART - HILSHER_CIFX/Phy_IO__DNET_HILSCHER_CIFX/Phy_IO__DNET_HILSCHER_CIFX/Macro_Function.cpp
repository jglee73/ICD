#include "stdafx.h"

#include "cifXErrors.h"
#include "cifXUser.h"


// ...
CString Macro__Get_Message_Of_GenErr(const unsigned char gen_err)
{
	if(gen_err == 0x000)			return "No error";

	if(gen_err == 0x002)			return "Resources unavailable";
	if(gen_err == 0x008)			return "Service not available";
	if(gen_err == 0x009)			return "Invalid attribute value";
	if(gen_err == 0x011)			return "Already in request mode";
	if(gen_err == 0x012)			return "Object state conflict";
	if(gen_err == 0x014)			return "Attribute not settable";
	if(gen_err == 0x015)			return "A permission check failed";
	if(gen_err == 0x016)			return "State conflict, device state prohibits the command execution";
	if(gen_err == 0x019)			return "Not enough data received";
	if(gen_err == 0x020)			return "Attribute not supported";
	if(gen_err == 0x021)			return "Too much data received";
	if(gen_err == 0x022)			return "Object does not exist";
	if(gen_err == 0x023)			return "Reply data too large, internal buffer to small";

	CString str_err;
	str_err.Format("Unknown Error (0x%02X)", gen_err);

	return str_err;
}

CString Macro__Get_CIFX_Error(const unsigned int cifx_err)
{
	if(cifx_err == CIFX_NO_ERROR)								return "No Error";

	// Generic Errors
	if(cifx_err == CIFX_INVALID_POINTER)						return "Invalid pointer (e.g. NULL) passed to driver";
	if(cifx_err == CIFX_INVALID_BOARD)							return "No board with the given name / index available";
	if(cifx_err == CIFX_INVALID_CHANNEL)						return "No channel with the given index available";
	if(cifx_err == CIFX_INVALID_HANDLE)							return "Invalid handle passed to driver";
	if(cifx_err == CIFX_INVALID_PARAMETER)						return "Invalid parameter";
	if(cifx_err == CIFX_INVALID_COMMAND)						return "Invalid command";
	if(cifx_err == CIFX_INVALID_BUFFERSIZE)						return "Invalid buffer size";
	if(cifx_err == CIFX_INVALID_ACCESS_SIZE)					return "Invalid access size";

	if(cifx_err == CIFX_FUNCTION_FAILED)						return "Function failed";

	if(cifx_err == CIFX_FILE_OPEN_FAILED)						return "File could not be opened";
	if(cifx_err == CIFX_FILE_SIZE_ZERO)							return "File size is zero";
	if(cifx_err == CIFX_FILE_LOAD_INSUFF_MEM)					return "Insufficient memory to load file";
	if(cifx_err == CIFX_FILE_CHECKSUM_ERROR)					return "File checksum compare failed";
	if(cifx_err == CIFX_FILE_READ_ERROR)						return "Error reading from file";
	if(cifx_err == CIFX_FILE_TYPE_INVALID)						return "Invalid file type";
	if(cifx_err == CIFX_FILE_NAME_INVALID)						return "Invalid file name";

	if(cifx_err == CIFX_FUNCTION_NOT_AVAILABLE)					return "Driver function not available";
	if(cifx_err == CIFX_BUFFER_TOO_SHORT)						return "Given buffer is too short";
	if(cifx_err == CIFX_MEMORY_MAPPING_FAILED)					return "Failed to map the memory";
	if(cifx_err == CIFX_NO_MORE_ENTRIES)						return "No more entries available";

	if(cifx_err == CIFX_CALLBACK_MODE_UNKNOWN)					return "Unknown callback handling mode";
	if(cifx_err == CIFX_CALLBACK_CREATE_EVENT_FAILED)			return "Failed to create callback events";
	if(cifx_err == CIFX_CALLBACK_CREATE_RECV_BUFFER)			return "Failed to create callback receive buffer";
	if(cifx_err == CIFX_CALLBACK_ALREADY_USED)					return "Callback already used";
	if(cifx_err == CIFX_CALLBACK_NOT_REGISTERED)				return "Callback was not registered before";
	if(cifx_err == CIFX_INTERRUPT_DISABLED)						return "Interrupt is disabled";

	// Generic Driver Errors
	if(cifx_err == CIFX_DRV_NOT_INITIALIZED)					return "Driver not initialized";
	if(cifx_err == CIFX_DRV_INIT_STATE_ERROR)					return "Driver init state error";
	if(cifx_err == CIFX_DRV_READ_STATE_ERROR)					return "Driver read state error";
	if(cifx_err == CIFX_DRV_CMD_ACTIVE)							return "Command is active on device";
	if(cifx_err == CIFX_DRV_DOWNLOAD_FAILED)					return "General error during download";
	if(cifx_err == CIFX_DRV_WRONG_DRIVER_VERSION)				return "Wrong driver version";
	if(cifx_err == CIFX_DRV_DRIVER_NOT_LOADED)					return "CIFx driver is not running";
	if(cifx_err == CIFX_DRV_INIT_ERROR)							return "Failed to initialize the device";
	if(cifx_err == CIFX_DRV_CHANNEL_NOT_INITIALIZED)			return "Channel not initialized (xOpenChannel not called)";
	if(cifx_err == CIFX_DRV_IO_CONTROL_FAILED)					return "IOControl call failed";
	if(cifx_err == CIFX_DRV_NOT_OPENED)							return "Driver was not opened";
	if(cifx_err == CIFX_DRV_DOWNLOAD_STORAGE_UNKNOWN)			return "Unknown download storage type (RAM/FLASH based) found";
	if(cifx_err == CIFX_DRV_DOWNLOAD_FW_WRONG_CHANNEL)			return "Channel number for a firmware download not supported";
	if(cifx_err == CIFX_DRV_DOWNLOAD_MODULE_NO_BASEOS)			return "Modules are not allowed without a Base OS firmware";

	// Generic Device Errors
	if(cifx_err == CIFX_DEV_DPM_ACCESS_ERROR)					return "Dual port memory not accessible (board not found)";
	if(cifx_err == CIFX_DEV_NOT_READY)							return "Device not ready (ready flag failed)";
	if(cifx_err == CIFX_DEV_NOT_RUNNING)						return "Device not running (running flag failed)";
	if(cifx_err == CIFX_DEV_WATCHDOG_FAILED)					return "Watchdog test failed";
	if(cifx_err == CIFX_DEV_SYSERR)								return "Error in handshake flags";
	if(cifx_err == CIFX_DEV_MAILBOX_FULL)						return "Send mailbox is full";
	if(cifx_err == CIFX_DEV_PUT_TIMEOUT)						return "Send packet timeout";
	if(cifx_err == CIFX_DEV_GET_TIMEOUT)						return "Receive packet timeout";
	if(cifx_err == CIFX_DEV_GET_NO_PACKET)						return "No packet available";
	if(cifx_err == CIFX_DEV_MAILBOX_TOO_SHORT)					return "Mailbox too short";
	if(cifx_err == CIFX_DEV_RESET_TIMEOUT)						return "Reset command timeout";
	if(cifx_err == CIFX_DEV_NO_COM_FLAG)						return "COM-flag not set";
	if(cifx_err == CIFX_DEV_EXCHANGE_FAILED)					return "I/O data exchange failed";
	if(cifx_err == CIFX_DEV_EXCHANGE_TIMEOUT)					return "I/O data exchange timeout";
	if(cifx_err == CIFX_DEV_COM_MODE_UNKNOWN)					return "Unknown I/O exchange mode";
	if(cifx_err == CIFX_DEV_FUNCTION_FAILED)					return "Device function failed";
	if(cifx_err == CIFX_DEV_DPMSIZE_MISMATCH)					return "DPM size differs from configuration";
	if(cifx_err == CIFX_DEV_STATE_MODE_UNKNOWN)					return "Unknown state mode";
	if(cifx_err == CIFX_DEV_HW_PORT_IS_USED)					return "Device is still accessed";
	if(cifx_err == CIFX_DEV_CONFIG_LOCK_TIMEOUT)				return "Configuration locking timeout";
	if(cifx_err == CIFX_DEV_CONFIG_UNLOCK_TIMEOUT)				return "Configuration unlocking timeout";
	if(cifx_err == CIFX_DEV_HOST_STATE_SET_TIMEOUT)				return "Set HOST state timeout";
	if(cifx_err == CIFX_DEV_HOST_STATE_CLEAR_TIMEOUT)			return "Clear HOST state timeout";
	if(cifx_err == CIFX_DEV_INITIALIZATION_TIMEOUT)				return "Timeout during channel initialization";
	if(cifx_err == CIFX_DEV_BUS_STATE_ON_TIMEOUT)				return "Set Bus ON Timeout";
	if(cifx_err == CIFX_DEV_BUS_STATE_OFF_TIMEOUT)				return "Set Bus OFF Timeout";
	if(cifx_err == CIFX_DEV_MODULE_ALREADY_RUNNING)				return "Module already running";
	if(cifx_err == CIFX_DEV_MODULE_ALREADY_EXISTS)				return "Module already exists";
	if(cifx_err == CIFX_DEV_DMA_INSUFF_BUFFER_COUNT)			return "Number of configured DMA buffers insufficient";
	if(cifx_err == CIFX_DEV_DMA_BUFFER_TOO_SMALL)				return "DMA buffers size too small (min size 256Byte)";
	if(cifx_err == CIFX_DEV_DMA_BUFFER_TOO_BIG)					return "DMA buffers size too big (max size 63,75KByte)";
	if(cifx_err == CIFX_DEV_DMA_BUFFER_NOT_ALIGNED)				return "DMA buffer alignment failed (must be 256Byte)";
	if(cifx_err == CIFX_DEV_DMA_HANSHAKEMODE_NOT_SUPPORTED)		return "I/O data uncontrolled handshake mode not supported";
	if(cifx_err == CIFX_DEV_DMA_IO_AREA_NOT_SUPPORTED)			return "I/O area in DMA mode not supported (only area 0 possible)";
	if(cifx_err == CIFX_DEV_DMA_STATE_ON_TIMEOUT)				return "Set DMA ON Timeout";
	if(cifx_err == CIFX_DEV_DMA_STATE_OFF_TIMEOUT)				return "Set DMA OFF Timeout";
	if(cifx_err == CIFX_DEV_SYNC_STATE_INVALID_MODE)			return "Device is in invalid mode for this operation";
	if(cifx_err == CIFX_DEV_SYNC_STATE_TIMEOUT)					return "Waiting for synchronization event bits timed out";

	// CIFX API Transport Errors
	if(cifx_err == CIFX_TRANSPORT_SEND_TIMEOUT)					return "Time out while sending data";
	if(cifx_err == CIFX_TRANSPORT_RECV_TIMEOUT)					return "Time out waiting for incoming data";
	if(cifx_err == CIFX_TRANSPORT_CONNECT)						return "Unable to communicate to the device / no answer";
	if(cifx_err == CIFX_TRANSPORT_ABORTED)						return "Transfer has been aborted due to keep alive timeout or interface detachment";
	if(cifx_err == CIFX_TRANSPORT_INVALID_RESPONSE)				return "The packet response was rejected due to invalid packet data";
	if(cifx_err == CIFX_TRANSPORT_UNKNOWN_DATALAYER)			return "The data layer provided by the device is not supported";
	if(cifx_err == CIFX_CONNECTOR_FUNCTIONS_READ_ERROR)			return "Error reading the connector functions from the DLL";
	if(cifx_err == CIFX_CONNECTOR_IDENTIFIER_TOO_LONG)			return "Connector delivers an identifier longer than 6 characters";
	if(cifx_err == CIFX_CONNECTOR_IDENTIFIER_EMPTY)				return "Connector delivers an empty identifier";
	if(cifx_err == CIFX_CONNECTOR_DUPLICATE_IDENTIFIER)			return "Connector identifier already used";

	// CIFX API Transport Header State Errors
	if(cifx_err == CIFX_TRANSPORT_ERROR_UNKNOWN)				return "Unknown error code in transport header";
	if(cifx_err == CIFX_TRANSPORT_CHECKSUM_ERROR)				return "CRC16 checksum failed";
	if(cifx_err == CIFX_TRANSPORT_LENGTH_INCOMPLETE)			return "Transaction with incomplete length detected";
	if(cifx_err == CIFX_TRANSPORT_DATA_TYPE_UNKOWN)				return "Device does not support requested data type";
	if(cifx_err == CIFX_TRANSPORT_DEVICE_UNKNOWN)				return "Device not available/unknown";
	if(cifx_err == CIFX_TRANSPORT_CHANNEL_UNKNOWN)				return "Channel not available/unknown";
	if(cifx_err == CIFX_TRANSPORT_SEQUENCE)						return "Sequence error detected";
	if(cifx_err == CIFX_TRANSPORT_BUFFEROVERFLOW)				return "Buffer overflow detected";
	if(cifx_err == CIFX_TRANSPORT_RESOURCE)						return "Device signals out of resources";
	if(cifx_err == CIFX_TRANSPORT_KEEPALIVE)					return "Device connection monitoring error (Keep alive)";
	if(cifx_err == CIFX_TRANSPORT_DATA_TOO_SHORT)				return "Received transaction data too short";

	// ...
	CString str_err;

	str_err.Format("Unknown Error (0x%08X)", cifx_err);
	return str_err;
}
