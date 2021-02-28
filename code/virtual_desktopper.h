#if !defined(VIRTUAL_DESKTOPPER_H)
#define VIRTUAL_DESKTOPPER_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef bool b32;

#define global_variable static
#define internal static

#include <inspectable.h> // IInspectable, HSTRING, TrustLevel
#include <shobjidl.h> // IObjectArray, ObjectArray, IVirtualDesktopManager, VirtualDesktopManager
#include <strsafe.h> // StringCbPrintf
#include <windows.h>

#define VIRTUAL_DESKTOP_NUMERAL_TIMEOUT 750

enum AdjacentDesktop
{
    LeftDirection = 3,
    RightDirection = 4
};

#define IAsyncCallback UINT
#define IImmersiveMonitor UINT
#define APPLICATION_VIEW_COMPATIBILITY_POLICY UINT
#define IApplicationViewOperation UINT
#define APPLICATION_VIEW_CLOAK_TYPE UINT
#define IApplicationViewPosition UINT

CLSID const CLSID_ImmersiveShell = {0xC2F03A33, 0x21F5, 0x47FA, 0xB4, 0xBB, 0x15, 0x63, 0x62, 0xA2, 0xF2, 0x39};
CLSID const CLSID_IVirtualNotificationService = {0xA501FDEC, 0x4A09, 0x464C, 0xAE, 0x4E, 0x1B, 0x9C, 0x21, 0xB8, 0x49, 0x18};
CLSID const CLSID_VirtualDesktopAPI_Unknown = {0xC5E0CDCA, 0x7B6E, 0x41B2, 0x9F, 0xC4, 0xD9, 0x39, 0x75, 0xCC, 0x46, 0x7B};
CLSID const CLSID_VirtualDesktopPinnedApps = {0xb5a399e7, 0x1c87, 0x46b8, 0x88, 0xe9, 0xfc, 0x57, 0x47, 0xb1, 0x71, 0xbd};

IID const IID_IApplicationViewCollection  = {0x1841C6D7, 0x4F9D, 0x42C0, {0xAF, 0x41, 0x87, 0x47, 0x53, 0x8F, 0x10, 0xE5}};
IID const IID_IVirtualDesktopNotification = {0xC179334C, 0x4295, 0x40D3, {0xBE, 0xA1, 0xC6, 0x54, 0xD9, 0x65, 0x60, 0x5A}};

