#include "virtual_desktopper.h"

global_variable HANDLE GlobalTimer;
global_variable HFONT GlobalFont;
global_variable HWND GlobalWindow;
global_variable IApplicationViewCollection *GlobalApplicationViewCollection;
global_variable int GlobalCurrentVirtualDesktopIndex;
global_variable IVirtualDesktopManagerInternal *GlobalVirtualDesktopManagerInternal;
global_variable IVirtualDesktopPinnedApps *GlobalVirtualDesktopPinnedApps;
global_variable char GlobalTempBuffer[MAX_PATH];

LRESULT __stdcall
Win32MainWindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
    LRESULT Result = 0;

    switch(Message)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT PaintStruct;
            HDC DeviceContext = BeginPaint(Window, &PaintStruct);

            RECT ClientRect;
            GetClientRect(Window, &ClientRect);
            FillRect(DeviceContext, &ClientRect, (HBRUSH)GetStockObject(BLACK_PEN));

            StringCbPrintf(GlobalTempBuffer, MAX_PATH, "%d", 1 + GlobalCurrentVirtualDesktopIndex);
            int TextLength = (int)strlen(GlobalTempBuffer);

            SelectObject(DeviceContext, GlobalFont);
            SetTextColor(DeviceContext, RGB(255, 255, 255));
            SetBkColor(DeviceContext, RGB(0, 0, 0));
            DrawText(DeviceContext, GlobalTempBuffer, TextLength, &ClientRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

            EndPaint(Window, &PaintStruct);
            break;
        }
        default:
        {
            Result = DefWindowProc(Window, Message, WParam, LParam);
            break;
        }
    }

    return(Result);
}

internal int
Win32GetVirtualDesktopCount()
{
    UINT Result = 1;

    IObjectArray *ObjectArray;
    if(S_OK == GlobalVirtualDesktopManagerInternal->GetDesktops(&ObjectArray))
    {
        ObjectArray->GetCount(&Result);
        ObjectArray->Release();
    }

    return(Result);
}

internal int
Win32GetVirtualDesktopIndexByID(GUID VirtualDesktopID)
{
    int Result = -1;
    IObjectArray *ObjectArray;

    if(S_OK == GlobalVirtualDesktopManagerInternal->GetDesktops(&ObjectArray))
    {
        UINT Count;
        if(S_OK == ObjectArray->GetCount(&Count))
        {
            for(UINT Index = 0;
                Index < Count;
                ++Index)
            {
                IVirtualDesktop *Desktop;
                if(FAILED(ObjectArray->GetAt(Index, __uuidof(IVirtualDesktop), (void **)&Desktop)))
                {
                    continue;
                }

                GUID ID;
                if((S_OK == Desktop->GetID(&ID)) && (ID == VirtualDesktopID))
                {
                    Result = Index;
                    Desktop->Release();
                    break;
                }

                Desktop->Release();
            }
        }

        ObjectArray->Release();
    }

    return(Result);
}

internal int
Win32GetVirtualDesktopIndex(IVirtualDesktop *Desktop)
{
    int Result = -1;
    GUID ID;

    if(S_OK == Desktop->GetID(&ID))
    {
        Result = Win32GetVirtualDesktopIndexByID(ID);
    }

    return(Result);
}

internal int
Win32GetCurrentVirtualDesktopIndex()
{
    IVirtualDesktop *Desktop;
    GlobalVirtualDesktopManagerInternal->GetCurrentDesktop(&Desktop);
    int Result = Win32GetVirtualDesktopIndex(Desktop);
    Desktop->Release();
    return(Result);
}

internal void
Win32PinWindow(HWND Window)
{
    IApplicationView *ApplicationView;
    GlobalApplicationViewCollection->GetViewForHwnd(Window, &ApplicationView);

    if(ApplicationView)
    {
        GlobalVirtualDesktopPinnedApps->PinView(ApplicationView);
        ApplicationView->Release();
    }
}

void __stdcall
Win32HideWindowCallback(void *Parameter, BOOLEAN TimerOrWaitFired)
{
    // NOTE(chuck): Hiding unpins the window.
    ShowWindow(GlobalWindow, SW_HIDE);
}

struct virtual_desktop_notification : IVirtualDesktopNotification
{
    ULONG Count;

