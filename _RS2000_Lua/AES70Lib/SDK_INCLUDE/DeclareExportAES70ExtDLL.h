#pragma once
#ifdef  _AES70_DEVICE_MANAGER_				//ע��:�����ɸ�DLL�ı������ж���
#define EXPORT_AES70_CLASS   __declspec(dllexport)
#else
#define EXPORT_AES70_CLASS   __declspec(dllimport)
#endif
