#include "Precompiled.h"
#include "VulkanWindow.h"
#include <cassert>
#include <functional>
#include "VLog.h"

using namespace vulkan;

class VulkanWindow::impl
{
public:
	explicit impl(const std::string& window_name): window_name(window_name) 
	{
		init_window();
	}

	~impl()
	{
		destroy_window();
	}

	const int Max_Name_Length = 0;

	WNDCLASSEX win_class = {};

	template<int max_name_length>
	struct info_data
	{
		HINSTANCE connection;       // hInstance - Windows Instance
		wchar_t name[max_name_length];				// Name to put on the window/icon
		HWND window;                // hWnd - window handle
		int width = 700;
		int height = 500;

		constexpr auto get_max_name_length() const { return max_name_length; }
	};
	info_data<80> info;

	std::string window_name;

	static void run(impl *self)
	{
		/* Placeholder for samples that want to show dynamic content */
	}

	// MS-Windows event handling function:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		auto self = reinterpret_cast<impl*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		switch (uMsg) {
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		case WM_PAINT:
			run(self);
			return 0;
		default:
			break;
		}
		return (DefWindowProc(hWnd, uMsg, wParam, lParam));
	}

	void init_window()
	{	
		assert(info.width > 0);
		assert(info.height > 0);

		info.connection = GetModuleHandle(nullptr);
		wsprintf(info.name, L"Vulkan Atlas Window");

		// Initialize the window class structure:
		win_class.cbSize = sizeof(WNDCLASSEX);
		win_class.style = CS_HREDRAW | CS_VREDRAW;
		win_class.lpfnWndProc = &WndProc;
		win_class.cbClsExtra = 0;
		win_class.cbWndExtra = 0;
		win_class.hInstance = info.connection; // hInstance
		win_class.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		win_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
		win_class.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		win_class.lpszMenuName = nullptr;
		win_class.lpszClassName = info.name;
		win_class.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);
		// Register window class:
		if (!RegisterClassEx(&win_class)) 
		{
			// It didn't work, so try to give a useful error:
			printf("Unexpected error trying to start the application!\n");
			fflush(stdout);
			exit(1);
		}
		// Create window with the registered class:
		RECT wr = { 0, 0, info.width, info.height };
		AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
		info.window = CreateWindowEx(0,
			info.name,            // class name
			info.name,            // app name
			WS_OVERLAPPEDWINDOW | // window style
			WS_VISIBLE | WS_SYSMENU,
			100, 100,           // x/y coords
			wr.right - wr.left, // width
			wr.bottom - wr.top, // height
			nullptr,               // handle to parent
			nullptr,               // handle to menu
			info.connection,    // hInstance
			nullptr);              // no extra parameters

		if (!info.window) 
		{
			// It didn't work, so try to give a useful error:
			printf("Cannot create a window in which to draw!\n");
			fflush(stdout);
			exit(1);
		}

		SetWindowLongPtr(info.window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

		vulkan::log() << "Created a window\n";
	}

	void destroy_window()
	{
		DestroyWindow(info.window);
	}
};

VulkanWindow::VulkanWindow(const std::string& window_name)
{
	pimpl = std::make_unique<impl>(window_name);
}

VulkanWindow::~VulkanWindow()
{
}

HINSTANCE VulkanWindow::getInstanceHandle() const
{
	return pimpl->win_class.hInstance;
}

HWND VulkanWindow::setWindowHandle() const
{
	return pimpl->info.window;
}
