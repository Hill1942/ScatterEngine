#ifndef SE_CONFIG_H
#define SE_CONFIG_H

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#define SE_WINDOWS
#define SE_WINDOWS_API
#define SE_WINDOWS_PLATFORM
#endif

#if defined(__APPLE__) && defined(__MACH__)
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR == 1  // iOS in Xcode simulator
#define SE_IOS_SIMULATOR
#define SE_IOS_SIMULATOR_API
#define SE_IOS_SIMULATOR_PLATFORM
#elif TARGET_OS_IPHONE == 1   // iOS on iPhone, iPad, etc
#define SE_IOS
#define SE_IOS_API
#define SE_IOS_PLATFORM
#elif TARGET_OS_MAC == 1   // OS X
#define SE_OSX
#define SE_OSX_API
#define SE_OSX_PLATFORM
#endif
#endif

#if defined(__linux__)
#define SE_LINUX
#define SE_LINUX_API
#define SE_LINUX_PLATFORM
#endif

#if defined(__ANDROID__)
#define SE_ANDROID
#define SE_ANDROID_API
#define SE_ANDROID_API_VERSION __ANDROID_API__
#define SE_ANDROID_PLATFORM
#endif

#endif