#include "stdafx.h"

using namespace System;
using namespace System::Reflection;
using namespace System::Runtime::CompilerServices;
using namespace System::Runtime::InteropServices;
using namespace System::Security::Permissions;

//
// �ե󪺤@���T�O�ѤU�C���ݩʶ�����C
// �ܧ�o���ݩʪ��ȧY�i�ק�ե󪺬���
// ��T�C
//
[assembly:AssemblyTitleAttribute("SunvisionCLRCommon")];
[assembly:AssemblyDescriptionAttribute("Sunvision CLR Common Library")];
[assembly:AssemblyConfigurationAttribute("")];
[assembly:AssemblyCompanyAttribute("Sunvision Inc.")];
[assembly:AssemblyProductAttribute("SunvisionCLRCommon")];
[assembly:AssemblyCopyrightAttribute("Sunvision Scientific Inc. Copyright (c)  2011")];
[assembly:AssemblyTrademarkAttribute("Sunvision")];
[assembly:AssemblyCultureAttribute("")];

//
// �ե󪺪�����T�O�ѤU�C�|���Ⱥc��:
//
//      �D�n����
//      ���n����
//      �իؽs��
//      �׭q�s��
//
// �z�i�H���w�Ҧ����ȡA�]�i�H�̷ӥH�U���覡�A�ϥ� '*' �N�׭q�M�իؽs��
// ���w���w�]��:

[assembly:AssemblyVersionAttribute("1.0.*")];

[assembly:ComVisible(false)];

[assembly:CLSCompliantAttribute(true)];

[assembly:SecurityPermission(SecurityAction::RequestMinimum, UnmanagedCode = true)];