    virtual HRESULT __stdcall
    QueryInterface(const GUID &InterfaceIdentifier, void **Object) override
    {
        HRESULT Result;
        OutputDebugString("QueryInterface\n");
        if(!Object)
        {
            Result = E_INVALIDARG;
        }
        else
        {
            *Object = 0;

            if((InterfaceIdentifier == IID_IUnknown) ||
               (InterfaceIdentifier == GUID_IVirtualDesktopNotification))
            {
                *Object = (void *)this;
                AddRef();
                Result = S_OK;
            }
            else
            {
                Result = E_NOINTERFACE;
            }
        }

        return(Result);
    }

    virtual ULONG __stdcall
    AddRef() override
    {
        OutputDebugString("AddRef\n");
        ULONG Result = InterlockedIncrement(&Count);
        return(Result);
    }

    virtual ULONG __stdcall
    Release() override
    {
        OutputDebugString("Release\n");
        ULONG Result = InterlockedDecrement(&Count);
        if(Result == 0)
        {
            delete this;
        }
        return(Result);
    }

    HRESULT
    ViewVirtualDesktopChanged(IApplicationView *View)
    {
        HRESULT Result = S_OK;
        OutputDebugString("ViewVirtualDesktopChanged\n");
        return(Result);
    }

    HRESULT
    CurrentVirtualDesktopChanged(IVirtualDesktop *OldDesktop, IVirtualDesktop *NewDesktop)
    {
        HRESULT Result = S_OK;

        // NOTE(chuck): Hiding unpins the window.
        ShowWindow(GlobalWindow, SW_SHOW);
        Win32PinWindow(GlobalWindow);

        GlobalCurrentVirtualDesktopIndex = Win32GetVirtualDesktopIndex(NewDesktop);
        int OldVirtualDesktopIndex = Win32GetVirtualDesktopIndex(OldDesktop);
        StringCbPrintf(GlobalTempBuffer, MAX_PATH, "CurrentVirtuaLDesktopChanged: old %d, new %d\n", OldVirtualDesktopIndex, GlobalCurrentVirtualDesktopIndex);
        OutputDebugString(GlobalTempBuffer);
        RedrawWindow(GlobalWindow, 0, 0, RDW_INVALIDATE);

        DeleteTimerQueueTimer(0, GlobalTimer, 0);
        CreateTimerQueueTimer(&GlobalTimer, 0, Win32HideWindowCallback, 0, VIRTUAL_DESKTOP_NUMERAL_TIMEOUT, 0, WT_EXECUTEONLYONCE);

        return(Result);
    }

    HRESULT
    VirtualDesktopCreated(IVirtualDesktop *Desktop)
    {
        HRESULT Result = S_OK;
        OutputDebugString("VirtualDesktopCreated\n");
        return(Result);
    }

    HRESULT
    VirtualDesktopDestroyBegin(IVirtualDesktop *DesktopDestroyed, IVirtualDesktop *DesktopFallback)
    {
        HRESULT Result = S_OK;
        OutputDebugString("VirtualDesktopDestroyBegin\n");
        return(Result);
    }

    HRESULT
    VirtualDesktopDestroyFailed(IVirtualDesktop *Destroyed, IVirtualDesktop *Fallback)
    {
        HRESULT Result = S_OK;
        OutputDebugString("VirtualDesktopDestroyFailed\n");
        return(Result);
    }

    HRESULT
    VirtualDesktopDestroyed(IVirtualDesktop *Destroyed, IVirtualDesktop *Fallback)
    {
        HRESULT Result = S_OK;
        OutputDebugString("VirtualDesktopDestroyed\n");
        return(Result);
    }
};

