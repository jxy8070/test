#pragma once
typedef enum class tagDeviceInterfaceNum:long {
	eUndefinedSubNetIf =-1,	//未定义使用的接口
	ePrimarySubNetIf   =0,
	eSecondarySubNetIf,
	eSubNetInterfaceNum,
} DEVICE_INTERFACE_NUM_T;