#include <iostream>
#include <Windows.h>
#include <pulse/pulseaudio.h>

class HotKeyRegistry {
public:
  bool numericRegistered;
  bool altNumericRegistered;
  HotKeyRegistry() {
    numericRegistered = false;
    altNumericRegistered = false;
  }
  ~HotKeyRegistry() {
    if (numericRegistered) unregisterNumeric();
    if (altNumericRegistered) unregisterAltNumeric();
  }
  static constexpr int idAltNumeric(char ch) { return ch - '0'; }
  static constexpr int idNumeric(char ch) { return ch - '0' + 10; }
  static constexpr int idLetter(char ch) { return ch - 'A' + 20; }
  static constexpr int vkChar(char ch) { return ch; } // ch: [0-9]|[A-Z]
  void registerAltNumeric() {
    altNumericRegistered = true;
    for (char ch = '0'; ch <= '9'; ch++)
      RegisterHotKey(NULL, idAltNumeric(ch), MOD_ALT | MOD_NOREPEAT, vkChar(ch));
  }
  void unregisterAltNumeric() {
    altNumericRegistered = false;
    for (char ch = '0'; ch <= '9'; ch++)
      UnregisterHotKey(NULL, idAltNumeric(ch));
  }
  void registerNumeric() {
    numericRegistered = true;
    for (char ch = '0'; ch <= '9'; ch++)
      RegisterHotKey(NULL, idNumeric(ch), MOD_ALT | MOD_NOREPEAT, vkChar(ch));
  }
  void unregisterNumeric() {
    numericRegistered = false;
    for (char ch = '0'; ch <= '9'; ch++)
      UnregisterHotKey(NULL, idNumeric(ch));
  }
};

HotKeyRegistry hotKeyRegistry;

int main(int argc, char* argv[]) {
  pa_mainloop* mainloop;
  pa_mainloop_api* api;
  pa_context* ctx;
  pa_context_flags_t flags = PA_CONTEXT_NOFLAGS;
  mainloop = pa_mainloop_new();
  api = pa_mainloop_get_api(mainloop);
  if (!(ctx = pa_context_new(api, "ctx")))
    { fprintf(stderr, "Context creating failed\n"); exit(1); }
  if (pa_context_connect(ctx, NULL, flags, NULL) < 0)
    { fprintf(stderr, "Context connecting failed\n"); exit(1); }

  hotKeyRegistry.registerAltNumeric();

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0) && msg.message == WM_HOTKEY) {
#define idAltNumeric HotKeyRegistry::idAltNumeric
#define idNumeric HotKeyRegistry::idNumeric
#define vkChar HotKeyRegistry::vkChar
    const auto& id = msg.wParam;
    using std::printf;
    if (idAltNumeric('0') <= id && id <= idAltNumeric('9')) {
      printf("Key ALT-%d\t", id - idAltNumeric('0'));
      switch(id) {
      case idAltNumeric('0'):
	printf("Quit\n");
	pa_mainloop_free(mainloop);
	PostQuitMessage(0);
	break;
      case idAltNumeric('1'):
	if (hotKeyRegistry.numericRegistered) {
	  hotKeyRegistry.unregisterNumeric();
	  printf("Numeric hot keys unregistered\n");
	} else {
	  hotKeyRegistry.registerNumeric();
	  // if (!hotKeyRegistry.numericRegistered) {
	  //   printf("Registering numeric hot keys failed\n");
	  // }
	  printf("Numeric keys registered\n");
	}
	break;
      default:
	printf("No operation\n");
      }
    } else if (idNumeric('0') <= id && id <= idNumeric('9')) {
      printf("Key %d\t", id - idNumeric('0'));
      printf("No operation\n");
    } else {
      printf("Hot Key ID %2x not identified\n", id);
    }
#undef idNumeric
#undef idAltNumeric
#undef vkChar
  }
}

