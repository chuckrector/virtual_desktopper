#include "virtual_desktopper.h"

global_variable HANDLE GlobalTimer;
global_variable HFONT GlobalFont;
global_variable HWND GlobalWindow;
global_variable IApplicationViewCollection *GlobalApplicationViewCollection;
global_variable int GlobalCurrentVirtualDesktopIndex;
global_variable IVirtualDesktopManagerInternal *GlobalVirtualDesktopManagerInternal;
global_variable IVirtualDesktopPinnedApps *GlobalVirtualDesktopPinnedApps;
global_variable char GlobalTempBuffer[MAX_PATH];

internal LRESULT __stdcall
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
    if(S_OK == GlobalVirtualDesktopManagerInternal->lpVtbl->GetDesktops(GlobalVirtualDesktopManagerInternal, &ObjectArray))
    {
        ObjectArray->lpVtbl->GetCount(ObjectArray, &Result);
        ObjectArray->lpVtbl->Release(ObjectArray);
    }

    return(Result);
}

internal int
Win32GetVirtualDesktopIndexByID(GUID VirtualDesktopID)
{
    int Result = -1;
    IObjectArray *ObjectArray;

    if(S_OK == GlobalVirtualDesktopManagerInternal->lpVtbl->GetDesktops(GlobalVirtualDesktopManagerInternal, &ObjectArray))
    {
        UINT Count;
        if(S_OK == ObjectArray->lpVtbl->GetCount(ObjectArray, &Count))
        {
            for(UINT Index = 0;
                Index < Count;
                ++Index)
            {
                IVirtualDesktop *Desktop;
                if(FAILED(ObjectArray->lpVtbl->GetAt(ObjectArray, Index, &GUID_IVirtualDesktop, (void **)&Desktop)))
                {
                    continue;
                }

                GUID ID;
                if((S_OK == Desktop->lpVtbl->GetID(Desktop, &ID)) && IsEqualGUID(&ID, &VirtualDesktopID))
                {
                    Result = Index;
                    Desktop->lpVtbl->Release(Desktop);
                    break;
                }

                Desktop->lpVtbl->Release(Desktop);
            }
        }

        ObjectArray->lpVtbl->Release(ObjectArray);
    }

    return(Result);
}

internal int
Win32GetVirtualDesktopIndex(IVirtualDesktop *Desktop)
{
    int Result = -1;
    GUID ID;

    if(S_OK == Desktop->lpVtbl->GetID(Desktop, &ID))
    {
        Result = Win32GetVirtualDesktopIndexByID(ID);
    }

    return(Result);
}

internal int
Win32GetCurrentVirtualDesktopIndex()
{
    IVirtualDesktop *Desktop;
    GlobalVirtualDesktopManagerInternal->lpVtbl->GetCurrentDesktop(GlobalVirtualDesktopManagerInternal, &Desktop);
    int Result = Win32GetVirtualDesktopIndex(Desktop);
    Desktop->lpVtbl->Release(Desktop);
    return(Result);
}

internal void
Win32PinWindow(HWND Window)
{
    IApplicationView *ApplicationView;
    GlobalApplicationViewCollection->lpVtbl->GetViewForHwnd(GlobalApplicationViewCollection, Window, &ApplicationView);

    if(ApplicationView)
    {
        GlobalVirtualDesktopPinnedApps->lpVtbl->PinView(GlobalVirtualDesktopPinnedApps, ApplicationView);
        ApplicationView->lpVtbl->Release(ApplicationView);
    }
}

internal void __stdcall
Win32HideWindowCallback(void *Parameter, BOOLEAN TimerOrWaitFired)
{
    // NOTE(chuck): Hiding unpins the window.
    ShowWindow(GlobalWindow, SW_HIDE);
}

internal ULONG
VDN_AddRef(virtual_desktop_notification *This)
{
    OutputDebugString("AddRef\n");
    ULONG Result = InterlockedIncrement(&This->Count);
    return(Result);
}

internal ULONG
VDN_Release(virtual_desktop_notification *This)
{
    OutputDebugString("Release\n");
    ULONG Result = InterlockedDecrement(&This->Count);
    if(Result == 0)
    {
        free(This);
    }
    return(Result);
}

