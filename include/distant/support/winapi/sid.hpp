#pragma once

#include <boost\winapi\basic_types.hpp>

namespace boost::winapi
{

#if !defined (BOOST_USE_WINDOWS_H)
	// Universal well-known SIDs
	constexpr auto SECURITY_NULL_SID_AUTHORITY_         = {0,0,0,0,0,0};
	constexpr auto SECURITY_WORLD_SID_AUTHORITY_        = {0,0,0,0,0,1};
	constexpr auto SECURITY_LOCAL_SID_AUTHORITY_        = {0,0,0,0,0,2};
	constexpr auto SECURITY_CREATOR_SID_AUTHORITY_      = {0,0,0,0,0,3};
	constexpr auto SECURITY_NON_UNIQUE_AUTHORITY_       = {0,0,0,0,0,4};
	constexpr auto SECURITY_RESOURCE_MANAGER_AUTHORITY_ = {0,0,0,0,0,9};

	// NT well-known SIDs
	constexpr auto SECURITY_NULL_RID_                                   = (0x00000000L);
	constexpr auto SECURITY_WORLD_RID_                                  = (0x00000000L);
	constexpr auto SECURITY_LOCAL_RID_                                  = (0x00000000L);
	constexpr auto SECURITY_LOCAL_LOGON_RID_                            = (0x00000001L);
	constexpr auto SECURITY_CREATOR_OWNER_RID_                          = (0x00000000L);
	constexpr auto SECURITY_CREATOR_GROUP_RID_                          = (0x00000001L);
	constexpr auto SECURITY_CREATOR_OWNER_SERVER_RID_                   = (0x00000002L);
	constexpr auto SECURITY_CREATOR_GROUP_SERVER_RID_                   = (0x00000003L);
	constexpr auto SECURITY_CREATOR_OWNER_RIGHTS_RID_                   = (0x00000004L);
	constexpr auto SECURITY_NT_AUTHORITY_                               = {0,0,0,0,0,5};
	constexpr auto SECURITY_DIALUP_RID_                                 = (0x00000001L);
	constexpr auto SECURITY_NETWORK_RID_                                = (0x00000002L);
	constexpr auto SECURITY_BATCH_RID_                                  = (0x00000003L);
	constexpr auto SECURITY_INTERACTIVE_RID_                            = (0x00000004L);
	constexpr auto SECURITY_LOGON_IDS_RID_                              = (0x00000005L);
	constexpr auto SECURITY_LOGON_IDS_RID_COUNT_                        = (3L);
	constexpr auto SECURITY_SERVICE_RID_                                = (0x00000006L);
	constexpr auto SECURITY_ANONYMOUS_LOGON_RID_                        = (0x00000007L);
	constexpr auto SECURITY_PROXY_RID_                                  = (0x00000008L);
	constexpr auto SECURITY_ENTERPRISE_CONTROLLERS_RID_                 = (0x00000009L);
	constexpr auto SECURITY_SERVER_LOGON_RID_                           = SECURITY_ENTERPRISE_CONTROLLERS_RID;
	constexpr auto SECURITY_SERVER_LOGON_RID_                           = SECURITY_ENTERPRISE_CONTROLLERS_RID;
	constexpr auto SECURITY_SERVER_LOGON_RID_                           = SECURITY_ENTERPRISE_CONTROLLERS_RID;
	constexpr auto SECURITY_PRINCIPAL_SELF_RID_                         = (0x0000000AL);
	constexpr auto SECURITY_AUTHENTICATED_USER_RID_                     = (0x0000000BL);
	constexpr auto SECURITY_RESTRICTED_CODE_RID_                        = (0x0000000CL);
	constexpr auto SECURITY_TERMINAL_SERVER_RID_                        = (0x0000000DL);
	constexpr auto SECURITY_REMOTE_LOGON_RID_                           = (0x0000000EL);
	constexpr auto SECURITY_THIS_ORGANIZATION_RID_                      = (0x0000000FL);
	constexpr auto SECURITY_IUSER_RID_                                  = (0x00000011L);
	constexpr auto SECURITY_LOCAL_SYSTEM_RID_                           = (0x00000012L);
	constexpr auto SECURITY_LOCAL_SERVICE_RID_                          = (0x00000013L);
	constexpr auto SECURITY_NETWORK_SERVICE_RID_                        = (0x00000014L);
	constexpr auto SECURITY_NT_NON_UNIQUE_                              = (0x00000015L);
	constexpr auto SECURITY_NT_NON_UNIQUE_SUB_AUTH_COUNT_               = (3L);
	constexpr auto SECURITY_ENTERPRISE_READONLY_CONTROLLERS_RID_        = (0x00000016L);
	constexpr auto SECURITY_BUILTIN_DOMAIN_RID_                         = (0x00000020L);
	constexpr auto SECURITY_WRITE_RESTRICTED_CODE_RID_                  = (0x00000021L);
	constexpr auto SECURITY_PACKAGE_BASE_RID_                           = (0x00000040L);
	constexpr auto SECURITY_PACKAGE_RID_COUNT_                          = (2L);
	constexpr auto SECURITY_PACKAGE_NTLM_RID_                           = (0x0000000AL);
	constexpr auto SECURITY_PACKAGE_SCHANNEL_RID_                       = (0x0000000EL);
	constexpr auto SECURITY_PACKAGE_DIGEST_RID_                         = (0x00000015L);
	constexpr auto SECURITY_CRED_TYPE_BASE_RID_                         = (0x00000041L);
	constexpr auto SECURITY_CRED_TYPE_RID_COUNT_                        = (2L);
	constexpr auto SECURITY_CRED_TYPE_THIS_ORG_CERT_RID_                = (0x00000001L);
	constexpr auto SECURITY_MIN_BASE_RID_                               = (0x00000050L);
	constexpr auto SECURITY_SERVICE_ID_BASE_RID_                        = (0x00000050L);
	constexpr auto SECURITY_SERVICE_ID_RID_COUNT_                       = (6L);
	constexpr auto SECURITY_RESERVED_ID_BASE_RID_                       = (0x00000051L);
	constexpr auto SECURITY_APPPOOL_ID_BASE_RID_                        = (0x00000052L);
	constexpr auto SECURITY_APPPOOL_ID_RID_COUNT_                       = (6L);
	constexpr auto SECURITY_VIRTUALSERVER_ID_BASE_RID_                  = (0x00000053L);
	constexpr auto SECURITY_VIRTUALSERVER_ID_RID_COUNT_                 = (6L);
	constexpr auto SECURITY_USERMODEDRIVERHOST_ID_BASE_RID_             = (0x00000054L);
	constexpr auto SECURITY_USERMODEDRIVERHOST_ID_RID_COUNT_            = (6L);
	constexpr auto SECURITY_CLOUD_INFRASTRUCTURE_SERVICES_ID_BASE_RID_  = (0x00000055L);
	constexpr auto SECURITY_CLOUD_INFRASTRUCTURE_SERVICES_ID_RID_COUNT_ = (6L);
	constexpr auto SECURITY_WMIHOST_ID_BASE_RID_                        = (0x00000056L);
	constexpr auto SECURITY_WMIHOST_ID_RID_COUNT_                       = (6L);
	constexpr auto SECURITY_TASK_ID_BASE_RID_                           = (0x00000057L);
	constexpr auto SECURITY_NFS_ID_BASE_RID_                            = (0x00000058L);
	constexpr auto SECURITY_COM_ID_BASE_RID_                            = (0x00000059L);
	constexpr auto SECURITY_WINDOW_MANAGER_BASE_RID_                    = (0x0000005AL);
	constexpr auto SECURITY_RDV_GFX_BASE_RID_                           = (0x0000005BL);
	constexpr auto SECURITY_DASHOST_ID_BASE_RID_                        = (0x0000005CL);
	constexpr auto SECURITY_DASHOST_ID_RID_COUNT_                       = (6L);
	constexpr auto SECURITY_VIRTUALACCOUNT_ID_RID_COUNT_                = (6L);
	constexpr auto SECURITY_MAX_BASE_RID_                               = (0x0000006FL);
	constexpr auto SECURITY_MAX_ALWAYS_FILTERED_                        = (0x000003E7L);
	constexpr auto SECURITY_MIN_NEVER_FILTERED_                         = (0x000003E8L);
	constexpr auto SECURITY_OTHER_ORGANIZATION_RID_                     = (0x000003E8L);


#endif

