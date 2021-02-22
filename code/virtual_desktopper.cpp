#include <stdint.h>
#include <strsafe.h>
#include <windows.h>

#include "virtual_desktopper.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef bool b32;

#define global_variable static
#define internal static

global_variable bool GlobalRunning;

LRESULT CALLBACK
MainWindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
    LRESULT Result = 0;

    switch (Message)
    {
        default:
        {
            Result = DefWindowProc(Window, Message, WParam, LParam);
            break;
        }
        case WM_ACTIVATEAPP:
        {
            OutputDebugString("WM_ACTIVATEAPP\n");
            break;
        }
        case WM_CLOSE:
        {
            GlobalRunning = false;
            break;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);

            EndPaint(Window, &Paint);

            break;
        }
    }

    return (Result);
}

BOOL CALLBACK
EnumWindowsProc(HWND Window, LPARAM LParam)
{
    if (IsWindowVisible(Window))
    {
        char WindowText[MAX_PATH];
        GetWindowText(Window, WindowText, MAX_PATH - 1);

        char LogText[MAX_PATH];
        StringCbPrintf(LogText, MAX_PATH, "%s\n", WindowText);
        OutputDebugString(LogText);
    }

    return TRUE;
}

struct IApplicationView;

MIDL_INTERFACE("FF72FFDD-BE7E-43FC-9C03-AD81681E88E4")
IVirtualDesktop : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE IsViewVisible(IApplicationView *pView, int *pfVisible) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetID(GUID *pGuid) = 0;
};

// extern "C" const IID IID_IVirtualDesktop;

// MIDL_INTERFACE("FF72FFDD-BE7E-43FC-9C03-AD81681E88E4")
// IVirtualDesktop : public IUnknown
// {
//     virtual HRESULT STDMETHODCALLTYPE IsViewVisible(
//         IApplicationView * pView, int *pfVisible) = 0;
//     virtual HRESULT STDMETHODCALLTYPE GetID(GUID * pGuid) = 0;
// };

// extern "C" const IID IID_IVirtualDesktopManagerInternal;

// struct IVirtualDesktopManagerInternal : public IUnknown
// {
//     virtual HRESULT STDMETHODCALLTYPE GetCount(UINT *pCount) = 0;
//     virtual HRESULT STDMETHODCALLTYPE
//     MoveViewToDesktop(IApplicationView *pView, IVirtualDesktop *pDesktop) =
//     0;
//     // 10240
//     virtual HRESULT STDMETHODCALLTYPE CanViewMoveDesktops(
//         IApplicationView *pView,
//         int *pfCanViewMoveDesktops) = 0;
//     virtual HRESULT STDMETHODCALLTYPE
//     GetCurrentDesktop(IVirtualDesktop **desktop) = 0;
//     virtual HRESULT STDMETHODCALLTYPE
//     GetDesktops(IObjectArray **ppDesktops) = 0;
//     virtual HRESULT STDMETHODCALLTYPE GetAdjacentDesktop(
//         IVirtualDesktop *pDesktopReference,
//         AdjacentDesktop uDirection,
//         IVirtualDesktop **ppAdjacentDesktop) = 0;
//     virtual HRESULT STDMETHODCALLTYPE
//     SwitchDesktop(IVirtualDesktop *pDesktop) = 0;
//     virtual HRESULT STDMETHODCALLTYPE
//     CreateDesktopW(IVirtualDesktop **ppNewDesktop) = 0;
//     virtual HRESULT STDMETHODCALLTYPE RemoveDesktop(
//         IVirtualDesktop *pRemove,
//         IVirtualDesktop *pFallbackDesktop) = 0;
//     // 10240
//     virtual HRESULT STDMETHODCALLTYPE
//     FindDesktop(GUID *desktopId, IVirtualDesktop **ppDesktop) = 0;
// };
// // 10130
// MIDL_INTERFACE("EF9F1A6C-D3CC-4358-B712-F84B635BEBE7")
// IVirtualDesktopManagerInternal10130 : public
// IVirtualDesktopManagerInternal{};
// // 10240
// MIDL_INTERFACE("AF8DA486-95BB-4460-B3B7-6E7A6B2962B5")
// IVirtualDesktopManagerInternal10240 : public
// IVirtualDesktopManagerInternal{};
// // 10536
// MIDL_INTERFACE("f31574d6-b682-4cdc-bd56-1827860abec6")
// IVirtualDesktopManagerInternal10536 : public
// IVirtualDesktopManagerInternal{};