struct __declspec(novtable uuid("372E1D3B-38D3-42E4-A15B-8AB2B178F513"))
IApplicationView : public IInspectable
{
    // IUnknown methods
    virtual HRESULT __stdcall QueryInterface(const IID &InterfaceID, LPVOID *Object) = 0;
    virtual ULONG __stdcall AddRef(void) = 0;
    virtual ULONG __stdcall Release(void) = 0;

    // IInspectable methods
    virtual HRESULT __stdcall GetIids(ULONG *Count, IID **InterfaceIDList) = 0;
    virtual HRESULT __stdcall GetRuntimeClassName(HSTRING *ClassName) = 0;
    virtual HRESULT __stdcall GetTrustLevel(TrustLevel *Level) = 0;

    // IApplicationView methods
    virtual HRESULT __stdcall SetFocus(void) = 0;
    virtual HRESULT __stdcall SwitchTo(void) = 0;
    virtual HRESULT __stdcall TryInvokeBack(IAsyncCallback *) = 0; // Proc8
    virtual HRESULT __stdcall GetThumbnailWindow(HWND *) = 0; // Proc9
    virtual HRESULT __stdcall GetMonitor(IImmersiveMonitor **) = 0; // Proc10
    virtual HRESULT __stdcall GetVisibility(int *) = 0; // Proc11
    virtual HRESULT __stdcall SetCloak(APPLICATION_VIEW_CLOAK_TYPE, int) = 0; // Proc12
    virtual HRESULT __stdcall GetPosition(REFIID, void **) = 0; // Proc13
    virtual HRESULT __stdcall SetPosition(IApplicationViewPosition *) = 0; // Proc14
    virtual HRESULT __stdcall InsertAfterWindow(HWND) = 0; // Proc15
    virtual HRESULT __stdcall GetExtendedFramePosition(RECT *) = 0; // Proc16
    virtual HRESULT __stdcall GetAppUserModelId(PWSTR *) = 0; // Proc17
    virtual HRESULT __stdcall SetAppUserModelId(PCWSTR) = 0; // Proc18
    virtual HRESULT __stdcall IsEqualByAppUserModelId(PCWSTR, int *) = 0; // Proc19
    virtual HRESULT __stdcall GetViewState(UINT *) = 0; // Proc20
    virtual HRESULT __stdcall SetViewState(UINT) = 0; // Proc21
    virtual HRESULT __stdcall GetNeediness(int *) = 0; // Proc22
    virtual HRESULT __stdcall GetLastActivationTimestamp(ULONGLONG *) = 0;
    virtual HRESULT __stdcall SetLastActivationTimestamp(ULONGLONG) = 0;
    virtual HRESULT __stdcall GetVirtualDesktopId(GUID *) = 0;
    virtual HRESULT __stdcall SetVirtualDesktopId(REFGUID) = 0;
    virtual HRESULT __stdcall GetShowInSwitchers(int *) = 0;
    virtual HRESULT __stdcall SetShowInSwitchers(int) = 0;
    virtual HRESULT __stdcall GetScaleFactor(int *) = 0;
    virtual HRESULT __stdcall CanReceiveInput(BOOL *) = 0;
    virtual HRESULT __stdcall GetCompatibilityPolicyType(APPLICATION_VIEW_COMPATIBILITY_POLICY *) = 0;
    virtual HRESULT __stdcall SetCompatibilityPolicyType(APPLICATION_VIEW_COMPATIBILITY_POLICY) = 0;
    //virtual HRESULT __stdcall GetPositionPriority(IShellPositionerPriority**) = 0; // removed in 1803
    //virtual HRESULT __stdcall SetPositionPriority(IShellPositionerPriority*) = 0; // removed in 1803
    virtual HRESULT __stdcall GetSizeConstraints(IImmersiveMonitor *, SIZE *, SIZE *) = 0;
    virtual HRESULT __stdcall GetSizeConstraintsForDpi(UINT, SIZE *, SIZE *) = 0;
    virtual HRESULT __stdcall SetSizeConstraintsForDpi(const UINT *, const SIZE *, const SIZE *) = 0;
    //virtual HRESULT __stdcall QuerySizeConstraintsFromApp(void) = 0; // removed in 1803
    virtual HRESULT __stdcall OnMinSizePreferencesUpdated(HWND) = 0;
    virtual HRESULT __stdcall ApplyOperation(IApplicationViewOperation*) = 0;
    virtual HRESULT __stdcall IsTray(BOOL *) = 0;
    virtual HRESULT __stdcall IsInHighZOrderBand(BOOL *) = 0;
    virtual HRESULT __stdcall IsSplashScreenPresented(BOOL *) = 0;
    virtual HRESULT __stdcall Flash(void) = 0;
    virtual HRESULT __stdcall GetRootSwitchableOwner(IApplicationView **) = 0; // proc45
    virtual HRESULT __stdcall EnumerateOwnershipTree(IObjectArray **) = 0; // proc46

    virtual HRESULT __stdcall GetEnterpriseId(PWSTR *) = 0; // proc47
    virtual HRESULT __stdcall IsMirrored(BOOL *) = 0; //

    virtual HRESULT __stdcall Unknown1(int *) = 0;
    virtual HRESULT __stdcall Unknown2(int *) = 0;
    virtual HRESULT __stdcall Unknown3(int *) = 0;
    virtual HRESULT __stdcall Unknown4(int) = 0;
    virtual HRESULT __stdcall Unknown5(int *) = 0;
    virtual HRESULT __stdcall Unknown6(int) = 0;
    virtual HRESULT __stdcall Unknown7(void) = 0;
    virtual HRESULT __stdcall Unknown8(int *) = 0;
    virtual HRESULT __stdcall Unknown9(int) = 0;
    virtual HRESULT __stdcall Unknown10(int, int) = 0;
    virtual HRESULT __stdcall Unknown11(int) = 0;
    virtual HRESULT __stdcall Unknown12(SIZE *) = 0;
};

struct __declspec(novtable uuid("FF72FFDD-BE7E-43FC-9C03-AD81681E88E4"))
IVirtualDesktop : public IUnknown
{
    virtual HRESULT __stdcall IsViewVisible(IApplicationView *View, int *Visible) = 0;
    virtual HRESULT __stdcall GetID(GUID *ID) = 0;
};

struct __declspec(novtable uuid("C179334C-4295-40D3-BEA1-C654D965605A"))
IVirtualDesktopNotification : public IUnknown
{
    virtual HRESULT __stdcall VirtualDesktopCreated(IVirtualDesktop *Desktop) = 0;
    virtual HRESULT __stdcall VirtualDesktopDestroyBegin(IVirtualDesktop *DesktopDestroyed, IVirtualDesktop *DesktopFallback) = 0;
    virtual HRESULT __stdcall VirtualDesktopDestroyFailed(IVirtualDesktop *DesktopDestroyed, IVirtualDesktop *DesktopFallback) = 0;
    virtual HRESULT __stdcall VirtualDesktopDestroyed(IVirtualDesktop *DesktopDestroyed, IVirtualDesktop *DesktopFallback) = 0;
    virtual HRESULT __stdcall ViewVirtualDesktopChanged(IApplicationView *View) = 0;
    virtual HRESULT __stdcall CurrentVirtualDesktopChanged(IVirtualDesktop *OldDesktop, IVirtualDesktop *NewDesktop) = 0;
};