	/////////////////////////////////////////////////////////////////////////////
	//                                                                         //
	// Universal well-known SIDs                                               //
	//                                                                         //
	//     Null SID                     S-1-0-0                                //
	//     World                        S-1-1-0                                //
	//     Local                        S-1-2-0                                //
	//     Creator Owner ID             S-1-3-0                                //
	//     Creator Group ID             S-1-3-1                                //
	//     Creator Owner Server ID      S-1-3-2                                //
	//     Creator Group Server ID      S-1-3-3                                //
	//                                                                         //
	//     (Non-unique IDs)             S-1-4                                  //
	//                                                                         //
	/////////////////////////////////////////////////////////////////////////////
	// **Comment taken from winnt.h
	constexpr auto SECURITY_NULL_SID_AUTHORITY_         = SECURITY_NULL_SID_AUTHORITY;
	constexpr auto SECURITY_WORLD_SID_AUTHORITY_        = SECURITY_WORLD_SID_AUTHORITY;
	constexpr auto SECURITY_LOCAL_SID_AUTHORITY_        = SECURITY_LOCAL_SID_AUTHORITY;
	constexpr auto SECURITY_CREATOR_SID_AUTHORITY_      = SECURITY_CREATOR_SID_AUTHORITY;
	constexpr auto SECURITY_NON_UNIQUE_AUTHORITY_       = SECURITY_NON_UNIQUE_AUTHORITY;
	constexpr auto SECURITY_RESOURCE_MANAGER_AUTHORITY_ = SECURITY_RESOURCE_MANAGER_AUTHORITY;