// extern "C" const IID IID_IVirtualDesktopNotification;
MIDL_INTERFACE("C179334C-4295-40D3-BEA1-C654D965605A")
IVirtualDesktopNotification : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE VirtualDesktopCreated(IVirtualDesktop *pDesktop) = 0;
    virtual HRESULT STDMETHODCALLTYPE VirtualDesktopDestroyBegin(IVirtualDesktop *pDesktopDestroyed, IVirtualDesktop *pDesktopFallback) = 0;
    virtual HRESULT STDMETHODCALLTYPE VirtualDesktopDestroyFailed(IVirtualDesktop *pDesktopDestroyed, IVirtualDesktop *pDesktopFallback) = 0;
    virtual HRESULT STDMETHODCALLTYPE VirtualDesktopDestroyed(IVirtualDesktop *pDesktopDestroyed, IVirtualDesktop *pDesktopFallback) = 0;
    virtual HRESULT STDMETHODCALLTYPE ViewVirtualDesktopChanged(IApplicationView *pView) = 0;
    virtual HRESULT STDMETHODCALLTYPE CurrentVirtualDesktopChanged(IVirtualDesktop *OldDesktop, IVirtualDesktop *NewDesktop) = 0;
};

struct virtual_desktop_notification : IVirtualDesktopNotification
{
    ULONG Count{0};

    virtual HRESULT STDMETHODCALLTYPE
    QueryInterface(REFIID InterfaceIdentifier, void **Object) override
    {
        HRESULT Result;
        OutputDebugString("QueryInterface\n");
        if (!Object)
        {
            OutputDebugString("  E_INVALIDARG\n");
            Result = E_INVALIDARG;
        }
        else
        {
            char Buffer[MAX_PATH];
            StringCbPrintf(Buffer, MAX_PATH, "  Interface identifier %x\n", InterfaceIdentifier);
            OutputDebugString(Buffer);
            *Object = 0;

            if (InterfaceIdentifier == IID_IUnknown ||
                InterfaceIdentifier == IID_IVirtualDesktopNotification)
            {
                // Increment the reference count and return the pointer.
                *Object = (LPVOID)this;
                AddRef();
                Result = S_OK;
            }
            else
            {
                Result = E_NOINTERFACE;
            }
        }

        return (Result);
    }

    virtual ULONG STDMETHODCALLTYPE
    AddRef() override
    {
        ULONG Result;
        OutputDebugString("AddRef\n");
        Result = InterlockedIncrement(&Count);
        return (Result);
    }

    virtual ULONG STDMETHODCALLTYPE
    Release() override
    {
        ULONG Result;

        OutputDebugString("Release\n");

        Result = InterlockedDecrement(&Count);
        if (Result == 0)
        {
            delete this;
        }

        return (Result);
    }

    HRESULT
    ViewVirtualDesktopChanged(IApplicationView *View)
    {
        HRESULT Result = S_OK;
        OutputDebugString("ViewVirtualDesktopChanged\n");
        return (Result);
    }

    HRESULT
    CurrentVirtualDesktopChanged(IVirtualDesktop *OldDesktop, IVirtualDesktop *NewDesktop)
    {
        HRESULT Result = S_OK;

        OutputDebugString("CurrentVirtualDesktopChanged\n");
        // // @TODO @PERF @FIXME .. how to get guid from IVirtualDesktp?
        // IVirtualDesktop *pd[2] = {OldDesktop, NewDesktop};
        // size_t idx[2] = {0};
        // size_t found = 0;
        // for (size_t i = 0; i < 2; ++i)
        // {
        //     for (size_t j = 0, je = m_vdm.m_desktops.size(); j < je; ++j)
        //     {
        //         IVirtualDesktop *ivd = nullptr;
        //         GUID g = {0};
        //         if (SUCCEEDED(
        //                 m_vdm.m_vdmi->FindDesktop(&m_vdm.m_desktops[j],
        //                 &ivd)))
        //         {
        //             scope_guard_t on_exit_ivd = mkScopeGuard(
        //                 std::mem_fun(&IVirtualDesktop::Release), ivd);
        //             if (ivd == pd[i])
        //             {
        //                 idx[i] = j;
        //                 ++found;
        //             }
        //         }
        //         else
        //             return S_FALSE;
        //     }
        // }
        // if (found == 2)
        // {
        //     bb::BlackBox::Instance().GetTasks().OnSwitchDesktopVDM(
        //         m_vdm.m_ids[idx[0]], m_vdm.m_ids[idx[1]]);
        //     bb::BlackBox::Instance().GetWorkSpaces().OnSwitchedDesktop();
        // }
        return (Result);
    }

    HRESULT
    VirtualDesktopCreated(IVirtualDesktop *pDesktop)
    {
        HRESULT Result = S_OK;
        OutputDebugString("VirtualDesktopCreated\n");
        return (Result);
    }

