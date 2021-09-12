#if !defined(VIRTUAL_DESKTOPPER_H)

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int b32;

#define global_variable static
#define internal static

#define COBJMACROS
#define WIN32_LEAN_AND_MEAN
#include <initguid.h>
#include <windows.h>
#include <inspectable.h> // IInspectable, HSTRING, TrustLevel
#include <shobjidl.h> // IObjectArray, ObjectArray, IVirtualDesktopManager, VirtualDesktopManager
#include <strsafe.h> // StringCbPrintf

#define VIRTUAL_DESKTOP_NUMERAL_TIMEOUT 750

enum AdjacentDesktop
{
    LeftDirection = 3,
    RightDirection = 4
};
typedef enum AdjacentDesktop AdjacentDesktop;

const GUID GUID_ImmersiveShell = {0xC2F03A33, 0x21F5, 0x47FA, 0xB4, 0xBB, 0x15, 0x63, 0x62, 0xA2, 0xF2, 0x39};
const GUID GUID_IVirtualNotificationService = {0xA501FDEC, 0x4A09, 0x464C, 0xAE, 0x4E, 0x1B, 0x9C, 0x21, 0xB8, 0x49, 0x18};
const GUID GUID_VirtualDesktopAPI_Unknown = {0xC5E0CDCA, 0x7B6E, 0x41B2, 0x9F, 0xC4, 0xD9, 0x39, 0x75, 0xCC, 0x46, 0x7B};
const GUID GUID_VirtualDesktopPinnedApps = {0xb5a399e7, 0x1c87, 0x46b8, 0x88, 0xe9, 0xfc, 0x57, 0x47, 0xb1, 0x71, 0xbd};
const GUID GUID_IApplicationViewCollection  = {0x1841C6D7, 0x4F9D, 0x42C0, 0xAF, 0x41, 0x87, 0x47, 0x53, 0x8F, 0x10, 0xE5};
const GUID GUID_IVirtualDesktopNotification = {0xC179334C, 0x4295, 0x40D3, 0xBE, 0xA1, 0xC6, 0x54, 0xD9, 0x65, 0x60, 0x5A};