internal b32
Win32InitVirtualDesktopNotifications()
{
    b32 Result = false;

    CoInitializeEx(0, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    IServiceProvider *ServiceProvider;
    CoCreateInstance(GUID_ImmersiveShell, 0, CLSCTX_LOCAL_SERVER, __uuidof(IServiceProvider), (void **)&ServiceProvider);

    IVirtualDesktopManager *VirtualDesktopManager;
    if(S_OK == ServiceProvider->QueryService(__uuidof(IVirtualDesktopManager), &VirtualDesktopManager))
    {
        if(S_OK == ServiceProvider->QueryService(GUID_VirtualDesktopAPI_Unknown, &GlobalVirtualDesktopManagerInternal))
        {
            IVirtualDesktopNotificationService *NotificationService;
            if(S_OK == ServiceProvider->QueryService(GUID_IVirtualNotificationService, &NotificationService))
            {
                DWORD Cookie;
                virtual_desktop_notification *VirtualDesktopNotification = new virtual_desktop_notification();
                if(S_OK == NotificationService->Register(VirtualDesktopNotification, &Cookie))
                {
                    if(S_OK == ServiceProvider->QueryService(GUID_IApplicationViewCollection, GUID_IApplicationViewCollection, (void **)&GlobalApplicationViewCollection))
                    {
                        if(S_OK == ServiceProvider->QueryService(GUID_VirtualDesktopPinnedApps, __uuidof(IVirtualDesktopPinnedApps), (void **)&GlobalVirtualDesktopPinnedApps))
                        {
                            Result = true;

                            int VirtualDesktopCount = Win32GetVirtualDesktopCount();

                            StringCbPrintf(GlobalTempBuffer, MAX_PATH, "Virtual desktop count: %d\n", VirtualDesktopCount);
                            OutputDebugString(GlobalTempBuffer);
                        }
                        else
                        {
                            OutputDebugString("VirtualDesktopPinnedApps was not created.\n");
                        }
                    }
                    else
                    {
                        OutputDebugString("ApplicationViewCollection was not created.\n");
                    }
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
        }
        else
        {
            OutputDebugString("VirtualDesktopManager1030 was not created.\n");
        }
    }
    else
    {
        OutputDebugString("VirtualDesktopManager was not created.\n");
    }

    return(Result);
}

int __stdcall
WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CommandLine, int ShowCode)
{
    if(Win32InitVirtualDesktopNotifications())
    {
        GlobalCurrentVirtualDesktopIndex = Win32GetCurrentVirtualDesktopIndex();
        CreateTimerQueueTimer(&GlobalTimer, 0, Win32HideWindowCallback, 0, VIRTUAL_DESKTOP_NUMERAL_TIMEOUT, 0, WT_EXECUTEONLYONCE);
    }
    else
    {
        OutputDebugString("Failed to initialize virtual desktop notifications.\n");
    }

    WNDCLASSEX WindowClass = {};
    WindowClass.cbSize = sizeof(WNDCLASSEX);
    WindowClass.hInstance = Instance;
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.lpszClassName = "VirtualDesktopperClassName";

    if(RegisterClassEx(&WindowClass))
    {
        int WindowWidth = 200;
        int WindowHeight = 200;
        int ClientWidth = GetSystemMetrics(SM_CXFULLSCREEN);
        int ClientHeight = GetSystemMetrics(SM_CYFULLSCREEN);

        StringCbPrintf(GlobalTempBuffer, MAX_PATH, "Full-screen client area: %dx%d\n", ClientWidth, ClientHeight);
        OutputDebugString(GlobalTempBuffer);

        GlobalWindow = CreateWindowEx(
            WS_EX_TOPMOST,
            WindowClass.lpszClassName, "Virtual Desktopper",
            WS_POPUP | WS_VISIBLE,
            (ClientWidth  / 2) - (WindowWidth  / 2),
            (ClientHeight / 2) - (WindowHeight / 2),
            WindowWidth, WindowHeight,
            0, 0, Instance, 0);

        if(GlobalWindow)
        {
            GlobalFont = CreateFont(
                160, 0, 0, 0, FW_DONTCARE, 0, 0, 0, ANSI_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH,
                "Arial");
            Win32PinWindow(GlobalWindow);

            ANIMATIONINFO AnimationInfo;
            AnimationInfo.cbSize = sizeof(ANIMATIONINFO);
            AnimationInfo.iMinAnimate = 0;
            SystemParametersInfo(SPI_SETANIMATION, sizeof(ANIMATIONINFO), &AnimationInfo, 0);

            MSG Message;
            while(GetMessage(&Message, 0, 0, 0))
            {
                TranslateMessage(&Message);
                DispatchMessageA(&Message);
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

    return(0);
}
