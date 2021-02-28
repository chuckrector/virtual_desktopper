#if !defined(VIRTUAL_DESKTOPPER_H)
#define VIRTUAL_DESKTOPPER_H

#include <shobjidl.h>
#include <unknwn.h>
#include <inspectable.h>

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

DECLARE_INTERFACE_IID_(IApplicationView, IInspectable, "372E1D3B-38D3-42E4-A15B-8AB2B178F513")
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObject) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /*** IInspectable methods ***/
    STDMETHOD(GetIids)(__RPC__out ULONG *iidCount, __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids) PURE;
    STDMETHOD(GetRuntimeClassName)(__RPC__deref_out_opt HSTRING *className) PURE;
    STDMETHOD(GetTrustLevel)(__RPC__out TrustLevel *trustLevel) PURE;

    /*** IApplicationView methods ***/
    STDMETHOD(SetFocus)(THIS) PURE;
    STDMETHOD(SwitchTo)(THIS) PURE;
    STDMETHOD(TryInvokeBack)(THIS_ IAsyncCallback*) PURE; // Proc8
    STDMETHOD(GetThumbnailWindow)(THIS_ HWND*) PURE; // Proc9
    STDMETHOD(GetMonitor)(THIS_ IImmersiveMonitor**) PURE; // Proc10
    STDMETHOD(GetVisibility)(THIS_ int*) PURE; // Proc11
    STDMETHOD(SetCloak)(THIS_ APPLICATION_VIEW_CLOAK_TYPE, int) PURE; // Proc12
    STDMETHOD(GetPosition)(THIS_ REFIID, void**) PURE; // Proc13
    STDMETHOD(SetPosition)(THIS_ IApplicationViewPosition*) PURE; // Proc14
    STDMETHOD(InsertAfterWindow)(THIS_ HWND) PURE; // Proc15
    STDMETHOD(GetExtendedFramePosition)(THIS_ RECT*) PURE; // Proc16
    STDMETHOD(GetAppUserModelId)(THIS_ PWSTR*) PURE; // Proc17
    STDMETHOD(SetAppUserModelId)(THIS_ PCWSTR) PURE; // Proc18
    STDMETHOD(IsEqualByAppUserModelId)(THIS_ PCWSTR, int*) PURE; // Proc19
    STDMETHOD(GetViewState)(THIS_ UINT*) PURE; // Proc20
    STDMETHOD(SetViewState)(THIS_ UINT) PURE; // Proc21
    STDMETHOD(GetNeediness)(THIS_ int*) PURE; // Proc22
    STDMETHOD(GetLastActivationTimestamp)(THIS_ ULONGLONG*) PURE;
    STDMETHOD(SetLastActivationTimestamp)(THIS_ ULONGLONG) PURE;
    STDMETHOD(GetVirtualDesktopId)(THIS_ GUID*) PURE;
    STDMETHOD(SetVirtualDesktopId)(THIS_ REFGUID) PURE;
    STDMETHOD(GetShowInSwitchers)(THIS_ int*) PURE;
    STDMETHOD(SetShowInSwitchers)(THIS_ int) PURE;
    STDMETHOD(GetScaleFactor)(THIS_ int*) PURE;
    STDMETHOD(CanReceiveInput)(THIS_ BOOL*) PURE;
    STDMETHOD(GetCompatibilityPolicyType)(THIS_ APPLICATION_VIEW_COMPATIBILITY_POLICY*) PURE;
    STDMETHOD(SetCompatibilityPolicyType)(THIS_ APPLICATION_VIEW_COMPATIBILITY_POLICY) PURE;
    //STDMETHOD(GetPositionPriority)(THIS_ IShellPositionerPriority**) PURE; // removed in 1803
    //STDMETHOD(SetPositionPriority)(THIS_ IShellPositionerPriority*) PURE; // removed in 1803
    STDMETHOD(GetSizeConstraints)(THIS_ IImmersiveMonitor*, SIZE*, SIZE*) PURE;
    STDMETHOD(GetSizeConstraintsForDpi)(THIS_ UINT, SIZE*, SIZE*) PURE;
    STDMETHOD(SetSizeConstraintsForDpi)(THIS_ const UINT*, const SIZE*, const SIZE*) PURE;
    //STDMETHOD(QuerySizeConstraintsFromApp)(THIS) PURE; // removed in 1803
    STDMETHOD(OnMinSizePreferencesUpdated)(THIS_ HWND) PURE;
    STDMETHOD(ApplyOperation)(THIS_ IApplicationViewOperation*) PURE;
    STDMETHOD(IsTray)(THIS_ BOOL*) PURE;
    STDMETHOD(IsInHighZOrderBand)(THIS_ BOOL*) PURE;
    STDMETHOD(IsSplashScreenPresented)(THIS_ BOOL*) PURE;
    STDMETHOD(Flash)(THIS) PURE;
    STDMETHOD(GetRootSwitchableOwner)(THIS_ IApplicationView**) PURE; // proc45
    STDMETHOD(EnumerateOwnershipTree)(THIS_ IObjectArray**) PURE; // proc46

    STDMETHOD(GetEnterpriseId)(THIS_ PWSTR*) PURE; // proc47
    STDMETHOD(IsMirrored)(THIS_ BOOL*) PURE; //

    STDMETHOD(Unknown1)(THIS_ int*) PURE;
    STDMETHOD(Unknown2)(THIS_ int*) PURE;
    STDMETHOD(Unknown3)(THIS_ int*) PURE;
    STDMETHOD(Unknown4)(THIS_ int) PURE;
    STDMETHOD(Unknown5)(THIS_ int*) PURE;
    STDMETHOD(Unknown6)(THIS_ int) PURE;
    STDMETHOD(Unknown7)(THIS) PURE;
    STDMETHOD(Unknown8)(THIS_ int*) PURE;
    STDMETHOD(Unknown9)(THIS_ int) PURE;
    STDMETHOD(Unknown10)(THIS_ int, int) PURE;
    STDMETHOD(Unknown11)(THIS_ int) PURE;
    STDMETHOD(Unknown12)(THIS_ SIZE*) PURE;
};