//struct __declspec(novtable uuid("372E1D3B-38D3-42E4-A15B-8AB2B178F513"))
const GUID GUID_IApplicationView = {0x372E1D3B, 0x38D3, 0x42E4, 0xA1, 0x5B, 0x8a, 0xb2, 0xb1, 0x78, 0xf5, 0x13};
#undef INTERFACE
#define INTERFACE IApplicationView
DECLARE_INTERFACE_(IApplicationView, IInspectable)
{
    // IUnknown
    STDMETHOD(QueryInterface) (THIS_ REFIID, LPVOID *Object) PURE;
    STDMETHOD(AddRef) (THIS) PURE; // TODO(chuck): STDMETHOD() is for HRESULT... but AddRef/Release return ULONG.
    STDMETHOD(Release) (THIS) PURE;

    // IInspectable
    STDMETHOD(GetIids) (THIS_ ULONG *Count, GUID **InterfaceIDList) PURE;
    STDMETHOD(GetRuntimeClassName) (THIS_ HSTRING *ClassName) PURE;
    STDMETHOD(GetTrustLevel) (THIS_ TrustLevel *Level) PURE;

    // IApplicationView
    STDMETHOD(SetFocus) (THIS) PURE;
    STDMETHOD(SwitchTo) (THIS) PURE;
    STDMETHOD(TryInvokeBack) (THIS_ UINT *) PURE;
    STDMETHOD(GetThumbnailWindow) (THIS_ HWND *) PURE;
    STDMETHOD(GetMonitor) (THIS_ UINT **) PURE;
    STDMETHOD(GetVisibility) (THIS_ int *) PURE;
    STDMETHOD(SetCloak) (THIS_ UINT, int) PURE;
    STDMETHOD(GetPosition) (THIS_ REFGUID, void **) PURE;
    STDMETHOD(SetPosition) (THIS_ UINT *) PURE;
    STDMETHOD(InsertAfterWindow) (THIS_ HWND) PURE;
    STDMETHOD(GetExtendedFramePosition) (THIS_ RECT *) PURE;
    STDMETHOD(GetAppUserModelId) (THIS_ PWSTR *) PURE;
    STDMETHOD(SetAppUserModelId) (THIS_ PCWSTR) PURE;
    STDMETHOD(IsEqualByAppUserModelId) (THIS_ PCWSTR, int *) PURE;
    STDMETHOD(GetViewState) (THIS_ PCWSTR, UINT *) PURE;
    STDMETHOD(SetViewState) (THIS_ UINT) PURE;
    STDMETHOD(GetNeediness) (THIS_ int *) PURE;
    STDMETHOD(GetLastActivationTimestamp) (THIS_ ULONGLONG *) PURE;
    STDMETHOD(SetLastActivationTimestamp) (THIS_ ULONGLONG) PURE;
    STDMETHOD(GetVirtualDesktopId) (THIS_ GUID *) PURE;
    STDMETHOD(SetVirtualDesktopId) (THIS_ REFGUID) PURE;
    STDMETHOD(GetShowInSwitchers) (THIS_ int *) PURE;
    STDMETHOD(SetShowInSwitchers) (THIS_ int) PURE;
    STDMETHOD(GetScaleFactor) (THIS_ int *) PURE;
    STDMETHOD(SetScaleFactor) (THIS_ int) PURE;
    STDMETHOD(CanReceiveInput) (THIS_ BOOL *) PURE;
    STDMETHOD(GetCompatibilityPolicyType) (THIS_ UINT *) PURE;
    STDMETHOD(SetCompatibilityPolicyType) (THIS_ UINT) PURE;

    //virtual HRESULT __stdcall GetPositionPriority(IShellPositionerPriority**) = 0; // removed in 1803
    //virtual HRESULT __stdcall SetPositionPriority(IShellPositionerPriority*) = 0; // removed in 1803
    
    STDMETHOD(GetSizeConstraints) (THIS_ UINT *, SIZE *, SIZE *) PURE;
    STDMETHOD(GetSizeConstraintsForDpi) (THIS_ UINT, SIZE *, SIZE *) PURE;
    STDMETHOD(SetSizeConstraintsForDpi) (THIS_ const UINT *, const SIZE *, const SIZE *) PURE; // TODO(chuck): Nuke const?
    
    //virtual HRESULT __stdcall QuerySizeConstraintsFromApp(void) = 0; // removed in 1803
    
    STDMETHOD(OnMinSizePreferencesUpdated) (THIS_ HWND) PURE;
    STDMETHOD(ApplyOperation) (THIS_ UINT *) PURE;
    STDMETHOD(IsTray) (THIS_ BOOL *) PURE;
    STDMETHOD(IsInHighZOrderBand) (THIS_ BOOL *) PURE;
    STDMETHOD(IsSplashScreenPresented) (THIS_ BOOL *) PURE;
    STDMETHOD(Flash) (THIS) PURE;
    STDMETHOD(GetRootSwitchableOwner) (THIS_ IApplicationView **) PURE;
    STDMETHOD(EnumerateOwnershipTree) (THIS_ IObjectArray **) PURE;
    
    STDMETHOD(GetEnterpriseId) (THIS_ PWSTR *) PURE;
    STDMETHOD(IsMirrored) (THIS_ BOOL *) PURE;
    
    STDMETHOD(Unknown1) (THIS_ int *) PURE;
    STDMETHOD(Unknown2) (THIS_ int *) PURE;
    STDMETHOD(Unknown3) (THIS_ int *) PURE;
    STDMETHOD(Unknown4) (THIS_ int) PURE;
    STDMETHOD(Unknown5) (THIS_ int *) PURE;
    STDMETHOD(Unknown6) (THIS_ int) PURE;
    STDMETHOD(Unknown7) (THIS) PURE;
    STDMETHOD(Unknown8) (THIS_ int *) PURE;
    STDMETHOD(Unknown9) (THIS_ int) PURE;
    STDMETHOD(Unknown10) (THIS_ int, int) PURE;
    STDMETHOD(Unknown11) (THIS_ int) PURE;
    STDMETHOD(Unknown12) (THIS_ SIZE *) PURE;
};

// struct __declspec(novtable uuid("FF72FFDD-BE7E-43FC-9C03-AD81681E88E4"))
const GUID GUID_IVirtualDesktop = {0xFF72FFDD, 0xBE7E, 0x43FC, 0x9c, 0x03, 0xad, 0x81, 0x68, 0x1e, 0x88, 0xe4};
#undef INTERFACE
#define INTERFACE IVirtualDesktop
DECLARE_INTERFACE_(IVirtualDesktop, IUknown)
{
    // IUnknown
    STDMETHOD(QueryInterface) (THIS_ REFIID, LPVOID *Object) PURE;
    STDMETHOD(AddRef) (THIS) PURE; // TODO(chuck): STDMETHOD() is for HRESULT... but AddRef/Release return ULONG.
    STDMETHOD(Release) (THIS) PURE;

    STDMETHOD(IsViewVisible) (THIS_ IApplicationView *View, int *Visible) PURE;
    STDMETHOD(GetID) (THIS_ GUID *ID) PURE;
};