internal HRESULT
VDN_QueryInterface(virtual_desktop_notification *This,
                   REFIID InterfaceIdentifier, void **Object)
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

        if(IsEqualGUID(InterfaceIdentifier, &IID_IUnknown) ||
           IsEqualGUID(InterfaceIdentifier, &GUID_IVirtualDesktopNotification))
        {
            *Object = This;
            VDN_AddRef(This);
            Result = S_OK;
        }
        else
        {
            Result = E_NOINTERFACE;
        }
    }

    return(Result);
}

internal HRESULT
VDN_ViewVirtualDesktopChanged(virtual_desktop_notification *This,
                              IApplicationView *View)
{
    HRESULT Result = S_OK;
    OutputDebugString("ViewVirtualDesktopChanged\n");
    return(Result);
}

internal HRESULT
VDN_CurrentVirtualDesktopChanged(virtual_desktop_notification *This,
                                 IVirtualDesktop *OldDesktop, IVirtualDesktop *NewDesktop)
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

internal HRESULT
VDN_VirtualDesktopCreated(virtual_desktop_notification *This,
                          IVirtualDesktop *Desktop)
{
    HRESULT Result = S_OK;
    OutputDebugString("VirtualDesktopCreated\n");
    return(Result);
}

internal HRESULT
VDN_VirtualDesktopDestroyBegin(virtual_desktop_notification *This,
                               IVirtualDesktop *DesktopDestroyed, IVirtualDesktop *DesktopFallback)
{
    HRESULT Result = S_OK;
    OutputDebugString("VirtualDesktopDestroyBegin\n");
    return(Result);
}

internal HRESULT
VDN_VirtualDesktopDestroyFailed(virtual_desktop_notification *This,
                                IVirtualDesktop *Destroyed, IVirtualDesktop *Fallback)
{
    HRESULT Result = S_OK;
    OutputDebugString("VirtualDesktopDestroyFailed\n");
    return(Result);
}

internal HRESULT
VDN_VirtualDesktopDestroyed(virtual_desktop_notification *This,
                            IVirtualDesktop *Destroyed, IVirtualDesktop *Fallback)
{
    HRESULT Result = S_OK;
    OutputDebugString("VirtualDesktopDestroyed\n");
    return(Result);
}

struct virtual_desktop_notification_vtable
{
    HRESULT (WINAPI *QueryInterface)(virtual_desktop_notification *This, REFIID ID, void **Object);
    ULONG (WINAPI *AddRef)(virtual_desktop_notification *This);
    ULONG (WINAPI *Release)(virtual_desktop_notification *This);
    
    HRESULT (WINAPI *VirtualDesktopCreated)(virtual_desktop_notification *This, IVirtualDesktop *Desktop);
    HRESULT (WINAPI *VirtualDesktopDestroyBegin)(virtual_desktop_notification *This, IVirtualDesktop *DesktopDestroyed, IVirtualDesktop *DesktopFallback);
    HRESULT (WINAPI *VirtualDesktopDestroyFailed)(virtual_desktop_notification *This, IVirtualDesktop *DesktopDestroyed, IVirtualDesktop *DesktopFallback);
    HRESULT (WINAPI *VirtualDesktopDestroyed)(virtual_desktop_notification *This, IVirtualDesktop *DesktopDestroyed, IVirtualDesktop *DesktopFallback);
    HRESULT (WINAPI *ViewVirtualDesktopChanged)(virtual_desktop_notification *This, IApplicationView *View);
    HRESULT (WINAPI *CurrentVirtualDesktopChanged)(virtual_desktop_notification *This, IVirtualDesktop *OldDesktop, IVirtualDesktop *NewDesktop);
}
static VirtualDesktopNotificationImpl =
{
    &VDN_QueryInterface,
    &VDN_AddRef,
    &VDN_Release,
    
    &VDN_VirtualDesktopCreated,
    &VDN_VirtualDesktopDestroyBegin,
    &VDN_VirtualDesktopDestroyFailed,
    &VDN_VirtualDesktopDestroyed,
    &VDN_ViewVirtualDesktopChanged,
    &VDN_CurrentVirtualDesktopChanged,
};
    