    HRESULT
    VirtualDesktopDestroyBegin(IVirtualDesktop *pDesktopDestroyed, IVirtualDesktop *pDesktopFallback)
    {
        HRESULT Result = S_OK;
        OutputDebugString("VirtualDesktopDestroyBegin\n");
        return (Result);
    }

    HRESULT
    VirtualDesktopDestroyFailed(IVirtualDesktop *Destroyed, IVirtualDesktop *Fallback)
    {
        HRESULT Result = S_OK;
        OutputDebugString("VirtualDesktopDestroyFailed\n");
        return (Result);
    }

    HRESULT
    VirtualDesktopDestroyed(IVirtualDesktop *Destroyed, IVirtualDesktop *Fallback)
    {
        HRESULT Result = S_OK;
        OutputDebugString("VirtualDesktopDestroyed\n");
        return (Result);
    }
};

extern "C" const IID IID_IVirtualDesktopNotificationService;
MIDL_INTERFACE("0CD45E71-D927-4F15-8B0A-8FEF525337BF")
IVirtualDesktopNotificationService : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE Register(IVirtualDesktopNotification *pNotification, DWORD *Cookie) = 0;
    virtual HRESULT STDMETHODCALLTYPE Unregister(DWORD Cookie) = 0;
};

internal b32
Win32InitVirtualDesktopNotifications()
{
    b32 Result = false;

    if (SUCCEEDED(CoInitializeEx(0, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
    {
        IServiceProvider *ServiceProvider = 0;
        if (SUCCEEDED(CoCreateInstance(CLSID_ImmersiveShell, 0, CLSCTX_LOCAL_SERVER, __uuidof(IServiceProvider), (PVOID *)&ServiceProvider)))
        {
            IVirtualDesktopManager *VirtualDesktopManager = 0;
            if (SUCCEEDED(ServiceProvider->QueryService(__uuidof(IVirtualDesktopManager), &VirtualDesktopManager)))
            {
                // IVirtualDesktopManagerInternal10536 *VDM10536 = 0;
                // if (SUCCEEDED(ServiceProvider->QueryService(
                //         CLSID_VirtualDesktopAPI_Unknown, &VDM10536)))
                // {
                IVirtualDesktopNotificationService *NotificationService = 0;
                if (SUCCEEDED(ServiceProvider->QueryService(CLSID_IVirtualNotificationService, &NotificationService)))
                {
                    DWORD Cookie = 0;
                    virtual_desktop_notification *VirtualDesktopNotification = new virtual_desktop_notification();
                    if (SUCCEEDED(NotificationService->Register(VirtualDesktopNotification, &Cookie)))
                    {
                        // EnumWindows(EnumWindowsProc, 0);
                        Result = true;
                    }
                    else
                    {
                        OutputDebugString("GlobalVirtualDesktopNotification was not registered.\n");
                    }
                }
                else
                {
                    OutputDebugString("NotificationService was not created.\n");
                }
                // }
                // else
                // {
                //     OutputDebugString(
                //         "VirtualDesktopManager1030 was not created.\n");
                // }
            }
            else
            {
                OutputDebugString("VirtualDesktopManager was not created.\n");
            }
        }
        else
        {
            OutputDebugString("ServiceProvider was not created.\n");
        }
    }
    else
    {
        OutputDebugString("COM was not initialized.\n");
    }

    return (Result);
}

int CALLBACK
WinMain(
    HINSTANCE Instance,
    HINSTANCE PrevInstance,
    LPSTR CommandLine,
    int ShowCode)
{
    if (Win32InitVirtualDesktopNotifications())
    {
        WNDCLASSEX WindowClass = {};
        WindowClass.cbSize = sizeof(WindowClass);
        WindowClass.hInstance = Instance;
        WindowClass.lpfnWndProc = MainWindowCallback;
        WindowClass.lpszClassName = "VirtualDesktopperClassName";

        if (RegisterClassEx(&WindowClass))
        {
            HWND Window = CreateWindowEx(
                0, WindowClass.lpszClassName, "Virtual Desktopper",
                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                3840 / 2 - 640, 2160 / 2 - 480, 640, 480,
                0, 0, Instance, 0);

            if (Window)
            {
                GlobalRunning = true;
                while (GlobalRunning)
                {
                    MSG Message;
                    while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
                    {
                        TranslateMessage(&Message);
                        DispatchMessageA(&Message);
                    }
                }
            }
            else
            {
                OutputDebugString("Failed to create window.\n");
            }
        }
        else
        {
            OutputDebugString("Failed to register window class.\n");
        }
    }
    else
    {
        OutputDebugString(
            "Failed to initialize virtual desktop notifications.\n");
    }

    return (0);
}
