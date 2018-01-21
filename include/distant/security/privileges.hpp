#pragma once

#include <distant\security\privilege.hpp>

namespace distant::security::privileges {

	////////////////////////////////////////////////////////////////////////
	//                                                                    //
	//               NT Defined Privileges                                //
	//                                                                    //
	////////////////////////////////////////////////////////////////////////
	constexpr security::privilege create_token			{ boost::winapi::SE_CREATE_TOKEN_NAME_ };
	constexpr security::privilege assign_primary_token	{ boost::winapi::SE_ASSIGNPRIMARYTOKEN_NAME_ };
	constexpr security::privilege lock_memory			{ boost::winapi::SE_LOCK_MEMORY_NAME_ };
	constexpr security::privilege increase_quota		{ boost::winapi::SE_INCREASE_QUOTA_NAME_ };
	constexpr security::privilege unsolicited_input		{ boost::winapi::SE_UNSOLICITED_INPUT_NAME_ };
	constexpr security::privilege machine_account		{ boost::winapi::SE_MACHINE_ACCOUNT_NAME_ };
	constexpr security::privilege tcb					{ boost::winapi::SE_TCB_NAME_ };
	constexpr security::privilege security				{ boost::winapi::SE_SECURITY_NAME_ };
	constexpr security::privilege take_ownership		{ boost::winapi::SE_TAKE_OWNERSHIP_NAME_ };
	constexpr security::privilege load_driver			{ boost::winapi::SE_LOAD_DRIVER_NAME_ };
	constexpr security::privilege system_profile		{ boost::winapi::SE_SYSTEM_PROFILE_NAME_ };
	constexpr security::privilege system_time			{ boost::winapi::SE_SYSTEMTIME_NAME_ };
	constexpr security::privilege prof_single_process	{ boost::winapi::SE_PROF_SINGLE_PROCESS_NAME_ };
	constexpr security::privilege increase_base_priority{ boost::winapi::SE_INC_BASE_PRIORITY_NAME_ };
	constexpr security::privilege create_pagefile		{ boost::winapi::SE_CREATE_PAGEFILE_NAME_ };
	constexpr security::privilege create_permanent		{ boost::winapi::SE_CREATE_PERMANENT_NAME_ };
	constexpr security::privilege backup				{ boost::winapi::SE_BACKUP_NAME_ };
	constexpr security::privilege restore				{ boost::winapi::SE_RESTORE_NAME_ };
	constexpr security::privilege shutdown				{ boost::winapi::SE_SHUTDOWN_NAME_ };
	constexpr security::privilege debug					{ boost::winapi::SE_DEBUG_NAME_ };
	constexpr security::privilege audit					{ boost::winapi::SE_AUDIT_NAME_ };
	constexpr security::privilege system_environment	{ boost::winapi::SE_SYSTEM_ENVIRONMENT_NAME_ };
	constexpr security::privilege change_notify			{ boost::winapi::SE_CHANGE_NOTIFY_NAME_ };
	constexpr security::privilege remote_shutdown		{ boost::winapi::SE_REMOTE_SHUTDOWN_NAME_ };
	constexpr security::privilege undock				{ boost::winapi::SE_UNDOCK_NAME_ };
	constexpr security::privilege sync_agent			{ boost::winapi::SE_SYNC_AGENT_NAME_ };
	constexpr security::privilege enable_delegation		{ boost::winapi::SE_ENABLE_DELEGATION_NAME_ };
	constexpr security::privilege manage_volume			{ boost::winapi::SE_MANAGE_VOLUME_NAME_ };
	constexpr security::privilege impersonate			{ boost::winapi::SE_IMPERSONATE_NAME_ };
	constexpr security::privilege create_global			{ boost::winapi::SE_CREATE_GLOBAL_NAME_ };
	constexpr security::privilege trusted_credman_access{ boost::winapi::SE_TRUSTED_CREDMAN_ACCESS_NAME_ };
	constexpr security::privilege relabel				{ boost::winapi::SE_RELABEL_NAME_ };
	constexpr security::privilege increase_working_set	{ boost::winapi::SE_INC_WORKING_SET_NAME_ };
	constexpr security::privilege time_zone				{ boost::winapi::SE_TIME_ZONE_NAME_ };
	constexpr security::privilege create_symbolic_link	{ boost::winapi::SE_CREATE_SYMBOLIC_LINK_NAME_ };

}

/*
*/