struct __declspec(novtable uuid("0CD45E71-D927-4F15-8B0A-8FEF525337BF"))
IVirtualDesktopNotificationService : public IUnknown
{
    virtual HRESULT __stdcall Register(IVirtualDesktopNotification *Notification, DWORD *Cookie) = 0;
    virtual HRESULT __stdcall Unregister(DWORD Cookie) = 0;
};

struct __declspec(novtable uuid("4ce81583-1e4c-4632-a621-07a53543148f"))
IVirtualDesktopPinnedApps : public IUnknown
{
    // IUnknown methods
    virtual HRESULT __stdcall QueryInterface(REFIID InterfaceID, LPVOID *Object) = 0;
    virtual ULONG __stdcall AddRef(void) = 0;
    virtual ULONG __stdcall Release(void) = 0;

    // IVirtualDesktopPinnedApps methods
    virtual HRESULT __stdcall IsAppIdPinned(PCWSTR AppID, BOOL *) = 0;
    virtual HRESULT __stdcall PinAppID(PCWSTR AppID) = 0;
    virtual HRESULT __stdcall UnpinAppID(PCWSTR AppID) = 0;
    virtual HRESULT __stdcall IsViewPinned(IApplicationView *, BOOL *) = 0;
    virtual HRESULT __stdcall PinView(IApplicationView *) = 0;
    virtual HRESULT __stdcall UnpinView(IApplicationView *) = 0;
};

struct __declspec(novtable uuid("1841C6D7-4F9D-42C0-AF41-8747538F10E5"))
IApplicationViewCollection : public IUnknown
{
    virtual HRESULT __stdcall GetViews(IObjectArray **Array) = 0;
    virtual HRESULT __stdcall GetViewsByZOrder(IObjectArray **Array) = 0;
    virtual HRESULT __stdcall GetViewsByAppUserModelId(BSTR pId, IObjectArray **Array) = 0;
    virtual HRESULT __stdcall GetViewForHwnd(HWND pHwnd, IApplicationView **View) = 0;
    virtual HRESULT __stdcall GetViewForApplication(void *pApplication, IApplicationView **View) = 0;
    virtual HRESULT __stdcall GetViewForAppUserModelId(BSTR ID, IApplicationView **View) = 0;
    virtual HRESULT __stdcall GetViewInFocus(int *View) = 0;
    virtual HRESULT __stdcall RefreshCollection() = 0;
    virtual HRESULT __stdcall RegisterForApplicationViewChanges(void *Listener, int *Cookie) = 0;
    virtual HRESULT __stdcall RegisterForApplicationViewPositionChanges(void *pListener, int *Cookie) = 0;
    virtual HRESULT __stdcall UnregisterForApplicationViewChanges(int Cookie) = 0;
};

struct __declspec(novtable uuid("f31574d6-b682-4cdc-bd56-1827860abec6"))
IVirtualDesktopManagerInternal : public IUnknown
{
    virtual HRESULT __stdcall GetCount(UINT *Count) = 0;
    virtual HRESULT __stdcall MoveViewToDesktop(IApplicationView *View, IVirtualDesktop *Desktop) = 0;
    virtual HRESULT __stdcall CanViewMoveDesktops(IApplicationView *View, int *CanViewMoveDesktops) = 0;
    virtual HRESULT __stdcall GetCurrentDesktop(IVirtualDesktop **Desktop) = 0;
    virtual HRESULT __stdcall GetDesktops(IObjectArray **Desktops) = 0;
    virtual HRESULT __stdcall GetAdjacentDesktop(IVirtualDesktop *DesktopReference, AdjacentDesktop Direction, IVirtualDesktop **AdjacentDesktop) = 0;
    virtual HRESULT __stdcall SwitchDesktop(IVirtualDesktop *Desktop) = 0;
    virtual HRESULT __stdcall CreateDesktopW(IVirtualDesktop **NewDesktop) = 0;
    virtual HRESULT __stdcall RemoveDesktop(IVirtualDesktop *Remove, IVirtualDesktop *pFallbackDesktop) = 0;
    virtual HRESULT __stdcall FindDesktop(GUID *DesktopID, IVirtualDesktop **Desktop) = 0;
};

#endif // VIRTUAL_DESKTOPPER_H