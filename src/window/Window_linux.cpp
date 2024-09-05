/*
 *  Linux backend for Windowing.
 */

#if ECS_LINUX || 1

#include "iro/logger.h"
#include "../math/util.h"
#include "../input/Keys.h"

#include "Window.h"
#include "Window_linux.h"

#include "../Engine.h"

static Logger logger = 
  Logger::create("ecs.window"_str, Logger::Verbosity::Notice);

X11Stuff x11;

template<typename... Args>
b8 error(Args... args)
{
  ERROR(args...);
  return false;
}

/* ----------------------------------------------------------------------------
 */
b8 Window::initializeBackend()
{
  Display* display = x11.display = XOpenDisplay(0);
  if (!display)
    return error("failed to open X11 display\n");

  int screen = x11.screen;

  X11Window root = x11.root = XRootWindow(display, screen);

  x11.context = XUniqueContext();
  x11.default_cursor = XCreateFontCursor(display, XC_left_ptr);

  // Create a blank cursor for when we'd like to hide it.
  XcursorImage* native = XcursorImageCreate(16,16);
  native->xhot = native->yhot = 0;
  for (s32 i = 0; i < 16*16; ++i)
    native->pixels[i] = 0;
  x11.hidden_cursor = XcursorImageLoadCursor(display, native);
  XcursorImageDestroy(native);

  // TODO(sushi) custom cursors
  return true;
}

static int window_event_masks = 
    ExposureMask      // caused when an invisible window becomes visible, 
                      // or when a hidden part of a window becomes visible
  | ButtonPressMask   // mouse button pressed
  | ButtonReleaseMask // mouse button released
  | KeyPressMask      
  | KeyReleaseMask
  | EnterWindowMask
  | LeaveWindowMask
  | PointerMotionMask // mouse movement event
  | StructureNotifyMask // window change events
  | FocusChangeMask;

/* ----------------------------------------------------------------------------
 */
b8 Window::init(str title)
{
  Display* display = x11.display;
  int screen = x11.screen;

  X11Window root, child;
  s32 root_x, root_y;
  s32 win_x, win_y;
  u32 mask;

  XQueryPointer(
    display,
    x11.root,
    &root,
    &child,
    &root_x, &root_y,
    &win_x, &win_y,
    &mask);

  // Find the currently focused monitor to put the window on.
  s32 n_monitors;
  XRRMonitorInfo* monitors = 
    XRRGetMonitors(
      display,
      root,
      1,
      &n_monitors);

  XRRMonitorInfo selection;
  for (s32 i = 0; i < n_monitors; ++i)
  {
    XRRMonitorInfo monitor = monitors[i];
    if (math::pointInRect(
          {root_x, root_y}, 
          {monitor.x, monitor.y},
          {monitor.width, monitor.height}))
    {
      selection = monitor;
      break;
    }
  }

  size.x = selection.width / 2;
  size.y = selection.height / 2;
  pos.x = selection.x + size.x / 2;
  pos.y = selection.y + size.y / 2;

  u32 black = XBlackPixel(display, screen);
  u32 white = XWhitePixel(display, screen);

  x11.root = XRootWindow(display, screen);
    
  this->title = title;

  X11Window window = 
    XCreateSimpleWindow(
      display,
      x11.root,
      pos.x, pos.y,
      size.x, size.y,
      0,
      white, black);

  handle = (void*)window;

  XSetStandardProperties(
    display, 
    window, 
    (const char*)title.bytes,
    0,0,0,0,0);

  int res = XSelectInput(display, window, window_event_masks);
  
  if (res == BadWindow)
    return error("XSelectInput failed with BadWindow\n");

  context = XCreateGC(display, window, 0, 0);
  XSetBackground(display, (GC)context, black);
  XSetForeground(display, (GC)context, white);

  XClearWindow(display, window);

  u32 bw, d;
  X11Window groot, gchild;
  XGetGeometry(
    display, 
    window, 
    &groot,
    &pos.x,&pos.y,
    (u32*)&size.x,(u32*)&size.y,
    &bw, &d);

  XMapRaised(display, window);

  return true;
} 

/* ----------------------------------------------------------------------------
 */
b8 Window::update(const Engine& engine)
{
  XEvent event;
  for (;;)
  {
    b8 got_event = 
      XCheckWindowEvent(
        x11.display, 
        (X11Window)handle, 
        window_event_masks,
        &event);

    if (!got_event)
      break;

    switch (event.type)
    {
    case FocusIn:
      focused = true;
      break;
    case FocusOut:
      focused = false;
      break;
    case KeyPress:
      {
        KeySym ks = XLookupKeysym(&event.xkey, 0);
        Key key = keysymToKey(ks);
        dispatchKeyPressed(engine, key);
      }
      break;
    case KeyRelease:
      {
        KeySym ks = XLookupKeysym(&event.xkey, 0);
        Key key = keysymToKey(ks);
        dispatchKeyReleased(engine, key);
      } 
      break;
    }
  }

  return true;
}

#endif

