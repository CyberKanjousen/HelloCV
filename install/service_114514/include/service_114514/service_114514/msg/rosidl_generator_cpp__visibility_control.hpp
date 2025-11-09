// generated from rosidl_generator_cpp/resource/rosidl_generator_cpp__visibility_control.hpp.in
// generated code does not contain a copyright notice

#ifndef SERVICE_114514__MSG__ROSIDL_GENERATOR_CPP__VISIBILITY_CONTROL_HPP_
#define SERVICE_114514__MSG__ROSIDL_GENERATOR_CPP__VISIBILITY_CONTROL_HPP_

#ifdef __cplusplus
extern "C"
{
#endif

// This logic was borrowed (then namespaced) from the examples on the gcc wiki:
//     https://gcc.gnu.org/wiki/Visibility

#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define ROSIDL_GENERATOR_CPP_EXPORT_service_114514 __attribute__ ((dllexport))
    #define ROSIDL_GENERATOR_CPP_IMPORT_service_114514 __attribute__ ((dllimport))
  #else
    #define ROSIDL_GENERATOR_CPP_EXPORT_service_114514 __declspec(dllexport)
    #define ROSIDL_GENERATOR_CPP_IMPORT_service_114514 __declspec(dllimport)
  #endif
  #ifdef ROSIDL_GENERATOR_CPP_BUILDING_DLL_service_114514
    #define ROSIDL_GENERATOR_CPP_PUBLIC_service_114514 ROSIDL_GENERATOR_CPP_EXPORT_service_114514
  #else
    #define ROSIDL_GENERATOR_CPP_PUBLIC_service_114514 ROSIDL_GENERATOR_CPP_IMPORT_service_114514
  #endif
#else
  #define ROSIDL_GENERATOR_CPP_EXPORT_service_114514 __attribute__ ((visibility("default")))
  #define ROSIDL_GENERATOR_CPP_IMPORT_service_114514
  #if __GNUC__ >= 4
    #define ROSIDL_GENERATOR_CPP_PUBLIC_service_114514 __attribute__ ((visibility("default")))
  #else
    #define ROSIDL_GENERATOR_CPP_PUBLIC_service_114514
  #endif
#endif

#ifdef __cplusplus
}
#endif

#endif  // SERVICE_114514__MSG__ROSIDL_GENERATOR_CPP__VISIBILITY_CONTROL_HPP_