	///////////////////////////////////////////////////////////////////////////////
	//                                                                           //
	// NT well-known SIDs                                                        //
	//                                                                           //
	//     NT Authority            S-1-5                                         //
	//     Dialup                  S-1-5-1                                       //
	//                                                                           //
	//     Network                 S-1-5-2                                       //
	//     Batch                   S-1-5-3                                       //
	//     Interactive             S-1-5-4                                       //
	//     (Logon IDs)             S-1-5-5-X-Y                                   //
	//     Service                 S-1-5-6                                       //
	//     AnonymousLogon          S-1-5-7       (aka null logon session)        //
	//     Proxy                   S-1-5-8                                       //
	//     Enterprise DC (EDC)     S-1-5-9       (aka domain controller account) //
	//     Self                    S-1-5-10      (self RID)                      //
	//     Authenticated User      S-1-5-11      (Authenticated user somewhere)  //
	//     Restricted Code         S-1-5-12      (Running restricted code)       //
	//     Terminal Server         S-1-5-13      (Running on Terminal Server)    //
	//     Remote Logon            S-1-5-14      (Remote Interactive Logon)      //
	//     This Organization       S-1-5-15                                      //
	//                                                                           //
	//     IUser                   S-1-5-17
	//     Local System            S-1-5-18                                      //
	//     Local Service           S-1-5-19                                      //
	//     Network Service         S-1-5-20                                      //
	//                                                                           //
	//     (NT non-unique IDs)     S-1-5-0x15-... (NT Domain Sids)               //
	//                                                                           //
	//     (Built-in domain)       S-1-5-0x20                                    //
	//                                                                           //
	//     (Security Package IDs)  S-1-5-0x40                                    //
	//     NTLM Authentication     S-1-5-0x40-10                                 //
	//     SChannel Authentication S-1-5-0x40-14                                 //
	//     Digest Authentication   S-1-5-0x40-21                                 //
	//                                                                           //
	//     Other Organization      S-1-5-1000    (>=1000 can not be filtered)    //
	//                                                                           //
	///////////////////////////////////////////////////////////////////////////////
	// **Comment taken from winnt.h
	constexpr auto SECURITY_NT_AUTHORITY_                               = SECURITY_NT_AUTHORITY;
	constexpr auto SECURITY_DIALUP_RID_                                 = SECURITY_DIALUP_RID;
	constexpr auto SECURITY_NETWORK_RID_                                = SECURITY_NETWORK_RID;
	constexpr auto SECURITY_BATCH_RID_                                  = SECURITY_BATCH_RID;
	constexpr auto SECURITY_INTERACTIVE_RID_                            = SECURITY_INTERACTIVE_RID;
	constexpr auto SECURITY_LOGON_IDS_RID_                              = SECURITY_LOGON_IDS_RID;
	constexpr auto SECURITY_LOGON_IDS_RID_COUNT_                        = SECURITY_LOGON_IDS_RID_COUNT;
	constexpr auto SECURITY_SERVICE_RID_                                = SECURITY_SERVICE_RID;
	constexpr auto SECURITY_ANONYMOUS_LOGON_RID_                        = SECURITY_ANONYMOUS_LOGON_RID;
	constexpr auto SECURITY_PROXY_RID_                                  = SECURITY_PROXY_RID;
	constexpr auto SECURITY_ENTERPRISE_CONTROLLERS_RID_                 = SECURITY_ENTERPRISE_CONTROLLERS_RID;
	constexpr auto SECURITY_SERVER_LOGON_RID_                           = SECURITY_SERVER_LOGON_RID;
	constexpr auto SECURITY_PRINCIPAL_SELF_RID_                         = SECURITY_PRINCIPAL_SELF_RID;
	constexpr auto SECURITY_AUTHENTICATED_USER_RID_                     = SECURITY_AUTHENTICATED_USER_RID;
	constexpr auto SECURITY_RESTRICTED_CODE_RID_                        = SECURITY_RESTRICTED_CODE_RID;
	constexpr auto SECURITY_TERMINAL_SERVER_RID_                        = SECURITY_TERMINAL_SERVER_RID;
	constexpr auto SECURITY_REMOTE_LOGON_RID_                           = SECURITY_REMOTE_LOGON_RID;
	constexpr auto SECURITY_THIS_ORGANIZATION_RID_                      = SECURITY_THIS_ORGANIZATION_RID;
	constexpr auto SECURITY_IUSER_RID_                                  = SECURITY_IUSER_RID;
	constexpr auto SECURITY_LOCAL_SYSTEM_RID_                           = SECURITY_LOCAL_SYSTEM_RID;
	constexpr auto SECURITY_LOCAL_SERVICE_RID_                          = SECURITY_LOCAL_SERVICE_RID;
	constexpr auto SECURITY_NETWORK_SERVICE_RID_                        = SECURITY_NETWORK_SERVICE_RID;
	constexpr auto SECURITY_NT_NON_UNIQUE_                              = SECURITY_NT_NON_UNIQUE;
	constexpr auto SECURITY_NT_NON_UNIQUE_SUB_AUTH_COUNT_               = SECURITY_NT_NON_UNIQUE_SUB_AUTH_COUNT;
	constexpr auto SECURITY_ENTERPRISE_READONLY_CONTROLLERS_RID_        = SECURITY_ENTERPRISE_READONLY_CONTROLLERS_RID;
	constexpr auto SECURITY_BUILTIN_DOMAIN_RID_                         = SECURITY_BUILTIN_DOMAIN_RID;
	constexpr auto SECURITY_WRITE_RESTRICTED_CODE_RID_                  = SECURITY_WRITE_RESTRICTED_CODE_RID;
	constexpr auto SECURITY_PACKAGE_BASE_RID_                           = SECURITY_PACKAGE_BASE_RID;
	constexpr auto SECURITY_PACKAGE_RID_COUNT_                          = SECURITY_PACKAGE_RID_COUNT;
	constexpr auto SECURITY_PACKAGE_NTLM_RID_                           = SECURITY_PACKAGE_NTLM_RID;
	constexpr auto SECURITY_PACKAGE_SCHANNEL_RID_                       = SECURITY_PACKAGE_SCHANNEL_RID;
	constexpr auto SECURITY_PACKAGE_DIGEST_RID_                         = SECURITY_PACKAGE_DIGEST_RID;
	constexpr auto SECURITY_CRED_TYPE_BASE_RID_                         = SECURITY_CRED_TYPE_BASE_RID;
	constexpr auto SECURITY_CRED_TYPE_RID_COUNT_                        = SECURITY_CRED_TYPE_RID_COUNT;
	constexpr auto SECURITY_CRED_TYPE_THIS_ORG_CERT_RID_                = SECURITY_CRED_TYPE_THIS_ORG_CERT_RID;
	constexpr auto SECURITY_MIN_BASE_RID_                               = SECURITY_MIN_BASE_RID;
	constexpr auto SECURITY_SERVICE_ID_BASE_RID_                        = SECURITY_SERVICE_ID_BASE_RID;
	constexpr auto SECURITY_SERVICE_ID_RID_COUNT_                       = SECURITY_SERVICE_ID_RID_COUNT;
	constexpr auto SECURITY_RESERVED_ID_BASE_RID_                       = SECURITY_RESERVED_ID_BASE_RID;
	constexpr auto SECURITY_APPPOOL_ID_BASE_RID_                        = SECURITY_APPPOOL_ID_BASE_RID;
	constexpr auto SECURITY_APPPOOL_ID_RID_COUNT_                       = SECURITY_APPPOOL_ID_RID_COUNT;
	constexpr auto SECURITY_VIRTUALSERVER_ID_BASE_RID_                  = SECURITY_VIRTUALSERVER_ID_BASE_RID;
	constexpr auto SECURITY_VIRTUALSERVER_ID_RID_COUNT_                 = SECURITY_VIRTUALSERVER_ID_RID_COUNT;
	constexpr auto SECURITY_USERMODEDRIVERHOST_ID_BASE_RID_             = SECURITY_USERMODEDRIVERHOST_ID_BASE_RID;
	constexpr auto SECURITY_USERMODEDRIVERHOST_ID_RID_COUNT_            = SECURITY_USERMODEDRIVERHOST_ID_RID_COUNT;
	constexpr auto SECURITY_CLOUD_INFRASTRUCTURE_SERVICES_ID_BASE_RID_  = SECURITY_CLOUD_INFRASTRUCTURE_SERVICES_ID_BASE_RID;
	constexpr auto SECURITY_CLOUD_INFRASTRUCTURE_SERVICES_ID_RID_COUNT_ = SECURITY_CLOUD_INFRASTRUCTURE_SERVICES_ID_RID_COUNT;
	constexpr auto SECURITY_WMIHOST_ID_BASE_RID_                        = SECURITY_WMIHOST_ID_BASE_RID;
	constexpr auto SECURITY_WMIHOST_ID_RID_COUNT_                       = SECURITY_WMIHOST_ID_RID_COUNT;
	constexpr auto SECURITY_TASK_ID_BASE_RID_                           = SECURITY_TASK_ID_BASE_RID;
	constexpr auto SECURITY_NFS_ID_BASE_RID_                            = SECURITY_NFS_ID_BASE_RID;
	constexpr auto SECURITY_COM_ID_BASE_RID_                            = SECURITY_COM_ID_BASE_RID;
	constexpr auto SECURITY_WINDOW_MANAGER_BASE_RID_                    = SECURITY_WINDOW_MANAGER_BASE_RID;
	constexpr auto SECURITY_RDV_GFX_BASE_RID_                           = SECURITY_RDV_GFX_BASE_RID;
	constexpr auto SECURITY_DASHOST_ID_BASE_RID_                        = SECURITY_DASHOST_ID_BASE_RID;
	constexpr auto SECURITY_DASHOST_ID_RID_COUNT_                       = SECURITY_DASHOST_ID_RID_COUNT;
	constexpr auto SECURITY_VIRTUALACCOUNT_ID_RID_COUNT_                = SECURITY_VIRTUALACCOUNT_ID_RID_COUNT;
	constexpr auto SECURITY_MAX_BASE_RID_                               = SECURITY_MAX_BASE_RID;
	constexpr auto SECURITY_MAX_ALWAYS_FILTERED_                        = SECURITY_MAX_ALWAYS_FILTERED;
	constexpr auto SECURITY_MIN_NEVER_FILTERED_                         = SECURITY_MIN_NEVER_FILTERED;
	constexpr auto SECURITY_OTHER_ORGANIZATION_RID_                     = SECURITY_OTHER_ORGANIZATION_RID;
}