typedef struct virtual_desktop_notification_vtable virtual_desktop_notification_vtable;
typedef struct
{
    virtual_desktop_notification_vtable *VTable;
    volatile LONG Count;
} virtual_desktop_notification;

// struct __declspec(novtable uuid("C179334C-4295-40D3-BEA1-C654D965605A"))
#undef INTERFACE
#define INTERFACE IVirtualDesktopNotification
DECLARE_INTERFACE_(IVirtualDesktopNotification, IUnknown)
{
    // IUnknown
    STDMETHOD(QueryInterface) (THIS_ REFIID, LPVOID *Object) PURE;
    STDMETHOD(AddRef) (THIS) PURE;
    STDMETHOD(Release) (THIS) PURE;

    STDMETHOD(VirtualDesktopCreated) (THIS_ IVirtualDesktop *Desktop) PURE;
    STDMETHOD(VirtualDesktopDestroyBegin) (THIS_ IVirtualDesktop *DesktopDestroyed, IVirtualDesktop *DesktopFallback) PURE;
    STDMETHOD(VirtualDesktopDestroyedFailed) (THIS_ IVirtualDesktop *DesktopDestroyed, IVirtualDesktop *DesktopFallback) PURE;
    STDMETHOD(VirtualDesktopDestroyed) (THIS_ IVirtualDesktop *DesktopDestroyed, IVirtualDesktop *DesktopFallback) PURE;
    STDMETHOD(ViewVirtualDesktopChanged) (THIS_ IApplicationView *View) PURE;
    STDMETHOD(CurrentVirtualDesktopChanged) (THIS_ IVirtualDesktop *OldDesktop, IVirtualDesktop *NewDesktop) PURE;
};

// struct __declspec(novtable uuid("0CD45E71-D927-4F15-8B0A-8FEF525337BF"))
const GUID GUID_IVirtualDesktopNotificationService = {0x0CD45E71, 0xD927, 0x4F15, 0x8B, 0x0A, 0x8F, 0xEF, 0x52, 0x53, 0x37, 0xBF};
#undef INTERFACE
#define INTERFACE IVirtualDesktopNotificationService
DECLARE_INTERFACE_(IVirtualDesktopNotificationService, IUnknown)
{
    // IUnknown
    STDMETHOD(QueryInterface) (THIS_ REFIID, LPVOID *Object) PURE;
    STDMETHOD(AddRef) (THIS) PURE;
    STDMETHOD(Release) (THIS) PURE;

    STDMETHOD(Register) (THIS_ virtual_desktop_notification *Notification, DWORD *Cookie) PURE;
    STDMETHOD(Unregister) (THIS_ DWORD Cookie) PURE;
};

// struct __declspec(novtable uuid("4ce81583-1e4c-4632-a621-07a53543148f"))
const GUID GUID_IVirtualDesktopPinnedApps = {0x4ce81583, 0x1e4c, 0x4632, 0xa6, 0x21, 0x07, 0xa5, 0x35, 0x43, 0x14, 0x8f};
#undef INTERFACE
#define INTERFACE IVirtualDesktopPinnedApps
DECLARE_INTERFACE_(IVirtualDesktopPinnedApps, IUnknown)
{
    // IUnknown
    STDMETHOD(QueryInterface) (THIS_ REFIID, LPVOID *Object) PURE;
    STDMETHOD(AddRef) (THIS) PURE;
    STDMETHOD(Release) (THIS) PURE;

    // IVirtualDesktopPinnedApps
    STDMETHOD(IsAppIdPinned) (THIS_ PCWSTR AppID, BOOL *) PURE;
    STDMETHOD(PinAppID) (THIS_ PCWSTR AppID) PURE;
    STDMETHOD(UnpinAppID) (THIS_ PCWSTR AppID) PURE;
    STDMETHOD(IsViewPinned) (THIS_ IApplicationView *, BOOL *) PURE;
    STDMETHOD(PinView) (THIS_ IApplicationView *) PURE;
    STDMETHOD(UnpinView) (THIS_ IApplicationView *) PURE;
};

