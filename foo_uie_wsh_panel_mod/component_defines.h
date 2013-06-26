#pragma once

/* 
 * IMPORTANT NOTE:
 * For modders: You should change these Defines/Guids below, in order to 
 *   co-exist with the original release of WSH Panel Mod.
 */

#define WSPM_NAME "WSH Panel SP"
#define WSPM_WINDOW_CLASS_NAME "uie_wsh_panel_sp_class"
#define WSPM_DLL_NAME "foo_uie_wsh_panel_sp.dll"

// {FE5F2F8D-42C5-43D4-9BD6-3C7375A25B66}
extern const GUID g_ui_pref_window_guid;
FOOGUIDDECL const GUID g_ui_pref_window_guid = 
{ 0xfe5f2f8d, 0x42c5, 0x43d4, { 0x9b, 0xd6, 0x3c, 0x73, 0x75, 0xa2, 0x5b, 0x66 } };

// {3D3E15E4-4347-41FD-A3F4-CE2BC9F7CA2C}
extern const GUID g_wsh_panel_window_extension_guid;
FOOGUIDDECL const GUID g_wsh_panel_window_extension_guid =
{ 0x3d3e15e4, 0x4347, 0x41fd, { 0xa3, 0xf4, 0xce, 0x2b, 0xc9, 0xf7, 0xca, 0x2c } };

// {AD68DBA0-DBE5-4675-9800-49CEE195DC84}
extern const GUID g_wsh_panel_window_dui_guid;
FOOGUIDDECL const GUID g_wsh_panel_window_dui_guid = 
{ 0xad68dba0, 0xdbe5, 0x4675, { 0x98, 0x0, 0x49, 0xce, 0xe1, 0x95, 0xdc, 0x84 } };

// {0CEF50C1-E86C-42FA-9CD0-BF61F6291FA3}
extern const GUID g_guid_prop_sets;
FOOGUIDDECL const GUID g_guid_prop_sets = 
{ 0xcef50c1, 0xe86c, 0x42fa, { 0x9c, 0xd0, 0xbf, 0x61, 0xf6, 0x29, 0x1f, 0xa3 } };

// {6C5EAC8A-D7BD-40E5-8367-584990B7F439}
extern const GUID g_guid_cfg_safe_mode;
FOOGUIDDECL const GUID g_guid_cfg_safe_mode = 
{ 0x6c5eac8a, 0xd7bd, 0x40e5, { 0x83, 0x67, 0x58, 0x49, 0x90, 0xb7, 0xf4, 0x39 } };

// {B3852974-928B-453F-97D9-0D5FC74C5E48}
extern const GUID g_guid_cfg_cui_warning_reported;
FOOGUIDDECL const GUID g_guid_cfg_cui_warning_reported = 
{ 0xb3852974, 0x928b, 0x453f, { 0x97, 0xd9, 0xd, 0x5f, 0xc7, 0x4c, 0x5e, 0x48 } };