MIDL_INTERFACE("FF72FFDD-BE7E-43FC-9C03-AD81681E88E4")
IVirtualDesktop : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE IsViewVisible(IApplicationView *View, int *Visible) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetID(GUID *ID) = 0;
};

MIDL_INTERFACE("C179334C-4295-40D3-BEA1-C654D965605A")
IVirtualDesktopNotification : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE VirtualDesktopCreated(IVirtualDesktop *Desktop) = 0;
    virtual HRESULT STDMETHODCALLTYPE VirtualDesktopDestroyBegin(IVirtualDesktop *DesktopDestroyed, IVirtualDesktop *DesktopFallback) = 0;
    virtual HRESULT STDMETHODCALLTYPE VirtualDesktopDestroyFailed(IVirtualDesktop *DesktopDestroyed, IVirtualDesktop *DesktopFallback) = 0;
    virtual HRESULT STDMETHODCALLTYPE VirtualDesktopDestroyed(IVirtualDesktop *DesktopDestroyed, IVirtualDesktop *DesktopFallback) = 0;
    virtual HRESULT STDMETHODCALLTYPE ViewVirtualDesktopChanged(IApplicationView *View) = 0;
    virtual HRESULT STDMETHODCALLTYPE CurrentVirtualDesktopChanged(IVirtualDesktop *OldDesktop, IVirtualDesktop *NewDesktop) = 0;
};

MIDL_INTERFACE("0CD45E71-D927-4F15-8B0A-8FEF525337BF")
IVirtualDesktopNotificationService : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE Register(IVirtualDesktopNotification *Notification, DWORD *Cookie) = 0;
    virtual HRESULT STDMETHODCALLTYPE Unregister(DWORD Cookie) = 0;
};