internal b32
Win32InitVirtualDesktopNotifications()
{
    b32 Result = 0;

    if(S_OK == CoInitializeEx(0, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE))
    {
        OutputDebugStringA("OK COM\n");
        IServiceProvider *ServiceProvider;
        if(S_OK == CoCreateInstance(&GUID_ImmersiveShell, 0, CLSCTX_LOCAL_SERVER, &IID_IServiceProvider, &ServiceProvider))
        {
            OutputDebugStringA("OK Instance\n");

            IVirtualDesktopManager *VirtualDesktopManager;
            if(S_OK == IServiceProvider_QueryService(ServiceProvider,
                                                     &IID_IVirtualDesktopManager,
                                                     &IID_IVirtualDesktopManager,
                                                     &VirtualDesktopManager))
            {
                OutputDebugStringA("OK VirtualDesktopManager\n");
                // TODO(chuck): I have no idea what's going on.  Why are these two different?  I randomly changed one and it worked.
                if(S_OK == IServiceProvider_QueryService(ServiceProvider,
                                                         &GUID_VirtualDesktopAPI_Unknown,
                                                         &GUID_IVirtualDesktopManagerInternal,
                                                         &GlobalVirtualDesktopManagerInternal))
                {
                    OutputDebugStringA("OK VirtualDesktopManagerInternal\n");
                    IVirtualDesktopNotificationService *NotificationService;
                    // TODO(chuck): I have no idea what's going on.  Why are these two different?  I randomly changed one and it worked.
                    if(S_OK == IServiceProvider_QueryService(ServiceProvider,
                                                             &GUID_IVirtualNotificationService,
                                                             &GUID_IVirtualDesktopNotificationService,
                                                             &NotificationService))
                    {
                        OutputDebugStringA("OK VirtualDesktopNotificationService\n");
                        DWORD Cookie;
                        virtual_desktop_notification *VirtualDesktopNotification = malloc(sizeof(*VirtualDesktopNotification));
                        VirtualDesktopNotification->VTable = &VirtualDesktopNotificationImpl;
                        VirtualDesktopNotification->Count = 1;
                        if(S_OK == NotificationService->lpVtbl->Register(NotificationService, VirtualDesktopNotification, &Cookie))
                        {
                            OutputDebugStringA("OK NotificationService\n");
                            if(S_OK == IServiceProvider_QueryService(ServiceProvider,
                                                                     &GUID_IApplicationViewCollection,
                                                                     &GUID_IApplicationViewCollection,
                                                                     &GlobalApplicationViewCollection))
                            {
                                OutputDebugStringA("OK ApplicationViewCollection\n");
                                if(S_OK == IServiceProvider_QueryService(ServiceProvider,
                                                                         &GUID_VirtualDesktopPinnedApps,
                                                                         &GUID_IVirtualDesktopPinnedApps,
                                                                         &GlobalVirtualDesktopPinnedApps))
                                {
                                    OutputDebugStringA("OK VirtualDesktopPinnedApps\n");
                                    Result = 1;

                                    int VirtualDesktopCount = Win32GetVirtualDesktopCount();

                                    StringCbPrintf(GlobalTempBuffer, MAX_PATH, "Virtual desktop count: %d\n", VirtualDesktopCount);
                                    OutputDebugString(GlobalTempBuffer);
                                }
                                else
                                {
                                    OutputDebugString("FAIL VirtualDesktopPinnedApps\n");
                                }
                            }
                            else
                            {
                                OutputDebugString("FAIL ApplicationViewCollection\n");
                            }
                        }
                        else
                        {
                            OutputDebugString("FAIL GlobalVirtualDesktopNotification\n");
                        }
                    }
                    else
                    {
                        OutputDebugString("FAIL NotificationService\n");
                    }
                }
                else
                {
                    OutputDebugString("FAIL VirtualDesktopManager1030\n");
                }
            }
            else
            {
                OutputDebugString("FAIL VirtualDesktopManager\n");
            }
        }
        else
        {
            OutputDebugStringA("FAIL immersive shell\n");
        }
    }
    else
    {
        OutputDebugStringA("FAIL COM\n");
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
        
        WNDCLASSEX WindowClass = {0};
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
                WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
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
    }
    else
    {
        // OutputDebugString("Failed to initialize virtual desktop notifications.\n");
    }

    return(0);
}