// struct __declspec(novtable uuid("1841C6D7-4F9D-42C0-AF41-8747538F10E5"))
#undef INTERFACE
#define INTERFACE IApplicationViewCollection
DECLARE_INTERFACE_(IApplicationViewCollection, IUnknown)
{
    // IUnknown
    STDMETHOD(QueryInterface) (THIS_ REFIID, LPVOID *Object) PURE;
    STDMETHOD(AddRef) (THIS) PURE;
    STDMETHOD(Release) (THIS) PURE;

    STDMETHOD(GetViews) (THIS_ IObjectArray **Array) PURE;
    STDMETHOD(GetViewsByZOrder) (THIS_ IObjectArray **Array) PURE;
    STDMETHOD(GetViewsByAppUserModelId) (THIS_ BSTR pId, IObjectArray **Array) PURE;
    STDMETHOD(GetViewForHwnd) (THIS_ HWND pHwnd, IApplicationView **View) PURE;
    STDMETHOD(GetViewForApplication) (THIS_ void *pApplication, IApplicationView **View) PURE;
    STDMETHOD(GetViewForAppUserModelId) (THIS_ BSTR ID, IApplicationView **View) PURE;
    STDMETHOD(GetViewInFocus) (THIS_ int *View) PURE;
    STDMETHOD(RefreshCollection) (THIS) PURE;
    STDMETHOD(RegisterForApplicationViewChanges) (THIS_ void *Listener, int *Cookie) PURE;
    STDMETHOD(RegisterForApplicationViewPositionChanges) (THIS_ void *pListener, int *Cookie) PURE;
    STDMETHOD(UnregisterForApplicationViewChanges) (THIS_ int Cookie) PURE;
};

// struct __declspec(novtable uuid("f31574d6-b682-4cdc-bd56-1827860abec6"))
// const GUID GUID_IVirtualDesktopManagerInternal = {0xf31574d6, 0xb682, 0x4cdc, 0xbd, 0x56, 0x18, 0x27, 0x86, 0x0a, 0xbe, 0xc6};
//{ 0xEF9F1A6C, 0xD3CC, 0x4358, 0xB7, 0x12, 0xF8, 0x4B, 0x63, 0x5B, 0xEB, 0xE7 };
const GUID GUID_IVirtualDesktopManagerInternal = {0xF31574D6, 0xB682, 0x4CDC, 0xBD, 0x56, 0x18, 0x27, 0x86, 0x0A, 0xBE, 0xC6};
#undef INTERFACE
#define INTERFACE IVirtualDesktopManagerInternal
DECLARE_INTERFACE_(IVirtualDesktopManagerInternal, IUnknown)
{
    // IUnknown
    STDMETHOD(QueryInterface) (THIS_ REFIID, LPVOID *Object) PURE;
    STDMETHOD(AddRef) (THIS) PURE;
    STDMETHOD(Release) (THIS) PURE;

    STDMETHOD(GetCount) (THIS_ UINT *Count) PURE;
    STDMETHOD(MoveViewToDesktop) (THIS_ IApplicationView *View, IVirtualDesktop *Desktop) PURE;
    STDMETHOD(CanViewMoveDesktop) (THIS_ IApplicationView *View, int *CanViewMoveDesktops) PURE;
    STDMETHOD(GetCurrentDesktop) (THIS_ IVirtualDesktop **Desktop) PURE;
    STDMETHOD(GetDesktops) (THIS_ IObjectArray **Desktops) PURE;
    STDMETHOD(GetAdjacentDesktop) (THIS_ IVirtualDesktop *DesktopReference, enum AdjacentDesktop Direction, IVirtualDesktop **AdjacentDesktop) PURE;
    STDMETHOD(SwitchDesktop) (THIS_ IVirtualDesktop *Desktop) PURE;
    STDMETHOD(CreateDesktop) (THIS_ IVirtualDesktop **NewDesktop) PURE;
    STDMETHOD(RemoveDesktop) (THIS_ IVirtualDesktop *Remove, IVirtualDesktop *pFallbackDesktop) PURE;
    STDMETHOD(FindDesktop) (THIS_ GUID *DesktopID, IVirtualDesktop **Desktop) PURE;
};
#undef INTERFACE

#define VIRTUAL_DESKTOPPER_H
#endif