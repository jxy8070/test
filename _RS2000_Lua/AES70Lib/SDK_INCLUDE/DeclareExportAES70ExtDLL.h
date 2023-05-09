#pragma once
#ifdef  _AES70_DEVICE_MANAGER_				//注意:在生成该DLL的编译器中定义
#define EXPORT_AES70_CLASS   __declspec(dllexport)
#else
#define EXPORT_AES70_CLASS   __declspec(dllimport)
#endif