DECLARE_INTERFACE_IID_(IVirtualDesktopPinnedApps, IUnknown, "4ce81583-1e4c-4632-a621-07a53543148f")
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID RefIID, LPVOID FAR *Object) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /*** IVirtualDesktopPinnedApps methods ***/
    STDMETHOD(IsAppIdPinned)(THIS_ PCWSTR appId, BOOL *) PURE;
    STDMETHOD(PinAppID)(THIS_ PCWSTR AppID) PURE;
    STDMETHOD(UnpinAppID)(THIS_ PCWSTR AppID) PURE;
    STDMETHOD(IsViewPinned)(THIS_ IApplicationView *, BOOL *) PURE;
    STDMETHOD(PinView)(THIS_ IApplicationView *) PURE;
    STDMETHOD(UnpinView)(THIS_ IApplicationView *) PURE;
};

// NOTE(chuck): The GUID of IApplicationViewCollection was changed (!) in build 1809.
// See https://github.com/Grabacr07/VirtualDesktop/issues/36
//MIDL_INTERFACE("2C08ADF0-A386-4B35-9250-0FE183476FCC") IApplicationViewCollection : public IUnknown
MIDL_INTERFACE("1841C6D7-4F9D-42C0-AF41-8747538F10E5") IApplicationViewCollection : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE GetViews(IObjectArray **Array) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetViewsByZOrder(IObjectArray **Array) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetViewsByAppUserModelId(BSTR pId, IObjectArray **Array) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetViewForHwnd(HWND pHwnd, IApplicationView **View) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetViewForApplication(void *pApplication, IApplicationView **View) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetViewForAppUserModelId(BSTR ID, IApplicationView **View) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetViewInFocus(int *View) = 0;
    virtual HRESULT STDMETHODCALLTYPE RefreshCollection() = 0;
    virtual HRESULT STDMETHODCALLTYPE RegisterForApplicationViewChanges(void *Listener, int *Cookie) = 0;
    virtual HRESULT STDMETHODCALLTYPE RegisterForApplicationViewPositionChanges(void *pListener, int *Cookie) = 0;
    virtual HRESULT STDMETHODCALLTYPE UnregisterForApplicationViewChanges(int Cookie) = 0;
};

struct IVirtualDesktopManagerInternal : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE GetCount(UINT *Count) = 0;
    virtual HRESULT STDMETHODCALLTYPE MoveViewToDesktop(IApplicationView *View, IVirtualDesktop *Desktop) = 0;
    // 10240
    virtual HRESULT STDMETHODCALLTYPE CanViewMoveDesktops(IApplicationView *View, int *CanViewMoveDesktops) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetCurrentDesktop(IVirtualDesktop **Desktop) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetDesktops(IObjectArray **Desktops) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetAdjacentDesktop(IVirtualDesktop *DesktopReference, AdjacentDesktop Direction, IVirtualDesktop **AdjacentDesktop) = 0;
    virtual HRESULT STDMETHODCALLTYPE SwitchDesktop(IVirtualDesktop *Desktop) = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateDesktopW(IVirtualDesktop **NewDesktop) = 0;
    virtual HRESULT STDMETHODCALLTYPE RemoveDesktop(IVirtualDesktop *Remove, IVirtualDesktop *pFallbackDesktop) = 0;
    // 10240
    virtual HRESULT STDMETHODCALLTYPE FindDesktop(GUID *DesktopID, IVirtualDesktop **Desktop) = 0;
};

// // 10130
// MIDL_INTERFACE("EF9F1A6C-D3CC-4358-B712-F84B635BEBE7")
// IVirtualDesktopManagerInternal10130 : public
// IVirtualDesktopManagerInternal{};
// // 10240
// MIDL_INTERFACE("AF8DA486-95BB-4460-B3B7-6E7A6B2962B5")
// IVirtualDesktopManagerInternal10240 : public
// IVirtualDesktopManagerInternal{};
// // 10536
MIDL_INTERFACE("f31574d6-b682-4cdc-bd56-1827860abec6")
IVirtualDesktopManagerInternal10536 : public
IVirtualDesktopManagerInternal{};

#endif // VIRTUAL_DESKTOPPER_H