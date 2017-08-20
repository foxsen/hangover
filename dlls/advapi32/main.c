/*
 * Copyright 2017 Stefan Dösinger for CodeWeavers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

/* NOTE: The guest side uses mingw's headers. The host side uses Wine's headers. */

#include <windows.h>
#include <stdio.h>

#include "windows-user-services.h"
#include "dll_list.h"
#include "advapi32.h"

#ifdef QEMU_DLL_GUEST

BOOL WINAPI DllMain(HMODULE mod, DWORD reason, void *reserved)
{
    return TRUE;
}

#else

#include <wine/debug.h>
WINE_DEFAULT_DEBUG_CHANNEL(qemu_advapi32);

const struct qemu_ops *qemu_ops;

static const syscall_handler dll_functions[] =
{
    qemu_AbortSystemShutdownA,
    qemu_AbortSystemShutdownW,
    qemu_AccessCheck,
    qemu_AccessCheckAndAuditAlarmA,
    qemu_AccessCheckAndAuditAlarmW,
    qemu_AccessCheckByType,
    qemu_AddAccessAllowedAce,
    qemu_AddAccessAllowedAceEx,
    qemu_AddAccessAllowedObjectAce,
    qemu_AddAccessDeniedAce,
    qemu_AddAccessDeniedAceEx,
    qemu_AddAccessDeniedObjectAce,
    qemu_AddAce,
    qemu_AddAuditAccessAce,
    qemu_AddAuditAccessAceEx,
    qemu_AddAuditAccessObjectAce,
    qemu_AddMandatoryAce,
    qemu_AdjustTokenGroups,
    qemu_AdjustTokenPrivileges,
    qemu_AllocateAndInitializeSid,
    qemu_AllocateLocallyUniqueId,
    qemu_AreAllAccessesGranted,
    qemu_AreAnyAccessesGranted,
    qemu_BuildExplicitAccessWithNameA,
    qemu_BuildExplicitAccessWithNameW,
    qemu_BuildSecurityDescriptorA,
    qemu_BuildSecurityDescriptorW,
    qemu_BuildTrusteeWithNameA,
    qemu_BuildTrusteeWithNameW,
    qemu_BuildTrusteeWithObjectsAndNameA,
    qemu_BuildTrusteeWithObjectsAndNameW,
    qemu_BuildTrusteeWithObjectsAndSidA,
    qemu_BuildTrusteeWithObjectsAndSidW,
    qemu_BuildTrusteeWithSidA,
    qemu_BuildTrusteeWithSidW,
    qemu_ChangeServiceConfig2A,
    qemu_ChangeServiceConfig2W,
    qemu_ChangeServiceConfigA,
    qemu_ChangeServiceConfigW,
    qemu_CheckTokenMembership,
    qemu_CloseEncryptedFileRaw,
    qemu_CloseServiceHandle,
    qemu_CommandLineFromMsiDescriptor,
    qemu_ControlService,
    qemu_ConvertSecurityDescriptorToStringSecurityDescriptorA,
    qemu_ConvertSecurityDescriptorToStringSecurityDescriptorW,
    qemu_ConvertSidToStringSidA,
    qemu_ConvertSidToStringSidW,
    qemu_ConvertStringSecurityDescriptorToSecurityDescriptorA,
    qemu_ConvertStringSecurityDescriptorToSecurityDescriptorW,
    qemu_ConvertStringSidToSidA,
    qemu_ConvertStringSidToSidW,
    qemu_ConvertToAutoInheritPrivateObjectSecurity,
    qemu_CopySid,
    qemu_CreatePrivateObjectSecurity,
    qemu_CreatePrivateObjectSecurityEx,
    qemu_CreatePrivateObjectSecurityWithMultipleInheritance,
    qemu_CreateProcessAsUserA,
    qemu_CreateProcessAsUserW,
    qemu_CreateProcessWithLogonW,
    qemu_CreateProcessWithTokenW,
    qemu_CreateRestrictedToken,
    qemu_CreateServiceA,
    qemu_CreateServiceW,
    qemu_CreateWellKnownSid,
    qemu_CryptAcquireContextA,
    qemu_CryptAcquireContextW,
    qemu_CryptContextAddRef,
    qemu_CryptCreateHash,
    qemu_CryptDecrypt,
    qemu_CryptDeriveKey,
    qemu_CryptDestroyHash,
    qemu_CryptDestroyKey,
    qemu_CryptDuplicateHash,
    qemu_CryptDuplicateKey,
    qemu_CryptEncrypt,
    qemu_CryptEnumProvidersA,
    qemu_CryptEnumProvidersW,
    qemu_CryptEnumProviderTypesA,
    qemu_CryptEnumProviderTypesW,
    qemu_CryptExportKey,
    qemu_CryptGenKey,
    qemu_CryptGenRandom,
    qemu_CryptGetDefaultProviderA,
    qemu_CryptGetDefaultProviderW,
    qemu_CryptGetHashParam,
    qemu_CryptGetKeyParam,
    qemu_CryptGetProvParam,
    qemu_CryptGetUserKey,
    qemu_CryptHashData,
    qemu_CryptHashSessionKey,
    qemu_CryptImportKey,
    qemu_CryptReleaseContext,
    qemu_CryptSetHashParam,
    qemu_CryptSetKeyParam,
    qemu_CryptSetProviderA,
    qemu_CryptSetProviderExA,
    qemu_CryptSetProviderExW,
    qemu_CryptSetProviderW,
    qemu_CryptSetProvParam,
    qemu_CryptSignHashA,
    qemu_CryptSignHashW,
    qemu_CryptVerifySignatureA,
    qemu_CryptVerifySignatureW,
    qemu_DecryptFileA,
    qemu_DecryptFileW,
    qemu_DeleteAce,
    qemu_DeleteService,
    qemu_DestroyPrivateObjectSecurity,
    qemu_DuplicateToken,
    qemu_DuplicateTokenEx,
    qemu_EncryptFileA,
    qemu_EncryptFileW,
    qemu_EnumDependentServicesA,
    qemu_EnumDependentServicesW,
    qemu_EnumServicesStatusA,
    qemu_EnumServicesStatusExA,
    qemu_EnumServicesStatusExW,
    qemu_EnumServicesStatusW,
    qemu_EqualPrefixSid,
    qemu_EqualSid,
    qemu_FileEncryptionStatusA,
    qemu_FileEncryptionStatusW,
    qemu_FindFirstFreeAce,
    qemu_FreeSid,
    qemu_GetAce,
    qemu_GetAclInformation,
    qemu_GetAuditedPermissionsFromAclA,
    qemu_GetAuditedPermissionsFromAclW,
    qemu_GetCurrentHwProfileA,
    qemu_GetCurrentHwProfileW,
    qemu_GetEffectiveRightsFromAclA,
    qemu_GetEffectiveRightsFromAclW,
    qemu_GetExplicitEntriesFromAclA,
    qemu_GetExplicitEntriesFromAclW,
    qemu_GetFileSecurityA,
    qemu_GetFileSecurityW,
    qemu_GetKernelObjectSecurity,
    qemu_GetLengthSid,
    qemu_GetNamedSecurityInfoA,
    qemu_GetNamedSecurityInfoExA,
    qemu_GetNamedSecurityInfoExW,
    qemu_GetNamedSecurityInfoW,
    qemu_GetPrivateObjectSecurity,
    qemu_GetSecurityDescriptorControl,
    qemu_GetSecurityDescriptorDacl,
    qemu_GetSecurityDescriptorGroup,
    qemu_GetSecurityDescriptorLength,
    qemu_GetSecurityDescriptorOwner,
    qemu_GetSecurityDescriptorSacl,
    qemu_GetSecurityInfo,
    qemu_GetSecurityInfoExA,
    qemu_GetSecurityInfoExW,
    qemu_GetServiceDisplayNameA,
    qemu_GetServiceDisplayNameW,
    qemu_GetServiceKeyNameA,
    qemu_GetServiceKeyNameW,
    qemu_GetSidIdentifierAuthority,
    qemu_GetSidLengthRequired,
    qemu_GetSidSubAuthority,
    qemu_GetSidSubAuthorityCount,
    qemu_GetTokenInformation,
    qemu_GetTrusteeFormA,
    qemu_GetTrusteeFormW,
    qemu_GetTrusteeNameA,
    qemu_GetTrusteeNameW,
    qemu_GetTrusteeTypeA,
    qemu_GetTrusteeTypeW,
    qemu_GetUserNameA,
    qemu_GetUserNameW,
    qemu_GetWindowsAccountDomainSid,
    qemu_ImpersonateAnonymousToken,
    qemu_ImpersonateLoggedOnUser,
    qemu_ImpersonateNamedPipeClient,
    qemu_ImpersonateSelf,
    qemu_InitializeAcl,
    qemu_InitializeSecurityDescriptor,
    qemu_InitializeSid,
    qemu_InitiateSystemShutdownA,
    qemu_InitiateSystemShutdownExA,
    qemu_InitiateSystemShutdownExW,
    qemu_InitiateSystemShutdownW,
    qemu_IsTextUnicode,
    qemu_IsTokenRestricted,
    qemu_IsValidAcl,
    qemu_IsValidSecurityDescriptor,
    qemu_IsValidSid,
    qemu_IsWellKnownSid,
    qemu_LockServiceDatabase,
    qemu_LogonUserA,
    qemu_LogonUserW,
    qemu_LookupAccountNameA,
    qemu_LookupAccountNameW,
    qemu_LookupAccountSidA,
    qemu_LookupAccountSidW,
    qemu_LookupPrivilegeDisplayNameA,
    qemu_LookupPrivilegeDisplayNameW,
    qemu_LookupPrivilegeNameA,
    qemu_LookupPrivilegeNameW,
    qemu_LookupPrivilegeValueA,
    qemu_LookupPrivilegeValueW,
    qemu_LookupSecurityDescriptorPartsA,
    qemu_LookupSecurityDescriptorPartsW,
    qemu_MakeAbsoluteSD,
    qemu_MakeSelfRelativeSD,
    qemu_MapGenericMask,
    qemu_NotifyBootConfigStatus,
    qemu_NotifyServiceStatusChangeW,
    qemu_ObjectCloseAuditAlarmA,
    qemu_ObjectCloseAuditAlarmW,
    qemu_ObjectDeleteAuditAlarmW,
    qemu_ObjectOpenAuditAlarmA,
    qemu_ObjectOpenAuditAlarmW,
    qemu_ObjectPrivilegeAuditAlarmA,
    qemu_ObjectPrivilegeAuditAlarmW,
    qemu_OpenEncryptedFileRawA,
    qemu_OpenEncryptedFileRawW,
    qemu_OpenProcessToken,
    qemu_OpenSCManagerA,
    qemu_OpenSCManagerW,
    qemu_OpenServiceA,
    qemu_OpenServiceW,
    qemu_OpenThreadToken,
    qemu_PrivilegeCheck,
    qemu_PrivilegedServiceAuditAlarmA,
    qemu_PrivilegedServiceAuditAlarmW,
    qemu_QueryServiceConfig2A,
    qemu_QueryServiceConfig2W,
    qemu_QueryServiceConfigA,
    qemu_QueryServiceConfigW,
    qemu_QueryServiceLockStatusA,
    qemu_QueryServiceLockStatusW,
    qemu_QueryServiceObjectSecurity,
    qemu_QueryServiceStatus,
    qemu_QueryServiceStatusEx,
    qemu_QueryWindows31FilesMigration,
    qemu_ReadEncryptedFileRaw,
    qemu_RegCloseKey,
    qemu_RegConnectRegistryA,
    qemu_RegConnectRegistryW,
    qemu_RegCopyTreeA,
    qemu_RegCopyTreeW,
    qemu_RegCreateKeyA,
    qemu_RegCreateKeyExA,
    qemu_RegCreateKeyExW,
    qemu_RegCreateKeyTransactedA,
    qemu_RegCreateKeyTransactedW,
    qemu_RegCreateKeyW,
    qemu_RegDeleteKeyA,
    qemu_RegDeleteKeyExA,
    qemu_RegDeleteKeyExW,
    qemu_RegDeleteKeyValueA,
    qemu_RegDeleteKeyValueW,
    qemu_RegDeleteKeyW,
    qemu_RegDeleteTreeA,
    qemu_RegDeleteTreeW,
    qemu_RegDeleteValueA,
    qemu_RegDeleteValueW,
    qemu_RegDisablePredefinedCache,
    qemu_RegDisableReflectionKey,
    qemu_RegEnumKeyA,
    qemu_RegEnumKeyExA,
    qemu_RegEnumKeyExW,
    qemu_RegEnumKeyW,
    qemu_RegEnumValueA,
    qemu_RegEnumValueW,
    qemu_RegFlushKey,
    qemu_RegGetKeySecurity,
    qemu_RegGetValueA,
    qemu_RegGetValueW,
    qemu_RegisterServiceCtrlHandlerA,
    qemu_RegisterServiceCtrlHandlerExA,
    qemu_RegisterServiceCtrlHandlerExW,
    qemu_RegisterServiceCtrlHandlerW,
    qemu_RegisterWaitChainCOMCallback,
    qemu_RegLoadKeyA,
    qemu_RegLoadKeyW,
    qemu_RegLoadMUIStringA,
    qemu_RegLoadMUIStringW,
    qemu_RegNotifyChangeKeyValue,
    qemu_RegOpenCurrentUser,
    qemu_RegOpenKeyA,
    qemu_RegOpenKeyExA,
    qemu_RegOpenKeyExW,
    qemu_RegOpenKeyW,
    qemu_RegOpenUserClassesRoot,
    qemu_RegOverridePredefKey,
    qemu_RegQueryInfoKeyA,
    qemu_RegQueryInfoKeyW,
    qemu_RegQueryMultipleValuesA,
    qemu_RegQueryMultipleValuesW,
    qemu_RegQueryValueA,
    qemu_RegQueryValueExA,
    qemu_RegQueryValueExW,
    qemu_RegQueryValueW,
    qemu_RegReplaceKeyA,
    qemu_RegReplaceKeyW,
    qemu_RegRestoreKeyA,
    qemu_RegRestoreKeyW,
    qemu_RegSaveKeyA,
    qemu_RegSaveKeyExA,
    qemu_RegSaveKeyExW,
    qemu_RegSaveKeyW,
    qemu_RegSetKeySecurity,
    qemu_RegSetKeyValueA,
    qemu_RegSetKeyValueW,
    qemu_RegSetValueA,
    qemu_RegSetValueExA,
    qemu_RegSetValueExW,
    qemu_RegSetValueW,
    qemu_RegUnLoadKeyA,
    qemu_RegUnLoadKeyW,
    qemu_RevertToSelf,
    qemu_SaferCloseLevel,
    qemu_SaferComputeTokenFromLevel,
    qemu_SaferCreateLevel,
    qemu_SaferGetPolicyInformation,
    qemu_SaferSetLevelInformation,
    qemu_SetAclInformation,
    qemu_SetEntriesInAclA,
    qemu_SetEntriesInAclW,
    qemu_SetFileSecurityA,
    qemu_SetFileSecurityW,
    qemu_SetKernelObjectSecurity,
    qemu_SetNamedSecurityInfoA,
    qemu_SetNamedSecurityInfoW,
    qemu_SetPrivateObjectSecurity,
    qemu_SetSecurityDescriptorControl,
    qemu_SetSecurityDescriptorDacl,
    qemu_SetSecurityDescriptorGroup,
    qemu_SetSecurityDescriptorOwner,
    qemu_SetSecurityDescriptorSacl,
    qemu_SetSecurityInfo,
    qemu_SetServiceBits,
    qemu_SetServiceObjectSecurity,
    qemu_SetServiceStatus,
    qemu_SetThreadToken,
    qemu_SetTokenInformation,
    qemu_StartServiceA,
    qemu_StartServiceCtrlDispatcherA,
    qemu_StartServiceCtrlDispatcherW,
    qemu_StartServiceW,
    qemu_SynchronizeWindows31FilesAndWindowsNTRegistry,
    qemu_SystemFunction030,
    qemu_SystemFunction035,
    qemu_SystemFunction036,
    qemu_SystemFunction040,
    qemu_SystemFunction041,
    qemu_TreeResetNamedSecurityInfoW,
    qemu_UnlockServiceDatabase,
    qemu_WriteEncryptedFileRaw,
};

const WINAPI syscall_handler *qemu_dll_register(const struct qemu_ops *ops, uint32_t *dll_num)
{
    WINE_TRACE("Loading host-side advapi32 wrapper.\n");

    qemu_ops = ops;
    *dll_num = QEMU_CURRENT_DLL;

    return dll_functions;
}

#endif
